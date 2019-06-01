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

#include "bot.h"

#include "bot_waypoint.h"
#include "bot_waypoint_visibility.h"
#include "bot_waypoint_locations.h"
#include "bot_genclass.h"
#include "bot_globals.h"

#include <vector>    //bir3yk
using namespace std;    //bir3yk

unsigned char CWaypointLocations :: g_iFailedWaypoints[CWaypoints::MAX_WAYPOINTS];
dataUnconstArray<int> CWaypointLocations :: m_iLocations[MAX_WPT_BUCKETS][MAX_WPT_BUCKETS][MAX_WPT_BUCKETS];
float CWaypointLocations :: m_fIgnoreSize = 0;
Vector CWaypointLocations :: m_vIgnoreLoc = Vector(0,0,0);
bool CWaypointLocations :: m_bIgnoreBox = false;

#define READ_LOC(loc) abs((int)((int)((loc) + HALF_MAX_MAP_SIZE) / BUCKET_SPACING));

unsigned char *CWaypointLocations :: resetFailedWaypoints (dataUnconstArray<int> *iIgnoreWpts)
{
	Q_memset(g_iFailedWaypoints,0,sizeof(unsigned char)*CWaypoints::MAX_WAYPOINTS);
	
	if ( iIgnoreWpts )
	{   
		//dataStack<int> ignoreWptStack = *iIgnoreWpts;
		int iWpt;
		
		//while ( !ignoreWptStack.IsEmpty() )
		for ( int l = 0; l < iIgnoreWpts->Size(); l ++ )
		{
			if ( (iWpt = (*iIgnoreWpts)[l]) != -1 )//(iWpt = ignoreWptStack.ChooseFromStack()) != -1 )
				g_iFailedWaypoints[iWpt] = 1;
		}
	}

	return g_iFailedWaypoints;
}

#define CLAMP_TO_ZERO(x) x=((x)<0)?0:x
#define CLAMP_TO(x,clamp) x=((x)>(clamp))?(clamp):x

void CWaypointLocations :: getMinMaxs (const int iLoc, const int jLoc, const int kLoc, 
									    int *iMinLoci, int *iMinLocj, int *iMinLock,
									    int *iMaxLoci, int *iMaxLocj, int *iMaxLock )
{
	static const int iMaxLoc = MAX_WPT_BUCKETS-1;

	// get current area
	*iMinLoci = iLoc-1;
	*iMaxLoci = iLoc+1;

	*iMinLocj = jLoc-1;
	*iMaxLocj = jLoc+1;

	*iMinLock = kLoc-1;
	*iMaxLock = kLoc+1;

	CLAMP_TO_ZERO(*iMinLoci);
	CLAMP_TO_ZERO(*iMinLocj);
	CLAMP_TO_ZERO(*iMinLock);

	CLAMP_TO(*iMaxLoci,iMaxLoc);
	CLAMP_TO(*iMaxLocj,iMaxLoc);
	CLAMP_TO(*iMaxLock,iMaxLoc);

}
///////////////
// return nearest waypoint that can be used to cover from vCoverFrom vector
void CWaypointLocations :: AutoPath ( edict_t *pPlayer, int iWpt )
{
	CWaypoint *pWpt = CWaypoints::getWaypoint(iWpt);
	Vector vOrigin = pWpt->getOrigin();

	int iLoc = READ_LOC(vOrigin.x);
	int jLoc = READ_LOC(vOrigin.y);
	int kLoc = READ_LOC(vOrigin.z);

	int i,j,k;

	int iMinLoci,iMaxLoci,iMinLocj,iMaxLocj,iMinLock,iMaxLock;

	getMinMaxs(iLoc,jLoc,kLoc,&iMinLoci,&iMinLocj,&iMinLock,&iMaxLoci,&iMaxLocj,&iMaxLock);

	for ( i = iMinLoci; i <= iMaxLoci; i++ )
	{
		for ( j = iMinLocj; j <= iMaxLocj; j++ )
		{			
			for ( k = iMinLock; k <= iMaxLock; k++ )
			{		
				// check each area around the current area
				// for closer waypoints
				AutoPathInBucket(pPlayer, i,j,k,iWpt);
			}
		}
	}
}

