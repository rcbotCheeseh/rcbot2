#include "engine_wrappers.h"

#include "bot.h"
#include "bot_zombie.h"
#include "bot_globals.h"
#include "bot_waypoint.h"
#include "bot_waypoint_locations.h"
#include "bot_schedule.h"

bool CBotZombie :: isEnemy ( edict_t *pEdict,bool bCheckWeapons )
{
	if ( pEdict == m_pEdict )
		return false;

	if ( !ENTINDEX(pEdict) || (ENTINDEX(pEdict) > CBotGlobals::maxClients()) )
		return false;

	if ( CBotGlobals::getTeamplayOn() )
	{
		if ( CBotGlobals::getTeam(pEdict) == getTeam() )
			return false;
	}

	return true;	
}

void CBotZombie :: modThink ( void )
{
	//
}

void CBotZombie :: getTasks ( unsigned int iIgnore )
{
	if ( m_pEnemy )
	{
		m_pSchedules->add(new CBotGotoOriginSched(m_pEnemy));
	}
	else
		CBot::getTasks();
}