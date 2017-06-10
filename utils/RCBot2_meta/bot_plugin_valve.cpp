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
 *///=================================================================================//
//
// HPB_bot2_main.cpp - bot source code file (Copyright 2004, Jeffrey "botman" Broome)
//
//=================================================================================//

#include <stdio.h>
#include <time.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#endif

//#include "cbase.h"
//#include "baseentity.h"
#include "filesystem.h"
#include "interface.h"
#include "engine/iserverplugin.h"
#include "iplayerinfo.h"
#include "IEngineTrace.h"
#include "tier2/tier2.h"
#ifdef __linux__
#include "shake.h"    //bir3yk
#endif
#include "IEffects.h"
#include "vplane.h"
#include "eiface.h"
#include "igameevents.h"

#include "Color.h"
#include "ndebugoverlay.h"
#include "server_class.h"
#include "time.h"
#include "irecipientfilter.h"
#include "bot.h"
#include "bot_commands.h"
#include "bot_client.h"
#include "bot_globals.h"
#include "bot_accessclient.h"
#include "bot_waypoint_visibility.h" // for initializing table
#include "bot_event.h"
#include "bot_profile.h"
#include "bot_weapons.h"
#include "bot_mods.h"
#include "bot_profiling.h"
#include "bot_menu.h"
#include "bot_squads.h"
#include "bot_kv.h"
#include "bot_fortress.h"
#include "vstdlib/random.h" // for random  seed 

#include "bot_wpt_dist.h"

#include "bot_configfile.h"

#include "bot_getprop.h"

#include "bot_sigscan.h"

#include "bot_hooks.h"

#include "bot_cvars.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "bot_valve_plguin.h"

bool bInitialised = false;

// Interfaces from the engine*/
IVEngineServer *engine = NULL;  // helper functions (messaging clients, loading content, making entities, running commands, etc)
IFileSystem *filesystem = NULL;  // file I/O 
IGameEventManager2 *gameeventmanager = NULL;
IGameEventManager *gameeventmanager1 = NULL;  // game events interface
IPlayerInfoManager *playerinfomanager = NULL;  // game dll interface to interact with players
IServerPluginHelpers *helpers = NULL;  // special 3rd party plugin helpers from the engine
IServerGameClients* gameclients = NULL;
IEngineTrace *enginetrace = NULL;
IEffects *g_pEffects = NULL;
IBotManager *g_pBotManager = NULL;
CGlobalVars *gpGlobals = NULL;
IVDebugOverlay *debugoverlay = NULL;
IServerGameEnts *servergameents = NULL; // for accessing the server game entities
IServerGameDLL *servergamedll = NULL;
IServerTools *servertools = NULL;

void (CBaseEntity::*RemovePlayerItem)(CBaseEntity*) = 0x0;
void (CBaseEntity::*Touch)(CBaseEntity*) = 0x0;
//GCC takes the this pointer on the stack as the first parameter

void UTIL_ApplyAttribute ( edict_t *pEdict, const char *name, float fVal )
{
	CAttributeList *address = CClassInterface::getAttributeList(pEdict);

	if ( address == NULL )
	{
		CBotGlobals::botMessage(NULL,1,"getAttributeList not found");
		return;
	}

	CEconItemAttributeDefinition *def = g_pGetAttributeDefinitionByName->callme(g_pGetEconItemSchema->callme(),name);

	if ( def )
	{
		g_pSetRuntimeAttributeValue->callme(pEdict,address,def,fVal);			
	}
	else
		CBotGlobals::botMessage(NULL,1,"CEconItemAttributeDefinition for %s not found",name);
}


CON_COMMAND( rcbot_ragemeter, "get rage meter value" )
{
	// get listen server edict
	edict_t *pplayer = CClients::getListenServerClient();

	if ( pplayer )
	{
		CBotGlobals::botMessage(NULL,0,"ragemeter = %0.2f",CClassInterface::getRageMeter(pplayer));
	}
}
/*
CON_COMMAND( rcbot_attribtest, "attributes for tf2 test" )
{
	// get listen server edict
	edict_t *pplayer = CClients::getListenServerClient();

	if ( pplayer && (args.ArgC() > 3))
	{
		edict_t *pEnt;
		
		if ( strcmp(args.Arg(1),"player") == 0 )
			pEnt = pplayer;
		else
			pEnt = CClassInterface::FindEntityByClassnameNearest(CBotGlobals::entityOrigin(pplayer),args.Arg(1),200.0f);

		const char *attrib = args.Arg(2);
		float fValue = atof(args.Arg(3));

		UTIL_ApplyAttribute(pEnt,attrib,fValue);

			// Call AttributeChanged
			
			//DWORD *mem = ( DWORD* )*( DWORD* )address;
			//*(DWORD*)&OnAttributesChanged = mem[12];CAttributeListSetValue
			//CAttributeManager *pAttributeManager = (CAttributeManager *)((unsigned long)address + 24);	

			//if ( pAttributeManager && OnAttributesChanged )
			//{
			//	(*pAttributeManager.*OnAttributesChanged)();
			//}
	
			
	}
}*/