// @param iFrom waypoint number from a and b within distance
void CWaypointLocations :: GetAllInArea ( Vector &vOrigin, vector <int> *pWaypointList, int iVisibleTo )
{
	int iLoc = READ_LOC(vOrigin.x);
	int jLoc = READ_LOC(vOrigin.y);
	int kLoc = READ_LOC(vOrigin.z);

	static dataUnconstArray<int> *arr;

	int i,j,k;
	int iWpt;

	CWaypointVisibilityTable *pTable = CWaypoints::getVisiblity();

	int iMinLoci,iMaxLoci,iMinLocj,iMaxLocj,iMinLock,iMaxLock;

	getMinMaxs(iLoc,jLoc,kLoc,&iMinLoci,&iMinLocj,&iMinLock,&iMaxLoci,&iMaxLocj,&iMaxLock);

	for ( i = iMinLoci; i <= iMaxLoci; i++ )
	{
		for ( j = iMinLocj; j <= iMaxLocj; j++ )	
		{			
			for ( k = iMinLock; k <= iMaxLock; k++ )
			{		
				//dataStack <int> tempStack = m_iLocations[i][j][k];

				arr = &(m_iLocations[i][j][k]);
				//while ( !tempStack.IsEmpty() )
				for (  int l = 0; l < m_iLocations[i][j][k].Size(); l ++ )
				{
					iWpt = arr->ReturnValueFromIndex(l);

					if ( iWpt == iVisibleTo )
						continue;

					if ( (iVisibleTo==-1) || pTable->GetVisibilityFromTo(iWpt,iVisibleTo) )
						pWaypointList->push_back(iWpt);
				}
			}
		}
	}
}


// @param iFrom waypoint number from a and b within distance
void CWaypointLocations :: GetAllVisible ( int iFrom, int iOther, Vector &vOrigin, 
										  Vector &vOther, float fEDist, dataUnconstArray<int> *iVisible, 
										  dataUnconstArray<int> *iInvisible )
{
	CWaypoint *pWpt;
	int iLoc = READ_LOC(vOrigin.x);
	int jLoc = READ_LOC(vOrigin.y);
	int kLoc = READ_LOC(vOrigin.z);

	static dataUnconstArray<int> *arr;

	int i,j,k;
	int iWpt;

	int iMinLoci,iMaxLoci,iMinLocj,iMaxLocj,iMinLock,iMaxLock;

	CWaypointVisibilityTable *pTable = CWaypoints::getVisiblity();
	
	if ( (iFrom == -1) || !pTable)
		return;

	getMinMaxs(iLoc,jLoc,kLoc,&iMinLoci,&iMinLocj,&iMinLock,&iMaxLoci,&iMaxLocj,&iMaxLock);

	for ( i = iMinLoci; i <= iMaxLoci; i++ )
	{
		for ( j = iMinLocj; j <= iMaxLocj; j++ )
		{			
			for ( k = iMinLock; k <= iMaxLock; k++ )
			{		
				//dataStack <int> tempStack = m_iLocations[i][j][k];

				arr = &(m_iLocations[i][j][k]);
				//while ( !tempStack.IsEmpty() )
				for (  int l = 0; l < m_iLocations[i][j][k].Size(); l ++ )
				{
					iWpt = arr->ReturnValueFromIndex(l);
					pWpt = CWaypoints::getWaypoint(iWpt);

					//int iWpt = tempStack.ChooseFromStack();
					
					// within range only deal with these waypoints
					if ( (pWpt->distanceFrom(vOrigin) < fEDist) && (pWpt->distanceFrom(vOther) < fEDist) )
					{
						// iFrom should be the enemy waypoint
						if ( pTable->GetVisibilityFromTo(iFrom,iWpt) ) //|| pTable->GetVisibilityFromTo(iOther,iWpt) )
						{   //CBotGlobals::isVisible(vVisibleFrom,CWaypoints::getWaypoint(iWpt)->getOrigin()) )
							iVisible->Add(iWpt);
						}
						else if ( !iVisible->IsMember(iWpt) )
							iInvisible->Add(iWpt);
					}
				}
			}
		}
	}
}

