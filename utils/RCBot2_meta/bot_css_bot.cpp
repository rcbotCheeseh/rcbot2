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

	engine->SetFakeClientConVarValue(m_pEdict,"cl_team","default");
	engine->SetFakeClientConVarValue(m_pEdict,"cl_autohelp","0");
}

void CCSSBot::updateConditions()
{
	CBot::updateConditions();

	if(m_pEnemy.get() != NULL)
	{
		if(engine->IndexOfEdict(m_pEnemy.get()) <= gpGlobals->maxClients)
		{
			// CSS Hack: Dead players are always "Alive" with 1 health.
			if(CClassInterface::getPlayerLifeState(m_pEnemy.get()) == LIFE_DEAD || CClassInterface::getPlayerLifeState(m_pEnemy.get()) == LIFE_DYING)
			{
				updateCondition(CONDITION_ENEMY_DEAD);
				m_pEnemy = NULL;
			}
		}
	}
}

bool CCSSBot::isAlive()
{
	if (!CBot::isAlive())
		return false;

	if(CClassInterface::getPlayerLifeState(getEdict()) == LIFE_DEAD || CClassInterface::getPlayerLifeState(getEdict()) == LIFE_DYING)
		return false;

	return getOrigin() != Vector(0,0,0);
}

bool CCSSBot::isEnemy(edict_t *pEdict,bool bCheckWeapons)
{
	if(rcbot_notarget.GetBool())
		return false;

	if(ENTINDEX(pEdict) > CBotGlobals::maxClients())
		return false;

	if(pEdict->IsFree())
		return false;

	if(!CBotGlobals::isNetworkable(pEdict))
		return false;
 
	IPlayerInfo *p = playerinfomanager->GetPlayerInfo(pEdict);

	if(p == NULL)
		return false;

	if(m_pEdict == pEdict)
		return false;

	if(!CBotGlobals::entityIsAlive(pEdict))
		return false;

	if(CClassInterface::getPlayerLifeState(pEdict) == LIFE_DEAD || CClassInterface::getPlayerLifeState(pEdict) == LIFE_DYING)
		return false;

	return p->GetTeamIndex() != getTeam();
}

bool CCSSBot::startGame()
{
	const int team = m_pPlayerInfo->GetTeamIndex();

	if(team <= CS_TEAM_SPECTATOR)
	{
		selectTeam();
		selectModel();
	}

	return true;
}

void CCSSBot::died(edict_t *pKiller, const char *pszWeapon)
{
	spawnInit();
}

void CCSSBot::spawnInit()
{
	CBot::spawnInit();

	m_bDidBuy = false;
	m_bInCombat = false;
	m_fCombatTime = 0.0f;
	m_pCurrentWeapon = NULL;
	m_fNextAttackTime = engine->Time();
	m_fCheckStuckTime = engine->Time() + 6.0f;
	m_fNextThinkSlow = engine->Time() + 1.0f;
	updateCondition(CONDITION_CHANGED); // Re-execute the utility system
	logger->Log(LogLevel::TRACE, "CSSBot::spawnInit() --> %s", m_pPlayerInfo->GetName());
}

