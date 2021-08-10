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
#include "bot_cvars.h"
#include "ndebugoverlay.h"
#include "bot_squads.h"
#include "bot_css_bot.h"
#include "in_buttons.h"
#include "bot_buttons.h"
#include "bot_globals.h"
#include "bot_profile.h"
#include "bot_getprop.h"
#include "bot_mtrand.h"
#include "bot_mods.h"
#include "bot_task.h"
#include "bot_schedule.h"
#include "bot_weapons.h"
#include "bot_waypoint.h"
#include "bot_waypoint_locations.h"
#include "bot_navigator.h"
#include "bot_perceptron.h"
#include "bot_plugin_meta.h"
#include "bot_waypoint_visibility.h"

void CCSSBot::init(bool bVarInit)
{
	CBot::init();// require this

	// initialize stuff for counter-strike source bot
}

void CCSSBot::setup()
{
	CBot::setup(); // require this

	// setup data structures for counter-strike source bot
	// this only gets called ONCE (when bot joins the game)

	engine->SetFakeClientConVarValue(m_pEdict,"cl_team","default");
	engine->SetFakeClientConVarValue(m_pEdict,"cl_autohelp","0");
}

bool CCSSBot::isAlive()
{
	if (!CBot::isAlive())
		return false;
	return (getOrigin() != Vector(0,0,0));
}

bool CCSSBot::isEnemy(edict_t *pEdict,bool bCheckWeapons)
{
	if (ENTINDEX(pEdict) > CBotGlobals::maxClients())
		return false;
	if (pEdict->IsFree())
		return false;

	if (!CBotGlobals::isNetworkable(pEdict))
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

bool CCSSBot::startGame()
{
	const int team = m_pPlayerInfo->GetTeamIndex();

	if(team <= CCounterStrikeSourceMod::CS_TEAM_SPECTATOR)
	{
		selectTeam();
		selectModel();
	}

	return true;
}

void CCSSBot::died(edict_t *pKiller, const char *pszWeapon)
{
	spawnInit();
}

void CCSSBot::spawnInit()
{
	CBot::spawnInit();

	if ( m_pSchedules )
		m_pSchedules->add(new CBotSchedule(new CAutoBuy()));
	m_fCheckStuckTime = engine->Time() + 6.0;
}

void CCSSBot::selectTeam() const
{
	const char* cmd = "jointeam 0";
	helpers->ClientCommand(m_pEdict,cmd);
}

void CCSSBot::selectModel() const
{
	const char* cmd = "joinclass 0";
	helpers->ClientCommand(m_pEdict,cmd);
}
