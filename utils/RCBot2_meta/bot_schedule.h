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
#ifndef __RCBOT_SCHEDULE_H__
#define __RCBOT_SCHEDULE_H__

#include "bot.h"
#include "bot_task.h"
//#include "bot_fortress.h"

#include <deque>

class CBotTask;
class CAttackEntityTask;
class IBotTaskInterrupt;

#define BITS_SCHED_PASS_INT		(1<<0)
#define BITS_SCHED_PASS_FLOAT	(1<<1)
#define BITS_SCHED_PASS_VECTOR	(1<<2)
#define BITS_SCHED_PASS_EDICT	(1<<3)

typedef enum
{
	SCHED_NONE = 0,
	SCHED_ATTACK,
	SCHED_RUN_FOR_COVER,
	SCHED_GOTO_ORIGIN,
	SCHED_GOOD_HIDE_SPOT,
	SCHED_TF2_GET_FLAG,
	SCHED_TF2_GET_HEALTH,
	SCHED_TF_BUILD,
	SCHED_HEAL,
	SCHED_GET_METAL,
	SCHED_SNIPE,
	SCHED_UPGRADE,
	SCHED_USE_TELE,
	SCHED_SPY_SAP_BUILDING,
	SCHED_USE_DISPENSER,
	SCHED_PICKUP,
	SCHED_TF2_GET_AMMO,
	SCHED_TF2_FIND_FLAG,
	SCHED_LOOKAFTERSENTRY,
	SCHED_DEFEND,
	SCHED_ATTACKPOINT,
	SCHED_DEFENDPOINT,
	SCHED_TF2_PUSH_PAYLOADBOMB,
	SCHED_TF2_DEFEND_PAYLOADBOMB,
	SCHED_TF2_DEMO_PIPETRAP,
	SCHED_TF2_DEMO_PIPEENEMY,
	SCHED_BACKSTAB,
	SCHED_REMOVESAPPER,
	SCHED_GOTONEST,
	SCHED_MESSAROUND,
	SCHED_TF2_ENGI_MOVE_BUILDING,
	SCHED_FOLLOW_LAST_ENEMY,
	SCHED_SHOOT_LAST_ENEMY_POS,
	SCHED_GRAVGUN_PICKUP,
	SCHED_HELP_PLAYER,
	SCHED_BOMB,
	SCHED_TF_SPYCHECK,
	SCHED_FOLLOW,
	SCHED_DOD_DROPAMMO,
	SCHED_INVESTIGATE_NOISE,
	SCHED_CROUCH_AND_HIDE,
	SCHED_DEPLOY_MACHINE_GUN,
	SCHED_ATTACK_SENTRY_GUN,
	SCHED_RETURN_TO_INTEL,
	SCHED_INVESTIGATE_HIDE,
	SCHED_TAUNT,
	SCHED_MAX
	//SCHED_HIDE_FROM_ENEMY
}eBotSchedule;

class CBotSchedule
{
public:
	CBotSchedule(CBotTask *pTask)
	{
		_init();

		addTask(pTask);
	}

	CBotSchedule();

	void _init ();
	virtual void init () { return; } // nothing, used by sub classes

	void addTask( CBotTask *pTask );

	void execute ( CBot *pBot );

	const char *getIDString ();

	CBotTask *currentTask ()
	{
		return m_Tasks.empty() ? NULL : m_Tasks.front();
	}

	bool hasFailed ()
	{
		return m_bFailed;
	}

	bool isComplete ()
	{
		return m_Tasks.empty();
	}

	void freeMemory ()
	{
		for (CBotTask* task : m_Tasks) {
			delete task;
		}
		m_Tasks.clear();
	}

	void removeTop ();

	//////////////////////////

	void clearPass () { m_bitsPass = 0; }

	void passInt(int i);
	void passFloat(float f);
	void passVector(Vector v);
	void passEdict(edict_t *p);
	//////////////////////////

	bool hasPassInfo () { return (m_bitsPass!=0); }