void CWaypointLocations :: AutoPathInBucket ( edict_t *pPlayer, int i, int j, int k, int iWptFrom )
{
	//dataStack <int> tempStack = m_iLocations[i][j][k];
	int iWpt;
	CWaypoint *pOtherWpt;
	extern ConVar bot_waypointpathdist;

	CWaypoint *pWpt = CWaypoints::getWaypoint(iWptFrom);
	Vector vWptOrigin = pWpt->getOrigin();
	Vector vOtherWptOrigin;

	trace_t tr;

	//CTraceFilterWorldOnly filter;

	//while ( !tempStack.IsEmpty() )
	dataUnconstArray<int> *arr = &(m_iLocations[i][j][k]);
	short int size = static_cast<short int>(arr->Size());
	
	for ( int l = 0; l < size; l ++ )
	{
		iWpt = arr->ReturnValueFromIndex(l);

		//iWpt = tempStack.ChooseFromStack();

		pOtherWpt = CWaypoints::getWaypoint(iWpt);

		if ( !pOtherWpt->isUsed() )
			continue;

		if ( pOtherWpt == pWpt )
			continue;
		
		vOtherWptOrigin = pOtherWpt->getOrigin();

	//	if ( fabs(vOtherWptOrigin.z-vWptOrigin.z) > 128 )
		//	continue;

		if ( (vWptOrigin-vOtherWptOrigin).Length() <= bot_waypointpathdist.GetFloat() )
		{
			if ( CBotGlobals::isVisible(vWptOrigin,vOtherWptOrigin) )
			{
				if ( CBotGlobals::walkableFromTo(pPlayer, vWptOrigin,vOtherWptOrigin) )
				{
					pWpt->addPathTo(iWpt);			
				}

				if ( CBotGlobals::walkableFromTo(pPlayer,vOtherWptOrigin,vWptOrigin) )
				{
					pOtherWpt->addPathTo(iWptFrom);		
				}
			}
		}
	}
}

void CWaypointLocations :: AddWptLocation ( int iIndex, const float *fOrigin )
{
// Add a waypoint with index and at origin (for quick insertion in the list)
//
	int i = READ_LOC(fOrigin[0]);
	int j = READ_LOC(fOrigin[1]);
	int k = READ_LOC(fOrigin[2]);

	m_iLocations[i][j][k].Add(iIndex);
	//m_iLocations[i][j][k].Push(iIndex);
}

void CWaypointLocations :: DeleteWptLocation ( int iIndex, const float *fOrigin )
// Delete the waypoint index at the origin (for finding it quickly in the list)
//
{
	int i = READ_LOC(fOrigin[0]);
	int j = READ_LOC(fOrigin[1]);
	int k = READ_LOC(fOrigin[2]);

	m_iLocations[i][j][k].Remove(iIndex);
	//m_iLocations[i][j][k].Remove(iIndex);
}