void CCSSBot::listenForPlayers()
{
	edict_t *pListenNearest = NULL;
	float fMaxFactor = 0;
	Vector vVelocity;
	bool bIsNearestAttacking = false;

	if(m_bListenPositionValid && m_fListenTime > engine->Time()) // already listening to something ?
	{
		setLookAtTask(LOOK_NOISE);
		return;
	}

	m_bListenPositionValid = false;

	for(short int i = 1; i <= gpGlobals->maxClients; i ++)
	{
		edict_t* pPlayer = INDEXENT(i);

		if(pPlayer == m_pEdict)
			continue; // don't listen to self

		if(pPlayer->IsFree())
			continue;

		CClient* pClient = CClients::get(pPlayer);

		if(!pClient->isUsed())
			continue;

		IPlayerInfo* p = playerinfomanager->GetPlayerInfo(pPlayer);

		// 05/07/09 fix crash bug
		if(!p || !p->IsConnected() || p->IsDead() || p->IsObserver() || !p->IsPlayer())
			continue;

		if(CClassInterface::getPlayerLifeState(pPlayer) == LIFE_DEAD || CClassInterface::getPlayerLifeState(pPlayer) == LIFE_DYING)
			continue;

		// Ignore teammates
		if(p->GetTeamIndex() == getTeam())
			continue;

		const float fDist = distanceFrom(pPlayer);

		if(fDist > rcbot_listen_dist.GetFloat())
			continue;
		
		float fFactor = 0.0f;

		const CBotCmd cmd = p->GetLastUserCommand();

		if(cmd.buttons & IN_ATTACK)
		{
			if(wantToListenToPlayerAttack(pPlayer))
				fFactor += 1000.0f;
		}
		
		// can't see this player and I'm on my own
		if(wantToListenToPlayerFootsteps(pPlayer) && !isVisible(pPlayer) && ( m_bStatsCanUse && m_StatsCanUse.stats.m_iTeamMatesVisible==0))
		{
			CClassInterface::getVelocity(pPlayer,&vVelocity);

			const float fVelocity = vVelocity.Length();

			if(fVelocity > rcbot_footstep_speed.GetFloat())
				fFactor += vVelocity.Length();
		}

		if(fFactor == 0.0f)
			continue;

		// add inverted distance to the factor (i.e. closer = better)
		fFactor += rcbot_listen_dist.GetFloat() - fDist;

		if(fFactor > fMaxFactor)
		{
			fMaxFactor = fFactor;
			pListenNearest = pPlayer;
			bIsNearestAttacking = cmd.buttons & IN_ATTACK;
		}
	}

	if(pListenNearest != NULL)
	{
		listenToPlayer(pListenNearest,false,bIsNearestAttacking);
	}
}

void CCSSBot::selectTeam()
{
	const char* cmd = "jointeam 0";
	helpers->ClientCommand(m_pEdict,cmd);
}

void CCSSBot::selectModel()
{
	const char* cmd = "joinclass 0";
	helpers->ClientCommand(m_pEdict,cmd);
}

/**
 * Executes the 'say' command
 * 
 * @param message		The message the bot will say
 * @return				No return
 **/
void CCSSBot::say(const char *message)
{
	char buffer[256];
	sprintf(buffer, "say \"%s\"", message);
	helpers->ClientCommand(m_pEdict,buffer);
}

/**
 * Executes the 'say_team' command
 * 
 * @param message		The message the bot will say
 * @return				No return
 **/
void CCSSBot::sayteam(const char *message)
{
	char buffer[256];
	sprintf(buffer, "say_team \"%s\"", message);
	helpers->ClientCommand(m_pEdict,buffer);
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
void CCSSBot::buy(const char *item)
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
	static int money;
	static int team;
	static int cost; // Computed buy cost
	static int remaining; // Remaining money (money - cost)
	static CBotWeapon *primary;
	static CBotWeapon *secondary;

	money = CClassInterface::getCSPlayerMoney(m_pEdict);
	team = getTeam();
	cost = 0;
	remaining = 0;
	primary = m_pWeapons->getCurrentWeaponInSlot(CS_WEAPON_SLOT_PRIMARY);
	secondary = m_pWeapons->getCurrentWeaponInSlot(CS_WEAPON_SLOT_SECONDARY);

	if(money <= rcbot_css_economy_eco_limit.GetInt())
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
	const bool hashelmet = CClassInterface::CSPlayerHasHelmet(m_pEdict);
	if(CClassInterface::getCSPlayerArmor(m_pEdict) <= 70 || !hashelmet)
	{
		buy("vesthelm");
		if(!hashelmet)
		{
			cost += 1000;
		}
		else
		{
			cost += 650;
		}
	}

	if(team == CS_TEAM_COUNTERTERRORIST && !CClassInterface::CSPlayerHasDefuser(m_pEdict) && CCounterStrikeSourceMod::isMapType(CS_MAP_BOMBDEFUSAL))
	{
		cost += 200;
		buy("defuser");
	}

	remaining = money - cost;
	if(primary)
	{
		// To-do: Upgrade primary logic
	}
	else
	{ // To-do: Buy selection logic
		if(remaining >= 7500)
		{
			cost += 4750;
			buy("awp");
		}
		else if(remaining >= 5000)
		{
			cost += 3200;
			buy("m4a1");
			buy("ak47");
		}
		else if(remaining >= 1500)
		{
			cost += 1500;
			buy("mp5navy");
		}
		else if(remaining >= 750)
		{
			cost += 750;
			buy("deagle");
		}
	}

	logger->Log(LogLevel::TRACE, "CSS --- Running buy logic for bot \"%s\"", m_pPlayerInfo->GetName());
	logger->Log(LogLevel::TRACE, "Team = %i --- Money = %i --- Cost = %i", team, money, cost);
	logger->Log(LogLevel::TRACE, "Primary Weapon = %s", primary ? primary->getWeaponInfo()->getWeaponName() : "No Primary");
	logger->Log(LogLevel::TRACE, "Secondary Weapon = %s", secondary ? secondary->getWeaponInfo()->getWeaponName() : "No Secondary");
	m_bDidBuy = true;
	updateCondition(CONDITION_CHANGED); // Buy done, update conditions
}

