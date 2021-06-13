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
#include "engine_wrappers.h"

#include "bot.h"
#include "bot_cvars.h"
#include "ndebugoverlay.h"
#include "bot_squads.h"
#include "bot_synergy.h"
#include "in_buttons.h"
#include "bot_buttons.h"
#include "bot_globals.h"
#include "bot_profile.h"
#include "bot_getprop.h"
#include "bot_mtrand.h"
#include "bot_mods.h"
#include "bot_task.h"
#include "bot_schedule.h"
#include "bot_weapons.h"
#include "bot_waypoint.h"
#include "bot_waypoint_locations.h"
#include "bot_navigator.h"
#include "bot_perceptron.h"
#include "bot_waypoint_visibility.h"

void CBotSynergy::init(bool bVarInit)
{
	CBot::init(bVarInit); // call base first
	m_fFov = 110.0f; // Coop mod, give bot larger FOV (default is 75)
}

void CBotSynergy::spawnInit()
{
    CBot::spawnInit();

	if ( m_pWeapons ) // reset weapons
		m_pWeapons->clearWeapons();

    m_CurrentUtil = BOT_UTIL_MAX;
	m_pNearbyAmmo = NULL;
	m_pNearbyBattery = NULL;
	m_pNearbyCrate = NULL;
	m_pNearbyHealthKit = NULL;
	m_pNearbyWeapon = NULL;
}

void CBotSynergy::died(edict_t *pKiller, const char *pszWeapon)
{
	CBot::died(pKiller, pszWeapon);

	if(pKiller)
	{
		if(CBotGlobals::entityIsValid(pKiller))
		{
			m_pNavigator->belief(CBotGlobals::entityOrigin(pKiller), getEyePosition(), bot_beliefmulti.GetFloat(), distanceFrom(pKiller), BELIEF_DANGER);
		}
	}
}

void CBotSynergy::modThink()
{
    m_fIdealMoveSpeed = CClassInterface::getMaxSpeed(m_pEdict);
}

bool CBotSynergy::isEnemy(edict_t *pEdict, bool bCheckWeapons)
{
    if(m_pEdict == pEdict) // Not self
        return false;

    if(ENTINDEX(pEdict) <= CBotGlobals::maxClients()) // Coop mod, don't attack players
        return false;

    const char* szclassname = pEdict->GetClassName();

	// BUGBUG!! Maps can override NPC relationship with the ai_relationship entity, making this classname filter useless
    if(strncmp(szclassname, "npc_", 4) == 0) // Attack NPCs
    {
		if (((strcmp(szclassname, "npc_combinegunship") == 0) || (strcmp(szclassname, "npc_helicopter") == 0) || (strcmp(szclassname, "npc_strider") == 0))
			&& m_pWeapons->hasWeapon(SYN_WEAPON_RPG))
		{
			return true; // ignore gunships, helicopters and striders if I don't have an RPG.
		}

		if (strcmp(szclassname, "npc_metropolice") == 0 || strcmp(szclassname, "npc_combine_s") == 0 || strcmp(szclassname, "npc_manhack") == 0 ||
			strcmp(szclassname, "npc_zombie") == 0 || strcmp(szclassname, "npc_fastzombie") == 0 || strcmp(szclassname, "npc_poisonzombie") == 0 || strcmp(szclassname, "npc_zombine") == 0 ||
			strcmp(szclassname, "npc_antlionguard") == 0 || strcmp(szclassname, "npc_antlion") == 0 || strcmp(szclassname, "npc_headcrab") == 0 || strcmp(szclassname, "npc_headcrab_fast") == 0 ||
			strcmp(szclassname, "npc_headcrab_black") == 0 || strcmp(szclassname, "npc_hunter") == 0 || strcmp(szclassname, "npc_fastzombie_torso") == 0 || strcmp(szclassname, "npc_zombie_torso") == 0 ||
			strcmp(szclassname, "npc_barnacle") == 0)
		{
			return true;
		}
    }

    return false;
}

