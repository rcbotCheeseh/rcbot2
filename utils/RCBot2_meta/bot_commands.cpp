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

#ifdef WIN32
#define sprintf sprintf_s
#endif

CBotCommandContainer* CBotGlobals::m_pCommands = new CRCBotCommand();
extern IVDebugOverlay* debugoverlay;
///////////////////////////////////////////////////
// Setup commands
CRCBotCommand::CRCBotCommand()
{
	setName("rcbot");
	setAccessLevel(0);
	add(new CWaypointCommand());
	add(new CAddBotCommand());
	add(new CControlCommand());
	add(new CPathWaypointCommand());
	add(new CDebugCommand());
	add(new CPrintCommands());
	add(new CConfigCommand());
	add(new CKickBotCommand());
	add(new CUsersCommand());
	add(new CUtilCommand());
}

CWaypointCommand::CWaypointCommand()
{
	setName("waypoint");
	//setAccessLevel(CMD_ACCESS_WAYPOINT);
	setAccessLevel(0);
	add(new CWaypointOnCommand());
	add(new CWaypointOffCommand());
	add(new CWaypointAddCommand());
	add(new CWaypointDeleteCommand());
	add(new CWaypointInfoCommand());
	add(new CWaypointSaveCommand());
	add(new CWaypointLoadCommand());
	add(new CWaypointClearCommand());
	add(new CWaypointGiveTypeCommand());
	add(new CWaypointDrawTypeCommand());
	add(new CWaypointAngleCommand());
	add(new CWaypointSetAngleCommand());
	add(new CWaypointSetAreaCommand());
	add(new CWaypointSetRadiusCommand());
	add(new CWaypointMenuCommand());
	add(new CWaypointCut());
	add(new CWaypointCopy());
	add(new CWaypointPaste());
	add(new CWaypointShiftAreas());
	add(new CWaypointTeleportCommand());
	add(new CWaypointAreaSetToNearest());
	add(new CWaypointShowCommand());
	add(new CWaypointCheckCommand());
	add(new CWaypointShowVisCommand());
	add(new CWaypointAutoWaypointCommand());
	add(new CWaypointAutoFix());
}///////////////

CWaypointShowCommand::CWaypointShowCommand()
{
	setName("show");
	setHelp("show <waypoint ID>, shows you to this waypoint");
}

eBotCommandResult CWaypointShowCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		int wpt = atoi(pcmd);
		CWaypoint* pwpt = CWaypoints::getWaypoint(wpt);

		if (pwpt)
		{
			g_pEffects->Beam(CBotGlobals::entityOrigin(pClient->getPlayer()), pwpt->getOrigin(), CWaypoints::waypointTexture(),
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
}

//////////////

CWaypointCopy::CWaypointCopy()
{
	setName("copy");
	setHelp("Go to a waypoint, and copy to hold its properties, then use paste to make a new waypoint with the same properties");
}

eBotCommandResult CWaypointCopy::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		pClient->updateCurrentWaypoint();

		CWaypoint* pwpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		if (pwpt)
		{
			pClient->setWaypointCopy(pwpt);
			return COMMAND_ACCESSED;
		}
	}

	return COMMAND_ERROR;
}
//////////////////////////////////////////

CWaypointCut::CWaypointCut()
{
	setName("cut");
	setHelp("allows you to move a waypoint by cutting it and pasting it");
}

eBotCommandResult CWaypointCut::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		pClient->updateCurrentWaypoint();

		CWaypoint* pwpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		if (pwpt)
		{
			pClient->setWaypointCut(pwpt);
			CWaypoints::deleteWaypoint(pClient->currentWaypoint());
		}

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}

////////////////////////////////
eBotCommandResult CSetProp::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		// classname           // key             // type          //value
		if ((pcmd && *pcmd) && (arg1 && *arg1) && (arg2 && *arg2) && (arg3 && *arg3))
		{
			//int i = 0;

			edict_t* pPlayer = pClient->getPlayer();
			//			edict_t *pEdict;
			edict_t* pNearest = NULL;
			//			float fDistance;
			//			float fNearest = 400.0f;

			pNearest = CClassInterface::FindEntityByNetClassNearest(pClient->getOrigin(), pcmd);

			if (pNearest)
			{
				void* data = NULL;

				extern bool g_PrintProps;
				unsigned int m_offset = 0;

				ServerClass* sc = UTIL_FindServerClass(pcmd);

				if (sc)
				{
					UTIL_FindSendPropInfo(sc, arg1, &m_offset);

					if (m_offset)
					{
						static IServerUnknown* pUnknown;
						static CBaseEntity* pEntity;
						Vector vdata;

						pUnknown = static_cast<IServerUnknown*>(pNearest->GetUnknown());

						pEntity = pUnknown->GetBaseEntity();

						data = static_cast<void*>(reinterpret_cast<char*>(pEntity) + m_offset);

						if (data)
						{
							bool* booldata = static_cast<bool*>(data);
							int* intdata = static_cast<int*>(data);
							float* floatdata = static_cast<float*>(data);

							if (strcmp(arg2, "int") == 0)
								*intdata = atoi(arg3);
							else if (strcmp(arg2, "bool") == 0)
								*booldata = (atoi(arg3) == 1);
							else if (strcmp(arg2, "float") == 0)
								*floatdata = atof(arg3);
						}
						else
							CBotGlobals::botMessage(pPlayer, 0, "NULL");
					}
					else
						CBotGlobals::botMessage(NULL, 0, "OFFSET NOT FOUND");
				}
				else
					CBotGlobals::botMessage(NULL, 0, "CLASS NOT FOUND");
			}
			else
				CBotGlobals::botMessage(NULL, 0, "EDICT NOT FOUND");
		}
		else
			CBotGlobals::botMessage(NULL, 0, "Usage: getprop CLASSNAME KEY TYPE(int,bool,float) VALUE");

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}

////////////////////////////////
eBotCommandResult CGetProp::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		if ((pcmd && *pcmd) && (arg1 && *arg1))
		{
			//int i = 0;

			edict_t* pPlayer = pClient->getPlayer();
			//			edict_t *pEdict;
			edict_t* pNearest = NULL;
			//			float fDistance;
			//			float fNearest = 400.0f;

			pNearest = CClassInterface::FindEntityByNetClassNearest(pClient->getOrigin(), pcmd);

			if (pNearest)
			{
				void* data = NULL;

				extern bool g_PrintProps;
				unsigned int m_offset = 0;

				ServerClass* sc = UTIL_FindServerClass(pcmd);

				if (sc)
				{
					UTIL_FindSendPropInfo(sc, arg1, &m_offset);

					if (m_offset)
					{
						static IServerUnknown* pUnknown;
						static CBaseEntity* pEntity;
						Vector vdata;

						pUnknown = static_cast<IServerUnknown*>(pNearest->GetUnknown());

						pEntity = pUnknown->GetBaseEntity();

						int preoffs = 0;

						if ((arg2 && *arg2))
						{
							preoffs = atoi(arg2);
						}

						data = static_cast<void*>(reinterpret_cast<char*>(pEntity) + m_offset);

						if (data)
						{
							vdata = *(static_cast<Vector*>(data) + preoffs);

							CBotGlobals::botMessage(pPlayer, 0, "int = %d, float = %f, bool = %s, Vector = (%0.4f,%0.4f,%0.4f)", *(static_cast<int*>(data) + preoffs), *(static_cast<float*>(data) + preoffs), *(static_cast<bool*>(data) + preoffs) ? ("true") : ("false"), vdata.x, vdata.y, vdata.z);
						}
						else
							CBotGlobals::botMessage(pPlayer, 0, "NULL");
					}
					else
						CBotGlobals::botMessage(NULL, 0, "OFFSET NOT FOUND");
				}
				else
					CBotGlobals::botMessage(NULL, 0, "CLASS NOT FOUND");
			}
			else
				CBotGlobals::botMessage(NULL, 0, "EDICT NOT FOUND");
		}
		else
			CBotGlobals::botMessage(NULL, 0, "Usage: getprop CLASS CLASSNAME KEY");

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}
//////////////////////////////
eBotCommandResult CFindProp::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pcmd && *pcmd)
	{
		UTIL_FindPropPrint(pcmd);
		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}
