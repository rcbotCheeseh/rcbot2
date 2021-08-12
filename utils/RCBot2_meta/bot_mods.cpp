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
#include "engine_wrappers.h"

#include "bot.h"

#include "in_buttons.h"

#include "bot_mods.h"
#include "bot_globals.h"
#include "bot_fortress.h"
#include "bot_weapons.h"
#include "bot_configfile.h"
#include "bot_getprop.h"
#include "bot_dod_bot.h"
#include "bot_navigator.h"
#include "bot_waypoint.h"
#include "bot_waypoint_locations.h"
#include "bot_perceptron.h"

#include "logging.h"

std::vector<edict_wpt_pair_t> CHalfLifeDeathmatchMod::m_LiftWaypoints;

void CBotMods :: parseFile ()
{
	char buffer[1024];
	char key[64];
	char val[256];

	eModId modtype;
	eBotType bottype;
	char gamefolder[256];
	char weaponlist[64];

	CBotGlobals::buildFileName(buffer,BOT_MOD_FILE,BOT_CONFIG_FOLDER,BOT_CONFIG_EXTENSION);

	FILE *fp = CBotGlobals::openFile(buffer,"r");

	CBotMod *curmod = NULL;

	if ( !fp )
	{
		createFile();
		fp = CBotGlobals::openFile(buffer,"r");
	}

	if ( !fp )
	{
		// ERROR!
		return;
	}

	while ( fgets(buffer,1023,fp) != NULL )
	{
		if ( buffer[0] == '#' )
			continue;

		unsigned int len = strlen(buffer);

		if ( len == 0 )
			continue;

		if ( buffer[len-1] == '\n' )
			buffer[--len] = 0;

		unsigned int i = 0;
		unsigned int j = 0;

		while ( i < len && buffer[i] != '=' )
		{
			if ( buffer[i] != ' ' )
				key[j++] = buffer[i];
			i++;
		}

		i++;

		key[j] = 0;

		j = 0;

		while ( i < len && buffer[i] != '\n' && buffer[i] != '\r' )
		{
			if ( j || buffer[i] != ' ' )
				val[j++] = buffer[i];
			i++;
		}

		val[j] = 0;

		if ( !strcmp(key,"mod") )
		{
			if ( curmod )
			{
				curmod->setup(gamefolder, modtype, bottype, weaponlist);
				m_Mods.push_back(curmod);
			}
			
			curmod = NULL;
			weaponlist[0] = 0;

			bottype = BOTTYPE_GENERIC;

			modtype = MOD_CUSTOM;

			if ( !strcmpi("CUSTOM",val) )
			{
				modtype = MOD_CUSTOM;
				curmod = new CBotMod();
			}
			else if ( !strcmpi("CSS",val) )
			{
				modtype = MOD_CSS;
				curmod = new CCounterStrikeSourceMod();
			}
			else if ( !strcmpi("HL1DM",val) )
			{
				modtype = MOD_HL1DMSRC;
				curmod = new CHLDMSourceMod();
			}
			else if ( !strcmpi("HL2DM",val) )
			{
				modtype = MOD_HLDM2;
				curmod = new CHalfLifeDeathmatchMod();
			}
			else if ( !strcmpi("FF",val) )
			{
				modtype = MOD_FF;
				curmod = new CFortressForeverMod();
			}
			else if ( !strcmpi("TF2",val) )
			{
				modtype = MOD_TF2;
				curmod = new CTeamFortress2Mod();
			}
			else if ( !strcmpi("SVENCOOP2",val) )
			{
				modtype = MOD_SVENCOOP2;
				curmod = new CBotMod();
			}
			else if ( !strcmpi("TIMCOOP",val) )
			{
				modtype = MOD_TIMCOOP;
				curmod = new CBotMod();
			}
			else if ( !strcmpi("NS2",val) )
			{
				modtype = MOD_NS2;
				curmod = new CBotMod();
			}
			else if ( !strcmpi("SYNERGY",val) )
			{
				modtype = MOD_SYNERGY;
				curmod = new CSynergyMod();
			}
			else if ( !strcmpi("DOD",val) )
			{
				modtype = MOD_DOD;
				curmod = new CDODMod();
			}
			else
				curmod = new CBotMod();
		}
		else if ( curmod && !strcmp(key,"bot") )
		{
			if ( !strcmpi("GENERIC",val) )
				bottype = BOTTYPE_GENERIC;
			else if ( !strcmpi("CSS",val) )
				bottype = BOTTYPE_CSS;
			else if ( !strcmpi("HL1DM",val) )
				bottype = BOTTYPE_HL1DM;
			else if ( !strcmpi("HL2DM",val) )
				bottype = BOTTYPE_HL2DM;
			else if ( !strcmpi("FF",val) )
				bottype = BOTTYPE_FF;
			else if ( !strcmpi("TF2",val) )
				bottype = BOTTYPE_TF2;
			else if ( !strcmpi("COOP",val) )
				bottype = BOTTYPE_COOP;
			else if ( !strcmpi("ZOMBIE",val) )
				bottype = BOTTYPE_ZOMBIE;
			else if ( !strcmpi("DOD",val) )
				bottype = BOTTYPE_DOD;
			else if ( !strcmpi("SYNERGY",val) )
				bottype = BOTTYPE_SYN;
		}
		else if ( curmod && !strcmpi(key,"gamedir") )
		{
			strncpy(gamefolder,val,255);
		}
		else if (curmod && !strcmpi(key, "weaponlist"))
		{
			strncpy(weaponlist, val, 63);
		}
	}

	if ( curmod )
	{
		curmod->setup(gamefolder, modtype, bottype, weaponlist);
		m_Mods.push_back(curmod);
	}

	fclose(fp);
}

