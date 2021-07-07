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
#include "bot_cvars.h"
#include "bot_client.h"
#include "bot_strings.h"
#include "bot_commands.h"
#include "bot_globals.h"
#include "bot_accessclient.h"
#include "bot_schedule.h"
#include "bot_waypoint.h" // for waypoint commands
#include "bot_waypoint_locations.h" // for waypoint commands
#include "ndebugoverlay.h"
#include "bot_waypoint_visibility.h"
#include "bot_getprop.h"
#include "bot_weapons.h"
#include "bot_menu.h"

#include "bot_tf2_points.h"

extern IVDebugOverlay *debugoverlay;

// include our subcommands
#include "rcbot_subcmds/config.cpp"
#include "rcbot_subcmds/debug.cpp"
#include "rcbot_subcmds/pathwaypoint.cpp"
#include "rcbot_subcmds/users.cpp"
#include "rcbot_subcmds/util.cpp"
#include "rcbot_subcmds/waypoint.cpp"

// temporarily declared at the bottom
// CBotSubcommands *CBotGlobals :: m_pCommands;

eBotCommandResult CBotCommandInline::execute(CClient *pClient, BotCommandArgs args) {
	// fire off callback function
	return m_Callback(pClient, args);
}

CBotCommandInline ControlCommand("control", CMD_ACCESS_BOT | CMD_ACCESS_DEDICATED, [](CClient *pClient, BotCommandArgs args)
{
	edict_t *pEntity = NULL;

	if ( pClient )
		pEntity = pClient->getPlayer();
	if ( args[0] && *args[0] )
	{

		if ( CBots::controlBot(args[0],args[0],args[2],args[3]) )
			CBotGlobals::botMessage(pEntity,0,"bot added");
		else
			CBotGlobals::botMessage(pEntity,0,"error: couldn't control bot '%s'",args[0]);

		return COMMAND_ACCESSED;

	}
	else
		return COMMAND_ERROR;
});

CBotCommandInline AddBotCommand("addbot", CMD_ACCESS_BOT | CMD_ACCESS_DEDICATED, [](CClient *pClient, BotCommandArgs args)
{	
//	bool bOkay = false;

	edict_t *pEntity = NULL;

	if ( pClient )
		pEntity = pClient->getPlayer();

	//if ( !bot_sv_cheat_warning.GetBool() || bot_sv_cheats_auto.GetBool() || (!sv_cheats || sv_cheats->GetBool()) )
	//{
		//if ( !args[0] || !*args[0] )
		//	bOkay = CBots::createBot();
		//else
		//bOkay = CBots::createBot();

		if ( CBots::createBot(args[0],args[1],args[2]) )
			CBotGlobals::botMessage(pEntity,0,"bot adding...");
		else
			CBotGlobals::botMessage(pEntity,0,"error: couldn't create bot! (Check maxplayers)");
	//}
	//else
	//	CBotGlobals::botMessage(pEntity,0,"error: sv_cheats must be 1 to add bots");

	return COMMAND_ACCESSED;
});

CBotCommandInline KickBotCommand("kickbot", CMD_ACCESS_BOT | CMD_ACCESS_DEDICATED, [](CClient *pClient, BotCommandArgs args)
{
	if ( !args[0] || !*args[0] )
	{
		//remove random bot
		CBots::kickRandomBot();
	}
	else
	{
		int team = atoi(args[0]);

		CBots::kickRandomBotOnTeam(team);
	}

	
	return COMMAND_ACCESSED;
}, "usage \"kickbot\" or \"kickbot <team>\" : kicks random bot or bot on team: <team>");

bool CBotCommand :: hasAccess ( CClient *pClient )
{
	// check access level excluding dedicated server flag
	int iClientAccessLevel = this->m_iAccessLevel & ~CMD_ACCESS_DEDICATED;
	return (iClientAccessLevel & pClient->accessLevel()) == iClientAccessLevel;
}

bool CBotCommand :: isCommand ( const char *szCommand )
{
	return FStrEq(szCommand,m_szCommand);
}

eBotCommandResult CBotCommand::execute(CClient *pClient, BotCommandArgs args) {
	return COMMAND_NOT_FOUND;
}