///////////////////////////
eBotCommandResult CFindClassname::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		if (pcmd && *pcmd)
		{
			const char* pclass = CClassInterface::FindEntityNetClass(0, pcmd);

			if (pclass)
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "%s network name = %s", pcmd, pclass);
			else
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "%s network name not found", pcmd, pclass);
		}

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}

////////////////////////////////
eBotCommandResult CFindClass::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		if (pcmd && *pcmd)
		{
			UTIL_FindServerClassPrint(pcmd);
		}

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}
///////////////////////////////////////////
////////////////////////////////
eBotCommandResult CPrintProps::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		if (pcmd && *pcmd)
		{
			extern bool g_PrintProps;
			unsigned int m_offset;
			g_PrintProps = true;

			ServerClass* sc = UTIL_FindServerClass(pcmd);

			if (sc)
				UTIL_FindSendPropInfo(sc, "", &m_offset);

			g_PrintProps = false;
		}

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}
///////////////////////////////////////////

CWaypointPaste::CWaypointPaste()
{
	setName("paste");
	setHelp("first copy a waypoint using the copy command and then use paste to make a new waypoint with the same properties");
}

eBotCommandResult CWaypointPaste::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		CWaypoints::addWaypoint(pClient, NULL, NULL, NULL, NULL, true);
		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}
//////////////////////////////////

CWaypointShiftAreas::CWaypointShiftAreas()
{
	setName("shiftareas");
	setHelp("shift the areas of flagged waypoints to a different area/only use this once");
}

eBotCommandResult CWaypointShiftAreas::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		int val = 1;

		if (pcmd && *pcmd)
			val = atoi(pcmd);

		if (arg1 && *arg1)
		{
			int newarea = atoi(arg1);

			if (pClient)
			{
				// change area val to newarea
				CWaypoints::shiftVisibleAreas(pClient->getPlayer(), val, newarea);
			}
		}
		else
			CWaypoints::shiftAreas(val);

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}
////////////////////////////////////
CWaypointSetAreaCommand::CWaypointSetAreaCommand()
{
	setName("setarea");
	setHelp("Go to a waypoint, use setarea <areaid>");
}

eBotCommandResult CWaypointSetAreaCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();

	if (pcmd && *pcmd && (CWaypoints::validWaypointIndex(pClient->currentWaypoint())))
	{
		CWaypoint* pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		pWpt->setArea(atoi(pcmd));
	}
	else
		return COMMAND_ERROR;

	return COMMAND_ACCESSED;
}
//////////////

CWaypointShowVisCommand::CWaypointShowVisCommand()
{
	setName("showvis");
	setHelp("Go to a waypoint, use showvis to see visibility");
}

eBotCommandResult CWaypointShowVisCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
#ifndef __linux__
	pClient->updateCurrentWaypoint();

	if (CWaypoints::validWaypointIndex(pClient->currentWaypoint()))
	{
		CWaypoint* pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		int i = 0;
		int index;
		bool bVis;
		float ftime;

		ftime = (pcmd && *pcmd) ? atof(pcmd) : 5.0f;

		if (pWpt)
		{
			index = CWaypoints::getWaypointIndex(pWpt);
			CWaypointVisibilityTable* pTable = CWaypoints::getVisiblity();

			for (i = 0; i < CWaypoints::numWaypoints(); i++)
			{
				CWaypoint* pOther = CWaypoints::getWaypoint(i);

				if (!pOther->isUsed())
					continue;

				if (pOther->distanceFrom(pWpt) > 1024.0f)
					continue;

				bVis = pTable->GetVisibilityFromTo(index, i);

				debugoverlay->AddTextOverlayRGB(pOther->getOrigin(), 0, ftime, bVis ? 0 : 255, bVis ? 255 : 0, 0, 200, bVis ? "VIS" : "INV");
			}
		}
	}
	else
		return COMMAND_ERROR;
#endif
	return COMMAND_ACCESSED;
}

///////////////
CWaypointSetRadiusCommand::CWaypointSetRadiusCommand()
{
	setName("setradius");
	setHelp("Go to a waypoint, use setradius <radius>");
}

eBotCommandResult CWaypointSetRadiusCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();

	if (pcmd && *pcmd && (CWaypoints::validWaypointIndex(pClient->currentWaypoint())))
	{
		CWaypoint* pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		pWpt->setRadius(atof(pcmd));
	}
	else
		return COMMAND_ERROR;

	return COMMAND_ACCESSED;
}
/////////////////

CWaypointMenuCommand::CWaypointMenuCommand()
{
	setName("menu");
};

eBotCommandResult CWaypointMenuCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
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
		if ( pcmd && *pcmd )
		{
			iLevel = atoi(pcmd);
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
};
////////////////////////////

CWaypointSetAngleCommand::CWaypointSetAngleCommand()
{
	setName("updateyaw");
}

eBotCommandResult CWaypointSetAngleCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();

	if (CWaypoints::validWaypointIndex(pClient->currentWaypoint()))
	{
		CWaypoint* pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		pWpt->setAim(CBotGlobals::playerAngles(pClient->getPlayer()).y);
	}

	return COMMAND_ACCESSED;
}

CUsersCommand::CUsersCommand()
{
	setName("users");
	setAccessLevel(0);

	add(new CShowUsersCommand());
}

CDebugCommand::CDebugCommand()
{
	setName("debug");
	setAccessLevel(CMD_ACCESS_DEBUG);

	add(new CDebugGameEventCommand());
	add(new CDebugBotCommand());
	add(new CDebugNavCommand());
	add(new CDebugVisCommand());
	add(new CDebugThinkCommand());
	add(new CDebugLookCommand());
	add(new CDebugHudCommand());
	add(new CDebugAimCommand());
	add(new CDebugChatCommand());
	add(new CBotGoto());
	add(new CBotFlush());
	add(new CDebugTaskCommand());
	add(new CBotTaskCommand());
	add(new CDebugButtonsCommand());
	add(new CDebugSpeedCommand());
	add(new CDebugUsercmdCommand());
	add(new CDebugUtilCommand());
	add(new CDebugProfilingCommand());
	add(new CDebugEdictsCommand());
	add(new CPrintProps());
	add(new CGetProp());
	add(new CSetProp());
	add(new CFindClass());
	add(new CFindClassname());
	add(new CFindProp());
	add(new CDebugMemoryScanCommand());
	add(new CDebugMemoryCheckCommand());
	add(new CDebugMstrOffsetSearch());
}
/////////////////////
CWaypointOnCommand::CWaypointOnCommand()
{
	setName("on");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

CWaypointDrawTypeCommand::CWaypointDrawTypeCommand()
{
	setName("drawtype");
	setHelp("0: for effects engine (maximum limit of beams)\n1: for debug overlay (no limit of beams) [LISTEN SERVER CLIENT ONLY]");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CWaypointDrawTypeCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		if (pcmd && *pcmd)
		{
			pClient->setDrawType(atoi(pcmd));
			return COMMAND_ACCESSED;
		}
	}

	return COMMAND_ERROR;
}

eBotCommandResult CWaypointOnCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		pClient->setWaypointOn(true);
		pClient->giveMessage("Waypoints On");
	}

	return COMMAND_ACCESSED;
}

