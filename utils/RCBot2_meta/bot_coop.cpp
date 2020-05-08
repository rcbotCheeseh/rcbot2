#include "bot.h"
#include "bot_coop.h"
#include "bot_buttons.h"
#include "bot_globals.h"
#include "bot_profile.h"

#include "vstdlib/random.h" // for random functions

void CBotCoop::modThink()
{
	// find enemies and health stations / objectives etc
}

bool CBotCoop::isEnemy(edict_t* pEdict, bool bCheckWeapons)
{
	extern ConVar rcbot_notarget;

	if (ENTINDEX(pEdict) == 0)
		return false;
	// if no target on - listen sever player is a non target
	if (rcbot_notarget.GetBool())
		return false;

	// not myself
	if (pEdict == m_pEdict)
		return false;
	// no shooting players
	if (ENTINDEX(pEdict) <= CBotGlobals::maxClients())
	{
		return false;
	}

	const char* szclassname = pEdict->GetClassName();

	// todo: filter NPCs
	if (strncmp(szclassname, "npc_", 4) == 0)
	{
		CClients::clientDebugMsg(this, BOT_DEBUG_EDICTS, "IsEnemy found NPC: %s", szclassname);
		return true;
	}

	return false;
}

bool CBotCoop::startGame()
{
	return true;
}