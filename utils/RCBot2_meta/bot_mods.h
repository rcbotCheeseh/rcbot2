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
#ifndef __BOT_MODS_H__
#define __BOT_MODS_H__

#include "bot_const.h"
#include "bot_strings.h"
#include "bot_fortress.h"
#include "bot_dod_bot.h"
#include "bot_waypoint.h"
#include "bot_tf2_points.h"

#define MAX_CAP_POINTS 32

#define DOD_MAPTYPE_UNKNOWN 0 
#define DOD_MAPTYPE_FLAG 1
#define DOD_MAPTYPE_BOMB 2

#define BOT_ADD_METHOD_DEFAULT 0
#define BOT_ADD_METHOD_PUPPET 1
#define BOT_ADD_PUPPET_COMMAND "bot"

class CBotNeuralNet;

#include <vector>


/*
		CSS
		TF2
		HL2DM
		HL1DM
		FF
		COOP
		ZOMBIE
*/
typedef enum
{
	BOTTYPE_GENERIC = 0,
	BOTTYPE_CSS,
	BOTTYPE_TF2,
	BOTTYPE_HL2DM,
	BOTTYPE_HL1DM,
	BOTTYPE_FF,
	BOTTYPE_COOP,
	BOTTYPE_ZOMBIE,
	BOTTYPE_DOD,
	BOTTYPE_NS2,
	BOTTYPE_MAX
}eBotType;

class CBotMod
{
public:
	CBotMod() 
	{
		m_szModFolder = NULL;
		m_szSteamFolder = NULL;
		m_szWeaponListName = NULL;
		m_iModId = MOD_UNSUPPORTED;
		m_iBotType = BOTTYPE_GENERIC;
		m_bPlayerHasSpawned = false;
		m_bBotCommand_ResetCheatFlag = false;
	}

	virtual bool checkWaypointForTeam(CWaypoint *pWpt, int iTeam)
	{
		return true; // okay -- no teams!!
	}
// linux fix
	void setup ( const char *szModFolder, eModId iModId, eBotType iBotType, const char *szWeaponListName );

	bool isModFolder ( char *szModFolder );

	char *getModFolder ();

	virtual const char *getPlayerClass ()
	{
		return "CBasePlayer";
	}

	eModId getModId ();

	virtual bool isAreaOwnedByTeam (int iArea, int iTeam) { return (iArea == 0); }

	eBotType getBotType () { return m_iBotType; }

	virtual void addWaypointFlags (edict_t *pPlayer, edict_t *pEdict, int *iFlags, int *iArea, float *fMaxDistance ){ return; }

////////////////////////////////
	virtual void initMod ();

	virtual void mapInit ();

	virtual bool playerSpawned ( edict_t *pPlayer );

	virtual void clientCommand ( edict_t *pEntity, int argc,const char *pcmd, const char *arg1, const char *arg2 ) {};

	virtual void modFrame () { };

	virtual void freeMemory() {};

	virtual bool isWaypointAreaValid ( int iWptArea, int iWptFlags ) { return true; }

	virtual void getTeamOnlyWaypointFlags ( int iTeam, int *iOn, int *iOff )
	{
		*iOn = 0;
		*iOff = 0;
	}

	inline bool needResetCheatFlag ()
	{
		return m_bBotCommand_ResetCheatFlag;
	}
private:
	char *m_szModFolder;
	char *m_szSteamFolder;
	eModId m_iModId;
	eBotType m_iBotType;
protected:
	char *m_szWeaponListName;
	bool m_bPlayerHasSpawned;
	bool m_bBotCommand_ResetCheatFlag;
};

///////////////////
/*
class CDODFlag
{
public:
	CDODFlag()
	{
		m_pEdict = NULL;
		m_iId = -1;
	}

	void setup (edict_t *pEdict, int id)
	{
		m_pEdict = pEdict;
		m_iId = id;
	}

	inline bool isFlag ( edict_t *pEdict ) { return m_pEdict == pEdict; }

	void update ();

private:
	edict_t *m_pEdict;
	int m_iId;
};
*/
#define MAX_DOD_FLAGS 8

class CDODFlags
{
public:
	CDODFlags()
	{
		init();
	}

	void init ()
	{
		m_iNumControlPoints = 0;
		m_vCPPositions = NULL;

		m_iAlliesReqCappers = NULL;
		m_iAxisReqCappers = NULL;
		m_iNumAllies = NULL;
		m_iNumAxis = NULL;
		m_iOwner = NULL;
		m_bBombPlanted_Unreliable = NULL;
		m_iBombsRequired = NULL;
		m_iBombsRemaining = NULL;
		m_bBombBeingDefused = NULL;
		m_iNumAxisBombsOnMap = 0;
		m_iNumAlliesBombsOnMap = 0;
		memset(m_bBombPlanted,0,sizeof(bool)*MAX_DOD_FLAGS);
		memset(m_pFlags,0,sizeof(edict_t*)*MAX_DOD_FLAGS);
		memset(m_pBombs,0,sizeof(edict_t*)*MAX_DOD_FLAGS*2);

		for ( short int i = 0; i < MAX_DOD_FLAGS; i ++ )
		{
			m_iWaypoint[i] = -1;
		}
	}

