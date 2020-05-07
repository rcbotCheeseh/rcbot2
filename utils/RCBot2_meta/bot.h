/*
 *    part of https://rcbot2.svn.sourceforge.net/svnroot/rcbot2
 *
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

 //=============================================================================//
 //
 // HPB_bot2.h - bot header file (Copyright 2004, Jeffrey "botman" Broome)
 //
 //=============================================================================//

#ifndef __RCBOT2_H__
#define __RCBOT2_H__

//#include "cbase.h"
//#include "baseentity.h"
#include "filesystem.h"
#include "interface.h"
#include "engine/iserverplugin.h"
#include "iplayerinfo.h"
#ifdef __linux__
#include "shake.h"    //bir3yk
#endif
#include "IEngineTrace.h" // for traceline functions
#include "IEffects.h"
#include "vplane.h"
#include "eiface.h"
#ifdef __linux__
#include "shareddefs.h" //bir3yk
#endif
#include "igameevents.h"
#include "Color.h"
#include "usercmd.h"

#include "bot_utility.h"
#include "bot_const.h"
#include "bot_ehandle.h"
#include <queue>

#if defined WIN32 && !defined snprintf
#define snprintf _snprintf
#endif

using namespace std;

#define MAX_AMMO_TYPES 32
#define MAX_VOICE_CMDS 32
#define MIN_WPT_TOUCH_DIST 16.0f

// Interfaces from the engine
//using namespace VEngineServerV21;
//using namespace ServerGameClientsV3;
extern IVEngineServer* engine;  // helper functions (messaging clients, loading content, making entities, running commands, etc)
extern IFileSystem* filesystem;  // file I/O
//extern IGameEventManager *gameeventmanager;  // game events interface
extern IGameEventManager2* gameeventmanager;
extern IPlayerInfoManager* playerinfomanager;  // game dll interface to interact with players
extern IServerPluginHelpers* helpers;  // special 3rd party plugin helpers from the engine
extern IServerGameClients* gameclients;
extern IEngineTrace* enginetrace;
extern IEffects* g_pEffects;
extern IBotManager* g_pBotManager;
extern CGlobalVars* gpGlobals;

#define GET_HEALTH 0
#define GET_TEAM   1
#define GET_AMMO   2

#define T_OFFSETMAX  3

class CBotSquad;

bool BotFunc_BreakableIsEnemy(edict_t* pBreakable, edict_t* pEdict);

//-----------------------------------------------------------------------------
// Purpose: This is a backward compatible IServerGameDLL
//-----------------------------------------------------------------------------
abstract_class IServerGameDLL_004
{
public:
	virtual ~IServerGameDLL_004() = default;
	// Initialize the game (one-time call when the DLL is first loaded )
	// Return false if there is an error during startup.
	virtual bool			DLLInit(CreateInterfaceFn engineFactory,
										CreateInterfaceFn physicsFactory,
										CreateInterfaceFn fileSystemFactory,
										CGlobalVars * pGlobals) = 0;

	// This is called when a new game is started. (restart, map)
	virtual bool			GameInit(void) = 0;

	// Called any time a new level is started (after GameInit() also on level transitions within a game)
	virtual bool			LevelInit(char const* pMapName,
									char const* pMapEntities, char const* pOldLevel,
									char const* pLandmarkName, bool loadGame, bool background) = 0;

	// The server is about to activate
	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) = 0;

	// The server should run physics/think on all edicts
	virtual void			GameFrame(bool simulating) = 0;

	// Called once per simulation frame on the final tick
	virtual void			PreClientUpdate(bool simulating) = 0;

	// Called when a level is shutdown (including changing levels)
	virtual void			LevelShutdown(void) = 0;
	// This is called when a game ends (server disconnect, death, restart, load)
	// NOT on level transitions within a game
	virtual void			GameShutdown(void) = 0;

	// Called once during DLL shutdown
	virtual void			DLLShutdown(void) = 0;

	// Get the simulation interval (must be compiled with identical values into both client and game .dll for MOD!!!)
	// Right now this is only requested at server startup time so it can't be changed on the fly, etc.
	virtual float			GetTickInterval(void) const = 0;

	// Give the list of datatable classes to the engine.  The engine matches class names from here with
	//  edict_t::classname to figure out how to encode a class's data for networking
	virtual ServerClass* GetAllServerClasses(void) = 0;

	// Returns string describing current .dll.  e.g., TeamFortress 2, Half-Life 2.
	//  Hey, it's more descriptive than just the name of the game directory
	virtual const char* GetGameDescription(void) = 0;

	// Let the game .dll allocate it's own network/shared string tables
	virtual void			CreateNetworkStringTables(void) = 0;

	// Save/restore system hooks
	virtual CSaveRestoreData* SaveInit(int size) = 0;
	virtual void			SaveWriteFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void			SaveReadFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void			SaveGlobalState(CSaveRestoreData*) = 0;
	virtual void			RestoreGlobalState(CSaveRestoreData*) = 0;
	virtual void			PreSave(CSaveRestoreData*) = 0;
	virtual void			Save(CSaveRestoreData*) = 0;
	virtual void			GetSaveComment(char* comment, int maxlength, float flMinutes, float flSeconds, bool bNoTime = false) = 0;
	virtual void			WriteSaveHeaders(CSaveRestoreData*) = 0;
	virtual void			ReadRestoreHeaders(CSaveRestoreData*) = 0;
	virtual void			Restore(CSaveRestoreData*, bool) = 0;
	virtual bool			IsRestoring() = 0;

	// Returns the number of entities moved across the transition
	virtual int				CreateEntityTransitionList(CSaveRestoreData*, int) = 0;
	// Build the list of maps adjacent to the current map
	virtual void			BuildAdjacentMapList(void) = 0;

	// Retrieve info needed for parsing the specified user message
	virtual bool			GetUserMessageInfo(int msg_type, char* name, int maxnamelength, int& size) = 0;

	// Hand over the StandardSendProxies in the game DLL's module.
	virtual CStandardSendProxies* GetStandardSendProxies() = 0;

	// Called once during startup, after the game .dll has been loaded and after the client .dll has also been loaded
	virtual void			PostInit() = 0;
	// Called once per frame even when no level is loaded...
	virtual void			Think(bool finalTick) = 0;

#ifdef _XBOX
	virtual void			GetTitleName(const char* pMapName, char* pTitleBuff, int titleBuffSize) = 0;
#endif

	virtual void			PreSaveGameLoaded(char const* pSaveName, bool bCurrentlyInGame) = 0;

	// Returns true if the game DLL wants the server not to be made public.
	// Used by commentary system to hide multiplayer commentary servers from the master.
	virtual bool			ShouldHideServer(void) = 0;

	virtual void			InvalidateMdlCache() = 0;

	// * This function is new with version 6 of the interface.
	//
	// This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
	// iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
	// Added with version 2 of the interface.
	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) = 0;

	// Called after the steam API has been activated post-level startup
	virtual void			GameServerSteamAPIActivated(void) = 0;

	virtual void			GameServerSteamAPIShutdown(void) = 0;
};

/////////// Voice commands

class IBotFunction
{
public:
	virtual ~IBotFunction() = default;
	virtual void execute(CBot* pBot) = 0;
};

class CBroadcastVoiceCommand : public IBotFunction
{
public:
	CBroadcastVoiceCommand(edict_t* pPlayer, byte voicecmd) { m_pPlayer = pPlayer; m_VoiceCmd = voicecmd; };
	void execute(CBot* pBot) override;

private:
	edict_t* m_pPlayer;
	byte m_VoiceCmd;
};

typedef union
{
	struct
	{
		unsigned v1 : 2; // menu
		unsigned v2 : 3; // extra info
		unsigned unused : 3;
	}b1;

	byte voicecmd;
}u_VOICECMD;

typedef union
{
	struct
	{
		unsigned said_in_position : 1;
		unsigned said_move_out : 1;
		unsigned said_area_clear : 1;
		unsigned unused : 5;
	}b1;

	byte dat;
}squad_u;

// events
class CRCBotEventListener : public IGameEventListener2
{
	void Event(IGameEvent* pevent);
};

//extern IVDebugOverlay *debugoverlay;

class CBotVisibles;
class CFindEnemyFunc;
class IBotNavigator;
class CBotMemoryNode;
class CBotMemoryPop;
class CGA;
class CPerceptron;
class CBotSchedules;
class CBotGAValues;
class CBotStuckValues;
class CBotButtons;
class IBotNavigator;
class CFindEnemyFunc;
class CBotWeapons;
class CBotProfile;
class CWaypoint;
class CBotWeapon;
class CWeapon;
class CBotNeuralNet;
class CTrainingSet;

#define MOVELOOK_DEFAULT 0
#define MOVELOOK_THINK 1
#define MOVELOOK_MODTHINK 2
#define MOVELOOK_TASK 3
#define MOVELOOK_LISTEN 4
#define MOVELOOK_EVENT 5
#define MOVELOOK_ATTACK 6
#define MOVELOOK_OVERRIDE 6

class CBotLastSee
{
public:
	CBotLastSee()
	{
		reset();
	}

	inline void reset()
	{
		m_pLastSee = NULL; // edict
		m_fLastSeeTime = 0.0f; // time
	}

	CBotLastSee(edict_t* pEdict);

	void update();

	inline bool check(edict_t* pEdict)
	{
		return (pEdict == (m_pLastSee.get()));
	}

	bool hasSeen(float fTime);

	Vector getLocation();
private:
	MyEHandle m_pLastSee; // edict
	float m_fLastSeeTime; // time
	Vector m_vLastSeeLoc; // location
	Vector m_vLastSeeVel; // velocity
};

typedef union bot_statistics_t
{
	int data;
	struct
	{
		byte m_iTeamMatesInRange;
		byte m_iEnemiesInRange;
		byte m_iEnemiesVisible;
		byte m_iTeamMatesVisible;
	} stats;
} bot_statistics_s;

/*
class CBotSquads
{
public:
	CBotSquads ()
	{
	}

	void freeMapMemory()
	{
		unsigned int i;

		for ( i = 0; i < m_theSquads.size(); i ++ )
			m_theSquads[i].freeMapMemory();

		m_theSquads.clear();
	}
private:
	vector<CBotSquad> m_theSquads;
}

class CBotSquad
{
public:
	CBotSquad ( edict_t *pLeader )
	{
		m_Leader = pLeader;
		m_Members.clear();
	}

	void addMember ( edict_t *pMember )
	{
		m_Members.push_back(MyEHandle(pMember));
	}

	void think ()
	{
		if ( !CBotGlobals::entityIsValid(m_Leader) || CBotGlobals::entityIsAlive(m_Leader) )
		{
			m_Leader = NULL;

			// find a new leader
			unsigned int i;

			for ( i = 0; i < m_Members.size(); i ++ )
			{
				if ( m_Members[i].get() )
				break;
			}
		}
	}

	bool killme()
	{
		return (m_Leader.get() == NULL) && (m_Members.size() == 0);
	}

private:
	MyEHandle m_Leader;
	vector<MyEHandle> m_Members;
};
*/