/**
 * Gets the bot primary weapon (will fallback to secondary)
 **/
CBotWeapon *CCSSBot::getPrimaryWeapon()
{
	static CBotWeapon *primary;
	static CBotWeapon *secondary;
	primary = m_pWeapons->getCurrentWeaponInSlot(CS_WEAPON_SLOT_PRIMARY);
	secondary = m_pWeapons->getCurrentWeaponInSlot(CS_WEAPON_SLOT_SECONDARY);

	if(primary)
		return primary;

	if(secondary)
		return secondary;

	return NULL;
}

/**
 * Custom primary attack function for Counter-Strike: Source bots
 * 
 * @param hold		Hold the attack button? (full-auto)
 * @return			No return
 **/
void CCSSBot::primaryattackCS(bool hold)
{
	if(hold)
	{
		primaryAttack(hold);
	}
	else
	{
		if(m_fNextAttackTime <= engine->Time())
		{
			tapButton(IN_ATTACK);
			CClients::clientDebugMsg(this, BOT_DEBUG_AIM, "[CSS-ATTACK] Primary Fire!");
			m_fNextAttackTime = engine->Time() + getNextAttackDelay(); // 50 ms delay between shots
		}
		else
		{
			CClients::clientDebugMsg(this, BOT_DEBUG_AIM, "[CSS-ATTACK] Wait!");
			letGoOfButton(IN_ATTACK);
		}
	}
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
		CBotWeapon* pWeapon = getBestWeapon(m_pEnemy);

		if(m_bWantToChangeWeapon && pWeapon != NULL && pWeapon != getCurrentWeapon() && pWeapon->getWeaponIndex())
		{
			selectWeapon(pWeapon->getWeaponIndex());
		}

		setLookAtTask(LOOK_ENEMY);

		if(!handleAttack(pWeapon, m_pEnemy))
		{
			m_pEnemy = NULL;
			m_pOldEnemy = NULL;
			wantToShoot(false);
		}
	}
}

bool CCSSBot::handleAttack(CBotWeapon *pWeapon, edict_t *pEnemy)
{
	if(pWeapon)
	{
		clearFailedWeaponSelect();

		if(pWeapon->isMelee())
			setMoveTo(CBotGlobals::entityOrigin(pEnemy));

		if(pWeapon->isZoomable() && !CCounterStrikeSourceMod::isScoped(this))
			secondaryAttack(false);

		primaryattackCS(false);
	}
	else
	{
		primaryattackCS(false);
	}

	return true;
}

float CCSSBot::getNextAttackDelay()
{
	static const float max = 4096.0f;
	static float dist;
	static float delay;
	delay = 0.050f; // Base delay

	dist = distanceFrom(getEnemy());
	delay = dist/max;
	clamp(delay, 0.050f, 0.300f);

	CClients::clientDebugMsg(this, BOT_DEBUG_AIM, "[CSS-ATTACK] Next Attack Delay: %2.4f", delay);

	return delay;
}