	int getNumFlags () { return m_iNumControlPoints; }
	int getNumFlagsOwned (int iTeam)
	{
		int count = 0;

		for ( short int i = 0; i < m_iNumControlPoints; i ++ )
		{
			if ( m_iOwner[i] == iTeam )
				count++;
		}

		return count;
	}

	int setup (edict_t *pResourceEntity);

	bool getRandomEnemyControlledFlag ( CBot *pBot, Vector *position, int iTeam, int *id = NULL );
	bool getRandomTeamControlledFlag ( CBot *pBot, Vector *position, int iTeam, int *id = NULL );

	bool getRandomBombToDefuse ( Vector *position, int iTeam, edict_t **pBombTarget, int *id = NULL );
	bool getRandomBombToPlant ( CBot *pBot, Vector *position, int iTeam, edict_t **pBombTarget, int *id = NULL );
	bool getRandomBombToDefend ( CBot *pBot, Vector *position, int iTeam, edict_t **pBombTarget, int *id = NULL );

	int findNearestObjective ( Vector vOrigin );

	inline int getWaypointAtFlag ( int iFlagId )
	{
		return m_iWaypoint[iFlagId];
	}

	inline int getNumBombsToDefend ( int iTeam )
	{
		int count = 0;

		for ( short int i = 0; i < m_iNumControlPoints; i ++ )
		{
			if ( canDefendBomb(iTeam,i) )
				count++;
		}

		return count;
	}

	inline int getNumBombsToDefuse ( int iTeam )
	{
		int count = 0;

		for ( short int i = 0; i < m_iNumControlPoints; i ++ )
		{
			if ( canDefuseBomb(iTeam,i) )
				count++;
		}

		return count;
	}

	inline int getNumPlantableBombs (int iTeam)
	{
		int count = 0;

		for ( short int i = 0; i < m_iNumControlPoints; i ++ )
		{
			if ( canPlantBomb(iTeam,i) )
				count += getNumBombsRequired(i);
		}

		return count;
	}

	inline float isBombExplodeImminent ( int id )
	{
		return (engine->Time() - m_fBombPlantedTime[id]) > DOD_BOMB_EXPLODE_IMMINENT_TIME;
	}

	inline void setBombPlanted ( int id, bool val )
	{
		m_bBombPlanted[id] = val;

		if ( val )
			m_fBombPlantedTime[id] = engine->Time();
		else
			m_fBombPlantedTime[id] = 0;
	}

	inline int getNumBombsToPlant ( int iTeam)
	{
		int count = 0;

		for ( short int i = 0; i < m_iNumControlPoints; i ++ )
		{
			if ( canPlantBomb(iTeam,i) )
				count += getNumBombsRemaining(i);
		}

		return count;
	}

	inline bool ownsFlag ( edict_t *pFlag, int iTeam ) { return ownsFlag(getFlagID(pFlag),iTeam); }
	inline bool ownsFlag ( int iFlag, int iTeam )
	{
		if ( iFlag == -1 )
			return false;

		return m_iOwner[iFlag] == iTeam;
	}

	inline int numFlagsOwned (int iTeam)
	{
		int count = 0;

		for ( short int i = 0; i < m_iNumControlPoints; i ++ )
		{
			if ( m_iOwner[i] == iTeam )
				count++;
		}

		return count;
	}

	inline int numCappersRequired ( edict_t *pFlag, int iTeam ) { return numCappersRequired(getFlagID(pFlag),iTeam); }
	inline int numCappersRequired ( int iFlag, int iTeam )
	{
		if ( iFlag == -1 )
			return 0;

		return (iTeam == TEAM_ALLIES) ? (m_iAlliesReqCappers[iFlag]) : (m_iAxisReqCappers[iFlag]);
	}

	inline bool isBombPlanted ( int iId )
	{
		if ( iId == -1 )
			return false;

		return m_bBombPlanted[iId];
	}

	inline bool isBombPlanted ( edict_t *pBomb )
	{
		return isBombPlanted(getBombID(pBomb));
	}

	inline bool canDefendBomb ( int iTeam, int iId )
	{
		return ((m_pBombs[iId][0]!=NULL)&&(m_iOwner[iId]!=iTeam) && isBombPlanted(iId));
	}

	inline bool canDefuseBomb ( int iTeam, int iId )
	{
		return ((m_pBombs[iId][0]!=NULL)&&(m_iOwner[iId]==iTeam) && isBombPlanted(iId));
	}

	inline bool canPlantBomb ( int iTeam, int iId )
	{
		return ((m_pBombs[iId][0]!=NULL)&&(m_iOwner[iId]!=iTeam) && !isBombPlanted(iId));
	}

