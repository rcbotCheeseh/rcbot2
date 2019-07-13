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

#include <vector>
using namespace std;
///////////

vector<CAccessClient*> CAccessClients :: m_Clients;

///////////

CAccessClient :: CAccessClient( char *szSteamId, const int iAccessLevel )
{
	m_iAccessLevel = iAccessLevel;
	m_szSteamId = CStrings::getString(szSteamId);
}

bool CAccessClient :: forBot ()
{
	return isForSteamId("BOT");
}

bool CAccessClient :: isForSteamId ( const char *szSteamId )
{
	CBotGlobals::botMessage(NULL, 0, "AccessClient: '%s','%s'", m_szSteamId, szSteamId);
	return FStrEq(m_szSteamId,szSteamId);
}

void CAccessClient :: save ( FILE *fp )
{
	fprintf(fp,"\"%s\":%d\n",m_szSteamId,m_iAccessLevel);
}

void CAccessClient :: giveAccessToClient ( CClient *pClient )
{
	// notify player
	if ( !forBot() )
		CBotGlobals::botMessage(pClient->getPlayer(),0,"%s authenticated for bot commands",pClient->getName());
	// notify server
	CBotGlobals::botMessage(NULL,0,"%s authenticated for bot commands",pClient->getName());

	pClient->setAccessLevel(m_iAccessLevel);
}

//////////////

void CAccessClients :: showUsers ( edict_t *pEntity )
{
	CAccessClient *pPlayer;
	CClient *pClient;

	CBotGlobals::botMessage(pEntity,0,"showing users...");

	if ( m_Clients.empty() )
		CBotGlobals::botMessage(NULL,0,"showUsers() : No users to show");

	for ( unsigned int i = 0; i < m_Clients.size(); i ++ )
	{
		pPlayer = m_Clients[i];
		
		pClient = CClients::findClientBySteamID(pPlayer->getSteamID());
		
		if ( pClient )
			CBotGlobals::botMessage(pEntity,0,"[ID: %s]/[AL: %d] (currently playing as : %s)\n",pPlayer->getSteamID(),pPlayer->getAccessLevel(),pClient->getName());
		else
			CBotGlobals::botMessage(pEntity,0,"[ID: %s]/[AL: %d]\n",pPlayer->getSteamID(),pPlayer->getAccessLevel());

	}	
}

void CAccessClients :: createFile ()
{
	char filename[1024];
	
	CBotGlobals::buildFileName(filename,BOT_ACCESS_CLIENT_FILE,BOT_CONFIG_FOLDER,BOT_CONFIG_EXTENSION);

	FILE *fp = CBotGlobals::openFile(filename,"w");

	CBotGlobals::botMessage(NULL,0,"Making an accessclients.ini file for you... Edit it in %s",filename);

	if ( fp )
	{
		fprintf(fp,"# format is ");
		fprintf(fp,"# \"<STEAM ID>\" <access level>\n");
		fprintf(fp,"# see http://rcbot.bots-united.com/accesslev.htm for access\n");
		fprintf(fp,"# levels\n");
		fprintf(fp,"#\n");
		fprintf(fp,"# example:\n");
		fprintf(fp,"#\n");
		fprintf(fp,"# \"STEAM_0:123456789\" 63\n");
		fprintf(fp,"# don't put one of '#' these before a line you want to be read \n");
		fprintf(fp,"# by the bot!\n");
		fprintf(fp,"# \n");
		fclose(fp);
	}
	else
		CBotGlobals::botMessage(NULL,0,"Error! Couldn't create config file %s",filename);
}

void CAccessClients :: freeMemory ()
{
	for ( unsigned int i = 0; i < m_Clients.size(); i ++ )
	{
		delete m_Clients[i];
		m_Clients[i] = NULL;
	}

	m_Clients.clear();
}

void CAccessClients :: load ()
{
	char filename[1024];
	
	CBotGlobals::buildFileName(filename,BOT_ACCESS_CLIENT_FILE,BOT_CONFIG_FOLDER,BOT_CONFIG_EXTENSION);

	FILE *fp = CBotGlobals::openFile(filename,"r");

	if ( fp )
	{
		char buffer[256];

		char szSteamId[32];
		int iAccess;

		int i;
		int len;
		int n;

		int iLine = 0;

		while ( fgets(buffer,255,fp) != NULL )
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

			len = strlen(buffer);

			i = 0;

			while (( i < len ) && ((buffer[i] == '\"') || (buffer[i] == ' ')))
				i++;

			n = 0;

			// parse Steam ID
			while ( (n<31) && (i < len) && (buffer[i] != '\"') )			
				szSteamId[n++] = buffer[i++];

			szSteamId[n] = 0;

			i++;

			while (( i < len ) && (buffer[i] == ' '))
				i++;

			if ( i == len )
			{
				CBotGlobals::botMessage(NULL,0,"line %d invalid in access client config, missing access level",iLine);
				continue; // invalid
			}

			iAccess = atoi(&buffer[i]);

			// invalid
			if ( (szSteamId[0] == 0) || (szSteamId[0] == ' ' ) )
			{
				CBotGlobals::botMessage(NULL,0,"line %d invalid in access client config, steam id invalid",iLine);
				continue;
			}
			if ( iAccess == 0 )
			{
				CBotGlobals::botMessage(NULL,0,"line %d invalid in access client config, access level can't be 0",iLine);
				continue;
			}

			m_Clients.push_back(new CAccessClient(szSteamId,iAccess));
		}

		fclose(fp);
	}
	else
		CAccessClients :: createFile();
}

void CAccessClients :: save ()
{
	char filename[1024];
	
	CBotGlobals::buildFileName(filename,BOT_ACCESS_CLIENT_FILE,BOT_CONFIG_FOLDER,BOT_CONFIG_EXTENSION);

	FILE *fp = CBotGlobals::openFile(filename,"w");

	if ( fp )
	{
		for ( unsigned int i = 0; i < m_Clients.size(); i ++ )
		{
			m_Clients[i]->save(fp);
		}

		fclose(fp);
	}
}

void CAccessClients :: checkClientAccess ( CClient *pClient )
{
	for ( unsigned int i = 0; i < m_Clients.size(); i ++ )
	{
		CAccessClient *pAC = m_Clients[i];

		if ( pAC->isForSteamId(pClient->getSteamID()) )
			pAC->giveAccessToClient(pClient);
	}
}