void CCSSBot::modAim(edict_t *pEntity, Vector &v_origin, Vector *v_desired_offset, Vector &v_size, float fDist, float fDist2D)
{
	static bool aimforhead;
	static CBotWeapon *pWp;
	static Vector vel;
	static Vector myvel;
	static Vector enemyvel;
	static float fDistFactor;
	static float fVelFactor;

	CBot::modAim(pEntity,v_origin,v_desired_offset,v_size,fDist,fDist2D);

	aimforhead = true;
	fVelFactor = 0.003125f;
	pWp = getCurrentWeapon();

	switch (pWp->getID())
	{
		case CS_WEAPON_AWP:
		case CS_WEAPON_SUPERSHOTGUN:
		case CS_WEAPON_AUTOSHOTGUN:
		{
			aimforhead = false;
			break;
		}
	}

	if ( pWp && pWp->isMelee() )
	{
		fDistFactor = 0;
		fVelFactor = 0;
	}
	else
	{
		if ( fDist < 160 )
			fVelFactor = 0.001f;

		fDistFactor = 1.0f - m_pProfile->m_fAimSkill + fDist*0.000125f*(m_fFov/90.0f);
	}

	myvel = Vector(0,0,0);
	enemyvel = Vector(0,0,0);

	// change in velocity
	if ( CClassInterface::getVelocity(pEntity,&enemyvel) && CClassInterface::getVelocity(m_pEdict,&myvel) )
	{
		vel = enemyvel - myvel; // relative velocity

		vel = vel * fVelFactor;//0.003125f;

		//fVelocityFactor = exp(-1.0f + ((vel.Length() * 0.003125f)*2)); // divide by max speed
	}
	else
	{
		vel = Vector(0.5f,0.5f,0.5f);
		//fVelocityFactor = 1.0f;
	}

	v_desired_offset->x = randomFloat(-vel.x,vel.x)*fDistFactor*v_size.x;
	v_desired_offset->y = randomFloat(-vel.y,vel.y)*fDistFactor*v_size.y;

	if(hasSomeConditions(CONDITION_SEE_ENEMY_HEAD) && aimforhead)
	{
		v_desired_offset->z = v_desired_offset->z + (v_size.z-1);
	}
	else
	{
		v_desired_offset->z = v_desired_offset->z + (v_size.z-16);
	}
}

void CCSSBot::modThink()
{
	m_pCurrentWeapon = CClassInterface::getCurrentWeapon(m_pEdict);
	static int team;
	team = getTeam();

	if(m_pCurrentWeapon)
	{
		CBotWeapon *weapon = m_pWeapons->getWeapon(CWeapons::getWeapon(m_pCurrentWeapon->GetClassName()));
		if(weapon && weapon->getClip1(this) == 0 && !weapon->isMelee() && weapon->getID() != CS_WEAPON_C4)
		{
			letGoOfButton(IN_ATTACK);
			tapButton(IN_RELOAD);
			updateCondition(CONDITION_OUT_OF_AMMO);
		}
		else
		{
			removeCondition(CONDITION_OUT_OF_AMMO);
		}
	}

	// Team Specific thinking
	switch (team)
	{
		case CS_TEAM_COUNTERTERRORIST:
		{
			if(!CCounterStrikeSourceMod::wasBombFound() && CCounterStrikeSourceMod::canHearPlantedBomb(this))
			{
				CCounterStrikeSourceMod::setBombFound(true);
				updateCondition(CONDITION_CHANGED);
				debugMsg(BOT_DEBUG_THINK, "[CSS-BOT] Found bomb!");
			}
			break;
		}
	}

	// Just saw my enemy?
	if(hasSomeConditions(CONDITION_SEE_CUR_ENEMY) && !m_bInCombat)
	{
		logger->Log(LogLevel::DEBUG, "Bot \"%s\" Entering combat mode at %.4f", m_pPlayerInfo->GetName(), engine->Time());
		m_fCombatTime = engine->Time();
		m_bInCombat = true;
		updateCondition(CONDITION_CHANGED); // Re-execute utility to enter into combat mode
	}
	else if(hasSomeConditions(CONDITION_ENEMY_DEAD) || hasSomeConditions(CONDITION_ENEMY_OBSCURED) && m_fCombatTime + 12.0f <= engine->Time())
	{
		m_bInCombat = false;
		m_pEnemy = NULL; // Bots are tracking enemies through walls
	}

	if(m_fNextThinkSlow <= engine->Time())
	{
		modThinkSlow();
	}
}

