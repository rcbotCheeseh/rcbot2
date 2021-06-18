#include "engine_wrappers.h"

#include "bot_wpt_dist.h"
#include "bot_globals.h"
#include "bot_waypoint.h"

typedef struct
{
	int version;
	int numwaypoints;
	int maxwaypoints;
}wpt_dist_hdr_t;

int CWaypointDistances::m_Distances [CWaypoints::MAX_WAYPOINTS][CWaypoints::MAX_WAYPOINTS];
float CWaypointDistances::m_fSaveTime = 0;

void CWaypointDistances :: load ()
{
	char filename[1024];	
	wpt_dist_hdr_t hdr;
	char *szMapName = CBotGlobals::getMapName();

	if ( szMapName  && *szMapName )
	{
		CBotGlobals::buildFileName(filename,szMapName,BOT_WAYPOINT_FOLDER,BOT_WAYPOINT_DST_EXTENSION,true);

		FILE *bfp = CBotGlobals::openFile(filename,"rb");

		if ( bfp == nullptr )
		{
			return; // give up
		}

		fread(&hdr,sizeof(wpt_dist_hdr_t),1,bfp);

		if ( hdr.maxwaypoints == CWaypoints::MAX_WAYPOINTS && hdr.numwaypoints == CWaypoints::numWaypoints() && hdr.version == WPT_DIST_VER )
		{
			fread(m_Distances,sizeof(int),CWaypoints::MAX_WAYPOINTS * CWaypoints::MAX_WAYPOINTS,bfp);
		}

		m_fSaveTime = engine->Time() + 100.0f;

		fclose(bfp);
	}
}

void CWaypointDistances :: save ()
{
	//if ( m_fSaveTime < engine->Time() )
	//{
		char filename[1024];	
		char *szMapName = CBotGlobals::getMapName();

		if ( szMapName && *szMapName )
		{
			wpt_dist_hdr_t hdr;

			CBotGlobals::buildFileName(filename,szMapName,BOT_WAYPOINT_FOLDER,BOT_WAYPOINT_DST_EXTENSION,true);

			FILE *bfp = CBotGlobals::openFile(filename,"wb");

			if ( bfp == nullptr )
			{
				m_fSaveTime = engine->Time() + 100.0f;
				return; // give up
			}

			hdr.maxwaypoints = CWaypoints::MAX_WAYPOINTS;
			hdr.numwaypoints = CWaypoints::numWaypoints();
			hdr.version = WPT_DIST_VER;

			fwrite(&hdr,sizeof(wpt_dist_hdr_t),1,bfp);

			fwrite(m_Distances,sizeof(int),CWaypoints::MAX_WAYPOINTS * CWaypoints::MAX_WAYPOINTS,bfp);

			m_fSaveTime = engine->Time() + 100.0f;

			fclose(bfp);
		}
	//}
}

float CWaypointDistances :: getDistance ( int iFrom, int iTo )
{
	if ( m_Distances[iFrom][iTo] == -1 )
		return (CWaypoints::getWaypoint(iFrom)->getOrigin()-CWaypoints::getWaypoint(iTo)->getOrigin()).Length();

	return (float)m_Distances[iFrom][iTo];
}