eBotCommandResult CWaypointGiveTypeCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pEntity = pClient->getPlayer();

	if (pcmd && *pcmd)
	{
		if (pClient->currentWaypoint() == -1)
			CBotGlobals::botMessage(pEntity, 0, "No waypoint nearby to give types (move closer to the waypoint you want to give types)");
		else
		{
			char* type = NULL;

			for (int i = 0; i < 4; i++)
			{
				if (i == 0)
					type = const_cast<char*>(pcmd);
				else if (i == 1)
					type = const_cast<char*>(arg1);
				else if (i == 2)
					type = const_cast<char*>(arg2);
				else if (i == 3)
					type = const_cast<char*>(arg3);

				if (!type || !*type)
					break;

				CWaypointType* pType = CWaypointTypes::getType(type);

				if (pType)
				{
					CWaypoint* pWaypoint = CWaypoints::getWaypoint(pClient->currentWaypoint());

					if (pWaypoint)
					{
						if (pWaypoint->hasFlag(pType->getBits()))
						{
							pWaypoint->removeFlag(pType->getBits());
							CBotGlobals::botMessage(pEntity, 0, "type %s removed from waypoint %d", type, CWaypoints::getWaypointIndex(pWaypoint));
							pClient->playSound("UI/buttonrollover");
						}
						else
						{
							pWaypoint->addFlag(pType->getBits());

							if (pType->getBits() & CWaypointTypes::W_FL_UNREACHABLE)
							{
								CWaypoints::deletePathsTo(CWaypoints::getWaypointIndex(pWaypoint));
								CWaypoints::deletePathsFrom(CWaypoints::getWaypointIndex(pWaypoint));
							}

							CBotGlobals::botMessage(pEntity, 0, "type %s added to waypoint %d", type, CWaypoints::getWaypointIndex(pWaypoint));

							pClient->playSound("UI/buttonclickrelease");
						}
					}
				}
				else
				{
					CBotGlobals::botMessage(pEntity, 0, "type '%s' not found", type);
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
}

//////////////////
CWaypointOffCommand::CWaypointOffCommand()
{
	setName("off");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CWaypointClearCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	CWaypoints::init();
	CBotGlobals::botMessage(pClient->getPlayer(), 0, "waypoints cleared");

	return COMMAND_ACCESSED;
}

eBotCommandResult CWaypointOffCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->setWaypointOn(false);
	pClient->giveMessage("Waypoints Off");
	CBotGlobals::botMessage(pClient->getPlayer(), 0, "waypoints off");

	return COMMAND_ACCESSED;
}
////////////////////////
CWaypointAddCommand::CWaypointAddCommand()
{
	setName("add");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CWaypointAddCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	CWaypoints::addWaypoint(pClient, pcmd, arg1, arg2, arg3);

	return COMMAND_ACCESSED;
}
////////////////////
CWaypointDeleteCommand::CWaypointDeleteCommand()
{
	setName("delete");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CWaypointDeleteCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		if (pcmd && *pcmd)
		{
			float radius = atof(pcmd);

			if (radius > 0)
			{
				vector<int> pWpt;
				int numdeleted = 0;
				Vector vOrigin = pClient->getOrigin();

				CWaypointLocations::GetAllInArea(vOrigin, &pWpt, -1);

				for (unsigned short int i = 0; i < pWpt.size(); i++)
				{
					CWaypoint* pWaypoint = CWaypoints::getWaypoint(pWpt[i]);

					if (pWaypoint->distanceFrom(vOrigin) < radius)
					{
						CWaypoints::deleteWaypoint(pWpt[i]);
						numdeleted++;
					}
				}

				if (numdeleted > 0)
				{
					CBotGlobals::botMessage(pClient->getPlayer(), 0, "%d waypoints within range of %0.0f deleted", numdeleted, radius);
					pClient->updateCurrentWaypoint(); // waypoint deleted so get a new one
					pClient->playSound("buttons/combine_button_locked");
					pClient->giveMessage("Waypoints deleted");
				}
				else
				{
					CBotGlobals::botMessage(pClient->getPlayer(), 0, "no waypoints within range of %0.0f", radius);
					pClient->playSound("weapons/wpn_denyselect");
					pClient->giveMessage("Waypoints deleted");
					pClient->updateCurrentWaypoint(); // waypoint deleted so get a new one
				}
			}
		}
		else
		{
			pClient->updateCurrentWaypoint();

			if (CWaypoints::validWaypointIndex(pClient->currentWaypoint()))
			{
				CWaypoints::deleteWaypoint(pClient->currentWaypoint());
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "waypoint %d deleted", pClient->currentWaypoint());
				pClient->updateCurrentWaypoint(); // waypoint deleted so get a new one
				pClient->playSound("buttons/combine_button_locked");
				pClient->giveMessage("Waypoint deleted");
			}
			else
			{
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "no waypoint nearby to delete");
				pClient->playSound("weapons/wpn_denyselect");
				pClient->giveMessage("No Waypoint");
			}
		}
	}

	return COMMAND_ACCESSED;
}
/////////////////////
CControlCommand::CControlCommand()
{
	setName("control");
	setAccessLevel(CMD_ACCESS_BOT);
}

eBotCommandResult CControlCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pEntity = NULL;

	if (pClient)
		pEntity = pClient->getPlayer();
	if (pcmd && *pcmd)
	{
		if (CBots::controlBot(pcmd, pcmd, arg2, arg3))
			CBotGlobals::botMessage(pEntity, 0, "bot added");
		else
			CBotGlobals::botMessage(pEntity, 0, "error: couldn't control bot '%s'", pcmd);

		return COMMAND_ACCESSED;
	}
	else
		return COMMAND_ERROR;
}
////////////////////

CAddBotCommand::CAddBotCommand()
{
	setName("addbot");
	setAccessLevel(CMD_ACCESS_BOT);
}

eBotCommandResult CAddBotCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	//	bool bOkay = false;

	edict_t* pEntity = NULL;

	if (pClient)
		pEntity = pClient->getPlayer();

	//extern ConVar *sv_cheats;
	//extern ConVar bot_sv_cheats_auto;
	//extern ConVar bot_sv_cheat_warning;

	//if ( !bot_sv_cheat_warning.GetBool() || bot_sv_cheats_auto.GetBool() || !CBots::controlBots() || (!sv_cheats || sv_cheats->GetBool()) )
	//{
		//if ( !pcmd || !*pcmd )
		//	bOkay = CBots::createBot();
		//else
		//bOkay = CBots::createBot();

		///if ( CBots::createBot(pcmd,arg1,arg2) )
	if (CBots::addBot(pcmd, arg1, arg2))
		CBotGlobals::botMessage(pEntity, 0, "bot adding...");
	else
		CBotGlobals::botMessage(pEntity, 0, "error: couldn't create bot! (Check maxplayers)");
	//}
	//else
	//	CBotGlobals::botMessage(pEntity,0,"error: sv_cheats must be 1 to add bots");

	return COMMAND_ACCESSED;
}
//////////////////////
//edits schedules

eBotCommandResult CBotTaskCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
#ifndef __linux__

	if (pClient && pClient->getDebugBot() != NULL)
	{
		edict_t* pEdict = pClient->getDebugBot();
		CBot* pBot = CBots::getBotPointer(pEdict);

		if (pBot->inUse())
		{
			CBotSchedules* pSched = pBot->getSchedule();

			if (pcmd && *pcmd)
			{
				//int task = atoi(pcmd);

				pSched->freeMemory();

				// 83
				if (!strcmp(pcmd, "pipe"))
				{
					CBotUtility util = CBotUtility(pBot, BOT_UTIL_PIPE_LAST_ENEMY, true, 1.0f);
					pBot->setLastEnemy(pClient->getPlayer());
					pBot->getSchedule()->freeMemory();
					static_cast<CBotTF2*>(pBot)->executeAction(&util);
				}
				// 71
				else if (!strcmp(pcmd, "gren"))
				{
					CBotWeapons* pWeapons;
					CBotWeapon* gren;

					pWeapons = pBot->getWeapons();
					gren = pWeapons->getGrenade();

					if (gren)
					{
						CBotSchedule* sched = new CBotSchedule(new CThrowGrenadeTask(gren, pBot->getAmmo(gren->getWeaponInfo()->getAmmoIndex1()), pClient->getOrigin()));
						pSched->add(sched);
					}
				}
				else if (!strcmp(pcmd, "snipe"))
				{
					if (pClient)
					{
						CWaypoint* pWaypoint = CWaypoints::getWaypoint(
							CWaypoints::nearestWaypointGoal(CWaypointTypes::W_FL_SNIPER, pClient->getOrigin(), 200.0f,
								pBot->getTeam()));

						if (pWaypoint)
						{
							if (CBotGlobals::isMod(MOD_TF2))
							{
								//if ( CClassInterface::getTF2Class() )
							}
							else
							{
								CBotWeapon* pWeapon;
								CBotWeapons* m_pWeapons;
								CBotSchedule* snipe = new CBotSchedule();
								CBotTask* findpath = new CFindPathTask(CWaypoints::getWaypointIndex(pWaypoint));
								CBotTask* snipetask;

								m_pWeapons = pBot->getWeapons();
								pWeapon = m_pWeapons->hasWeapon(DOD_WEAPON_K98_SCOPED) ? m_pWeapons->getWeapon(CWeapons::getWeapon(DOD_WEAPON_K98_SCOPED)) : m_pWeapons->getWeapon(CWeapons::getWeapon(DOD_WEAPON_SPRING));

								if (pWeapon)
								{
									// linux fix - copy origin onto vector here
									Vector vOrigin = pWaypoint->getOrigin();
									snipetask = new CBotDODSnipe(pWeapon, vOrigin, pWaypoint->getAimYaw(), false, 0, pWaypoint->getFlags());

									findpath->setCompleteInterrupt(CONDITION_PUSH);
									snipetask->setCompleteInterrupt(CONDITION_PUSH);

									snipe->setID(SCHED_DEFENDPOINT);
									snipe->addTask(findpath);
									snipe->addTask(snipetask);

									pSched->add(snipe);
								}
								else
									CBotGlobals::botMessage(NULL, 0, "Bot is not a sniper");
							}
						}
						else
							CBotGlobals::botMessage(NULL, 0, "Sniper waypoint not found");
					}
				}
			}
		}
	}