	bool isTeamMateDefusing ( edict_t *pIgnore, int iTeam, int id );
	bool isTeamMatePlanting ( edict_t *pIgnore, int iTeam, int id );

	bool isTeamMateDefusing ( edict_t *pIgnore, int iTeam, Vector vOrigin );
	bool isTeamMatePlanting ( edict_t *pIgnore, int iTeam, Vector vOrigin );

	inline int getNumBombsRequired ( int iId )
	{
		if ( iId == -1 )
			return false;

		return m_iBombsRequired[iId];
	}

	inline int getNumBombsRequired ( edict_t *pBomb )
	{
		return getNumBombsRequired(getBombID(pBomb));
	}

	inline int getNumBombsRemaining ( int iId )
	{
		if ( iId == -1 )
			return false;

		return m_iBombsRemaining[iId];
	}

	inline int getNumBombsRemaining ( edict_t *pBomb )
	{
		return getNumBombsRemaining(getBombID(pBomb));
	}

	inline bool isBombBeingDefused ( int iId )
	{
		if ( iId == -1 )
			return false;

		return m_bBombBeingDefused[iId];
	}

	inline bool isBombBeingDefused ( edict_t *pBomb )
	{
		return isBombBeingDefused(getBombID(pBomb));
	}

	inline int numEnemiesAtCap ( edict_t *pFlag, int iTeam ) { return numEnemiesAtCap(getFlagID(pFlag),iTeam); }

	inline int numFriendliesAtCap ( edict_t *pFlag, int iTeam ) { return numFriendliesAtCap(getFlagID(pFlag),iTeam); }

	inline int numFriendliesAtCap ( int iFlag, int iTeam )
	{
		if ( iFlag == -1 )
			return 0;

		return (iTeam == TEAM_ALLIES) ? (m_iNumAllies[iFlag]) : (m_iNumAxis[iFlag]);
	}

	inline int numEnemiesAtCap ( int iFlag, int iTeam )
	{
		if ( iFlag == -1 )
			return 0;

		return (iTeam == TEAM_ALLIES) ? (m_iNumAxis[iFlag]) : (m_iNumAllies[iFlag]);
	}

	inline edict_t *getFlagByID ( int id )
	{
		if ( (id >= 0) && (id < m_iNumControlPoints) )
			return m_pFlags[id];

		return NULL;
	}

	inline int getFlagID ( edict_t *pent )
	{
		for ( short int i = 0; i < m_iNumControlPoints; i ++ )
		{
			if ( m_pFlags[i] == pent )
				return i;
		}

		return -1;
	}

	inline int getBombID ( edict_t *pent )
	{
		if ( pent == NULL )
			return -1;

		for ( short int i = 0; i < m_iNumControlPoints; i ++ )
		{
			if ( (m_pBombs[i][0] == pent) || (m_pBombs[i][1] == pent) )
				return i;
		}

		return -1;
	}

	inline bool isFlag ( edict_t *pent )
	{
		return getFlagID(pent) != -1;
	}

	inline bool isBomb ( edict_t *pent )
	{
		return getBombID(pent) != -1;
	}

	inline int getNumBombsOnMap ( int iTeam )
	{
		if ( iTeam == TEAM_ALLIES )
			return m_iNumAlliesBombsOnMap;
		return m_iNumAxisBombsOnMap;
	}

	inline void reset ()
	{
		// time up
		m_iNumControlPoints = 0;
	}

private:
	edict_t *m_pFlags[MAX_DOD_FLAGS];
	edict_t *m_pBombs[MAX_DOD_FLAGS][2]; // maximum of 2 bombs per capture point
	int m_iWaypoint[MAX_DOD_FLAGS];

	int m_iNumControlPoints;
	Vector *m_vCPPositions;

	int *m_iAlliesReqCappers;
	int *m_iAxisReqCappers;
	int *m_iNumAllies;
	int *m_iNumAxis;
	int *m_iOwner;

	// reply on this one
	bool *m_bBombPlanted_Unreliable;
    bool m_bBombPlanted[MAX_DOD_FLAGS];
	float m_fBombPlantedTime[MAX_DOD_FLAGS];
	int *m_iBombsRequired;
	int *m_iBombsRemaining;
	bool *m_bBombBeingDefused;
	int m_iNumAlliesBombsOnMap;
	int m_iNumAxisBombsOnMap;
};

class CDODMod : public CBotMod
{
public:
	CDODMod()
	{
		setup("dod",MOD_DOD,BOTTYPE_DOD,"DOD");
	}

	static void roundStart ();

	bool checkWaypointForTeam(CWaypoint *pWpt, int iTeam);
	
	static int numClassOnTeam( int iTeam, int iClass );

	static int getScore(edict_t *pPlayer);

	static int getHighestScore ();

	void clientCommand ( edict_t *pEntity, int argc, const char *pcmd, const char *arg1, const char *arg2 );

	static float getMapStartTime ();

