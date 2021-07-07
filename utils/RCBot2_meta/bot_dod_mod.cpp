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
#include "server_class.h"

#include "bot.h"

#include "in_buttons.h"

#include "bot_mods.h"
#include "bot_globals.h"
#include "bot_weapons.h"
#include "bot_configfile.h"
#include "bot_getprop.h"
#include "bot_dod_bot.h"
#include "bot_navigator.h"
#include "bot_waypoint.h"
#include "bot_waypoint_locations.h"
#include "bot_perceptron.h"

edict_t *CDODMod::m_pResourceEntity = NULL;
CDODFlags CDODMod::m_Flags;
edict_t * CDODMod::m_pPlayerResourceEntity = NULL;
float CDODMod::m_fMapStartTime = 0.0f;
edict_t * CDODMod::m_pGameRules = NULL;
int CDODMod::m_iMapType = 0;
bool CDODMod::m_bCommunalBombPoint = false;
int CDODMod::m_iBombAreaAllies = 0;
int CDODMod::m_iBombAreaAxis = 0;
//CPerceptron *CDODMod::gNetAttackOrDefend = NULL;
float CDODMod::fAttackProbLookUp[MAX_DOD_FLAGS+1][MAX_DOD_FLAGS+1];
std::vector<edict_wpt_pair_t> CDODMod::m_BombWaypoints;
std::vector<edict_wpt_pair_t> CDODMod::m_BreakableWaypoints;


eDODVoiceCommand_t g_DODVoiceCommands[DOD_VC_INVALID] = 
{
	{DOD_VC_GOGOGO,"attack"},
	{DOD_VC_YES,"yessir"},
	{DOD_VC_DROPWEAP,"dropweapons"},
	{DOD_VC_HOLD,"hold"},
	{DOD_VC_NO,"negative"},
	{DOD_VC_DISPLACE,"displace"},
	{DOD_VC_GO_LEFT,"left"},
	{DOD_VC_NEED_BACKUP,"backup"},
	{DOD_VC_MGAHEAD,"mgahead"},
	{DOD_VC_GO_RIGHT,"right"},
	{DOD_VC_FIRE_IN_THE_HOLE,"fireinhole"},
	{DOD_VC_ENEMY_BEHIND,"enemybehind"},
	{DOD_VC_STICK_TOGETHER,"sticktogether"},
	{DOD_VC_USE_GRENADE,"usegrens"},
	{DOD_VC_ENEMY_DOWN,"wegothim"},
	{DOD_VC_COVERING_FIRE,"cover"},      
	{DOD_VC_SNIPER,"sniper"},
	{DOD_VC_NEED_MG,"moveupmg"},
	{DOD_VC_SMOKE,"usesmoke"},
	{DOD_VC_NICE_SHOT,"niceshot"},
	{DOD_VC_NEED_AMMO,"needammo"},
	{DOD_VC_GRENADE2,"grenade"},
	{DOD_VC_THANKS,"thanks"},
	{DOD_VC_USE_BAZOOKA,"usebazooka"},
	{DOD_VC_CEASEFIRE,"ceasefire"},
	{DOD_VC_AREA_CLEAR,"areaclear"},
	{DOD_VC_BAZOOKA,"bazookaspotted"}
};

//
//
//

// Returns true if team can go to waypoint
bool CDODMod :: checkWaypointForTeam(CWaypoint *pWpt, int iTeam)
{
	return (!pWpt->hasFlag(CWaypointTypes::W_FL_NOALLIES)||(iTeam!=TEAM_ALLIES))&&(!pWpt->hasFlag(CWaypointTypes::W_FL_NOAXIS)||(iTeam!=TEAM_AXIS));
}