class CBot
{
public:
	virtual ~CBot() = default;

	static const float m_fAttackLowestHoldTime;
	static const float m_fAttackHighestHoldTime;
	static const float m_fAttackLowestLetGoTime;
	static const float m_fAttackHighestLetGoTime;

	CBot();

	inline void clearFailedWeaponSelect() { m_iPrevWeaponSelectFailed = 0; }
	inline void failWeaponSelect() { m_iPrevWeaponSelectFailed++; }

	void debugMsg(int iLev, const char* szMsg);

	virtual unsigned int maxEntityIndex() { return MAX_PLAYERS; }

	// linux fix 1
	virtual void onInventoryApplication() {}

	// return distance from this origin

	int isDesiredClass(int iclass)
	{
		return m_iDesiredClass == iclass;
	}

	virtual void handleWeapons();

	inline Vector getOrigin()
	{
		return m_pController->GetLocalOrigin();
	}
	// linux fix 2
	inline float distanceFrom(Vector vOrigin)
	{
		return (vOrigin - m_pController->GetLocalOrigin()).Length();
	}
	inline float distanceFrom(edict_t* pEntity)
	{
		return (pEntity->GetCollideable()->GetCollisionOrigin() - m_pController->GetLocalOrigin()).Length();
		//return distanceFrom(CBotGlobals::entityOrigin(pEntity));
	}

