#ifndef __BOT_WPT_DIST_H__
#define __BOT_WPT_DIST_H__

#include "bot_waypoint.h"

#define WPT_DIST_VER 0x03

#define BOT_WAYPOINT_DST_EXTENSION "rcd"

class CWaypointDistances
{
public:
	CWaypointDistances()
	{
		m_fSaveTime = 0;
	}

	static float getDistance ( int iFrom, int iTo );

	static inline bool isSet ( int iFrom, int iTo )
	{
		return m_Distances[iFrom][iTo] >= 0;
	}

	static inline void setDistance ( int iFrom, int iTo, float fDist )
	{
		m_Distances[iFrom][iTo] = (int)fDist;
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