bool CDODMod :: shouldAttack ( int iTeam )
// uses the perceptron to return probability of attack
{
	static short int iFlags_0;
	static short int iFlags_1;
	static short int iNumFlags;


	iNumFlags = m_Flags.getNumFlags();

	iFlags_0 = (int) (((float)m_Flags.getNumFlagsOwned(iTeam == TEAM_ALLIES ? TEAM_AXIS : TEAM_ALLIES) / iNumFlags)*MAX_DOD_FLAGS);
	iFlags_1 = (int) (((float)m_Flags.getNumFlagsOwned(iTeam) / iNumFlags)*MAX_DOD_FLAGS);

	return randomFloat(0.0,1.0) < fAttackProbLookUp[iFlags_0][iFlags_1];//gNetAttackOrDefend->getOutput();
}
////////////////////////////////////////////////
void CDODMod :: initMod ()
{
///-------------------------------------------------
	CBotGlobals::botMessage(NULL,0,"Training DOD:S capture decision 'NN' ... hold on...");

	CBotNeuralNet *nn = new CBotNeuralNet(2,2,2,1,0.4f);

	CTrainingSet *tset = new CTrainingSet(2,1,4);

	tset->setScale(0.0,1.0);

	tset->addSet();
	tset->in(1.0/5); // E - enemy flag ratio
	tset->in(1.0/5); // T - team flag ratio
	tset->out(0.9f); // probability of attack

	tset->addSet();
	tset->in(4.0/5); // E - enemy flag ratio
	tset->in(1.0/5); // T - team flag ratio
	tset->out(0.2f); // probability of attack (mostly defend)
	
	tset->addSet();
	tset->in(1.0/5); // E - enemy flag ratio
	tset->in(4.0/5); // T - team flag ratio
	tset->out(0.9f); // probability of attack

	tset->addSet();
	tset->in(0.5f); // E - enemy flag ratio
	tset->in(0.5f); // T - team flag ratio
	tset->out(0.6f); // probability of attack

	nn->batch_train(tset,1000);

	// create look up table for probabilities
	for ( short int i = 0; i <= MAX_DOD_FLAGS; i ++ )
	{
		for ( short int j = 0; j <= MAX_DOD_FLAGS; j ++ )
		{
			tset->init();
			tset->addSet();
			tset->in(((float)i) / MAX_DOD_FLAGS);
			tset->in(((float)j) / MAX_DOD_FLAGS);
			nn->execute(tset->getBatches()->in,&(fAttackProbLookUp[i][j]),0.0f,1.0f);
		}
	}

	tset->freeMemory();
	delete tset;
	delete nn;

	CBotGlobals::botMessage(NULL,0,"... done!");
///-------------------------------------------------

	CWeapons::loadWeapons((m_szWeaponListName == NULL) ? "DOD" : m_szWeaponListName, DODWeaps);
	//CWeapons::loadWeapons("DOD", DODWeaps);
	/*
	for ( i = 0; i < DOD_WEAPON_MAX; i ++ )
		CWeapons::addWeapon(new CWeapon(DODWeaps[i]));*/

	m_pResourceEntity = NULL;
}

void CDODMod :: mapInit ()
{
	CBotMod::mapInit();

	m_pResourceEntity = NULL;
	m_pGameRules = NULL;
	m_pPlayerResourceEntity = NULL;
	m_Flags.init();
	m_fMapStartTime = engine->Time();
	m_iMapType = DOD_MAPTYPE_UNKNOWN;
	m_bCommunalBombPoint = false;
}


float CDODMod::getMapStartTime () 
{ 
	//if ( !m_pGameRules ) 
	//	return 0; 
	return m_fMapStartTime;
	//return CClassInterface::getRoundTime(m_pGameRules); 
}

int CDODMod::getHighestScore ()
{
	if ( !m_pPlayerResourceEntity )
		return 0;

	int highest = 0;
	int score;
	short int i = 0;
	edict_t *edict;

	for ( i = 1; i <= gpGlobals->maxClients; i ++ )
	{
		edict = INDEXENT(i);

		if ( edict && CBotGlobals::entityIsValid(edict) )
		{
			score = (short int)getScore(edict);
		
			if ( score > highest )
			{
				highest = score;
			}
		}
	}

	return highest;
}

bool CDODFlags::isTeamMateDefusing ( edict_t *pIgnore, int iTeam, int id )
{
	if ( m_pBombs[id][0] != NULL )
		return isTeamMateDefusing(pIgnore,iTeam,CBotGlobals::entityOrigin(m_pBombs[id][0]));

	return false;
}

