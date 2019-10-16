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
 //====================================================================================//
 //
 // HPB_bot2_usercmd.cpp - bot source code file (Copyright 2004, Jeffrey "botman" Broome)
 //
 //====================================================================================//

#include <stdio.h>

#include "interface.h"
#include "filesystem.h"
#include "engine/iserverplugin.h"
#include "iplayerinfo.h"
#include "vplane.h"
#include "eiface.h"
#include "igameevents.h"

#ifdef __linux__
#include "shareddefs.h" //bir3yk
#endif

#include "usercmd.h"
#include "bitbuf.h"

#include "bot.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void WriteUsercmd(bf_write* buf, CUserCmd* cmd)
{
	buf->WriteOneBit(0);  // command_number

	buf->WriteOneBit(0);  // tick_count

	buf->WriteOneBit(1);
	buf->WriteBitAngle(cmd->viewangles.x, 16);  // viewangles[0]
	buf->WriteOneBit(1);
	buf->WriteBitAngle(cmd->viewangles.y, 16);  // viewangles[1]
	buf->WriteOneBit(1);
	buf->WriteBitAngle(cmd->viewangles.z, 8);  // viewangles[2]

	buf->WriteOneBit(1);
	buf->WriteSBitLong(cmd->forwardmove, 16);

	buf->WriteOneBit(1);
	buf->WriteSBitLong(cmd->sidemove, 16);

	buf->WriteOneBit(1);
	buf->WriteSBitLong(cmd->upmove, 16);

	buf->WriteOneBit(1);
	buf->WriteUBitLong(cmd->buttons, 32);

	buf->WriteOneBit(1);
	buf->WriteUBitLong(cmd->impulse, 8);

	buf->WriteOneBit(1);
	buf->WriteUBitLong(cmd->weaponselect, MAX_EDICT_BITS);

	buf->WriteOneBit(0);  // weaponsubtype

	buf->WriteOneBit(0);  // mousedx
	buf->WriteOneBit(0);  // mousedy
}