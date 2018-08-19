/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Sample Plugin
 * Written by AlliedModders LLC.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 *
 * This sample plugin is public domain.
 */

#include <stdio.h>

#include "bot_plugin_meta.h"

#include "filesystem.h"
#include "interface.h"

#ifdef __linux__
#include "shake.h"    //bir3yk
#endif
#include "IEffects.h"
#include "igameevents.h"
#include "IEngineTrace.h"

#include "Color.h"
#include "ndebugoverlay.h"
#include "server_class.h"
#include "time.h"
#include "irecipientfilter.h"

#include "KeyValues.h"

#include "bot_cvars.h"

// for IServerTools
#include "bot.h"
#include "bot_configfile.h"
#include "bot_globals.h"
#include "bot_profile.h"
#include "bot_waypoint.h"
#include "bot_menu.h"
#include "bot_getprop.h"
#include "bot_fortress.h"
#include "bot_event.h"
#include "bot_profiling.h"
#include "bot_wpt_dist.h"
#include "bot_squads.h"
#include "bot_accessclient.h"
#include "bot_weapons.h"
#include "bot_waypoint_visibility.h"
#include "bot_kv.h"
#include "bot_sigscan.h"

//#include "ndebugoverlay.h"
CBotTF2 *g_pLastBot;

SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, 0, bool, char const *, char const *, char const *, char const *, bool, bool);
SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int);
SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool);
SH_DECL_HOOK0_void(IServerGameDLL, LevelShutdown, SH_NOATTRIB, 0);
SH_DECL_HOOK2_void(IServerGameClients, ClientActive, SH_NOATTRIB, 0, edict_t *, bool);
SH_DECL_HOOK1_void(IServerGameClients, ClientDisconnect, SH_NOATTRIB, 0, edict_t *);
SH_DECL_HOOK2_void(IServerGameClients, ClientPutInServer, SH_NOATTRIB, 0, edict_t *, char const *);
SH_DECL_HOOK1_void(IServerGameClients, SetCommandClient, SH_NOATTRIB, 0, int);
SH_DECL_HOOK1_void(IServerGameClients, ClientSettingsChanged, SH_NOATTRIB, 0, edict_t *);
SH_DECL_HOOK5(IServerGameClients, ClientConnect, SH_NOATTRIB, 0, bool, edict_t *, const char*, const char *, char *, int);
SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);

#if SOURCE_ENGINE >= SE_ORANGEBOX
SH_DECL_HOOK2_void(IServerGameClients, NetworkIDValidated, SH_NOATTRIB, 0, const char *, const char *);
SH_DECL_HOOK2_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, edict_t *, const CCommand &);
#else
SH_DECL_HOOK1_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, edict_t *);
#endif

SH_DECL_MANUALHOOK2_void(MHook_PlayerRunCmd, 0, 0, 0, CUserCmd*, IMoveHelper*); 
SH_DECL_MANUALHOOK4(MHook_GiveNamedItem, 0, 0, 0,CBaseEntity*, const char *,int,CEconItemView*,bool); 

SH_DECL_MANUALHOOK1_void(MHook_EquipWearable, 0, 0, 0, CEconWearable*);
SH_DECL_MANUALHOOK1_void(MHook_EquipWeapon, 0, 0, 0, CBaseEntity*);

SH_DECL_MANUALHOOK1_void(MHook_RemovePlayerItem, 0, 0, 0, CBaseEntity*);

SH_DECL_MANUALHOOK1(MHook_GetPlayerWeaponSlot, 0, 0, 0, CBaseEntity*, int);
SH_DECL_MANUALHOOK1_void(MHook_RemoveWearable, 0, 0, 0, CBaseEntity*);

/*
SH_DECL_HOOK1_void(bf_write, WriteChar, SH_NOATTRIB, 0, int);
SH_DECL_HOOK1_void(bf_write, WriteShort, SH_NOATTRIB, 0, int);
SH_DECL_HOOK1_void(bf_write, WriteByte, SH_NOATTRIB, 0, int);
SH_DECL_HOOK1_void(bf_write, WriteFloat, SH_NOATTRIB, 0, float);
SH_DECL_HOOK1(bf_write, WriteString, SH_NOATTRIB, 0, bool, const char *);

SH_DECL_HOOK2(IVEngineServer, UserMessageBegin, SH_NOATTRIB, 0, bf_write*, IRecipientFilter*, int);
SH_DECL_HOOK0_void(IVEngineServer, MessageEnd, SH_NOATTRIB, 0);

bf_write *current_msg = NULL;

#define BUF_SIZ 1024
char current_msg_buffer[BUF_SIZ];
*/

CBaseEntity* (CBaseEntity::*TF2EquipWearable)(CBaseEntity*) = 0x0;
CBaseEntity* (CBaseEntity::*TF2PlayerWeaponSlot)(int) = 0x0;
void (CAttributeManager::*OnAttributeValuesChanged)(void) = 0x0;
void (CBaseEntity::*TF2RemoveWearable)(CBaseEntity*) = 0x0;
void (CBaseEntity::*TF2RemovePlayerItem)(CBaseEntity*) = 0x0;
//void (CBaseEntity::*TF2WeaponEquip)(CBaseEntity*) = 0x0;

IServerGameDLL *server = NULL;
IGameEventManager2 *gameevents = NULL;
IServerPluginCallbacks *vsp_callbacks = NULL;
ICvar *icvar = NULL;
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

RCBotPluginMeta g_RCBotPluginMeta;

PLUGIN_EXPOSE(RCBotPluginMeta, g_RCBotPluginMeta);

static ConVar rcbot2_ver_cvar(BOT_VER_CVAR, BOT_VER, FCVAR_REPLICATED, BOT_NAME_VER);


int UTIL_ListAttributesOnEntity(edict_t *pEdict)
{
	CAttributeList *pAttributeList = CClassInterface::getAttributeList(pEdict);
	int offset = CClassInterface::getOffset(GETPROP_TF2_ATTRIBUTELIST);
	CBaseEntity *pEntity = servergameents->EdictToBaseEntity(pEdict);

	if (!pAttributeList)
		return 0;

	//local variable is initialized but not referenced - [APG]RoboCop[CL]
	int *pAttribList1 = (int*)((unsigned int)pAttributeList + 4);

	int *pAttribList = (int*)((unsigned int)pEntity + offset + 4);

	if ((unsigned int)pAttribList < 0x10000)
		return 0;

	int iNumAttribs = *(int*)((unsigned int)pAttributeList + 16);
	short int iAttribIndices[16];

	CBotGlobals::botMessage(NULL, 0, "There are %d attributes on %s entity", iNumAttribs, pEdict->GetClassName());

	for (int i = 0; i < iNumAttribs; i++)	//THIS IS HOW YOU GET THE ATTRIBUTES ON AN ITEM!
	{
		iAttribIndices[i] = *(short int*)((unsigned int)pAttribList + (i * 16) + 4);

		CBotGlobals::botMessage(NULL, 0, "%d) %d", i, iAttribIndices[i]);
	}

	return iNumAttribs;
}

