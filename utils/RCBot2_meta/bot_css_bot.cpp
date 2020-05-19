#include "bot.h"
#include "bot_css_bot.h"
#include "bot_schedule.h"
#include "bot_globals.h"

#include "bot_mtrand.h"

void CCSSBot :: init ()
{
	CBot::init();// require this

	// initialize stuff for counter-strike source bot
}

void CCSSBot :: setup ()
{
	CBot::setup(); // require this

	// setup data structures for counter-strike source bot
	// this only gets called ONCE (when bot joins the game)

	engine->SetFakeClientConVarValue(m_pEdict,"cl_team","default");
	engine->SetFakeClientConVarValue(m_pEdict,"cl_autohelp","0");
}

bool CCSSBot :: isAlive ()
{
	if ( !CBot::isAlive() )
		return false;
	return (getOrigin() != Vector(0,0,0));
}

bool CCSSBot :: isEnemy ( edict_t *pEdict,bool bCheckWeapons )
{
	if ( ENTINDEX(pEdict) > CBotGlobals::maxClients() )
		return false;
	if ( pEdict->IsFree() )
		return false;

	if ( !CBotGlobals::isNetworkable(pEdict) )
		return false;
 
	IPlayerInfo *p = playerinfomanager->GetPlayerInfo(pEdict);

	if ( p == NULL )
		return false;
	if ( m_pEdict == pEdict )
		return false;
	if ( !CBotGlobals::entityIsAlive ( pEdict ) )
		return false;

	return (p->GetTeamIndex() != getTeam());
}

bool CCSSBot :: startGame ()
{
	// do whatever is necessary here to join the game...
	IPlayerInfo* pInfo = playerinfomanager->GetPlayerInfo(m_pEdict);

	if ( pInfo->GetTeamIndex() == 0 )
	{
		pInfo->ChangeTeam(randomInt(2,3));
	}

	return (pInfo->GetTeamIndex() != 0);
}

void CCSSBot :: died ()
{
	spawnInit();
}

void CCSSBot :: spawnInit ()
{
	CBot::spawnInit();

	if ( m_pSchedules )
		m_pSchedules->add(new CBotSchedule(new CAutoBuy()));
	m_fCheckStuckTime = engine->Time() + 6.0;
}