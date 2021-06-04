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
#ifndef __RCBOT_WAYPOINT_H__
#define __RCBOT_WAYPOINT_H__

#include <cstdio>

// this must be before bot_client.h to avoid unknown override / missing type warnings
#include <vector>
using WaypointList = std::vector<int>;

#include "bot.h"
#include "bot_client.h"
#include "bot_wpt_color.h"
#include "bot_mtrand.h"

//#include "bot_navigator.h"

class CWaypointVisibilityTable;
class CClient;


class CWaypointAuthorInfo
{
public:
	char szAuthor[32];
	char szModifiedBy[32];
};

class CWaypointHeader
{
public:
	char szFileType[16];
	char szMapName[64];
	int iVersion;
	int iNumWaypoints;
	int iFlags;
};

typedef struct
{
	MyEHandle pEdict; // MyEHandle fixes problems with reused edict slots
	CWaypoint *pWaypoint;
	Vector v_ground;
}edict_wpt_pair_t;

enum
{
 DRAWTYPE_EFFECTS = 0,
 DRAWTYPE_DEBUGENGINE,
 DRAWTYPE_DEBUGENGINE2,
 DRAWTYPE_DEBUGENGINE3,
 DRAWTYPE_BELIEF,
 DRAWTYPE_MAX
};

class CWaypoint;


class CWaypointType
{
public:

	CWaypointType ( int iBit, const char *szName, const char *szDescription, WptColor vColour, int iModBits = BITS_MOD_ALL, int iImportance = 0 );

	inline const char *getName () { return m_szName; }
	inline const char *getDescription () { return m_szDescription; }

	inline bool isBitsInFlags ( int iFlags ) { return (iFlags & m_iBit)==m_iBit; }
	inline int getBits () { return m_iBit; }
	inline void setMods ( int iMods ){ m_iMods = iMods; }// input bitmask of mods (32 max)
	inline bool forMod ( int iMod ) { return ((1<<iMod)&m_iMods)==(1<<iMod); }
	inline WptColor getColour () { return m_vColour; }
	inline int getImportance () { return m_iImportance; }

	bool operator < ( CWaypointType *other )
	{
		return m_iImportance < other->getImportance();
	}

	//virtual void giveTypeToWaypoint ( CWaypoint *pWaypoint );
	//virtual void removeTypeFromWaypoint ( CWaypoint *pWaypoint );

private:
	int m_iMods; // mods allowed
	int m_iBit; // bits used
	char *m_szName; // e.g. "jump"/"ladder"
	char *m_szDescription; // e.g. "will jump here"/"will climb here"
	int m_iImportance;
	WptColor m_vColour;
};


/*
class CCrouchWaypointType : public CWaypointType
{
public:
	CCrouchWaypointType();
    
	void giveTypeToWaypoint ( CWaypoint *pWaypoint );
	void removeTypeFromWaypoint ( CWaypoint *pWaypoint );
};*/

