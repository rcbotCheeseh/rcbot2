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
#ifndef __HLDM_RCBOT_H__
#define __HLDM_RCBOT_H__

#include "bot_utility.h"

// bot for HLDM
class CHLDMBot : public CBot
{
public:
	bool handleAttack ( CBotWeapon *pWeapon, edict_t *pEnemy );

	void handleWeapons ();

	bool isHLDM () { return true; }

	void modThink ();

	void init ();
	void setup ();

	bool startGame ();

	void died ( edict_t *pKiller, const char *pszWeapon );
	void killed ( edict_t *pVictim, char *weapon );

	void spawnInit ();

	bool isEnemy ( edict_t *pEdict,bool bCheckWeapons = true );

	void getTasks (unsigned int iIgnore=0);
	bool executeAction ( eBotAction iAction );

	float getArmorPercent () { return (0.01f * m_pPlayerInfo->GetArmorValue()); }

	bool setVisible ( edict_t *pEntity, bool bVisible );

	virtual unsigned int maxEntityIndex ( ) { return gpGlobals->maxEntities; }

	void enemyLost (edict_t *pEnemy);

	inline void setFailedObject ( edict_t *pent ) 
	{ 
		m_FailedPhysObj = pent; 

		if ( m_NearestPhysObj == pent ) 
			m_NearestPhysObj = NULL;
	}

	bool checkStuck ();

	bool willCollide ( edict_t *pEntity, bool *bCanJump, float *fTime );

	edict_t *getFailedObject () { return m_FailedPhysObj; }

	virtual void touchedWpt ( CWaypoint *pWaypoint );

private:
	// blah blah
	MyEHandle m_NearestPhysObj;
	MyEHandle m_NearestBreakable;
	edict_t *m_FailedPhysObj;
	float m_flSprintTime;
	MyEHandle m_pHealthCharger;
	MyEHandle m_pHealthKit;
	MyEHandle m_pAmmoKit; // nearest healthkit
	MyEHandle m_pBattery; // nearest battery
	MyEHandle m_pCharger; // nearest charger
	MyEHandle m_pNearbyWeapon;
	MyEHandle m_pNearestButton;
	//MyEHandle m_pNearestBreakable;
	MyEHandle m_pAmmoCrate;
	edict_t *m_pCurrentWeapon;

	float m_fUseButtonTime;
	float m_fUseCrateTime;

	CBaseHandle *m_Weapons;

	float m_fFixWeaponTime;

	int m_iClip1;
	int m_iClip2;

	edict_t *m_pCarryingObject; // using grav gun
};

#endif