///////////////
// return nearest waypoint that can be used to cover from vCoverFrom vector
int CWaypointLocations :: GetCoverWaypoint ( const Vector vPlayerOrigin, const Vector vCoverFrom, 
											dataUnconstArray<int> *iIgnoreWpts, Vector *vGoalOrigin, 
											int iTeam, float fMinDist, float fMaxDist )
{
	int iWaypoint;

	iWaypoint = CWaypointLocations::NearestWaypoint(vCoverFrom,REACHABLE_RANGE,-1,true,true,false,NULL,false,0,false,true,vPlayerOrigin);

	if ( iWaypoint == -1 )
		return -1;

	float fNearestDist = fMaxDist;
	
	int iNearestIndex = -1;

	int iLoc = READ_LOC(vPlayerOrigin.x);
	int jLoc = READ_LOC(vPlayerOrigin.y);
	int kLoc = READ_LOC(vPlayerOrigin.z);

	int i,j,k;

	int iMinLoci,iMaxLoci,iMinLocj,iMaxLocj,iMinLock,iMaxLock;

	getMinMaxs(iLoc,jLoc,kLoc,&iMinLoci,&iMinLocj,&iMinLock,&iMaxLoci,&iMaxLocj,&iMaxLock);

	Q_memset(g_iFailedWaypoints,0,sizeof(unsigned char)*CWaypoints::MAX_WAYPOINTS);
	
	if ( iIgnoreWpts )
	{   
		//dataStack<int> ignoreWptStack = *iIgnoreWpts;
		int iWpt;
		
		//while ( !ignoreWptStack.IsEmpty() )
		for ( int l = 0; l < iIgnoreWpts->Size(); l ++ )
		{
			if ( (iWpt = (*iIgnoreWpts)[l]) != -1 )//(iWpt = ignoreWptStack.ChooseFromStack()) != -1 )
				g_iFailedWaypoints[iWpt] = 1;
		}
	}

	for ( i = iMinLoci; i <= iMaxLoci; i++ )
	{
		for ( j = iMinLocj; j <= iMaxLocj; j++ )
		{
			for ( k = iMinLock; k <= iMaxLock; k++ )
			{
				// check each area around the current area
				// for closer waypoints
				FindNearestCoverWaypointInBucket(i,j,k,vPlayerOrigin,&fNearestDist,&iNearestIndex,iIgnoreWpts,iWaypoint,vGoalOrigin,iTeam,fMinDist);
			}
		}
	}

	return iNearestIndex;
}

void CWaypointLocations :: FindNearestCoverWaypointInBucket ( int i, int j, int k, 
															 const Vector &vOrigin, float *pfMinDist, 
															 int *piIndex, 
															 dataUnconstArray<int> *iIgnoreWpts, 
															 int iCoverFromWpt, Vector *vGoalOrigin, 
															 int iTeam, float fMinDist )
// Search for the nearest waypoint : I.e.
// Find the waypoint that is closest to vOrigin from the distance pfMinDist
// And set the piIndex to the waypoint index if closer.
{
	//dataStack <int> tempStack = m_iLocations[i][j][k];
	
	CWaypoint *curr_wpt;
	int iSelectedIndex;
	float fDist;
	dataUnconstArray<int> *arr = &(m_iLocations[i][j][k]);
	short int size = static_cast<short int>(arr->Size());
	//CBotMod *curmod = CBotGlobals::getCurrentMod();

	for ( int l = 0; l < size; l ++ )
	//while ( !tempStack.IsEmpty() )
	{
		iSelectedIndex = arr->ReturnValueFromIndex(l);//tempStack.ChooseFromStack();

		if ( iCoverFromWpt == iSelectedIndex )
			continue;
		if ( g_iFailedWaypoints[iSelectedIndex] )
		    continue;

		curr_wpt = CWaypoints::getWaypoint(iSelectedIndex);

		if ( !curr_wpt->isUsed() )
			continue; 
		if ( curr_wpt->hasFlag(CWaypointTypes::W_FL_UNREACHABLE) )
			continue;
	    if ( !curr_wpt->forTeam(iTeam) )
		{
			continue;
		}
		if ( CWaypoints::getVisiblity()->GetVisibilityFromTo(iCoverFromWpt,iSelectedIndex) )
			continue;


		(fDist = curr_wpt->distanceFrom(vOrigin));

		if ( vGoalOrigin != NULL )
		{
			fDist += curr_wpt->distanceFrom(*vGoalOrigin);
		}

		if ( (fDist > fMinDist) && (fDist < *pfMinDist) )
		{
			*piIndex = iSelectedIndex;
			*pfMinDist = fDist;			
		}
	}
}