CON_COMMAND(rcbot_printattribs, "print attributes")
{
	if (args.ArgC() > 1)
	{
		int slot = atoi(args.Arg(1));

		edict_t *pEdict = INDEXENT(1);

		if (slot >= 0)
		{

			CBaseEntity *pEntity = RCBotPluginMeta::TF2_getPlayerWeaponSlot(pEdict, slot);

			if (pEntity)
				pEdict = servergameents->BaseEntityToEdict(pEntity);
		}

		if (pEdict)
			UTIL_ListAttributesOnEntity(pEdict);
	}
}

CON_COMMAND(rcbot_setattrib, "set an attribute")
{
	if (args.ArgC() > 2)
	{		
		edict_t *pPlayer = CClients::getListenServerClient();

		CBaseEntity *pEntity = RCBotPluginMeta::TF2_getPlayerWeaponSlot(pPlayer, TF2_SLOT_PRMRY);

		if (pEntity != NULL)
		{
			edict_t *pEdict = servergameents->BaseEntityToEdict(pEntity);

			if (pEdict && !pEdict->IsFree())
			{			
				const char *strAttrib = args.Arg(1);
				float flVal = atof(args.Arg(2));
				//void (edict_t *pEdict, const char *szName, float flVal)
				if (TF2_setAttribute(pEdict, strAttrib, flVal))
					CBotGlobals::botMessage(NULL, 0, "OK");
				else
					CBotGlobals::botMessage(NULL, 0, "FAIL");

				RCBotPluginMeta::TF2_ClearAttributeCache(pPlayer);
			}
		}
	}
}

CON_COMMAND(rcbotd, "access the bot commands on a server")
{
	eBotCommandResult iResult;

	if (!engine->IsDedicatedServer() || !CBotGlobals::IsMapRunning())
	{
		CBotGlobals::botMessage(NULL, 0, "Error, no map running or not dedicated server");
		return;
	}

	//iResult = CBotGlobals::m_pCommands->execute(NULL,engine->Cmd_Argv(1),engine->Cmd_Argv(2),engine->Cmd_Argv(3),engine->Cmd_Argv(4),engine->Cmd_Argv(5),engine->Cmd_Argv(6));
	iResult = CBotGlobals::m_pCommands->execute(NULL, args.Arg(1), args.Arg(2), args.Arg(3), args.Arg(4), args.Arg(5), args.Arg(6));

	if (iResult == COMMAND_ACCESSED)
	{
		// ok
	}
	else if (iResult == COMMAND_REQUIRE_ACCESS)
	{
		CBotGlobals::botMessage(NULL, 0, "You do not have access to this command");
	}
	else if (iResult == COMMAND_NOT_FOUND)
	{
		CBotGlobals::botMessage(NULL, 0, "bot command not found");
	}
	else if (iResult == COMMAND_ERROR)
	{
		CBotGlobals::botMessage(NULL, 0, "bot command returned an error");
	}
}

/*
bool RCBotPluginMeta :: ClearAttributeCache(edict_t *pedict)
{
	if (hSDKOnAttribValuesChanged == INVALID_HANDLE) return false;

	if (pedict == NULL || pedict->IsFree() ) 
		return false;

	new offs = GetEntSendPropOffs(entity, "m_AttributeList", true);
	if (offs <= 0) return false;
	new Address:pAttribs = GetEntityAddress(entity);
	if (pAttribs < Address_MinimumValid) return false;
	pAttribs = Address:LoadFromAddress(pAttribs + Address:(offs + 24), NumberType_Int32);
	if (pAttribs < Address_MinimumValid) return false;
	SDKCall(hSDKOnAttribValuesChanged, pAttribs);
	return true;
}*/


CBaseEntity *RCBotPluginMeta::TF2_getPlayerWeaponSlot(edict_t *pPlayer, int iSlot)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pPlayer);

	return SH_MCALL(pEnt, MHook_GetPlayerWeaponSlot)(iSlot);
}

void RCBotPluginMeta::TF2_removeWearable(edict_t *pPlayer, CBaseEntity *pWearable)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pPlayer);

	SH_MCALL(pEnt, MHook_RemoveWearable)(pWearable);
}


void RCBotPluginMeta::TF2_equipWearable(edict_t *pPlayer, CBaseEntity *pWearable)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pPlayer);

	SH_MCALL(pEnt, MHook_EquipWearable)((CEconWearable*)pWearable);
}
/*			
"CAttributeManager::OnAttributeValuesChanged"	//use instead of ClearCache/NotifyManagerOfAttributeValueChanges
{
	"windows"	"12"
	"linux"		"13"
	"mac"		"13"
}
*/
bool RCBotPluginMeta::TF2_ClearAttributeCache(edict_t *pEdict)
{
	CAttributeList *pList = CClassInterface::getAttributeList(pEdict);

	CAttributeManager *pManager = (CAttributeManager*)(((unsigned int)pList) + 24);

	if (!pManager)
		return false;

	unsigned int *mem = (unsigned int*)*(unsigned int*)pManager;

	if (!mem)
		return false;

	int offset = 12;
	
	*(unsigned int*)&OnAttributeValuesChanged = mem[offset];

	if (!OnAttributeValuesChanged)
		return false;

	(*pManager.*OnAttributeValuesChanged)();

	return true;
}
void RCBotPluginMeta::TF2_equipWeapon(edict_t *pPlayer, CBaseEntity *pWeapon)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pPlayer);

	SH_MCALL(pEnt, MHook_EquipWeapon)(pWeapon);
}

void RCBotPluginMeta::TF2_removePlayerItem(edict_t *pPlayer, CBaseEntity *pItem)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pPlayer);

	SH_MCALL(pEnt, MHook_RemovePlayerItem)(pItem);
}
class CBotRecipientFilter : public IRecipientFilter
{
public:
	CBotRecipientFilter(edict_t *pPlayer)
	{
		m_iPlayerSlot = ENTINDEX(pPlayer);
	}

	bool IsReliable(void) const { return false; }
	bool IsInitMessage(void) const { return false; }

	int	GetRecipientCount(void) const { return 1; }
	int	GetRecipientIndex(int slot) const { return m_iPlayerSlot; }

private:
	int m_iPlayerSlot;
};

class CClientBroadcastRecipientFilter : public IRecipientFilter
{
public:

	CClientBroadcastRecipientFilter() {
		m_iMaxCount = 0;

		for (int i = 0; i < MAX_PLAYERS; ++i) {
			CClient* client = CClients::get(i);

			if (client->isUsed()) {
				IPlayerInfo *p = playerinfomanager->GetPlayerInfo(client->getPlayer());

				if (p->IsConnected() && !p->IsFakeClient()) {
					m_iPlayerSlot[m_iMaxCount] = i;
					m_iMaxCount++;
				}
			}
		}
	}

	bool IsReliable(void) const { return false; }
	bool IsInitMessage(void) const { return false; }

	int	GetRecipientCount(void) const { return m_iMaxCount; }
	int	GetRecipientIndex(int slot) const { return m_iPlayerSlot[slot] + 1; }

private:

	int m_iMaxCount;
	int m_iPlayerSlot[MAX_PLAYERS];
};

///////////////
// hud message
///////////////
void RCBotPluginMeta::HudTextMessage(edict_t *pEntity, const char *szMessage)
{
	int msgid = 0;
	int imsgsize = 0;
	char msgbuf[64];
	bool bOK;

	int hint = -1;
	int say = -1;

	while ((bOK = servergamedll->GetUserMessageInfo(msgid, msgbuf, 63, imsgsize)) == true)
	{
		if (strcmp(msgbuf, "HintText") == 0)
			hint = msgid;
		else if (strcmp(msgbuf, "SayText") == 0)
			say = msgid;

		msgid++;
	}

	if (msgid == 0)
		return;

	// if (!bOK)
	// return;

	CBotRecipientFilter *filter = new CBotRecipientFilter(pEntity);

	bf_write *buf = nullptr;

	if (hint > 0) {
		buf = engine->UserMessageBegin(filter, hint);
		buf->WriteString(szMessage);
		engine->MessageEnd();
	}

	if (say > 0) {
		char chatline[128];
		snprintf(chatline, sizeof(chatline), "\x01\x04[RCBot2]\x01 %s\n", szMessage);

		buf = engine->UserMessageBegin(filter, say);
		buf->WriteString(chatline);
		engine->MessageEnd();
	}

	delete filter;
}

//////////////////////////
// chat broadcast message
//////////////////////////
void RCBotPluginMeta::BroadcastTextMessage(const char *szMessage)
{
	int msgid = 0;
	int imsgsize = 0;
	char msgbuf[64];
	bool bOK;

	int hint = -1;
	int say = -1;

	while ((bOK = servergamedll->GetUserMessageInfo(msgid, msgbuf, 63, imsgsize)) == true)
	{
		if (strcmp(msgbuf, "HintText") == 0)
			hint = msgid;
		else if (strcmp(msgbuf, "SayText") == 0)
			say = msgid;

		msgid++;
	}

	if (msgid == 0)
		return;

	CClientBroadcastRecipientFilter *filter = new CClientBroadcastRecipientFilter();

	bf_write *buf = nullptr;

	if (say > 0) {
		char chatline[128];
		snprintf(chatline, sizeof(chatline), "\x01\x04[RCBot2]\x01 %s\n", szMessage);

		buf = engine->UserMessageBegin(filter, say);
		buf->WriteString(chatline);
		engine->MessageEnd();
	}

	delete filter;
}

void RCBotPluginMeta::TF2_RemoveWeaponSlot(edict_t *pPlayer, int iSlot)
{
	CBaseEntity *pWeaponInSlot = RCBotPluginMeta::TF2_getPlayerWeaponSlot(pPlayer, iSlot);

	if (pWeaponInSlot)
	{
		// bug #6206
		// papering over a valve bug where a weapon's extra wearables aren't properly removed from the weapon's owner
		edict_t *extraWearable = CClassInterface::getExtraWearable(pPlayer);

		if (extraWearable != NULL)
		{
			CBaseEntity *pEnt = servergameents->EdictToBaseEntity(extraWearable);
			RCBotPluginMeta::TF2_removeWearable(pPlayer, pEnt);
			engine->RemoveEdict(extraWearable);
		}

		extraWearable = CClassInterface::getExtraWearableViewModel(pPlayer);

		if (extraWearable != NULL)
		{
			CBaseEntity *pEnt = servergameents->EdictToBaseEntity(extraWearable);
			RCBotPluginMeta::TF2_removeWearable(pPlayer, pEnt);
			engine->RemoveEdict(extraWearable);
		}

		RCBotPluginMeta::TF2_removePlayerItem(pPlayer, pWeaponInSlot);

		edict_t *pWeaponInSlotEdict = servergameents->BaseEntityToEdict(pWeaponInSlot);
		//AcceptEntityInput(weaponIndex, "Kill");
		engine->RemoveEdict(pWeaponInSlotEdict);
	}
}

void RCBotPluginMeta::giveRandomLoadout(edict_t *pPlayer, int iClass, int iSlot, void *pVTable, void *pVTable_Attributes)
{
	CTF2Loadout *p = CTeamFortress2Mod::findRandomWeaponLoadOutInSlot(iClass, iSlot);

	if (p)
	{
		RCBotPluginMeta::givePlayerLoadOut(pPlayer, p, iSlot, pVTable, pVTable_Attributes);
	}
}

// TF2 Items
bool RCBotPluginMeta::givePlayerLoadOut(edict_t *pPlayer, CTF2Loadout *pLoadout, int iSlot, void *pVTable, void *pVTable_Attributes)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pPlayer);
	// first remove any thing from the slot
	RCBotPluginMeta::TF2_RemoveWeaponSlot(pPlayer, iSlot);

	CEconItemView hScriptCreatedItem;
	memset(&hScriptCreatedItem, 0, sizeof(CEconItemView));

	hScriptCreatedItem.m_pVTable = pVTable;
	hScriptCreatedItem.m_AttributeList.m_pVTable = pVTable_Attributes;
	hScriptCreatedItem.m_NetworkedDynamicAttributesForDemos.m_pVTable = pVTable_Attributes;

	const char *strWeaponClassname = pLoadout->m_pszClassname;
	hScriptCreatedItem.m_iItemDefinitionIndex = pLoadout->m_iIndex;
	hScriptCreatedItem.m_iEntityLevel = randomInt(pLoadout->m_iMinLevel, pLoadout->m_iMaxLevel);
	hScriptCreatedItem.m_iEntityQuality = pLoadout->m_iQuality;
	CEconItemAttribute attribs[16];
	int iSize = pLoadout->copyAttributesIntoArray(attribs, pVTable);
	hScriptCreatedItem.m_AttributeList.m_Attributes.CopyArray(attribs, iSize);// pLoadout->m_Attributes, pLoadout->m_Attributes.size());
	hScriptCreatedItem.m_bInitialized = true;
	hScriptCreatedItem.m_bDoNotIterateStaticAttributes = false; // true breaks pyro bot flamethrowers

	if (hScriptCreatedItem.m_iEntityQuality == 0 && iSize > 0)
	{
		hScriptCreatedItem.m_iEntityQuality = 6;
	}

	CBaseEntity *added = SH_MCALL(pEnt, MHook_GiveNamedItem)(strWeaponClassname, 0, &hScriptCreatedItem, rcbot_force_generation.GetBool());

	if (added)
	{
		if ((iSlot == TF2_SLOT_MELEE) || (iSlot == TF2_SLOT_PRMRY) || (iSlot == TF2_SLOT_SCNDR))
			RCBotPluginMeta::TF2_equipWeapon(pPlayer, added);
		else
			RCBotPluginMeta::TF2_equipWearable(pPlayer, added);
	}

	return added != NULL;
}