	inline static bool isBombMap () { return (m_iMapType & DOD_MAPTYPE_BOMB) == DOD_MAPTYPE_BOMB; }
	inline static bool isFlagMap () { return (m_iMapType & DOD_MAPTYPE_FLAG) == DOD_MAPTYPE_FLAG; }
	inline static bool mapHasBombs () { return (m_iMapType & DOD_MAPTYPE_BOMB) == DOD_MAPTYPE_BOMB; }

	inline static bool isCommunalBombPoint () { return m_bCommunalBombPoint; }
	inline static int getBombPointArea (int iTeam) { if ( iTeam == TEAM_ALLIES ) return m_iBombAreaAllies; return m_iBombAreaAxis; } 

	void addWaypointFlags (edict_t *pPlayer, edict_t *pEdict, int *iFlags, int *iArea, float *fMaxDistance );

	static CDODFlags m_Flags;

	static bool shouldAttack ( int iTeam ); // uses the neural net to return probability of attack

	static edict_t *getBombTarget ( CWaypoint *pWpt );
	static edict_t *getBreakable ( CWaypoint *pWpt );

	void getTeamOnlyWaypointFlags ( int iTeam, int *iOn, int *iOff );

	static bool isBreakableRegistered ( edict_t *pBreakable, int iTeam );

	static inline CWaypoint *getBombWaypoint ( edict_t *pBomb )
	{
		for ( unsigned int i = 0; i < m_BombWaypoints.size(); i ++ )
		{
			if ( m_BombWaypoints[i].pEdict == pBomb )
				return m_BombWaypoints[i].pWaypoint;
		}

		return NULL;
	}

	static inline bool isPathBomb ( edict_t *pBomb )
	{
		for ( unsigned int i = 0; i < m_BombWaypoints.size(); i ++ )
		{
			if ( m_BombWaypoints[i].pEdict == pBomb )
				return true;
		}

		return false;
	}

	// for getting the ground of bomb to open waypoints
	// the ground might change
	static Vector getGround ( CWaypoint *pWaypoint );

	//to do for snipers and machine gunners
	/*static unsigned short int getNumberOfClassOnTeam ( int iClass );
	static unsigned short int getNumberOfPlayersOnTeam ( int iClass );*/

protected:

	void initMod ();

	void mapInit ();

	void modFrame ();

	void freeMemory ();

	static edict_t *m_pResourceEntity;
	static edict_t *m_pPlayerResourceEntity;
	static edict_t *m_pGameRules;
	static float m_fMapStartTime;
	static int m_iMapType;
	static bool m_bCommunalBombPoint; // only one bomb suuply point for both teams
	static int m_iBombAreaAllies;
	static int m_iBombAreaAxis;

	static std::vector<edict_wpt_pair_t> m_BombWaypoints;
	static std::vector<edict_wpt_pair_t> m_BreakableWaypoints;

									// enemy			// team
	static float fAttackProbLookUp[MAX_DOD_FLAGS+1][MAX_DOD_FLAGS+1];
};

class CCounterStrikeSourceMod : public CBotMod
{
public:
	CCounterStrikeSourceMod()
	{
		setup("cstrike", MOD_CSS, BOTTYPE_CSS, "CSS");
	}

	//void initMod ();

	//void mapInit ();

	//void entitySpawn ( edict_t *pEntity );
protected:
	// storing mod specific info
	std::vector<edict_t*> m_pHostages;
	std::vector<edict_t*> m_pBombPoints;
	std::vector<edict_t*> m_pRescuePoints;
};

class CTimCoopMod : public CBotMod
{
public:
	CTimCoopMod()
	{
		setup("SourceMods",MOD_TIMCOOP,BOTTYPE_COOP,"HL2DM");
	}

	//void initMod ();

	//void mapInit ();

	//void entitySpawn ( edict_t *pEntity );
};

class CSvenCoop2Mod : public CBotMod
{
public:
	CSvenCoop2Mod()
	{
		setup("SourceMods",MOD_SVENCOOP2,BOTTYPE_COOP,"SVENCOOP2");
	}

	//void initMod ();

	//void mapInit ();

	//void entitySpawn ( edict_t *pEntity );
};

class CFortressForeverMod : public CBotMod
{
public:
	CFortressForeverMod()
	{
		setup("FortressForever", MOD_FF, BOTTYPE_FF, "FF");
	}
private:

};

class CHLDMSourceMod : public CBotMod
{
public:
	CHLDMSourceMod()
	{
		setup("hl1mp",MOD_HL1DMSRC,BOTTYPE_HL1DM,"HLDMSRC");
	}
};

class CSynergyMod : public CBotMod
{
public:
	CSynergyMod()
	{
		setup("synergy",MOD_SYNERGY,BOTTYPE_COOP,"SYNERGY");
	}

	//void initMod ();

	//void mapInit ();

	//void entitySpawn ( edict_t *pEntity );
};

#define NEWENUM typedef enum {