void CBotMods :: createFile ()
{
	char filename[1024];

	CBotGlobals::buildFileName(filename,BOT_MOD_FILE,BOT_CONFIG_FOLDER,BOT_CONFIG_EXTENSION);

	FILE *fp = CBotGlobals::openFile(filename,"w");

	CBotGlobals::botMessage(NULL,0,"Making a %s.%s file for you... Edit it in '%s'",BOT_MOD_FILE,BOT_CONFIG_EXTENSION,filename);

	if ( fp )
	{
		fprintf(fp,"# EXAMPLE MOD FILE");
		fprintf(fp,"# valid mod types\n");
		fprintf(fp,"# ---------------\n");
		fprintf(fp,"# CSS\n");
		fprintf(fp,"# TF2\n");
		fprintf(fp,"# HL2DM\n");
		fprintf(fp,"# HL1DM\n");
		fprintf(fp,"# FF\n");
		fprintf(fp,"# SVENCOOP2\n");
		fprintf(fp,"# TIMCOOP\n");
		fprintf(fp,"# NS2\n");
		fprintf(fp,"# DOD (day of defeat source)\n");
		fprintf(fp,"#\n");
		fprintf(fp,"# valid bot types\n");
		fprintf(fp,"# ---------------\n");
		fprintf(fp,"# CSS\n");
		fprintf(fp,"# TF2\n");
		fprintf(fp,"# HL2DM\n");
		fprintf(fp,"# HL1DM\n");
		fprintf(fp,"# FF\n");
		fprintf(fp,"# COOP\n");
		fprintf(fp,"# ZOMBIE\n");
		fprintf(fp,"# DOD\n");
		fprintf(fp,"#\n");
		fprintf(fp,"# weaponlists are changeable in config / weapons.ini\n");
		fprintf(fp,"#\n");
		fprintf(fp,"#mod = CSS\n");
		fprintf(fp,"#steamdir = counter-strike source\n");
		fprintf(fp,"#gamedir = cstrike\n");
		fprintf(fp,"#bot = CSS\n");
		fprintf(fp,"#\n");
		fprintf(fp,"#mod = TF2\n");
		fprintf(fp,"#steamdir = teamfortress 2\n");
		fprintf(fp,"#gamedir = tf\n");
		fprintf(fp,"#bot = TF2\n");
		fprintf(fp,"#\n");
		fprintf(fp,"#mod = FF\n");
		fprintf(fp,"#steamdir = sourcemods\n");
		fprintf(fp,"#gamedir = ff\n");
		fprintf(fp,"#bot = FF\n");
		fprintf(fp,"#\n");
		fprintf(fp,"#mod = HL2DM\n");
		fprintf(fp,"#steamdir = half-life 2 deathmatch\n");
		fprintf(fp,"#gamedir = hl2mp\n");
		fprintf(fp,"#bot = HL2DM\n");
		fprintf(fp,"#\n");
		fprintf(fp,"#mod = HL1DM\n");
		fprintf(fp,"#steamdir = half-life 1 deathmatch\n");
		fprintf(fp,"#gamedir = hl1dm\n");
		fprintf(fp,"#bot = HL1DM\n");
		fprintf(fp,"#\n");
		fprintf(fp,"#mod = DOD\n");
		fprintf(fp,"#steamdir = orangebox\n");
		fprintf(fp,"#gamedir = dod\n");
		fprintf(fp,"#bot = DOD\n");
		fprintf(fp,"#weaponlist = DOD\n");
		fprintf(fp,"#\n");

		fclose(fp);
	}
	else
		logger->Log(LogLevel::ERROR, "Couldn't create config file %s", filename);
}

