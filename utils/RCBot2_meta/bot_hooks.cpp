

#include "filesystem.h"

#ifdef __linux__
#include "shake.h"    //bir3yk
#endif
#include "enginecallback.h"

#include "bot_const.h"
#include "bot.h"
#include "bot_fortress.h"
#include "bot_kv.h"
#include "bot_globals.h"
#include "bot_getprop.h"
#include "bot_weapons.h"
#include "bot_hooks.h"

extern IServerGameEnts *servergameents;

void *g_pVTable = NULL;
void *g_pVTable_Attributes = NULL;

// needed to call the original playerruncommand function
void (CBaseEntity::*pPlayerRunCommand)(CUserCmd*, IMoveHelper*) = 0x0;
CBaseEntity* (CBaseEntity::*TF2GiveNamedItem)(char const*, int, CEconItemView*, bool) = 0x0;
CBaseEntity* (CBaseEntity::*TF2EquipWearable)(CBaseEntity*) = 0x0;

//DWORD* pPlayerRunCommandHookedClass = 0;
DWORD* player_vtable = 0;

// linux:  Ted  http://rcbot.bots-united.com/forums/index.php?showuser=2257
// Needed to hook virtual tables and member functions
DWORD VirtualTableHook( DWORD* pdwNewInterface, int vtable, DWORD newInterface )
{
    DWORD dwOld, dwStor = 0x0;
#ifndef __linux
    VirtualProtect( &pdwNewInterface[vtable], 4, PAGE_EXECUTE_READWRITE, &dwOld );
#else
   // need page aligned address
    char *addr = reinterpret_cast<char *>(reinterpret_cast<DWORD>(&pdwNewInterface[vtable])
                      - reinterpret_cast<DWORD>(&pdwNewInterface[vtable])
                      % sysconf(_SC_PAGE_SIZE));
    int len = sizeof(DWORD) + reinterpret_cast<DWORD>(&pdwNewInterface[vtable])
      % sysconf(_SC_PAGE_SIZE);
    if (mprotect(addr, len, PROT_EXEC|PROT_READ|PROT_WRITE) == -1) {
      Warning("In VirtualTableHook while calling mprotect for write access: %s.\n",
          strerror(errno));
    } 
	else 
	{
#endif
    dwStor = pdwNewInterface[vtable];
    *(DWORD*)&pdwNewInterface[vtable] = newInterface;
#ifndef __linux
    VirtualProtect(&pdwNewInterface[vtable], 4, dwOld, &dwOld);
#else
		if (mprotect(addr, len, PROT_EXEC|PROT_READ) == -1) 
		{
		  Warning("In VirtualTableHook while calling mprotect to remove write access: %s.\n",
			  strerror(errno));
		}
    }
#endif

    return dwStor;
}

// PlayerRunCommmand Hook 
// Some Mods have their own puppet bots that run around and override RCBOT if this is not here
// this function overrides the puppet bots movements
#ifdef __linux__
void FASTCALL nPlayerRunCommand( CBaseEntity *_this, CUserCmd* pCmd, IMoveHelper* pMoveHelper)
#else
void __fastcall nPlayerRunCommand( CBaseEntity *_this, void *unused, CUserCmd* pCmd, IMoveHelper* pMoveHelper)
#endif
{
	edict_t *pEdict = servergameents->BaseEntityToEdict(_this);

	static CBot *pBot;

	pBot = CBots::getBotPointer(pEdict);
	
	if ( pBot )
	{
		static CBotCmd *cmd;
		
		cmd = pBot->getUserCMD();

		// put the bot's commands into this move frame
		pCmd->buttons = cmd->buttons;
		pCmd->forwardmove = cmd->forwardmove;
		pCmd->impulse = cmd->impulse;
		pCmd->sidemove = cmd->sidemove;
		pCmd->upmove = cmd->upmove;
		pCmd->viewangles = cmd->viewangles;
		pCmd->weaponselect = cmd->weaponselect;
		pCmd->weaponsubtype = cmd->weaponsubtype;
		pCmd->tick_count = cmd->tick_count;
		pCmd->command_number = cmd->command_number;
	}
	//try
	//{
		(*_this.*pPlayerRunCommand)(pCmd, pMoveHelper);
	/*}
	catch(...)
	{
		UnhookPlayerRunCommand();
		UnhookGiveNamedItem();
		Error("RCBOT:  nPlayerRunCommand Failed. bad offset?");
		return;
	}*/
}