CON_COMMAND( rcbot_enginetime, "get engine time" )
{
	Msg("%f\n",engine->Time());
}

CON_COMMAND( rcbot_maptime, "get map time" )
{
	Msg("CDODMod::getMapStartTime() = %f\n",CDODMod::getMapStartTime());
	Msg("engine->Time() = %f\n",engine->Time());
	Msg("diff = %f\n",engine->Time() - CDODMod::getMapStartTime());
	Msg("gpGlobals->realtime = %f\n",gpGlobals->realtime);
	Msg("gpGlobals->curtime = %f\n",gpGlobals->curtime);
}

//CBaseEntityList * g_pEntityList;
// 
// The plugin is a static singleton that is exported as an interface
//
CRCBotPlugin g_RCBOTServerPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CRCBotPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_RCBOTServerPlugin );

//---------------------------------------------------------------------------------
// Purpose: constructor/destructor
//---------------------------------------------------------------------------------
CRCBotPlugin::CRCBotPlugin()
{
	m_iClientCommandIndex = 0;
}

CRCBotPlugin::~CRCBotPlugin()
{
}

//------------
//---------------------------------------------------------------------------------
// Purpose: an example of how to implement a new command
//---------------------------------------------------------------------------------
static ConVar empty_cvar(BOT_VER_CVAR, BOT_VER, FCVAR_REPLICATED, BOT_NAME_VER);

CON_COMMAND( rcbotd, "access the bot commands on a server" )
{
        eBotCommandResult iResult;

		if ( !engine->IsDedicatedServer() || !CBotGlobals::IsMapRunning() )
		{
			CBotGlobals::botMessage(NULL,0,"Error, no map running or not dedicated server");
			return;
		}

		//iResult = CBotGlobals::m_pCommands->execute(NULL,engine->Cmd_Argv(1),engine->Cmd_Argv(2),engine->Cmd_Argv(3),engine->Cmd_Argv(4),engine->Cmd_Argv(5),engine->Cmd_Argv(6));
		iResult = CBotGlobals::m_pCommands->execute(NULL,args.Arg(1),args.Arg(2),args.Arg(3),args.Arg(4),args.Arg(5),args.Arg(6));

		if ( iResult == COMMAND_ACCESSED )
		{
			// ok
		}
		else if ( iResult == COMMAND_REQUIRE_ACCESS )
		{
			CBotGlobals::botMessage(NULL,0,"You do not have access to this command");
		}
		else if ( iResult == COMMAND_NOT_FOUND )
		{
			CBotGlobals::botMessage(NULL,0,"bot command not found");	
		}
		else if ( iResult == COMMAND_ERROR )
		{
			CBotGlobals::botMessage(NULL,0,"bot command returned an error");	
		}
}

void CRCBotPlugin::OnEdictAllocated( edict_t *edict )
{

}

void CRCBotPlugin::OnEdictFreed( const edict_t *edict  )
{

}

class CBotRecipientFilter : public IRecipientFilter
{
public:
	CBotRecipientFilter ( edict_t *pPlayer )
	{
		m_iPlayerSlot = ENTINDEX(pPlayer);
	}

	bool IsReliable( void ) const { return false; }
	bool IsInitMessage( void ) const { return false; }

	int	GetRecipientCount( void ) const { return 1; }
	int	GetRecipientIndex( int slot ) const { return m_iPlayerSlot; }

private:
	int m_iPlayerSlot;
};///////////////
// hud message


void CRCBotPlugin :: HudTextMessage ( edict_t *pEntity, const char *szMessage )
{
	int msgid = 0;
	int imsgsize = 0;
	char msgbuf[64];
	bool bOK;

	while ( (bOK=servergamedll->GetUserMessageInfo(msgid,msgbuf,63,imsgsize))==true )
	{
		if ( strcmp(msgbuf,"HintText") == 0 )
			break;
		else
			msgid++;
	}

	if ( msgid == 0 )
		return;
	if ( !bOK )
		return;

	CBotRecipientFilter *filter = new CBotRecipientFilter(pEntity);

	bf_write *buf = engine->UserMessageBegin(filter,msgid);

	buf->WriteString(szMessage);
	
	engine->MessageEnd();

	delete filter;

	/*
	KeyValues *kv = new KeyValues( "msg" );
	kv->SetString( "title", szMessage );
	kv->SetString( "msg", "This is the msg" );
	
	kv->SetColor( "color", colour);
	kv->SetInt( "level", level);
	kv->SetInt( "time", time);
//DIALOG_TEXT
	helpers->CreateMessage( pEntity, DIALOG_MSG, kv, &g_RCBOTServerPlugin );

	kv->deleteThis();*/
}

