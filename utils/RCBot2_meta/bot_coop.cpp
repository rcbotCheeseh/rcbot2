#include "bot.h"
#include "bot_coop.h"
#include "bot_buttons.h"
#include "bot_globals.h"
#include "bot_profile.h"

#include "vstdlib/random.h" // for random functions

#include <cstring>

void CBotCoop :: modThink ()
{
	// find enemies and health stations / objectives etc
}

bool CBotCoop :: isEnemy ( edict_t *pEdict,bool bCheckWeapons )
{
	if ( ENTINDEX(pEdict) == 0 ) 
		return false;

	// no shooting players
	if ( ENTINDEX(pEdict) <= CBotGlobals::maxClients() )
	{
		return false;
	}

	const char* classname = pEdict->GetClassName();

	if ( std::strncmp(classname,"npc_",4) == 0 )
	{
		if ( !std::strcmp(classname,"npc_antlionguard") || !std::strcmp(classname,"npc_citizen") || 
			 !std::strcmp(classname,"npc_barney") || !std::strcmp(classname,"npc_kliener") || !std::strcmp(classname,"npc_alyx") )
		{
			return false; // ally
		}

		return true;
	}

	return false;
}

bool CBotCoop :: startGame ()
{
	return true;
}

