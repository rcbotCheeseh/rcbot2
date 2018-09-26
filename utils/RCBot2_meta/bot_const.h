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
#ifndef __BOT_CONST_H__
#define __BOT_CONST_H__

#include "shareddefs.h"

#ifndef __linux__
#define BOT_WELCOME_MESSAGE "Welcome to RCBot by Cheeseh"
#else
#define BOT_WELCOME_MESSAGE "Welcome to RCBot by Cheeseh for Linux"
#endif

#define BOT_DEFAULT_FOV 75.0f

#define __to_lower(a) (((a)>='A')&&((a)<='Z'))?('a'+((a)-'A')):(a)
#define __strlow(str) { char *__strx = str; while ( __strx && *__strx ) { *__strx = __to_lower(*__strx); __strx++; } }
//#define strlow(str) { unsigned short int len = strlen(str); unsigned short int i;	for ( i = 0; i < len; i ++ ) { str[i] = to_lower(str[i]); } }
#define __round(a) (((a-(int)a) >= 0.5) ? ((int)a+1) : ((int)a))

//#define RANDOM_INT(min,max) (min + round(((float)rand()/RAND_MAX)*(float)(max-min)))
//#define RANDOM_FLOAT(min,max) (min + ((float)rand()/RAND_MAX)*(float)(max-min))

#define DEFAULT_BOT_NAME "RCBot"

#define BOT_CONVAR_FLAGS_OFFSET 20

#define BOT_WPT_TOUCH_DIST 72 // distance for bot to touch waypoint

#define BOT_DEBUG_GAME_EVENT	0 
#define BOT_DEBUG_NAV			1 
#define BOT_DEBUG_SPEED			2 
#define BOT_DEBUG_VIS			3
#define BOT_DEBUG_TASK			4 
#define BOT_DEBUG_BUTTONS		5  
#define BOT_DEBUG_USERCMD		6 
#define BOT_DEBUG_UTIL			7
#define BOT_DEBUG_PROFILE		8 
#define BOT_DEBUG_EDICTS		9 
#define BOT_DEBUG_THINK			10 
#define BOT_DEBUG_LOOK			11 
#define BOT_DEBUG_HUD			12 
#define BOT_DEBUG_AIM			13 
#define BOT_DEBUG_CHAT			14

// from sourcemod
enum RoundState 
{
	// initialize the game, create teams
	RoundState_Init,
	//Before players have joined the game. Periodically checks to see if enough players are ready
	//to start a game. Also reverts to this when there are no active players
	RoundState_Pregame,
	//The game is about to start, wait a bit and spawn everyone
	RoundState_StartGame,
	//All players are respawned, frozen in place
	RoundState_Preround,
	//Round is on, playing normally
	RoundState_RoundRunning,
	//Someone has won the round
	RoundState_TeamWin,
	//Noone has won, manually restart the game, reset scores
	RoundState_Restart,
	//Noone has won, restart the game
	RoundState_Stalemate,
	//Game is over, showing the scoreboard etc
	RoundState_GameOver,
	//Game is over, doing bonus round stuff
	RoundState_Bonus,
	//Between rounds
	RoundState_BetweenRounds,
};

typedef enum
{
	LOOK_NONE = 0,
	LOOK_VECTOR,
	LOOK_WAYPOINT,
	LOOK_WAYPOINT_NEXT_ONLY,
	LOOK_AROUND,
	LOOK_ENEMY,
	LOOK_LAST_ENEMY,
	LOOK_HURT_ORIGIN,
	LOOK_EDICT,
	LOOK_GROUND,
	LOOK_SNIPE,
	LOOK_WAYPOINT_AIM,
	LOOK_BUILD,
	LOOK_NOISE,
	LOOK_MAX
}eLookTask;

extern const char *g_szLookTaskToString[LOOK_MAX];

#define BOT_CONFIG_FOLDER "config"
#define BOT_MOD_FILE "bot_mods"
#define BOT_ACCESS_CLIENT_FILE "accessclients"
#define BOT_PROFILE_FOLDER "profiles"
#define BOT_WAYPOINT_FOLDER "waypoints"
#define BOT_CONFIG_EXTENSION "ini"

#define BOT_WAYPOINT_EXTENSION "rcw" // extension for waypoint files
#define BOT_WAYPOINT_FILE_TYPE "RCBot2\0" // for waypoint file header

#define BOT_TAG "[RCBot] " // for printing messages
/*
// Engine player info, no game related infos here
// If you change this, change the two byteswap defintions: 
// cdll_client_int.cpp and cdll_engine_int.cpp
typedef struct player_info_s
{
	DECLARE_BYTESWAP_DATADESC();
	// scoreboard information
	char			name[MAX_PLAYER_NAME_LENGTH];
	// local server user ID, unique while server is running
	int				userID;
	// global unique player identifer
	char			guid[SIGNED_GUID_LEN + 1];
	// friends identification number
	uint32			friendsID;
	// friends name
	char			friendsName[MAX_PLAYER_NAME_LENGTH];
	// true, if player is a bot controlled by game.dll
	bool			fakeplayer;
	// true if player is the HLTV proxy
	bool			ishltv;
	// custom files CRC for this player
	CRC32_t			customFiles[MAX_CUSTOM_FILES];
	// this counter increases each time the server downloaded a new file
	unsigned char	filesDownloaded;
} player_info_t;*/