bool CDODFlags::isTeamMateDefusing ( edict_t *pIgnore, int iTeam, Vector vOrigin )
{
	int i;
	edict_t *pPlayer;
	IPlayerInfo *pPlayerinfo;
	CBotCmd cmd;

	for ( i = 1; i <= gpGlobals->maxClients; i ++ )
	{
		pPlayer = INDEXENT(i);

		if ( pIgnore == pPlayer )
			continue;

		if ( !CBotGlobals::entityIsValid(pPlayer) )
			continue;

		pPlayerinfo = playerinfomanager->GetPlayerInfo(pPlayer);
		cmd = pPlayerinfo->GetLastUserCommand();

		if ( CClassInterface::isPlayerDefusingBomb_DOD(pPlayer) || (cmd.buttons & IN_USE) )
		{
			if ( CClassInterface::getTeam(pPlayer) != iTeam )
				continue;

			if ( (vOrigin - CBotGlobals::entityOrigin(pPlayer)).Length() < 128 )
			{
				return true;
			}
		}
	}

	return false;
}


bool CDODFlags::isTeamMatePlanting ( edict_t *pIgnore, int iTeam, Vector vOrigin )
{
	int i;
	edict_t *pPlayer;

	for ( i = 1; i <= gpGlobals->maxClients; i ++ )
	{
		pPlayer = INDEXENT(i);

		if ( pIgnore == pPlayer )
			continue;

		if ( !CBotGlobals::entityIsValid(pPlayer) )
			continue;

		if ( CClassInterface::isPlayerPlantingBomb_DOD(pPlayer) )
		{
			if ( CClassInterface::getTeam(pPlayer) != iTeam )
				continue;

			if ( (vOrigin - CBotGlobals::entityOrigin(pPlayer)).Length() < 128 )
			{
				return true;
			}
		}
	}

	return false;
}

bool CDODFlags::isTeamMatePlanting ( edict_t *pIgnore, int iTeam, int id )
{
	if ( m_pBombs[id][0] )
		return isTeamMatePlanting(pIgnore,iTeam,CBotGlobals::entityOrigin(m_pBombs[id][0]));

	return false;
}

int CDODFlags::findNearestObjective ( Vector vOrigin )
{
	float fNearest = 1024.0f;
	float fDistance;
	int iNearest = -1;
	
	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( m_iWaypoint[i] != -1 )
		{
			if ( (fDistance = (CWaypoints::getWaypoint(m_iWaypoint[i])->getOrigin()-vOrigin).Length()) < fNearest )
			{
				fNearest = fDistance;
				iNearest = i;
			}
		}
	}

	return iNearest;

}

// return the flag with the least danger (randomly)
bool CDODFlags::getRandomEnemyControlledFlag ( CBot *pBot, Vector *position, int iTeam, int *id )
{
	IBotNavigator *pNav;
	float fTotal;
	float fRand;

	if ( id )
		*id = -1;

	pNav = pBot->getNavigator();

	fTotal = 0.0f;

	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( m_iWaypoint[i] != -1 )
		{
			if ( ( m_pFlags[i] == NULL ) || ( m_iOwner[i] == iTeam ) )
				continue;

			if ( (iTeam == TEAM_ALLIES) && (m_iAlliesReqCappers[i] == 0) )
				continue;

			if ( (iTeam == TEAM_AXIS) && (m_iAxisReqCappers[i] == 0) )
				continue;

			if ( iTeam == TEAM_ALLIES )
				fTotal += (((MAX_BELIEF + 1.0f) - pNav->getBelief(m_iWaypoint[i])) / MAX_BELIEF) * (m_iNumAllies[i]+1);
			else
				fTotal += (((MAX_BELIEF + 1.0f) - pNav->getBelief(m_iWaypoint[i])) / MAX_BELIEF) * (m_iNumAxis[i]+1);
		}
	}

	if ( fTotal == 0.0f )
		return false;

	fRand = randomFloat(0,fTotal);
	fTotal = 0.0f;

	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( m_iWaypoint[i] != -1 )
		{
			if ( ( m_pFlags[i] == NULL ) || ( m_iOwner[i] == iTeam ) )
				continue;

			if ( (iTeam == TEAM_ALLIES) && (m_iAlliesReqCappers[i] == 0) )
				continue;

			if ( (iTeam == TEAM_AXIS) && (m_iAxisReqCappers[i] == 0) )
				continue;

			if ( iTeam == TEAM_ALLIES )
				fTotal += (((MAX_BELIEF + 1.0f) - pNav->getBelief(m_iWaypoint[i])) / MAX_BELIEF) * (m_iNumAllies[i]+1);
			else
				fTotal += (((MAX_BELIEF + 1.0f) - pNav->getBelief(m_iWaypoint[i])) / MAX_BELIEF) * (m_iNumAxis[i]+1);
		}

		if ( fRand <= fTotal )
		{
			if ( id )
				*id = i;
			*position = m_vCPPositions[i];
			return true;
		}
	}

	return false;
}

