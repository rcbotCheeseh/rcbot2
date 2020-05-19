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
#include "bot_globals.h"
#include "bot_profile.h"
#include "bot_genclass.h"
#include "bot_visibles.h"
#include "bot_navigator.h"
#include "bot_kv.h"

std::vector <CBotProfile*> CBotProfiles :: m_Profiles;
CBotProfile *CBotProfiles :: m_pDefaultProfile = NULL;

CBotProfile :: CBotProfile ( CBotProfile &other )
{
	*this = other;

	m_szName = CStrings::getString(other.m_szName);
	m_szModel = CStrings::getString(other.m_szModel);
}

CBotProfile :: CBotProfile (
		const char *szName, 
		const char *szModel, 
		int iTeam, 
		int iVisionTicks, 
		int iPathTicks, 
		int iVisionTicksClients,
		int iSensitivity,
		float fBraveness,
		float fAimSkill,
		int iClass )
{ 
	m_iVisionTicksClients = iVisionTicksClients;
	m_iSensitivity = iSensitivity;
	m_fBraveness = fBraveness;
	m_fAimSkill = fAimSkill;
	m_szName = CStrings::getString(szName);
	m_szModel = CStrings::getString(szModel);
	m_iPathTicks = iPathTicks;
	m_iVisionTicks = iVisionTicks;
	m_iTeam = iTeam;
	m_iClass = iClass;
}

void CBotProfiles :: deleteProfiles ()
{
	for ( unsigned int i = 0; i < m_Profiles.size(); i ++ )
	{
		delete m_Profiles[i];
		m_Profiles[i] = NULL;
	}

	m_Profiles.clear();

	delete m_pDefaultProfile;
	m_pDefaultProfile = NULL;
}

// requires CBotProfile 'read' declared
#ifndef __linux__
#define READ_PROFILE_STRING(kvname,varname) if ( !pKVL->getString(##kvname##,&read.varname) ) { read.varname = m_pDefaultProfile->varname; }
#define READ_PROFILE_INT(kvname,varname) if ( !pKVL->getInt(##kvname##,&read.varname) ) { read.varname = m_pDefaultProfile->varname; }
// reads integers between 0 and 100 and converts to between 0.0 and 1.0
#define READ_PROFILE_FLOAT(kvname,varname) { float fval; if ( !pKVL->getFloat(##kvname##,&fval) ) { read.varname = m_pDefaultProfile->varname; } else { read.varname = fval * 0.01f; } }
#else
#define READ_PROFILE_STRING(kvname,varname) if ( !pKVL->getString(kvname,&read.varname) ) { read.varname = m_pDefaultProfile->varname; }
#define READ_PROFILE_INT(kvname,varname) if ( !pKVL->getInt(kvname,&read.varname) ) { read.varname = m_pDefaultProfile->varname; }
// reads integers between 0 and 100 and converts to between 0.0 and 1.0
#define READ_PROFILE_FLOAT(kvname,varname) { float fval; if ( !pKVL->getFloat(kvname,&fval) ) { read.varname = m_pDefaultProfile->varname; } else { read.varname = fval * 0.01f; } }
#endif
// find profiles and setup list
void CBotProfiles :: setupProfiles ()
{
	unsigned int iId;
	bool bDone;
	char szId[4];
	char filename[512];

	extern ConVar bot_anglespeed;

	// Setup Default profile
	m_pDefaultProfile = new CBotProfile(
		DEFAULT_BOT_NAME, // name
		"default", // model (team in HL2DM)
		-1, // iTeam
		CBotVisibles::DEFAULT_MAX_TICKS, // vis ticks
		IBotNavigator::MAX_PATH_TICKS, // path ticks
		2, // visrevs clients
		8.0f, // sensitivity
		0.5f, // braveness
		0.5f, // aim skill
		-1 // class
		);	

	// read profiles
	iId = 1;
	bDone = false;

	while ( (iId < 999) && (!bDone) )
	{
		sprintf(szId,"%d",iId);
		CBotGlobals::buildFileName(filename,szId,BOT_PROFILE_FOLDER,BOT_CONFIG_EXTENSION);

		FILE *fp = CBotGlobals::openFile(filename,"r");

		if ( fp )
		{
			CBotProfile read;
			CRCBotKeyValueList *pKVL = new CRCBotKeyValueList();

			CBotGlobals::botMessage(NULL,0,"Reading bot profile \"%s\"",filename);

			pKVL->parseFile(fp);

			READ_PROFILE_INT("team",m_iTeam);
			READ_PROFILE_STRING("model",m_szModel);
			READ_PROFILE_STRING("name",m_szName);
			READ_PROFILE_INT("visionticks",m_iVisionTicks);
			READ_PROFILE_INT("pathticks",m_iPathTicks);
			READ_PROFILE_INT("visionticks_clients",m_iVisionTicksClients);
			READ_PROFILE_INT("sensitivity",m_iSensitivity);
			READ_PROFILE_FLOAT("aim_skill",m_fAimSkill);
			READ_PROFILE_FLOAT("braveness",m_fBraveness);
			READ_PROFILE_INT("class",m_iClass);

			m_Profiles.push_back(new CBotProfile(read));

			delete pKVL;

			fclose(fp);
		}
		else
		{
			bDone = true;
			CBotGlobals::botMessage(NULL,0,"Bot profile \"%s\" not found",filename);
		}

		iId ++;
	}

}

CBotProfile *CBotProfiles :: getDefaultProfile ()
{
	if ( m_pDefaultProfile == NULL )
		CBotGlobals::botMessage(NULL,1,"Error, default profile is NULL (Caused by memory problem, bad initialisation or overwrite) Exiting..");

	return m_pDefaultProfile;
}

// return a profile unused by a bot
CBotProfile *CBotProfiles :: getRandomFreeProfile ()
{
	unsigned int i;
	dataUnconstArray<int> iList;
	CBotProfile *found = NULL;

	for ( i = 0; i < m_Profiles.size(); i ++ )
	{
		if ( !CBots::findBotByProfile(m_Profiles[i]) )
			iList.Add(i);
	}

	if ( iList.IsEmpty() )
		return NULL;
	
	found = m_Profiles[iList.Random()];
	iList.Clear();

	return found;
}

	

