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

#ifndef __BOT_GLOBALS_H__
#define __BOT_GLOBALS_H__

#include "bot_mods.h"
#include "bot_const.h" // for Mod id
#include "bot_commands.h" // for main rcbot command

#ifdef _WIN32
#include <ctype.h>
#endif

#define MAX_MAP_STRING_LEN 64
#define MAX_PATH_LEN 512
#define MAX_ENTITIES 2048

class CBotGlobals
{
public:
	CBotGlobals ();

	static void init ();
	static bool initModFolder ();

	static bool gameStart ();	

	static QAngle entityEyeAngles ( edict_t *pEntity );

	static QAngle playerAngles ( edict_t *pPlayer );

	static inline bool isPlayer ( edict_t *pEdict )
	{
		static int index;

		index = ENTINDEX(pEdict);

		return (index>0)&&(index<=gpGlobals->maxClients);
	}

	static bool walkableFromTo (edict_t *pPlayer,Vector v_src, Vector v_dest);

	static void teleportPlayer ( edict_t *pPlayer, Vector v_dest );

	static float yawAngleFromEdict(edict_t *pEntity,Vector vOrigin);
	//static float getAvoidAngle(edict_t *pEdict,Vector origin);

	// make folders for a file if they don't exist
	static bool makeFolders ( char *szFile );
	// just open file but also make folders if possible
	static FILE *openFile ( char *szFile, char *szMode );
	// get the proper location
	static void buildFileName ( char *szOutput, const char *szFile, const char *szFolder = NULL, const char *szExtension = NULL, bool bModDependent = false );
	// add a directory delimiter to the string like '/' (linux) or '\\' (windows) or
	static void addDirectoryDelimiter ( char *szString );
	// print a message to client pEntity with bot formatting
	static void botMessage ( edict_t *pEntity, int iErr, const char *fmt, ... );	
	
	static void fixFloatAngle ( float *fAngle );

	static float DotProductFromOrigin ( edict_t *pEnemy, Vector pOrigin );
	static float DotProductFromOrigin ( Vector vPlayer, Vector vFacing, QAngle eyes );

	static int numPlayersOnTeam(int iTeam, bool bAliveOnly);
	static void setMapName ( const char *szMapName );
	static char *getMapName (); 

	static bool IsMapRunning () { return m_bMapRunning; }
	static void setMapRunning ( bool bSet ) { m_bMapRunning = bSet; }

	static bool isNetworkable ( edict_t *pEntity );

	static inline bool entityIsValid ( edict_t *pEntity )
	{
		return pEntity && !pEntity->IsFree() && (pEntity->GetNetworkable() != NULL) && (pEntity->GetIServerEntity() != NULL) && (pEntity->m_NetworkSerialNumber != 0);	
	}

	static void serverSay ( char *fmt, ... );

	static bool isAlivePlayer ( edict_t *pEntity );

	static bool setWaypointDisplayType ( int iType );

	static void fixFloatDegrees360 ( float *pFloat );

	static edict_t *findPlayerByTruncName ( const char *name );

// linux fix
	inline static CBotMod *getCurrentMod ()
	{
		return m_pCurrentMod;
	}

	////////////////////////////////////////////////////////////////////////
	// useful functions
	static bool boundingBoxTouch2d ( 
		const Vector2D &a1, const Vector2D &a2,
		const Vector2D &bmins, const Vector2D &bmaxs );

	static bool onOppositeSides2d ( 
		const Vector2D &amins, const Vector2D &amaxs,
		const Vector2D &bmins, const Vector2D &bmaxs );

	static bool linesTouching2d ( 
		const Vector2D &amins, const Vector2D &amaxs,
		const Vector2D &bmins, const Vector2D &bmaxs );

	static bool boundingBoxTouch3d (
		const Vector &a1, const Vector &a2,
		const Vector &bmins, const Vector &bmaxs );