#ifdef __linux__
#define LOAD_INTERFACE(var,type,version) if ( (var = (type*)interfaceFactory(version,NULL)) == NULL ) { Warning("[RCBOT] Cannot open interface "#version " "#type " "#var "\n"); return false; } else { Msg("[RCBOT] Found interface "#version " "#type " "#var "\n"); }
#define LOAD_GAME_SERVER_INTERFACE(var,type,version) if ( (var = (type*)gameServerFactory(version,NULL)) == NULL ) { Warning("[RCBOT] Cannot open game server interface "#version " "#type " "#var "\n"); return false; } else { Msg("[RCBOT] Found interface "#version " "#type " "#var "\n"); }
#else
#define LOAD_INTERFACE(var,type,version) if ( (var = (type*)interfaceFactory(version,NULL)) == NULL ) { Warning("[RCBOT] Cannot open interface "## #version ##" "## #type ##" "## #var ##"\n"); return false; } else { Msg("[RCBOT] Found interface "## #version ##" "## #type ##" "## #var ## "\n"); }
#define LOAD_GAME_SERVER_INTERFACE(var,type,version) if ( (var = (type*)gameServerFactory(version,NULL)) == NULL ) { Warning("[RCBOT] Cannot open game server interface "## #version ##" "## #type ##" "## #var ##"\n"); return false; } else { Msg("[RCBOT] Found interface "## #version ##" "## #type ##" "## #var ## "\n"); }
#endif

//#define FILESYSTEM_INT FILESYSTEM_INTERFACE_VERSION
//#define FILESYSTEM_MAXVER 19

#ifndef __linux__

