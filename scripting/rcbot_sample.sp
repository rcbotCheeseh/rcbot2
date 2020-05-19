/**
 * Sample plugin for native calls to RCBot2 from SourceMod.
 */
#pragma semicolon 1
#include <sourcemod>

#pragma newdecls required

#include <rcbot2>

public void OnPluginStart() {
	RegAdminCmd("sm_addrcbot", AddRCBot, ADMFLAG_ROOT);
	RegAdminCmd("sm_rcbot_supported", TestRCBotSupport, ADMFLAG_ROOT);
}

public Action TestRCBotSupport(int client, int argc) {
	ReplyToCommand(client, "RCBot waypoint available: %b", RCBot2_IsWaypointAvailable());
	return Plugin_Handled;
}

public Action AddRCBot(int client, int argc) {
	char name[64];
	if (argc) {
		GetCmdArg(1, name, sizeof(name));
	}
	
	int bot = RCBot2_CreateBot(name);
	if (bot == -1) {
		ReplyToCommand(client, "Failed to create RCBot.");
	} else {
		ReplyToCommand(client, "Created RCBot '%N', slot %d", bot, bot);
		PrintToServer("%N->AimSkill = %.2f", bot, RCBot2_GetProfileFloat(bot, RCBotProfile_fAimSkill));
		PrintToServer("%N->Braveness = %.2f", bot, RCBot2_GetProfileFloat(bot, RCBotProfile_fBraveness));
		PrintToServer("%N->PathTicks: %d", bot, RCBot2_GetProfileInt(bot, RCBotProfile_iPathTicks));
		PrintToServer("%N->Sensitivity: %d", bot, RCBot2_GetProfileInt(bot, RCBotProfile_iSensitivity));
		PrintToServer("%N->VisionTicks: %d", bot, RCBot2_GetProfileInt(bot, RCBotProfile_iVisionTicks));
		PrintToServer("%N->VisionTicksClients: %d", bot, RCBot2_GetProfileInt(bot, RCBotProfile_iVisionTicksClients));
	}
	return Plugin_Handled;
}
