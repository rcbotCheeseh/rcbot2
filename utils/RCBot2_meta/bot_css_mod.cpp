/*
 *    This file is part of RCBot.
 *
 *    RCBot by Paul Murphy adapted from Botman's HPB Bot 2 template.
 *
 *    RCBot is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    RCBot is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RCBot; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */
#include "server_class.h"

#include "bot.h"

#include "in_buttons.h"

#include "bot_mods.h"
#include "bot_globals.h"
#include "bot_weapons.h"
#include "bot_configfile.h"
#include "bot_getprop.h"
#include "bot_css_bot.h"
#include "bot_navigator.h"
#include "bot_waypoint.h"
#include "bot_waypoint_locations.h"
#include "bot_perceptron.h"

#include "logging.h"

// For debug messages
const char *szMapTypes[CS_MAP_MAX+1] =
{
    "DEATHMATCH",
    "BOMB DEFUSAL",
    "HOSTAGE RESCUE",
    "MAP TYPE MAX"
};

eCSSMapType CCounterStrikeSourceMod::m_MapType = CS_MAP_DEATHMATCH;
float CCounterStrikeSourceMod::m_fRoundStartTime = 0.0f;
float CCounterStrikeSourceMod::m_fBombPlantedTime = 0.0f;
bool CCounterStrikeSourceMod::m_bIsBombPlanted = false;
bool CCounterStrikeSourceMod::m_bBombWasFound = false;
CBaseHandle CCounterStrikeSourceMod::m_hBomb = NULL;
std::vector<CBaseHandle> CCounterStrikeSourceMod::m_hHostages;

void CCounterStrikeSourceMod::initMod()
{
    CWeapons::loadWeapons(m_szWeaponListName == NULL ? "CSS" : m_szWeaponListName, CSSWeaps); // Load weapon list
    logger->Log(LogLevel::TRACE, "CCounterStrikeSourceMod::initMod()");
}

void CCounterStrikeSourceMod::mapInit()
{
	const string_t mapname = gpGlobals->mapname;
	const char *szmapname = mapname.ToCStr();

    if(strncmp(szmapname, "de_", 3) == 0)
        m_MapType = CS_MAP_BOMBDEFUSAL;
    else if(strncmp(szmapname, "cs_", 3) == 0)
        m_MapType = CS_MAP_HOSTAGERESCUE;
    else
        m_MapType = CS_MAP_DEATHMATCH;

    logger->Log(LogLevel::TRACE, "CCounterStrikeSourceMod::mapInit()\nMap Type: %s", szMapTypes[m_MapType]);
}

bool CCounterStrikeSourceMod::checkWaypointForTeam(CWaypoint *pWpt, int iTeam)
{
    return (!pWpt->hasFlag(CWaypointTypes::W_FL_NOCOUNTERTR)||iTeam!=CS_TEAM_COUNTERTERRORIST)&&(!pWpt->hasFlag(CWaypointTypes::W_FL_NOTERRORIST)||iTeam!=CS_TEAM_TERRORIST);
}

/**
 * Checks if the given bot is a bomb carrier (has C4)
 * 
 * @param pBot      The bot to check
 * @return          TRUE if the bot is a bomb carrier
 **/
bool CCounterStrikeSourceMod::isBombCarrier(CBot *pBot)
{
    return pBot->getWeapons()->hasWeapon(CS_WEAPON_C4);
}

/**
 * Checks if the C4 is dropped on the ground
 * 
 * @return      TRUE if dropped
 **/
bool CCounterStrikeSourceMod::isBombDropped()
{
    edict_t *pBomb = getBomb();

    if(m_bIsBombPlanted)
        return false;

    if(pBomb)
    {
        return CClassInterface::getWeaponState(pBomb) == WEAPON_NOT_CARRIED;
    }

    return false;
}

/**
 * Checks if the C4 was defused
 * 
 * @return      TRUE if defused
 **/
bool CCounterStrikeSourceMod::isBombDefused()
{
    return !CClassInterface::isCSBombTicking(INDEXENT(m_hBomb.GetEntryIndex()));
}

/**
 * Checks if the given bot can hear the planted c4 ticking
 * 
 * @param pBot      The bot to check
 * @return          TRUE if the bot can hear
 **/
bool CCounterStrikeSourceMod::canHearPlantedBomb(CBot *pBot)
{
    if(!isBombPlanted())
        return false;

    edict_t *pBomb = getBomb();

    if(pBomb)
    {
        return pBot->distanceFrom(pBomb) <= 2048.0f;
    }

    return false;
}

/**
 * Checks if the given bot is scoped
 * 
 * @param pBot      The bot to check
 * @return          TRUE if the bot is scoped
 **/
bool CCounterStrikeSourceMod::isScoped(CBot *pBot)
{
    const int fov = CClassInterface::getPlayerFOV(pBot->getEdict());
    return fov != 0 && fov != 90; // For bots, FOVs are 0 or 90 when not scoped.
}

/**
 * Called when a new round starts
 **/
