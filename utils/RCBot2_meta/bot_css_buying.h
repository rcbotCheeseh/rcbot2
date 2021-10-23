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
#ifndef __BOT_CSS_BUYING_H__
#define __BOT_CSS_BUYING_H__

/**
 * Ammo: primammo, secammo
 * Armor: vest, vesthelm
 * Misc: defuser, nvgs
 * Pistols: usp, glock, p228, fiveseven, elite, deagle
 * Shotguns: m3, xm1014
 * SMGs: tmp, mac10, mp5navy, ump45, p90
 * Rifles: famas, galil, ak47, m4a1, aug, sg552
 * Snipers: scout, awp, sg550, g3sg1
 * Machine Guns: m249
 **/

typedef enum
{
    CS_BUY_AMMO_PRIMARY = 0,
    CS_BUY_AMMO_SECONDARY,
    CS_BUY_ARMOR_VEST,
    CS_BUY_ARMOR_VESTHELM,
    CS_BUY_MISC_DEFUSER,
    CS_BUY_MISC_NVGOGGLES,
    CS_BUY_PISTOL_USP,
    CS_BUY_PISTOL_GLOCK,
    CS_BUY_PISTOL_P228,
    CS_BUY_PISTOL_FIVESEVEN,
    CS_BUY_PISTOL_ELITES,
    CS_BUY_PISTOL_DEAGLE,
    CS_BUY_SHOTGUN_M3,
    CS_BUY_SHOTGUN_XM1014,
    CS_BUY_SMG_TMP,
    CS_BUY_SMG_MAC10,
    CS_BUY_SMG_MP5NAVY,
    CS_BUY_SMG_UMP45,
    CS_BUY_SMG_P90,
    CS_BUY_RIFLE_FAMAS,
    CS_BUY_RIFLE_GALIL,
    CS_BUY_RIFLE_AK47,
    CS_BUY_RIFLE_M4A1,
    CS_BUY_RIFLE_AUG,
    CS_BUY_RIFLE_SG552,
    CS_BUY_SNIPER_SCOUT,
    CS_BUY_SNIPER_AWP,
    CS_BUY_SNIPER_SG550,
    CS_BUY_SNIPER_G3SG1,
    CS_BUY_MG_M249,
    CS_BUY_GRENADE_EXPLOSIVE,
    CS_BUY_GRENADE_SMOKE,
    CS_BUY_GRENADE_FLASHBANG,
    CS_BUY_MAX,
}eCSSBuyItem;

/**
 * CS Buy Profile
 * Determines what kind of weapons the bot likes
 * Currently it's selected randomly
 **/
typedef enum
{
    CS_BUY_PROFILE_INVALID = -1,
    CS_BUY_PROFILE_SMG = 0,
    CS_BUY_PROFILE_SHOTGUNNER,
    CS_BUY_PROFILE_RIFLE,
    CS_BUY_PROFILE_SCOPEDRIFLE,
    CS_BUY_PROFILE_SNIPER,
    CS_BUY_PROFILE_AUTOSNIPER,
    CS_BUY_PROFILE_MACHINEGUNNER,
    CS_BUY_PROFILE_MAX
}eCSSBuyProfile;

/**
 * List of buy "types" for the bot
 **/
typedef enum
{
    CS_BUY_TYPE_INVALID = -1,
    CS_BUY_TYPE_ECO = 0, // Save money
    CS_BUY_TYPE_FORCE, // Save some money
    CS_BUY_TYPE_FULL, // Buy everything
    CS_BUY_TYPE_UPGRADE, // Upgrade current weapon
    CS_BUY_TYPE_MAX
}eCSSBuyType;

/**
 * This class manages the Counter-Strike: Source bot buying logic
 **/
class CCSSBotBuying
{
public:
    CCSSBotBuying(CBot *pBot);
    ~CCSSBotBuying();
    void reset();
    void update();
    void execute();
    void buyItem(eCSSBuyItem item);
    int getPrice(eCSSBuyItem item);
    bool canAfford(eCSSBuyItem item, int money = -1);
    bool wantsToBuy();
    void selectRandomBuyProfile();
    void setBuyProfile(eCSSBuyProfile profile);
    eCSSBuyProfile getBuyProfile();
    void onRoundStart();
    void onDeath();

private:
    bool shouldBuyArmor(eCSSBuyType buytype);
    bool shouldBuyDefuser(eCSSBuyType buytype);
    void buildBuyList(std::queue<eCSSBuyItem> &queue, int &cost);
    eCSSBuyType determineBuyType();
    void processBuyList(std::queue<eCSSBuyItem> &queue);

    CBot *m_pBot; // Bot pointer
    int m_money; // Amount of money the bot have
    int m_deaths; // How many times the bot died
    float m_fLastBuyTime; // The last time the bot bought something
    CBotWeapon *m_pPrimaryWeapon; // Primary weapon
    CBotWeapon *m_pSecondaryWeapon; // Secondary weapon
    eCSSBuyProfile m_eProfile; // Bot weapon preference profile
};

#endif