	inline float distanceFrom2D(edict_t* pEntity)
	{
		return (pEntity->GetCollideable()->GetCollisionOrigin() - m_pController->GetLocalOrigin()).Length2D();
		//return distanceFrom(CBotGlobals::entityOrigin(pEntity));
	}

	/*
	 * init()
	 *
	 * initialize all bot variables
	 * (this is called when bot is made for the first time)
	 */
	virtual void init(bool bVarInit = false);
	// setup buttons and data structures
	virtual void setup();

	/*
	 * runPlayerMove()
	 *
	 * make bot move in the game
	 * botman : see CBasePlayer::RunNullCommand() for example of PlayerRunCommand()...
	 */
	inline void runPlayerMove();

	/*
	 * called when a bot dies
	 */
	virtual void died(edict_t* pKiller, const char* pszWeapon);
	virtual void killed(edict_t* pVictim, char* weapon);

	virtual int getTeam();

	bool isUnderWater();

	CBotWeapon* getBestWeapon(edict_t* pEnemy, bool bAllowMelee = true, bool bAllowMeleeFallback = true, bool bMeleeOnly = false, bool bExplosivesOnly = false);

	virtual void modThink() { return; }

	virtual bool isEnemy(edict_t* pEdict, bool bCheckWeapons = true) { return false; }

	inline bool hasSomeConditions(int iConditions)
	{
		return (m_iConditions & iConditions) > 0;
	}

	virtual bool handleAttack(CBotWeapon* pWeapon, edict_t* pEnemy);

	float DotProductFromOrigin(Vector pOrigin);

	bool FVisible(edict_t* pEdict, bool bCheckHead = false);

	bool isVisible(edict_t* pEdict);

	inline void setEnemy(edict_t* pEnemy)
	{
		m_pEnemy = pEnemy;
	}

	inline int getConditions()
	{
		return m_iConditions;
	}

	inline bool hasAllConditions(int iConditions)
	{
		return (m_iConditions & iConditions) == iConditions;
	}

	inline void updateCondition(int iCondition)
	{
		m_iConditions |= iCondition;
	}

	inline void removeCondition(int iCondition)
	{
		m_iConditions &= ~iCondition;
	}

	bool FInViewCone(edict_t* pEntity);

	/*
	 * make bot start the gmae, e.g join a team first
	 */
	virtual bool startGame();

	virtual bool checkStuck();

	virtual void currentlyDead();

	/*
	 * initialize this bot as a new bot with the edict of pEdict
	 */
	bool createBotFromEdict(edict_t* pEdict, CBotProfile* pProfile);

	/*
	 * returns true if bot is used in game
	 */
	inline bool inUse()
	{
		return (m_bUsed && (m_pEdict != NULL));
	}

	edict_t* getEdict();

	void setEdict(edict_t* pEdict);