	static bool onOppositeSides3d (
		const Vector &amins, const Vector &amaxs,
		const Vector &bmins, const Vector &bmaxs );

	static bool linesTouching3d (
		const Vector &amins, const Vector &amaxs,
		const Vector &bmins, const Vector &bmaxs );

	static float grenadeWillLand (  Vector vOrigin, Vector vEnemy, float fProjSpeed = 400.0f, float fGrenadePrimeTime = 5.0f, float *fAngle = NULL );
	////////////////////////////////////////////////////////////////////////

	/*static Vector forwardVec ();
	static Vector rightVec ();
	static Vector upVec ();*/
	////////
	static trace_t *getTraceResult () { return &m_TraceResult; }
	static bool isVisibleHitAllExceptPlayer ( edict_t *pPlayer, Vector vSrc, Vector vDest, edict_t *pDest = NULL );
	static bool isVisible ( edict_t *pPlayer, Vector vSrc, Vector vDest);
	static bool isVisible ( edict_t *pPlayer, Vector vSrc, edict_t *pDest);
	static bool isShotVisible ( edict_t *pPlayer, Vector vSrc, Vector vDest, edict_t *pDest );
	static bool isVisible ( Vector vSrc, Vector vDest);
	static void traceLine ( Vector vSrc, Vector vDest, unsigned int mask, ITraceFilter *pFilter);
	static float quickTraceline ( edict_t *pIgnore, Vector vSrc, Vector vDest ); // return fFraction
	static bool traceVisible (edict_t *pEnt);
	////////
	static inline Vector entityOrigin ( edict_t *pEntity ) 
	{ 
		return pEntity->GetIServerEntity()->GetCollideable()->GetCollisionOrigin(); 
	}
	static int getTeam ( edict_t *pEntity );
	static bool entityIsAlive ( edict_t *pEntity );
	static int countTeamMatesNearOrigin ( Vector vOrigin, float fRange, int iTeam, edict_t *pIgnore = NULL );
	static int numClients ();
	static void levelInit();

	static inline void setClientMax ( int iMaxClients ) { m_iMaxClients = iMaxClients; }

	static inline void setEventVersion ( int iVersion ){m_iEventVersion = iVersion;}

	static inline bool isEventVersion ( int iVersion ){return (m_iEventVersion == iVersion);}

	static inline bool getTeamplayOn (){return m_bTeamplay;}

	static inline void setTeamplay ( bool bOn ){m_bTeamplay = bOn;}

	static inline bool isMod ( eModId iMod ) { 	return m_iCurrentMod == iMod; }

	static inline char *modFolder (){return m_szModFolder;}

	static inline int maxClients () {return m_iMaxClients;}

	static edict_t *playerByUserId(int iUserId);

	static bool isCurrentMod ( eModId modid );

	static bool checkOpensLater ( Vector vSrc, Vector vDest );

	inline static bool setupMapTime ( ) { return m_fMapStartTime == 0; }

	static bool isBreakableOpen ( edict_t *pBreakable );

	static Vector getVelocity ( edict_t *pPlayer );

	////////
	static CBotSubcommands *m_pCommands;

	static void readRCBotFolder();
	
	static bool dirExists(const char *path);

	static bool str_is_empty(const char *s) {
		while (*s != '\0') {
			if (!isspace(*s))
				return false;
			s++;
		}

		return true;
	}

private:
	static eModId m_iCurrentMod;
	static CBotMod *m_pCurrentMod;
	static char *m_szModFolder;
	static char m_szMapName[MAX_MAP_STRING_LEN];
	static int m_iDebugLevels;
	static bool m_bMapRunning;
	static trace_t m_TraceResult;
	static int m_iMaxClients;
	static int m_iEventVersion;
	static int m_iWaypointDisplayType;
	static bool m_bTeamplay;
	static float m_fMapStartTime;
	static char *m_szRCBotFolder;

	/*static Vector m_vForward;
	static Vector m_vRight;
	static Vector m_vUp;*/
};

#endif
