#ifndef __BOT_WPT_DIST_H__
#define __BOT_WPT_DIST_H__

#include "bot_waypoint.h"

enum
{
	WPT_DIST_VER = 0x03
};

#define BOT_WAYPOINT_DST_EXTENSION "rcd"

class CWaypointDistances
{
public:
	CWaypointDistances()
	{
		m_fSaveTime = 0.0f;
	}

	static float getDistance ( int iFrom, int iTo );

	static bool isSet ( int iFrom, int iTo )
	{
		return m_Distances[iFrom][iTo] >= 0;
	}

	static void setDistance ( int iFrom, int iTo, float fDist )
	{
		m_Distances[iFrom][iTo] = static_cast<int>(fDist);
	}

	static void load ();

	static void save ();

	static void reset ()
	{
		memset(m_Distances,0xFF,sizeof(int)*CWaypoints::MAX_WAYPOINTS*CWaypoints::MAX_WAYPOINTS);
	}
private:
	static int m_Distances [CWaypoints::MAX_WAYPOINTS][CWaypoints::MAX_WAYPOINTS];
	static float m_fSaveTime;

};



#endif