#ifdef __linux__
CBaseEntity * FASTCALL nTF2GiveNamedItem( CBaseEntity *_this, void *punused, const char *name, int subtype, CEconItemView *cscript, bool b )
#else
CBaseEntity * __fastcall nTF2GiveNamedItem( CBaseEntity *_this, void *punused, const char *name, int subtype, CEconItemView *cscript, bool b )
#endif
{
	const char *pszOverrideName = name;
	edict_t *pEdict = servergameents->BaseEntityToEdict(_this);
	CTF2Loadout *weap = NULL;
	CBot *pBot = NULL;
	CBaseEntity *pAdded = NULL;
	extern ConVar rcbot_customloadouts;
	extern ConVar rcbot_force_generation;
	//CEconItemView newItem;

	if ( cscript && (g_pVTable == NULL) )
	{
		g_pVTable = cscript->m_pVTable;	
		g_pVTable_Attributes = cscript->m_pVTable_Attributes;
	}
	
	// Custom Loadouts are bugged right now? - pongo1231
	if ( /*rcbot_customloadouts.GetBool()*/ false && (cscript != NULL) && ((pBot=CBots::getBotPointer(pEdict)) != NULL) )
	{		
		int iclass = CClassInterface::getTF2Class(pEdict);

		// prevent any weird things from happening
		// only change weapons if this class is the class I want to be and I just spawned
		if ( pBot->isDesiredClass(iclass) && pBot->recentlySpawned(5.0f) )
		{
			weap = CTeamFortress2Mod::findRandomWeaponLoadOut(iclass,name);
			// this is an RC bot
			if ( weap != NULL )
			{
				weap->getScript(cscript);

				pszOverrideName = weap->m_pszClassname;

				if ( weap->m_Attributes.size() > 0 )
				{
					if ( strcmp( pszOverrideName, "saxxy" ) )
					{
						if ( b == false )
							b = rcbot_force_generation.GetBool();
					}
				}
			}
		}
	}

	//try
	//{
		pAdded = (*_this.*TF2GiveNamedItem)(pszOverrideName,subtype,cscript,b);
	//}

	/*catch ( ... )
	{
		UnhookGiveNamedItem();
		UnhookPlayerRunCommand();
		Error("RCBOT: nTF2GiveNamedItem Failed. bad offset?");
	}*/

	if ( pBot && cscript )
		((CBotTF2*)pBot)->PostGiveNamedItem(pAdded,weap,cscript);		

	return pAdded;

}

void UTIL_TF2EquipStockWeapon ( edict_t *pEdict, int islot, void *vTable, void *vTableAttributes )
{
	int iClass = CClassInterface::getTF2Class(pEdict);
	/*
	switch ( iClass )
	{

	}*/
}

bool UTIL_TF2EquipHat ( edict_t *pEdict, CTF2Loadout *pHat, void *vTable, void *vTableAttributes )
{	
	extern ConVar rcbot_force_generation;

	CEconItemView script;
	memset(&script,0,sizeof(CEconItemView));

	if ( pHat && TF2GiveNamedItem && TF2EquipWearable )
	{
		CBaseEntity *pEnt = servergameents->EdictToBaseEntity(pEdict);

		if ( pEnt )
		{
			CBaseEntity *added = NULL;

			
			script.m_pVTable = vTable;
			script.m_pVTable_Attributes = vTableAttributes;
			//pHat->applyAttributes(&script);
			pHat->getScript(&script);
			added = (*pEnt.*TF2GiveNamedItem)(pHat->m_pszClassname,0,&script,rcbot_force_generation.GetBool());

			if ( added )
			{
				(*pEnt.*TF2EquipWearable)(added);

				return true;
			}
		}
	}

	return false;
}