void CBotMods :: readMods()
{
	// TODO improve game detection
	// caxanga334: Better game detection required if we want to support multiple mods on the same engine (IE: SDK 2013)
	#if SOURCE_ENGINE == SE_TF2
		m_Mods.push_back(new CTeamFortress2Mod());
	#elif SOURCE_ENGINE == SE_DODS
		m_Mods.push_back(new CDODMod());
	#elif SOURCE_ENGINE == SE_CSS
		m_Mods.push_back(new CCounterStrikeSourceMod());
	#elif SOURCE_ENGINE == SE_HL2DM
		m_Mods.push_back(new CHalfLifeDeathmatchMod());
	#elif SOURCE_ENGINE == SE_SDK2013
		m_Mods.push_back(new CSynergyMod());
	#else

		m_Mods.push_back(new CFortressForeverMod());

		m_Mods.push_back(new CHLDMSourceMod());

		// Look for extra MODs

		parseFile();
	#endif
}

//////////////////////////////////////////////////////////////////////////////

void CBotMod :: setup ( const char *szModFolder, eModId iModId, eBotType iBotType, const char *szWeaponListName )
{
	m_szModFolder = CStrings::getString(szModFolder);
	m_iModId = iModId;
	m_iBotType = iBotType;

	if (szWeaponListName && *szWeaponListName )
		m_szWeaponListName = CStrings::getString(szWeaponListName);
}

/*CBot *CBotMod :: makeNewBots ()
{
	return NULL;
}*/

bool CBotMod :: isModFolder ( char *szModFolder )
{
	return FStrEq(m_szModFolder,szModFolder);
}

char *CBotMod :: getModFolder ()
{
	return m_szModFolder;
}

eModId CBotMod :: getModId ()
{
	return m_iModId;
}

//
// MOD LIST

std::vector<CBotMod*> CBotMods::m_Mods;

void CBotMods :: freeMemory ()
{
	for ( unsigned int i = 0; i < m_Mods.size(); i ++ )
	{
		m_Mods[i]->freeMemory();
		delete m_Mods[i];
		m_Mods[i] = NULL;
	}

	m_Mods.clear();
}

CBotMod *CBotMods :: getMod ( char *szModFolder )
{
	for ( unsigned int i = 0; i < m_Mods.size(); i ++ )
	{
		if ( m_Mods[i]->isModFolder(szModFolder) )
		{
			logger->Log(LogLevel::INFO, "HL2 MOD ID %d (Game Folder = %s) FOUND", m_Mods[i]->getModId(), szModFolder);

			return m_Mods[i];
		}
	}

	logger->Log(LogLevel::FATAL, "HL2 MODIFICATION \"%s\" NOT FOUND, EXITING... see bot_mods.ini in bot config folder", szModFolder);

	return NULL;
}

void CBotMod :: initMod ()
{
	m_bPlayerHasSpawned = false;

	CWeapons::loadWeapons(m_szWeaponListName, NULL);
}

void CBotMod :: mapInit ()
{
	m_bPlayerHasSpawned = false;
}

bool CBotMod :: playerSpawned ( edict_t *pEntity )
{
	if ( m_bPlayerHasSpawned )
		return false;

	m_bPlayerHasSpawned = true;

	return true;
}

bool CHalfLifeDeathmatchMod :: playerSpawned ( edict_t *pPlayer )
{
	if ( CBotMod::playerSpawned(pPlayer) )
	{
		m_LiftWaypoints.clear();

		CWaypoints::updateWaypointPairs(&m_LiftWaypoints,CWaypointTypes::W_FL_LIFT,"func_button");
	}

	return true;
}

void CHalfLifeDeathmatchMod :: initMod ()
{

	CWeapons::loadWeapons(m_szWeaponListName==NULL?"HL2DM":m_szWeaponListName, HL2DMWeaps);
	
//	for ( i = 0; i < HL2DM_WEAPON_MAX; i ++ )
	//	CWeapons::addWeapon(new CWeapon(HL2DMWeaps[i]));//.iSlot,HL2DMWeaps[i].szWeaponName,HL2DMWeaps[i].iId,HL2DMWeaps[i].m_iFlags,HL2DMWeaps[i].m_iAmmoIndex,HL2DMWeaps[i].minPrimDist,HL2DMWeaps[i].maxPrimDist,HL2DMWeaps[i].m_iPreference,HL2DMWeaps[i].m_fProjSpeed));
}

void CHalfLifeDeathmatchMod :: mapInit ()
{
	CBotMod::mapInit();

	m_LiftWaypoints.clear();
}