typedef enum
{
	TF_MAP_DM = 0,
	TF_MAP_CTF,
	TF_MAP_CP,
	TF_MAP_TC,
	TF_MAP_CART,
	TF_MAP_CARTRACE,
	TF_MAP_ARENA,
	TF_MAP_KOTH,
	TF_MAP_SD, // special delivery : added 15 jul 12
	TF_MAP_TR,
	TF_MAP_MVM,
	TF_MAP_RD,
	TF_MAP_BUMPERCARS,
	TF_MAP_MAX
}eTFMapType;

// These must be MyEHandles because they may be destroyed at any time
typedef struct
{
	MyEHandle entrance;
	MyEHandle exit;
	MyEHandle sapper;
	float m_fLastTeleported;
	int m_iWaypoint;
//	short builder;
}tf_tele_t;

typedef struct
{
	MyEHandle sentry;
	MyEHandle sapper;
//	short builder;
}tf_sentry_t;

typedef struct
{
	MyEHandle disp;
	MyEHandle sapper;
//	short builder;
}tf_disp_t;


class CTeamControlPointRound;
class CTeamControlPointMaster;
class CTeamControlPoint;
class CTeamRoundTimer;

class CTeamFortress2Mod : public CBotMod
{
public:
	CTeamFortress2Mod()
	{
		setup("tf",MOD_TF2,BOTTYPE_TF2,"TF2");

		m_pResourceEntity = NULL;
	}

	void mapInit ();

	void modFrame ();

	bool isAreaOwnedByTeam (int iArea, int iTeam);

	static void updatePointMaster ();

	void clientCommand ( edict_t *pEntity, int argc, const char *pcmd, const char *arg1, const char *arg2 );

	virtual const char *getPlayerClass ()
	{
		return "CTFPlayer";
	}

	void initMod ();

	static void roundStart ();

	static int getTeam ( edict_t *pEntity );

	static TF_Class getSpyDisguise ( edict_t *pPlayer );

	static int getSentryLevel ( edict_t *pSentry );
	static int getDispenserLevel ( edict_t *pDispenser );

	static bool isDispenser ( edict_t *pEntity, int iTeam, bool checkcarrying = false );

	static bool isPayloadBomb ( edict_t *pEntity, int iTeam );

	static int getTeleporterWaypoint ( edict_t *pTele );

	bool isWaypointAreaValid ( int iWptArea, int iWptFlags );

	static bool isSuddenDeath(void);

	static bool isHealthKit ( edict_t *pEntity );

	static bool isAmmo ( edict_t *pEntity );

	static int getArea (); // get current area of map

	static void setArea ( int area ) { m_iArea = area; }

	static bool isSentry ( edict_t *pEntity, int iTeam, bool checkcarrying = false );
	static bool isTankBoss(edict_t *pEntity);
	static void checkMVMTankBoss(edict_t *pEntity);
	static bool isTeleporter ( edict_t *pEntity, int iTeam, bool checkcarrying = false );

	static void updateTeleportTime ( edict_t *pOwner );
	static float getTeleportTime ( edict_t *pOwner );

	static bool isTeleporterEntrance ( edict_t *pEntity, int iTeam, bool checkcarrying = false );

	static bool isTeleporterExit ( edict_t *pEntity, int iTeam, bool checkcarrying = false );

	static inline bool isMapType ( eTFMapType iMapType ) { return iMapType == m_MapType; }

	static bool isFlag ( edict_t *pEntity, int iTeam );

	static bool withinEndOfRound ( float fTime );

	static bool isPipeBomb ( edict_t *pEntity, int iTeam);

	static bool isHurtfulPipeGrenade ( edict_t *pEntity, edict_t *pPlayer, bool bCheckOwner = true );

	static bool isRocket ( edict_t *pEntity, int iTeam );

	static int getEnemyTeam ( int iTeam );

	static bool buildingNearby ( int iTeam, Vector vOrigin );

// Naris @ AlliedModders .net

	static bool TF2_IsPlayerZoomed(edict_t *pPlayer);

	static bool TF2_IsPlayerSlowed(edict_t *pPlayer);

	static bool TF2_IsPlayerDisguised(edict_t *pPlayer);

	static bool TF2_IsPlayerCloaked(edict_t *pPlayer);

	static bool TF2_IsPlayerInvuln(edict_t *pPlayer);

	static bool TF2_IsPlayerKrits(edict_t *pPlayer);

	static bool TF2_IsPlayerOnFire(edict_t *pPlayer);

	static bool TF2_IsPlayerTaunting(edict_t *pPlayer);

	static float TF2_GetPlayerSpeed(edict_t *pPlayer, TF_Class iClass );

	static void teleporterBuilt ( edict_t *pOwner, eEngiBuild type, edict_t *pBuilding );

	static edict_t *getTeleporterExit ( edict_t *pTele );

	static void setPointOpenTime ( int time );

	static void setSetupTime ( int time );

	static void resetSetupTime ();

	static bool isArenaPointOpen ();

	static bool hasRoundStarted ();

	static int getHighestScore ();

	static edict_t *nearestDispenser ( Vector vOrigin, int team );