void RCBotPluginMeta::Hook_PlayerRunCmd(CUserCmd *ucmd, IMoveHelper *moveHelper)
{
	static CBot *pBot;

	CBaseEntity *pPlayer = META_IFACEPTR(CBaseEntity);

	edict_t *pEdict = servergameents->BaseEntityToEdict(pPlayer);

	pBot = CBots::getBotPointer(pEdict);
	
	if ( pBot )
	{
		static CBotCmd *cmd;
		
		cmd = pBot->getUserCMD();

		// put the bot's commands into this move frame
		ucmd->buttons = cmd->buttons;
		ucmd->forwardmove = cmd->forwardmove;
		ucmd->impulse = cmd->impulse;
		ucmd->sidemove = cmd->sidemove;
		ucmd->upmove = cmd->upmove;
		ucmd->viewangles = cmd->viewangles;
		ucmd->weaponselect = cmd->weaponselect;
		ucmd->weaponsubtype = cmd->weaponsubtype;
		ucmd->tick_count = cmd->tick_count;
		ucmd->command_number = cmd->command_number;

		g_pLastBot = (CBotTF2*)pBot;
	}

//g_pSM->LogMessage(NULL, "H %i %i %f %f %f %f %i", ucmd->command_number, ucmd->tick_count, ucmd->viewangles.x, ucmd->viewangles.y, ucmd->viewangles.z, ucmd->forwardmove, ucmd->buttons); 

RETURN_META(MRES_IGNORED); 
}


void RCBotPluginMeta::Hook_EquipWeapon(CBaseEntity *pWeapon)
{
	RETURN_META(MRES_IGNORED);
}