bool CDODFlags::getRandomBombToDefuse  ( Vector *position, int iTeam, edict_t **pBombTarget, int *id )
{
	std::vector<int> iPossible; // int is control point entry
	short int j;
	int selection;

	if ( id )
		*id = -1;

	// more possibility to return bomb targets with no bomb already
	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( (m_iOwner[i] == iTeam) && isBombPlanted(i) && !isBombBeingDefused(i) && (m_pBombs[i][0] != NULL) )
			for ( j = 0; j < getNumBombsRequired(i); j ++ ) { iPossible.push_back(i); }
	}

	if ( iPossible.size() > 0 )
	{
		selection = iPossible[randomInt(0,iPossible.size()-1)];

		if ( m_pBombs[selection][1] != NULL )
		{
			if ( CClassInterface::getDODBombState(m_pBombs[selection][1]) == DOD_BOMB_STATE_ACTIVE )
				*pBombTarget = m_pBombs[selection][1];
			else
				*pBombTarget = m_pBombs[selection][0];
		}
		else
			*pBombTarget = m_pBombs[selection][0];

		*position = CBotGlobals::entityOrigin(*pBombTarget);

		if ( id ) // area of the capture point
			*id = selection;
	}

	return (iPossible.size()>0);
}

//return random bomb with highest danger
bool CDODFlags:: getRandomBombToDefend ( CBot *pBot, Vector *position, int iTeam, edict_t **pBombTarget, int *id )
{
	std::vector<int> iPossible; // int is control point entry
	short int j;
	int selection;

	if ( id )
		*id = -1;

	// more possibility to return bomb targets with no bomb already
	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( (m_iOwner[i] != iTeam) && isBombPlanted(i) && (m_pBombs[i][0] != NULL) )
			for ( j = 0; j < getNumBombsRequired(i); j ++ ) { iPossible.push_back(i); }
	}

	if ( iPossible.size() > 0 )
	{
		selection = iPossible[randomInt(0,iPossible.size()-1)];

		if ( m_pBombs[selection][1] != NULL )
		{
			if ( CClassInterface::getDODBombState(m_pBombs[selection][1]) != 0 )
				*pBombTarget = m_pBombs[selection][1];
			else
				*pBombTarget = m_pBombs[selection][0];
		}
		else
			*pBombTarget = m_pBombs[selection][0];

		*position = CBotGlobals::entityOrigin(*pBombTarget);

		if ( id ) // area of the capture point
			*id = selection;
	}

	return (iPossible.size()>0);
}

