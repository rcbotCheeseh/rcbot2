#pragma once

using namespace SourceMod;
using namespace SourcePawn;

cell_t sm_RCBotIsWaypointAvailable(IPluginContext *pContext, const cell_t *params);

cell_t sm_RCBotCreate(IPluginContext *pContext, const cell_t *params);
cell_t sm_RCBotIsClientBot(IPluginContext *pContext, const cell_t *params);

cell_t sm_RCBotSetProfileInt(IPluginContext *pContext, const cell_t *params);
cell_t sm_RCBotGetProfileInt(IPluginContext *pContext, const cell_t *params);

cell_t sm_RCBotSetProfileFloat(IPluginContext *pContext, const cell_t *params);
cell_t sm_RCBotGetProfileFloat(IPluginContext *pContext, const cell_t *params);

const sp_nativeinfo_t g_RCBotNatives[] = {
	{ "RCBot2_IsWaypointAvailable", sm_RCBotIsWaypointAvailable },
	
	{ "RCBot2_CreateBot", sm_RCBotCreate },
	{ "IsRCBot2Client", sm_RCBotIsClientBot },
	
	{ "RCBot2_SetProfileInt", sm_RCBotSetProfileInt },
	{ "RCBot2_GetProfileInt", sm_RCBotGetProfileInt },
	
	{ "RCBot2_SetProfileFloat", sm_RCBotSetProfileFloat },
	{ "RCBot2_GetProfileFloat", sm_RCBotGetProfileFloat },

	{NULL, NULL},
};