////////////////////////////////////
////////////////////////////////////////////////
/////////////////////////////
// get the nearest waypoint INDEX from an origin
int CWaypointLocations :: NearestBlastWaypoint ( const Vector &vOrigin, const Vector &vSrc, 
												float fNearestDist, int iIgnoreWpt, 
												bool bGetVisible, bool bGetUnReachable, 
												bool bIsBot, bool bNearestAimingOnly, 
												int iTeam, bool bCheckArea, float fBlastRadius )
{
	int iNearestIndex = -1;
	int iLoc;
	int jLoc;
	int kLoc;
	int i,j,k;

	Vector vMid = (vSrc - vOrigin);

	vMid = (vMid / vMid.Length());
	vMid = (vMid*((vSrc-vOrigin).Length()/2));
	vMid = vOrigin + vMid;

	iLoc = READ_LOC(vMid.x);
	jLoc = READ_LOC(vMid.y);
	kLoc = READ_LOC(vMid.z);

	int iMinLoci,iMaxLoci,iMinLocj,iMaxLocj,iMinLock,iMaxLock;

	getMinMaxs(iLoc,jLoc,kLoc,&iMinLoci,&iMinLocj,&iMinLock,&iMaxLoci,&iMaxLocj,&iMaxLock);

	for ( i = iMinLoci; i <= iMaxLoci; i++ )
	{
		for ( j = iMinLocj; j <= iMaxLocj; j++ )
		{
			for ( k = iMinLock; k <= iMaxLock; k++ )
			{
				FindNearestBlastInBucket(i,j,k,vOrigin,vSrc,&fNearestDist,&iNearestIndex,iIgnoreWpt,bGetVisible,bGetUnReachable,bIsBot,bNearestAimingOnly,iTeam,bCheckArea,fBlastRadius);
			}
		}
	}

	return iNearestIndex;
}

///////////////////////////////////////////////
// find a waypoint I can fire a blast (e.g. rpg or grenade to)
void CWaypointLocations :: FindNearestBlastInBucket ( int i, int j, int k, const Vector &vOrigin, 
													 const Vector &vSrc, float *pfMinDist, 
													 int *piIndex, int iIgnoreWpt, bool bGetVisible, 
													 bool bGetUnReachable, bool bIsBot, 
													 bool bNearestAimingOnly, int iTeam, 
													 bool bCheckArea, float fBlastRadius )
// Search for the nearest waypoint : I.e.
// Find the waypoint that is closest to vOrigin from the distance pfMinDist
// And set the piIndex to the waypoint index if closer.
{
	//dataStack <int> tempStack = m_iLocations[i][j][k];

	CWaypoint *curr_wpt;
	int iSelectedIndex;
	float fDist;
//	int iWptFlags;

	trace_t tr;
	
	bool bAdd;
	
	dataUnconstArray<int> *arr = &(m_iLocations[i][j][k]);
	short int size = static_cast<short int>(arr->Size());
	CBotMod *curmod = CBotGlobals::getCurrentMod();

	for ( register short int l = 0; l < size; l ++ )
	//while ( !tempStack.IsEmpty() )
	{
		iSelectedIndex = arr->ReturnValueFromIndex(l);//tempStack.ChooseFromStack();

		if ( iSelectedIndex == iIgnoreWpt )
			continue;

		curr_wpt = CWaypoints::getWaypoint(iSelectedIndex);

		if ( !bGetUnReachable && curr_wpt->hasFlag(CWaypointTypes::W_FL_UNREACHABLE) )
			continue;

		if ( !curr_wpt->isUsed() )
			continue;

		if ( !curr_wpt->forTeam(iTeam) )
		{
			continue;
		}
		// DOD:S compatibility
		if (bCheckArea && !curmod->isWaypointAreaValid(curr_wpt->getArea(), curr_wpt->getFlags()))
			continue;

		if ( bIsBot )
		{
			if ( curr_wpt->getFlags() & (CWaypointTypes::W_FL_DOUBLEJUMP | CWaypointTypes::W_FL_ROCKET_JUMP | CWaypointTypes::W_FL_JUMP) ) // fix : BIT OR
				continue;
		}

		if ( curr_wpt->distanceFrom(vOrigin) < (fBlastRadius*2) )
		{
			if ( (fDist = (curr_wpt->distanceFrom(vSrc)+curr_wpt->distanceFrom(vOrigin))) < *pfMinDist )
			{
				bAdd = false;
				
				if ( bGetVisible == false )
					bAdd = true;
				else
				{
					bAdd = CBotGlobals::isVisible(vSrc,curr_wpt->getOrigin()) && CBotGlobals::isVisible(vOrigin,curr_wpt->getOrigin());
				}
				
				if ( bAdd )
				{
					*piIndex = iSelectedIndex;
					*pfMinDist = fDist;
				}
			}
		}
	}
}