class CWaypointTypes
{
public:

// if you're adding a new waypoint type, don't forget to update CWaypointTypes :: setup()
	static const int W_FL_NONE           = 0;
	static const int W_FL_JUMP           = (1 << 0);
	static const int W_FL_CROUCH         = (1 << 1);
	static const int W_FL_UNREACHABLE    = (1 << 2);
	static const int W_FL_LADDER         = (1 << 3);
	static const int W_FL_FLAG           = (1 << 4);
	static const int W_FL_CAPPOINT       = (1 << 5);
	static const int W_FL_NOBLU          = (1 << 6);
	static const int W_FL_NOAXIS         = (1 << 6);
	static const int W_FL_NORED          = (1 << 7);
	static const int W_FL_NOALLIES       = (1 << 7);
	static const int W_FL_HEALTH         = (1 << 8);
	static const int W_FL_OPENS_LATER    = (1 << 9);
	static const int W_FL_ROCKET_JUMP    = (1 << 10);
	static const int W_FL_BOMB_TO_OPEN   = (1 << 10); // DOD:S
	static const int W_FL_SNIPER         = (1 << 11);
	static const int W_FL_AMMO           = (1 << 12);
	static const int W_FL_RESUPPLY       = (1 << 13);
	static const int W_FL_BOMBS_HERE     = (1 << 13);
	static const int W_FL_SENTRY         = (1 << 14);
	static const int W_FL_MACHINEGUN     = (1 << 14);
	static const int W_FL_DOUBLEJUMP     = (1 << 15);
	static const int W_FL_PRONE          = (1 << 15);
	static const int W_FL_TELE_ENTRANCE  = (1 << 16);
	static const int W_FL_TELE_EXIT      = (1 << 17);
	static const int W_FL_DEFEND         = (1 << 18);
	static const int W_FL_AREAONLY       = (1 << 19);
	static const int W_FL_ROUTE          = (1 << 20);
	static const int W_FL_WAIT_GROUND    = (1 << 21);
	static const int W_FL_NO_FLAG        = (1 << 22);
	static const int W_FL_COVER_RELOAD   = (1 << 22); // DOD:S only
	static const int W_FL_LIFT           = (1 << 23);
	static const int W_FL_FLAGONLY       = (1 << 24);
	static const int W_FL_FALL           = (1 << 25);
	static const int W_FL_BREAKABLE      = (1 << 26);
	static const int W_FL_SPRINT         = (1 << 27);
	static const int W_FL_TELEPORT_CHEAT = (1 << 28); // teleports bots to the next waypoint (cheat)
	static const int W_FL_OWNER_ONLY     = (1 << 29); // Only owners of this area can use the waypoint
	//static const int W_FL_ATTACKPOINT = (1 << 30); // Tactical waypoint -- each squad will go to different attack points and signal others to go

	static void setup ();

	static void addType ( CWaypointType *type );

	static void printInfo ( CWaypoint *pWpt, edict_t *pPrintTo, float duration = 6.0f );

	static void displayTypesMenu ( edict_t *pPrintTo );
	
	static CWaypointType *getType( const char *szType );

	static void showTypesOnConsole( edict_t *pPrintTo );

	static void selectedType ( CClient *pClient );

	static void freeMemory ();

	static WptColor getColour ( int iFlags );

	static CWaypointType *getTypeByIndex ( unsigned int iIndex );

	static unsigned int getNumTypes ();

	static CWaypointType *getTypeByFlags ( int iFlags );

private:
	static std::vector<CWaypointType*> m_Types;
};

class CWaypointTest
{
public:
	void go ( edict_t *pPlayer );
};

typedef struct
{
	float fNextCheck;
	Vector vOrigin;
	bool bVisibleLastCheck;
}wpt_opens_later_t;

class CWaypoint //: public INavigatorNode
{
public:
	//static const int MAX_PATHS = 8;
	// Waypoint flags (up to 32)


	static const int WAYPOINT_HEIGHT = 72;
	static const int WAYPOINT_WIDTH = 8;
	static const int PATHWAYPOINT_WIDTH = 4;

	CWaypoint ()
	{
		m_thePaths.clear();
		init();
//		m_iId = -1;
	}

	CWaypoint ( Vector vOrigin, int iFlags = 0, int iYaw = 0, int m_fRadius = 0 )
	{
		m_thePaths.clear();
		init();
		m_iFlags = iFlags;
		m_vOrigin = vOrigin;		
		m_bUsed = true;
		setAim(iYaw);
		m_fNextCheckGroundTime = 0;
		m_bHasGround = false;
		m_fRadius = 0;
		m_OpensLaterInfo.clear();
		m_bIsReachable = true; 
		m_fCheckReachableTime = 0;
//		m_iId = iId;
	}

	bool checkGround ();

	inline void setAim ( int iYaw )
	{
		m_iAimYaw = iYaw;
	}

	inline float getAimYaw ()
	{
		return (float)m_iAimYaw;
	}

	inline Vector getOrigin ()
	{
		return m_vOrigin;
	}

	void init ();

	inline void addFlag ( int iFlag )
	{
		m_iFlags |= iFlag;
	}

	inline void removeFlag ( int iFlag )
	{
		m_iFlags &= ~iFlag;
	}