void CCounterStrikeSourceMod::onRoundStart()
{
    // Empty for now, reset round based logic
    logger->Log(LogLevel::TRACE, "CCounterStrikeSourceMod::OnRoundStart()");
    m_bIsBombPlanted = false;
    setBombFound(false);
    m_hBomb.Term();
	for(short int i = 0; i < MAX_PLAYERS; i++)
	{
        CBot *pBot = CBots::get(i);
        CCSSBot *pCSBot = (CCSSBot*)pBot;

		if(pCSBot && pCSBot->inUse())
        {
            pCSBot->onRoundStart();
        }
	}
	
    if(CCounterStrikeSourceMod::isMapType(CS_MAP_HOSTAGERESCUE))
    {
        updateHostages();
    }
}

/**
 * Called when the freeze time ends. Note: This is always called even if freeze time is disabled.
 **/
void CCounterStrikeSourceMod::onFreezeTimeEnd()
{
    logger->Log(LogLevel::TRACE, "CCounterStrikeSourceMod::OnFreezeTimeEnd()");
    m_fRoundStartTime = engine->Time();

    edict_t *pC4 = CClassInterface::FindEntityByClassnameNearest(Vector(0.0, 0.0, 0.0), "weapon_c4", 32000.0f);
    if(pC4)
    {
        m_hBomb.Init(engine->IndexOfEdict(pC4), pC4->m_NetworkSerialNumber);
    }

	for(short int i = 0; i < MAX_PLAYERS; i++)
	{
        CBot *pBot = CBots::get(i);

		if(pBot && pBot->inUse())
        {
            pBot->select_CWeapon(CWeapons::getWeapon(CS_WEAPON_KNIFE));
        }
	}
}

/**
 * Called when the bomb is planted
 **/
void CCounterStrikeSourceMod::onBombPlanted()
{
    logger->Log(LogLevel::TRACE, "CCounterStrikeSourceMod::OnBombPlanted()");
    m_bIsBombPlanted = true;
    m_fBombPlantedTime = engine->Time();
    m_hBomb.Term();

    edict_t *pPlantedC4 = CClassInterface::FindEntityByNetClass(gpGlobals->maxClients+1, "CPlantedC4");
    if(pPlantedC4)
    {
        m_hBomb.Init(engine->IndexOfEdict(pPlantedC4), pPlantedC4->m_NetworkSerialNumber);
    }

	for(short int i = 0; i < MAX_PLAYERS; i++)
	{
        CBot *pBot = CBots::get(i);

		if(pBot && pBot->inUse())
        {
            if(pBot->distanceFrom(getBomb()) >= 512.0f)
            {
                pBot->updateCondition(CONDITION_CHANGED);
            }
        }
	}
}

/**
 * Hostage entities needs to be updated on round start since killed hostages gets their entity deleted.
 **/
void CCounterStrikeSourceMod::updateHostages()
{
    edict_t *current;
    CBaseHandle bh;
    m_hHostages.clear();

    for(int i = gpGlobals->maxClients + 1; i < gpGlobals->maxEntities; i++)
    {
        bh.Term();
		current = engine->PEntityOfEntIndex(i);
		if (current == NULL)
		{
			continue;
		}

		IServerNetworkable *network = current->GetNetworkable();
		if (network == NULL)
		{
			continue;
		}

        //const char *classname = current->GetClassName();
		ServerClass *sClass = network->GetServerClass();
		const char *sname = sClass->GetName();

        if(strcmp(sname, "CHostage") == 0)
        {
            bh.Init(i, current->m_NetworkSerialNumber);
            m_hHostages.push_back(bh);
        }
    }

    logger->Log(LogLevel::DEBUG, "Hostage Vector Size %i", m_hHostages.size());

    for(CBaseHandle i : m_hHostages)
    {
        logger->Log(LogLevel::DEBUG, "Stored Hostage: %i - %i", i.GetEntryIndex(), i.GetSerialNumber());
    }
}

/**
 * Checks if there are hostages that can be rescued
 **/
bool CCounterStrikeSourceMod::canRescueHostages()
{
    if(m_hHostages.size() == 0)
        return false;

    edict_t *pHostage;

    for(CBaseHandle i : m_hHostages)
    {
        pHostage = INDEXENT(i.GetEntryIndex());

        if(CBotGlobals::entityIsValid(pHostage) && !CClassInterface::isCSHostageRescued(pHostage) && CClassInterface::getCSHostageLeader(pHostage) == NULL)
        {
            return true;
        }
    }

    return false;
}

edict_t *CCounterStrikeSourceMod::getRandomHostage()
{
    std::vector<CBaseHandle> temp;
    edict_t *pEdict;

    // Build a new vector with hostages that are valid to be rescued
    for(CBaseHandle i : temp)
    {
        pEdict = INDEXENT(i.GetEntryIndex());

        if(CBotGlobals::entityIsValid(pEdict) && !CClassInterface::isCSHostageRescued(pEdict) && CClassInterface::getCSHostageLeader(pEdict) == NULL && CClassInterface::getCSHostageHealth(pEdict) > 0)
        {
            temp.push_back(i);
        }
    }

    if(temp.size() > 0)
    {   
        return INDEXENT(temp.at(randomInt(0, temp.size() - 1)).GetEntryIndex());
    }
    else
    {
        return NULL;
    }
}