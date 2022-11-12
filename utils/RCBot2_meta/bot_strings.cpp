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
#include <cstdlib>
#include <cstring>

#include "bot.h"
#include "bot_strings.h"
#include <vector>    //bir3yk

std::vector<char *> CStrings::m_Strings[MAX_STRINGS_HASH];

CStrings :: CStrings ()
{
	return;
}

void CStrings :: freeAllMemory()
{
	// clear strings 
	for ( int i = 0; i < MAX_STRINGS_HASH; i ++ )
	{
		for ( unsigned int j = 0; j < m_Strings[i].size(); j ++ )
		{
			const char* pszFree = m_Strings[i][j];

			if ( pszFree )
				delete pszFree;

			m_Strings[i][j] = nullptr;
		}

		m_Strings[i].clear();
	}
}

// Either : 1 . Return the existing string or 2 . make a new string and return it.
char *CStrings :: getString ( const char *szString )
{
	if ( szString == nullptr)
		return nullptr;

	const unsigned short int iHash = szString[0]%MAX_STRINGS_HASH;
	
	for ( unsigned int i = 0; i < m_Strings[iHash].size(); i ++ )
	{
		char *szCompString = m_Strings[iHash][i];

		// check if pointers match first
		if ( szCompString == szString )
			return szCompString;

		// if not do a full string comparison
		if ( FStrEq(szString,szCompString) )
			return szCompString;
	}

	const unsigned int len = strlen(szString);

	char *szNew = new char[len+1];

	strcpy(szNew,szString);

	szNew[len] = 0;

	m_Strings[iHash].emplace_back(szNew);

    return szNew;
}