CBaseEntity *RCBotPluginMeta::Hook_GetPlayerWeaponSlot(int iSlot)
{
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void RCBotPluginMeta::Hook_RemoveWearable(CBaseEntity *pWearable)
{
	RETURN_META(MRES_IGNORED);
}


void RCBotPluginMeta::Hook_RemovePlayerItem(CBaseEntity *pWeapon)
{
	RETURN_META(MRES_IGNORED);
}

void RCBotPluginMeta::Hook_EquipWearable(CEconWearable *pItem)
{
	RETURN_META(MRES_IGNORED);
}


CBaseEntity *RCBotPluginMeta::Hook_GiveNamedItem( const char *name, int subtype, CEconItemView *cscript, bool b )
{
	CBaseEntity *pPlayer = META_IFACEPTR(CBaseEntity);
	edict_t *pEdict = servergameents->BaseEntityToEdict(pPlayer);
	CBot *pBot = NULL;

	if (rcbot_customloadouts.GetBool() && ((pBot = CBots::getBotPointer(pEdict)) != NULL) && cscript)
	{
		((CBotTF2*)pBot)->PostGiveNamedItem(cscript);
	}
	
	RETURN_META_VALUE(MRES_IGNORED, NULL); 
}
/** 
 * Something like this is needed to register cvars/CON_COMMANDs.
 */
class BaseAccessor : public IConCommandBaseAccessor
{
public:
	bool RegisterConCommandBase(ConCommandBase *pCommandBase)
	{
		/* Always call META_REGCVAR instead of going through the engine. */
		return META_REGCVAR(pCommandBase);
	}
} s_BaseAccessor;

// --- you're going to take over message begin
bf_write *RCBotPluginMeta::Hook_MessageBegin(IRecipientFilter *filter, int msg_type)
{
	/*
	bool bfound = false;

	for (int i = 0; i < filter->GetRecipientCount(); i++)
	{
		if (filter->GetRecipientIndex(i) == 1)
		{
			bfound = true;
			break;
		}
	}

	if (bfound)
	{
		
		int msgid = 0;
		int imsgsize = 0;
		char msgbuf[64];
		bool bOK;

		if (servergamedll->GetUserMessageInfo(msg_type, msgbuf, 63, imsgsize))
		{
			sprintf(current_msg_buffer, "MessageBegin() msg_type = %d name = %s\n", msg_type,msgbuf);
		}

	}
	else
		current_msg_buffer[0] = 0;
	
	current_msg = SH_CALL(engine, &IVEngineServer::UserMessageBegin)(filter, msg_type);

	if (current_msg)
	{
		SH_ADD_HOOK_MEMFUNC(bf_write, WriteString, current_msg, this, &RCBotPluginMeta::Hook_WriteString, true);
		SH_ADD_HOOK_MEMFUNC(bf_write, WriteByte, current_msg, this, &RCBotPluginMeta::Hook_WriteByte, true);
		SH_ADD_HOOK_MEMFUNC(bf_write, WriteChar, current_msg, this, &RCBotPluginMeta::Hook_WriteChar, true);
		SH_ADD_HOOK_MEMFUNC(bf_write, WriteShort, current_msg, this, &RCBotPluginMeta::Hook_WriteShort, true);
		SH_ADD_HOOK_MEMFUNC(bf_write, WriteFloat, current_msg, this, &RCBotPluginMeta::Hook_WriteFloat, true);
	}

	//
	RETURN_META_VALUE(MRES_SUPERCEDE, current_msg);*/

	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

void RCBotPluginMeta::Hook_WriteChar(int val)
{
	/*char tocat[64];

	sprintf(tocat, "\nWriteChar(%c)", (char)val);
	strcat(current_msg_buffer, tocat);*/
}
void RCBotPluginMeta::Hook_WriteShort(int val)
{
	/*char tocat[64];

	sprintf(tocat, "\nWriteShort(%d)", val);
	strcat(current_msg_buffer, tocat);*/
}
void RCBotPluginMeta::Hook_WriteByte(int val)
{
	/*char tocat[64];

	sprintf(tocat, "\nWriteByte(%d)", val);
	strcat(current_msg_buffer, tocat);*/
}
void RCBotPluginMeta::Hook_WriteFloat(float val)
{
	/*char tocat[64];

	sprintf(tocat, "\nWriteFloat(%0.1f)", val);
	strcat(current_msg_buffer, tocat);*/
}

bool RCBotPluginMeta::Hook_WriteString(const char *pStr)
{
	/*char *tocat = new char[strlen(pStr) + 16];
	
	sprintf(tocat, "\nWriteString(%s)", pStr);
	strcat(current_msg_buffer, tocat);
	
	delete tocat;*/

	RETURN_META_VALUE(MRES_IGNORED, false);
}

void RCBotPluginMeta::Hook_MessageEnd()
{
	// probe the current_msg m_pData
	// deep copy the data because it might free itself later
	//strncpy(current_msg_buffer, (char*)current_msg->m_pData, BUF_SIZ - 1);
	//current_msg_buffer[BUF_SIZ - 1] = 0;
	/*if (current_msg)
	{
		SH_REMOVE_HOOK_MEMFUNC(bf_write, WriteString, current_msg, this, &RCBotPluginMeta::Hook_WriteString, true);
		SH_REMOVE_HOOK_MEMFUNC(bf_write, WriteByte, current_msg, this, &RCBotPluginMeta::Hook_WriteByte, true);
		SH_REMOVE_HOOK_MEMFUNC(bf_write, WriteChar, current_msg, this, &RCBotPluginMeta::Hook_WriteChar, true);
		SH_REMOVE_HOOK_MEMFUNC(bf_write, WriteShort, current_msg, this, &RCBotPluginMeta::Hook_WriteShort, true);
		SH_REMOVE_HOOK_MEMFUNC(bf_write, WriteFloat, current_msg, this, &RCBotPluginMeta::Hook_WriteFloat, true);
	}

	current_msg_buffer[0] = 0;*/

	RETURN_META(MRES_IGNORED);
}

bool RCBotPluginMeta::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	extern MTRand_int32 irand;

	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, enginetrace, IEngineTrace, INTERFACEVERSION_ENGINETRACE_SERVER);	
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetEngineFactory, gameevents, IGameEventManager2, INTERFACEVERSION_GAMEEVENTSMANAGER2);
	GET_V_IFACE_CURRENT(GetEngineFactory, helpers, IServerPluginHelpers, INTERFACEVERSION_ISERVERPLUGINHELPERS);
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);

	GET_V_IFACE_ANY(GetEngineFactory, filesystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION)

	GET_V_IFACE_ANY(GetServerFactory, servergameents, IServerGameEnts, INTERFACEVERSION_SERVERGAMEENTS);
	GET_V_IFACE_ANY(GetServerFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(GetServerFactory, gameclients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	GET_V_IFACE_ANY(GetServerFactory, playerinfomanager, IPlayerInfoManager, INTERFACEVERSION_PLAYERINFOMANAGER);

	GET_V_IFACE_ANY(GetServerFactory, g_pEffects, IEffects, IEFFECTS_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, g_pBotManager, IBotManager, INTERFACEVERSION_PLAYERBOTMANAGER);
	GET_V_IFACE_ANY(GetServerFactory, servertools, IServerTools, VSERVERTOOLS_INTERFACE_VERSION);

#ifndef __linux__
	GET_V_IFACE_CURRENT(GetEngineFactory,debugoverlay, IVDebugOverlay, VDEBUG_OVERLAY_INTERFACE_VERSION);
#endif
	GET_V_IFACE_ANY(GetServerFactory, servergamedll, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(GetServerFactory, gameclients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);

	gpGlobals = ismm->GetCGlobals();

	META_LOG(g_PLAPI, "Starting plugin.");

	/* Load the VSP listener.  This is usually needed for IServerPluginHelpers. */
	if ((vsp_callbacks = ismm->GetVSPInfo(NULL)) == NULL)
	{
		ismm->AddListener(this, this);
		ismm->EnableVSPListener();
	}


	/*SH_ADD_HOOK_MEMFUNC(IVEngineServer, UserMessageBegin, engine, this, &RCBotPluginMeta::Hook_MessageBegin, false);
	SH_ADD_HOOK_MEMFUNC(IVEngineServer, MessageEnd, engine, this, &RCBotPluginMeta::Hook_MessageEnd, false);*/
	
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelInit, server, this, &RCBotPluginMeta::Hook_LevelInit, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, server, this, &RCBotPluginMeta::Hook_ServerActivate, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GameFrame, server, this, &RCBotPluginMeta::Hook_GameFrame, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, server, this, &RCBotPluginMeta::Hook_LevelShutdown, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientActive, gameclients, this, &RCBotPluginMeta::Hook_ClientActive, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, gameclients, this, &RCBotPluginMeta::Hook_ClientDisconnect, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, gameclients, this, &RCBotPluginMeta::Hook_ClientPutInServer, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, SetCommandClient, gameclients, this, &RCBotPluginMeta::Hook_SetCommandClient, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientSettingsChanged, gameclients, this, &RCBotPluginMeta::Hook_ClientSettingsChanged, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientConnect, gameclients, this, &RCBotPluginMeta::Hook_ClientConnect, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientCommand, gameclients, this, &RCBotPluginMeta::Hook_ClientCommand, false);
	//Hook FireEvent to our function - unstable for TF2? [APG]RoboCop[CL]
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &RCBotPluginMeta::FireGameEvent, false);


#if SOURCE_ENGINE >= SE_ORANGEBOX
	g_pCVar = icvar;
	ConVar_Register(0, &s_BaseAccessor);
#else
	ConCommandBaseMgr::OneTimeInit(&s_BaseAccessor);
#endif


	// Read Signatures and Offsets
	CBotGlobals::initModFolder();
	CBotGlobals::readRCBotFolder();

	char filename[512];
	// Load RCBOT2 hook data
	CBotGlobals::buildFileName(filename, "hookinfo", BOT_CONFIG_FOLDER, "ini");

	FILE *fp = fopen(filename, "r");

	CRCBotKeyValueList *pKVL = new CRCBotKeyValueList();

	if (fp)
		pKVL->parseFile(fp);

	void *gameServerFactory = reinterpret_cast<void*>(ismm->GetServerFactory(false));

	int val;

#ifdef _WIN32

	if (pKVL->getInt("givenameditem_win", &val))		
		rcbot_givenameditem_offset.SetValue(val);
	if (pKVL->getInt("equipwearable_win", &val))
		rcbot_equipwearable_offset.SetValue(val);
	if (pKVL->getInt("runplayermove_tf2_win", &val))
		rcbot_runplayercmd_tf2.SetValue(val);
	if (pKVL->getInt("runplayermove_dods_win", &val))
		rcbot_runplayercmd_dods.SetValue(val);
	if (pKVL->getInt("getweaponslot_win", &val))
		rcbot_getweaponslot_offset.SetValue(val);
	if (pKVL->getInt("removewearable_win", &val))
		rcbot_removewearable_offset.SetValue(val);
	if (pKVL->getInt("removeplayeritem_win", &val))
		rcbot_rmplayeritem_offset.SetValue(val);
	if (pKVL->getInt("weaponequip_win", &val))
		rcbot_weaponequip_offset.SetValue(val);
	if (pKVL->getInt("gamerules_win", &val))
		rcbot_gamerules_offset.SetValue(val);
	if (pKVL->getInt("mstr_offset_win", &val)) {
		rcbot_const_point_master_offset.SetValue(val);
		//rcbot_const_round_offset.SetValue(val);
	}
#else

	if (pKVL->getInt("givenameditem_linux", &val))
		rcbot_givenameditem_offset.SetValue(val);
	if (pKVL->getInt("equipwearable_linux", &val))
		rcbot_equipwearable_offset.SetValue(val);
	if (pKVL->getInt("runplayermove_tf2_linux", &val))
		rcbot_runplayercmd_tf2.SetValue(val);
	if (pKVL->getInt("runplayermove_dods_linux", &val))
		rcbot_runplayercmd_dods.SetValue(val);
	if (pKVL->getInt("getweaponslot_linux", &val))
		rcbot_getweaponslot_offset.SetValue(val);
	if (pKVL->getInt("removewearable_linux", &val))
		rcbot_removewearable_offset.SetValue(val);
	if (pKVL->getInt("removeplayeritem_linux", &val))
		rcbot_rmplayeritem_offset.SetValue(val);
	if (pKVL->getInt("weaponequip_linux", &val))
		rcbot_weaponequip_offset.SetValue(val);
	if (pKVL->getInt("mstr_offset_linux", &val)) {
		rcbot_const_point_master_offset.SetValue(val);
		//rcbot_const_round_offset.SetValue(val);
	}
#endif

	g_pGetEconItemSchema = new CGetEconItemSchema(pKVL, gameServerFactory);
	g_pSetRuntimeAttributeValue = new CSetRuntimeAttributeValue(pKVL, gameServerFactory);
	g_pGetAttributeDefinitionByName = new CGetAttributeDefinitionByName(pKVL, gameServerFactory);
	g_pAttribList_GetAttributeByID = new CAttributeList_GetAttributeByID(pKVL, gameServerFactory);
	g_pGameRules_Obj = new CGameRulesObject(pKVL, gameServerFactory);
	g_pGameRules_Create_Obj = new CCreateGameRulesObject(pKVL, gameServerFactory);
	g_pGetAttributeDefinitionByID = new CGetAttributeDefinitionByID(pKVL, gameServerFactory);

	delete pKVL;

	if (fp)
		fclose(fp);

	if (!CBotGlobals::gameStart())
		return false;

	CBotMod *pMod = CBotGlobals::getCurrentMod();

	if (CBots::controlBots())
	{
		if (pMod->getModId() == MOD_TF2)
			SH_MANUALHOOK_RECONFIGURE(MHook_PlayerRunCmd, rcbot_runplayercmd_tf2.GetInt(), 0, 0);
		else if (pMod->getModId() == MOD_DOD)
			SH_MANUALHOOK_RECONFIGURE(MHook_PlayerRunCmd, rcbot_runplayercmd_dods.GetInt(), 0, 0);
	}
	if (pMod->getModId() == MOD_TF2)
	{
		if (rcbot_givenameditem_offset.GetInt() > 0)
			SH_MANUALHOOK_RECONFIGURE(MHook_GiveNamedItem, rcbot_givenameditem_offset.GetInt(), 0, 0);

		if (rcbot_equipwearable_offset.GetInt() > 0)
			SH_MANUALHOOK_RECONFIGURE(MHook_EquipWearable, rcbot_equipwearable_offset.GetInt(), 0, 0);

		if (rcbot_weaponequip_offset.GetInt() > 0)
			SH_MANUALHOOK_RECONFIGURE(MHook_EquipWeapon, rcbot_weaponequip_offset.GetInt(), 0, 0);

		if (rcbot_rmplayeritem_offset.GetInt() > 0)
			SH_MANUALHOOK_RECONFIGURE(MHook_RemovePlayerItem, rcbot_rmplayeritem_offset.GetInt(), 0, 0);

		if (rcbot_getweaponslot_offset.GetInt() > 0)
			SH_MANUALHOOK_RECONFIGURE(MHook_GetPlayerWeaponSlot, rcbot_getweaponslot_offset.GetInt(), 0, 0);

		if (rcbot_removewearable_offset.GetInt() > 0)
			SH_MANUALHOOK_RECONFIGURE(MHook_RemoveWearable, rcbot_removewearable_offset.GetInt(), 0, 0);
	}
	ENGINE_CALL(LogPrint)("All hooks started!\n");



	//MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );
	//ConVar_Register( 0 );
	//InitCVars( interfaceFactory ); // register any cvars we have defined

	srand( (unsigned)time(NULL) );  // initialize the random seed
	irand.seed( (unsigned)time(NULL) );

	// Find the RCBOT2 Path from metamod VDF
	extern IFileSystem *filesystem;
	KeyValues *mainkv = new KeyValues("metamodplugin");
	
	const char *rcbot2path;
	CBotGlobals::botMessage(NULL, 0, "Reading rcbot2 path from VDF...");
	
	mainkv->LoadFromFile(filesystem, "addons/metamod/rcbot2.vdf", "MOD");
	
	mainkv = mainkv->FindKey("Metamod Plugin");

	if (mainkv)
		rcbot2path = mainkv->GetString("rcbot2path", "\0");

	mainkv->deleteThis();
	//eventListener2 = new CRCBotEventListener();

	// Initialize bot variables
	CBotProfiles::setupProfiles();


	//CBotEvents::setupEvents();
	CWaypointTypes::setup();
	CWaypoints::setupVisibility();

	CBotConfigFile::reset();	
	CBotConfigFile::load();

	CBotMenuList::setupMenus();

	//CRCBotPlugin::ShowLicense();	

	//RandomSeed((unsigned int)time(NULL));

	CClassInterface::init();

	RCBOT2_Cvar_setup(g_pCVar);

	// Bot Quota Settings
	char bq_line[128];

	int bot_count = 0;
	int human_count = 0;

	for (int i = 0; i < MAX_PLAYERS; ++i) {
		m_iTargetBots[i] = 0;
	}

	CBotGlobals::buildFileName(filename, "bot_quota", BOT_CONFIG_FOLDER, "ini");
	fp = fopen(filename, "r");

	memset(bq_line, 0, sizeof(bq_line));

	if (fp != NULL) {
		while (fgets(bq_line, sizeof(bq_line), fp) != NULL) {
			if (bq_line[0] == '#')
				continue;

			for (int i = 0; i < sizeof(bq_line); ++i) {
				if (bq_line[i] == '\0')
					break;

				if (!isdigit(bq_line[i]))
					bq_line[i] = ' ';
			}

			if (sscanf(bq_line, "%d %d", &human_count, &bot_count) == 2) {
				if (human_count < 0 || human_count > 32) {
					CBotGlobals::botMessage(NULL, 0, "Bot Quota - Invalid Human Count %d", human_count);
					continue;
				}

				if (bot_count < 0 || bot_count > 32) {
					CBotGlobals::botMessage(NULL, 0, "Bot Quota - Invalid Bot Count %d", bot_count);
					continue;
				}

				m_iTargetBots[human_count] = bot_count;
				CBotGlobals::botMessage(NULL, 0, "Bot Quota - Humans: %d, Bots: %d", human_count, bot_count);
			}
		}
	}

	if (fp) {
		fclose(fp);
	}

	return true;
}