#endif
	return COMMAND_ACCESSED;
}
//////////////////////
//clear bots schedules

eBotCommandResult CBotFlush::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient && pClient->getDebugBot() != NULL)
	{
		edict_t* pEdict = pClient->getDebugBot();
		CBot* pBot = CBots::getBotPointer(pEdict);

		if (pBot->inUse())
		{
			CBotSchedules* pSched = pBot->getSchedule();
			pSched->freeMemory();
		}
	}

	return COMMAND_ACCESSED;
}
///////////////////////
eBotCommandResult CBotGoto::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient && pClient->getDebugBot() != NULL)
	{
		edict_t* pEdict = pClient->getDebugBot();
		CBot* pBot = CBots::getBotPointer(pEdict);

		if (pBot->inUse())
		{
			int iWpt;

			if (pcmd && *pcmd)
			{
				iWpt = atoi(pcmd);

				if ((iWpt < 0) || (iWpt >= CWaypoints::numWaypoints()))
					iWpt = -1;
			}
			else
				iWpt = pClient->currentWaypoint();

			if (iWpt != -1)
				pBot->forceGotoWaypoint(iWpt);
		}
	}

	return COMMAND_ACCESSED;
}
/////////////////////

CPathWaypointCommand::CPathWaypointCommand()
{
	setName("pathwaypoint");
	setAccessLevel(CMD_ACCESS_WAYPOINT);

	add(new CPathWaypointOnCommand());
	add(new CPathWaypointOffCommand());
	add(new CPathWaypointAutoOnCommand());
	add(new CPathWaypointAutoOffCommand());
	add(new CPathWaypointCreate1Command());
	add(new CPathWaypointCreate2Command());
	add(new CPathWaypointRemove1Command());
	add(new CPathWaypointRemove2Command());
	add(new CPathWaypointDeleteToCommand());
	add(new CPathWaypointDeleteFromCommand());
	add(new CPathWaypointCreateFromToCommand());
	add(new CPathWaypointRemoveFromToCommand());
}

CPathWaypointDeleteToCommand::CPathWaypointDeleteToCommand()
{
	setName("deleteto");
}

eBotCommandResult CPathWaypointDeleteToCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();

	if (CWaypoints::validWaypointIndex(pClient->currentWaypoint()))
	{
		CWaypoints::deletePathsTo(pClient->currentWaypoint());
	}

	return COMMAND_ACCESSED;;
}

CPathWaypointDeleteFromCommand::CPathWaypointDeleteFromCommand()
{
	setName("deletefrom");
}

eBotCommandResult CPathWaypointDeleteFromCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();

	if (CWaypoints::validWaypointIndex(pClient->currentWaypoint()))
	{
		CWaypoints::deletePathsFrom(pClient->currentWaypoint());
	}

	return COMMAND_ACCESSED;
}

CPathWaypointOnCommand::CPathWaypointOnCommand()
{
	setName("on");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointOnCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->setPathWaypoint(true);
	pClient->giveMessage("Pathwaypoints visible");
	return COMMAND_ACCESSED;
}

CPathWaypointOffCommand::CPathWaypointOffCommand()
{
	setName("off");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointOffCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->setPathWaypoint(false);
	pClient->giveMessage("Pathwaypoints hidden");
	return COMMAND_ACCESSED;
}

CWaypointAutoWaypointCommand::CWaypointAutoWaypointCommand()
{
	setName("autowaypoint");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CWaypointAutoWaypointCommand::execute(CClient* pClient, const char* pcmd, const char* arg1,
	const char* arg2, const char* arg3, const char* arg4,
	const char* arg5)
{
	if (pClient)
	{
		pClient->setAutoWaypointMode(atoi(pcmd) > 0, atoi(pcmd) == 2);
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "Autowaypointing Mode %s, Debug %s",
			(atoi(pcmd) > 0) ? "ON" : "OFF", (atoi(pcmd) == 2) ? "ON" : "OFF");
	}

	return COMMAND_ACCESSED;
}

CPathWaypointAutoOnCommand::CPathWaypointAutoOnCommand()
{
	setName("enable");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointAutoOnCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
		pClient->setAutoPath(true);
	return COMMAND_ACCESSED;
}

CPathWaypointAutoOffCommand::CPathWaypointAutoOffCommand()
{
	setName("disable");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointAutoOffCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
		pClient->setAutoPath(false);
	return COMMAND_ACCESSED;
}

CPathWaypointCreate1Command::CPathWaypointCreate1Command()
{
	setName("create1");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointCreate1Command::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pClient)
		return COMMAND_ERROR;

	pClient->updateCurrentWaypoint();

	if (pClient->currentWaypoint() == -1)
	{
		pClient->playSound("common/wpn_denyselect.wav");
	}
	else
	{
		pClient->setPathFrom(pClient->currentWaypoint());

		pClient->playSound("common/wpn_hudoff.wav");
	}

	return COMMAND_ACCESSED;
}

CPathWaypointCreate2Command::CPathWaypointCreate2Command()
{
	setName("create2");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointCreate2Command::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();
	pClient->setPathTo(pClient->currentWaypoint());

	CWaypoint* pWpt = CWaypoints::getWaypoint(pClient->getPathFrom());

	// valid?
	if (pWpt)
	{
		pWpt->addPathTo(pClient->getPathTo());
		pClient->playSound("buttons/button9");
	}
	else
		pClient->playSound("common/wpn_denyselect");

	return COMMAND_ACCESSED;
}

CPathWaypointCreateFromToCommand::CPathWaypointCreateFromToCommand()
{
	setName("createfromto");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointCreateFromToCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient && pcmd && *pcmd && arg1 && *arg1)
	{
		CWaypoint* pWaypoint = CWaypoints::getWaypoint(atoi(pcmd));

		if (pWaypoint && pWaypoint->isUsed())
		{
			CWaypoint* pWaypoint2 = CWaypoints::getWaypoint(atoi(arg1));

			if (pWaypoint2 && pWaypoint2->isUsed())
			{
				pWaypoint->addPathTo(atoi(arg1));
				CBotGlobals::botMessage(pClient != NULL ? pClient->getPlayer() : NULL,
					0, "Added path from <%d> to <%d>", atoi(pcmd), atoi(arg1));

				pWaypoint->draw(pClient->getPlayer(), true, DRAWTYPE_DEBUGENGINE);
				pWaypoint->info(pClient->getPlayer());
				pWaypoint2->draw(pClient->getPlayer(), true, DRAWTYPE_DEBUGENGINE);
				pWaypoint2->info(pClient->getPlayer());

				pClient->playSound("buttons/button9");

				return COMMAND_ACCESSED;
			}
			else
				CBotGlobals::botMessage(pClient != NULL ? pClient->getPlayer() : NULL,
					0, "Waypoint id <%d> not found", atoi(arg1));
		}
		else
			CBotGlobals::botMessage(pClient != NULL ? pClient->getPlayer() : NULL,
				0, "Waypoint id <%d> not found", atoi(pcmd));
	}
	else
		CBotGlobals::botMessage(pClient != NULL ? pClient->getPlayer() : NULL,
			0, "missing args <id1> <id2>");

	return COMMAND_ERROR;
}

