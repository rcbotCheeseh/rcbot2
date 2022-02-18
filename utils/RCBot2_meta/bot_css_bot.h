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
#ifndef __CSS_RCBOT_H__
#define __CSS_RCBOT_H__

#define CS_TEAM_UNASSIGNED 0
#define CS_TEAM_SPECTATOR 1
#define CS_TEAM_TERRORIST 2
#define CS_TEAM_COUNTERTERRORIST 3

#define CS_WEAPON_SLOT_PRIMARY 0
#define CS_WEAPON_SLOT_SECONDARY 1
#define CS_WEAPON_SLOT_MELEE 2
#define CS_WEAPON_SLOT_GRENADE 3
#define CS_WEAPON_SLOT_C4 4

class CCSSBotBuying;

// bot for CS Source
class CCSSBot : public CBot
{
public:
	bool isCSS() override { return true; }
    void init(bool bVarInit=false) override;
    void spawnInit() override;
	void died(edict_t *pKiller, const char *pszWeapon) override;
	void setup() override;
	void selectTeam();
	void selectModel();
	bool startGame() override;
	bool isAlive() override;
	bool isEnemy(edict_t *pEdict,bool bCheckWeapons = true) override;
    void handleWeapons() override;
    bool handleAttack(CBotWeapon *pWeapon, edict_t *pEnemy) override;
	void modAim(edict_t *pEntity, Vector &v_origin, Vector *v_desired_offset, Vector &v_size, float fDist, float fDist2D) override;
	void updateConditions() override;
	void modThink() override;
	void listenForPlayers() override;
	void freeMapMemory() override;
	void touchedWpt(CWaypoint *pWaypoint, int iNextWaypoint = -1, int iPrevWaypoint = -1) override;
	bool canGotoWaypoint(Vector vPrevWaypoint, CWaypoint *pWaypoint, CWaypoint *pPrev = NULL) override;
	bool setVisible(edict_t *pEntity, bool bVisible) override;
	virtual void modThinkSlow();
	unsigned int maxEntityIndex() override { return gpGlobals->maxEntities; }
	void getTasks (unsigned int iIgnore=0) override;
	virtual bool executeAction(eBotAction iAction);
	virtual void runBuy();
	virtual void say(const char *message);
	virtual void sayteam(const char *message);
	virtual void primaryattackCS(bool hold = false);
	virtual float getNextAttackDelay();
	virtual CBotWeapon *getPrimaryWeapon();
	virtual bool IsSniper();
	virtual void onRoundStart();
	virtual bool IsLeadingHostage();
private:
	edict_t *m_pCurrentWeapon;	  		// The bot current weapon
	float m_fNextAttackTime;	  		// Control timer for bot primary attack
	float m_fNextThinkSlow;		  		// Control timer for slow think
	float m_fVisibleEnemyTime;	  		// The last time my enemy was visible
	CBaseHandle m_hNearestBreakable; 	// Nearest breakable entity
	CCSSBotBuying *m_pBuyManager; 		// CSS Bot buy manager
};

#endif