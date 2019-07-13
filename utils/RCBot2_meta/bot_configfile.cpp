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
 *///=================================================================================//

/// NOT WORKING

#include <stdio.h>
#include <time.h>

//#include "cbase.h"
//#include "baseentity.h"
#include "filesystem.h"
#include "interface.h"
#include "engine/iserverplugin.h"
#include "iplayerinfo.h"

#ifdef __linux__
#include "shake.h"    //bir3yk
#endif

#include "IEngineTrace.h" // for traceline functions
#include "tier2/tier2.h"
#include "IEffects.h"
#include "vplane.h"
#include "eiface.h"
#include "igameevents.h"
#include "icvar.h"
//#include "iconvar.h"
#include "convar.h"
#include "Color.h"
#include "ndebugoverlay.h"
#include "server_class.h"
#include "time.h"

#include "bot.h"
#include "bot_commands.h"
#include "bot_client.h"
#include "bot_globals.h"
#include "bot_accessclient.h"
#include "bot_waypoint_visibility.h" // for initializing table
#include "bot_event.h"
#include "bot_profile.h"
#include "bot_weapons.h"
#include "bot_mods.h"
#include "bot_profiling.h"
#include "vstdlib/random.h" // for random  seed 

#include "bot_wpt_dist.h"

#include "bot_configfile.h"
#include "bot_strings.h"

#include "bot_fortress.h"

vector <char *> CBotConfigFile::m_Commands;
unsigned int CBotConfigFile::m_iCmd = 0; // current command (time delayed)
float CBotConfigFile::m_fNextCommandTime = 0.0f;

// 
bot_util_t CRCBotTF2UtilFile::m_fUtils[UTIL_TYPE_MAX][BOT_UTIL_MAX][9];

void CBotConfigFile :: load ()
{
	char filename[512];
	char line[256];
	//int len;
	m_Commands.clear();

	CBotGlobals::buildFileName(filename,"config",BOT_CONFIG_FOLDER,BOT_CONFIG_EXTENSION);

	FILE *fp = CBotGlobals::openFile(filename,"r");

	if ( !fp )
	{
		CBotGlobals::botMessage(NULL,0,"config file not found");
		return;
	}

	while ( fgets(line,255,fp) != NULL )
	{
		if ( line[0] == '#' )
			continue;

		size_t len = strlen(line);

		if (line[len-1] == '\n') {
			line[--len] = '\0';
		}

		if (line[len-1] == '\r') {
			line[--len] = '\0';
		}

		CBotGlobals::botMessage(NULL, 0, line);
		m_Commands.push_back(CStrings::getString(line));
	}

	fclose(fp);

}

void CBotConfigFile :: doNextCommand ()
{
	char cmd[64] = {0};

	if ( (m_fNextCommandTime < engine->Time()) && (m_iCmd < m_Commands.size()) )
	{
		snprintf(cmd, sizeof(cmd), "%s\n", m_Commands[m_iCmd]);
		engine->ServerCommand(cmd);

		CBotGlobals::botMessage(NULL,0,"Bot Command '%s' executed",m_Commands[m_iCmd]);
		m_iCmd ++;
		m_fNextCommandTime = engine->Time() + 0.1f;
	}
}

void CBotConfigFile :: executeCommands ()
{
	char cmd[64] = {0};

	while ( (m_iCmd < m_Commands.size()) )
	{
		snprintf(cmd, sizeof(cmd), "%s\n", m_Commands[m_iCmd]);
		engine->ServerCommand(cmd);

		CBotGlobals::botMessage(NULL,0,"Bot Command '%s' executed",m_Commands[m_iCmd]);
		m_iCmd ++;
	}

	engine->ServerExecute();
}

void CRCBotTF2UtilFile :: init()
{
	short unsigned int i,j,k;

	for ( i = 0; i < UTIL_TYPE_MAX; i ++ )
	{
		for ( j = 0; j < BOT_UTIL_MAX; j ++ )
		{
			for ( k = 0; k < 9; k ++ )
			{
				m_fUtils[i][j][k].min = 0;
				m_fUtils[i][j][k].max = 0;
			}
		}
	}
}

void CRCBotTF2UtilFile :: addUtilPerturbation (const eBotAction iAction, const eTF2UtilType iUtil, float fUtility[9][2])
{
	short unsigned int i;

	for ( i = 0; i < 9; i ++ )
	{
		m_fUtils[iUtil][iAction][i].min = fUtility[i][0];
		m_fUtils[iUtil][iAction][i].max = fUtility[i][1];
	}
}

void CRCBotTF2UtilFile :: loadConfig()
{
	 eTF2UtilType iFile;
	 char szFullFilename[512];
	 char szFilename[64];
	 char line[256];
	 FILE *fp;

	 init();

	 for ( iFile = BOT_ATT_UTIL; iFile < UTIL_TYPE_MAX; iFile = static_cast<eTF2UtilType>(static_cast<int>(iFile) + 1) )
	 {
		 if ( iFile == BOT_ATT_UTIL )
		 {
			sprintf(szFilename,"attack_util.csv");
		 }
		 else
		 {
			sprintf(szFilename,"normal_util.csv");
		}

		CBotGlobals::buildFileName(szFullFilename,szFilename,BOT_CONFIG_FOLDER);
		fp = CBotGlobals::openFile(szFullFilename,"r");

		if ( fp )
		{
			eBotAction iUtil = static_cast<eBotAction>(0);

			while ( fgets(line,255,fp) != NULL )
			{
				float iClassList[TF_CLASS_MAX][2];
				char utiltype[64];

				if ( line[0] == 'B' && line[1] == 'O' && 
					 line[2] == 'T' && line[3] == '_') // OK
				{

					// Format:    U, 1, 2, 3, 4, 5, 6, 7, 8, 9
					//                
					//               s  s  s  d  m  h  p  s  e
					//               c  n  o  e  e  w  y  p  n
					//               o  i  l  m  d  g  r  y  g
					// 
					
					if ( sscanf(line,"%[^,],%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n",utiltype,
						&(iClassList[0][0]),&(iClassList[0][1]),
						&(iClassList[1][0]),&(iClassList[1][1]),
						&(iClassList[2][0]),&(iClassList[2][1]),
						&(iClassList[3][0]),&(iClassList[3][1]),
						&(iClassList[4][0]),&(iClassList[4][1]),
						&(iClassList[5][0]),&(iClassList[5][1]),
						&(iClassList[6][0]),&(iClassList[6][1]),
						&(iClassList[7][0]),&(iClassList[7][1]),
						&(iClassList[8][0]),&(iClassList[8][1])) )
					{

						addUtilPerturbation(iUtil,iFile,iClassList);

						iUtil = static_cast<eBotAction>((int)iUtil + 1);

						if ( iUtil >= BOT_UTIL_MAX )
							break;

					}

				}
			}

			fclose(fp);
		}
	 }

}