bool RCBotPluginMeta::FireGameEvent(IGameEvent * pevent, bool bDontBroadcast)
{
	static char szKey[128];
	static char szValue[128];

	CBotEvents::executeEvent((void*)pevent,TYPE_IGAMEEVENT);	

RETURN_META_VALUE(MRES_IGNORED, true);
}

bool RCBotPluginMeta::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelInit, server, this, &RCBotPluginMeta::Hook_LevelInit, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, server, this, &RCBotPluginMeta::Hook_ServerActivate, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GameFrame, server, this, &RCBotPluginMeta::Hook_GameFrame, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, server, this, &RCBotPluginMeta::Hook_LevelShutdown, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientActive, gameclients, this, &RCBotPluginMeta::Hook_ClientActive, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, gameclients, this, &RCBotPluginMeta::Hook_ClientDisconnect, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, gameclients, this, &RCBotPluginMeta::Hook_ClientPutInServer, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, SetCommandClient, gameclients, this, &RCBotPluginMeta::Hook_SetCommandClient, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientSettingsChanged, gameclients, this, &RCBotPluginMeta::Hook_ClientSettingsChanged, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientConnect, gameclients, this, &RCBotPluginMeta::Hook_ClientConnect, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientCommand, gameclients, this, &RCBotPluginMeta::Hook_ClientCommand, false);
	
	//SH_REMOVE_MANUALHOOK(MHook_PlayerRunCmd, player_vtable, SH_STATIC(Hook_Function2), false);

	// if another instance is running dont run through this
	//if ( !bInitialised )
	//	return;
	
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
	CAttributeLookup::freeMemory();
	//unloadSignatures();

	//UnhookPlayerRunCommand();
	//UnhookGiveNamedItem();

	//ConVar_Unregister();

	//if ( gameevents )
	//	gameevents->RemoveListener(this);

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

	return true;
}