#define RCBot_LoadUndefinedInterface(var,type,vername,maxver,minver) { \
	int ver = maxver; \
	char str [256]; \
	char tempver[8]; \
	do{ \
		strcpy(str,vername); \
		sprintf(tempver,"%03d",ver); \
		strcat(str,tempver); \
		ver--; \
		var = (type*)interfaceFactory(str,NULL); \
		Msg("Trying... %s\n",str); \
	}while((var==NULL)&&(ver>minver)); \
    if ( var == NULL ) \
    { \
	Warning("[RCBOT] Cannot open interface "## #vername ##" "## #type ##" "## #var ##" (Max ver: "## #maxver ##") Min ver: ("## #minver ##") \n"); \
		return false; \
	} else { \
	Msg("[RCBOT] Found interface "## #vername ##" "## #type ##" "## #var ##", ver = %03d\n",ver+1); \
	}\
}

#define RCBot_LoadUndefinedGameInterface(var,type,vername,maxver,minver) { \
	int ver = maxver; \
	char str [256]; \
	char tempver[8]; \
	do{ \
		strcpy(str,vername); \
		sprintf(tempver,"%03d",ver); \
		strcat(str,tempver); \
		ver--; \
		var = (type*)gameServerFactory(str,NULL); \
		Msg("Trying... %s\n",str); \
	}while((var==NULL)&&(ver>minver)); \
    if ( var == NULL ) \
    { \
	Warning("[RCBOT] Cannot open interface "## #vername ##" "## #type ##" "## #var ##" (Max ver: "## #maxver ##") Min ver: ("## #minver ##") \n"); \
		return false; \
	} else { \
	Msg("[RCBOT] Found interface "## #vername ##" "## #type ##" "## #var ##", ver = %03d\n",ver+1); \
	}\
}
#else

// we need to change preprocessor concatonations for this

#define RCBot_LoadUndefinedInterface(var,type,vername,maxver,minver) { \
	int ver = maxver; \
	char str [256]; \
	char tempver[8]; \
	do{ \
		strcpy(str,vername); \
		sprintf(tempver,"%03d",ver); \
		strcat(str,tempver); \
		ver--; \
		var = (type*)interfaceFactory(str,NULL); \
		Msg("Trying... %s\n",str); \
	}while((var==NULL)&&(ver>minver)); \
    if ( var == NULL ) \
    { \
	Warning("[RCBOT] Cannot open interface " #vername " " #type " " #var " (Max ver: " #maxver ") Min ver: (" #minver ") \n"); \
		return false; \
	} else { \
	Msg("[RCBOT] Found interface " #vername " " #type " " #var ", ver = %03d\n",ver+1); \
	}\
}

//
#define RCBot_LoadUndefinedGameInterface(var,type,vername,maxver,minver) { \
	int ver = maxver; \
	char str [256]; \
	char tempver[8]; \
	do{ \
		strcpy(str,vername); \
		sprintf(tempver,"%03d",ver); \
		strcat(str,tempver); \
		ver--; \
		var = (type*)gameServerFactory(str,NULL); \
		Msg("Trying... %s\n",str); \
	}while((var==NULL)&&(ver>minver)); \
    if ( var == NULL ) \
    { \
	Warning("[RCBOT] Cannot open interface " #vername " " #type " " #var " (Max ver: " #maxver ") Min ver: (" #minver ") \n"); \
		return false; \
	} else { \
	Msg("[RCBOT] Found interface " #vername " " #type " " #var ", ver = %03d\n",ver+1); \
	}\
}

#endif

void unloadSignatures ()
{
	if ( g_pGetEconItemSchema )
		delete g_pGetEconItemSchema;

	if ( g_pSetRuntimeAttributeValue )
		delete g_pSetRuntimeAttributeValue;

	if ( g_pGetAttributeDefinitionByName )
		delete g_pGetAttributeDefinitionByName;

	if ( g_pAttribList_GetAttributeByID )
		delete g_pAttribList_GetAttributeByID;
}

void findSignaturesAndOffsets(void *gameServerFactory)
{
	char filename[512];
	// Load RCBOT2 hook data
	CBotGlobals::buildFileName(filename,"hookinfo",BOT_CONFIG_FOLDER,"ini");
	
	FILE *fp = fopen(filename,"r");
	
	CRCBotKeyValueList *pKVL = new CRCBotKeyValueList();

	if ( fp )
		pKVL->parseFile(fp);	

	g_pGetEconItemSchema = new CGetEconItemSchema(pKVL,gameServerFactory);
	g_pSetRuntimeAttributeValue = new CSetRuntimeAttributeValue(pKVL,gameServerFactory);
	g_pGetAttributeDefinitionByName = new CGetAttributeDefinitionByName(pKVL,gameServerFactory);
	g_pAttribList_GetAttributeByID = new CAttributeList_GetAttributeByID(pKVL,gameServerFactory);
		/*
#ifdef _WIN32
	if ( pKVL->getString("set_attribute_value_win",&sig) && sig )
#else
	if ( pKVL->getString("set_attribute_value_linux",&sig) && sig )
#endif
		SetRuntimeAttributeValue = findSignature((void*)gameServerFactory,sig);
	else
		SetRuntimeAttributeValue = findSignature((void*)gameServerFactory,"\\x55\\x8B\\xEC\\x83\\xEC\\x14\\x33\\xD2\\x53\\x8B\\xD9\\x56\\x57\\x8B\\x73\\x10\\x85\\xF6");

#ifdef _WIN32
	if ( pKVL->getString("get_item_schema_win",&sig) && sig )
#else
	if ( pKVL->getString("get_item_schema_linux",&sig) && sig )
#endif
		GEconItemSchemaFunc = findSignature((void*)gameServerFactory,sig);
	else
		GEconItemSchemaFunc = findSignature((void*)gameServerFactory,"\\xE8\\x2A\\x2A\\x2A\\x2A\\x83\\xC0\\x04\\xC3");

#ifdef _WIN32
	if ( pKVL->getString("get_attrib_definition_win",&sig) && sig )
#else
	if ( pKVL->getString("get_attrib_definition_linux",&sig) && sig )
#endif
		GetAttributeDefinitionByName = findSignature((void*)gameServerFactory,sig);
	else
		GetAttributeDefinitionByName = findSignature((void*)gameServerFactory,"\\x55\\x8B\\xEC\\x83\\xEC\\x1C\\x53\\x8B\\xD9\\x8B\\x0D\\x2A\\x2A\\x2A\\x2A\\x56\\x33\\xF6\\x89\\x5D\\xF8\\x89\\x75\\xE4\\x89\\x75\\xE8");

#ifdef _WIN32
	if ( pKVL->getString("attributelist_get_attrib_by_id_win",&sig) && sig )
#else
	if ( pKVL->getString("attributelist_get_attrib_by_id_linux",&sig) && sig )
#endif
		AttributeList_GetAttributeByID = findSignature((void*)gameServerFactory,sig);
	else
		AttributeList_GetAttributeByID = findSignature((void*)gameServerFactory,"\\x55\\x8B\\xEC\\x51\\x8B\\xC1\\x53\\x56\\x33\\xF6\\x89\\x45\\xFC\\x8B\\x58\\x10"); 
		*/
	int val;

#ifdef _WIN32

	if ( pKVL->getInt("givenameditem_win",&val) )
		rcbot_givenameditem_offset.SetValue(val);
	if ( pKVL->getInt("equipwearable_win",&val) )
		rcbot_equipwearable_offset.SetValue(val);
	if ( pKVL->getInt("runplayermove_tf2_win",&val) )
		rcbot_runplayercmd_tf2.SetValue(val);
	if ( pKVL->getInt("runplayermove_dods_win",&val) )
		rcbot_runplayercmd_dods.SetValue(val);
	
#else

	if ( pKVL->getInt("givenameditem_linux",&val) )
		rcbot_givenameditem_offset.SetValue(val);
	if ( pKVL->getInt("equipwearable_linux",&val) )
		rcbot_equipwearable_offset.SetValue(val);
	if ( pKVL->getInt("runplayermove_tf2_linux",&val) )
		rcbot_runplayercmd_tf2.SetValue(val);
	if ( pKVL->getInt("runplayermove_dods_linux",&val) )
		rcbot_runplayercmd_dods.SetValue(val);
	
#endif

	delete pKVL;
	fclose(fp);
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is loaded, load the interface we need from the engine
//---------------------------------------------------------------------------------
bool CRCBotPlugin::Load( CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
{
	extern MTRand_int32 irand;
	// TODO: check rcbot isn't already loaded already

	ConVar *rcbot_instance;

	ConnectTier1Libraries( &interfaceFactory, 1 );
	ConnectTier2Libraries( &interfaceFactory, 1 );

	rcbot_instance = cvar->FindVar("rcbot_ver");

	if ( rcbot_instance != NULL )
	{
		Msg("An instance of RCBOT is already running. Can't load!");
		DisconnectTier2Libraries( );
		DisconnectTier1Libraries( );
		return false;
	}

	findSignaturesAndOffsets((void*)gameServerFactory);

	LOAD_GAME_SERVER_INTERFACE(playerinfomanager,IPlayerInfoManager,INTERFACEVERSION_PLAYERINFOMANAGER);

	gpGlobals = playerinfomanager->GetGlobalVars();	

	LOAD_INTERFACE(engine,IVEngineServer,INTERFACEVERSION_VENGINESERVER);
	RCBot_LoadUndefinedInterface(filesystem,IFileSystem,"VFileSystem",22,1);

	if ( !CBotGlobals::gameStart() )
		return false;

	LOAD_INTERFACE(helpers,IServerPluginHelpers,INTERFACEVERSION_ISERVERPLUGINHELPERS);
	LOAD_INTERFACE(enginetrace,IEngineTrace,INTERFACEVERSION_ENGINETRACE_SERVER);
	LOAD_GAME_SERVER_INTERFACE(servergameents,IServerGameEnts,INTERFACEVERSION_SERVERGAMEENTS);
	LOAD_GAME_SERVER_INTERFACE(g_pEffects,IEffects,IEFFECTS_INTERFACE_VERSION);
	LOAD_GAME_SERVER_INTERFACE(g_pBotManager,IBotManager,INTERFACEVERSION_PLAYERBOTMANAGER);
	LOAD_GAME_SERVER_INTERFACE(servertools,IServerTools,VSERVERTOOLS_INTERFACE_VERSION);

#ifndef __linux__
    LOAD_INTERFACE(debugoverlay,IVDebugOverlay,VDEBUG_OVERLAY_INTERFACE_VERSION);
#endif
	LOAD_INTERFACE(gameeventmanager,IGameEventManager2,INTERFACEVERSION_GAMEEVENTSMANAGER2)
	LOAD_INTERFACE(gameeventmanager1,IGameEventManager,INTERFACEVERSION_GAMEEVENTSMANAGER)

	RCBot_LoadUndefinedGameInterface(servergamedll,IServerGameDLL,"ServerGameDLL",8,2);
	RCBot_LoadUndefinedGameInterface(gameclients,IServerGameClients,"ServerGameClients",4,1);

	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );
	ConVar_Register( 0 );
	//InitCVars( interfaceFactory ); // register any cvars we have defined

	srand( (unsigned)time(NULL) );  // initialize the random seed
	irand.seed( (unsigned)time(NULL) );

	eventListener2 = new CRCBotEventListener();

	// Initialize bot variables
	CBotProfiles::setupProfiles();


	//CBotEvents::setupEvents();
	CWaypointTypes::setup();
	CWaypoints::setupVisibility();

	CBotConfigFile::reset();	
	CBotConfigFile::load();

	CBotMenuList::setupMenus();

	CRCBotPlugin::ShowLicense();	

	RandomSeed((unsigned int)time(NULL));

	CClassInterface::init();

	RCBOT2_Cvar_setup();
	bInitialised = true;
	
	return true;
}

void CRCBotPlugin::ShowLicense ( void )
{
	
 Msg ("-----------------------------------------------------------------\n");
 Msg (" RCBOT LICENSE\n");
 Msg ("-----------------------------------------------------------------\n");
 Msg ("RCBot by Paul Murphy adapted from Botman's HPB Bot 2 template.\n\n");

 Msg ("RCBot is free software; you can redistribute it and/or modify it\n");
 Msg ("under the terms of the GNU General Public License as published by the\n");
 Msg ("Free Software Foundation; either version 2 of the License, or (at\n");
 Msg ("your option) any later version.\n\n");

 Msg ("RCBot is distributed in the hope that it will be useful, but\n");
 Msg ("WITHOUT ANY WARRANTY; without even the implied warranty of\n");
 Msg ("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
 Msg ("General Public License for more details.\n\n");

 Msg ("You should have received a copy of the GNU General Public License\n");
 Msg ("along with RCBot; if not, write to the Free Software Foundation,\n");
 Msg ("Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n\n");

 Msg ("In addition, as a special exception, the author gives permission to\n");
 Msg ("link the code of this program with the Half-Life Game Engine (\"HL\"\n");
 Msg ("Engine\") and Modified Game Libraries (\"MODs\") developed by Valve,\n");
 Msg ("L.L.C (\"Valve\").  You must obey the GNU General Public License in all\n");
 Msg ("respects for all of the code used other than the HL Engine and MODs\n");
 Msg ("from Valve.  If you modify this file, you may extend this exception\n");
 Msg ("to your version of the file, but you are not obligated to do so.  If\n");
 Msg ("you do not wish to do so, delete this exception statement from your\n");
 Msg ("version.\n");
 Msg ("-----------------------------------------------------------------\n");
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void CRCBotPlugin::Unload( void )
{
	// if another instance is running dont run through this
	if ( !bInitialised )
		return;
	
	CBots::freeAllMemory();
	CStrings::freeAllMemory();
	CBotGlobals::freeMemory();
	CBotMods::freeMemory();
	CAccessClients::freeMemory();
	CBotEvents::freeMemory();
	CWaypoints::freeMemory();
	CWaypointTypes::freeMemory();
	CBotProfiles::deleteProfiles();
	CWeapons::freeMemory();
	CBotMenuList::freeMemory();

	unloadSignatures();

	UnhookPlayerRunCommand();
	UnhookGiveNamedItem();

	//ConVar_Unregister();

	if ( gameeventmanager1 )
		gameeventmanager1->RemoveListener( this ); // make sure we are unloaded from the event system
	if ( gameeventmanager )
	{
		if ( eventListener2 )
		{
			gameeventmanager->RemoveListener( eventListener2 );
			delete eventListener2;
		}
	}

	// Reset Cheat Flag
	if ( puppet_bot_cmd != NULL )
	{
		if ( !puppet_bot_cmd->IsFlagSet(FCVAR_CHEAT) )
		{
			int *m_nFlags = (int*)((unsigned long)puppet_bot_cmd + BOT_CONVAR_FLAGS_OFFSET); // 20 is offset to flags
			
			*m_nFlags |= FCVAR_CHEAT;
		}
	}

	ConVar_Unregister( );
		
	DisconnectTier2Libraries( );
	DisconnectTier1Libraries( );
	
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is paused (i.e should stop running but isn't unloaded)
//---------------------------------------------------------------------------------
void CRCBotPlugin::Pause( void )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unpaused (i.e should start executing again)
//---------------------------------------------------------------------------------
void CRCBotPlugin::UnPause( void )
{
}

//---------------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//---------------------------------------------------------------------------------
const char *CRCBotPlugin::GetPluginDescription( void )
{
	return "RCBot2 Plugin, by Cheeseh";
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CRCBotPlugin::LevelInit( char const *pMapName )
{
	//CClients::initall();
	// Must set this
	CBotGlobals::setMapName(pMapName);

	Msg( "Level \"%s\" has been loaded\n", pMapName );

	CWaypoints::precacheWaypointTexture();

	CWaypointDistances::reset();

	CProfileTimers::reset();

	CWaypoints::init();
	CWaypoints::load();

	CBotGlobals::setMapRunning(true);
	CBotConfigFile::reset();
	
	if ( mp_teamplay )
		CBotGlobals::setTeamplay(mp_teamplay->GetBool());
	else
		CBotGlobals::setTeamplay(false);

	gameeventmanager1->AddListener( this, true );	

	CBotEvents::setupEvents();

	CBots::mapInit();

	CBotMod *pMod = CBotGlobals::getCurrentMod();
	
	if ( pMod )
		pMod->mapInit();

	CBotSquads::FreeMemory();

	CClients::setListenServerClient(NULL);
}

//---------------------------------------------------------------------------------
// Purpose: called on level start, when the server is ready to accept client connections
//		edictCount is the number of entities in the level, clientMax is the max client count
//---------------------------------------------------------------------------------
void CRCBotPlugin::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	Msg( "clientMax is %d\n", clientMax );

	CAccessClients::load();

	CBotGlobals::setClientMax(clientMax);
}

void CRCBotPlugin::PreClientUpdate(bool simulating)
{
	//if ( simulating && CBotGlobals::IsMapRunning() )
	//{
	//	CBots::runPlayerMoveAll();
	//}
}

//---------------------------------------------------------------------------------
// Purpose: called once per server frame, do recurring work here (like checking for timeouts)
//---------------------------------------------------------------------------------
void CRCBotPlugin::GameFrame( bool simulating )
{
	static CBotMod *currentmod;

	if ( simulating && CBotGlobals::IsMapRunning() )
	{
		CBots::botThink();
		if ( !CBots::controlBots() )
			gameclients->PostClientMessagesSent();
		CBots::handleAutomaticControl();
		CClients::clientThink();

		if ( CWaypoints::getVisiblity()->needToWorkVisibility() )
		{
			CWaypoints::getVisiblity()->workVisibility();
		}

		// Profiling
#ifdef _DEBUG
		if ( CClients::clientsDebugging(BOT_DEBUG_PROFILE) )
		{
			CProfileTimers::updateAndDisplay();
		}
#endif

		// Config Commands
		CBotConfigFile::doNextCommand();
		currentmod = CBotGlobals::getCurrentMod();

		currentmod->modFrame();
	}
}

//---------------------------------------------------------------------------------
// Purpose: called on level end (as the server is shutting down or going to a new map)
//---------------------------------------------------------------------------------
void CRCBotPlugin::LevelShutdown( void ) // !!!!this can get called multiple times per map change
{
	CClients::initall();
	CWaypointDistances::save();

	//if ( !rcbot_runplayercmd_hookonce.GetBool() )
	//{
		UnhookPlayerRunCommand();
		UnhookGiveNamedItem();
	//}

	CBots::freeMapMemory();	
	CWaypoints::init();

	CBotGlobals::setMapRunning(false);
	CBotEvents::freeMemory();

	gameeventmanager1->RemoveListener( this );

	if ( gameeventmanager )
		gameeventmanager->RemoveListener( eventListener2 );
}

//---------------------------------------------------------------------------------
// Purpose: called when a client spawns into a server (i.e as they begin to play)
//---------------------------------------------------------------------------------
void CRCBotPlugin::ClientActive( edict_t *pEntity )
{
	CClients::clientActive(pEntity);
}

//---------------------------------------------------------------------------------
// Purpose: called when a client leaves a server (or is timed out)
//---------------------------------------------------------------------------------
void CRCBotPlugin::ClientDisconnect( edict_t *pEntity )
{
	// this is sorted in CClients::clientDisconnected()
	//CBot *pBot = CBots::getBotPointer(pEntity);
	//pBot->freeMapMemory();

	CClients::clientDisconnected(pEntity);
}

//---------------------------------------------------------------------------------
// Purpose: called on client being added to this server
//---------------------------------------------------------------------------------
void CRCBotPlugin::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	bool is_Rcbot = false;

	CClient *pClient = CClients::clientConnected(pEntity);

	if ( CBots::controlBots() )
		is_Rcbot = CBots::handlePlayerJoin(pEntity,playername);
	
	if ( !is_Rcbot && pClient )
	{	
		if ( !engine->IsDedicatedServer() )
		{
			if ( CClients::noListenServerClient() )
			{
				// give listenserver client all access to bot commands
				CClients::setListenServerClient(pClient);		
				pClient->setAccessLevel(CMD_ACCESS_ALL);
				pClient->resetMenuCommands();
			}
		}
	}

	CBotMod *pMod = CBotGlobals::getCurrentMod();

	pMod->playerSpawned(pEntity);

}

CRCBotEventListener *CRCBotPlugin:: getEventListener ( void )
{
	return eventListener2;
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CRCBotPlugin::SetCommandClient( int index )
{
	m_iClientCommandIndex = index;
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CRCBotPlugin::ClientSettingsChanged( edict_t *pEdict )
{
	/*
	if ( playerinfomanager )
	{
		IPlayerInfo *playerinfo = playerinfomanager->GetPlayerInfo( pEdict );
		
		const char *name = engine->GetClientConVarValue( engine->IndexOfEdict(pEdict), "name" );

		if ( playerinfo && name && playerinfo->GetName() && 
			 Q_stricmp( name, playerinfo->GetName()) ) // playerinfo may be NULL if the MOD doesn't support access to player data 
													   // OR if you are accessing the player before they are fully connected
		{
			char msg[128];
			Q_snprintf( msg, sizeof(msg), "Your name changed to \"%s\" (from \"%s\"\n", name, playerinfo->GetName() ); 
			engine->ClientPrintf( pEdict, msg ); // this is the bad way to check this, the better option it to listen for the "player_changename" event in FireGameEvent()
												// this is here to give a real example of how to use the playerinfo interface
		}
	}*/
}

//---------------------------------------------------------------------------------
// Purpose: called when a client joins a server
//---------------------------------------------------------------------------------
PLUGIN_RESULT CRCBotPlugin::ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	CClients::init(pEntity);

	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client types in a command (only a subset of commands however, not CON_COMMAND's)
//---------------------------------------------------------------------------------
PLUGIN_RESULT CRCBotPlugin::ClientCommand( edict_t *pEntity, const CCommand &args )
{
	const char *pcmd = args.Arg(0);
	CBotMod *pMod;
	//const char *pcmd = engine->Cmd_Argv(0);

	if ( !pEntity || pEntity->IsFree() ) 
	{
		return PLUGIN_CONTINUE;
	}

	CClient *pClient = CClients::get(pEntity);

	// is bot command?
	if ( CBotGlobals::m_pCommands->isCommand(pcmd) )
	{		
		//eBotCommandResult iResult = CBotGlobals::m_pCommands->execute(pClient,engine->Cmd_Argv(1),engine->Cmd_Argv(2),engine->Cmd_Argv(3),engine->Cmd_Argv(4),engine->Cmd_Argv(5),engine->Cmd_Argv(6));
		eBotCommandResult iResult = CBotGlobals::m_pCommands->execute(pClient,args.Arg(1),args.Arg(2),args.Arg(3),args.Arg(4),args.Arg(5),args.Arg(6));

		if ( iResult == COMMAND_ACCESSED )
		{
			// ok
		}
		else if ( iResult == COMMAND_REQUIRE_ACCESS )
		{
			CBotGlobals::botMessage(pEntity,0,"You do not have access to this command");
		}
		else if ( iResult == COMMAND_NOT_FOUND )
		{
			CBotGlobals::botMessage(pEntity,0,"bot command not found");	
		}
		else if ( iResult == COMMAND_ERROR )
		{
			CBotGlobals::botMessage(pEntity,0,"bot command returned an error");	
		}

		return PLUGIN_STOP; // we handled this function
	}
	else if ( strncmp(pcmd,"menuselect",10) == 0 ) // menu command
	{
		if ( pClient->isUsingMenu() )
		{
			int iCommand = atoi(args.Arg(1));

			// format is 1.2.3.4.5.6.7.8.9.0
			if ( iCommand == 0 )
				iCommand = 9;
			else
				iCommand --;

			pClient->getCurrentMenu()->selectedMenu(pClient,iCommand);
		}
	}

	// command capturing
	pMod = CBotGlobals::getCurrentMod();

	// capture some client commands e.g. voice commands
	pMod->clientCommand(pEntity,args.ArgC(),pcmd,args.Arg(1),args.Arg(2));

	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client is authenticated
//---------------------------------------------------------------------------------
PLUGIN_RESULT CRCBotPlugin::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	return PLUGIN_CONTINUE;
}

void CRCBotPlugin::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{
	return;
}

// Data types from Keyvalues.h
enum types_t
{
	TYPE_NONE = 0,
	TYPE_STRING,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_PTR,
	TYPE_WSTRING,
	TYPE_COLOR,
	TYPE_UINT64,
	TYPE_NUMTYPES, 
};

//---------------------------------------------------------------------------------
// Purpose: called when an event is fired
//---------------------------------------------------------------------------------
void CRCBotPlugin::FireGameEvent( KeyValues * pevent )
{
	static char szKey[128];
	static char szValue[128];

	if ( CBotGlobals :: isEventVersion (1) )
	{
		const char *type = pevent->GetName();

		CBotEvents::executeEvent((void*)pevent,TYPE_KEYVALUES);	

		if ( CClients::clientsDebugging(BOT_DEBUG_GAME_EVENT) )
		{
			KeyValues *pk = pevent->GetFirstTrueSubKey();

			CClients::clientDebugMsg(NULL,BOT_DEBUG_GAME_EVENT,"[BEGIN \"%s\"]",type);

			/*while ( pk != NULL )
			{
				CClients::clientDebugMsg(NULL,BOT_DEBUG_GAME_EVENT,"BEGIN %s",pk->GetName());
				pk = pk->GetNextTrueSubKey();
			}*/

			pk = pevent->GetFirstValue();

//For some reason KeyValues CRASHES when receiving the GetString of a non string
//so we have to be careful below:

			while ( pk != NULL )
			{
				strncpy(szKey,pk->GetName(),127);
				szKey[127] = 0;

				switch ( pk->GetDataType() )
				{
		case TYPE_FLOAT:
					sprintf(szValue,"%0.2f",pk->GetFloat());
					break;
		case TYPE_INT:
					sprintf(szValue,"%d",pk->GetInt());
					break;
		case TYPE_PTR:
					sprintf(szValue,"%x",(int)pk->GetPtr());
					break;
		case TYPE_UINT64:
					sprintf(szValue,"%d",pk->GetUint64());
					break;
		case TYPE_WSTRING:
					strcpy(szValue,"WSTRING");
					break;
		case TYPE_STRING:
					strncpy(szValue,pk->GetString(),127);
					break;
		default:
				strcpy(szValue,"NULL");
				break;
				}
				
				szValue[127] = 0;

				CClients::clientDebugMsg(NULL,BOT_DEBUG_GAME_EVENT,"\t%s = %s",szKey,szValue);
				pk = pk->GetNextValue();
			}

			CClients::clientDebugMsg(NULL,BOT_DEBUG_GAME_EVENT,"[END \"%s\"]",type);

/*
			for ( KeyValues *pk = pevent->GetFirstTrueSubKey(); pk; pk = pk->GetNextTrueSubKey())
			{
				CClients::clientDebugMsg(BOT_DEBUG_GAME_EVENT,pk->GetName());
			}
			for ( KeyValues *pk = pevent->GetFirstValue(); pk; pk = pk->GetNextValue() )
			{		
				char szMsg[512];

				sprintf(szMsg,"%s = %s",pk->GetName(),pk->GetString());
				CClients::clientDebugMsg(BOT_DEBUG_GAME_EVENT,szMsg);
			}*/
		}
	}
}

void CRCBotEventListener::FireGameEvent( IGameEvent * pevent )
{
	if ( CBotGlobals :: isEventVersion (2) )
	{
		CBotEvents::executeEvent((void*)pevent,TYPE_IGAMEEVENT);
	}
}



///////////////////
//useful functions
// defined in const more efficiently
/*int round ( float f )
{
	f = f-(float)((int)f);

	if ( f >= 0.5 )
		return (int)f+1;
	
	return (int)f;
}*/
//defined in const more efficiently
/*int RANDOM_INT(int min, int max)
{    
    return min + round(((float)rand()/RAND_MAX)*(float)(max-min));
}*/

//////////////////////


/*edict_t* INDEXENT( int iEdictNum )		
{ 
	return engine->PEntityOfEntIndex(iEdictNum); 
}

#ifndef GAME_DLL
// get entity index
int ENTINDEX( edict_t *pEdict )		
{ 
	return engine->IndexOfEdict(pEdict);
}
#endif*/

int Ceiling ( float fVal )
{
	int loVal = (int)fVal;

	fVal -= (float)loVal;

	if ( fVal == 0.0 )
		return loVal;
	
	return loVal+1;
}

float VectorDistance(Vector &vec)
{
	return (float)sqrt(((vec.x*vec.x) + (vec.y*vec.y) + (vec.z*vec.z)));
}

// Testing the three types of "entity" for nullity
bool FNullEnt(const edict_t* pent)
{ 
	return pent == NULL || ENTINDEX((edict_t*)pent) == 0; 
}