	bool FVisible(Vector& vOrigin, edict_t* pDest = NULL);

	Vector getEyePosition();

	void think();

	virtual void friendlyFire(edict_t* pEdict) { };

	virtual void freeMapMemory();

	virtual void freeAllMemory();

	///////////////////////////////
	inline bool moveToIsValid()
	{
		return m_bMoveToIsValid;
	}

	inline bool lookAtIsValid()
	{
		return m_bLookAtIsValid;
	}

	inline Vector getMoveTo()
	{
		return m_vMoveTo;
	}

	inline bool moveFailed()
	{
		bool ret = m_bFailNextMove;

		m_bFailNextMove = false;

		return ret;
	}

	void selectWeaponSlot(int iSlot);

	edict_t* getAvoidEntity() { return m_pAvoidEntity; }

	void setAvoidEntity(edict_t* pEntity) { m_pAvoidEntity = pEntity; }

	virtual void updateConditions();

	virtual bool canAvoid(edict_t* pEntity);

	inline bool hasEnemy() { return m_pEnemy && hasSomeConditions(CONDITION_SEE_CUR_ENEMY); }
	inline edict_t* getEnemy() { return m_pEnemy; }

	inline void setMoveTo(const Vector& vNew)
	{
		if (m_iMoveLookPriority >= m_iMovePriority)
		{
			m_vMoveTo = vNew;
			m_bMoveToIsValid = true;
			m_iMovePriority = m_iMoveLookPriority;
		}
	}

	// this allows move speed to be changed in tasks
	inline void setMoveSpeed(float fNewSpeed)
	{
		if (m_iMoveLookPriority >= m_iMoveSpeedPriority)
		{
			m_fIdealMoveSpeed = fNewSpeed;
			m_iMoveSpeedPriority = m_iMoveLookPriority;
		}
	}

	void findEnemy(edict_t* pOldEnemy = NULL);
	virtual void enemyFound(edict_t* pEnemy);

	virtual void checkDependantEntities();

	inline IBotNavigator* getNavigator() { return m_pNavigator; }

	inline void setMoveLookPriority(int iPriority) { m_iMoveLookPriority = iPriority; }

	inline void stopMoving()
	{
		if (m_iMoveLookPriority >= m_iMovePriority)
		{
			m_bMoveToIsValid = false;
			m_iMovePriority = m_iMoveLookPriority;
			m_fWaypointStuckTime = 0;
			m_fCheckStuckTime = engine->Time() + 4.0f;
		}
	}

	inline void stopLooking()
	{
		if (m_iMoveLookPriority >= m_iLookPriority)
		{
			m_bLookAtIsValid = false;
			m_iLookPriority = m_iMoveLookPriority;
		}
	}

	inline void setLookAtTask(eLookTask lookTask, float fTime = 0)
	{
		if ((m_iMoveLookPriority >= m_iLookPriority) && ((fTime > 0) || (m_fLookSetTime < engine->Time())))
		{
			m_iLookPriority = m_iMoveLookPriority;
			m_iLookTask = lookTask;

			if (fTime > 0)
				m_fLookSetTime = engine->Time() + fTime;
		}
	}

	virtual void enemyLost(edict_t* pEnemy) {};

	void setLastEnemy(edict_t* pEnemy);

	virtual void enemyDown(edict_t* pEnemy)
	{
		if (pEnemy == m_pEnemy)
			updateCondition(CONDITION_ENEMY_DEAD);
		if (pEnemy == m_pLastEnemy)
		{
			m_pLastEnemy = NULL;
		}
	}
	//////////////////////
	virtual bool isCSS() { return false; }
	virtual bool isHLDM() { return false; }
	virtual bool isTF() { return false; }

	virtual void spawnInit();

	QAngle eyeAngles();

	virtual bool isAlive();

	bool onLadder();

	inline bool currentEnemy(edict_t* pEntity) { return m_pEnemy == pEntity; }

	Vector getAimVector(edict_t* pEntity);
	virtual void modAim(edict_t* pEntity, Vector& v_origin,
		Vector* v_desired_offset, Vector& v_size,
		float fDist, float fDist2D);

	inline Vector* getGoalOrigin()
	{
		return &m_vGoal;
	}

	inline bool hasGoal()
	{
		return m_bHasGoal;
	}

	bool isHoldingPrimaryAttack();

	void primaryAttack(bool bHold = false, float fTime = 0.0f);
	void secondaryAttack(bool bHold = false);
	void jump();
	void duck(bool hold = false);
	void use();
	void reload();

	virtual bool setVisible(edict_t* pEntity, bool bVisible);

	virtual bool hurt(edict_t* pAttacker, int iHealthNow, bool bDontHide = false);
	virtual void shot(edict_t* pEnemy);
	virtual void shotmiss();
	//inline void setAvoidEntity (edict_t *pEntity) { m_pAvoidEntity = pEntity; };

	int getPlayerID(); // return player ID on server
	int getHealth();

	float getHealthPercent();

	inline CBotSchedules* getSchedule() { return m_pSchedules; }

	virtual void reachedCoverSpot(int flags);

	virtual bool wantToFollowEnemy();