CPathWaypointRemoveFromToCommand::CPathWaypointRemoveFromToCommand()
{
	setName("removefromto");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointRemoveFromToCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient && pcmd && *pcmd && arg1 && *arg1)
	{
		CWaypoint* pWaypoint = CWaypoints::getWaypoint(atoi(pcmd));

		if (pWaypoint && pWaypoint->isUsed())
		{
			CWaypoint* pWaypoint2 = CWaypoints::getWaypoint(atoi(arg1));

			if (pWaypoint2 && pWaypoint2->isUsed())
			{
				pWaypoint->removePathTo(atoi(arg1));
				CBotGlobals::botMessage(pClient != NULL ? pClient->getPlayer() : NULL,
					0, "Removed path from <%d> to <%d>", atoi(pcmd), atoi(arg1));

				pWaypoint->draw(pClient->getPlayer(), true, DRAWTYPE_DEBUGENGINE);
				pWaypoint->info(pClient->getPlayer());
				pWaypoint2->draw(pClient->getPlayer(), true, DRAWTYPE_DEBUGENGINE);
				pWaypoint2->info(pClient->getPlayer());

				pClient->playSound("buttons/button24");

				return COMMAND_ACCESSED;
			}
			else
				CBotGlobals::botMessage(pClient != NULL ? pClient->getPlayer() : NULL,
					0, "Waypoint id <%d> not found", atoi(arg1));
		}
		else
			CBotGlobals::botMessage(pClient != NULL ? pClient->getPlayer() : NULL,
				0, "Waypoint id <%d> not found", atoi(pcmd));
	}
	else
		CBotGlobals::botMessage(pClient != NULL ? pClient->getPlayer() : NULL,
			0, "missing args <id1> <id2>");

	return COMMAND_ERROR;
}

CPathWaypointRemove1Command::CPathWaypointRemove1Command()
{
	setName("remove1");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointRemove1Command::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();

	if (pClient->currentWaypoint() != -1)
	{
		pClient->setPathFrom(pClient->currentWaypoint());
		pClient->playSound("common/wpn_hudoff.wav");
	}
	else
		pClient->playSound("common/wpn_moveselect.wav");

	return COMMAND_ACCESSED;
}

CPathWaypointRemove2Command::CPathWaypointRemove2Command()
{
	setName("remove2");
	setAccessLevel(CMD_ACCESS_WAYPOINT);
}

eBotCommandResult CPathWaypointRemove2Command::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();
	pClient->setPathTo(pClient->currentWaypoint());

	CWaypoint* pWpt = CWaypoints::getWaypoint(pClient->getPathFrom());

	// valid?
	if (!pWpt)
		pClient->playSound("common/wpn_moveselect");
	else
	{
		pClient->playSound("buttons/button9");

		pWpt->removePathTo(pClient->getPathTo());
	}

	return COMMAND_ACCESSED;
}
/////////////
CWaypointAutoFix::CWaypointAutoFix()
{
	setName("autofix");
}

eBotCommandResult CWaypointAutoFix::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	bool bFixSentry_Sniper_Defend_TeleExtWpts = false;

	if (pcmd && *pcmd)
	{
		bFixSentry_Sniper_Defend_TeleExtWpts = (atoi(pcmd) == 1);
	}

	CWaypoints::autoFix(bFixSentry_Sniper_Defend_TeleExtWpts);

	return COMMAND_ACCESSED;
}

////////////

CWaypointAreaSetToNearest::CWaypointAreaSetToNearest()
{
	setName("setareatonearest");
}

eBotCommandResult CWaypointAreaSetToNearest::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		int id;
		CWaypoint* pWpt;
		bool bOk = false;
		int setarea = 0;

		id = pClient->currentWaypoint();

		if ((pWpt = CWaypoints::getWaypoint(id)) != NULL)
		{
			if (pWpt->isUsed())
			{
				if (CBotGlobals::isCurrentMod(MOD_TF2))
					setarea = CTeamFortress2Mod::m_ObjectiveResource.NearestArea(pWpt->getOrigin());
				else if (CBotGlobals::isCurrentMod(MOD_DOD))
					setarea = CDODMod::m_Flags.findNearestObjective(pWpt->getOrigin());

				if (setarea > 0)
					pWpt->setArea(setarea);

				bOk = true;
			}
		}

		if (bOk)
		{
			if (setarea > 0)
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "Changed waypoint %d area to %d", id, setarea);
			else
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "No nearest area to wpt id %d", id);
		}
		else
		{
			CBotGlobals::botMessage(pClient->getPlayer(), 0, "Invalid waypoint id %d", id);
		}
		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}

///////////////
CWaypointCheckCommand::CWaypointCheckCommand()
{
	setName("check");
}

eBotCommandResult CWaypointCheckCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	// loop through every waypoint and check the areas are not outside the number of control points

	CWaypoints::checkAreas((pClient == NULL) ? NULL : pClient->getPlayer());

	return COMMAND_ACCESSED;
}

CWaypointTeleportCommand::CWaypointTeleportCommand()
{
	setName("teleport");
}

eBotCommandResult CWaypointTeleportCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient && pcmd && *pcmd)
	{
		int id;
		CWaypoint* pWpt;
		bool bTele = false;

		id = atoi(pcmd);

		if ((pWpt = CWaypoints::getWaypoint(id)) != NULL)
		{
			if (pWpt->isUsed())
			{
				pClient->teleportTo(pWpt->getOrigin() + Vector(0, 0, 8));
				bTele = true;
			}
		}

		if (bTele)
		{
			CBotGlobals::botMessage(pClient->getPlayer(), 0, "Teleported to waypoint %d", id);
		}
		else
		{
			CBotGlobals::botMessage(pClient->getPlayer(), 0, "Invalid waypoint id %d", id);
		}

		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}
//////////////

CWaypointAngleCommand::CWaypointAngleCommand()
{
	setName("angle");
}

eBotCommandResult CWaypointAngleCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient && pClient->getPlayer())
	{
		pClient->updateCurrentWaypoint();

		CWaypoint* pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

		if (pWpt)
		{
			QAngle eye = CBotGlobals::playerAngles(pClient->getPlayer());
			CBotGlobals::botMessage(pClient->getPlayer(), 0, "Waypoint Angle == %0.3f deg, (Eye == %0.3f)", CBotGlobals::yawAngleFromEdict(pClient->getPlayer(), pWpt->getOrigin()), eye.y);
			pClient->playSound("buttons/combine_button1");
		}
	}

	return COMMAND_ACCESSED;
}

CWaypointInfoCommand::CWaypointInfoCommand()
{
	setName("info");
}

eBotCommandResult CWaypointInfoCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	pClient->updateCurrentWaypoint();

	CWaypoint* pWpt = CWaypoints::getWaypoint(pClient->currentWaypoint());

	if (pWpt)
		pWpt->info(pClient->getPlayer());

	return COMMAND_ACCESSED;
}

eBotCommandResult CWaypointSaveCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (CWaypoints::save(false, (pClient != NULL) ? pClient->getPlayer() : NULL, ((pcmd != NULL) && (*pcmd != 0)) ? pcmd : NULL, ((arg1 != NULL) && (*arg1 != 0)) ? arg1 : NULL))
	{
		CBotGlobals::botMessage(NULL, 0, "waypoints saved");
		if (pClient)
			pClient->giveMessage("Waypoints Saved");
	}
	else
		CBotGlobals::botMessage(NULL, 0, "error: could not save waypoints");

	return COMMAND_ACCESSED;
}