// return rnaomd flag with lowest danger
bool CDODFlags:: getRandomBombToPlant ( CBot *pBot, Vector *position, int iTeam, edict_t **pBombTarget, int *id )
{
	float fTotal;
	float fRand;

	IBotNavigator *pNav;

//	short int j;
	int selection;

	if ( id )
		*id = -1;

	selection = -1;

	pNav = pBot->getNavigator();

	fTotal = 0.0f;

	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		// if no waypoint -- can't go there
		if ( m_iWaypoint[i] != -1 )
		{
			if ( ( m_pBombs[i][0] == NULL ) || ( m_iOwner[i] == iTeam ) || isBombPlanted(i) || (m_iBombsRemaining[i] == 0) )
				continue;

			fTotal += (((MAX_BELIEF + 1.0f) - pNav->getBelief(m_iWaypoint[i])) / MAX_BELIEF) * getNumBombsRemaining(i);
		}
	}

	if ( fTotal == 0.0f )
		return false;

	fRand = randomFloat(0.0f,fTotal);

	fTotal = 0.0f;

	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( m_iWaypoint[i] != -1 )
		{
			if ( ( m_pBombs[i][0] == NULL ) || ( m_iOwner[i] == iTeam ) || isBombPlanted(i) )
				continue;

				fTotal += (((MAX_BELIEF + 1.0f) - pNav->getBelief(m_iWaypoint[i])) / MAX_BELIEF) * getNumBombsRemaining(i);
		}
		else
			fTotal += 0.1f;

		if ( fRand <= fTotal )
		{
			selection = i;

			if ( m_pBombs[selection][1] != NULL )
			{
				if ( CClassInterface::getDODBombState(m_pBombs[selection][1]) == DOD_BOMB_STATE_AVAILABLE )
					*pBombTarget = m_pBombs[selection][1];
				else
					*pBombTarget = m_pBombs[selection][0];
			}
			else
				*pBombTarget = m_pBombs[selection][0];

			*position = CBotGlobals::entityOrigin(*pBombTarget);

			if ( id ) // area of the capture point
				*id = selection;

			return true;
		}
	}

	return false;
}


bool CDODFlags::getRandomTeamControlledFlag ( CBot *pBot, Vector *position, int iTeam, int *id )
{
	IBotNavigator *pNav;
	float fTotal;
	float fRand;

	if ( id )
		*id = -1;

	pNav = pBot->getNavigator();

	fTotal = 0.0f;

	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( m_iWaypoint[i] != -1 )
		{
			if ( ( m_pFlags[i] == NULL ) || ( m_iOwner[i] != iTeam ) )
				continue;

			if ( iTeam == TEAM_AXIS )
				fTotal += ((pNav->getBelief(m_iWaypoint[i])+MAX_BELIEF)/(MAX_BELIEF*2)) * (m_iNumAllies[i]+1);
			else
				fTotal += ((pNav->getBelief(m_iWaypoint[i])+MAX_BELIEF)/(MAX_BELIEF*2)) * (m_iNumAxis[i]+1);
		}
	}

	if ( fTotal == 0.0f )
		return false;

	fRand = randomFloat(0,fTotal);
	fTotal = 0.0f;

	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( m_iWaypoint[i] != -1 )
		{
			if ( ( m_pFlags[i] == NULL ) || ( m_iOwner[i] != iTeam ) )
				continue;

			if ( iTeam == TEAM_AXIS )
				fTotal += ((pNav->getBelief(m_iWaypoint[i])+MAX_BELIEF)/(MAX_BELIEF*2)) * (m_iNumAllies[i]+1);
			else
				fTotal += ((pNav->getBelief(m_iWaypoint[i])+MAX_BELIEF)/(MAX_BELIEF*2)) * (m_iNumAxis[i]+1);
		}

		if ( fRand <= fTotal )
		{
			if ( id )
				*id = i;
			*position = m_vCPPositions[i];
			return true;
		}
	}

	return false;
}

void CDODMod::freeMemory()
{

}

