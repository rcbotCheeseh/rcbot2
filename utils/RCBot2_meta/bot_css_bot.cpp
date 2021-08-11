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
#include "bot_css_bot.h"
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
#include "bot_plugin_meta.h"
#include "bot_waypoint_visibility.h"

#include "logging.h"

void CCSSBot::init(bool bVarInit)
{
	CBot::init();// require this

	// initialize stuff for counter-strike source bot
}

void CCSSBot::setup()
{
	CBot::setup(); // require this

	// setup data structures for counter-strike source bot
	// this only gets called ONCE (when bot joins the game)

	engine->SetFakeClientConVarValue(m_pEdict, "cl_team", "default");
	engine->SetFakeClientConVarValue(m_pEdict, "cl_autohelp", "0");
}

bool CCSSBot::isAlive()
{
	if (!CBot::isAlive())
		return false;
	return (getOrigin() != Vector(0, 0, 0));
}

bool CCSSBot::isEnemy(edict_t* pEdict, bool bCheckWeapons)
{
	if (ENTINDEX(pEdict) > CBotGlobals::maxClients())
		return false;
	if (pEdict->IsFree())
		return false;

	if (!CBotGlobals::isNetworkable(pEdict))
		return false;

	IPlayerInfo* p = playerinfomanager->GetPlayerInfo(pEdict);

	if (p == NULL)
		return false;
	if (m_pEdict == pEdict)
		return false;
	if (!CBotGlobals::entityIsAlive(pEdict))
		return false;

	return (p->GetTeamIndex() != getTeam());
}

bool CCSSBot::startGame()
{
	const int team = m_pPlayerInfo->GetTeamIndex();

	if (team <= CCounterStrikeSourceMod::CS_TEAM_SPECTATOR)
	{
		selectTeam();
		selectModel();
	}

	return true;
}

void CCSSBot::died(edict_t* pKiller, const char* pszWeapon)
{
	spawnInit();
}

void CCSSBot::spawnInit()
{
	CBot::spawnInit();

	if (m_pWeapons) // reset weapons
		m_pWeapons->clearWeapons();

	m_bDidBuy = false;
	m_fCheckStuckTime = engine->Time() + 6.0;
	updateCondition(CONDITION_CHANGED); // Re-execute the utility system
	logger->Log(LogLevel::TRACE, "CSSBot::spawnInit() --> %s", m_pPlayerInfo->GetName());
}

void CCSSBot::selectTeam() const
{
	const char* cmd = "jointeam 0";
	helpers->ClientCommand(m_pEdict, cmd);
}

void CCSSBot::selectModel() const
{
	const char* cmd = "joinclass 0";
	helpers->ClientCommand(m_pEdict, cmd);
}

/**
 * Executes the 'say' command
 *
 * @param message		The message the bot will say
 * @return				No return
 **/
void CCSSBot::say(const char* message)
{
	char buffer[256];
	sprintf(buffer, "say \"%s\"", message);
	helpers->ClientCommand(m_pEdict, buffer);
}

/**
 * Executes the 'say_team' command
 *
 * @param message		The message the bot will say
 * @return				No return
 **/
void CCSSBot::sayteam(const char* message)
{
	char buffer[256];
	sprintf(buffer, "say_team \"%s\"", message);
	helpers->ClientCommand(m_pEdict, buffer);
}

/**
 * Ammo: primammo, secammo
 * Armor: vest, vesthelm
 * Misc: defuser, nvgs
 * Pistols: usp, glock, p228, fiveseven, elite, deagle
 * Shotguns: m3, xm1014
 * SMGs: tmp, mac10, mp5navy, ump45, p90
 * Rifles: famas, galil, ak47, m4a1, aug, sg552
 * Snipers: scout, awp, sg550, g3sg1
 * Machine Guns: m249
 **/

 /**
  * Executes the buy console command.
  *
  * @param item		The item to buy
  * @return			No return
  **/
void CCSSBot::buy(const char* item)
{
	char buffer[32];
	sprintf(buffer, "buy %s", item);
	helpers->ClientCommand(m_pEdict, buffer);
}

/**
 * Executes the buy logic
 **/