bool CBotSynergy::setVisible ( edict_t *pEntity, bool bVisible )
{
	bool bValid = CBot::setVisible(pEntity, bVisible);

	static float fDist = distanceFrom(pEntity);
	const char* szclassname = pEntity->GetClassName();
	CBotWeapon* pWeapon = NULL;

	// Is valid and NOT invisible
	if (bValid && bVisible && !(CClassInterface::getEffects(pEntity) & EF_NODRAW))
	{
		if(strcmp(szclassname, "item_ammo_crate") == 0 && (!m_pNearbyCrate.get() || fDist < distanceFrom(m_pNearbyCrate.get())))
		{
			m_pNearbyCrate = pEntity;
		}
		else if(strncmp(szclassname, "item_ammo", 9) == 0 && (!m_pNearbyAmmo.get() || fDist < distanceFrom(m_pNearbyAmmo.get())))
		{
			m_pNearbyAmmo = pEntity;
			if(strncmp(szclassname, "item_ammo_crate", 15))
			{
				m_pNearbyAmmo = NULL; // Invalidate if this entity is an ammo crate
			}
		}
		else if(strncmp(szclassname, "item_healthkit", 14) == 0 && (!m_pNearbyHealthKit.get() || fDist < distanceFrom(m_pNearbyHealthKit.get())))
		{
			m_pNearbyHealthKit = pEntity;
			if(getHealthPercent() <= 0.90f)
				updateCondition(CONDITION_CHANGED);
		}
		else if(strncmp(szclassname, "item_battery", 12) == 0 && (!m_pNearbyBattery.get() || fDist < distanceFrom(m_pNearbyBattery.get())))
		{
			m_pNearbyHealthKit = pEntity;
			if(getArmorPercent() < 1.0f)
				updateCondition(CONDITION_CHANGED);

		}
		else if(strncmp(szclassname, "weapon_", 7) == 0 && (!m_pNearbyWeapon.get() || fDist < distanceFrom(m_pNearbyWeapon.get())))
		{
			pWeapon = m_pWeapons->getWeapon(CWeapons::getWeapon(szclassname));
			if(pWeapon && pWeapon->hasWeapon())
			{
				m_pNearbyWeapon = NULL; // bot already has this weapon
			}
			else
			{
				edict_t* pOwner = CClassInterface::getOwner(pEntity);
				if(pOwner == NULL) // Don't pick if owned by someone
				{
					m_pNearbyWeapon = pEntity;
					updateCondition(CONDITION_CHANGED);
				}
			}
		}		
	}
	else
	{
		if(pEntity == m_pNearbyAmmo.get_old())
			m_pNearbyAmmo = NULL;
		else if(pEntity == m_pNearbyCrate.get_old())
			m_pNearbyCrate = NULL;
		else if(pEntity == m_pNearbyHealthKit.get_old())
			m_pNearbyHealthKit = NULL;
		else if(pEntity == m_pNearbyBattery.get_old())
			m_pNearbyBattery = NULL;
		else if(pEntity == m_pNearbyWeapon.get_old())
			m_pNearbyWeapon = NULL;
	}

	return bValid;
}

void CBotSynergy::getTasks (unsigned int iIgnore)
{
    static CBotUtilities utils;
    static CBotUtility* next;
    static bool bCheckCurrent;

	if (!hasSomeConditions(CONDITION_CHANGED) && !m_pSchedules->isEmpty())
		return;

    removeCondition(CONDITION_CHANGED);
    bCheckCurrent = true; // important for checking current schedule

	// Utilities
	ADD_UTILITY(BOT_UTIL_PICKUP_WEAPON, m_pNearbyWeapon.get() != NULL, 0.75f) // New weapons are interesting, high priority
	ADD_UTILITY(BOT_UTIL_GETHEALTHKIT, m_pNearbyHealthKit.get() != NULL, 1.0f - getHealthPercent()); // Pick up health kits
	ADD_UTILITY(BOT_UTIL_HL2DM_FIND_ARMOR, m_pNearbyBattery.get() != NULL, 1.0f - getArmorPercent()); // Pick up armor batteries
	ADD_UTILITY(BOT_UTIL_ROAM, true, 0.01f); // Roam around

	utils.execute();

	while ((next = utils.nextBest()) != NULL)
	{
		if (!m_pSchedules->isEmpty() && bCheckCurrent)
		{
			if (m_CurrentUtil != next->getId())
				m_pSchedules->freeMemory();
			else
				break;
		}

		bCheckCurrent = false;

		if (executeAction(next->getId()))
		{
			m_CurrentUtil = next->getId();

			if (m_fUtilTimes[next->getId()] < engine->Time())
				m_fUtilTimes[next->getId()] = engine->Time() + randomFloat(0.1f, 2.0f); // saves problems with consistent failing

			if (CClients::clientsDebugging(BOT_DEBUG_UTIL))
			{
				CClients::clientDebugMsg(BOT_DEBUG_UTIL, g_szUtils[next->getId()], this);
			}
			break;
		}
	}

	utils.freeMemory();
}

bool CBotSynergy::executeAction(eBotAction iAction)
{
    switch (iAction)
    {
	case BOT_UTIL_PICKUP_WEAPON:
		m_pSchedules->addFront(new CBotPickupSched(m_pNearbyWeapon.get()));
		return true;
	break;
	case BOT_UTIL_GETHEALTHKIT:
		m_pSchedules->addFront(new CBotPickupSched(m_pNearbyHealthKit.get()));
		return true;
	break;
    case BOT_UTIL_ROAM:
    {
		// roam
		CWaypoint* pWaypoint = NULL;
		CWaypoint* pRoute = NULL;
		CBotSchedule* pSched = new CBotSchedule();

		pSched->setID(SCHED_GOTO_ORIGIN);

        pWaypoint = CWaypoints::randomWaypointGoal(-1);

		if (pWaypoint)
		{
			pRoute = CWaypoints::randomRouteWaypoint(this, getOrigin(), pWaypoint->getOrigin(), 0, 0);
			if ((m_fUseRouteTime < engine->Time()))
			{
				if (pRoute)
				{
					int iRoute = CWaypoints::getWaypointIndex(pRoute);
					pSched->addTask(new CFindPathTask(iRoute, LOOK_WAYPOINT));
					pSched->addTask(new CMoveToTask(pRoute->getOrigin()));
					m_pSchedules->add(pSched);
					m_fUseRouteTime = engine->Time() + 30.0f;
				}
			}

			if (pRoute == NULL)
			{
				int iWaypoint = CWaypoints::getWaypointIndex(pWaypoint);
				pSched->addTask(new CFindPathTask(iWaypoint, LOOK_WAYPOINT));
				pSched->addTask(new CMoveToTask(pWaypoint->getOrigin()));
				m_pSchedules->add(pSched);
			}

			return true;
		}

		break;
    }
    }

    return false;
}