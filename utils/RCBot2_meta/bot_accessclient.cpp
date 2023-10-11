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
#include "bot.h"
#include "bot_strings.h"
#include "bot_accessclient.h"
#include "bot_globals.h"

#include "rcbot/logging.h"

#include <cstring>
#include <vector>
///////////

std::vector<CAccessClient*> CAccessClients :: m_Clients;

///////////

CAccessClient :: CAccessClient( const char *szSteamID, int iAccessLevel )
{
	m_iAccessLevel = iAccessLevel;
	m_szSteamID = CStrings::getString(szSteamID);
}

bool CAccessClient :: forBot () const
{
	return isForSteamID("BOT");
}

bool CAccessClient :: isForSteamID ( const char *szSteamID ) const
{
	logger->Log(LogLevel::DEBUG, "AccessClient: '%s','%s'", m_szSteamID, szSteamID);
	return FStrEq(m_szSteamID,szSteamID);
}

void CAccessClient::save(std::fstream& fp) const
{
	fp << '"' << m_szSteamID << '"' << ":" << m_iAccessLevel << "\n";
}

void CAccessClient :: giveAccessToClient ( CClient *pClient ) const
{
	// notify player
	if ( !forBot() )
		CBotGlobals::botMessage(pClient->getPlayer(),0,"%s authenticated for bot commands",pClient->getName());
	// notify server
	logger->Log(LogLevel::INFO, "%s authenticated for bot commands", pClient->getName());

	pClient->setAccessLevel(m_iAccessLevel);
}

//////////////

void CAccessClients :: showUsers ( edict_t *pEntity )
{
	CBotGlobals::botMessage(pEntity,0,"showing users...");

	if ( m_Clients.empty() )
		logger->Log(LogLevel::DEBUG, "showUsers() : No users to show");

	for ( unsigned int i = 0; i < m_Clients.size(); i ++ )
	{
		const CAccessClient* pPlayer = m_Clients[i];

		const CClient* pClient = CClients::findClientBySteamID(pPlayer->getSteamID());
		
		if ( pClient )
			CBotGlobals::botMessage(pEntity,0,"[ID: %s]/[AL: %d] (currently playing as : %s)\n",pPlayer->getSteamID(),pPlayer->getAccessLevel(),pClient->getName());
		else
			CBotGlobals::botMessage(pEntity,0,"[ID: %s]/[AL: %d]\n",pPlayer->getSteamID(),pPlayer->getAccessLevel());

	}	
}

void CAccessClients :: freeMemory ()
{
	for ( unsigned int i = 0; i < m_Clients.size(); i ++ )
	{
		delete m_Clients[i];
		m_Clients[i] = nullptr;
	}

	m_Clients.clear();
}

void CAccessClients :: load ()
{
	char filename[1024];
	
	CBotGlobals::buildFileName(filename,BOT_ACCESS_CLIENT_FILE,BOT_CONFIG_FOLDER,BOT_CONFIG_EXTENSION);

	std::fstream fp = CBotGlobals::openFile(filename, std::fstream::in);

	if ( fp )
	{
		char buffer[256];

		char szSteamID[32];

		int iLine = 0;

		while (fp.getline(buffer, 255))
		{
			iLine++;

			buffer[255] = 0;

			if ( buffer[0] == 0 )
				continue;
			if ( buffer[0] == '\n' )
				continue;
			if ( buffer[0] == '\r' )
				continue;
			if ( buffer[0] == '#' )
				continue;

			const int len = std::strlen(buffer);

			int i = 0;

			while (( i < len ) && ((buffer[i] == '\"') || (buffer[i] == ' ')))
				i++;

			int n = 0;

			// parse Steam ID
			while ( (n<31) && (i < len) && (buffer[i] != '\"') )			
				szSteamID[n++] = buffer[i++];

			szSteamID[n] = 0;

			i++;

			while (( i < len ) && (buffer[i] == ' '))
				i++;

			if ( i == len )
			{
				logger->Log(LogLevel::WARN, "line %d invalid in access client config, missing access level", iLine);
				continue; // invalid
			}

			const int iAccess = atoi(&buffer[i]);

			// invalid
			if ( (szSteamID[0] == 0) || (szSteamID[0] == ' ' ) )
			{
				logger->Log(LogLevel::WARN, "line %d invalid in access client config, steam id invalid", iLine);
				continue;
			}
			if ( iAccess == 0 )
			{
				logger->Log(LogLevel::WARN, "line %d invalid in access client config, access level can't be 0", iLine);
				continue;
			}

			m_Clients.emplace_back(new CAccessClient(szSteamID,iAccess));
		}
	}
	else
	{
		logger->Log(LogLevel::ERROR, "Failed to open file '%s' for reading", filename);
	}
}

void CAccessClients :: save ()
{
	char filename[1024];
	
	CBotGlobals::buildFileName(filename,BOT_ACCESS_CLIENT_FILE,BOT_CONFIG_FOLDER,BOT_CONFIG_EXTENSION);

	std::fstream fp = CBotGlobals::openFile(filename, std::fstream::out);

	if ( fp )
	{
		for ( unsigned int i = 0; i < m_Clients.size(); i ++ )
		{
			m_Clients[i]->save(fp);
		}
	}
	else
	{
		logger->Log(LogLevel::ERROR, "Failed to open file '%s' for writing", filename);
	}
}

void CAccessClients :: checkClientAccess ( CClient *pClient )
{
	for ( unsigned int i = 0; i < m_Clients.size(); i ++ )
	{
		const CAccessClient *pAC = m_Clients[i];

		if ( pAC->isForSteamID(pClient->getSteamID()) )
			pAC->giveAccessToClient(pClient);
	}
}