	inline int passedInt () { return iPass; }
	inline float passedFloat() { return fPass; }
	inline Vector passedVector() { return vPass; }
	inline edict_t *passedEdict() { return pPass; }
	inline bool isID ( eBotSchedule iId ) { return m_iSchedId == iId; }

	inline bool hasPassInt () { return ((m_bitsPass&BITS_SCHED_PASS_INT)>0); }
	inline bool hasPassFloat () { return ((m_bitsPass&BITS_SCHED_PASS_FLOAT)>0); }
	inline bool hasPassVector () { return ((m_bitsPass&BITS_SCHED_PASS_VECTOR)>0); }
	inline bool hasPassEdict () { return ((m_bitsPass&BITS_SCHED_PASS_EDICT)>0); }

	inline void setID ( eBotSchedule iId ) { m_iSchedId = iId; }


private:
	std::deque<CBotTask*> m_Tasks;
	bool m_bFailed;
	eBotSchedule m_iSchedId;
	
	// passed information to next task(s)
	int iPass;
	float fPass;
	Vector vPass;
	edict_t *pPass;

	int m_bitsPass;
};

class CBotSchedules
{
public:
	bool hasSchedule ( eBotSchedule iSchedule )
	{
		for (CBotSchedule *sched : m_Schedules) {
			if (sched->isID(iSchedule)) {
				return true;
			}
		}
		return false;
	}

	bool isCurrentSchedule ( eBotSchedule iSchedule )
	{
		if ( m_Schedules.empty() )
			return false;
		return m_Schedules.front()->isID(iSchedule);
	}

	// remove the first schedule in the queue matching this schedule identifier
	void removeSchedule ( eBotSchedule iSchedule )
	{
		for (auto it = m_Schedules.begin(); it != m_Schedules.end(); ) {
			if ((*it)->isID(iSchedule)) {
				m_Schedules.erase(it);
				return;
			} else {
				++it;
			}
		}
	}

	void execute ( CBot *pBot )
	{
		if ( isEmpty() )
			return;

		CBotSchedule *pSched = m_Schedules.front();
		pSched->execute(pBot);

		if ( pSched->isComplete() || pSched->hasFailed() )
			removeTop();
	}

	void removeTop ()
	{
		CBotSchedule *pSched = m_Schedules.front();
		m_Schedules.pop_front();

		// TODO: eradicate freeMemory from the codebase
		pSched->freeMemory();

		delete pSched;
	}

	void freeMemory ()
	{
		for (CBotSchedule *sched : m_Schedules) {
			delete sched;
		}
		m_Schedules.clear();
	}

	void add ( CBotSchedule *pSchedule )
	{
		// initialize
		pSchedule->init();
		// add
		m_Schedules.push_back(pSchedule);
	}

	void addFront ( CBotSchedule *pSchedule )
	{
		pSchedule->init();
		m_Schedules.push_front(pSchedule);
	}

	inline bool isEmpty ()
	{
		return m_Schedules.empty();
	}

	CBotTask *getCurrentTask ()
	{
		if ( !m_Schedules.empty() )
		{
			CBotSchedule *sched = m_Schedules.front();

			if ( sched != NULL )
			{
				return sched->currentTask();
			}
		}

		return NULL;
	}

	CBotSchedule *getCurrentSchedule ()
	{
		if ( isEmpty() )
			return NULL;

		return m_Schedules.front();
	}

private:
	std::deque<CBotSchedule*> m_Schedules;
};
///////////////////////////////////////////
class CBotTF2DemoPipeTrapSched : public CBotSchedule
{
public:
	CBotTF2DemoPipeTrapSched ( eDemoTrapType type, Vector vStand, Vector vLoc, Vector vSpread, bool bAutoDetonate = false, int wptarea = -1 );

	void init();
};

class CBotTF2DemoPipeEnemySched : public CBotSchedule
{
public:
	CBotTF2DemoPipeEnemySched ( 
		CBotWeapon *pLauncher,
		Vector vStand, 
		edict_t *pEnemy );

	void init();
};

///////////////////////////////////////////////
class CBotTF2ShootLastEnemyPos : public CBotSchedule
{
public:
	CBotTF2ShootLastEnemyPos ( Vector vLastSeeEnemyPos, Vector vVelocity, edict_t *pLastEnemy );