void CCSSBot::executeBuy()
{
	const int money = CClassInterface::getCSPlayerMoney(m_pEdict);
	const int team = getTeam();
	int cost = 0; // Computed buy cost
	int remaining = 0; // Remaining money (money - cost)
	int tobuy = 0; // Things the bot should buy
	CBotWeapon* primary = m_pWeapons->getCurrentWeaponInSlot(1);
	CBotWeapon* secondary = m_pWeapons->getCurrentWeaponInSlot(2);

	if (money <= rcbot_css_economy_eco_limit.GetInt())
	{
		m_bDidBuy = true;
		updateCondition(CONDITION_CHANGED); // Buy done, update conditions
		return; // eco
	}

	/**
	 * Armor costs:
	 * 1000 -> vest + helm
	 * 650 -> vest
	 * 350 -> helm upgrade (armor = 100)
	 * 1000 -> helm upgrade (armor <= 99)
	 * 650 -> repair (any armor % with helm)
	 **/
	bool hashelmet = CClassInterface::CSPlayerHasHelmet(m_pEdict);
	if (CClassInterface::getCSPlayerArmor(m_pEdict) <= 70 || !hashelmet)
	{
		if (!hashelmet)
		{
			cost += 1000;
		}
		else
		{
			cost += 650;
		}
	}

	if (team == CCounterStrikeSourceMod::CS_TEAM_COUNTERTERRORIST && !CClassInterface::CSPlayerHasDefuser(m_pEdict)) // To-do: filter for bomb maps
	{
		cost += 200;
	}

	remaining = money - cost;
	if (primary)
	{
		// To-do: Upgrade primary logic
	}
	else
	{ // To-do: Buy selection logic
		if (remaining >= 1500)
		{
			buy("mp5navy");
		}
	}

	logger->Log(LogLevel::TRACE, "CSS --- Running buy logic for bot \"%s\"", m_pPlayerInfo->GetName());
	logger->Log(LogLevel::TRACE, "Team = %i --- Money = %i --- Cost = %i --- Buy Bits = %i", team, money, cost, tobuy);
	logger->Log(LogLevel::TRACE, "Primary Weapon = %s", primary ? primary->getWeaponInfo()->getWeaponName() : "No Primary");
	logger->Log(LogLevel::TRACE, "Secondary Weapon = %s", secondary ? secondary->getWeaponInfo()->getWeaponName() : "No Secondary");
	//processBuyList(tobuy);
	m_bDidBuy = true;
	updateCondition(CONDITION_CHANGED); // Buy done, update conditions
}

void CCSSBot::handleWeapons()
{
	//
	// Handle attacking at this point
	//
	if (m_pEnemy && !hasSomeConditions(CONDITION_ENEMY_DEAD) &&
		hasSomeConditions(CONDITION_SEE_CUR_ENEMY) && wantToShoot() &&
		isVisible(m_pEnemy) && isEnemy(m_pEnemy))
	{
		CBotWeapon* pWeapon;

		pWeapon = getBestWeapon(m_pEnemy);

		if (m_bWantToChangeWeapon && (pWeapon != NULL) && (pWeapon != getCurrentWeapon()) && pWeapon->getWeaponIndex())
		{
			selectWeapon(pWeapon->getWeaponIndex());
		}

		setLookAtTask(LOOK_ENEMY);

		if (!handleAttack(pWeapon, m_pEnemy))
		{
			m_pEnemy = NULL;
			m_pOldEnemy = NULL;
			wantToShoot(false);
		}
	}
}

bool CCSSBot::handleAttack(CBotWeapon* pWeapon, edict_t* pEnemy)
{
	if (pWeapon)
	{
		clearFailedWeaponSelect();

		if (pWeapon->isMelee())
			setMoveTo(CBotGlobals::entityOrigin(pEnemy));

		m_pButtons->holdButton(IN_ATTACK, 0.0f, 0.15f, 0.0f);
	}
	else
		primaryAttack();
	{
		m_pButtons->holdButton(IN_ATTACK, 0.0f, 0.15f, 0.0f);
	}

	return true;
}

void CCSSBot::getTasks(unsigned int iIgnore)
{
	static CBotUtilities utils;
	static CBotUtility* next;
	static bool bCheckCurrent;

	if (!hasSomeConditions(CONDITION_CHANGED) && !m_pSchedules->isEmpty())
		return;

	removeCondition(CONDITION_CHANGED);
	bCheckCurrent = true; // important for checking current schedule

	// Utilities
	ADD_UTILITY(BOT_UTIL_BUY, !m_bDidBuy, 1.0f); // Buy weapons
	ADD_UTILITY(BOT_UTIL_ROAM, true, 0.0001f); // Roam around

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

bool CCSSBot::executeAction(eBotAction iAction)
{
	switch (iAction)
	{
	case BOT_UTIL_BUY:
	{
		CBotSchedule* pSched = new CBotSchedule();
		pSched->setID(SCHED_BUY);
		pSched->addTask(new CCSSPerformBuyTask());
		m_pSchedules->add(pSched);
		break;
	}
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
			if ((m_fUseRouteTime <= engine->Time()))
			{
				if (pRoute)
				{
					int iRoute = CWaypoints::getWaypointIndex(pRoute); // Route waypoint
					pSched->addTask(new CFindPathTask(iRoute, LOOK_WAYPOINT));
					pSched->addTask(new CMoveToTask(pRoute->getOrigin()));
					m_pSchedules->add(pSched);
					m_fUseRouteTime = engine->Time() + 30.0f;
				}
			}

			int iWaypoint = CWaypoints::getWaypointIndex(pWaypoint);
			pSched->addTask(new CFindPathTask(iWaypoint, LOOK_WAYPOINT));
			pSched->addTask(new CMoveToTask(pWaypoint->getOrigin()));
			m_pSchedules->add(pSched);

			return true;
		}
		break;
	}
}

return false;
}