// returns map type
int CDODFlags::setup(edict_t *pResourceEntity)
{
	int iNumBombCaps = 0;
	int iNumFlags = 0;
	//bool *CPsVisible;

	m_iNumControlPoints = 0;

	memset(m_bBombPlanted,0,sizeof(bool)*MAX_DOD_FLAGS); // all false

	//CPsVisible = CClassInterface::getDODCPVisible(pResourceEntity);

	if ( pResourceEntity )
	{  
		// get the arrays from the resource entity
		CClassInterface::getDODFlagInfo(pResourceEntity,&m_iNumAxis,&m_iNumAllies,&m_iOwner,&m_iAlliesReqCappers,&m_iAxisReqCappers);
		CClassInterface::getDODBombInfo(pResourceEntity,&m_bBombPlanted_Unreliable,&m_iBombsRequired,&m_iBombsRemaining,&m_bBombBeingDefused);
		m_iNumControlPoints = CClassInterface::getDODNumControlPoints(pResourceEntity);
		// get the Capture point positions
		m_vCPPositions = CClassInterface::getDODCP_Positions(pResourceEntity);

	}

	short int i,j;

//	string_t model;		
//	const char *modelname;
//	bool bVisible;
				

	// find the edicts of the flags using the origin and classname

	for ( j = 0; j < m_iNumControlPoints; j ++ )
	{
		edict_t *pent;

		Vector vOrigin;

		i = gpGlobals->maxClients;

		// find visible flags -- with a model
		while ( (++i < gpGlobals->maxEntities) &&  (m_pFlags[j] == NULL ) )
		{
			pent = INDEXENT(i);

			if ( !pent || pent->IsFree() )
				continue;

			if ( strcmp(pent->GetClassName(),DOD_CLASSNAME_CONTROLPOINT) == 0 )
			{
				vOrigin = CBotGlobals::entityOrigin(pent);

				if ( vOrigin == m_vCPPositions[j] )
				{
					/*
					bVisible = ((CClassInterface::getEffects(pent) & EF_NODRAW) != EF_NODRAW);

					model = pent->GetIServerEntity()->GetModelName();
					modelname = model.ToCStr();

					if ( bVisible && modelname && *modelname )
					{*/

					if ( m_iAlliesReqCappers[j] || m_iAxisReqCappers[j] || m_iBombsRequired[j] )
					{
						m_pFlags[j] = pent;
					}

					break; // found it
				}
			}
		}

		// no flag for this point
		if ( m_pFlags[j] == NULL ) 
			continue;

		// don't need to check for bombs
		if ( m_iBombsRequired[j] == 0 )
			continue;

		// find bombs near flag
		i = gpGlobals->maxClients;

		while ( (++i < gpGlobals->maxEntities) && ((m_pBombs[j][0]==NULL)||(m_pBombs[j][1]==NULL)) )
		{
			pent = INDEXENT(i);

			if ( !pent || pent->IsFree() )
				continue;

			if ( strcmp(pent->GetClassName(),DOD_CLASSNAME_BOMBTARGET) == 0 )
			{
				vOrigin = CBotGlobals::entityOrigin(pent);

				if ( (vOrigin - m_vCPPositions[j]).Length() < 400.0f )
				{
					if ( m_pBombs[j][0] == NULL )
					{
						m_pBombs[j][0] = pent;						
					}
					else
						m_pBombs[j][1] = pent;
				}
			}
		}
	}

	// find waypoints
	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		// if we don't know any cap waypoint yet here find one
		if ( m_iWaypoint[i] == -1 )
		{
			// get any nearby waypoint so the bot knows which waypoint to get danger from
			// look for the nearest waypoint which is a cap point
			m_iWaypoint[i] = CWaypointLocations::NearestWaypoint(m_vCPPositions[i],400.0f,-1,false,false,false,0,false,0,false,false,Vector(0,0,0),CWaypointTypes::W_FL_CAPPOINT );

			// still no waypoint, search for any capture waypoint with the same area
			if ( m_iWaypoint[i] == -1 )
			{
				m_iWaypoint[i] = CWaypoints::getWaypointIndex(CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_CAPPOINT,0,i,true));
			}
		}
	}

	m_iNumAxisBombsOnMap = getNumPlantableBombs(TEAM_AXIS);
	m_iNumAlliesBombsOnMap = getNumPlantableBombs(TEAM_ALLIES);

	// sometimes m_iNumControlPoints is larger than it  should be. check the number of flags and bombs we found on the map
	for ( short int i = 0; i < m_iNumControlPoints; i ++ )
	{
		if ( m_pFlags[i] != NULL )
			iNumFlags++;
		if ( m_pBombs[i][0] != NULL )
			iNumBombCaps++;
		if ( m_pBombs[i][1] != NULL )
			iNumBombCaps++;
	}

	// update new number
	m_iNumControlPoints = iNumFlags;

	if ( iNumBombCaps >= iNumFlags )
		return DOD_MAPTYPE_BOMB;

	return DOD_MAPTYPE_FLAG;
}