eBotCommandResult CBotSubcommands::execute(CClient *pClient, BotCommandArgs args) {
	const char* subcmd = args[0];
	args.pop_front();
	
	for (auto cmd : m_theCommands) {
		if (!cmd->isCommand(subcmd)) {
			continue;
		}
		
		if (pClient && !cmd->hasAccess(pClient)) {
			return COMMAND_REQUIRE_ACCESS;
		}
		
		if (!pClient && !cmd->canbeUsedDedicated()){
			CBotGlobals::botMessage(nullptr, 0, "Sorry, this command cannot be used on a dedicated server");
			return COMMAND_ERROR;
		}
		
		// shift arguments and call
		eBotCommandResult result = cmd->execute(pClient, args);
		if (result == COMMAND_ERROR) {
			cmd->printHelp(pClient? pClient->getPlayer() : nullptr);
		}
		return COMMAND_ACCESSED;
	}
	
	printHelp(pClient? pClient->getPlayer() : nullptr);
	return COMMAND_NOT_FOUND;
}

void CBotSubcommands::printCommand(edict_t *pPrintTo, int indent)
{
	if ( indent )
	{
		const int maxIndent = 64;
		char szIndent[maxIndent];

		int i;

		for ( i = 0; (i < (indent*2)) && (i < maxIndent-1); i ++ )
			szIndent[i] = ' ';

		szIndent[maxIndent-1] = 0;
		szIndent[i]=0;

		CBotGlobals::botMessage(pPrintTo,0,"%s[%s]",szIndent,m_szCommand);
	}
	else
		CBotGlobals::botMessage(pPrintTo,0,"[%s]",m_szCommand);

	for ( unsigned int i = 0; i < m_theCommands.size(); i ++ )
	{
		m_theCommands[i]->printCommand(pPrintTo,indent+1);
	}
}

void CBotSubcommands::printHelp( edict_t *pPrintTo ) {
	this->printCommand(pPrintTo);
}

CBotCommandInline PrintCommands("printcommands", CMD_ACCESS_DEDICATED, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient != NULL )
	{
		CBotGlobals::botMessage(pClient->getPlayer(),0,"All bot commands:");
		CBotGlobals::m_pCommands->printCommand(pClient->getPlayer());
	}
	else
	{
		CBotGlobals::botMessage(NULL,0,"All bot commands:");
		CBotGlobals::m_pCommands->printCommand(NULL);
	}

	return COMMAND_ACCESSED;
});

///////////////////////////////////////////

void CBotCommand :: printCommand ( edict_t *pPrintTo, int indent )
{
	if ( indent )
	{
		const int maxIndent = 64;
		char szIndent[maxIndent];
		int i;

		for ( i = 0; (i < (indent*2)) && (i < maxIndent-1); i ++ )
			szIndent[i] = ' ';

		szIndent[maxIndent-1] = 0;
		szIndent[i]=0;

		if ( !pPrintTo && !canbeUsedDedicated() )
			CBotGlobals::botMessage(pPrintTo,0,"%s%s [can't use]",szIndent,m_szCommand);
		else
			CBotGlobals::botMessage(pPrintTo,0,"%s%s",szIndent,m_szCommand);
	}
	else
	{
		if ( !pPrintTo && !canbeUsedDedicated() )
			CBotGlobals::botMessage(pPrintTo,0,"%s [can't use]",m_szCommand);
		else
			CBotGlobals::botMessage(pPrintTo,0,m_szCommand);
	}
}

void CBotCommand :: printHelp ( edict_t *pPrintTo )
{
	if ( m_szHelp )
		CBotGlobals::botMessage(pPrintTo,0,m_szHelp);
	else
		CBotGlobals::botMessage(pPrintTo,0,"Sorry, no help for this command (yet)");

	return;
}

CBotCommandInline CTestCommand("test", 0, [](CClient *pClient, BotCommandArgs args)
{
	// for developers
	// first argument is at args[0]
	return COMMAND_NOT_FOUND;
});

CBotSubcommands* CBotGlobals::m_pCommands = new CBotSubcommands("rcbot", CMD_ACCESS_DEDICATED, {
	&WaypointSubcommands,
	&AddBotCommand,
	&ControlCommand,
	&PathWaypointSubcommands,
	&DebugSubcommands,
	&PrintCommands,
	&ConfigSubcommands,
	&KickBotCommand,
	&UserSubcommands,
	&UtilSubcommands
});