void CCSSBot::modThinkSlow()
{
	static Vector velocity;
	static float fvelocity;

	m_fNextThinkSlow = engine->Time() + 1.0f;

	velocity = Vector(0,0,0);
	CClassInterface::getVelocity(getEdict(), &velocity);
	fvelocity = velocity.Length();

	if(fvelocity >= 16.0f && CCounterStrikeSourceMod::isScoped(this))
	{
		secondaryAttack(false);
	}
}

void CCSSBot::getTasks(unsigned int iIgnore)
{
    static CBotUtilities utils;
    static CBotUtility* next;
    static bool bCheckCurrent;
	static int team;

	if(!hasSomeConditions(CONDITION_CHANGED) && !m_pSchedules->isEmpty())
		return;

    removeCondition(CONDITION_CHANGED);
    bCheckCurrent = true; // important for checking current schedule
	team = getTeam();
	setMoveSpeed(CClassInterface::getMaxSpeed(m_pEdict)); // Some tasks changes the bot move speed, reset it back.

	// Utilities

	switch (team)
	{
		case CS_TEAM_COUNTERTERRORIST: // CT specific utilities
		{
			if(CCounterStrikeSourceMod::isMapType(CS_MAP_BOMBDEFUSAL))
			{
				ADD_UTILITY(BOT_UTIL_DEFEND_BOMB, !CCounterStrikeSourceMod::isBombPlanted() && bot_defrate.GetFloat() <= randomFloat(0.0f, 1.0f), 0.80f);
				ADD_UTILITY(BOT_UTIL_SEARCH_FOR_BOMB, !CCounterStrikeSourceMod::wasBombFound() && CCounterStrikeSourceMod::isBombPlanted(), 0.81f);
				ADD_UTILITY(BOT_UTIL_DEFUSE_BOMB, CCounterStrikeSourceMod::wasBombFound(), 0.82f);
			}
			break;
		}
		case CS_TEAM_TERRORIST: // TR specific utilities
		{
			if(CCounterStrikeSourceMod::isMapType(CS_MAP_BOMBDEFUSAL))
			{
				ADD_UTILITY(BOT_UTIL_PLANT_BOMB, CCounterStrikeSourceMod::isBombCarrier(this), 0.80f);
				ADD_UTILITY(BOT_UTIL_PICKUP_BOMB, CCounterStrikeSourceMod::isBombDropped(), 0.80f);
				ADD_UTILITY(BOT_UTIL_DEFEND_NEAREST_BOMB, CCounterStrikeSourceMod::isBombPlanted(), 0.80f);
			}
			break;
		}
	}

	// Combat Utilities
	ADD_UTILITY(BOT_UTIL_ENGAGE_ENEMY, hasSomeConditions(CONDITION_SEE_CUR_ENEMY) && !hasSomeConditions(CONDITION_OUT_OF_AMMO), 0.98f);
	ADD_UTILITY(BOT_UTIL_WAIT_LAST_ENEMY, shouldWaitForEnemy(), 0.95f);
	ADD_UTILITY(BOT_UTIL_HIDE_FROM_ENEMY, hasSomeConditions(CONDITION_SEE_CUR_ENEMY) && hasSomeConditions(CONDITION_OUT_OF_AMMO), 0.98f);

	// Generic Utilities
	ADD_UTILITY(BOT_UTIL_BUY, !m_bDidBuy, 1.0f); // Buy weapons
	ADD_UTILITY(BOT_UTIL_ROAM, true, 0.0001f); // Roam around

	utils.execute();

	while ((next = utils.nextBest()) != NULL)
	{
		if(!m_pSchedules->isEmpty() && bCheckCurrent)
		{
			if(m_CurrentUtil != next->getId())
				m_pSchedules->freeMemory();
			else
				break;
		}

		bCheckCurrent = false;

		if(executeAction(next->getId()))
		{
			m_CurrentUtil = next->getId();

			if(m_fUtilTimes[next->getId()] < engine->Time())
				m_fUtilTimes[next->getId()] = engine->Time() + randomFloat(0.1f, 2.0f); // saves problems with consistent failing

			if(CClients::clientsDebugging(BOT_DEBUG_UTIL))
			{
				char buffer[128];
				sprintf(buffer, "(%.4f) %s\nTeam: %i\nBomb Carrier: %s\nBomb Dropped: %s\nBomb Planted: %s", engine->Time(), g_szUtils[next->getId()], team, 
				CCounterStrikeSourceMod::isBombCarrier(this) ? "Yes" : "No", CCounterStrikeSourceMod::isBombDropped() ? "Yes" : "No", 
				CCounterStrikeSourceMod::isBombPlanted() ? "Yes" : "No");
				CClients::clientDebugMsg(BOT_DEBUG_UTIL, buffer, this);
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
		case BOT_UTIL_ENGAGE_ENEMY:
		{
			CBotSchedule* pSched = new CBotSchedule();
			pSched->setID(SCHED_ATTACK);
			pSched->addTask(new CCSSEngageEnemyTask(m_pEnemy.get()));
			m_pSchedules->add(pSched);
			return true;
			break;
		}
		case BOT_UTIL_WAIT_LAST_ENEMY:
		{
			CBotSchedule* pSched = new CBotSchedule();
			CBotTask* pTask = new CBotWaitTask(randomFloat(3.0f, 6.0f), m_vLastSeeEnemy);
			pTask->setCompleteInterrupt(CONDITION_ENEMY_DEAD);
			pTask->setFailInterrupt(CONDITION_SEE_CUR_ENEMY);
			pSched->setID(SCHED_WAIT_FOR_ENEMY);
			pSched->addTask(pTask);
			m_pSchedules->add(pSched);
			return true;
			break;
		}
		case BOT_UTIL_HIDE_FROM_ENEMY:
		{
			CBotSchedule *pSched = new CBotSchedule();
			pSched->setID(SCHED_RUN_FOR_COVER);
			const int cover = CWaypointLocations::GetCoverWaypoint(getOrigin(), CBotGlobals::entityOrigin(m_pEnemy.get()), NULL, NULL, 0, 8.0f, 1024.0f);
			if(cover != -1)
			{
				CBotTask *pTask = new CFindPathTask(cover);
				pTask->setCompleteInterrupt(CONDITION_ENEMY_DEAD, CONDITION_OUT_OF_AMMO);
				pSched->addTask(pTask);
				m_pSchedules->add(pSched);
				return true;
			}
			break;			
		}
		case BOT_UTIL_BUY:
		{
			CBotSchedule* pSched = new CBotSchedule();
			pSched->setID(SCHED_BUY);
			pSched->addTask(new CCSSPerformBuyTask());
			m_pSchedules->add(pSched);
			return true;
			break;
		}
		case BOT_UTIL_PLANT_BOMB:
		{
			CWaypoint* pWaypoint = CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_GOAL);

			if(pWaypoint)
			{
				CWaypoint * pRoute = NULL;
				if(m_fUseRouteTime <= engine->Time())
				{
					pRoute = CWaypoints::randomRouteWaypoint(this, getOrigin(), pWaypoint->getOrigin(), getTeam(), pWaypoint->getArea());
				}
				m_pSchedules->add(new CCSSPlantBombSched(pWaypoint, pRoute));
				return true;
			}
			break;
		}
		case BOT_UTIL_PICKUP_BOMB:
		{
			edict_t *pBomb = CCounterStrikeSourceMod::getBomb();
			if(pBomb)
			{
				m_pSchedules->add(new CBotPickupSched(pBomb));
				return true;
			}
			break;
		}
		case BOT_UTIL_DEFEND_NEAREST_BOMB: /** T: Defend planted bomb **/
		{
			CBotSchedule* pSched = new CBotSchedule();
			pSched->setID(SCHED_DEFENDPOINT);
			edict_t *pBomb = CCounterStrikeSourceMod::getBomb();
			Vector vBomb = CBotGlobals::entityOrigin(pBomb);
			if(pBomb)
			{
				// Find the nearest bomb waypoint to retreive the area from
				CWaypoint *pBombWpt = CWaypoints::getWaypoint(CWaypoints::nearestWaypointGoal(CWaypointTypes::W_FL_GOAL, vBomb, 256.0f, 0));
				if(pBombWpt)
				{
					CWaypoint *pDefend = CWaypoints::randomWaypointGoalNearestArea(CWaypointTypes::W_FL_DEFEND, getTeam(), pBombWpt->getArea(), true, this, false, &vBomb);
					if(pDefend)
					{
						pSched->addTask(new CFindPathTask(CWaypoints::getWaypointIndex(pDefend)));
						pSched->addTask(new CCSSGuardTask(getPrimaryWeapon(), pDefend->getOrigin(), pDefend->getAimYaw(), false, 0.0f, pDefend->getFlags()));
						m_pSchedules->add(pSched);
						CClients::clientDebugMsg(this, BOT_DEBUG_UTIL, "[BOT_UTIL_DEFEND_NEAREST_BOMB] Bomb Waypoint (%i) Defend Waypoint (%i)", 
						CWaypoints::getWaypointIndex(pBombWpt), CWaypoints::getWaypointIndex(pDefend));
						return true;
					}
				}
			}
			break;
		}
		case BOT_UTIL_DEFEND_BOMB: /** CT: Defend bomb site **/
		{
			CBotSchedule* pSched = new CBotSchedule();
			pSched->setID(SCHED_DEFENDPOINT);
			CWaypoint *pGoal = CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_GOAL, getTeam());
			if(pGoal)
			{
				CWaypoint *pDefend = CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_DEFEND, getTeam(), pGoal->getArea(), true, this);
				if(pDefend)
				{
					pSched->addTask(new CFindPathTask(CWaypoints::getWaypointIndex(pDefend)));
					pSched->addTask(new CCSSGuardTask(getPrimaryWeapon(), pDefend->getOrigin(), pDefend->getAimYaw(), false, 0.0f, pDefend->getFlags()));
					m_pSchedules->add(pSched);
					return true;
				}
			}
			break;		
		}
		case BOT_UTIL_DEFUSE_BOMB:
		{
			edict_t *pBomb = CCounterStrikeSourceMod::getBomb();
			if(pBomb)
			{
				CBotSchedule *pSched = new CBotSchedule();
				CBotTask *pFindPath = new CFindPathTask(pBomb);
				CBotTask *pMoveTask = new CMoveToTask(pBomb);
				pFindPath->setFailInterrupt(CONDITION_SEE_CUR_ENEMY);
				pMoveTask->setFailInterrupt(CONDITION_SEE_CUR_ENEMY);
				pSched->setID(SCHED_BOMB);
				pSched->addTask(pFindPath);
				pSched->addTask(pMoveTask);
				pSched->addTask(new CCSSDefuseTheBombTask(CBotGlobals::entityOrigin(pBomb)));
				m_pSchedules->add(pSched);
				return true;
			}
			break;
		}
		case BOT_UTIL_SEARCH_FOR_BOMB:
		{
			CBotSchedule* pSched = new CBotSchedule();

			pSched->setID(SCHED_GOTO_ORIGIN);
			CWaypoint* pWaypoint = CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_GOAL);

			if(pWaypoint)
			{
				CWaypoint* pRoute = CWaypoints::randomRouteWaypoint(this, getOrigin(), pWaypoint->getOrigin(), getTeam(),
				                                                    pWaypoint->getArea());
				if(m_fUseRouteTime <= engine->Time())
				{
					if(pRoute)
					{
						const int iRoute = CWaypoints::getWaypointIndex(pRoute); // Route waypoint
						pSched->addTask(new CFindPathTask(iRoute, LOOK_WAYPOINT));
						pSched->addTask(new CMoveToTask(pRoute->getOrigin()));
						m_pSchedules->add(pSched);
						m_fUseRouteTime = engine->Time() + 30.0f;
					}
				}

				const int iWaypoint = CWaypoints::getWaypointIndex(pWaypoint);
				pSched->addTask(new CFindPathTask(iWaypoint, LOOK_WAYPOINT));
				pSched->addTask(new CMoveToTask(pWaypoint->getOrigin()));
				m_pSchedules->add(pSched);

				return true;
			}
			break;
		}
		case BOT_UTIL_ROAM:
		{
			CBotSchedule* pSched = new CBotSchedule();

			pSched->setID(SCHED_GOTO_ORIGIN);
			CWaypoint* pWaypoint = CWaypoints::randomWaypointGoal(-1);

			if(pWaypoint)
			{
				const int iWaypoint = CWaypoints::getWaypointIndex(pWaypoint);
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