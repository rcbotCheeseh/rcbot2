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
 *
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

#ifndef __BOT_PLUGIN_META_H__
#define __BOT_PLUGIN_META_H__

 //Fix by nosoop
#define swap V_swap
#include "mathlib/mathlib.h"
#undef swap

#include <ISmmPlugin.h>
#include <igameevents.h>
#include <iplayerinfo.h>
#include <sh_vector.h>
#include "engine_wrappers.h"
#include <shareddefs.h>

class CUserCmd;
class IMoveHelper;
class CEconItemView;
class CTF2Loadout;
class CEconWearable;

#if defined WIN32 && !defined snprintf
#define snprintf _snprintf
#endif

class RCBotPluginMeta final : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late) override;
	bool Unload(char* error, size_t maxlen) override;
	bool Pause(char* error, size_t maxlen) override;
	bool Unpause(char* error, size_t maxlen) override;
	void AllPluginsLoaded() override;
public: //IMetamodListener stuff
	void OnVSPListening(IServerPluginCallbacks* iface) override;
public: //hooks

	void Hook_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax);
	bool Hook_LevelInit(const char* pMapName,
		char const* pMapEntities,
		char const* pOldLevel,
		char const* pLandmarkName,
		bool loadGame,
		bool background);
	void Hook_GameFrame(bool simulating);
	void Hook_LevelShutdown(void);
	void Hook_ClientActive(edict_t* pEntity, bool bLoadGame);
	void Hook_ClientDisconnect(edict_t* pEntity);
	void Hook_ClientPutInServer(edict_t* pEntity, char const* playername);
	void Hook_SetCommandClient(int index);
	void Hook_ClientSettingsChanged(edict_t* pEdict);
	//Called for a game event.  Same definition as server plugins???
	bool FireGameEvent(IGameEvent* pevent, bool bDontBroadcast);
	void Hook_PlayerRunCmd(CUserCmd* ucmd, IMoveHelper* moveHelper);
	CBaseEntity* Hook_GiveNamedItem(const char* name, int subtype, CEconItemView* cscript, bool b);
	void Hook_EquipWearable(CEconWearable* pItem);
	void Hook_EquipWeapon(CBaseEntity* pWeapon);
	void Hook_RemovePlayerItem(CBaseEntity* pWeapon);

	CBaseEntity* Hook_GetPlayerWeaponSlot(int iSlot);
	void Hook_RemoveWearable(CBaseEntity* pWearable);
	bool Hook_ClientConnect(edict_t* pEntity,
		const char* pszName,
		const char* pszAddress,
		char* reject,
		int maxrejectlen);
	bf_write* Hook_MessageBegin(IRecipientFilter* filter, int msg_type);
	void Hook_MessageEnd();

	void Hook_WriteChar(int val);
	void Hook_WriteShort(int val);
	void Hook_WriteByte(int val);
	void Hook_WriteFloat(float val);
	bool Hook_WriteString(const char* pStr);

	static CBaseEntity* TF2_getPlayerWeaponSlot(edict_t* pPlayer, int iSlot);
	static void TF2_removeWearable(edict_t* pPlayer, CBaseEntity* pWearable);
	static void TF2_removePlayerItem(edict_t* pPlayer, CBaseEntity* pItem);
	static void TF2_RemoveWeaponSlot(edict_t* pPlayer, int iSlot);
	static void TF2_equipWeapon(edict_t* pPlayer, CBaseEntity* pWeapon);
	static bool givePlayerLoadOut(edict_t* pPlayer, CTF2Loadout* pLoadout, int iSlot, void* pVTable, void* pVTable_Attributes);
	static void giveRandomLoadout(edict_t* pPlayer, int iClass, int iSlot, void* pVTable, void* pVTable_Attributes);
	static void TF2_equipWearable(edict_t* pPlayer, CBaseEntity* pWearable);
	static bool TF2_ClearAttributeCache(edict_t* pEdict);

	static void HudTextMessage(edict_t* pEntity, const char* szMessage);
	static void BroadcastTextMessage(const char* szMessage);

#if SOURCE_ENGINE >= SE_ORANGEBOX
	void Hook_ClientCommand(edict_t* pEntity, const CCommand& args);
#else
	void Hook_ClientCommand(edict_t* pEntity);
#endif

public:

	const char* GetAuthor() override;
	const char* GetName() override;
	const char* GetDescription() override;
	const char* GetURL() override;
	const char* GetLicense() override;
	const char* GetVersion() override;
	const char* GetDate() override;
	const char* GetLogTag() override;

	static bool UTIL_TF2EquipHat(edict_t* pEdict, CTF2Loadout* pHat, void* vTable, void* vTableAttributes);
	static CTF2Loadout* UTIL_TF2EquipRandomHat(edict_t* pEdict, void* vTable, void* vTableAttributes);

private:
	int m_iClientCommandIndex = 0;

	// Bot Quota
	float m_fBotQuotaTimer = 0;
	int m_iTargetBots[MAX_PLAYERS];

	void BotQuotaCheck(void);
};

extern RCBotPluginMeta g_RCBotPluginMeta;

PLUGIN_GLOBALVARS()

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_