typedef enum
{
	MOD_UNSUPPORTED = 0,
	MOD_HLDM2,
	MOD_CSS,
	MOD_FF,
	MOD_TF2,
	MOD_SVENCOOP2,
	MOD_TIMCOOP,
	MOD_HL1DMSRC,
	MOD_NS2,
	MOD_SYNERGY,
	MOD_DOD,
	MOD_CUSTOM,
	MOD_ANY,
	MOD_MAX
}eModId;

#define BITS_MOD_ALL ~(1<<MOD_MAX)

#define BOT_JUMP_HEIGHT 45

#define MIN_COVER_MOVE_DIST 128

#undef INDEXENT
#define INDEXENT(iEdictNum) engine->PEntityOfEntIndex(iEdictNum)

#undef ENTINDEX
#define ENTINDEX(pEdict) engine->IndexOfEdict(pEdict)
/*
#define BUILDNAME "TF2/HL2DM/DOD:S"
#define BUILDVER "BETA"
#define BUILDNUM "365"*/
#define BOT_NAME "RCBot"
#ifdef __linux__
#define BOT_VER "TF2/HL2DM/DOD:S 1.04 META Linux 490-apg (merge) (BUILD " __DATE__ "-" __TIME__ ")" //bir3yk
#else
#define BOT_VER "TF2/HL2DM/DOD:S 1.04 META Win32 490-apg (merge) (BUILD " ## __DATE__ ## "-" ## __TIME__ ## ")"
#endif
#define BOT_NAME_VER "RCbot version"
#define BOT_VER_CVAR "rcbot_ver"
#define BOT_FOLDER "rcbot2"

typedef enum
{
	BOT_FUNC_FAIL = 0,
    BOT_FUNC_CONTINUE,
	BOT_FUNC_COMPLETE,
}eBotFuncState;

//////////////////////////////////
#define CONDITION_ENEMY_OBSCURED		1 // bit 0 - bot lost sight of enemy and can't see clearly
#define CONDITION_NO_WEAPON				2 // bit 1 - bot doesn't have a weapon
#define CONDITION_OUT_OF_AMMO			4 // bit 2 - bot has no ammo
#define CONDITION_SEE_CUR_ENEMY			8 // bit 3 - bot can see current enemy
#define CONDITION_ENEMY_DEAD			16 // bit 4 - bot s enemy is dead
#define CONDITION_SEE_WAYPOINT			32 // bit 5 - bot can see the current waypoint
#define CONDITION_NEED_AMMO				64 // bit 6 - bot needs ammo (low)
#define CONDITION_NEED_HEALTH			128 // bit 7 - bot needs health
#define CONDITION_SEE_LOOK_VECTOR		256 // bit 8 - bot can see his 'look' vector
#define CONDITION_POINT_CAPTURED		512 // bit 9 a point has been captured recently
#define CONDITION_PUSH					1024 // bit 10 - bots are more likely to attack and stop sniping etc
#define CONDITION_LIFT					2048 // bit 11 - bot is on a lift
#define CONDITION_SEE_HEAL				4096 // bit 12 - medic bot (tf2) can see his player he wants to heal
#define CONDITION_SEE_PLAYERTOHELP		4096 // same thing as heal for other mods
#define CONDITION_SEE_LAST_ENEMY_POS	8192 // bit 13 - bots can see the last place they saw an enemy
#define CONDITION_CHANGED				16384 // bit 14 - bots want to change their course of action
#define CONDITION_COVERT				32768 // bit 15 - bots are more sensitive to enemies and more likely to take alternate paths
#define CONDITION_RUN					65536 // bit 16 - bots have to run e.g. there is a grenade nearby
#define CONDITION_GREN					131072 // bit 17 - bots will be more likely to throw a grenade
#define CONDITION_NEED_BOMB				262144 // bit 18 - bot needs a bomb for dod:s bomb maps
#define CONDITION_SEE_ENEMY_HEAD		524288 // bit 19 - bot can aim for a headshot
#define CONDITION_PRONE					1048576 // bit 20 - bot needs to go prone (lie down)
#define CONDITION_PARANOID				2097152 // bit 21 - bot is paranoid of spies or unknown enemy
#define CONDITION_SEE_SQUAD_LEADER		4194304 // bit 22 - bot can see his leader
#define CONDITION_SQUAD_LEADER_DEAD		8388608 // bit 23 - bots leader is dead
#define CONDITION_SQUAD_LEADER_INRANGE	16777216 // bit 24 - bots leader is in range
#define CONDITION_SQUAD_IDLE			33554432 // bit 25 - bots squad isn't doing anything fun
#define CONDITION_DEFENSIVE				67108864 // bit 26 - bot leader told me to defend
#define CONDITION_BUILDING_SAPPED		134217728 // bit 27 - one of engineers buildings sapped
#define CONDITION_SEE_ENEMY_GROUND		268435456 // bit 28 - can see enemy ground so aim for it if i have explosive
#define CONDITION_MAX					CONDITION_SEE_ENEMY_GROUND
#define CONDITION_MAX_BITS				28

////////////////////////
#define BLAST_RADIUS 200
///////////////////////
typedef enum 
{
	STATE_IDLE = 0,
	STATE_RUNNING,
	STATE_FAIL,
	STATE_COMPLETE
}eTaskState;
////////////////////

#endif