#include "bot_sm_ext.h"
#include "bot_sm_natives.h"

#include "bot.h"
#include "bot_profile.h"
#include "bot_waypoint.h"

enum RCBotProfileVar {
	RCBotProfile_iVisionTicks,
	RCBotProfile_iPathTicks,
	RCBotProfile_iVisionTicksClients,
	RCBotProfile_iSensitivity,
	RCBotProfile_fBraveness,
	RCBotProfile_fAimSkill,
	RCBotProfile_iClass,
};

int* GetIntProperty(CBotProfile* profile, RCBotProfileVar profileVar);
float* GetFloatProperty(CBotProfile* profile, RCBotProfileVar profileVar);

cell_t sm_RCBotIsWaypointAvailable(IPluginContext *pContext, const cell_t *params) {
	return CWaypoints::numWaypoints() > 0;
}

cell_t sm_RCBotCreate(IPluginContext *pContext, const cell_t *params) {
	char *name;
	pContext->LocalToString(params[1], &name);

	const int slot = CBots::createDefaultBot(name);
	
	// player slots are off-by-one (though this calculation is performed in the function)
	return (slot != -1)? slot + 1 : -1;
}

/* native void RCBot2_SetProfileInt(int client, RCBotProfileVar property, int value); */
cell_t sm_RCBotSetProfileInt(IPluginContext *pContext, const cell_t *params) {
	const int client = params[1];
	const RCBotProfileVar profileVar = static_cast<RCBotProfileVar>(params[2]);
	if (client < 1 || client > gpGlobals->maxClients) {
		pContext->ThrowNativeError("Invalid client index %d", client);
	}

	const CBot* bot = CBots::getBot(client - 1);
	if (!bot) {
		pContext->ThrowNativeError("Client index %d is not a RCBot", client);
	}
	
	CBotProfile* profile = bot->getProfile();
	int* value = GetIntProperty(profile, profileVar);
	if (!value) {
		pContext->ThrowNativeError("RCBot property %d is not an integer property", profileVar);
		return 0;
	}
	*value = params[3];
	
	return 0;
}

/* native void RCBot2_SetProfileFloat(int client, RCBotProfileVar property, float value); */
cell_t sm_RCBotSetProfileFloat(IPluginContext *pContext, const cell_t *params) {
	const int client = params[1];
	const RCBotProfileVar profileVar = static_cast<RCBotProfileVar>(params[2]);
	if (client < 1 || client > gpGlobals->maxClients) {
		pContext->ThrowNativeError("Invalid client index %d", client);
	}

	const CBot* bot = CBots::getBot(client - 1);
	if (!bot) {
		pContext->ThrowNativeError("Client index %d is not a RCBot", client);
	}
	
	CBotProfile* profile = bot->getProfile();
	float* value = GetFloatProperty(profile, profileVar);
	if (!value) {
		pContext->ThrowNativeError("RCBot property %d is not a float property", profileVar);
		return 0;
	}
	*value = sp_ctof(params[3]);
	
	return 0;
}

/* native int RCBot2_GetProfileInt(int client, RCBotProfileVar property); */
cell_t sm_RCBotGetProfileInt(IPluginContext *pContext, const cell_t *params) {
	const int client = params[1];
	const RCBotProfileVar profileVar = static_cast<RCBotProfileVar>(params[2]);
	if (client < 1 || client > gpGlobals->maxClients) {
		pContext->ThrowNativeError("Invalid client index %d", client);
	}

	const CBot* bot = CBots::getBot(client - 1);
	if (!bot) {
		pContext->ThrowNativeError("Client index %d is not a RCBot", client);
	}
	
	CBotProfile* profile = bot->getProfile();
	const int* value = GetIntProperty(profile, profileVar);
	if (!value) {
		pContext->ThrowNativeError("RCBot property %d is not an integer property", profileVar);
		return 0;
	}
	return *value;
}

/* native float RCBot2_GetProfileFloat(int client, RCBotProfileVar property); */
cell_t sm_RCBotGetProfileFloat(IPluginContext *pContext, const cell_t *params) {
	const int client = params[1];
	const RCBotProfileVar profileVar = static_cast<RCBotProfileVar>(params[2]);
	if (client < 1 || client > gpGlobals->maxClients) {
		pContext->ThrowNativeError("Invalid client index %d", client);
	}

	const CBot* bot = CBots::getBot(client - 1);
	if (!bot) {
		pContext->ThrowNativeError("Client index %d is not a RCBot", client);
	}
	
	CBotProfile* profile = bot->getProfile();
	const float* value = GetFloatProperty(profile, profileVar);
	if (!value) {
		pContext->ThrowNativeError("RCBot property %d is not a float property", profileVar);
		return 0;
	}
	return sp_ftoc(*value);
}

cell_t sm_RCBotIsClientBot(IPluginContext *pContext, const cell_t *params) {
	const int client = params[1];
	if (client < 1 || client > gpGlobals->maxClients) {
		pContext->ThrowNativeError("Invalid client index %d", client);
		return 0;
	}
	return CBots::getBot(client - 1) != nullptr;
}

int* GetIntProperty(CBotProfile* profile, RCBotProfileVar profileVar) {
	switch (profileVar) {
		case RCBotProfile_iVisionTicks:
			return &profile->m_iVisionTicks;
		case RCBotProfile_iPathTicks:
			return &profile->m_iPathTicks;
		case RCBotProfile_iClass:
			return &profile->m_iClass;
		case RCBotProfile_iVisionTicksClients:
			return &profile->m_iVisionTicksClients;
		case RCBotProfile_iSensitivity:
			return &profile->m_iSensitivity;
	}
	return nullptr;
}

float* GetFloatProperty(CBotProfile* profile, RCBotProfileVar profileVar) {
	switch (profileVar) {
		case RCBotProfile_fBraveness:
			return &profile->m_fBraveness;
		case RCBotProfile_fAimSkill:
			return &profile->m_fAimSkill;
	}
	return nullptr;
}
