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
#ifndef _BOT_SYNERGY_H_
#define _BOT_SYNERGY_H_

class CBotSynergy : public CBot
{
public:
    bool isSYN () override { return true; }
    void init (bool bVarInit=false) override;
    void spawnInit () override;
    void died ( edict_t *pKiller, const char *pszWeapon ) override;
    void modThink () override;
    void getTasks (unsigned int iIgnore=0) override;
    virtual bool executeAction(eBotAction iAction);
    virtual float getArmorPercent() { return 0.01f * m_pPlayerInfo->GetArmorValue(); }
    unsigned int maxEntityIndex() override { return gpGlobals->maxEntities; }
    bool isEnemy ( edict_t *pEdict, bool bCheckWeapons = true ) override;
    bool setVisible ( edict_t *pEntity, bool bVisible ) override;
    void touchedWpt ( CWaypoint *pWaypoint, int iNextWaypoint = -1, int iPrevWaypoint = -1 ) override;
    bool walkingTowardsWaypoint ( CWaypoint *pWaypoint, bool *bOffsetApplied, Vector &vOffset ) override;
    void reachedCoverSpot (int flags) override;
    void updateConditions () override; // Overridden due to Synergy's quirks
    void handleWeapons() override;
    bool handleAttack(CBotWeapon *pWeapon, edict_t *pEnemy) override;
    virtual bool needHealth();
    virtual bool needAmmo();
    virtual bool filterAmmo(edict_t *pAmmo, const char *szclassname);
protected:
    MyEHandle m_pNearbyWeapon; // weapons
    MyEHandle m_pNearbyHealthKit; // Healthkit
    MyEHandle m_pNearbyBattery; // Armor battery
    MyEHandle m_pNearbyAmmo; // ammo pickups
    MyEHandle m_pNearbyCrate; // ammo crate
    MyEHandle m_pNearbyGrenade; // grenades
    MyEHandle m_pNearbyMine; // combine mine
    MyEHandle m_pNearbyItemCrate; // breakable item crate
    MyEHandle m_pNearbyHealthCharger; // Health charger
    MyEHandle m_pNearbyArmorCharger; // Armor/Suit charger
    edict_t *m_pCurrentWeapon; // The bot current weapon
    float m_flSuitPower; // HEV suit power level, range: 100-0
    float m_flNextSprintTime; // Used to control the bot's sprinting
    float m_flUseCrateTime; // Use ammo crate time delay
    float m_flPickUpTime; // Pick ammo delay
    //float m_fGoToGoalTime; // Time control used to check if the bot should roam or not
};

#endif