void RCBotPluginMeta::OnVSPListening(IServerPluginCallbacks *iface)
{
	vsp_callbacks = iface;
}

void RCBotPluginMeta::Hook_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
{
	META_LOG(g_PLAPI, "ServerActivate() called: edictCount = %d, clientMax = %d", edictCount, clientMax);

	CAccessClients::load();

	CBotGlobals::setClientMax(clientMax);
}

void RCBotPluginMeta::AllPluginsLoaded()
{
	/* This is where we'd do stuff that relies on the mod or other plugins 
	 * being initialized (for example, cvars added and events registered).
	 */
}

void RCBotPluginMeta::Hook_ClientActive(edict_t *pEntity, bool bLoadGame)
{
	META_LOG(g_PLAPI, "Hook_ClientActive(%d, %d)", IndexOfEdict(pEntity), bLoadGame);

	CClients::clientActive(pEntity);
}

#if SOURCE_ENGINE >= SE_ORANGEBOX
void RCBotPluginMeta::Hook_ClientCommand(edict_t *pEntity, const CCommand &args)
#else
void RCBotPluginMeta::Hook_ClientCommand(edict_t *pEntity)
#endif
{
	static CBotMod *pMod = NULL;

#if SOURCE_ENGINE <= SE_DARKMESSIAH
	CCommand args;
#endif

	const char *pcmd = args.Arg(0);

	if (!pEntity || pEntity->IsFree())
	{
		return;
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

		RETURN_META(MRES_SUPERCEDE);
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

	RETURN_META(MRES_IGNORED); 
}

void RCBotPluginMeta::Hook_ClientSettingsChanged(edict_t *pEdict)
{

}

bool RCBotPluginMeta::Hook_ClientConnect(edict_t *pEntity,
									const char *pszName,
									const char *pszAddress,
									char *reject,
									int maxrejectlen)
{
	META_LOG(g_PLAPI, "Hook_ClientConnect(%d, \"%s\", \"%s\")", IndexOfEdict(pEntity), pszName, pszAddress);

	CClients::init(pEntity);

	return true;
}

void RCBotPluginMeta::Hook_ClientPutInServer(edict_t *pEntity, char const *playername)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pEntity);
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

	if ( pEnt )
	{
		if (CBots::controlBots())
			SH_ADD_MANUALHOOK_MEMFUNC(MHook_PlayerRunCmd, pEnt, this, &RCBotPluginMeta::Hook_PlayerRunCmd, false);

		if (pMod->getModId() == MOD_TF2)
		{
			SH_ADD_MANUALHOOK_MEMFUNC(MHook_GiveNamedItem, pEnt, this, &RCBotPluginMeta::Hook_GiveNamedItem, false);

			SH_ADD_MANUALHOOK_MEMFUNC(MHook_EquipWearable, pEnt, this, &RCBotPluginMeta::Hook_EquipWearable, false);

			SH_ADD_MANUALHOOK_MEMFUNC(MHook_EquipWeapon, pEnt, this, &RCBotPluginMeta::Hook_EquipWeapon, false);

			SH_ADD_MANUALHOOK_MEMFUNC(MHook_RemovePlayerItem, pEnt, this, &RCBotPluginMeta::Hook_RemovePlayerItem, false);

			SH_ADD_MANUALHOOK_MEMFUNC(MHook_GetPlayerWeaponSlot, pEnt, this, &RCBotPluginMeta::Hook_GetPlayerWeaponSlot, false);
			SH_ADD_MANUALHOOK_MEMFUNC(MHook_RemoveWearable, pEnt, this, &RCBotPluginMeta::Hook_RemoveWearable, false);
		}
	}
}