eBotCommandResult CWaypointLoadCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	char* szMapName = CBotGlobals::getMapName();
	bool bLoadOK = false;

	if (pcmd && *pcmd)
	{
		bLoadOK = CWaypoints::load(pcmd);
		szMapName = const_cast<char*>(pcmd);
	}
	else
		bLoadOK = CWaypoints::load();

	if (bLoadOK)
		CBotGlobals::botMessage(NULL, 0, "waypoints %s loaded", szMapName);
	else
		CBotGlobals::botMessage(NULL, 0, "error: could not load %s waypoints", szMapName);

	return COMMAND_ACCESSED;
}

//usage \"memorycheck <classname> <offset> <type>\"");
eBotCommandResult CDebugMstrOffsetSearch::execute(CClient *pClient, const char *pcmd, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5)
{
	if (strcmp("cp_dustbowl", STRING(gpGlobals->mapname)) != 0)
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "Command can only be used on cp_dustbowl -- change the map first");
		return COMMAND_ERROR;
	}

	edict_t *pMaster = CClassInterface::FindEntityByClassnameNearest(Vector(0, 0, 0), "team_control_point_master", 65535);

	if (pMaster == NULL)
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "pMaster not found -- have you started the game yet?");
		return COMMAND_ERROR;
	}

	extern IServerGameEnts *servergameents;

	CBaseEntity *pMasterEntity = servergameents->EdictToBaseEntity(pMaster);

	//local variable is initialized but not referenced - [APG]RoboCop[CL]
	unsigned long full_size = sizeof(pMasterEntity);
	unsigned long offset = 800;
	

	while (offset < 1000)
	{
		unsigned long mempoint = ((unsigned long)pMasterEntity) + offset;
		CTeamControlPointMaster* PointMaster = (CTeamControlPointMaster*)mempoint;

#ifdef WIN32
		__try
		{
			if (PointMaster->m_iTeamBaseIcons[0] == 0 && PointMaster->m_iTeamBaseIcons[2] == 5 && PointMaster->m_iTeamBaseIcons[3] == 6)
			{
				if (strcmp(PointMaster->m_iszTeamBaseIcons[3].ToCStr(), "sprites/obj_icons/icon_base_blu") == 0)
				{
					CBotGlobals::botMessage(pClient->getPlayer(), 0, "pMaster offset is %d", offset);
					return COMMAND_ACCESSED;
				}
			}
		}
		
		__except (0)
		{
			// SEH handling 
		}
		offset++;
#endif
		
	}

	return COMMAND_ERROR;
}

//usage \"memorycheck <classname> <offset> <type>\"");
eBotCommandResult CDebugMemoryCheckCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	// pcmd = classname
	// arg1 = offset
	// arg2 = type
	NEED_ARG(pcmd);
	NEED_ARG(arg1);
	NEED_ARG(arg2);
	// find edict
	edict_t* pEdict = CClassInterface::FindEntityByClassnameNearest(pClient->getOrigin(), pcmd);

	if (pEdict == NULL)
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "Edict not found");
		return COMMAND_ERROR;
	}

	CBaseEntity* pent = pEdict->GetUnknown()->GetBaseEntity();

	unsigned int offset = atoi(arg1);

	if ((strcmp(arg2, "bool") == 0) || (strcmp(arg2, "byte") == 0))
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "%s - offset %d - Value(byte) = %d", pcmd, offset, *reinterpret_cast<byte*>(reinterpret_cast<unsigned long>(pent) + offset));
	}
	else if (strcmp(arg2, "int") == 0)
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "%s - offset %d - Value(int) = %d", pcmd, offset, *reinterpret_cast<int*>(reinterpret_cast<unsigned long>(pent) + offset));
		/*
		if ( strcmp(pcmd,"team_control_point_master") == 0 )
		{
			CTeamControlPointMaster *p;
			CTeamControlPointMaster check;

			unsigned int knownoffset = (unsigned int)&check.m_iCurrentRoundIndex - (unsigned int)&check;

			p = (CTeamControlPointMaster*)((((unsigned long)pent) + offset) - knownoffset); //MAP_CLASS(CTeamControlPoint,(((unsigned long)pent) + offset),knownoffset);
		}
		else if ( strcmp(pcmd,"team_control_point") == 0 )
		{
			extern ConVar rcbot_const_point_offset;
			extern ConVar rcbot_const_point_data_offset;

			CTeamControlPoint *p = (CTeamControlPoint*)((((unsigned long)pent) + rcbot_const_point_offset.GetInt())); //MAP_CLASS(CTeamControlPoint,(((unsigned long)pent) + offset),knownoffset);
//			CTeamControlPointData *d = (CTeamControlPointData*)((((unsigned long)pent) + rcbot_const_point_data_offset.GetInt()));

			CBotGlobals::botMessage(NULL,0,"NULL MSG");
		}*/
	}
	else if (strcmp(arg2, "float") == 0)
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "%s - offset %d - Value(float) = %0.6f", pcmd, offset, *reinterpret_cast<float*>(reinterpret_cast<unsigned long>(pent) + offset));
	else if (strcmp(arg2, "string") == 0)
	{
		string_t* str = reinterpret_cast<string_t*>(reinterpret_cast<unsigned long>(pent) + offset);
		if (str)
			CBotGlobals::botMessage(pClient->getPlayer(), 0, "%s - offset %d - Value(string) = %s", pcmd, offset, STRING(*str));
		else
			CBotGlobals::botMessage(pClient->getPlayer(), 0, "%s - offset %d - INVALID string", pcmd, offset);
	}
	else
		return COMMAND_ERROR;

	return COMMAND_ACCESSED;
}

#define MEMSEARCH_BYTE 1
#define MEMSEARCH_INT 2
#define MEMSEARCH_FLOAT 3
#define MEMSEARCH_STRING 4

eBotCommandResult CDebugMemoryScanCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	//pcmd = classname
	// arg1 = value
	// arg2 = size in bytes
	// arg3 = want to remember offsets or not

	NEED_ARG(pcmd);
	NEED_ARG(arg1);
	NEED_ARG(arg2);

	unsigned int m_prev_size = m_size;

	if ((strcmp(arg2, "bool") == 0) || (strcmp(arg2, "byte") == 0))
		m_size = MEMSEARCH_BYTE;
	else if (strcmp(arg2, "int") == 0)
		m_size = MEMSEARCH_INT;
	else if (strcmp(arg2, "float") == 0)
		m_size = MEMSEARCH_FLOAT;
	else if (strcmp(arg2, "string") == 0)
		m_size = MEMSEARCH_STRING;
	else
		m_size = 0;

	if ((m_prev_size != m_size) || ((m_size == 0) || !arg3 || !*arg3) || (atoi(arg3) == 0))
	{
		memset(stored_offsets, 0, sizeof(u_MEMSEARCH) * MAX_MEM_SEARCH);
	}

	// find edict
	edict_t* pEdict = CClassInterface::FindEntityByClassnameNearest(pClient->getOrigin(), pcmd);

	if (pEdict == NULL)
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "Edict not found");
		return COMMAND_ERROR;
	}

	// begin memory scan
	CBaseEntity* pent = pEdict->GetUnknown()->GetBaseEntity();

	byte* mempoint = reinterpret_cast<byte*>(pent);
	byte value = static_cast<byte>(atoi(arg1));
	int ivalue = (atoi(arg1));
	float fvalue = (atof(arg1));

	bool bfound;

	for (int i = 0; i < MAX_MEM_SEARCH; i++) // 2KB search
	{
		bfound = false;

		if (m_size == MEMSEARCH_BYTE)
			bfound = (value == *mempoint);
		else if (m_size == MEMSEARCH_INT)
			bfound = (ivalue == *reinterpret_cast<int*>(mempoint));
		else if (m_size == MEMSEARCH_FLOAT)
			bfound = (fvalue == *reinterpret_cast<float*>(mempoint));
		else if (m_size == MEMSEARCH_STRING)
		{
			try
			{
				string_t* str = reinterpret_cast<string_t*>(mempoint);

				if (str != NULL)
				{
					const char* pszstr = STRING(*str);

					if (pszstr)
						bfound = (strcmp(pszstr, arg1) == 0);
				}
			}
			catch (...)
			{
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "Invalid string");
			}
		}

		if (bfound)
		{
			if (!stored_offsets[i].b1.searched)
				stored_offsets[i].b1.found = 1;
		}
		else if (stored_offsets[i].b1.searched)
			stored_offsets[i].b1.found = 0;

		stored_offsets[i].b1.searched = 1;

		mempoint++;
	}

	// Current valid offsets print
	for (int i = 0; i < MAX_MEM_SEARCH; i++)
	{
		if (stored_offsets[i].data != 0)
		{
			if (stored_offsets[i].b1.found)
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "%d", i);
		}
	}
	//

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugGameEventCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_GAME_EVENT, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugVisCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_VIS, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugThinkCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_THINK, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugLookCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_LOOK, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugHudCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_HUD, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}
eBotCommandResult CDebugAimCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_AIM, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugChatCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_CHAT, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugProfilingCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_PROFILE, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugNavCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_NAV, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugTaskCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_TASK, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CGodModeUtilCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		edict_t* pEntity = pClient->getPlayer();

		if (pEntity)
		{
			int* playerflags = CClassInterface::getPlayerFlagsPointer(pEntity);

			if (playerflags)
			{
				char msg[256];

				if (*playerflags & FL_GODMODE)
					*playerflags &= ~FL_GODMODE;
				else
					*playerflags |= FL_GODMODE;

				sprintf(msg, "god mode %s", (*playerflags & FL_GODMODE) ? "enabled" : "disabled");

				//CRCBotPlugin::HudTextMessage(pEntity,msg);
				CBotGlobals::botMessage(pEntity, 0, msg);

				return COMMAND_ACCESSED;
			}
		}
	}

	return COMMAND_ERROR;
}

