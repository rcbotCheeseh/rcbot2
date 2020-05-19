#if defined __rcbot_included
	#endinput
#endif

#define __rcbot_included

enum RCBotProfileVar {
	// speed of recognizing non-player entities (NPCs / teleporters)
	RCBotProfile_iVisionTicks,
	
	// speed of finding a path
	RCBotProfile_iPathTicks,
	
	// speed of recognizing other players
	RCBotProfile_iVisionTicksClients,
	
	// sensitivity of bot's "mouse"; integer between 1 and 20
	RCBotProfile_iSensitivity,
	
	// sensitivity to danger (brave = less sensitive); float between 0.0 to 1.0
	RCBotProfile_fBraveness,
	
	// ability to predict player movements; float between 0.0 to 1.0
	RCBotProfile_fAimSkill,
	
	// preferred player class
	RCBotProfile_iClass,
};

/**
 * Returns true if a waypoint is available for the current map.
 */
native bool RCBot2_IsWaypointAvailable();

/**
 * Instantiates an RCBot2 bot for the current mod with a copy of the default profile.
 * 
 * @return RCBot2 client index, or -1 on failure
 */
native int RCBot2_CreateBot(const char[] name);

/**
 * Returns true if the client index is an RCBot2 bot.  This does not currently true until after
 * the bot is in the game.
 * 
 * @error Client not in game or invalid client index.
 */
native int IsRCBot2Client(int client);

/**
 * Sets the profile property to the given integer value.
 * 
 * @noreturn
 * @error Client is not an RCBot2 bot or specified property is not an integer value
 */
native void RCBot2_SetProfileInt(int client, RCBotProfileVar property, int value);

/**
 * Gets the float value for a given bot's profile property.
 * 
 * @error Client is not an RCBot2 bot or specified property is not an integer value
 */
native int RCBot2_GetProfileInt(int client, RCBotProfileVar property);

/**
 * Sets the profile property to the given float value.
 * 
 * @noreturn
 * @error Client is not an RCBot2 bot or specified property is not a float value
 */
native void RCBot2_SetProfileFloat(int client, RCBotProfileVar property, float value);

/**
 * Gets the float value for a given bot's profile property.
 * 
 * @error Client is not an RCBot2 bot or specified property is not a float value
 */
native float RCBot2_GetProfileFloat(int client, RCBotProfileVar property);

public Extension __ext_rcbot2 = {
	name = "RCBot2",
	file = "rcbot2.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
public void __ext_rcbot2_SetNTVOptional() {
}
#endif