	// removes all waypoint flags
	inline void removeFlags ()
	{
		m_iFlags = 0;
	}

	inline bool hasFlag ( int iFlag )
	{
		return (m_iFlags & iFlag) == iFlag;
	}

	inline bool hasSomeFlags ( int iFlag )
	{
		return (m_iFlags & iFlag) > 0;
	}

	inline void move ( Vector origin )
	{
		// move to new origin
		m_vOrigin = origin;
	}

	void checkAreas ( edict_t *pActivator );

	// show info to player
	void info ( edict_t *pEdict );

	// methods
    void touched ();

	void draw ( edict_t *pEdict, bool bDrawPaths, unsigned short int iDrawType );

	bool addPathTo ( int iWaypointIndex );
	void removePathTo ( int iWaypointIndex );
	
	void addPathFrom ( int iWaypointIndex );
	void removePathFrom ( int iWaypointIndex );

	bool checkReachable ();

	bool isPathOpened ( Vector vPath );

	inline bool isUsed ()
	{
		return m_bUsed;
	}

	//bool touched ( edict_t *pEdict );
	bool touched ( Vector vOrigin, Vector vOffset, float fTouchDist, bool onground = true );

	void botTouch ( CBot *pBot );

	inline void freeMapMemory ()
	{
		m_thePaths.clear();
	}

	inline int getArea () { return m_iArea; }
	inline void setArea (int area) { m_iArea = area; }

	void drawPaths ( edict_t *pEdict, unsigned short int iDrawType );

	void drawPathBeam ( CWaypoint *to, unsigned short int iDrawType );

	inline void setUsed ( bool bUsed ){	m_bUsed = bUsed;}

	inline void clearPaths ();

	inline float distanceFrom ( CWaypoint *other )
	{
		return distanceFrom(other->getOrigin());
	}

	float distanceFrom ( Vector vOrigin );

	int numPaths ();

	int numPathsToThisWaypoint ();
	int getPathToThisWaypoint ( int i );

	int getPath ( int i );

	void load ( FILE *bfp, int iVersion );

	void save ( FILE *bfp );

	inline int getFlags (){return m_iFlags;}

	bool forTeam ( int iTeam );

	inline float getRadius () { return m_fRadius; }

	inline void setRadius ( float fRad ) { m_fRadius = fRad; }

	Vector applyRadius ();

	bool isAiming ( void );

private:
	Vector m_vOrigin;
	// aim of vector (used with certain waypoint types)
	int m_iAimYaw;
	int m_iFlags;
	int m_iArea;
	float m_fRadius;
	// not deleted
	bool m_bUsed;
	// paths to other waypoints
	WaypointList m_thePaths;
	// for W_FL_WAIT_GROUND waypoints
	float m_fNextCheckGroundTime;
	bool m_bHasGround;
	// Update m_iNumPathsTo (For display)
	bool m_bIsReachable; 
	float m_fCheckReachableTime;
	WaypointList m_PathsTo; // paths to this waypoint from other waypoints

	std::vector<wpt_opens_later_t> m_OpensLaterInfo;
};

class CWaypoints
{
public:
	static const int MAX_WAYPOINTS = 1024;
	static const int WAYPOINT_VERSION = 4; // waypoint version 4 add author information

	static const int W_FILE_FL_VISIBILITY = 1;

	static void init (const char *pszAuthor = NULL, const char *pszModifiedBy = NULL);

	static inline int getWaypointIndex ( CWaypoint *pWpt )
	{
		if ( pWpt == NULL )
			return -1;

		return ((int)pWpt - (int)m_theWaypoints)/sizeof(CWaypoint);
	}

	static void autoFix ( bool bAutoFixNonArea );

	static void checkAreas ( edict_t *pActivator );

	static void shiftVisibleAreas ( edict_t *pPlayer, int from, int to );

	static void drawWaypoints ( CClient *pClient );

	static int addWaypoint ( CClient *pClient, const char *type1, const char *type2,const char *type3,const char *type4, bool bUseTemplate = false );