eBotCommandResult CSetTeleportUtilCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		pClient->setTeleportVector();
		engine->ClientPrintf(pClient->getPlayer(), "Teleport Position Remembered!");
		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}

eBotCommandResult CTeleportUtilCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		Vector* vTeleport;

		vTeleport = pClient->getTeleportVector();

		if (vTeleport != NULL)
		{
			CBotGlobals::teleportPlayer(pClient->getPlayer(), *vTeleport);
			//CRCBotPlugin::HudTextMessage(pClient->getPlayer(),"teleported to your remembered location");
			CBotGlobals::botMessage(pClient->getPlayer(), 0, "teleported to your remembered location");

			return COMMAND_ACCESSED;
		}
	}

	return COMMAND_ERROR;
}

eBotCommandResult CNoTouchCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient)
	{
		edict_t* pEntity = pClient->getPlayer();

		if (pEntity)
		{
			int* playerflags = CClassInterface::getPlayerFlagsPointer(pEntity);

			if (playerflags)
			{
				char msg[256];

				if (*playerflags & FL_DONTTOUCH)
					*playerflags &= ~FL_DONTTOUCH;
				else
					*playerflags |= FL_DONTTOUCH;

				sprintf(msg, "notouch mode %s", (*playerflags & FL_DONTTOUCH) ? "enabled" : "disabled");
				CBotGlobals::botMessage(NULL, 0, msg);
				//CRCBotPlugin::HudTextMessage(pEntity,msg);

				return COMMAND_ACCESSED;
			}
		}
	}

	return COMMAND_ERROR;
}

eBotCommandResult CNoClipCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pEntity = NULL;

	if (pClient)
		pEntity = pClient->getPlayer();

	if (pEntity)
	{
		char msg[256];
		byte* movetype = CClassInterface::getMoveTypePointer(pEntity);

		if ((*movetype & 15) != MOVETYPE_NOCLIP)
		{
			*movetype &= ~15;
			*movetype |= MOVETYPE_NOCLIP;
		}
		else
		{
			*movetype &= ~15;
			*movetype |= MOVETYPE_WALK;
		}

		sprintf(msg, "%s used no_clip %d on self\n", pClient->getName(), ((*movetype & 15) == MOVETYPE_NOCLIP));

		// CRCBotPlugin::HudTextMessage(pEntity,msg);
		CBotGlobals::botMessage(pEntity, 0, msg);
		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}

eBotCommandResult CDebugUtilCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_UTIL, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugEdictsCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_EDICTS, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugSpeedCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_SPEED, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugUsercmdCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_USERCMD, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugButtonsCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	pClient->setDebug(BOT_DEBUG_BUTTONS, atoi(pcmd) > 0);

	return COMMAND_ACCESSED;
}

eBotCommandResult CDebugBotCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if ((!pcmd || !*pcmd))
	{
		extern IServerGameEnts* servergameents;
		// do a traceline in front of player

		Vector vOrigin = pClient->getOrigin();
		QAngle angles = CBotGlobals::entityEyeAngles(pClient->getPlayer());
		Vector forward;

		AngleVectors(angles, &forward);

		CBotGlobals::quickTraceline(pClient->getPlayer(), vOrigin, vOrigin + forward * 1024.0f);
		CBaseEntity* pEntity;

		if ((pEntity = CBotGlobals::getTraceResult()->m_pEnt) != NULL)
		{
			edict_t* pEdict = servergameents->BaseEntityToEdict(pEntity);
			if (CBots::getBotPointer(pEdict) != NULL)
			{
				pClient->setDebugBot(pEdict);
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "debug bot set to bot you are looking at");
				return COMMAND_ACCESSED;
			}
			else
			{
				pClient->setDebugBot(NULL);
				CBotGlobals::botMessage(pClient->getPlayer(), 0, "debug bot cleared");
			}
		}
		else
		{
			pClient->setDebugBot(NULL);
			CBotGlobals::botMessage(pClient->getPlayer(), 0, "debug bot cleared");
		}
		return COMMAND_ERROR;
	}

	edict_t* pEnt = CBotGlobals::findPlayerByTruncName(pcmd);

	if (!pEnt)
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "can't find a player with that name");
		return COMMAND_ERROR;
	}

	CBot* pBot = CBots::getBotPointer(pEnt);

	if (!pBot)
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "can't find a bot with that name");
		return COMMAND_ERROR;
	}

	pClient->setDebugBot(pBot->getEdict());

	return COMMAND_ACCESSED;
}

///////////////////////
// command

CUtilCommand::CUtilCommand()
{
	setName("util");
	add(new CSearchCommand());
	add(new CSetTeleportUtilCommand());
	add(new CTeleportUtilCommand());
	add(new CNoClipCommand());
	add(new CGodModeUtilCommand());
	add(new CNoTouchCommand());
}

CConfigCommand::CConfigCommand()
{
	setName("config");
	add(new CGameEventVersion());
	add(new CMaxBotsCommand());
	add(new CMinBotsCommand());
}

eBotCommandResult CGameEventVersion::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
		return COMMAND_ERROR;

	CBotGlobals::setEventVersion(atoi(pcmd));

	return COMMAND_ACCESSED;
}

// kickbot
eBotCommandResult CKickBotCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (!pcmd || !*pcmd)
	{
		//remove random bot
		CBots::kickRandomBot();
	}
	else
	{
		int team = atoi(pcmd);

		CBots::kickRandomBotOnTeam(team);
	}

	return COMMAND_ACCESSED;
}

eBotCommandResult CShowUsersCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pEntity = NULL;

	if (pClient)
		pEntity = pClient->getPlayer();

	CAccessClients::showUsers(pEntity);

	return COMMAND_ACCESSED;
}

eBotCommandResult CMaxBotsCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pEntity = NULL;

	if (pClient)
		pEntity = pClient->getPlayer();

	if (pcmd && *pcmd)
	{
		int max = atoi(pcmd);

		bool err = false;
		int min_bots = CBots::getMinBots();

		if (max <= -1)// skip check for disabling max bots (require <=)
			max = -1;
		else if ((min_bots >= 0) && (max <= min_bots))
		{
			CBotGlobals::botMessage(pEntity, 0, "max_bots must be greater than min_bots (min_bots is currently : %d)", min_bots);
			err = true;
		}
		if (max > CBotGlobals::maxClients())
			max = CBotGlobals::maxClients();

		if (!err)
		{
			CBots::setMaxBots(max);

			CBotGlobals::botMessage(pEntity, 0, "max_bots set to %d", max);
		}
	}
	else
		CBotGlobals::botMessage(pEntity, 0, "max_bots is currently %d", CBots::getMaxBots());

	return COMMAND_ACCESSED;
}