int CDODMod ::getScore(edict_t *pPlayer)
{
	if ( m_pPlayerResourceEntity )
		return CClassInterface::getPlayerScoreDOD(m_pPlayerResourceEntity,pPlayer) + 
		CClassInterface::getPlayerObjectiveScoreDOD(m_pPlayerResourceEntity,pPlayer) - 
		CClassInterface::getPlayerDeathsDOD(pPlayer,m_pPlayerResourceEntity);

	return 0;
}

edict_t *CDODMod :: getBreakable ( CWaypoint *pWpt )
{
	const unsigned short int size = m_BreakableWaypoints.size();

	for (unsigned short int i = 0; i < size; i ++ )
	{
		if ( m_BreakableWaypoints[i].pWaypoint == pWpt )
			return m_BreakableWaypoints[i].pEdict;
	}

	return NULL;
}

edict_t *CDODMod :: getBombTarget ( CWaypoint *pWpt )
{
	const unsigned short int size = m_BombWaypoints.size();

	for (unsigned short int i = 0; i < size; i ++ )
	{
		if ( m_BombWaypoints[i].pWaypoint == pWpt )
			return m_BombWaypoints[i].pEdict;
	}

	return NULL;
}

void CDODMod ::roundStart()
{
	if ( !m_pResourceEntity )
		m_pResourceEntity = CClassInterface::FindEntityByNetClass(gpGlobals->maxClients+1, "CDODObjectiveResource");
	if ( !m_pPlayerResourceEntity )
		m_pPlayerResourceEntity = CClassInterface::FindEntityByNetClass(gpGlobals->maxClients+1, "CDODPlayerResource");
	if ( !m_pGameRules )
		m_pGameRules = CClassInterface::FindEntityByNetClass(gpGlobals->maxClients+1, "CDODGameRulesProxy");

	// find main map type
	m_iMapType = m_Flags.setup(m_pResourceEntity);

	//if ( m_iMapType == DOD_MAPTYPE_UNKNOWN )
	//{
		if ( CClassInterface::FindEntityByNetClass(gpGlobals->maxClients+1,"CDODBombDispenserMapIcon") != NULL )
		{
			CWaypoint *pWaypointAllies;
			CWaypoint *pWaypointAxis;

			// add bitmask
			m_iMapType |= DOD_MAPTYPE_BOMB;
/*
			if ( m_iMapType == DOD_MAPTYPE_FLAG) 
				CRCBotPlugin::HudTextMessage(CClients::get(0)->getPlayer(),"RCBot detected Flag map","RCBot2","RCbot2 detected a flag map");
			else if ( m_iMapType == DOD_MAPTYPE_BOMB )
				CRCBotPlugin::HudTextMessage(CClients::get(0)->getPlayer(),"RCBot detected bomb map","RCBot2","RCbot2 detected a bomb map");
			else if ( m_iMapType == 3 )
				CRCBotPlugin::HudTextMessage(CClients::get(0)->getPlayer(),"RCBot detected flag map with bombs ","RCBot2","RCbot2 detected a flag capture map with bombs");

*/
			pWaypointAllies = CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_BOMBS_HERE,TEAM_ALLIES);
			pWaypointAxis = CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_BOMBS_HERE,TEAM_AXIS);

			if ( pWaypointAllies && pWaypointAxis )
			{
				m_bCommunalBombPoint = (pWaypointAllies->getArea()>0) || (pWaypointAxis->getArea()>0);

				m_iBombAreaAllies = pWaypointAllies->getArea();
				m_iBombAreaAxis = pWaypointAxis->getArea();
			}
		}
		//else
		//	m_iMapType = DOD_MAPTYPE_FLAG;
	//}

	// find bombs at waypoints
	m_BombWaypoints.clear();
	m_BreakableWaypoints.clear();

	CWaypoints::updateWaypointPairs(&m_BombWaypoints,CWaypointTypes::W_FL_BOMB_TO_OPEN,"dod_bomb_target");
	CWaypoints::updateWaypointPairs(&m_BreakableWaypoints,CWaypointTypes::W_FL_BREAKABLE,"func_breakable");

	//m_Flags.updateAll();
}
// when a bomb explodes it might leave a part of the ground available
// find it and add it as a waypoint offset
Vector CDODMod :: getGround ( CWaypoint *pWaypoint )
{
	for ( unsigned int i = 0; i < m_BombWaypoints.size(); i ++ )
	{
		if ( m_BombWaypoints[i].pWaypoint == pWaypoint )
		{
			if ( m_BombWaypoints[i].pEdict )
			{
				if ( CClassInterface::getDODBombState(m_BombWaypoints[i].pEdict) == 0 )
					return m_BombWaypoints[i].v_ground;
				
				break;
			}
		}
	}

	return pWaypoint->getOrigin();
}