	void init();
};

///////////////////////////////////////////////
class CBotTF2MessAroundSched : public CBotSchedule
{
public:
	CBotTF2MessAroundSched ( edict_t *pFriendly, int iMaxVoiceCmd );

	void init();
};

///////////////////////////////////////////////////
class CBotTauntSchedule : public CBotSchedule
{
public:
	CBotTauntSchedule ( edict_t *pPlayer, float fYaw );

	void init ();
private:
	MyEHandle m_pPlayer;
	float m_fYaw;
};
////////////////////////////////////////////
class CBotUseTeleSched : public CBotSchedule
{
public:
	CBotUseTeleSched ( edict_t *pTele );

	void init ();
};
///////////////////////////////////////////////////
class CBotEngiMoveBuilding : public CBotSchedule
{
public:
	CBotEngiMoveBuilding ( edict_t *pBotEdict, edict_t *pBuilding, eEngiBuild iObject, Vector vNewLocation, bool bCarrying );

	void init ();
};

/////////////////////////////////////////////////
class CBotAttackPointSched : public CBotSchedule
{
public:
	CBotAttackPointSched ( Vector vPoint, int iRadius, int iArea, bool bHasRoute = false, Vector vRoute = Vector(0,0,0), bool bNest = false, edict_t *pLastEnemySentry = NULL );

	void init ();
}; 
//////////////////////////////////////////////////
class CBotDefendPointSched : public CBotSchedule
{
public:
	CBotDefendPointSched ( Vector vPoint, int iRadius, int iArea );

	void init ();
}; 
///////////////////////////////////////////////
class CBotTF2PushPayloadBombSched : public CBotSchedule
{
public:
	CBotTF2PushPayloadBombSched (edict_t * ePayloadBomb);

	void init ();
}; 
///////////////////////////////////////////////
class CBotTF2DefendPayloadBombSched : public CBotSchedule
{
public:
	CBotTF2DefendPayloadBombSched (edict_t * ePayloadBomb);

	void init ();
}; 
///////////////////////////////////////////////////
class CBotSpySapBuildingSched : public CBotSchedule
{
public:
	CBotSpySapBuildingSched ( edict_t *pBuilding, eEngiBuild id );

	void init ();
};
////////////////////////////////////////////////
class CBotUseDispSched : public CBotSchedule
{
public:
	CBotUseDispSched ( CBot *pBot, edict_t *pDisp );

	void init ();
}; 
///////////////////////////////////////////////////

class CBotTFEngiUpgrade : public CBotSchedule
{
public:
	CBotTFEngiUpgrade ( CBot *pBot, edict_t *pBuilding );

	void init ();
};
//////////////////////////////////////////////////////
class CBotTFEngiMoveBuilding : public CBotSchedule
{
public:
	CBotTFEngiMoveBuilding ( edict_t *pBuilding, Vector vNewOrigin );

	void init ();
};
///////////////////////////////////////////////////
class CBotTFEngiBuild : public CBotSchedule
{
public:
	CBotTFEngiBuild ( CBot *pBot, eEngiBuild iObject, CWaypoint *pWaypoint );

	void init ();
};

class CBotRemoveSapperSched : public CBotSchedule
{
public:
	CBotRemoveSapperSched ( edict_t *pBuilding, eEngiBuild id );

	void init();
};

class CBotTF2HealSched : public CBotSchedule
{
public:
	CBotTF2HealSched(edict_t *pHeal);
	void init ();
};

class CBotDefendSched : public CBotSchedule
{
public:
	CBotDefendSched ( Vector vOrigin, float fMaxTime = 0 );

	CBotDefendSched ( int iWaypointID, float fMaxTime = 0 );

	void init ();
};

class CBotGetMetalSched : public CBotSchedule
{
public:
	CBotGetMetalSched ( Vector vOrigin );

	void init ();
};

class CBotBackstabSched : public CBotSchedule
{
public:
	CBotBackstabSched ( edict_t *pEnemy );

	void init();
};