	virtual void seeFriendlyHurtEnemy(edict_t* pTeammate, edict_t* pEnemy, CWeapon* pWeapon) { };
	virtual void seeEnemyHurtFriendly(edict_t* pTeammate, edict_t* pEnemy, CWeapon* pWeapon) { };
	virtual void seeFriendlyDie(edict_t* pDied, edict_t* pKiller, CWeapon* pWeapon) { };
	virtual void seeFriendlyKill(edict_t* pTeamMate, edict_t* pDied, CWeapon* pWeapon) { };

	inline void selectWeapon(int iWeaponId) { m_iSelectWeapon = iWeaponId; }

	void selectWeaponName(const char* szWeaponName);

	virtual CBotWeapon* getCurrentWeapon();

	void kill();

	bool isUsingProfile(CBotProfile* pProfile);

	inline CBotProfile* getProfile() { return m_pProfile; }

	virtual bool canGotoWaypoint(Vector vPrevWaypoint, CWaypoint* pWaypoint, CWaypoint* pPrev = NULL);

	void tapButton(int iButton);

	inline int getAmmo(int iIndex) { if (!m_iAmmo) return 0; else if (iIndex == -1) return 0;  return m_iAmmo[iIndex]; }

	inline void lookAtEdict(edict_t* pEdict) { m_pLookEdict = pEdict; }

	virtual bool select_CWeapon(CWeapon* pWeapon);
	virtual bool selectBotWeapon(CBotWeapon* pBotWeapon);

	void updatePosition();

	MyEHandle m_pLookEdict;

	CBotWeapons* getWeapons() { return m_pWeapons; }

	virtual float getEnemyFactor(edict_t* pEnemy);

	virtual void checkCanPickup(edict_t* pPickup);

	virtual void touchedWpt(CWaypoint* pWaypoint, int iNextWaypoint = -1, int iPrevWaypoint = -1);

	inline void setAiming(const Vector& aiming) { m_vWaypointAim = aiming; }

	inline Vector getAiming() { return m_vWaypointAim; }

	inline void setLookVector(const Vector& vLook) { m_vLookVector = vLook; }

	inline Vector getLookVector() { return m_vLookVector; }

	inline void resetLookAroundTime() { m_fLookAroundTime = 0.0f; }

	Vector snipe(Vector& vAiming);

	//inline void dontAvoid () { m_fAvoidTime = engine->Time() + 1.0f; }

	float m_fWaypointStuckTime;

	inline float getSpeed() { return m_vVelocity.Length2D(); }

	void updateStatistics(); // updates number of teammates/enemies nearby/visible
	virtual void listenForPlayers();
	// listens to this player
	void listenToPlayer(edict_t* pListenTo, bool bIsEnemy = false, bool bIsAttacking = false);
	virtual bool wantToListenToPlayerAttack(edict_t* pPlayer, int iWeaponID = -1) { return true; }
	virtual bool wantToListenToPlayerFootsteps(edict_t* pPlayer) { return true; }

	virtual bool wantToInvestigateSound();
	inline void wantToInvestigateSound(bool bSet) { m_bWantToInvestigateSound = bSet; }
	inline bool wantToShoot(void) { return m_bOpenFire; }
	inline void wantToShoot(bool bSet) { m_bOpenFire = bSet; }
	inline void wantToListen(bool bSet) { m_bWantToListen = bSet; }
	bool wantToListen();
	inline void wantToChangeWeapon(bool bSet) { m_bWantToChangeWeapon = bSet; }

	int nearbyFriendlies(float fDistance);

	bool isFacing(Vector vOrigin);

	bool isOnLift(void);

	virtual bool isDOD() { return false; }

	virtual bool isTF2() { return false; }

	// return an enemy sentry gun / special visible (e.g.) for quick checking - voffset is the 'head'
	virtual edict_t* getVisibleSpecial();

	void updateDanger(float fBelief);

	inline void reduceTouchDistance() { if (m_fWaypointTouchDistance > MIN_WPT_TOUCH_DIST) { m_fWaypointTouchDistance *= 0.9; } }

	inline void resetTouchDistance(float fDist) { m_fWaypointTouchDistance = fDist; }

	inline float getTouchDistance() { return m_fWaypointTouchDistance; }

	inline CBotCmd* getUserCMD() { return &cmd; }

	void forceGotoWaypoint(int wpt);

	// bot is defending -- mod specific stuff
	virtual void defending() {}

	virtual void hearVoiceCommand(edict_t* pPlayer, byte cmd) {};

	virtual void grenadeThrown();

	virtual void voiceCommand(int cmd) { };

	void addVoiceCommand(int cmd);

	void letGoOfButton(int button);

	virtual bool overrideAmmoTypes() { return true; }

	virtual void debugBot(char* msg);

	virtual bool walkingTowardsWaypoint(CWaypoint* pWaypoint, bool* bOffsetApplied, Vector& vOffset);

	void setCoverFrom(edict_t* pCoverFrom) { m_pLastCoverFrom = MyEHandle(pCoverFrom); }

	virtual void areaClear() { }

	inline void resetAreaClear() { m_uSquadDetail.b1.said_area_clear = false; }