CTF2Loadout *UTIL_TF2EquipRandomHat ( edict_t *pEdict, void *vTable, void *vTableAttributes )
{
	extern ConVar rcbot_force_generation;
	CTF2Loadout *pHat = CTeamFortress2Mod::getRandomHat();

	if ( UTIL_TF2EquipHat(pEdict,pHat,vTable,vTableAttributes) )
	{
		return pHat;
	}

	return NULL;
}

void HookGiveNamedItem ( edict_t *edict )
{
	extern ConVar rcbot_givenameditem_offset;
	extern ConVar rcbot_equipwearable_offset;
	CBaseEntity *BasePlayer = servergameents->EdictToBaseEntity(edict);//(CBaseEntity *)(edict->GetUnknown()->GetBaseEntity());

	if((TF2GiveNamedItem == 0x0) && BasePlayer && CBotGlobals::isCurrentMod(MOD_TF2) )
	{
		int vtable = rcbot_givenameditem_offset.GetInt();

		DWORD *mem = ( DWORD* )*( DWORD* )BasePlayer;
		int offset = rcbot_equipwearable_offset.GetInt();
		*(DWORD*)&TF2EquipWearable = mem[offset];

		if ( vtable != 0 )
		{
	    // hook it
			if ( player_vtable == NULL )
				player_vtable = ( DWORD* )*( DWORD* )BasePlayer;

			*(DWORD*)&TF2GiveNamedItem = VirtualTableHook( player_vtable, vtable, ( DWORD )nTF2GiveNamedItem );
			
		}
	}
	
}

void UnhookGiveNamedItem ()
{
	extern ConVar rcbot_givenameditem_offset;

	if ( TF2GiveNamedItem && player_vtable )
	{
		int vtable = rcbot_givenameditem_offset.GetInt();

		if ( vtable != 0 )
		{
			/* linux and windows offsets are now separate in hooksinfo.ini
	#ifndef WIN32
			++vtable;
	#endif
	       */
			VirtualTableHook( player_vtable, vtable, *(DWORD*)&TF2GiveNamedItem );
			TF2GiveNamedItem = 0x0;
			//GiveNamedItemHookedClass = 0x0;
		}
	}
}

//----------------------------------
// begin hook
void HookPlayerRunCommand ( edict_t *edict )
{
	extern ConVar rcbot_runplayercmd_dods;
	extern ConVar rcbot_runplayercmd_tf2;

	if ( CBots::controlBots() ) //rcbot_override.GetBool() )
	{
		CBaseEntity *BasePlayer = (CBaseEntity *)(edict->GetUnknown()->GetBaseEntity());

		if(BasePlayer)
		{
			int vtable = 0;

			if ( CBotGlobals::isCurrentMod(MOD_DOD) )
				vtable = rcbot_runplayercmd_dods.GetInt();
			else
				vtable = rcbot_runplayercmd_tf2.GetInt();

			if ( vtable != 0 )
			{
				/*
	#ifndef WIN32
				++vtable;
	#endif*/
	       // hook it
				if ( pPlayerRunCommand == 0x0 )
				{
					player_vtable = ( DWORD* )*( DWORD* )BasePlayer;
					*(DWORD*)&pPlayerRunCommand = VirtualTableHook( player_vtable, vtable, ( DWORD )nPlayerRunCommand );
				}
			}
		}
	}
}

// end hook
void UnhookPlayerRunCommand ()
{
	extern ConVar rcbot_runplayercmd_dods;
	extern ConVar rcbot_runplayercmd_tf2;

	if ( pPlayerRunCommand && player_vtable )
	{
		int vtable = 0;

		if ( CBotGlobals::isCurrentMod(MOD_DOD) )
			vtable = rcbot_runplayercmd_dods.GetInt();
		else
			vtable = rcbot_runplayercmd_tf2.GetInt();

		if ( vtable != 0 )
		{
			/*
	#ifndef WIN32
			++vtable;
	#endif
	       */
			VirtualTableHook( player_vtable, vtable, *(DWORD*)&pPlayerRunCommand );
			//pPlayerRunCommandHookedClass = NULL;
			pPlayerRunCommand = NULL;
		}
	}
}
