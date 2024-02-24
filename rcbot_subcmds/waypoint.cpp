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
 */

CBotCommandInline WaypointOnCommand("on", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		pClient->setWaypointOn(true);
		pClient->giveMessage("Waypoints On");
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointOffCommand("off", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	pClient->setWaypointOn(false);
	pClient->giveMessage("Waypoints Off");
	CBotGlobals::botMessage(pClient->getPlayer(),0,"waypoints off");

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointAddCommand("add", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	CWaypoints::addWaypoint(pClient,args[0],args[1],args[2],args[3]);

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointDeleteCommand("delete", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{	
	if ( pClient )
	{
		if ( args[0] && *args[0] )
		{
			float radius = atof(args[0]);

			if ( radius > 0 )
			{
				int numdeleted = 0;
				Vector vOrigin = pClient->getOrigin();

				WaypointList pWpt;
				CWaypointLocations::GetAllInArea(vOrigin,&pWpt,-1);

				for ( unsigned short int i = 0; i < pWpt.size(); i ++ )
				{
					CWaypoint *pWaypoint = CWaypoints::getWaypoint(pWpt[i]);

					if ( pWaypoint->distanceFrom(vOrigin) < radius)
					{
						CWaypoints::deleteWaypoint(pWpt[i]);
						numdeleted++;
					}
				}

				if ( numdeleted > 0 )
				{
					CBotGlobals::botMessage(pClient->getPlayer(),0,"%d waypoints within range of %0.0f deleted",numdeleted,radius);
					pClient->updateCurrentWaypoint(); // waypoint deleted so get a new one
					pClient->playSound("buttons/combine_button_locked");
					pClient->giveMessage("Waypoints deleted");
				}
				else
				{
					CBotGlobals::botMessage(pClient->getPlayer(),0,"no waypoints within range of %0.0f",radius);
					pClient->playSound("weapons/wpn_denyselect");
					pClient->giveMessage("Waypoints deleted");
					pClient->updateCurrentWaypoint(); // waypoint deleted so get a new one
				}
			}
		}
		else
		{
			pClient->updateCurrentWaypoint();

			if ( CWaypoints::validWaypointIndex(pClient->currentWaypoint()) )
			{
				CWaypoints::deleteWaypoint(pClient->currentWaypoint());
				CBotGlobals::botMessage(pClient->getPlayer(),0,"waypoint %d deleted",pClient->currentWaypoint());
				pClient->updateCurrentWaypoint(); // waypoint deleted so get a new one
				pClient->playSound("buttons/combine_button_locked");
				pClient->giveMessage("Waypoint deleted");
			}
			else
			{
				CBotGlobals::botMessage(pClient->getPlayer(),0,"no waypoint nearby to delete");
				pClient->playSound("weapons/wpn_denyselect");
				pClient->giveMessage("No Waypoint");
			}
		}
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointInfoCommand("info", 0, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();

	CWaypoint *pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

	if ( pWpt )
		pWpt->info(pClient->getPlayer());

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointSaveCommand("save", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( CWaypoints::save(false,(pClient!=NULL)?pClient->getPlayer():NULL,((args[0]!=NULL) && (*args[0]!=0))?args[0]:NULL,((args[1]!=NULL) && (*args[1]!=0))?args[1]:NULL) )
	{
		CBotGlobals::botMessage(NULL,0,"waypoints saved");
		if ( pClient )
			pClient->giveMessage("Waypoints Saved");
	}
	else
		CBotGlobals::botMessage(NULL,0,"error: could not save waypoints");

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointLoadCommand("load", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	char *szMapName = CBotGlobals::getMapName();
	bool bLoadOK = false;

	if ( args[0] && *args[0] )
	{
		bLoadOK = CWaypoints::load(args[0]);
		szMapName = (char*)args[0];
	}
	else
		bLoadOK = CWaypoints::load();

	if ( bLoadOK )
		CBotGlobals::botMessage(NULL,0,"waypoints %s loaded",szMapName);
	else
		CBotGlobals::botMessage(NULL,0,"error: could not load %s waypoints",szMapName);

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointClearCommand("clear", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	CWaypoints::init();
	CBotGlobals::botMessage(pClient->getPlayer(),0,"waypoints cleared");

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointGiveTypeCommand("givetype", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	edict_t *pEntity = pClient->getPlayer();

	if ( args[0] && *args[0] )
	{
		if ( pClient->currentWaypoint() == -1 )
			CBotGlobals::botMessage(pEntity,0,"No waypoint nearby to give types (move closer to the waypoint you want to give types)");
		else
		{
			char *type = NULL;

			for ( int i = 0; i < 4; i ++ )
			{
				if ( i == 0 )
					type = (char*)args[0];
				else if ( i == 1 )
					type = (char*)args[1];
				else if ( i == 2 )
					type = (char*)args[2];
				else if ( i == 3 )
					type = (char*)args[3];

				if ( !type || !*type )
					break;

				CWaypointType *pType = CWaypointTypes::getType(type);

				if ( pType )
				{
					CWaypoint *pWaypoint = CWaypoints::getWaypoint(pClient->currentWaypoint());

					if ( pWaypoint )
					{
						if ( pWaypoint->hasFlag(pType->getBits()) )
						{
							pWaypoint->removeFlag(pType->getBits());
							CBotGlobals::botMessage(pEntity,0,"type %s removed from waypoint %d",type,CWaypoints::getWaypointIndex(pWaypoint));
							pClient->playSound("UI/buttonrollover");
						}
						else
						{
							pWaypoint->addFlag(pType->getBits());
							
							if ( pType->getBits() & CWaypointTypes::W_FL_UNREACHABLE )
							{
								CWaypoints::deletePathsTo(CWaypoints::getWaypointIndex(pWaypoint));
								CWaypoints::deletePathsFrom(CWaypoints::getWaypointIndex(pWaypoint));
							}

							CBotGlobals::botMessage(pEntity,0,"type %s added to waypoint %d",type,CWaypoints::getWaypointIndex(pWaypoint));

							pClient->playSound("UI/buttonclickrelease");
						}
						
					}
				}
				else
				{
					CBotGlobals::botMessage(pEntity,0,"type '%s' not found",type);
					CWaypointTypes::showTypesOnConsole(pEntity);
				}

			}
		}
	}
	else
	{
		CWaypointTypes::showTypesOnConsole(pEntity);
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointDrawTypeCommand("drawtype", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		if ( args[0] && *args[0] )
		{
			pClient->setDrawType(atoi(args[0]));
			return COMMAND_ACCESSED;
		}
	}
	return COMMAND_ERROR;
}, "0: for effects engine (maximum limit of beams)\n1: for debug overlay (no limit of beams) [LISTEN SERVER CLIENT ONLY]");

CBotCommandInline WaypointAngleCommand("angle", 0, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient && pClient->getPlayer() )
	{
		pClient->updateCurrentWaypoint();

		CWaypoint *pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		if ( pWpt )
		{
			QAngle eye = CBotGlobals::playerAngles(pClient->getPlayer());
			CBotGlobals::botMessage(pClient->getPlayer(),0,"Waypoint Angle == %0.3f deg, (Eye == %0.3f)",CBotGlobals::yawAngleFromEdict(pClient->getPlayer(),pWpt->getOrigin()),eye.y);
			pClient->playSound("buttons/combine_button1");
		}
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointSetAngleCommand("updateyaw", 0, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();

	if ( CWaypoints::validWaypointIndex(pClient->currentWaypoint()) )
	{
		CWaypoint *pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		pWpt->setAim(CBotGlobals::playerAngles(pClient->getPlayer()).y);
	}

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointSetAreaCommand("setarea", 0, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();

	if ( args[0] && *args[0] && ( CWaypoints::validWaypointIndex(pClient->currentWaypoint()) ) )
	{
		CWaypoint *pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		pWpt->setArea(atoi(args[0]));
	}
	else
		return COMMAND_ERROR;

	return COMMAND_ACCESSED;
}, "Go to a waypoint, use setarea <areaid>");

CBotCommandInline WaypointSetRadiusCommand("setradius", 0, [](CClient *pClient, BotCommandArgs args)
{
	pClient->updateCurrentWaypoint();

	if ( args[0] && *args[0] && ( CWaypoints::validWaypointIndex(pClient->currentWaypoint()) ) )
	{
		CWaypoint *pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		pWpt->setRadius(atof(args[0]));
	}
	else
		return COMMAND_ERROR;

	return COMMAND_ACCESSED;
}, "Go to a waypoint, use setradius <radius>");

CBotCommandInline WaypointMenuCommand("menu", 0, [](CClient *pClient, BotCommandArgs args)
{
	pClient->setCurrentMenu(CBotMenuList::getMenu(BOT_MENU_WPT));

	return COMMAND_ACCESSED;
	/*
	if ( pClient )
	{
		// number of waypoint types to show per menu
		unsigned const int iNumToShow = 7;

		unsigned int i = 0;
		// menu level .. ?
		unsigned int iLevel = 0;
		// number of possible waypoint types to show
		unsigned int iNumTypes = CWaypointTypes::getNumTypes();

		char num[64], msg[64], cmd[128];
		CWaypointType *p;
		CWaypoint *pWpt;

		// get current waypoint types on current waypoint
		pClient->updateCurrentWaypoint();
		pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		if ( !pWpt )
		{
			CBotGlobals::botMessage(pClient->getPlayer(),0,"Not next to any waypoints to show a menu for");
			return COMMAND_ERROR;
		}

		// Menu level to start, 0 = beginning
		if ( args[0] && *args[0] )
		{
			iLevel = atoi(args[0]);
		}
		else
		{
			iLevel = 0;
		}

		//pClient->update

		KeyValues *kv = new KeyValues( "menu" );
		kv->SetString( "title", "Waypoint Menu" );
		kv->SetInt( "level", 1 );
		kv->SetColor( "color", Color( 255, 0, 0, 255 ));
		kv->SetInt( "time", 15 );
		kv->SetString( "msg", "Select Waypoint Type" );
		
		// start at this waypoint type index
		unsigned int iIndex = iLevel*iNumToShow;

		// run through a small number
		while ( (i < iNumToShow) && (iIndex < iNumTypes) )
		{
			p = CWaypointTypes::getTypeByIndex(iIndex);
			
			Q_snprintf( num, sizeof(num), "%i", i );

			// if waypoint has this type show it by putting a "[x]" next to it
			if ( pWpt->getFlags() & p->getBits() )
				Q_snprintf( msg, sizeof(msg), "%s [x]", p->getName() );
			else
				Q_snprintf( msg, sizeof(msg), "%s [ ]", p->getName() );

			Q_snprintf( cmd, sizeof(cmd), "rcbot waypoint givetype %s", p->getName() );

			KeyValues *item1 = kv->FindKey( num, true );
			item1->SetString( "msg", msg );
			item1->SetString( "command", cmd );

			iIndex ++;
			i++;
		}

		// finally show "More" option if available
		if ( iIndex < iNumTypes )
		{
			Q_snprintf( num, sizeof(num), "%i", i );
			Q_snprintf( msg, sizeof(msg), "More..." );
			Q_snprintf( cmd, sizeof(cmd), "rcbot waypoint menu %d", iLevel+1 );

			KeyValues *item1 = kv->FindKey( num, true );
			item1->SetString( "msg", msg );
			item1->SetString( "command", cmd );
		}

		helpers->CreateMessage( pClient->getPlayer(), DIALOG_MENU, kv, &g_RCBOTServerPlugin );
		kv->deleteThis();

		//pClient->showMenu();

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;*/
});

CBotCommandInline WaypointCut("cut", 0, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		pClient->updateCurrentWaypoint();

		CWaypoint *pwpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		if ( pwpt )
		{
			pClient->setWaypointCut(pwpt);
			CWaypoints::deleteWaypoint(pClient->currentWaypoint());
		}

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}, "allows you to move a waypoint by cutting it and pasting it");

CBotCommandInline WaypointCopy("copy", 0, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		pClient->updateCurrentWaypoint();

		CWaypoint *pwpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		if ( pwpt )
		{
			pClient->setWaypointCopy(pwpt);
			return COMMAND_ACCESSED;
		}
	}

	return COMMAND_ERROR;
}, "Go to a waypoint, and copy to hold its properties, then use paste to make a new waypoint with the same properties");

CBotCommandInline WaypointPaste("paste", 0, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		CWaypoints::addWaypoint(pClient,NULL,NULL,NULL,NULL,true);
		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}, "first copy a waypoint using the copy command and then use paste to make a new waypoint with the same properties");

CBotCommandInline WaypointShiftAreas("shiftareas", 0, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		int val = 1;

		if ( args[0] && *args[0] )
			val = atoi(args[0]);

		if ( args[1] && *args[1] )
		{
			int newarea = atoi(args[1]);

			if ( pClient )
			{
				// change area val to newarea
				CWaypoints::shiftVisibleAreas(pClient->getPlayer(),val,newarea);
			}
		}
		else
			CWaypoints::shiftAreas(val);

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}, "shift the areas of flagged waypoints to a different area/only use this once");

CBotCommandInline WaypointTeleportCommand("teleport", 0, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient && args[0] && *args[0] )
	{
		int id;
		CWaypoint *pWpt;
		bool bTele = false;

		id = atoi(args[0]);

		if ( (pWpt=CWaypoints::getWaypoint(id)) != NULL )
		{
			if ( pWpt->isUsed() )
			{
				pClient->teleportTo(pWpt->getOrigin()+Vector(0,0,8));
				bTele = true;
			}
		}

		if ( bTele )
		{
			CBotGlobals::botMessage(pClient->getPlayer(),0,"Teleported to waypoint %d",id);
		}
		else
		{
			CBotGlobals::botMessage(pClient->getPlayer(),0,"Invalid waypoint id %d",id);
		}

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
});

CBotCommandInline WaypointAreaSetToNearest("setareatonearest", 0, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		int id;
		CWaypoint *pWpt;
		bool bOk = false;
		int setarea = 0;

		id = pClient->currentWaypoint();

		if ( (pWpt=CWaypoints::getWaypoint(id)) != NULL )
		{
			if ( pWpt->isUsed() )
			{

				#if SOURCE_ENGINE == SE_TF2
					setarea = CTeamFortress2Mod::m_ObjectiveResource.NearestArea(pWpt->getOrigin());
				#elif SOURCE_ENGINE == SE_DODS
					setarea = CDODMod::m_Flags.findNearestObjective(pWpt->getOrigin());
				#endif

				if ( setarea > 0 )
					pWpt->setArea(setarea);

				bOk = true;
			}
		}

		if ( bOk )
		{
			if ( setarea > 0 )
				CBotGlobals::botMessage(pClient->getPlayer(),0,"Changed waypoint %d area to %d",id,setarea);
			else
				CBotGlobals::botMessage(pClient->getPlayer(),0,"No nearest area to wpt id %d",id);
		}
		else
		{
			CBotGlobals::botMessage(pClient->getPlayer(),0,"Invalid waypoint id %d",id);
		}
		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
});

CBotCommandInline WaypointShowCommand("show", 0, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		int wpt = atoi(args[0]);
		CWaypoint *pwpt = CWaypoints::getWaypoint(wpt);

		if ( pwpt )
		{
			g_pEffects->Beam( CBotGlobals::entityOrigin(pClient->getPlayer()), pwpt->getOrigin(), CWaypoints::waypointTexture(), 
				0, 0, 1,
				5, 12, 12, 255, 
				10, 255, 255, 255, 200, 10);	

			//pClient->setShowWpt(wpt);

			return COMMAND_ACCESSED;
		}
		//else
			//pClient->setShowWpt(-1);
	}

	return COMMAND_ERROR;
}, "show <waypoint ID>, shows you to this waypoint");

CBotCommandInline WaypointCheckCommand("check", 0, [](CClient *pClient, BotCommandArgs args)
{
	// loop through every waypoint and check the areas are not outside the number of control points

	CWaypoints::checkAreas((pClient==NULL)?NULL:pClient->getPlayer());

	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointShowVisCommand("showvis", 0, [](CClient *pClient, BotCommandArgs args)
{
#ifndef __linux__
	pClient->updateCurrentWaypoint();

	if ( CWaypoints::validWaypointIndex(pClient->currentWaypoint()) )
	{
		CWaypoint *pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		int i = 0; 
		int index;
		bool bVis;
		float ftime;

		ftime = (args[0]&&*args[0]) ? atof(args[0]) : 5.0f;

		if ( pWpt )
		{
			index = CWaypoints::getWaypointIndex(pWpt);
			CWaypointVisibilityTable *pTable = CWaypoints::getVisiblity();
	
			for ( i = 0; i < CWaypoints::numWaypoints(); i ++ )
			{
				CWaypoint *pOther = CWaypoints::getWaypoint(i);

				if ( !pOther->isUsed() )
					continue;

				if ( pOther->distanceFrom(pWpt) > 1024.0f )
					continue;
				
				bVis = pTable->GetVisibilityFromTo(index,i);
				
				debugoverlay->AddTextOverlayRGB(pOther->getOrigin(),0,ftime,bVis ? 0 : 255,bVis ? 255 : 0,0,200,bVis ? "VIS" : "INV" );
			}
		}
		
	}
	else
		return COMMAND_ERROR;
#endif
	return COMMAND_ACCESSED;
}, "Go to a waypoint, use showvis to see visibility");

CBotCommandInline WaypointAutoWaypointCommand("autowaypoint", CMD_ACCESS_WAYPOINT, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		pClient->setAutoWaypointMode(atoi(args[0])>0,atoi(args[0])==2);
		CBotGlobals::botMessage(pClient->getPlayer(),0,"Autowaypointing Mode %s, Debug %s",(atoi(args[0])>0)?"ON":"OFF",(atoi(args[0])==2)?"ON":"OFF");
	}
	return COMMAND_ACCESSED;
});

CBotCommandInline WaypointAutoFix("autofix", 0, [](CClient *pClient, BotCommandArgs args)
{
	bool bFixSentry_Sniper_Defend_TeleExtWpts = false;

	if ( args[0] && *args[0] )
	{
		bFixSentry_Sniper_Defend_TeleExtWpts = ( atoi(args[0]) == 1 );
	}

	CWaypoints::autoFix(bFixSentry_Sniper_Defend_TeleExtWpts);
	
	return COMMAND_ACCESSED;
});

CBotSubcommands WaypointSubcommands("waypoint", CMD_ACCESS_DEDICATED, {
	&WaypointOnCommand,
	&WaypointOffCommand,
	&WaypointAddCommand,
	&WaypointDeleteCommand,
	&WaypointInfoCommand,
	&WaypointSaveCommand,
	&WaypointLoadCommand,
	&WaypointClearCommand,
	&WaypointGiveTypeCommand,
	&WaypointDrawTypeCommand,
	&WaypointAngleCommand,
	&WaypointSetAngleCommand,
	&WaypointSetAreaCommand,
	&WaypointSetRadiusCommand,
	&WaypointMenuCommand,
	&WaypointCut,
	&WaypointCopy,
	&WaypointPaste,
	&WaypointShiftAreas,
	&WaypointTeleportCommand,
	&WaypointAreaSetToNearest,
	&WaypointShowCommand,
	&WaypointCheckCommand,
	&WaypointShowVisCommand,
	&WaypointAutoWaypointCommand,
	&WaypointAutoFix
});