	inline bool inSquad(CBotSquad* pSquad)
	{
		return m_pSquad == pSquad;
	}

	inline bool inSquad(void)
	{
		return m_pSquad != NULL;
	}

	bool isSquadLeader(void);

	inline void setSquadIdleTime(float fTime)
	{
		m_fSquadIdleTime = fTime;
	}

	void clearSquad();

	inline void setSquad(CBotSquad* pSquad)
	{
		m_pSquad = pSquad;
	}

	void SquadInPosition();
	virtual void sayInPosition() { }
	virtual void sayMoveOut() { }

	bot_statistics_t* getStats() { if (m_bStatsCanUse) return &m_StatsCanUse; return NULL; }

	virtual void hearPlayerAttack(edict_t* pAttacker, int iWeaponID);

	inline bool isListeningToPlayer(edict_t* pPlayer)
	{
		return (m_PlayerListeningTo.get() == pPlayer);
	}

	inline IBotController* getController() const
	{
		return m_pController;
	}

	void updateUtilTime(int util);

	virtual bool getIgnoreBox(Vector* vLoc, float* fSize)
	{
		return false;
	}

	bool recentlyHurt(float fTime);

	eBotAction getCurrentUtil(void) { return m_CurrentUtil; }

	bool recentlySpawned(float fTime);

protected:

	inline void setLookAt(const Vector& vNew)
	{
		m_vLookAt = vNew;
		m_bLookAtIsValid = true;
	}

	static void checkEntity(edict_t** pEdict);
	/////////////////////////
	void doMove();

	void doLook();

	virtual void getLookAtVector();

	void doButtons();
	/////////////////////////

	void changeAngles(float fSpeed, float* fIdeal, float* fCurrent, float* fUpdate);

	// look for new tasks
	virtual void getTasks(unsigned int iIgnore = 0);

	// really only need 249 bits (32 bytes) + WEAPON_SUBTYPE_BITS (whatever that is)
	static const int CMD_BUFFER_SIZE = 64;
	///////////////////////////////////
	// bots edict
	edict_t* m_pEdict;
	MyEHandle m_pAvoidEntity;
	//float m_fLastPrintDebugInfo;
	// is bot used in the game?
	bool m_bUsed;
	// time the bot was made in the server
	float m_fTimeCreated;
	// next think time
	float m_fNextThink;

	float m_fFov;

	bool m_bInitAlive;
	bool m_bThinkStuck;

	int m_iMovePriority;
	int m_iLookPriority;
	int m_iMoveSpeedPriority;

	int m_iMoveLookPriority;

	int* m_iAmmo;
	bool m_bLookedForEnemyLast;

	//CBotStuckValues *m_pGAvStuck;
	//CGA *m_pGAStuck;
	//CPerceptron *m_pThinkStuck;
	Vector m_vStuckPos;
	//int m_iTimesStuck;
	float m_fAvoidTime;
	///////////////////////////////////
	// current impulse command
	int m_iImpulse;
	// buttons held
	int m_iButtons;
	// bots forward move speed
	float m_fForwardSpeed;
	// bots side move speed
	float m_fSideSpeed;
	// bots upward move speed (e.g in water)
	float m_fUpSpeed;
	float m_fAimMoment; // aiming "mouse" momentum

	float m_fLookAtTimeStart;
	float m_fLookAtTimeEnd;
	// Look task can't be changed if this is greater than Time()
	float m_fLookSetTime;
	float m_fLookAroundTime;

	int m_iFlags;
	// Origin a second ago to check if stuck
	Vector m_vLastOrigin;
	// Generated velocity found from last origin (not always correct)
	Vector m_vVelocity;
	// next update time (1 second update)
	float m_fUpdateOriginTime;
	float m_fStuckTime;
	float m_fCheckStuckTime;
	float m_fNextUpdateStuckConstants;

	float m_fStrafeTime;
	float m_fLastSeeEnemy;
	float m_fLastUpdateLastSeeEnemy;

	float m_fUpdateDamageTime;
	// Damage bot accumulated over the last second or so
	int m_iAccumulatedDamage;
	int m_iPrevHealth;
	///////////////////////////////////
	int m_iConditions;
	// bot tasks etc -- complex actuators
	CBotSchedules* m_pSchedules;
	// buttons held -- simple actuators
	CBotButtons* m_pButtons;
	// Navigation used for this bot -- environment sensor 1
	IBotNavigator* m_pNavigator;
	// bot visible list -- environment sensor 2
	CBotVisibles* m_pVisibles;
	// visible functions -- sensory functions
	CFindEnemyFunc* m_pFindEnemyFunc;
	// weapons storage -- sensor
	CBotWeapons* m_pWeapons;
	////////////////////////////////////
	IPlayerInfo* m_pPlayerInfo; //-- sensors
	IBotController* m_pController; //-- actuators
	CBotCmd cmd; // actuator command
	////////////////////////////////////
	MyEHandle m_pEnemy; // current enemy
	MyEHandle m_pOldEnemy;
	Vector m_vLastSeeEnemy;
	Vector m_vLastSeeEnemyBlastWaypoint;
	MyEHandle m_pLastEnemy; // enemy we were fighting before we lost it
	//edict_t *m_pAvoidEntity; // avoid this guy
	Vector m_vHurtOrigin;
	Vector m_vLookVector;
	Vector m_vLookAroundOffset;
	MyEHandle m_pPickup;
	Vector m_vWaypointAim;

