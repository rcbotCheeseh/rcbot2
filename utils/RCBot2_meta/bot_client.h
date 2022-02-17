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
#ifndef __RCBOT_CLIENT_H__
#define __RCBOT_CLIENT_H__

#include <vector>

#include "bot_const.h"
//#include "bot_ehandle.h"
#include "bot_waypoint.h"

#define MAX_STORED_AUTOWAYPOINT 5

typedef enum eWptCopyType
{
	WPT_COPY_NONE = 0,
	WPT_COPY_COPY,
	WPT_COPY_CUT
}eWptCopyType_s;

struct edict_t;

class CBot;
class CWaypoint;
class CBotMenu;

/**** Autowaypoint stuff borrowed from RCBot1 *****/
// Store a vector as short integers and return as 
// normal floats for less space usage.
template <class T>
class CTypeVector
{
public:

	CTypeVector()
	{
		memset(this,0,sizeof(CTypeVector<T>));
	}

	void SetVector ( Vector vVec ) 
	{ 
		m_x = (T)vVec.x;
		m_y = (T)vVec.y;
		m_z = (T)vVec.z;

		m_bVectorSet = TRUE;
	}

	Vector GetVector () const
	{
		return Vector((float)m_x,(float)m_y,(float)m_z);
	}

	BOOL IsVectorSet ()
	{
		return m_bVectorSet;
	}

	void UnSet ()
	{
		m_bVectorSet = FALSE;
	}
protected:
	T m_x,m_y,m_z;

	BOOL m_bVectorSet;
};

class CAutoWaypointCheck : public CTypeVector<vec_t>
{
public:
	void SetPoint ( Vector vec, int iFlags )
	{
		m_iFlags = iFlags;

		SetVector(vec);
	}

	int getFlags ()
	{
		return m_iFlags;
	}

	void UnSetPoint ()
	{
		m_bVectorSet = FALSE;
		m_iFlags = 0;
	}
private:
	int m_iFlags;
};

class CToolTip
{
public:
	CToolTip ( const char *pszMessage, const char *pszSound = NULL )
	{
		m_pszMessage = pszMessage;
		m_pszSound = pszSound;
	}

	void send(edict_t *pPlayer);
private:
	const char *m_pszMessage;
	const char *m_pszSound;
};

class CClient
{
public:
	CClient ()
	{
		m_szSteamID = NULL;
		m_pPlayerInfo = NULL;
		m_pDebugBot = NULL;
		m_WaypointCopyType = WPT_COPY_NONE;
		m_pMenu = NULL;
		m_iMenuCommand = -1;
		m_fNextUpdateMenuTime = 0.0f;
		m_iWaypointShowFlags = 0;
		m_iWaypointDrawType = 3;
		m_szSoundToPlay[0] = 0;
		m_iAutoEventWaypoint = 0;
		m_fAutoEventWaypointRadius = 0.0f;
		m_vAutoEventWaypointOrigin = Vector(0,0,0);
		m_bAutoEventWaypointAutoType = false;
		m_iAutoEventWaypointArea = 0;
		m_fNextBotServerMessage = 0;
		m_bSentWelcomeMessage = false;
		m_fSpeed = 0;
		m_fUpdatePos = 0;
		m_bTeleportVectorValid = false;
		m_vTeleportVector = Vector(0,0,0);
		m_fMonitorHighFiveTime = 0;
	}

	void monitorHighFive ()
	{
		m_fMonitorHighFiveTime = engine->Time() + 5.0f;
	}

	void init ();

	void setupMenuCommands ();
	void resetMenuCommands ();

	void setTeleportVector ();
	Vector *getTeleportVector () { if ( m_bTeleportVectorValid ) return &m_vTeleportVector; return NULL; }

	bool isUsingMenu () { return m_pMenu != NULL; }

	void setCurrentMenu ( CBotMenu *pMenu ) 
	{ 
		m_pMenu = pMenu; 

		if ( pMenu == NULL )
			resetMenuCommands();
		else
			setupMenuCommands();
	}

	CBotMenu *getCurrentMenu () { return m_pMenu; }
	void setMenuCommand ( int iCommand ) { m_iMenuCommand = iCommand; }
	int getLastMenuCommand () { return m_iMenuCommand; }
	bool needToRenderMenu ();
	void updateRenderMenuTime ();

	int accessLevel ();
	// this player joins with pPlayer edict
	void clientConnected ( edict_t *pPlayer );
	// this player disconnects
	void clientDisconnected ();