	static int addWaypoint ( edict_t *pPlayer, Vector vOrigin, int iFlags = CWaypointTypes::W_FL_NONE, bool bAutoPath = false, int iYaw = 0, int iArea = 0, float fRadius = 0 );

	static void removeWaypoint ( int iIndex );

	static int numWaypoints ();

	static bool checkReachable ( CWaypoint *pWaypoint, int iStart );

	static CWaypoint *nearestPipeWaypoint ( Vector vTarget, Vector vOrigin, int *iAiming );

	static int freeWaypointIndex ();

	static void deletePathsTo ( int iWpt );
	static void deletePathsFrom ( int iWpt );

	static void shiftAreas (int val);

	static inline CWaypoint *getWaypoint ( int iIndex )
	{
		if ( !validWaypointIndex(iIndex) )
			return NULL;

		return &m_theWaypoints[iIndex];
	}

	static CWaypoint *getNextCoverPoint ( CBot *pBot, CWaypoint *pCurrent, CWaypoint *pBlocking );

	// save waypoints
	static bool save ( bool bVisiblityMade, edict_t *pPlayer = NULL, const char *pszAuthor = NULL, const char *pszModifier = NULL );
	// load waypoints
	static bool load (const char *szMapName = NULL);

	static inline bool validWaypointIndex ( int iIndex )
	{
		return ((iIndex >= 0) && (iIndex < m_iNumWaypoints));
	}

	static void precacheWaypointTexture ();

	static int waypointTexture () { return m_iWaypointTexture; }

	static void deleteWaypoint ( int iIndex );

	static void freeMemory ();

	static int getClosestFlagged ( int iFlags, Vector &vOrigin, int iTeam,float *fReturnDist =NULL, unsigned char *failedwpts = NULL );

	static int nearestWaypointGoal ( int iFlags, Vector &origin, float fDist, int iTeam = 0 );
	static CWaypoint *randomRouteWaypoint ( CBot *pBot, Vector vOrigin, Vector vGoal, int iTeam, int iArea );
	static CWaypoint *randomWaypointGoal ( int iFlags, int iTeam = 0, int iArea = 0, bool bForceArea = false, CBot *pBot = NULL, bool bHighDanger = false, int iSearchFlags = 0, int iIgnore = -1 );
	static CWaypoint *randomWaypointGoalBetweenArea ( int iFlags, int iTeam, int iArea, bool bForceArea, CBot *pBot, bool bHighDanger, Vector *org1, Vector *org2, bool bIgnoreBelief = false, int iWpt1 = -1, int iWpt2 = -1 );
	static CWaypoint *randomWaypointGoalNearestArea ( int iFlags, int iTeam, int iArea, bool bForceArea, CBot *pBot, bool bHighDanger, Vector *origin, int iIgnore = -1, bool bIgnoreBelief = false, int iWpt1 = -1 );
	static int randomFlaggedWaypoint (int iTeam = 0);

	static CWaypointVisibilityTable *getVisiblity () { return m_pVisibilityTable; }
	static void setupVisibility ();
	static CWaypoint *getPinchPointFromWaypoint ( Vector vPlayerOrigin, Vector vPinchOrigin );
	static CWaypoint *getNestWaypoint ( int iTeam, int iArea, bool bForceArea = false, CBot *pBot = NULL );

	static void updateWaypointPairs ( std::vector<edict_wpt_pair_t> *pPairs, int iWptFlag, const char *szClassname );
	static bool hasAuthor () { return (m_szAuthor[0]!=0); }
	static const char *getAuthor() { return m_szAuthor; }
	static bool isModified () { return (m_szModifiedBy[0]!=0); }
	static const char *getModifier() { return m_szModifiedBy; }
	static const char *getWelcomeMessage () { return m_szWelcomeMessage; }
private:
	static CWaypoint m_theWaypoints[MAX_WAYPOINTS];	
	static int m_iNumWaypoints;
	static float m_fNextDrawWaypoints;
	static int m_iWaypointTexture;
	static CWaypointVisibilityTable *m_pVisibilityTable;
	static char m_szAuthor[32];
	static char m_szModifiedBy[32];
	static char m_szWelcomeMessage[128];
};


#endif