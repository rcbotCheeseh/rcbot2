/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod (C)2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "rcbot/tf2/conditions.h"
#include "rcbot/entprops.h"
#include "rcbot/logging.h"

static CTF2Conditions s_tf2_conditions;
CTF2Conditions *tf2_conditions = &s_tf2_conditions;

/// @brief Checks if the player is in a specific condition
/// @param client Client/Player entity index to check
/// @param cond Condition number to check
/// @return TRUE if the given condition is active on the player
bool CTF2Conditions::TF2_IsPlayerInCondition(int client, TFCond cond)
{
	const int iCond = cond;

	char nPlayerCond1[] = "m_nPlayerCond";
	char nPlayerCond2[] = "_condition_bits";

	char nPlayerCondEx1[] = "m_nPlayerCondEx";
	char nPlayerCondEx2[] = "m_nPlayerCondEx2";
	char nPlayerCondEx3[] = "m_nPlayerCondEx3";
	char nPlayerCondEx4[] = "m_nPlayerCondEx4";

	switch (iCond / 32)
	{
		case 0:
		{
			const int bit = 1 << iCond;
			if ((entprops->GetEntProp(client, Prop_Send, nPlayerCond1) & bit) == bit)
			{
				return true;
			}

			if ((entprops->GetEntProp(client, Prop_Send, nPlayerCond2) & bit) == bit)
			{
				return true;
			}
			break;
		}
		case 1:
		{
			const int bit = (1 << (iCond - 32));
			if ((entprops->GetEntProp(client, Prop_Send, nPlayerCondEx1) & bit) == bit)
			{
				return true;
			}
			break;
		}
		case 2:
		{
			const int bit = (1 << (iCond - 64));
			if ((entprops->GetEntProp(client, Prop_Send, nPlayerCondEx2) & bit) == bit)
			{
				return true;
			}
			break;
		}
		case 3:
		{
			const int bit = (1 << (iCond - 96));
			if ((entprops->GetEntProp(client, Prop_Send, nPlayerCondEx3) & bit) == bit)
			{
				return true;
			}
			break;
		}
		case 4:
		{
			const int bit = (1 << (iCond - 128));
			if ((entprops->GetEntProp(client, Prop_Send, nPlayerCondEx4) & bit) == bit)
			{
				return true;
			}
			break;
		}
		default:
		{
			logger->Log(LogLevel::ERROR, "Invalid TFCond value %d", iCond);
			return false;
			//break;
		}
	}

	return false;
}