	void showMenu () { m_bShowMenu = true; };

	bool isUsed ();

	Vector getOrigin ();

	float getSpeed () { return m_fSpeed; }
	Vector getVelocity () { return m_vVelocity; }

	void setWaypointCut ( CWaypoint *pWaypoint );
	void setWaypointCopy (CWaypoint *pWaypoint); 
	void setEdict ( edict_t *pPlayer );

	edict_t *getPlayer () { return m_pPlayer; }

	bool isPlayer ( edict_t *pPlayer ) { return m_pPlayer == pPlayer; }

	bool isWaypointOn () { return m_bWaypointOn; }
	void setWaypointOn ( bool bOn ) { m_bWaypointOn = bOn; }
	void setWaypoint ( int iWpt ) { m_iCurrentWaypoint = iWpt; }
	int currentWaypoint () { return m_iCurrentWaypoint; }

	void setAccessLevel ( int iLev ) { m_iAccessLevel = iLev; }

	bool isAutoPathOn () { return m_bAutoPaths; }
	void setAutoPath ( bool bOn ) { m_bAutoPaths = bOn; }
	bool isPathWaypointOn () { return m_bPathWaypointOn; }
	void setPathWaypoint ( bool bOn ) { m_bPathWaypointOn = bOn; }

	int getWptArea () { return m_iWptArea; }
	void setWptArea ( int area ) { m_iWptArea = area; }

	void setPathFrom ( int iWpt ) { m_iPathFrom = iWpt; }
	void setPathTo ( int iWpt ) { m_iPathTo = iWpt; }

	int getPathFrom () { return m_iPathFrom; }
	int getPathTo () { return m_iPathTo; }

	void teleportTo ( Vector vOrigin );

	const char *getSteamID () { return m_szSteamID; }
	const char *getName ();

	void updateCurrentWaypoint ();

	void clientActive ();

	void setDebug ( int iLevel, bool bSet ) { if ( bSet ) { m_iDebugLevels |= 1<<iLevel; } else { m_iDebugLevels &= ~(1<<iLevel); } }
	bool isDebugOn ( int iLevel ) { return (m_iDebugLevels & 1<<iLevel)>0; }
	void clearDebug ( ) { m_iDebugLevels = 0; }
	bool isDebugging () { return m_iDebugLevels != 0; }

	void setDebugBot ( edict_t *pBot ) { m_pDebugBot = pBot; }
	bool isDebuggingBot ( edict_t *pBot ) { return m_pDebugBot == pBot; }
	edict_t *getDebugBot () { return m_pDebugBot; }

	void think ();

	void setDrawType ( unsigned short int iType ) { m_iWaypointDrawType = iType; }
	unsigned short int getDrawType () { return m_iWaypointDrawType; }

	float getWptCopyRadius() { return m_fCopyWptRadius; }
	int getWptCopyFlags () { return m_iCopyWptFlags; }
	int getWptCopyArea () { return m_iCopyWptArea; }

	eWptCopyType getWptCopyType () { return m_WaypointCopyType; }

	bool isShowingWaypoint ( int iFlags ) { return (m_iWaypointShowFlags & iFlags) > 0; }
	void showWaypoints ( int iFlags ) { m_iWaypointShowFlags |= iFlags; }
	void dontShowWaypoints ( int iFlags ) { m_iWaypointShowFlags &= ~iFlags; }
	bool isShowingAllWaypoints () { return m_iWaypointShowFlags == 0; }
	int getShowWaypointFlags () { return m_iWaypointShowFlags; }
	void playSound ( const char *pszSound );

	void setAutoWaypointMode ( bool mode, bool debug ) 
	{ 
		m_bAutoWaypoint = mode; 
		m_bDebugAutoWaypoint = debug; 
	}

	bool autoWaypointOn () { return m_bAutoWaypoint; }
	void autoEventWaypoint ( int iType, float fRadius, bool bAtOtherOrigin = false, int iTeam = 0, Vector vOrigin = Vector(0,0,0), bool bIgnoreTeam = false, bool bAutoType = false );
	void giveMessage(const char*msg, float fTime=0.1f);
private:
	edict_t *m_pPlayer;
	// steam id
	char *m_szSteamID;
	// is drawing waypoints ON for this player
	bool m_bWaypointOn;
	// player editing this waypoint
	int m_iCurrentWaypoint;

	int m_iPathFrom;
	int m_iPathTo;

	int m_iAccessLevel;

