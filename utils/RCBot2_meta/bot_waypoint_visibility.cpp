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
#include "bot_waypoint.h"
#include "bot_waypoint_visibility.h"
#include "bot_globals.h"
#include <stdio.h>

/*unsigned char *CWaypointVisibilityTable :: m_VisTable = NULL;
bool CWaypointVisibilityTable :: bWorkVisibility = false;
int CWaypointVisibilityTable :: iCurFrom = 0;
int CWaypointVisibilityTable :: iCurTo = 0;*/

void CWaypointVisibilityTable :: workVisibility ()
{		
	int percent;
	int iTicks = 0;
	register unsigned short int iSize = (unsigned short int) CWaypoints::numWaypoints();

	for ( iCurFrom = iCurFrom; iCurFrom < iSize; iCurFrom ++ )
	{
		for ( iCurTo = iCurTo; iCurTo < iSize; iCurTo ++ )
		{
			CWaypoint *pWaypoint1 = CWaypoints::getWaypoint(iCurFrom);
			CWaypoint *pWaypoint2 = CWaypoints::getWaypoint(iCurTo);

			SetVisibilityFromTo(iCurFrom,iCurTo,CBotGlobals::isVisible(pWaypoint1->getOrigin(),pWaypoint2->getOrigin()));

			iTicks++;

			if ( iTicks >= WAYPOINT_VIS_TICKS )
			{
				if ( m_fNextShowMessageTime < engine->Time() )
				{
					percent = (int)(((float)iCurFrom / iSize) * 100);

					if ( m_iPrevPercent != percent )
					{
						Msg(" *** working out visibility %d percent***\n",percent);
						m_fNextShowMessageTime = engine->Time() + 2.5f;
						m_iPrevPercent = percent;
					}
				}

				return;
			}
		}

		iCurTo = 0;
	}

	if ( iCurFrom == iSize )
	{
		// finished
		Msg(" *** finished working out visibility ***\n");
		/////////////////////////////
		// for "concurrent" reading of 
		// visibility throughout frames
		bWorkVisibility = false;
		iCurFrom = 0;
		iCurTo = 0;

		// save waypoints with visibility flag now
		if ( SaveToFile() )
		{
			CWaypoints::save(true);
			Msg(" *** saving waypoints with visibility information ***\n");
		}
		else
			Msg(" *** error, couldn't save waypoints with visibility information ***\n");
		////////////////////////////
	}
}

void CWaypointVisibilityTable :: workVisibilityForWaypoint ( int i, int iNumWaypoints, bool bTwoway )
{
	static CWaypoint *Waypoint1;
	static CWaypoint *Waypoint2;
	static bool bVisible;

	Waypoint1 = CWaypoints::getWaypoint(i);

	if ( !Waypoint1->isUsed() )
		return;

	for ( register short int j = 0; j < iNumWaypoints; j ++ )
	{
		if ( i == j )
		{
			SetVisibilityFromTo(i,j,1);
			continue;
		}

		Waypoint2 = CWaypoints::getWaypoint(j);

		if ( !Waypoint2->isUsed() )
			continue;

		bVisible = CBotGlobals::isVisible(Waypoint1->getOrigin(),Waypoint2->getOrigin());

		SetVisibilityFromTo(i,j,bVisible);

		if ( bTwoway )
			SetVisibilityFromTo(j,i,bVisible);
	}
}

void CWaypointVisibilityTable :: WorkOutVisibilityTable ()
{
	register short int i;

	int iNumWaypoints = CWaypoints::numWaypoints();

	ClearVisibilityTable();

	// loop through all waypoint possibilities.
	for ( i = 0; i < iNumWaypoints; i ++ )
	{
		workVisibilityForWaypoint(i,iNumWaypoints,false);
	}
}

bool CWaypointVisibilityTable :: SaveToFile ( void )
{
    char filename[1024];
	wpt_vis_header_t header;

	CBotGlobals::buildFileName(filename,CBotGlobals::getMapName(),BOT_WAYPOINT_FOLDER,"rcv",true);

	FILE *bfp = CBotGlobals::openFile(filename,"wb");

   if ( bfp == NULL )
   {
	   CBotGlobals::botMessage(NULL,0,"Can't open Waypoint Visibility table for writing!");
	   return false;
   }

	header.numwaypoints = CWaypoints::numWaypoints();
	strncpy(header.szMapName,CBotGlobals::getMapName(),63);
	header.waypoint_version = CWaypoints::WAYPOINT_VERSION;

	fwrite(&header,sizeof(wpt_vis_header_t),1,bfp);
	fwrite(m_VisTable,sizeof(byte),g_iMaxVisibilityByte,bfp);

   fclose(bfp);

   return true;
}

bool CWaypointVisibilityTable :: ReadFromFile ( int numwaypoints )
{
    char filename[1024];

	wpt_vis_header_t header;

	CBotGlobals::buildFileName(filename,CBotGlobals::getMapName(),BOT_WAYPOINT_FOLDER,"rcv",true);

   FILE *bfp =  CBotGlobals::openFile(filename,"rb");

   if ( bfp == NULL )
   {
	   Msg(" *** Can't open Waypoint Visibility table for reading!\n");
	   return false;
   }

   fread(&header,sizeof(wpt_vis_header_t),1,bfp);

   if ( header.numwaypoints != numwaypoints )
	   return false;
   if ( header.waypoint_version != CWaypoints::WAYPOINT_VERSION )
	   return false;
   if ( strncmp(header.szMapName,CBotGlobals::getMapName(),63) )
	   return false;

   fread(m_VisTable,sizeof(byte),g_iMaxVisibilityByte,bfp);

   fclose(bfp);

   return true;
}