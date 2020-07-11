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

#include "vector.h"

#include "bot_const.h"
#include "bot.h"
#include "bot_globals.h"
#include "bot_squads.h"
#include "bot_getprop.h"

#include <algorithm>

std::deque<CBotSquad*> CBotSquads::m_theSquads;

class CRemoveBotFromSquad : public IBotFunction
{
public:

	CRemoveBotFromSquad ( CBotSquad *pSquad )
	{
		m_pSquad = pSquad;
	}

	virtual void execute ( CBot *pBot )
	{
		if ( pBot->inSquad(m_pSquad) )
			pBot->clearSquad();
	}

private:
	CBotSquad *m_pSquad;
};

//-------------

void CBotSquads::FreeMemory ( void )
{
	// TODO inline squad or use unique pointers or something so they're freed automatically
	for (CBotSquad *squad : m_theSquads) {
		delete squad;
	}
	m_theSquads.clear();
}

void CBotSquads::removeSquadMember ( CBotSquad *pSquad, edict_t *pMember )
{
	pSquad->removeMember(pMember);

	if ( pSquad->numMembers() <= 1 )
	{
		RemoveSquad(pSquad);
	}
}

edict_t *CBotSquad::getMember ( size_t iMember )
{
	// TODO: this is only used in CBotSquads::SquadJoin() -- inline the logic
	if (iMember < 0 || iMember >= m_SquadMembers.size()) {
		return nullptr;
	}
	
	return m_SquadMembers[iMember];
}

// AddSquadMember can have many effects
// 1. scenario: squad leader exists as squad leader
//              assign bot to squad
// 2. scenario: 'squad leader' exists as squad member in another squad
//              assign bot to 'squad leaders' squad
// 3. scenario: no squad has 'squad leader' 
//              make a new squad
CBotSquad *CBotSquads::AddSquadMember ( edict_t *pLeader, edict_t *pMember )
{
	CBot *pBot;

	//char msg[120];

	if ( !pLeader )
		return NULL;

	if ( CClassInterface::getTeam(pLeader) != CClassInterface::getTeam(pMember) )
		return NULL;

	//CClient *pClient = gBotGlobals.m_Clients.GetClientByEdict(pLeader);

	//if ( pClient )
	//{
	//	pClient->AddNewToolTip(BOT_TOOL_TIP_SQUAD_HELP);
	//}

	//sprintf(msg,"%s %s has joined your squad",BOT_DBG_MSG_TAG,STRING(pMember->v.netname));
	//ClientPrint(pLeader,HUD_PRINTTALK,msg);
	
	// member joins whatever squad the leader is in
	for (CBotSquad *squad : m_theSquads) {
		if (squad->IsLeader(pLeader) || squad->IsMember(pLeader)) {
			squad->AddMember(pMember);
			return squad;
		}
	}
	
	// no squad with leader, make one
	CBotSquad *theSquad = new CBotSquad(pLeader, pMember);
	
	if ( theSquad != NULL )
	{
		m_theSquads.push_back(theSquad);
		
		if ( (pBot = CBots::getBotPointer(pLeader)) != NULL )
			pBot->setSquad(theSquad);
	}
	
	return theSquad;
}
// SquadJoin 
// join two possile squads together if pMember is a leader of another squad
//
CBotSquad *CBotSquads::SquadJoin ( edict_t *pLeader, edict_t *pMember )
{
	CBotSquad *theSquad;
	CBotSquad *joinSquad;

	//char msg[120];

	if ( !pLeader )
		return NULL;

	if ( CClassInterface::getTeam(pLeader) != CClassInterface::getTeam(pMember) )
		return NULL;

	// no squad with leader, make pMember join SquadLeader
	theSquad = FindSquadByLeader(pMember);

	if ( theSquad != NULL )
	{
		theSquad->AddMember(pMember);

		joinSquad = FindSquadByLeader(pLeader);

		if ( joinSquad )
		{
			// TODO make this a friend class so we could just join the squads directly?
			for ( size_t i = 0; i < joinSquad->numMembers(); i ++ )
			{
				theSquad->AddMember(joinSquad->getMember(i));
			}

			RemoveSquad(joinSquad);
		}
		else
			return NULL;
	}
	
	// no squad with leader, make pMember join SquadLeader
	theSquad = FindSquadByLeader(pLeader);

	if ( theSquad != NULL )
		theSquad->AddMember(pMember);

	return theSquad;
}

CBotSquad *CBotSquads::FindSquadByLeader ( edict_t *pLeader )
{
	for (CBotSquad *squad : m_theSquads) {
		if (squad->IsLeader(pLeader)) {
			return squad;
		}
	}
	return nullptr;
}

void CBotSquads::RemoveSquad ( CBotSquad *pSquad )
{
	// TODO see if we can modify this so the squad unregisters itself
	// TODO call this logic in CBotSquad::~CBotSquad() instead
	CRemoveBotFromSquad func(pSquad);
	CBots::botFunction(&func);
	
	m_theSquads.erase(std::remove(m_theSquads.begin(), m_theSquads.end(), pSquad), m_theSquads.end());
	
	if (pSquad)
		delete pSquad;
}

void CBotSquads::UpdateAngles ( void )
{
	for (CBotSquad *squad : m_theSquads) {
		squad->UpdateAngles();
	}
}

//-------------

void CBotSquad::UpdateAngles ( void )
{
	edict_t *pLeader = GetLeader();

	Vector velocity;

	CClassInterface::getVelocity(pLeader,&velocity);

	if ( velocity.Length2D() > 1.0f )
	{
		VectorAngles(velocity,m_vLeaderAngle);
	}
}