	Vector m_vLastPos;
	float m_fUpdatePos;

	int m_iWptArea;

	// auto path waypointing
	bool m_bAutoPaths;
	bool m_bPathWaypointOn;
	unsigned short int m_iWaypointDrawType;

	unsigned int m_iDebugLevels;

	IPlayerInfo *m_pPlayerInfo;

	MyEHandle m_pDebugBot;

	float m_fSpeed;
	Vector m_vVelocity;

	bool m_bShowMenu;

	float m_fCopyWptRadius;
	int m_iCopyWptFlags;
	int m_iCopyWptArea;

	/* TODO m_WaypointCutPaths never gets read from -- only ::clear and ::emplace_back are used */
	WaypointList m_WaypointCutPaths;
	
	eWptCopyType m_WaypointCopyType;

	float m_fNextPrintDebugInfo;

	// menu stuff
	CBotMenu *m_pMenu;
	int m_iPrevMenu;
	int m_iMenuCommand;

	float m_fNextUpdateMenuTime;

	int m_iWaypointShowFlags; // 0 = showall (default)

	char m_szSoundToPlay[128];

	/**** Autowaypoint stuff below borrowed and converted from RCBot1 ****/
	CAutoWaypointCheck m_vLastAutoWaypointCheckPos[MAX_STORED_AUTOWAYPOINT]; 

	bool m_bDebugAutoWaypoint;
	bool m_bAutoWaypoint;
	float m_fLastAutoWaypointCheckTime;
	Vector m_vLastAutoWaypointPlacePos;
	bool m_bSetUpAutoWaypoint;
	float m_fCanPlaceJump;
	int m_iLastButtons;

	int m_iLastJumpWaypointIndex;
	int m_iLastLadderWaypointIndex;
	int m_iLastMoveType;
	float m_fCanPlaceLadder;
	int m_iJoinLadderWaypointIndex;

	// new stuff
	int m_iAutoEventWaypoint;
	float m_fAutoEventWaypointRadius;
	Vector m_vAutoEventWaypointOrigin;
	int m_iAutoEventWaypointTeamOn; // waypoint flags to enable for team specific
	int m_iAutoEventWaypointTeamOff;  // waypoint flags to disable for team specific
	int m_iAutoEventWaypointTeam; // the player's team of the waypoint to add
	int m_iAutoEventWaypointArea;
	bool m_bAutoEventWaypointAutoType;
	float m_fNextBotServerMessage;
	std::queue<CToolTip> m_NextTooltip;
	bool m_bSentWelcomeMessage;

	bool m_bTeleportVectorValid;
	Vector m_vTeleportVector;

	bool m_bIsTeleporting;
	float m_fTeleportTime;

	float m_fMonitorHighFiveTime;
};

class CClients
{
public:
	// called when player joins
	static CClient *clientConnected ( edict_t *pPlayer );
	static void clientDisconnected ( edict_t *pPlayer );
	// player starts game
	static void clientActive ( edict_t *pPlayer );
	// get index in array
	static int slotOfEdict (const edict_t* pPlayer);
	static void init ( edict_t *pPlayer );
	static CClient *get ( int iIndex ) { return &m_Clients[iIndex]; }
	static CClient *get ( edict_t *pPlayer ) { return &m_Clients[slotOfEdict(pPlayer)]; }
	static void setListenServerClient ( CClient *pClient ) { m_pListenServerClient = pClient; }
	static bool isListenServerClient ( CClient *pClient ) { return m_pListenServerClient == pClient; }
	static bool noListenServerClient () { return m_pListenServerClient == NULL; }
	static void clientThink ();
	static bool clientsDebugging ( int iLev = 0 );
	static void clientDebugMsg ( int iLev, const char *szMsg, CBot *pBot = NULL );
	static void clientDebugMsg(CBot *pBot, int iLev, const char *fmt, ... );
	static CClient *findClientBySteamID (const char* szSteamID);
	static edict_t *getListenServerClient() { if ( m_pListenServerClient ) return m_pListenServerClient->getPlayer(); else return NULL; }

	static void initall () { for ( int i = 0; i < MAX_PLAYERS; i ++ ) { m_Clients[i].init(); } }
	static void giveMessage (const char* msg, float fTime = 0.1f, edict_t* pPlayer = NULL);// NULL to everyone
private:
	static CClient m_Clients[MAX_PLAYERS];
	static CClient *m_pListenServerClient;
	static bool m_bClientsDebugging;
};
#endif
