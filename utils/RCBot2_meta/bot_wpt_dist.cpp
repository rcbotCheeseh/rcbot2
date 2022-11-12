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
	wpt_dist_hdr_t hdr;
	const char *szMapName = CBotGlobals::getMapName();

	if ( szMapName  && *szMapName )
	{
		char filename[1024];
		CBotGlobals::buildFileName(filename,szMapName,BOT_WAYPOINT_FOLDER,BOT_WAYPOINT_DST_EXTENSION,true);

		std::fstream bfp = CBotGlobals::openFile(filename, std::fstream::in | std::fstream::binary);

		if ( !bfp )
		{
			return; // give up
		}

		bfp.read(reinterpret_cast<char*>(&hdr), sizeof(wpt_dist_hdr_t));

		if ( (hdr.maxwaypoints == CWaypoints::MAX_WAYPOINTS) && (hdr.numwaypoints == CWaypoints::numWaypoints()) && (hdr.version == WPT_DIST_VER) )
		{
			bfp.read(reinterpret_cast<char*>(m_Distances), sizeof(int) * CWaypoints::MAX_WAYPOINTS * CWaypoints::MAX_WAYPOINTS);
		}

		m_fSaveTime = engine->Time() + 100.0f;
	}
}

void CWaypointDistances :: save ()
{
	const char *szMapName = CBotGlobals::getMapName();

		if ( szMapName && *szMapName )
		{
			char filename[1024];
			wpt_dist_hdr_t hdr;

			CBotGlobals::buildFileName(filename,szMapName,BOT_WAYPOINT_FOLDER,BOT_WAYPOINT_DST_EXTENSION,true);

			std::fstream bfp = CBotGlobals::openFile(filename, std::fstream::out | std::fstream::binary);

			if ( !bfp )
			{
				m_fSaveTime = engine->Time() + 100.0f;
				return; // give up
			}

			hdr.maxwaypoints = CWaypoints::MAX_WAYPOINTS;
			hdr.numwaypoints = CWaypoints::numWaypoints();
			hdr.version = WPT_DIST_VER;

			bfp.write(reinterpret_cast<char*>(&hdr), sizeof(wpt_dist_hdr_t));

			bfp.write(reinterpret_cast<char*>(m_Distances), sizeof(int) * CWaypoints::MAX_WAYPOINTS * CWaypoints::MAX_WAYPOINTS);

			m_fSaveTime = engine->Time() + 100.0f;
		}
	//}
}

float CWaypointDistances :: getDistance ( int iFrom, int iTo )
{
	if ( m_Distances[iFrom][iTo] == -1 )
		return (CWaypoints::getWaypoint(iFrom)->getOrigin()-CWaypoints::getWaypoint(iTo)->getOrigin()).Length();

	return static_cast<float>(m_Distances[iFrom][iTo]);
}
