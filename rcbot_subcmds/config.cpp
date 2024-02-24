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

CBotCommandInline GameEventVersion("event_version", CMD_ACCESS_CONFIG, [](CClient *pClient, BotCommandArgs args)
{
	if ( !args[0] || !*args[0] )
		return COMMAND_ERROR;

	CBotGlobals::setEventVersion(atoi(args[0]));
	
	return COMMAND_ACCESSED;
});

CBotCommandInline MaxBotsCommand("max_bots", CMD_ACCESS_CONFIG | CMD_ACCESS_DEDICATED, [](CClient *pClient, BotCommandArgs args)
{
	edict_t *pEntity = NULL;

	if ( pClient )
		pEntity = pClient->getPlayer();

	if ( args[0] && *args[0] )
	{
		int max = atoi(args[0]);

		bool err = false;
		int min_bots = CBots::getMinBots();

		if ( max <= -1 )// skip check for disabling max bots (require <=)
			max = -1;
		else if ( (min_bots >= 0) && (max <= min_bots) )
		{
			CBotGlobals::botMessage(pEntity,0,"max_bots must be greater than min_bots (min_bots is currently : %d)",min_bots);
			err = true;
		}
		if ( max > CBotGlobals::maxClients() )				
			max = CBotGlobals::maxClients();

		if ( !err )
		{
			CBots :: setMaxBots(max);

			CBotGlobals::botMessage(pEntity,0,"max_bots set to %d",max);
		}
		
	}
	else
		CBotGlobals::botMessage(pEntity,0,"max_bots is currently %d",CBots::getMaxBots());

	return COMMAND_ACCESSED;
});

CBotCommandInline MinBotsCommand("min_bots", CMD_ACCESS_CONFIG | CMD_ACCESS_DEDICATED, [](CClient *pClient, BotCommandArgs args)
{
	edict_t *pEntity = NULL;

	if ( pClient )
		pEntity = pClient->getPlayer();

	if ( args[0] && *args[0] )
	{
		int min = atoi(args[0]);
		int max_bots = CBots::getMaxBots();

		bool err = false;

		if ( min > CBotGlobals::maxClients() )
			min = CBotGlobals::maxClients();	
		
		if ( min <= -1 ) // skip check for disabling min bots (require <=)
			min = -1;
		else if ( (max_bots >= 0) && (min >= CBots::getMaxBots()) )
		{
			CBotGlobals::botMessage(pEntity,0,"min_bots must be less than max_bots (max_bots is currently : %d)",max_bots);
			err = true;
		}	

		if ( !err )
		{
			CBots :: setMinBots(min);

			CBotGlobals::botMessage(pEntity,0,"min_bots set to %d",min);
		}
	}
	else
		CBotGlobals::botMessage(pEntity,0,"min_bots is currently %d",CBots::getMinBots());

	return COMMAND_ACCESSED;
});

CBotSubcommands ConfigSubcommands("config", CMD_ACCESS_DEDICATED, {
	&GameEventVersion,
	&MaxBotsCommand,
	&MinBotsCommand
});