eBotCommandResult CMinBotsCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pEntity = NULL;

	if (pClient)
		pEntity = pClient->getPlayer();

	if (pcmd && *pcmd)
	{
		int min = atoi(pcmd);
		int max_bots = CBots::getMaxBots();

		bool err = false;

		if (min > CBotGlobals::maxClients())
			min = CBotGlobals::maxClients();

		if (min <= -1) // skip check for disabling min bots (require <=)
			min = -1;
		else if ((max_bots >= 0) && (min >= CBots::getMaxBots()))
		{
			CBotGlobals::botMessage(pEntity, 0, "min_bots must be less than max_bots (max_bots is currently : %d)", max_bots);
			err = true;
		}

		if (!err)
		{
			CBots::setMinBots(min);

			CBotGlobals::botMessage(pEntity, 0, "min_bots set to %d", min);
		}
	}
	else
		CBotGlobals::botMessage(pEntity, 0, "min_bots is currently %d", CBots::getMinBots());

	return COMMAND_ACCESSED;
}

eBotCommandResult CSearchCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	int i = 0;

	edict_t* pPlayer = pClient->getPlayer();
	edict_t* pEdict;
	float fDistance;
	string_t model;

	for (i = 0; i < gpGlobals->maxEntities; i++)
	{
		pEdict = INDEXENT(i);

		if (pEdict)
		{
			if (!pEdict->IsFree())
			{
				if (pEdict->m_pNetworkable && pEdict->GetIServerEntity())
				{
					if ((fDistance = (CBotGlobals::entityOrigin(pEdict) - CBotGlobals::entityOrigin(pPlayer)).Length()) < 128)
					{
						float fVelocity;
						Vector v;

						if (CClassInterface::getVelocity(pEdict, &v))
							fVelocity = v.Length();
						else
							fVelocity = 0;

						model = pEdict->GetIServerEntity()->GetModelName();

						CBotGlobals::botMessage(pPlayer, 0, "(%d) D:%0.2f C:'%s', Mid:%d, Mn:'%s' Health=%d, Tm:%d, Fl:%d, Spd=%0.2f", i, fDistance, pEdict->GetClassName(), pEdict->GetIServerEntity()->GetModelIndex(), model.ToCStr(), static_cast<int>(CClassInterface::getPlayerHealth(pEdict)), static_cast<int>(CClassInterface::getTeam(pEdict)), pEdict->m_fStateFlags, fVelocity);
					}
				}
			}
		}
	}

	return COMMAND_ACCESSED;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CBotCommand::setName(char* szName)
{
	m_szCommand = CStrings::getString(szName);
}

void CBotCommand::setHelp(char* szHelp)
{
	m_szHelp = CStrings::getString(szHelp);
}

void CBotCommand::setAccessLevel(const int iAccessLevel)
{
	m_iAccessLevel = iAccessLevel;
}

CBotCommand::CBotCommand(char* szCommand, const int iAccessLevel)
{
	m_szCommand = CStrings::getString(szCommand);
	m_iAccessLevel = iAccessLevel;
}

void CBotCommand::freeMemory()
{
	// nothing to free -- done in CStrings
}

bool CBotCommand::hasAccess(CClient* pClient)
{
	return (m_iAccessLevel & pClient->accessLevel()) == m_iAccessLevel;
}

bool CBotCommand::isCommand(const char* szCommand)
{
	return FStrEq(szCommand, m_szCommand);
}

eBotCommandResult CBotCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	return COMMAND_NOT_FOUND;
}

////////////////////////////
// container of commands
eBotCommandResult CBotCommandContainer::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	for (unsigned int i = 0; i < m_theCommands.size(); i++)
	{
		CBotCommand* pCommand = m_theCommands[i];

		if (pCommand->isCommand(pcmd))
		{
			if (pClient && !pCommand->hasAccess(pClient))
				return COMMAND_REQUIRE_ACCESS;
			if (!pClient && !canbeUsedDedicated())
			{
				CBotGlobals::botMessage(NULL, 0, "Sorry, this command cannot be used on a dedicated server");
				return COMMAND_ERROR;
			}
			// move arguments
			eBotCommandResult iResult = pCommand->execute(pClient, arg1, arg2, arg3, arg4, arg5, NULL);

			if (iResult == COMMAND_ERROR)
			{
				if (pClient)
					pCommand->printHelp(pClient->getPlayer());
				else
					pCommand->printHelp(NULL);
			}

			return COMMAND_ACCESSED;
		}
	}

	if (pClient)
		printHelp(pClient->getPlayer());
	else
		printHelp(NULL);

	return COMMAND_NOT_FOUND;
}

void CBotCommandContainer::freeMemory()
{
	for (unsigned int i = 0; i < m_theCommands.size(); i++)
	{
		m_theCommands[i]->freeMemory();
		delete m_theCommands[i];
		m_theCommands[i] = NULL;
	}

	m_theCommands.clear();
}
//////////////////////////////////////////

eBotCommandResult CPrintCommands::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient != NULL)
	{
		CBotGlobals::botMessage(pClient->getPlayer(), 0, "All bot commands:");
		CBotGlobals::m_pCommands->printCommand(pClient->getPlayer());
	}
	else
	{
		CBotGlobals::botMessage(NULL, 0, "All bot commands:");
		CBotGlobals::m_pCommands->printCommand(NULL);
	}

	return COMMAND_ACCESSED;
}

///////////////////////////////////////////

void CBotCommand::printCommand(edict_t* pPrintTo, const int indent)
{
	if (indent)
	{
		const int maxIndent = 64;
		char szIndent[maxIndent];
		int i;

		for (i = 0; (i < (indent * 2)) && (i < maxIndent - 1); i++)
			szIndent[i] = ' ';

		szIndent[maxIndent - 1] = 0;
		szIndent[i] = 0;

		if (!pPrintTo && !canbeUsedDedicated())
			CBotGlobals::botMessage(pPrintTo, 0, "%s%s [can't use]", szIndent, m_szCommand);
		else
			CBotGlobals::botMessage(pPrintTo, 0, "%s%s", szIndent, m_szCommand);
	}
	else
	{
		if (!pPrintTo && !canbeUsedDedicated())
			CBotGlobals::botMessage(pPrintTo, 0, "%s [can't use]", m_szCommand);
		else
			CBotGlobals::botMessage(pPrintTo, 0, m_szCommand);
	}
}

void CBotCommand::printHelp(edict_t* pPrintTo)
{
	if (m_szHelp)
		CBotGlobals::botMessage(pPrintTo, 0, m_szHelp);
	else
		CBotGlobals::botMessage(pPrintTo, 0, "Sorry, no help for this command (yet)");

	return;
}

void CBotCommandContainer::printCommand(edict_t* pPrintTo, const int indent)
{
	//char cmd1[512];
	//char cmd2[512];

	//sprintf(cmd1,"%%%ds",indent);
	//sprintf(cmd2,cmd1,m_szCommand);

	if (indent)
	{
		const int maxIndent = 64;
		char szIndent[maxIndent];

		int i;

		for (i = 0; (i < (indent * 2)) && (i < maxIndent - 1); i++)
			szIndent[i] = ' ';

		szIndent[maxIndent - 1] = 0;
		szIndent[i] = 0;

		CBotGlobals::botMessage(pPrintTo, 0, "%s[%s]", szIndent, m_szCommand);
	}
	else
		CBotGlobals::botMessage(pPrintTo, 0, "[%s]", m_szCommand);

	for (unsigned int i = 0; i < m_theCommands.size(); i++)
	{
		m_theCommands[i]->printCommand(pPrintTo, indent + 1);
	}
}

void CBotCommandContainer::printHelp(edict_t* pPrintTo)
{
	printCommand(pPrintTo);
	return;
}

eBotCommandResult CTestCommand::execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	// for developers
	return COMMAND_NOT_FOUND;
}