///////////////////////////////////////////////
//

void CWaypointLocations :: FindNearestInBucket ( int i, int j, int k, const Vector &vOrigin,
												float *pfMinDist, int *piIndex, int iIgnoreWpt, 
												bool bGetVisible, bool bGetUnReachable, bool bIsBot, 
												dataUnconstArray<int> *iFailedWpts, bool bNearestAimingOnly, 
												int iTeam, bool bCheckArea, bool bGetVisibleFromOther, 
												Vector vOther, int iFlagsOnly, edict_t *pPlayer )
// Search for the nearest waypoint : I.e.
// Find the waypoint that is closest to vOrigin from the distance pfMinDist
// And set the piIndex to the waypoint index if closer.
{
	//dataStack <int> tempStack = m_iLocations[i][j][k];

	CWaypoint *curr_wpt;
	int iSelectedIndex;
	float fDist;
//	int iWptFlags;

	trace_t tr;
	
	bool bAdd;
	CBotMod *curmod = CBotGlobals::getCurrentMod();
	
	dataUnconstArray<int> *arr = &(m_iLocations[i][j][k]);
	short int size = static_cast<short int>(arr->Size());
	
	for ( int l = 0; l < size; l ++ )
	//while ( !tempStack.IsEmpty() )
	{
		iSelectedIndex = arr->ReturnValueFromIndex(l);//tempStack.ChooseFromStack();

		if ( iSelectedIndex == iIgnoreWpt )
			continue;
		if ( g_iFailedWaypoints[iSelectedIndex] == 1 )
		{
			g_iFailedWaypoints[iSelectedIndex] = 2; // flag this 
			continue;
		}

		curr_wpt = CWaypoints::getWaypoint(iSelectedIndex);

		if ( !bGetUnReachable && curr_wpt->hasFlag(CWaypointTypes::W_FL_UNREACHABLE) )
			continue;

		if ( !curr_wpt->isUsed() )
			continue;

		if ( bIsBot )
		{
			if ( curr_wpt->hasFlag(CWaypointTypes::W_FL_OWNER_ONLY) )
			{
				if ( !curmod->isAreaOwnedByTeam(curr_wpt->getArea(),iTeam) )
					continue;
			}
			else if ( !curr_wpt->forTeam(iTeam) )
			{
				continue;
			}
		}
		// DOD:S compatibility
		if (bCheckArea && !curmod->isWaypointAreaValid(curr_wpt->getArea(), curr_wpt->getFlags()))
			continue;;

		if ( iFlagsOnly != 0 )
		{
			if ( !curr_wpt->getFlags() || (!curr_wpt->hasSomeFlags(iFlagsOnly)) )
				continue;
		}

		if ( bIsBot )
		{
			if (curr_wpt->getFlags() & (CWaypointTypes::W_FL_DOUBLEJUMP | CWaypointTypes::W_FL_ROCKET_JUMP |
				CWaypointTypes::W_FL_JUMP | CWaypointTypes::W_FL_OPENS_LATER)) // fix : bit OR
				continue;
		}

		// Used to ignore waypoints where objects are e.g. Sentry guns
		if ( m_bIgnoreBox )
		{
			Vector vcomp = curr_wpt->getOrigin() - vOrigin;
			vcomp = vcomp / vcomp.Length();
			
			if ( curr_wpt->distanceFrom(m_vIgnoreLoc) < m_fIgnoreSize )
				continue;
			else if ( ((vOrigin + (vcomp*((vOther-vOrigin).Length()))) - vOther).Length() < m_fIgnoreSize )
				continue;
		}

		if ( (fDist = curr_wpt->distanceFrom(vOrigin)) < *pfMinDist )
		{
			bAdd = false;
			
			if ( bGetVisible == false )
				bAdd = true;
			else
			{
				if ( bGetVisibleFromOther )
					bAdd = CBotGlobals::isVisible(vOther,curr_wpt->getOrigin());
				else
				{
					if ( pPlayer != NULL )
					{
						CBotGlobals::quickTraceline(pPlayer,vOrigin,curr_wpt->getOrigin());
						bAdd = CBotGlobals::getTraceResult()->fraction>=1.0f;
					}
					else
						bAdd = CBotGlobals::isVisible(vOrigin,curr_wpt->getOrigin());
				}
			}
			
			if ( bAdd )
			{
				*piIndex = iSelectedIndex;
				*pfMinDist = fDist;
			}
		}
	}
}