class CBotTFEngiLookAfterSentry : public CBotSchedule
{
public:
	CBotTFEngiLookAfterSentry ( edict_t *pSentry );

	void init ();
};

class CBotFollowLastEnemy : public CBotSchedule
{
public:
	CBotFollowLastEnemy ( CBot *pBot, edict_t *pEnemy, Vector vLastSee );

	void init ();
};

class CBotTF2SnipeSched : public CBotSchedule
{
public:
	CBotTF2SnipeSched ( Vector vOrigin, int iWpt );

	void init ();
};

class CBotTF2SnipeCrossBowSched : public CBotSchedule
{
public:
	CBotTF2SnipeCrossBowSched(Vector vOrigin, int iWpt);

	void init();
};

class CBotTF2AttackSentryGun : public CBotSchedule 
{
public:
	CBotTF2AttackSentryGun( edict_t *pSentry, CBotWeapon *pWeapon );

	void init ()
	{
		setID(SCHED_ATTACK_SENTRY_GUN);
	}
};

class CBotTF2GetAmmoSched : public CBotSchedule
{
public:
	CBotTF2GetAmmoSched ( Vector vOrigin );

	void init ();
};

class CBotTF2GetHealthSched : public CBotSchedule
{
public:
	CBotTF2GetHealthSched ( Vector vOrigin );

	void init ();
};

class CBotTF2GetFlagSched : public CBotSchedule
{
public:
	CBotTF2GetFlagSched ( Vector vOrigin, bool bUseRoute = false, Vector vRoute = Vector(0,0,0) );

	void init ();
};

class CBotTF2FindFlagSched : public CBotSchedule
{
public:
	CBotTF2FindFlagSched ( Vector vOrigin );

	void init ();
};

class CBotPickupSched : public CBotSchedule
{
public:
	CBotPickupSched ( edict_t *pEdict );

	void init ();
};

class CBotPickupSchedUse : public CBotSchedule
{
public:
	CBotPickupSchedUse ( edict_t *pEdict );

	void init ();
};

class CBotGotoOriginSched : public CBotSchedule
{
public:
	CBotGotoOriginSched ( Vector vOrigin );

	CBotGotoOriginSched ( edict_t *pEdict );

	void init ();
};

class CBotAttackSched : public CBotSchedule
{
public:
	CBotAttackSched ( edict_t *pEdict );

	void init ();
};

class CRunForCover : public CBotSchedule
{
public:
	// run for cover to this spot
	CRunForCover ( Vector vOrigin );

	void init ()
	{
		setID(SCHED_RUN_FOR_COVER);
	}
};

class CBotInvestigateNoiseSched : public CBotSchedule
{
public:
	CBotInvestigateNoiseSched ( Vector vSource, Vector vAttackPoint );

	void init ();
};

class CGotoHideSpotSched : public CBotSchedule
{
public:
	// find a hide spot
	// hide from an enemy (pEdict)
	CGotoHideSpotSched ( CBot *pBot, edict_t *pEdict, bool bIsGrenade = false );
	// hide from a Vector
	CGotoHideSpotSched ( CBot *pBot, Vector vOrigin, IBotTaskInterrupt *interrupt = NULL );

	void init ();
};

class CGotoNestSched : public CBotSchedule
{
public:
	// iWaypoint = the waypoint number the bot will go to (to nest)
	// if iWaypoint is -1 it will find a random, suitable nest
	CGotoNestSched ( int iWaypoint );

	void init ();
};

class CCrouchHideSched : public CBotSchedule
{
public:
	// iWaypoint = the waypoint number the bot will go to (to nest)
	// if iWaypoint is -1 it will find a random, suitable nest
	CCrouchHideSched ( edict_t *pCoverFrom );

	void init ();
};

class CDeployMachineGunSched : public CBotSchedule
{
public:
	// iWaypoint = the waypoint number the bot will go to (to nest)
	// if iWaypoint is -1 it will find a random, suitable nest
	CDeployMachineGunSched ( CBotWeapon *pWeapon, CWaypoint *pWaypoint, Vector vEnemy );

	void init ()
	{
		setID(SCHED_DEPLOY_MACHINE_GUN);
	}
};


#endif