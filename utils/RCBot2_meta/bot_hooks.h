#ifndef __BOT_HOOKS_H__
#define __BOT_HOOKS_H__

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#endif

#ifdef GetClassName
#undef GetClassName
#endif

//Required for RCBot2? [APG]RoboCop[CL]
#include <iserverentity.h>
#include <usercmd.h>
#include "bot_fortress.h"
#include <edict.h>

DWORD VirtualTableHook( DWORD* pdwNewInterface, int vtable, DWORD newInterface );

extern DWORD* player_vtable;
extern void (CBaseEntity::*pPlayerRunCommand)(CUserCmd*, IMoveHelper*);
extern CBaseEntity* (CBaseEntity::*TF2GiveNamedItem)(char const*, int, CEconItemView*, bool);
extern CBaseEntity* (CBaseEntity::*TF2EquipWearable)(CBaseEntity*);
// PlayerRunCommmand Hook 
// Some Mods have their own puppet bots that run around and override RCBOT if this is not here
// this function overrides the puppet bots movements
#ifdef __linux__
void FASTCALL nPlayerRunCommand( CBaseEntity *_this, CUserCmd* pCmd, IMoveHelper* pMoveHelper);
#else
void __fastcall nPlayerRunCommand( CBaseEntity *_this, void *unused, CUserCmd* pCmd, IMoveHelper* pMoveHelper);
#endif

#ifdef __linux__
CBaseEntity * FASTCALL nTF2GiveNamedItem( CBaseEntity *_this, void *punused, const char *name, int subtype, CEconItemView *cscript, bool b );
#else
CBaseEntity * __fastcall nTF2GiveNamedItem( CBaseEntity *_this, void *punused, const char *name, int subtype, CEconItemView *cscript, bool b );
#endif

bool UTIL_TF2EquipHat ( edict_t *pEdict, CTF2Loadout *pHat, void *vTable, void *vTableAttributes );
CTF2Loadout *UTIL_TF2EquipRandomHat ( edict_t *pEdict, void *vTable, void *vTableAttributes );

void HookGiveNamedItem ( edict_t *edict );

void UnhookGiveNamedItem ();

//----------------------------------
// begin hook
void HookPlayerRunCommand ( edict_t *edict );
// end hook
void UnhookPlayerRunCommand ();

#endif