	static void flagPickedUp (int iTeam, edict_t *pPlayer);
	static void flagReturned (int iTeam);

	static void setAttackDefendMap ( bool bSet ) { m_bAttackDefendMap = bSet; }
	static bool isAttackDefendMap () { return m_bAttackDefendMap; }

	void addWaypointFlags (edict_t *pPlayer, edict_t *pEdict, int *iFlags, int *iArea, float *fMaxDistance );

	void getTeamOnlyWaypointFlags ( int iTeam, int *iOn, int *iOff );

	static bool getFlagLocation ( int iTeam, Vector *vec );

	static bool getDroppedFlagLocation ( int iTeam, Vector *vec )
	{
		if ( iTeam == TF2_TEAM_BLUE )
		{
			*vec = m_vFlagLocationBlue;
			return m_bFlagLocationValidBlue;
		}
		else if ( iTeam == TF2_TEAM_RED )
		{
			*vec = m_vFlagLocationRed;
			return m_bFlagLocationValidRed;
		}

		return false;
	}

	static void flagDropped (int iTeam, Vector vLoc)
	{
		if ( iTeam == TF2_TEAM_BLUE )
		{
			m_pFlagCarrierBlue = NULL;
			m_vFlagLocationBlue = vLoc;
			m_bFlagLocationValidBlue = true;
		}
		else if ( iTeam == TF2_TEAM_RED )
		{
			m_pFlagCarrierRed = NULL;
			m_vFlagLocationRed = vLoc;
			m_bFlagLocationValidRed = true;
		}

		m_iFlagCarrierTeam = iTeam;
	}

	static void roundStarted ()
	{
		m_bHasRoundStarted = true;
	    m_bRoundOver = false;
		m_iWinningTeam = 0; 
	}

	static void roundWon ( int iWinningTeam )
	{
		m_bHasRoundStarted = false;
		m_bRoundOver = true;
		m_iWinningTeam = iWinningTeam;
		m_iLastWinningTeam = m_iWinningTeam;
	}

	static inline bool wonLastRound(int iTeam)
	{
		return m_iLastWinningTeam == iTeam;
	}

	static inline bool isLosingTeam ( int iTeam )
	{
		return !m_bHasRoundStarted && m_bRoundOver && m_iWinningTeam && (m_iWinningTeam != iTeam); 
	}

	static void roundReset ();

	static inline bool isFlagCarrier (edict_t *pPlayer)
	{
		return (m_pFlagCarrierBlue==pPlayer)||(m_pFlagCarrierRed==pPlayer);
	}

	static inline edict_t *getFlagCarrier (int iTeam)
	{
		if ( iTeam == TF2_TEAM_BLUE )
			return m_pFlagCarrierBlue;
		else if ( iTeam == TF2_TEAM_RED )
			return m_pFlagCarrierRed;

		return NULL;
	}

	static bool isFlagCarried (int iTeam)
	{
		if ( iTeam == TF2_TEAM_BLUE )
			return (m_pFlagCarrierBlue != NULL);
		else if ( iTeam == TF2_TEAM_RED )
			return (m_pFlagCarrierRed != NULL);

		return false;
	}

	static void sapperPlaced(edict_t *pOwner,eEngiBuild type,edict_t *pSapper);
	static void sapperDestroyed(edict_t *pOwner,eEngiBuild type,edict_t *pSapper);
	static void sentryBuilt(edict_t *pOwner, eEngiBuild type, edict_t *pBuilding);
	static void dispenserBuilt(edict_t *pOwner, eEngiBuild type, edict_t *pBuilding);

	static CWaypoint *getBestWaypointMVM ( CBot *pBot, int iFlags );

	static edict_t *getMySentryGun ( edict_t *pOwner )
	{
		const int id = ENTINDEX(pOwner)-1;

		if ( id>=0 )
		{
			return m_SentryGuns[id].sentry.get();
		}

		return NULL;
	}

	static edict_t *getSentryOwner ( edict_t *pSentry )
	{
		//for ( short int i = 1; i <= gpGlobals->maxClients; i ++ )
		for ( short int i = 0; i < MAX_PLAYERS; i ++ )
		{			
			if ( m_SentryGuns[i].sentry.get() == pSentry )
				return INDEXENT(i+1);
		}

		return NULL;
	}

	static bool isMySentrySapped ( edict_t *pOwner ) 
	{
		const int id = ENTINDEX(pOwner)-1;

		if ( id>=0 )
		{
			return (m_SentryGuns[id].sentry.get()!=NULL)&&(m_SentryGuns[id].sapper.get()!=NULL);
		}

		return false;
	}

	static edict_t *getSentryGun ( int id )
	{
		return m_SentryGuns[id].sentry.get();
	}

	static edict_t *getTeleEntrance ( int id )
	{
		return m_Teleporters[id].entrance.get();
	}

