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
#ifndef __RCBOT_WAYPOINT_LOC_H__
#define __RCBOT_WAYPOINT_LOC_H__

#include "bot_waypoint.h"

/*#define WAYPOINT_LOC(x)\
	{\
	m_iLocations[i][j][k].x;\
	}\

#define EACH_WAYPOINT_LOC(x)\
	{\
			int i,j,k;\
\
		for ( i = 0; i < MAX_WPT_BUCKETS; i ++ )\
		{\
			for ( j = 0; j < MAX_WPT_BUCKETS; j ++ )\
			{\
				for ( k = 0; k < MAX_WPT_BUCKETS; k ++ )\
				{\
					x;\
				}\
			}\
		}\
	}\*/

class CWaypoint;

class CWaypointLocations
// Hash table of waypoint indexes accross certian
// buckets in the map on X/Y Co-ords for quicker
// nearest waypoint finding and waypoint displaying.
{
public:

	static const int REACHABLE_RANGE = 400;

	// max map size is 32768
	static const int HALF_MAX_MAP_SIZE = 16384; // need to know half (negative + positive halves = max)
	/*
	// want bucket spacing of 256 units
	static const int MAX_WPT_BUCKETS = 128;
	*/
	// want bucket spacing of 512 units
	static const int MAX_WPT_BUCKETS = 64;

	static const int BUCKET_SPACING = HALF_MAX_MAP_SIZE*2/MAX_WPT_BUCKETS;

	static unsigned char g_iFailedWaypoints [ CWaypoints::MAX_WAYPOINTS ];
	
	static void AddWptLocation ( CWaypoint *pWaypoint, int iIndex );

	CWaypointLocations()
	{
		Init();
	}

	static unsigned char *resetFailedWaypoints (WaypointList *iIgnoreWpts);

	static void Init()
	{
		Clear();
	}

	static void Clear ()
	{
		for ( int i = 0; i < MAX_WPT_BUCKETS; i ++ )
		{
			for ( int j = 0; j < MAX_WPT_BUCKETS; j ++ )
			{
				for ( int k = 0; k < MAX_WPT_BUCKETS; k ++ )
				{
					m_iLocations[i][j][k].clear();
				}
			}
		}
	}

	static void GetAllInArea ( Vector &vOrigin, WaypointList *pWaypointList, int iVisibleTo );
		
	static void getMinMaxs ( int iLoc, int jLoc, int kLoc, 
									    int *iMinLoc, int *jMinLoc, int *kMinLoc,
									    int *iMaxLoc, int *jMaxLoc, int *kMaxLoc );

	static int GetCoverWaypoint ( Vector vPlayerOrigin, Vector vCoverFrom, WaypointList *iIgnoreWpts, Vector *vGoalOrigin = nullptr, int iTeam = 0, float fMinDist = MIN_COVER_MOVE_DIST, float fMaxDist = HALF_MAX_MAP_SIZE );

	static void FindNearestCoverWaypointInBucket ( int i, int j, int k, const Vector &vOrigin, float *pfMinDist, int *piIndex, WaypointList *iIgnoreWpts, int iCoverFromWpt, Vector *vGoalOrigin = nullptr, int iTeam = 0, float fMinDist = MIN_COVER_MOVE_DIST );

	static void AddWptLocation ( int iIndex, const float *fOrigin );

	static void FindNearestInBucket ( int i, int j, int k, const Vector &vOrigin, float *pfMinDist, int *piIndex,int iIgnoreWpt, bool bGetVisible = true, bool bGetUnreachable = false, bool bIsBot = false, WaypointList *iFailedWpts = nullptr, bool bNearestAimingOnly = false, int iTeam = 0, bool bCheckArea = false, bool bGetVisibleFromOther = false, Vector vOther = Vector(0,0,0), int iFlagsOnly = 0, edict_t *pPlayer = nullptr );
	static void DrawWaypoints ( CClient *pClient, float fDist );
	
	static void DeleteWptLocation ( int iIndex, const float *fOrigin );
	
	static int NearestWaypoint ( const Vector &vOrigin, float fDist, int iIgnoreWpt, bool bGetVisible = true, 
		bool bGetUnreachable = false, bool bIsBot = false, WaypointList *iFailedWpts = nullptr, 
		bool bNearestAimingOnly = false, int iTeam = 0, bool bCheckArea = false, 
		bool bGetVisibleFromOther = false, Vector vOther = Vector(0,0,0), int FlagsOnly = 0, 
		edict_t *pPlayer = nullptr, bool bIgnorevOther = false, float fIgnoreSize = 0.0f );

	static void GetAllVisible( int iFrom, int iOther, Vector &vOrigin, Vector &vOther, float fEDist, WaypointList *iVisible, WaypointList *iInvisible );

	///////////

	static void AutoPath ( edict_t *pPlayer, int iWpt );

	static void AutoPathInBucket ( edict_t *pPlayer, int i, int j, int k, int iWpt );
	
	static int NearestBlastWaypoint ( const Vector &vOrigin, const Vector &vSrc, float fNearestDist, int iIgnoreWpt, bool bGetVisible, bool bGetUnReachable, bool bIsBot, bool bNearestAimingOnly, int iTeam, bool bCheckArea, float fBlastRadius = BLAST_RADIUS );

	static void FindNearestBlastInBucket ( int i, int j, int k, const Vector &vOrigin, const Vector &vSrc, float *pfMinDist, int *piIndex, int iIgnoreWpt, bool bGetVisible, bool bGetUnReachable, bool bIsBot, bool bNearestAimingOnly, int iTeam, bool bCheckArea, float fBlastRadius = BLAST_RADIUS );

	static int NearestGrenadeWaypoint ( const Vector &vTarget, int iTeam, bool bCheckArea, float fBlastRadius = BLAST_RADIUS );

	static void FindNearestGrenadeWptInBucket ( int i, int j, int k, const Vector &vOrigin, const Vector &vSrc, float *pfMinDist, int *piIndex, int iIgnoreWpt, bool bGetVisible, bool bGetUnReachable, bool bIsBot, bool bNearestAimingOnly, int iTeam, bool bCheckArea, float fBlastRadius = BLAST_RADIUS );

private:
	
	//static dataStack<int> m_iLocations[MAX_WPT_BUCKETS][MAX_WPT_BUCKETS][MAX_WPT_BUCKETS];
	static WaypointList m_iLocations[MAX_WPT_BUCKETS][MAX_WPT_BUCKETS][MAX_WPT_BUCKETS];
	static float m_fIgnoreSize;
	static Vector m_vIgnoreLoc;
	static bool m_bIgnoreBox;
};
#endif
