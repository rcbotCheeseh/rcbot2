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

#ifndef __BOT_ACCESS_CLIENT_H__
#define __BOT_ACCESS_CLIENT_H__

#include <vector>
using namespace std;

#include "bot_client.h"

class CAccessClient
{
public:
	CAccessClient(char* szSteamId, int iAccessLevel);

	bool isForSteamId(const char* szSteamId);

	void giveAccessToClient(CClient* pClient);

	void save(FILE* fp);

	bool forBot();

	inline char* getSteamID() { return m_szSteamId; }

	inline int getAccessLevel() { return m_iAccessLevel; }
private:
	char* m_szSteamId;
	int m_iAccessLevel;
};

class CAccessClients
{
public:
	static void freeMemory();

	static void showUsers(edict_t* pEntity);

	static void load();

	static void save();

	static void checkClientAccess(CClient* pClient);

	static void createFile(); // create file if it doesn't exist (and mention of creation etc)
private:
	static vector<CAccessClient*> m_Clients;
};

#endif
