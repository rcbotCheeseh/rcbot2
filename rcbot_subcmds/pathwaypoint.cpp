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

CBotCommandInline PathWaypointOnCommand("on", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	pClient->setPathWaypoint(true);
	pClient->giveMessage("Pathwaypoints visible");
	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointOffCommand("off", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	pClient->setPathWaypoint(false);
	pClient->giveMessage("Pathwaypoints hidden");
	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointAutoOnCommand("enable", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
		pClient->setAutoPath(true);
	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointAutoOffCommand("disable", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
		pClient->setAutoPath(false);
	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointCreate1Command("create1", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( !pClient )
		return COMMAND_ERROR;

	pClient->updateCurrentWaypoint();

	if ( pClient->currentWaypoint() == -1 )
	{
		pClient->playSound("common/wpn_denyselect.wav");
	}
	else
	{
		pClient->setPathFrom(pClient->currentWaypoint());

		pClient->playSound("common/wpn_hudoff.wav");
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointCreate2Command("create2", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();
	pClient->setPathTo(pClient->currentWaypoint());

	CWaypoint *pWpt = CWaypoints::getWaypoint(pClient->getPathFrom());

	// valid?
	if ( pWpt )
	{
		pWpt->addPathTo(pClient->getPathTo());
		pClient->playSound("buttons/button9");
	}
	else
		pClient->playSound("common/wpn_denyselect");

	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointRemove1Command("remove1", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();

	if ( pClient->currentWaypoint() != -1 )
	{
		pClient->setPathFrom(pClient->currentWaypoint());
		pClient->playSound("common/wpn_hudoff.wav");
	}
	else
		pClient->playSound("common/wpn_moveselect.wav");

	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointRemove2Command("remove2", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();
	pClient->setPathTo(pClient->currentWaypoint());

	CWaypoint *pWpt = CWaypoints::getWaypoint(pClient->getPathFrom());

	// valid?
	if ( !pWpt )
		pClient->playSound("common/wpn_moveselect");
	else
	{
		pClient->playSound("buttons/button9");

		pWpt->removePathTo(pClient->getPathTo());
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointDeleteToCommand("deleteto", 0, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();

	if ( CWaypoints::validWaypointIndex(pClient->currentWaypoint()) )
	{
		CWaypoints::deletePathsTo(pClient->currentWaypoint());
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointDeleteFromCommand("deletefrom", 0, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();

	if ( CWaypoints::validWaypointIndex(pClient->currentWaypoint()) )
	{
		CWaypoints::deletePathsFrom(pClient->currentWaypoint());
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline PathWaypointCreateFromToCommand("createfromto", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient && args[0] && *args[0] && args[1] && *args[1] )
	{
		CWaypoint *pWaypoint = CWaypoints::getWaypoint(atoi(args[0]));

		if ( pWaypoint && pWaypoint->isUsed() )
		{
			CWaypoint *pWaypoint2 = CWaypoints::getWaypoint(atoi(args[1]));

			if ( pWaypoint2 && pWaypoint2->isUsed() )
			{
				pWaypoint->addPathTo(atoi(args[1]));
				CBotGlobals::botMessage(pClient!=NULL ? pClient->getPlayer() : NULL,
					0,"Added path from <%d> to <%d>",atoi(args[0]),atoi(args[1]));

				pWaypoint->draw(pClient->getPlayer(),true,DRAWTYPE_DEBUGENGINE);
				pWaypoint->info(pClient->getPlayer());
				pWaypoint2->draw(pClient->getPlayer(),true,DRAWTYPE_DEBUGENGINE);
				pWaypoint2->info(pClient->getPlayer());

				pClient->playSound("buttons/button9");

				return COMMAND_ACCESSED;
			}
			else
				CBotGlobals::botMessage(pClient!=NULL ? pClient->getPlayer() : NULL,
					0,"Waypoint id <%d> not found",atoi(args[1]));

		}
		else
			CBotGlobals::botMessage(pClient!=NULL ? pClient->getPlayer() : NULL,
				0,"Waypoint id <%d> not found",atoi(args[0]));
	}
	else
		CBotGlobals::botMessage(pClient!=NULL ? pClient->getPlayer() : NULL,
				0,"missing args <id1> <id2>");
	
	return COMMAND_ERROR;
});

CBotCommandInline PathWaypointRemoveFromToCommand("removefromto", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient && args[0] && *args[0] && args[1] && *args[1] )
	{
		CWaypoint *pWaypoint = CWaypoints::getWaypoint(atoi(args[0]));

		if ( pWaypoint && pWaypoint->isUsed() )
		{
			CWaypoint *pWaypoint2 = CWaypoints::getWaypoint(atoi(args[1]));

			if ( pWaypoint2 && pWaypoint2->isUsed() )
			{
				pWaypoint->removePathTo(atoi(args[1]));
				CBotGlobals::botMessage(pClient!=NULL ? pClient->getPlayer() : NULL,
					0,"Removed path from <%d> to <%d>",atoi(args[0]),atoi(args[1]));

				pWaypoint->draw(pClient->getPlayer(),true,DRAWTYPE_DEBUGENGINE);
				pWaypoint->info(pClient->getPlayer());
				pWaypoint2->draw(pClient->getPlayer(),true,DRAWTYPE_DEBUGENGINE);
				pWaypoint2->info(pClient->getPlayer());

				pClient->playSound("buttons/button24");

				return COMMAND_ACCESSED;
			}
			else
				CBotGlobals::botMessage(pClient!=NULL ? pClient->getPlayer() : NULL,
					0,"Waypoint id <%d> not found",atoi(args[1]));

		}
		else
			CBotGlobals::botMessage(pClient!=NULL ? pClient->getPlayer() : NULL,
				0,"Waypoint id <%d> not found",atoi(args[0]));
	}
	else
		CBotGlobals::botMessage(pClient!=NULL ? pClient->getPlayer() : NULL,
				0,"missing args <id1> <id2>");
	
	return COMMAND_ERROR;
});

CBotSubcommands PathWaypointSubcommands("pathwaypoint", CMD_ACCESS_WAYPOINT | CMD_ACCESS_DEDICATED, {
	&PathWaypointOnCommand,
	&PathWaypointOffCommand,
	&PathWaypointAutoOnCommand,
	&PathWaypointAutoOffCommand,
	&PathWaypointCreate1Command,
	&PathWaypointCreate2Command,
	&PathWaypointRemove1Command,
	&PathWaypointRemove2Command,
	&PathWaypointDeleteToCommand,
	&PathWaypointDeleteFromCommand,
	&PathWaypointCreateFromToCommand,
	&PathWaypointRemoveFromToCommand,
});
