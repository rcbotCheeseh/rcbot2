#include "bot.h"
#include "bot_coop.h"
#include "bot_buttons.h"
#include "bot_globals.h"
#include "bot_profile.h"

#include "vstdlib/random.h" // for random functions


void CBotCoop :: modThink ()
{
	// find enemies and health stations / objectives etc
}

bool CBotCoop :: isEnemy ( edict_t *pEdict,bool bCheckWeapons )
{
	const char *classname;

	if ( ENTINDEX(pEdict) == 0 ) 
		return false;

	// no shooting players
	if ( ENTINDEX(pEdict) <= CBotGlobals::maxClients() )
	{
		return false;
	}

	classname = pEdict->GetClassName();

	if ( strncmp(classname,"npc_",4) == 0 )
	{
		if ( !strcmp(classname,"npc_antlionguard") || !strcmp(classname,"npc_citizen") || 
			 !strcmp(classname,"npc_barney") || !strcmp(classname,"npc_kliener") || !strcmp(classname,"npc_alyx") )
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