void CDODMod :: addWaypointFlags (edict_t *pPlayer, edict_t *pEdict, int *iFlags, int *iArea, float *fMaxDistance )
{
	if ( isBombMap()  )
	{
		const int id = m_Flags.getBombID(pEdict);

		if ( id != -1 )
		{
			*iFlags |= CWaypointTypes::W_FL_CAPPOINT;
			*iArea = id;
		}
	}
	
	if ( isFlagMap() )
	{
		const int id = m_Flags.getFlagID(pEdict);

		if ( id != -1 )
		{
			*iFlags |= CWaypointTypes::W_FL_CAPPOINT;
			*iArea = id;
		}
	}

}

void CDODMod :: modFrame()
{

}


int CDODMod ::numClassOnTeam( int iTeam, int iClass )
{
	int i = 0;
	int num = 0;
	edict_t *pEdict;

	for ( i = 1; i <= CBotGlobals::numClients(); i ++ )
	{
		pEdict = INDEXENT(i);

		if ( CBotGlobals::entityIsValid(pEdict) )
		{
			if ( CClassInterface::getTeam(pEdict) == iTeam )
			{
				if ( CClassInterface::getPlayerClassDOD(pEdict) == iClass )
					num++;
			}
		}
	}

	return num;
}

void CDODMod ::clientCommand( edict_t *pEntity, int argc, const char *pcmd, const char *arg1, const char *arg2 )
{
	if ( argc == 1 )
	{
		if ( strncmp(pcmd,"voice_",6) == 0 )
		{
			short int i;
			// somebody said a voice command
			u_VOICECMD vcmd;

			for ( i = 0; i < DOD_VC_INVALID; i ++ )
			{
				if ( strcmp(&pcmd[6],g_DODVoiceCommands[i].pcmd) == 0 )
				{
					vcmd.voicecmd = i;

					CBroadcastVoiceCommand voicecmd = CBroadcastVoiceCommand(pEntity,vcmd.voicecmd); 

					CBots::botFunction(&voicecmd);

					break;
				}
			}
		}
	}
}

bool CDODMod :: isBreakableRegistered ( edict_t *pBreakable, int iTeam )
{
	static CWaypoint *pWpt;

	for ( unsigned int i = 0; i < m_BreakableWaypoints.size(); i ++ )
	{
		if ( m_BreakableWaypoints[i].pEdict == pBreakable )
		{
			pWpt = m_BreakableWaypoints[i].pWaypoint;

			if ( pWpt->hasFlag(CWaypointTypes::W_FL_NOALLIES) )
				return iTeam != TEAM_ALLIES;
			else if ( pWpt->hasFlag(CWaypointTypes::W_FL_NOAXIS) )
				return iTeam != TEAM_AXIS;

			return true;
		}
	}

	return false;
}

void CDODMod :: getTeamOnlyWaypointFlags ( int iTeam, int *iOn, int *iOff )
{
	if ( iTeam == TEAM_ALLIES )
	{
		*iOn = CWaypointTypes::W_FL_NOAXIS;
		*iOff = CWaypointTypes::W_FL_NOALLIES;
	}
	else if ( iTeam == TEAM_AXIS )
	{
		*iOn = CWaypointTypes::W_FL_NOALLIES;
		*iOff = CWaypointTypes::W_FL_NOAXIS;
	}


}