void CBotSquad::Init ()
{
	edict_t *pLeader;

	m_theDesiredFormation = SQUAD_FORM_WEDGE; // default wedge formation
	m_fDesiredSpread = SQUAD_DEFAULT_SPREAD; 

	m_CombatType = COMBAT_COMBAT;

	bCanFire = true;

	if ( (pLeader = GetLeader()) != NULL )
	{
		IPlayerInfo *p = playerinfomanager->GetPlayerInfo(pLeader);
		m_vLeaderAngle = p->GetLastUserCommand().viewangles;
	}
}

// Change a leader of a squad, this can cause lots of effects
void CBotSquads :: ChangeLeader ( CBotSquad *pSquad )
{
	// first change leader to next squad member
	pSquad->ChangeLeader();

	// if no leader anymore/no members in group
	if ( pSquad->IsLeader(NULL) )
	{
		CRemoveBotFromSquad func(pSquad);
		CBots::botFunction(&func);
		
		// must also remove from available squads.
		m_theSquads.erase(std::remove(m_theSquads.begin(), m_theSquads.end(), pSquad), m_theSquads.end());
	}
}

/**
 * Make the succeeding squad member the new leader if they have any subordinates, otherwise
 * disband the squad.
 */
void CBotSquad::ChangeLeader ( void )
{
	if ( m_SquadMembers.empty() )
	{
		SetLeader(NULL);
	}
	else
	{
		m_pLeader = m_SquadMembers.front();
		m_SquadMembers.pop_front();

		if ( m_SquadMembers.empty() )
			SetLeader(NULL);
		else
		{
			Init(); // new squad init
		}
	}
}

Vector CBotSquad :: GetFormationVector ( edict_t *pEdict )
{
	Vector vLeaderOrigin;
	Vector vBase; 
	Vector v_forward;
	Vector v_right;
	QAngle angle_right;
	// vBase = first : offset from leader origin without taking into consideration spread and position
	int iPosition;
	trace_t *tr = CBotGlobals::getTraceResult();

	edict_t *pLeader = GetLeader();
	
	iPosition = GetFormationPosition(pEdict);
	vLeaderOrigin = CBotGlobals::entityOrigin(pLeader);

	int iMod = iPosition % 2;

	AngleVectors(m_vLeaderAngle,&v_forward); // leader body angles as base

	angle_right = m_vLeaderAngle;
	angle_right.y += 90.0f;

	CBotGlobals::fixFloatAngle(&(angle_right.y));

	AngleVectors(angle_right,&v_right); // leader body angles as base

	// going to have members on either side.
	switch ( m_theDesiredFormation ) 
	{
	case SQUAD_FORM_VEE:
		{
			if ( iMod )			
				vBase = (v_forward-v_right);			
			else
				vBase = (v_forward+v_right);
		}
		break;
	case SQUAD_FORM_WEDGE:
		{
			if ( iMod )			
				vBase = -(v_forward-v_right);			
			else
				vBase = -(v_forward+v_right);
		}
		break;
	case SQUAD_FORM_LINE:
		{

			// have members on either side of leader

			if ( iMod )			
				vBase = v_right;			
			else
				vBase = -v_right;
		}
		break;
	case SQUAD_FORM_COLUMN:
		{
			vBase = -v_forward;
		}
		break;
	case SQUAD_FORM_ECH_LEFT:
		{
			vBase = -v_forward - v_right;
		}
		break;
	case SQUAD_FORM_ECH_RIGHT:
		{
			vBase = -v_forward + v_right;
		}
		break;
	}
	
	vBase = (vBase * m_fDesiredSpread) * iPosition;

	CBotGlobals::quickTraceline(pLeader,vLeaderOrigin,vLeaderOrigin+vBase);

	if ( tr->fraction < 1.0 )
	{
		return vLeaderOrigin + (vBase*tr->fraction*0.5f);
	}

	return vLeaderOrigin+vBase;
}

/**
 * Returns the edict's position in the squad.
 */
int CBotSquad::GetFormationPosition ( edict_t *pEdict )
{
	auto it = std::find(m_SquadMembers.begin(), m_SquadMembers.end(), pEdict);
	return it != m_SquadMembers.end()? std::distance(m_SquadMembers.begin(), it) : 0;
}

void CBotSquad::removeMember ( edict_t *pMember )
{
	auto it = std::find(m_SquadMembers.begin(), m_SquadMembers.end(), pMember);
	if (it != m_SquadMembers.end()) {
		m_SquadMembers.erase(it);
	}
}

void CBotSquad::AddMember ( edict_t *pEdict )
{
	if ( !IsMember(pEdict) )
	{
		MyEHandle newh;
		//CBot *pBot;

		newh = pEdict;

		m_SquadMembers.push_back(newh);

		/*if ( (pBot=CBots::getBotPointer(pEdict))!=NULL )
		{
			pBot->clearSquad();
			pBot->setSquad(this);
		}*/
	}
}

size_t CBotSquad::numMembers ()
{
	return m_SquadMembers.size();
}

void CBotSquad :: ReturnAllToFormation ( void )
{
	for (edict_t *member : m_SquadMembers) {
		CBot *pBot = CBots::getBotPointer(member);
		if (pBot) {
			pBot->removeCondition(CONDITION_PUSH);
			pBot->removeCondition(CONDITION_COVERT);
			pBot->updateCondition(CONDITION_CHANGED);
			pBot->setSquadIdleTime(0.0f);
		}
	}
}

bool CBotSquad::IsMember ( edict_t *pEdict )
{
	return std::find(m_SquadMembers.begin(), m_SquadMembers.end(), pEdict)
			!= m_SquadMembers.end();
}