	Vector m_vMoveTo;
	Vector m_vLookAt;
	Vector m_vGoal; // goal vector
	bool m_bHasGoal;
	QAngle m_vViewAngles;
	float m_fNextUpdateAimVector;
	float m_fStartUpdateAimVector;
	Vector m_vAimVector;
	Vector m_vPrevAimVector;
	Vector m_vLastDiedOrigin;
	bool m_bPrevAimVectorValid;

	eLookTask m_iLookTask;

	bool m_bMoveToIsValid;
	bool m_bLookAtIsValid;

	float m_fIdealMoveSpeed;

	float m_fLastWaypointVisible;

	bool m_bFailNextMove;

	int m_iSelectWeapon;

	int m_iDesiredTeam;
	int m_iDesiredClass;

	// bots profile data
	CBotProfile* m_pProfile;

	float m_fPercentMoved;

	/////////////////////////////////

	char m_szBotName[64];

	/////////////////////////////////
	Vector m_vListenPosition; // listening player position, heard someone shoot
	bool m_bListenPositionValid;
	float m_fListenTime;
	MyEHandle m_PlayerListeningTo;
	float m_fWantToListenTime;
	bool m_bOpenFire;
	unsigned int m_iPrevWeaponSelectFailed;

	bool m_bWantToListen;
	float m_fListenFactor; // the current weight of bots listening vector (higher = better)
	float m_fUseRouteTime;

	bool m_bWantToChangeWeapon;

	bool m_bAvoidRight;
	float m_fAvoidSideSwitch;
	float m_fHealClickTime;

	unsigned int m_iSpecialVisibleId;
	float m_fCurrentDanger;
	float m_fLastHurtTime;

	float m_fUtilTimes[BOT_UTIL_MAX];

	float m_fWaypointTouchDistance;

	eBotAction m_CurrentUtil;
	//CBotNeuralNet *stucknet;
	//CTrainingSet *stucknet_tset;

	queue<int> m_nextVoicecmd;
	float m_fNextVoiceCommand;
	float m_fLastVoiceCommand[MAX_VOICE_CMDS];

	float m_fTotalAimFactor;
	Vector m_vAimOffset;
	MyEHandle m_pLastCoverFrom;

	bot_statistics_t m_Stats; // this updates progressively
	bot_statistics_t m_StatsCanUse; // this updates fully every 5 seconds max
	bool m_bStatsCanUse;
	float m_fStatsTime;
	short int m_iStatsIndex;

	CBotSquad* m_pSquad;
	float m_fSquadIdleTime;
	squad_u m_uSquadDetail;
	CBotWeapon* m_pPrimaryWeapon;
	float m_fLastSeeEnemyPlayer;
	// if bot's profile sensitivity is too small it may not complete tasks
	// this is true during tasks that need high sensitivity e.g. rocket jumping
	bool m_bIncreaseSensitivity;
	float m_fSpawnTime;
	bool m_bWantToInvestigateSound;
};

class CAddbot
{
public:

	CAddbot()
	{
		memset(this, 0, sizeof(CAddbot));
	};

	const char* m_szClass;
	const char* m_szTeam;
	const char* m_szBotName;
};

class CBots
{
public:
	static void botThink();

	static CBot* getBotPointer(edict_t* pEdict);

	static void freeMapMemory();

	static void freeAllMemory();

	static CBot* findBotByProfile(CBotProfile* pProfile);

	static void init();

	static void controlBotSetup(bool m_bSetting) { m_bControlBotsOnly = m_bSetting; }

	// If true, then a puppet bot must be added to be controlled
	static bool controlBots() { return m_bControlBotsOnly; }

	static bool controlBot(edict_t* pEdict);

	static bool controlBot(const char* szOldName, const char* szName, const char* szTeam, const char* szClass);

	static bool createBot(const char* szClass, const char* szTeam, const char* szName);

	static int numBots();

	static bool handlePlayerJoin(edict_t* pEdict, const char* name);

	static int slotOfEdict(edict_t* pEdict);

	static void roundStart();

	static void kickRandomBot();

	static void kickRandomBotOnTeam(int team);

	static void mapInit();

	static bool needToAddBot();

	static bool needToKickBot();

	static void setMaxBots(int iMax) { m_iMaxBots = iMax; }

	static int getMaxBots() { return m_iMaxBots; }

	static void setMinBots(int iMin) { m_iMinBots = iMin; }

	static int getMinBots() { return m_iMinBots; }

	static void botFunction(IBotFunction* function);

	static void handleAutomaticControl();

	static void runPlayerMoveAll();

	static bool addBot(const char* szClass, const char* szTeam, const char* szName);

	static CBot* get(int iIndex) { return m_Bots[iIndex]; }
	static CBot* get(edict_t* pPlayer) { return m_Bots[slotOfEdict(pPlayer)]; }

private:
	static CBot** m_Bots;