	static bool isMyTeleporterSapped ( edict_t *pOwner )
	{
		const int id = ENTINDEX(pOwner)-1;

		if ( id>=0 )
		{
			return ((m_Teleporters[id].exit.get()!=NULL)||(m_Teleporters[id].entrance.get()!=NULL))&&(m_Teleporters[id].sapper.get()!=NULL);
		}

		return false;
	}

	static bool isMyDispenserSapped ( edict_t *pOwner )
	{
		const int id = ENTINDEX(pOwner)-1;

		if ( id>=0 )
		{
			return (m_Dispensers[id].disp.get()!=NULL)&&(m_Dispensers[id].sapper.get()!=NULL);
		}

		return false;
	}

	static bool isSentrySapped ( edict_t *pSentry )
	{
		unsigned int i;

		for ( i = 0; i < MAX_PLAYERS; i ++ )
		{
			if ( m_SentryGuns[i].sentry.get() == pSentry )
				return m_SentryGuns[i].sapper.get()!=NULL;
		}

		return false;
	}

	static bool isTeleporterSapped ( edict_t *pTele )
	{
		unsigned int i;

		for ( i = 0; i < MAX_PLAYERS; i ++ )
		{
			if ( (m_Teleporters[i].entrance.get() == pTele) || (m_Teleporters[i].exit.get() == pTele) )
				return m_Teleporters[i].sapper.get()!=NULL;
		}

		return false;
	}

	static bool isDispenserSapped ( edict_t *pDisp )
	{
		unsigned int i;

		for ( i = 0; i < MAX_PLAYERS; i ++ )
		{
			if ( m_Dispensers[i].disp.get() == pDisp )
				return m_Dispensers[i].sapper.get()!=NULL;
		}

		return false;
	}

	static edict_t *findResourceEntity ();

	static void addCapDefender ( edict_t *pPlayer, int iCapIndex )
	{
		m_iCapDefenders[iCapIndex] |= (1<<(ENTINDEX(pPlayer)-1));
	}

	static void removeCapDefender ( edict_t *pPlayer, int iCapIndex )
	{
		m_iCapDefenders[iCapIndex] &= ~(1<<(ENTINDEX(pPlayer)-1));
	}

	static void resetDefenders ()
	{
		memset(m_iCapDefenders,0,sizeof(int)*MAX_CONTROL_POINTS);
	}

	static bool isDefending ( edict_t *pPlayer );//, int iCapIndex = -1 );

	static bool isCapping ( edict_t *pPlayer );//, int iCapIndex = -1 );
	
	static void addCapper ( int cp, int capper )
	{
		if ( capper && (cp < MAX_CAP_POINTS) )
			m_Cappers[cp] |= (1<<(capper-1));
	}

	static void removeCappers ( int cp )
	{
		m_Cappers[cp] = 0;
	}

	static void resetCappers ()
	{
		memset(m_Cappers,0,sizeof(int)*MAX_CONTROL_POINTS);
	}

	static int numPlayersOnTeam ( int iTeam, bool bAliveOnly = false );
	static int numClassOnTeam ( int iTeam, int iClass );

	static int getFlagCarrierTeam () { return m_iFlagCarrierTeam; }
	static bool canTeamPickupFlag_SD(int iTeam,bool bGetUnknown);

	static edict_t *getBuildingOwner (eEngiBuild object, short index);
	static edict_t *getBuilding (eEngiBuild object, edict_t *pOwner);

	static bool isBoss ( edict_t *pEntity, float *fFactor = NULL );

	static void initBoss ( bool bSummoned ) { m_bBossSummoned = bSummoned; m_pBoss = NULL; }

	static bool isBossSummoned () { return m_bBossSummoned; }

	static bool isSentryGun ( edict_t *pEdict );

	static edict_t *getMediGun ( edict_t *pPlayer );

	static void findMediGun ( edict_t *pPlayer );


	bool checkWaypointForTeam(CWaypoint *pWpt, int iTeam);
	

	static bool isFlagAtDefaultState () { return bFlagStateDefault; }
	static void resetFlagStateToDefault() { bFlagStateDefault = true; }
	static void setDontClearPoints ( bool bClear ) { m_bDontClearPoints = bClear; }
	static bool dontClearPoints () { return m_bDontClearPoints; }
	static CTFObjectiveResource m_ObjectiveResource;

	static CTeamControlPointRound *getCurrentRound() { return m_pCurrentRound; }

	static CTeamControlPointMaster *getPointMaster () { return m_PointMaster;}

	static void updateRedPayloadBomb ( edict_t *pent );
	static void updateBluePayloadBomb ( edict_t *pent ); 

	static edict_t *getPayloadBomb ( int team );

	static void MVMAlarmSounded () { m_bMVMAlarmSounded = true; }
	static void MVMAlarmReset () { m_bMVMAlarmSounded = false; }
	static float getMVMCapturePointRadius ( )
	{
		return m_fMVMCapturePointRadius;
	}
	static bool getMVMCapturePoint ( Vector *vec )
	{
		if ( m_bMVMCapturePointValid )
		{
			*vec = m_vMVMCapturePoint;
			return true;
		}

		return ( getFlagLocation(TF2_TEAM_BLUE,vec) );
	}