/////////////////////////////
// get the nearest waypoint INDEX from an origin
int CWaypointLocations :: NearestWaypoint ( const Vector &vOrigin, float fNearestDist, 
										   int iIgnoreWpt, bool bGetVisible, bool bGetUnReachable, 
										   bool bIsBot, dataUnconstArray<int> *iFailedWpts, 
										   bool bNearestAimingOnly, int iTeam, bool bCheckArea,
										   bool bGetVisibleFromOther, const Vector vOther, int iFlagsOnly, 
										   edict_t *pPlayer, bool bIgnorevOther, float fIgnoreSize )
{
	int iNearestIndex = -1;
	
	int iLoc = READ_LOC(vOrigin.x);
	int jLoc = READ_LOC(vOrigin.y);
	int kLoc = READ_LOC(vOrigin.z);

	int i,j,k;

	int iMinLoci,iMaxLoci,iMinLocj,iMaxLocj,iMinLock,iMaxLock;

	getMinMaxs(iLoc,jLoc,kLoc,&iMinLoci,&iMinLocj,&iMinLock,&iMaxLoci,&iMaxLocj,&iMaxLock);

	m_bIgnoreBox = bIgnorevOther;
	m_vIgnoreLoc = vOther;
	m_fIgnoreSize = fIgnoreSize;

	if ( !bNearestAimingOnly )
	{
		Q_memset(g_iFailedWaypoints,0,sizeof(unsigned char)*CWaypoints::MAX_WAYPOINTS);
		
		if ( iFailedWpts )
		{   
			int iWpt;
			
			for ( int l = 0; l < iFailedWpts->Size(); l ++ )
			{
				if ( (iWpt=(*iFailedWpts)[l]) != -1 ) //( (iWpt = tempStack.ChooseFromStack()) != -1 )
					g_iFailedWaypoints[iWpt] = 1;
			}
		}
	}

	for ( i = iMinLoci; i <= iMaxLoci; i++ )
	{
		for ( j = iMinLocj; j <= iMaxLocj; j++ )
		{
			for ( k = iMinLock; k <= iMaxLock; k++ )
			{
				FindNearestInBucket(i,j,k,vOrigin,&fNearestDist,&iNearestIndex,iIgnoreWpt,bGetVisible,bGetUnReachable,bIsBot,iFailedWpts,bNearestAimingOnly,iTeam,bCheckArea,bGetVisibleFromOther,vOther,iFlagsOnly,pPlayer);
			}
		}
	}

	if ( iFailedWpts )
	{   
		int iWpt;
		
		for ( int l = 0; l < iFailedWpts->Size(); l ++ )
		{
			if ( (iWpt=(*iFailedWpts)[l]) != -1 ) //( (iWpt = tempStack.ChooseFromStack()) != -1 )
			{
				if ( g_iFailedWaypoints[iWpt] == 2 )
				{
					iFailedWpts->Remove(iWpt);
				}
			}
		}
	}

	m_bIgnoreBox = false;

	return iNearestIndex;
}