void RCBotPluginMeta::Hook_ClientDisconnect(edict_t *pEntity)
{
	CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pEntity);

	if ( pEnt )
	{
		CBotMod *pMod = CBotGlobals::getCurrentMod();

		if (CBots::controlBots())
			SH_REMOVE_MANUALHOOK_MEMFUNC(MHook_PlayerRunCmd, pEnt, this, &RCBotPluginMeta::Hook_PlayerRunCmd, false);

		if (pMod->getModId() == MOD_TF2)
		{
			SH_REMOVE_MANUALHOOK_MEMFUNC(MHook_GiveNamedItem, pEnt, this, &RCBotPluginMeta::Hook_GiveNamedItem, false);

			SH_REMOVE_MANUALHOOK_MEMFUNC(MHook_EquipWearable, pEnt, this, &RCBotPluginMeta::Hook_EquipWearable, false);

			SH_REMOVE_MANUALHOOK_MEMFUNC(MHook_EquipWeapon, pEnt, this, &RCBotPluginMeta::Hook_EquipWeapon, false);

			SH_REMOVE_MANUALHOOK_MEMFUNC(MHook_RemovePlayerItem, pEnt, this, &RCBotPluginMeta::Hook_RemovePlayerItem, false);

			SH_REMOVE_MANUALHOOK_MEMFUNC(MHook_GetPlayerWeaponSlot, pEnt, this, &RCBotPluginMeta::Hook_GetPlayerWeaponSlot, false);
			SH_REMOVE_MANUALHOOK_MEMFUNC(MHook_RemoveWearable, pEnt, this, &RCBotPluginMeta::Hook_RemoveWearable, false);
		}
	}

	CClients::clientDisconnected(pEntity);

	META_LOG(g_PLAPI, "Hook_ClientDisconnect(%d)", IndexOfEdict(pEntity));
}

void RCBotPluginMeta::Hook_GameFrame(bool simulating)
{
	/**
	 * simulating:
	 * ***********
	 * true  | game is ticking
	 * false | game is not ticking
	 */

	static CBotMod *currentmod;

	if ( simulating && CBotGlobals::IsMapRunning() )
	{
		CBots::botThink();
		//if ( !CBots::controlBots() )
			//gameclients->PostClientMessagesSent();
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

		// Bot Quota
		if (rcbot_bot_quota_interval.GetInt() > 0) {
			BotQuotaCheck();
		}
	}
}

void RCBotPluginMeta::BotQuotaCheck() {
	if (rcbot_bot_quota_interval.GetInt() < 0) {
		return;
	}

	if (m_fBotQuotaTimer < 1.0f) {
		m_fBotQuotaTimer = engine->Time() + 10.0f; // Sleep 10 seconds
	}

	if (m_fBotQuotaTimer < engine->Time() - rcbot_bot_quota_interval.GetInt()) {
		m_fBotQuotaTimer = engine->Time();

		// Target Bot Count
		int bot_target = 0;
		int bot_diff = 0;

		// Change Notification
		bool notify = false;

		// Current Bot Count
		int bot_count = 0;
		int human_count = 0;

		// Count Players
		for (int i = 0; i < MAX_PLAYERS; ++i) {
			CClient* client = CClients::get(i);
			CBot* bot = CBots::get(i);

			if (bot != NULL && bot->getEdict() != NULL && bot->inUse()) {
				IPlayerInfo *p = playerinfomanager->GetPlayerInfo(bot->getEdict());

				if (p->IsConnected() && p->IsFakeClient()) {
					bot_count++;
				}
			}

			if (client != NULL && client->getPlayer() != NULL && client->isUsed()) {
				IPlayerInfo *p = playerinfomanager->GetPlayerInfo(client->getPlayer());

				if (p->IsConnected() && !p->IsFakeClient()) {
					human_count++;
				}
			}
		}

		if (human_count >= MAX_PLAYERS) {
			human_count = 0;
		}

		// Get Bot Quota
		bot_target = m_iTargetBots[human_count];

		// Change Bot Quota
		if (bot_target < bot_count) {
			bot_diff = bot_count - bot_target;

			for (int i = 0; i < bot_diff; ++i) {
				CBots::kickRandomBot();
			}

			notify = true;
		} else if (bot_target > bot_count) {
			bot_diff = bot_target - bot_count;

			for (int i = 0; i < bot_diff; ++i) {
				CBots::addBot("", "", "");
				break; // Bug-Fix, only add one bot at a time
			}

			notify = true;
		}

		if (notify) {
			char chatmsg[128];
			snprintf(chatmsg, sizeof(chatmsg), "[Bot Quota] Humans: %d, Bots: %d", human_count, bot_target);

			CBotGlobals::botMessage(NULL, 0, "=======================================");
			CBotGlobals::botMessage(NULL, 0, chatmsg);
			CBotGlobals::botMessage(NULL, 0, "=======================================");

			// RCBotPluginMeta::BroadcastTextMessage(chatmsg);
		}
	}
}

bool RCBotPluginMeta::Hook_LevelInit(const char *pMapName,
								char const *pMapEntities,
								char const *pOldLevel,
								char const *pLandmarkName,
								bool loadGame,
								bool background)
{
	META_LOG(g_PLAPI, "Hook_LevelInit(%s)", pMapName);

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

	CBotEvents::setupEvents();

	CBots::mapInit();

	CBotMod *pMod = CBotGlobals::getCurrentMod();
	
	if ( pMod )
		pMod->mapInit();

	CBotSquads::FreeMemory();

	CClients::setListenServerClient(NULL);

	// Setup game rules
	extern void **g_pGameRules;

	if (g_pGameRules_Obj && g_pGameRules_Obj->found())
	{
		g_pGameRules = g_pGameRules_Obj->getGameRules();
	}
	else if (g_pGameRules_Create_Obj && g_pGameRules_Create_Obj->found())
	{
		g_pGameRules = g_pGameRules_Create_Obj->getGameRules();
	}

	return true;
}

void RCBotPluginMeta::Hook_LevelShutdown()
{
	META_LOG(g_PLAPI, "Hook_LevelShutdown()");

	CClients::initall();
	CWaypointDistances::save();

	CBots::freeMapMemory();	
	CWaypoints::init();

	CBotGlobals::setMapRunning(false);
	CBotEvents::freeMemory();
}

void RCBotPluginMeta::Hook_SetCommandClient(int index)
{
	META_LOG(g_PLAPI, "Hook_SetCommandClient(%d)", index);
}

bool RCBotPluginMeta::Pause(char *error, size_t maxlen)
{
	return true;
}

bool RCBotPluginMeta::Unpause(char *error, size_t maxlen)
{
	return true;
}

const char *RCBotPluginMeta::GetLicense()
{
	return "GPL General Public License";
}

const char *RCBotPluginMeta::GetVersion()
{
	return "1.01 (r487-apg-ch)";
}

const char *RCBotPluginMeta::GetDate()
{
	return __DATE__;
}

const char *RCBotPluginMeta::GetLogTag()
{
	return "RCBOT2";
}

const char *RCBotPluginMeta::GetAuthor()
{
	return "Cheeseh, RoboCop";
}

const char *RCBotPluginMeta::GetDescription()
{
	return "Bot for HL2DM, TF2 and DOD:S";
}

const char *RCBotPluginMeta::GetName()
{
	return "RCBot2";
}

const char *RCBotPluginMeta::GetURL()
{
	return "http://rcbot.bots-united.com/";
}