	static bool isMedievalMode();

private:


	static float TF2_GetClassSpeed(int iClass);

	static CTeamControlPointMaster *m_PointMaster;
	static CTeamControlPointRound *m_pCurrentRound;
	static MyEHandle m_PointMasterResource;
	static CTeamRoundTimer m_Timer;

	static eTFMapType m_MapType;	

	static MyEHandle m_pPayLoadBombRed;
	static MyEHandle m_pPayLoadBombBlue;

	static tf_tele_t m_Teleporters[MAX_PLAYERS];	// used to let bots know who made a teleport ans where it goes
	static tf_sentry_t m_SentryGuns[MAX_PLAYERS];	// used to let bots know if sentries have been sapped or not
	static tf_disp_t  m_Dispensers[MAX_PLAYERS];	// used to let bots know where friendly/enemy dispensers are

	static int m_iArea;

	static float m_fSetupTime;

	static float m_fRoundTime;

	static MyEHandle m_pFlagCarrierRed;
	static MyEHandle m_pFlagCarrierBlue;

	static float m_fPointTime;
	static float m_fArenaPointOpenTime;

	static MyEHandle m_pResourceEntity;
	static MyEHandle m_pGameRules;
	static bool m_bAttackDefendMap;

	static int m_Cappers[MAX_CONTROL_POINTS];
	static int m_iCapDefenders[MAX_CONTROL_POINTS];

	static bool m_bHasRoundStarted;

	static int m_iFlagCarrierTeam;
	static MyEHandle m_pBoss;
	static bool m_bBossSummoned;
	static bool bFlagStateDefault;

	static MyEHandle pMediGuns[MAX_PLAYERS];
	static bool m_bDontClearPoints;

	static bool m_bRoundOver;
	static int m_iWinningTeam;
	static int m_iLastWinningTeam;
	static Vector m_vFlagLocationBlue;
	static bool m_bFlagLocationValidBlue;
	static Vector m_vFlagLocationRed;
	static bool m_bFlagLocationValidRed;


	static bool m_bMVMFlagStartValid;
	static Vector m_vMVMFlagStart;
	static bool m_bMVMCapturePointValid;
	static Vector m_vMVMCapturePoint;
	static bool m_bMVMAlarmSounded;
	static float m_fMVMCapturePointRadius;
	static int m_iCapturePointWptID;
	static int m_iFlagPointWptID;

	static MyEHandle m_pNearestTankBoss;
	static float m_fNearestTankDistance;
	static Vector m_vNearestTankLocation;

};

class CHalfLifeDeathmatchMod : public CBotMod
{
public:
	CHalfLifeDeathmatchMod()
	{
		setup("hl2mp", MOD_HLDM2, BOTTYPE_HL2DM, "HL2DM");
	}

	void initMod ();

	void mapInit ();

	bool playerSpawned ( edict_t *pPlayer );

	static inline edict_t *getButtonAtWaypoint ( CWaypoint *pWaypoint )
	{
		for ( unsigned int i = 0; i < m_LiftWaypoints.size(); i ++ )
		{
			if ( m_LiftWaypoints[i].pWaypoint == pWaypoint )
				return m_LiftWaypoints[i].pEdict;
		}

		return NULL;
	}

	//void entitySpawn ( edict_t *pEntity );
private:
	static std::vector<edict_wpt_pair_t> m_LiftWaypoints;
};

/*
class CNaturalSelection2Mod : public CBotMod
{
public:
	CNaturalSelection2Mod() 
	{
		setup("ns2",MOD_NS2,BOTTYPE_NS2);
	}
// linux fix

	virtual const char *getPlayerClass ()
	{
		return "CBaseNS2Player";
	}

	virtual bool isAreaOwnedByTeam (int iArea, int iTeam) { return (iArea == 0); }

	virtual void addWaypointFlags (edict_t *pPlayer, edict_t *pEdict, int *iFlags, int *iArea, float *fMaxDistance ){ return; }

////////////////////////////////
	virtual void initMod ();

	virtual void mapInit ();

	virtual bool playerSpawned ( edict_t *pPlayer );

	virtual void clientCommand ( edict_t *pEntity, int argc,const char *pcmd, const char *arg1, const char *arg2 ) {};

	virtual void modFrame () { };

	virtual void freeMemory() {};

	virtual bool isWaypointAreaValid ( int iWptArea, int iWptFlags ) { return true; }

	virtual void getTeamOnlyWaypointFlags ( int iTeam, int *iOn, int *iOff )
	{
		*iOn = 0;
		*iOff = 0;
	}
};
*/

class CBotMods
{
public:

	static void parseFile ();

	static void createFile ();

	static void readMods();

	static void freeMemory ();

	static CBotMod *getMod ( char *szModFolder );

private:
	static std::vector<CBotMod*> m_Mods;
};

#endif