//////////////////////////////////
// Draw waypoints around a player
void CWaypointLocations :: DrawWaypoints ( CClient *pClient, float fDist )
{
	static byte m_bPvs[MAX_MAP_CLUSTERS/8];
	static int clusterIndex;
	static dataUnconstArray<int> *arr;
	static short int size;
	static int iWpt;
	static CWaypoint *pWpt;
	static int i,j,k;
	static Vector vWpt;
	//static bool bDrawPaths;
	static unsigned short int iDrawType;
	static Vector vOrigin;
	static edict_t *pEntity;
	static int iMinLoci,iMaxLoci,iMinLocj,iMaxLocj,iMinLock,iMaxLock;

	int iLoc = READ_LOC(vOrigin.x);
	int jLoc = READ_LOC(vOrigin.y);
	int kLoc = READ_LOC(vOrigin.z);

	pEntity = pClient->getPlayer();
	vOrigin = pClient->getOrigin();
	//bDrawPaths = false;
	iDrawType = pClient->getDrawType();

	getMinMaxs(iLoc,jLoc,kLoc,&iMinLoci,&iMinLocj,&iMinLock,&iMaxLoci,&iMaxLocj,&iMaxLock);


	for ( i = iMinLoci; i <= iMaxLoci; i++ )
	{
		for ( j = iMinLocj; j <= iMaxLocj; j++ )
		{
			for ( k = iMinLock; k <= iMaxLock; k++ )
			{
				arr = &(m_iLocations[i][j][k]);
				size = static_cast<short int>(arr->Size());
				
				for ( short int l = 0; l < size; l ++ )
				//while ( !tempStack.IsEmpty() )
				{
					iWpt = arr->ReturnValueFromIndex(l);//tempStack.ChooseFromStack();

					pWpt = CWaypoints::getWaypoint(iWpt);//tempStack.ChooseFromStack());

					if ( !pWpt->isUsed() ) // deleted
						continue;

					if ( !pClient->isShowingAllWaypoints() )
					{
						if ( pWpt->getFlags() == 0 )
							continue; // hidden
						else if ( !pClient->isShowingWaypoint(pWpt->getFlags()) )
							continue;
					}

					vWpt = pWpt->getOrigin();

					if ( fabs(vWpt.z - vOrigin.z) <= 256.0 ) // also in z range
					{
						//if ( CBotGlobals::FInViewCone(pEntity) )
						{
							// from Valve developer community wiki
							// http://developer.valvesoftware.com/wiki/Transforming_the_Multiplayer_SDK_into_Coop

							clusterIndex = engine->GetClusterForOrigin( vOrigin );
							engine->GetPVSForCluster( clusterIndex, sizeof(m_bPvs), m_bPvs );							

							if ( engine->CheckOriginInPVS( vWpt, m_bPvs, sizeof( m_bPvs ) ) )
								pWpt->draw(pEntity,pClient->isPathWaypointOn()&&(pClient->currentWaypoint()==iWpt),iDrawType);
						}
					}
				}
			}
		}
	}
}

void CWaypointLocations ::AddWptLocation (CWaypoint *pWaypoint, int iIndex)
{
	Vector vOrigin = pWaypoint->getOrigin();
	float flOrigin[3] = { vOrigin.x, vOrigin.y, vOrigin.z };

	AddWptLocation(iIndex,flOrigin);
}