	//config
	static int m_iMaxBots;
	static int m_iMinBots;

	// Workaround for add bot bug
	//
	static bool m_bControlBotsOnly;
	static bool m_bControlNext;
	static CBotProfile* m_pNextProfile;
	static char m_szNextName[64];
	// End - workaround

	// add or kick bot time
	static float m_flAddKickBotTime;

	static queue<edict_t*> m_ControlQueue;

	static queue<CAddbot> m_AddBotQueue;
};

class IEntityFactoryDictionary;
class IClientEntity;
class CBaseAnimating;
class ITempEntsSystem;
class CTakeDamageInfo;
//-----------------------------------------------------------------------------
// Purpose: Interface from engine to tools for manipulating entities
//-----------------------------------------------------------------------------
class IServerTools : public IBaseInterface
{
public:
	virtual IServerEntity* GetIServerEntity(IClientEntity* pClientEntity) = 0;
	virtual bool SnapPlayerToPosition(const Vector& org, const QAngle& ang, IClientEntity* pClientPlayer = NULL) = 0;
	virtual bool GetPlayerPosition(Vector& org, QAngle& ang, IClientEntity* pClientPlayer = NULL) = 0;
	virtual bool SetPlayerFOV(int fov, IClientEntity* pClientPlayer = NULL) = 0;
	virtual int GetPlayerFOV(IClientEntity* pClientPlayer = NULL) = 0;
	virtual bool IsInNoClipMode(IClientEntity* pClientPlayer = NULL) = 0;

	// entity searching
	virtual CBaseEntity* FirstEntity(void) = 0;
	virtual CBaseEntity* NextEntity(CBaseEntity* pEntity) = 0;
	virtual CBaseEntity* FindEntityByHammerID(int iHammerID) = 0;

	// entity query
	virtual bool GetKeyValue(CBaseEntity* pEntity, const char* szField, char* szValue, int iMaxLen) = 0;
	virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, const char* szValue) = 0;
	virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, float flValue) = 0;
	virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, const Vector& vecValue) = 0;

	// entity spawning
	virtual CBaseEntity* CreateEntityByName(const char* szClassName) = 0;
	virtual void DispatchSpawn(CBaseEntity* pEntity) = 0;

	// This reloads a portion or all of a particle definition file.
	// It's up to the server to decide if it cares about this file
	// Use a UtlBuffer to crack the data
	virtual void ReloadParticleDefintions(const char* pFileName, const void* pBufData, int nLen) = 0;

	virtual void AddOriginToPVS(const Vector& org) = 0;
	virtual void MoveEngineViewTo(const Vector& vPos, const QAngle& vAngles) = 0;

	virtual bool DestroyEntityByHammerId(int iHammerID) = 0;
	virtual CBaseEntity* GetBaseEntityByEntIndex(int iEntIndex) = 0;
	virtual void RemoveEntity(CBaseEntity* pEntity) = 0;
	virtual void RemoveEntityImmediate(CBaseEntity* pEntity) = 0;
	virtual IEntityFactoryDictionary* GetEntityFactoryDictionary(void) = 0;

	virtual void SetMoveType(CBaseEntity* pEntity, int val) = 0;
	virtual void SetMoveType(CBaseEntity* pEntity, int val, int moveCollide) = 0;
	virtual void ResetSequence(CBaseAnimating* pEntity, int nSequence) = 0;
	virtual void ResetSequenceInfo(CBaseAnimating* pEntity) = 0;

	virtual void ClearMultiDamage(void) = 0;
	virtual void ApplyMultiDamage(void) = 0;
	virtual void AddMultiDamage(const CTakeDamageInfo& pTakeDamageInfo, CBaseEntity* pEntity) = 0;
	virtual void RadiusDamage(const CTakeDamageInfo& info, const Vector& vecSrc, float flRadius, int iClassIgnore, CBaseEntity* pEntityIgnore) = 0;

	virtual ITempEntsSystem* GetTempEntsSystem(void) = 0;
};

typedef IServerTools IServerTools001;

#define VSERVERTOOLS_INTERFACE_VERSION_1	"VSERVERTOOLS001"
#define VSERVERTOOLS_INTERFACE_VERSION		"VSERVERTOOLS002"
#define VSERVERTOOLS_INTERFACE_VERSION_INT	2

void DrawLine(const Vector& origin, const Vector& target, int r, int g, int b, bool noDepthTest, float duration);

void WriteUsercmd(bf_write* buf, CUserCmd* cmd);
// useful helper funcs...
//int RANDOM_INT(int min, int max);
// Misc useful
inline bool FStrEq(const char* sz1, const char* sz2)
{
	return (sz1 == sz2 || Q_stricmp(sz1, sz2) == 0);
}
//edict_t* INDEXENT( int iEdictNum );
// get entity index
//int ENTINDEX( edict_t *pEdict );
float VectorDistance(Vector& vec);
int Ceiling(float fVal);
bool FNullEnt(const edict_t* pent);
// ??
//#define SAFE_STRCPY (to,from,siz)\
//	strncpy(to,from,siz-1);\
//	to[siz-1] = 0;

#endif // __RCBOT2_H__
