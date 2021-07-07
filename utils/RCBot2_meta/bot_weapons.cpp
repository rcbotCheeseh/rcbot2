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
#include "bot_globals.h"
#include "bot_weapons.h"
#include "bot_getprop.h"

const char *g_szDODWeapons[] = 
{
	"weapon_amerknife", 
	"weapon_spade", 
	"weapon_colt", 
	"weapon_p38", 
	"weapon_m1carbine", 
	"weapon_c96",
    "weapon_garand", 
	"weapon_k98", 
	"weapon_thompson", 
	"weapon_mp40", 
	"weapon_bar", 
	"weapon_mp44",
    "weapon_spring",
	"weapon_k98_scoped",
	"weapon_30cal", 
	"weapon_mg42", 
	"weapon_bazooka", 
	"weapon_pschreck",
    "weapon_riflegren_us", 
	"weapon_riflegren_ger", 
	"weapon_frag_us", 
	"weapon_frag_ger", 
	"weapon_smoke_us", 
	"weapon_smoke_ger",
	"weapon_basebomb"
};

const char *g_szHL2DMWeapons[] =
{
	"weapon_pistol",
	"weapon_crowbar",
	"weapon_357",
	"weapon_smg1",
	"weapon_ar2",
	"weapon_frag",
	"weapon_stunstick",
	"weapon_crossbow",
	"weapon_rpg",
	"weapon_slam",
	"weapon_shotgun",
	"weapon_physcannon"
};

  /*  0, 0, 1, 2, 6, 3,
    4, 5, 8, 8, 9, 8,
    7, 5, 10, 11, 12, 12,
    21, 22, 13, 14, 17, 18*/

WeaponsData_t DODWeaps[] =
{
/*
	slot, id , weapon name, flags, min dist, max dist, ammo index, preference
	*/
	{1,DOD_WEAPON_AMERKNIFE, g_szDODWeapons[0],	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,100,-1,1,0},
	{1,DOD_WEAPON_SPADE, g_szDODWeapons[1],		WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,100,-1,1,0},
	{2,DOD_WEAPON_COLT, g_szDODWeapons[2],		WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,800,1,2,0},
	{2,DOD_WEAPON_P38, g_szDODWeapons[3],		WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,800,2,2,0},
	{3,DOD_WEAPON_M1, g_szDODWeapons[4],		WEAP_FL_PRIM_ATTACK,0,1600,6,4,0},
	{3,DOD_WEAPON_C96, g_szDODWeapons[5],		WEAP_FL_PRIM_ATTACK,0,1600,-1,4,0},
	{3,DOD_WEAPON_GARAND, g_szDODWeapons[6],	WEAP_FL_PRIM_ATTACK|WEAP_FL_ZOOMABLE,0,1600,-1,3,0},
	{3,DOD_WEAPON_K98, g_szDODWeapons[7],		WEAP_FL_PRIM_ATTACK|WEAP_FL_ZOOMABLE,0,1600,-1,3,0},
	{3,DOD_WEAPON_THOMPSON, g_szDODWeapons[8],	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE_SEC_ATT,0,900,-1,3,0},
	{3,DOD_WEAPON_MP40, g_szDODWeapons[9],		WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE_SEC_ATT,0,1600,-1,4,0},
	{3,DOD_WEAPON_BAR, g_szDODWeapons[10],		WEAP_FL_PRIM_ATTACK,0,1600,-1,3,0},
	{3,DOD_WEAPON_MP44, g_szDODWeapons[11],		WEAP_FL_PRIM_ATTACK,0,1600,-1,3,0},
	{3,DOD_WEAPON_SPRING, g_szDODWeapons[12],	WEAP_FL_PRIM_ATTACK|WEAP_FL_SCOPE|WEAP_FL_CANTFIRE_NORM|WEAP_FL_ZOOMABLE,0,3200,-1,3,0},
	{3,DOD_WEAPON_K98_SCOPED, g_szDODWeapons[13],	WEAP_FL_PRIM_ATTACK|WEAP_FL_SCOPE|WEAP_FL_CANTFIRE_NORM|WEAP_FL_ZOOMABLE,0,3200,-1,4,0},
	{3,DOD_WEAPON_20CAL, g_szDODWeapons[14],		WEAP_FL_PRIM_ATTACK|WEAP_FL_DEPLOYABLE|WEAP_FL_HIGH_RECOIL,0,2000,-1,4,0},
	{3,DOD_WEAPON_MG42, g_szDODWeapons[15],			WEAP_FL_PRIM_ATTACK|WEAP_FL_DEPLOYABLE|WEAP_FL_HIGH_RECOIL,0,2000,-1,4,0},
	{3,DOD_WEAPON_BAZOOKA, g_szDODWeapons[16],		WEAP_FL_PROJECTILE|WEAP_FL_EXPLOSIVE|WEAP_FL_PRIM_ATTACK|WEAP_FL_CANTFIRE_NORM|WEAP_FL_DEPLOYABLE,500,3200,-1,5,1300},
	{3,DOD_WEAPON_PSCHRECK, g_szDODWeapons[17],		WEAP_FL_PROJECTILE|WEAP_FL_EXPLOSIVE|WEAP_FL_PRIM_ATTACK|WEAP_FL_CANTFIRE_NORM|WEAP_FL_DEPLOYABLE,500,3200,-1,5,1300},
	{3,DOD_WEAPON_RIFLEGREN_US, g_szDODWeapons[18],	WEAP_FL_EXPLOSIVE_SEC|WEAP_FL_PRIM_ATTACK,500,1800,-1,4,0},
	{3,DOD_WEAPON_RIFLEGREN_GER, g_szDODWeapons[19],	WEAP_FL_EXPLOSIVE_SEC|WEAP_FL_PRIM_ATTACK,500,1800,-1,4,0},
	{3,DOD_WEAPON_FRAG_US, g_szDODWeapons[20],		WEAP_FL_PROJECTILE|WEAP_FL_GRENADE|WEAP_FL_EXPLOSIVE|WEAP_FL_NONE,0,1200,-1,1,0},
	{3,DOD_WEAPON_FRAG_GER, g_szDODWeapons[21],		WEAP_FL_PROJECTILE|WEAP_FL_GRENADE|WEAP_FL_EXPLOSIVE|WEAP_FL_NONE,0,1200,-1,1,0},
	{3,DOD_WEAPON_SMOKE_US, g_szDODWeapons[22],		WEAP_FL_PROJECTILE|WEAP_FL_GRENADE,0,1200,-1,1,0},
	{3,DOD_WEAPON_SMOKE_GER, g_szDODWeapons[23],	WEAP_FL_PROJECTILE|WEAP_FL_GRENADE,0,1200,-1,1,0},
	{3,DOD_WEAPON_BOMB, g_szDODWeapons[24], WEAP_FL_NONE,0,0,-1,1,0},
	{ 0, 0, "\0", 0, 0, 0, 0, 0, 0 }//signal last weapon
};


WeaponsData_t HL2DMWeaps[] =
{
/*
	slot, id , weapon name, flags, min dist, max dist, ammo index, preference
*/
	{2,HL2DM_WEAPON_PISTOL,		g_szHL2DMWeapons[0],	WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,1000,-1,1,0},
	{1,HL2DM_WEAPON_CROWBAR,	g_szHL2DMWeapons[1],	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,128,-1,1,0},
	{2,HL2DM_WEAPON_357,		g_szHL2DMWeapons[2],	WEAP_FL_PRIM_ATTACK,0,768,-1,2,0},
	{3,HL2DM_WEAPON_SMG1,		g_szHL2DMWeapons[3],	WEAP_FL_PRIM_ATTACK|WEAP_FL_SEC_ATTACK,0,1400,-1,2,0},
	{2,HL2DM_WEAPON_AR2,		g_szHL2DMWeapons[4],	WEAP_FL_PRIM_ATTACK|WEAP_FL_SEC_ATTACK,0,1400,-1,3,0},
	{1,HL2DM_WEAPON_FRAG,		g_szHL2DMWeapons[5],	WEAP_FL_GRENADE|WEAP_FL_EXPLOSIVE,0,180,-1,1,0},
	{2,HL2DM_WEAPON_STUNSTICK,	g_szHL2DMWeapons[6],	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,128,-1,1,0},
	{3,HL2DM_WEAPON_CROSSBOW,	g_szHL2DMWeapons[7],	WEAP_FL_PRIM_ATTACK|WEAP_FL_SCOPE|WEAP_FL_UNDERWATER,0,2000,-1,2,0},
	{2,HL2DM_WEAPON_RPG,		g_szHL2DMWeapons[8],	WEAP_FL_PROJECTILE|WEAP_FL_EXPLOSIVE|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,400,2000,-1,3,1000.0f},
	{1,HL2DM_WEAPON_SLAM,		g_szHL2DMWeapons[9],	WEAP_FL_EXPLOSIVE,0,180,-1,1,0},
	{2,HL2DM_WEAPON_SHOTGUN,	g_szHL2DMWeapons[10],	WEAP_FL_PRIM_ATTACK,0,768,-1,2,0},
	{1,HL2DM_WEAPON_PHYSCANNON,	g_szHL2DMWeapons[11],	WEAP_FL_GRAVGUN|WEAP_FL_PRIM_ATTACK,0,768,-1,4,0},
	{ 0, 0, "\0", 0, 0, 0, 0, 0, 0 }//signal last weapon
};

//SENTRYGUN ID = 34

WeaponsData_t TF2Weaps[] =
{

//{"slot, id , weapon name, flags, min dist, max dist, ammo index, preference

	{TF2_SLOT_MELEE,TF2_WEAPON_BAT,		"tf_weapon_bat",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_BOTTLE,		"tf_weapon_bottle",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_FIREAXE,			"tf_weapon_fireaxe",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_CLUB,				"tf_weapon_club",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_KNIFE,				"tf_weapon_knife",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,220,0,2,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_FISTS,				"tf_weapon_fists",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_SHOVEL,				"tf_weapon_shovel",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_WRENCH,				"tf_weapon_wrench",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,3,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_BONESAW,			"tf_weapon_bonesaw",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_PRMRY,TF2_WEAPON_SHOTGUN_PRIMARY,	"tf_weapon_shotgun_primary",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,800,1,2,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_SHOTGUN_SOLDIER,	"tf_weapon_shotgun_soldier",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,500,2,2,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_SHOTGUN_HWG,		"tf_weapon_shotgun_hwg",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,800,2,2,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_SHOTGUN_PYRO,		"tf_weapon_shotgun_pyro",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,800,2,2,0},
	{TF2_SLOT_PRMRY,TF2_WEAPON_SCATTERGUN,			"tf_weapon_scattergun",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,800,1,3,0},
	{TF2_SLOT_PRMRY,TF2_WEAPON_SNIPERRIFLE,		"tf_weapon_sniperrifle",	WEAP_FL_SCOPE|WEAP_FL_PRIM_ATTACK,1000,4000,1,3,0},
	{TF2_SLOT_PRMRY,TF2_WEAPON_MINIGUN,			"tf_weapon_minigun",	WEAP_FL_PRIM_ATTACK|WEAP_FL_HOLDATTACK,120,1800,1,3,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_SMG,				"tf_weapon_smg",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,1000,2,2,0},
	{TF2_SLOT_PRMRY,TF2_WEAPON_SYRINGEGUN,			"tf_weapon_syringegun_medic",	WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,1000,1,2,0},
	{TF2_SLOT_PRMRY,TF2_WEAPON_ROCKETLAUNCHER,		"tf_weapon_rocketlauncher",	WEAP_FL_PRIM_ATTACK|WEAP_FL_EXPLOSIVE|WEAP_FL_UNDERWATER,BLAST_RADIUS,4096,1,3,TF2_ROCKETSPEED},
	{TF2_SLOT_PRMRY,TF2_WEAPON_GRENADELAUNCHER,	"tf_weapon_grenadelauncher",	WEAP_FL_PROJECTILE|WEAP_FL_PRIM_ATTACK|WEAP_FL_EXPLOSIVE|WEAP_FL_UNDERWATER,100,1200,1,2,TF2_GRENADESPEED},
	{TF2_SLOT_SCNDR,TF2_WEAPON_PIPEBOMBS,			"tf_weapon_pipebomblauncher",	WEAP_FL_NONE,0,1000,2,1,0},
	{TF2_SLOT_PRMRY,TF2_WEAPON_FLAMETHROWER,		"tf_weapon_flamethrower",	WEAP_FL_DEFLECTROCKETS|WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_HOLDATTACK|WEAP_FL_SPECIAL,0,400,1,3,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_PISTOL,				"tf_weapon_pistol",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,2000,2,1,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_PISTOL_SCOUT,		"tf_weapon_pistol_scout",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,1800,2,2,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_REVOLVER,			"tf_weapon_revolver",	WEAP_FL_KILLPIPEBOMBS|WEAP_FL_PRIM_ATTACK|WEAP_FL_UNDERWATER,0,1400,2,1,0},
	{ TF2_SLOT_PRMRY, TF2_WEAPON_POMSON6000, "tf_weapon_drg_pomson", WEAP_FL_PRIM_ATTACK | WEAP_FL_UNDERWATER, 0, 800, 1, 2, 0 },
	//{TF2_SLOT_PDA,TF2_WEAPON_PDA_ENGI_BUILD,		"tf_weapon_pda_engineer_build",	WEAP_FL_NONE,0,100,0,1,0},
	// this class is used with all classes that can use shotgun but the slot might be different
	{ TF2_SLOT_SCNDR, TF2_WEAPON_SHOTGUN, "tf_weapon_shotgun", WEAP_FL_KILLPIPEBOMBS | WEAP_FL_PRIM_ATTACK | WEAP_FL_UNDERWATER, 0, 500, 2, 2, 0 },
	//{TF2_SLOT_OTHER,TF2_WEAPON_PDA_ENGI_DESTROY,	"tf_weapon_pda_engineer_destroy",	WEAP_FL_NONE,0,100,0,1,0},
	//{TF2_SLOT_PDA, TF2_WEAPON_PDA_SPY, "tf_weapon_pda_spy", WEAP_FL_NONE, 0, 100, 0, 1, 0 },
	{ TF2_SLOT_PRMRY, TF2_WEAPON_FRONTIERJUSTICE, "tf_weapon_sentry_revenge", WEAP_FL_KILLPIPEBOMBS | WEAP_FL_PRIM_ATTACK | WEAP_FL_UNDERWATER, 0, 800, 1, 2, 0 },
	{TF2_SLOT_PDA, TF2_WEAPON_BUILDER, "tf_weapon_builder", WEAP_FL_NONE, 0, 100, 0, 1, 0 },
	{TF2_SLOT_SCNDR,TF2_WEAPON_MEDIGUN,			"tf_weapon_medigun",	WEAP_FL_NONE,0,100,0,1,0},
	{TF2_SLOT_PDA, TF2_WEAPON_INVIS, "tf_weapon_invis", WEAP_FL_NONE, 0, 100, 0, 1, 0 },
	{TF2_SLOT_SCNDR,TF2_WEAPON_BUFF_ITEM,	"tf_weapon_buff_item",	WEAP_FL_NONE,0,100,0,1,0},
	{ TF2_SLOT_SCNDR, TF2_WEAPON_FLAREGUN, "tf_weapon_flaregun", WEAP_FL_PRIM_ATTACK, 0, 1600, 2, 2, TF2_GRENADESPEED },
	{TF2_SLOT_PDA, TF2_WEAPON_SENTRYGUN, "obj_sentrygun", 0, 0, 0, 0, 0, 0 },
	{TF2_SLOT_MELEE,TF2_WEAPON_SAXXY,		"saxxy",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,150,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_BAT_WOOD,		"tf_weapon_bat_wood",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_LUNCHBOX_DRINK,		"tf_weapon_lunchbox_drink",	WEAP_FL_NONE,0,180,0,1,0},
	{ TF2_SLOT_PRMRY, TF2_WEAPON_BOW, "tf_weapon_compound_bow", WEAP_FL_SCOPE | WEAP_FL_PRIM_ATTACK | WEAP_FL_PROJECTILE, 400, 2500, 1, 3, 1875},
	{TF2_SLOT_SCNDR,TF2_WEAPON_JAR,		"tf_weapon_jar",	WEAP_FL_NONE,0,180,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_BAT_FISH,		"tf_weapon_bat_fish",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,180,0,1,0},
	{TF2_SLOT_PRMRY,TF2_WEAPON_DIRECTHIT,		"tf_weapon_rocketlauncher_directhit",	WEAP_FL_PRIM_ATTACK|WEAP_FL_EXPLOSIVE|WEAP_FL_UNDERWATER,BLAST_RADIUS,4096,1,3,TF2_ROCKETSPEED*1.8},
	{TF2_SLOT_MELEE,TF2_WEAPON_SWORD,		"tf_weapon_sword",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,190,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_KATANA,		"tf_weapon_katana",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,210,0,1,0},
	{ TF2_SLOT_PRMRY, TF2_WEAPON_COWMANGLER, "tf_weapon_particle_cannon", WEAP_FL_KILLPIPEBOMBS | WEAP_FL_PRIM_ATTACK | WEAP_FL_UNDERWATER, 0, 1500, 1, 2, TF2_ROCKETSPEED },
	{TF2_SLOT_PRMRY,TF2_WEAPON_CROSSBOW,		"tf_weapon_crossbow",	WEAP_FL_PRIM_ATTACK,600,2500,1,3,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_CLEAVER,		"tf_weapon_cleaver",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,150,0,1,0},
	{TF2_SLOT_MELEE,TF2_WEAPON_BAT_GIFTWRAP,		"tf_weapon_bat_giftwrap",	WEAP_FL_PRIM_ATTACK|WEAP_FL_MELEE|WEAP_FL_UNDERWATER,0,150,0,1,0},
	{TF2_SLOT_SCNDR,TF2_WEAPON_RAYGUN,		"tf_weapon_raygun",	WEAP_FL_PRIM_ATTACK,100,2000,2,2,0},
	{0, 0, "\0", 0, 0, 0, 0, 0, 0}//signal last weapon
};	

bool CBotWeapon :: needToReload (CBot *pBot) 
{ 
	if ( m_iClip1 )
	{
		return (*m_iClip1 == 0) && (getAmmo(pBot)>0);
	}

	return false;
}

// static init (all weapons in game)
std::vector<CWeapon*> CWeapons :: m_theWeapons;

int CBotWeapon :: getAmmo (CBot *pBot, int type )
{
	if ( type == AMMO_PRIM )
		return pBot->getAmmo(m_pWeaponInfo->getAmmoIndex1());

	if ( type == AMMO_SEC )
		return pBot->getAmmo(m_pWeaponInfo->getAmmoIndex2());


	return 0;
}


bool CBotWeapons::hasExplosives( void )
{
	CBotWeapon *pWeapon;

	for ( int i = 0; i < MAX_WEAPONS; i ++ )
	{
		pWeapon = &(m_theWeapons[i]);
		// find weapon info from weapon id
		if ( pWeapon->hasWeapon() && pWeapon->isExplosive() )
		{
			if ( pWeapon->getAmmo(m_pBot) > 1 )
				return true;
		}
	}

	return false;
}


bool CBotWeapons::hasWeapon(int id)
{
	for (int i = 0; i < MAX_WEAPONS; i++)
	{
		if (m_theWeapons[i].getWeaponInfo() == NULL)
			continue;
		if (m_theWeapons[i].hasWeapon() == false)
			continue;
		if (m_theWeapons[i].getID() == id)
			// find weapon info from weapon id
		{
			return true;
		}
	}
	return false;
}
// Bot Weapons
CBotWeapons::CBotWeapons(CBot *pBot)
{
	m_pBot = pBot;
	clearWeapons();
	/*
	for ( int i = 0; i < MAX_WEAPONS; i ++ )
	{
	// find weapon info from weapon id
	m_theWeapons[i].setWeapon(CWeapons::getWeapon(i));
	}
	*/
	m_fUpdateWeaponsTime = 0;
	m_iWeaponsSignature = 0x0;
}

edict_t *CWeapons :: findWeapon ( edict_t *pPlayer, const char *pszWeaponName )
{
	register unsigned short int j;
	CBaseHandle *m_Weapons = CClassInterface::getWeaponList(pPlayer);
	edict_t *pWeapon;
	CBaseHandle *m_Weapon_iter = m_Weapons;
	// loop through the weapons array and see if it is in the CBaseCombatCharacter

	pWeapon = NULL;

	for ( j = 0; j < MAX_WEAPONS; j ++ )
	{
		pWeapon = INDEXENT(m_Weapon_iter->GetEntryIndex());

		// TODO get familiar with validity of handles / edicts
		if (pWeapon && !pWeapon->IsFree() && strcmp(pWeapon->GetClassName(),pszWeaponName) == 0 )
			return pWeapon;

		m_Weapon_iter++;
	}

	return NULL;
}


bool CBotWeapons::update(bool bOverrideAllFromEngine)
{
	// create mask of weapons data
	short int i = 0;
	unsigned short int iWeaponsSignature = 0x0; // check sum of weapons
	edict_t *pWeapon;
	CBaseHandle *m_Weapons = CClassInterface::getWeaponList(m_pBot->getEdict());
	CBaseHandle *m_Weapon_iter;

	m_Weapon_iter = m_Weapons;

	for (i = 0; i < MAX_WEAPONS; i++)
	{
		// create a 'hash' of current weapons
		pWeapon = (m_Weapon_iter == NULL) ? NULL : INDEXENT(m_Weapon_iter->GetEntryIndex());
		iWeaponsSignature += ((unsigned int)pWeapon) + ((pWeapon == NULL) ? 0 : (unsigned int)CClassInterface::getWeaponState(pWeapon));
		m_Weapon_iter++;
	}

	// if weapons have changed this will be different
	if (iWeaponsSignature != m_iWeaponsSignature) // m_fUpdateWeaponsTime < engine->Time() )
	{
		this->clearWeapons();

		int iWeaponState;
		register unsigned short int i;
		bool bFound;

		const char *pszClassname;

		CBaseHandle *m_Weapons = CClassInterface::getWeaponList(m_pBot->getEdict());
		CBotWeapon *m_BotWeapon_iter = m_theWeapons;

		// loop through the weapons array and see if it is in the CBaseCombatCharacter
		for (i = 0; i < MAX_WEAPONS; i++)
		{
			m_Weapon_iter = &m_Weapons[i];
			iWeaponState = 0;
			bFound = false;

			pWeapon = INDEXENT(m_Weapon_iter->GetEntryIndex());

			if (!pWeapon || pWeapon->IsFree())
			{
				continue;
			}

			iWeaponState = CClassInterface::getWeaponState(pWeapon);

			pszClassname = pWeapon->GetClassName();

			CWeapon *pWeaponInfo = CWeapons::getWeapon(pszClassname);

			if (pWeaponInfo != NULL)
			{
				if (iWeaponState != WEAPON_NOT_CARRIED)
				{
					CBotWeapon *pAdded = addWeapon(pWeaponInfo, i, pWeapon, bOverrideAllFromEngine);
					pAdded->setHasWeapon(true);
				}
			}
		}

		// check again in 1 second
		m_fUpdateWeaponsTime = engine->Time() + 1.0f;

		m_iWeaponsSignature = iWeaponsSignature;

		return true; // change
	}

	return false;
}
/*
bool CBotWeapons ::update ( bool bOverrideAllFromEngine )
{
	// create mask of weapons data
	short int i = 0;
	unsigned short int iWeaponsSignature = 0x0; // check sum of weapons
	edict_t *pWeapon;
	CBaseHandle *m_Weapons = CClassInterface::getWeaponList(m_pBot->getEdict());
	CBaseHandle *m_Weapon_iter;

	m_Weapon_iter = m_Weapons;

	for ( i = 0; i < MAX_WEAPONS; i ++ )
	{
		// create a 'hash' of current weapons
		pWeapon = (m_Weapon_iter==NULL) ? NULL : INDEXENT(m_Weapon_iter->GetEntryIndex());
		iWeaponsSignature += ((unsigned int)pWeapon) + ((pWeapon == NULL) ? 0 : (unsigned int)CClassInterface::getWeaponState(pWeapon));
		m_Weapon_iter++;
	}

	// if weapons have changed this will be different
	if ( iWeaponsSignature != m_iWeaponsSignature ) // m_fUpdateWeaponsTime < engine->Time() )
	{
		int iWeaponState;
		register unsigned short int i,j;
		bool bFound;

		CBaseHandle *m_Weapons = CClassInterface::getWeaponList(m_pBot->getEdict());
		CBotWeapon *m_BotWeapon_iter = m_theWeapons;

		// loop through the weapons array and see if it is in the CBaseCombatCharacter
		for ( i = 0; i < MAX_WEAPONS; i ++ )
		{
			m_Weapon_iter = m_Weapons;
			iWeaponState = 0;
			bFound = false;
			pWeapon = NULL;

			if ( (m_BotWeapon_iter->getWeaponInfo()!=NULL) && (m_BotWeapon_iter->getWeaponInfo()->getWeaponName()!=NULL) )
			{
				for ( j = 0; j < MAX_WEAPONS; j ++ )
				{
					pWeapon = INDEXENT(m_Weapon_iter->GetEntryIndex());

					if ( pWeapon && CBotGlobals::entityIsValid(pWeapon) && strcmp(pWeapon->GetClassName(),m_BotWeapon_iter->getWeaponInfo()->getWeaponName())==0 )
					{
						iWeaponState = CClassInterface::getWeaponState(pWeapon);
						// found it
						bFound = true;
						break;
					}

					m_Weapon_iter++;
				}

				if ( bFound && pWeapon && (iWeaponState != WEAPON_NOT_CARRIED) )
				{
					if ( !m_BotWeapon_iter->hasWeapon() )
						addWeapon(m_BotWeapon_iter->getID(),pWeapon,bOverrideAllFromEngine);
					else if ( m_BotWeapon_iter->getWeaponEntity() != pWeapon )
						m_BotWeapon_iter->setWeaponEntity(pWeapon,bOverrideAllFromEngine);
				}
				else
				{
					if ( m_BotWeapon_iter->hasWeapon() )
						m_BotWeapon_iter->setHasWeapon(false);
				}
			}

			m_BotWeapon_iter++;
		}

		// check again in 1 second
		m_fUpdateWeaponsTime = engine->Time() + 1.0f;

		m_iWeaponsSignature = iWeaponsSignature;

		return true; // change
	}

	return false;
}*/

CBotWeapon *CBotWeapons :: getBestWeapon ( edict_t *pEnemy, bool bAllowMelee, bool bAllowMeleeFallback, bool bMeleeOnly, bool bExplosivesOnly, bool bIgnorePrimaryMinimum )
{
	CBotWeapon *m_theBestWeapon = NULL;
	CBotWeapon *m_FallbackMelee = NULL;
	int iBestPreference = 0;
	Vector vEnemyOrigin;

	if ( pEnemy )
		vEnemyOrigin = CBotGlobals::entityOrigin(pEnemy);
	else
		vEnemyOrigin = m_pBot->getOrigin();

	float flDist = 0;

	if ( pEnemy )
		flDist = m_pBot->distanceFrom(vEnemyOrigin);

	for ( unsigned int i = 0; i < MAX_WEAPONS; i ++ )
	{
		CBotWeapon *pWeapon = &(m_theWeapons[i]);

		if ( !pWeapon )
			continue;

		if ( !pWeapon->hasWeapon() )
			continue;

		if ( bMeleeOnly && !pWeapon->isMelee() )
			continue;

		if ( !bAllowMelee && pWeapon->isMelee() )
			continue;

		if ( bExplosivesOnly && !pWeapon->isExplosive() )
			continue;

		if ( !pWeapon->isMelee() || pWeapon->isSpecial() )
		{
			if ( pWeapon->outOfAmmo(m_pBot) )
				continue;
		}

		if ( !pWeapon->canAttack() )
			continue;

		if ( m_pBot->isUnderWater() && !pWeapon->canUseUnderWater() )
			continue;

		if ( !pWeapon->primaryInRange(flDist) )
		{
			if ( pWeapon->isMelee() && !pWeapon->isSpecial() )
				m_FallbackMelee = pWeapon;

			if ( !pWeapon->isExplosive() || !bIgnorePrimaryMinimum )
				continue; // ignore explosive range if I'm invincible
		}

		if ( pWeapon->getPreference() > iBestPreference )
		{
			iBestPreference = pWeapon->getPreference();
			m_theBestWeapon = pWeapon;
		}
	}

	if ( bMeleeOnly || (bAllowMeleeFallback && ((m_theBestWeapon == NULL) && (flDist < 400.0f) && (fabs(vEnemyOrigin.z-m_pBot->getOrigin().z)<BOT_JUMP_HEIGHT))) )
		m_theBestWeapon = m_FallbackMelee;

	return m_theBestWeapon;
}

void CBotWeapon::setWeaponEntity(edict_t *pent, bool bOverrideAmmoTypes)
{
	m_pEnt = pent;
	m_iClip1 = CClassInterface::getWeaponClip1Pointer(pent);
	m_iClip2 = CClassInterface::getWeaponClip2Pointer(pent);

	if (bOverrideAmmoTypes)
	{
		int iAmmoType1, iAmmoType2;
		CClassInterface::getAmmoTypes(pent, &iAmmoType1, &iAmmoType2);
		m_pWeaponInfo->setAmmoIndex(iAmmoType1, iAmmoType2);
	}

	setWeaponIndex(ENTINDEX(m_pEnt));
}


CBotWeapon *CBotWeapons::addWeapon(CWeapon *pWeaponInfo, int iId, edict_t *pent, bool bOverrideAll)
{
	register int i = 0;
	Vector origin;
	const char *classname;
	edict_t *pEnt = NULL;

	m_theWeapons[iId].setHasWeapon(true);
	m_theWeapons[iId].setWeapon(pWeaponInfo);

	if (!m_theWeapons[iId].getWeaponInfo())
		return NULL;

	classname = pWeaponInfo->getWeaponName();

	origin = m_pBot->getOrigin();

	// if entity comes from the engine use the entity
	if (pent)
	{
		m_theWeapons[iId].setWeaponEntity(pent, bOverrideAll);
	}
	else // find the weapon entity
	{
		for (i = (gpGlobals->maxClients + 1); i <= gpGlobals->maxEntities; i++)
		{
			pEnt = INDEXENT(i);

			if (pEnt && CBotGlobals::entityIsValid(pEnt))
			{
				if (CBotGlobals::entityOrigin(pEnt) == origin)
				{
					if (strcmp(pEnt->GetClassName(), classname) == 0)
					{
						m_theWeapons[iId].setWeaponEntity(pEnt, bOverrideAll);// .setWeaponIndex(ENTINDEX(pEnt));

						return &m_theWeapons[iId];
					}
				}
			}
		}
	}

	return &m_theWeapons[iId];

}
/*
void CBotWeapons :: addWeapon ( int iId, edict_t *pent, bool bOverrideAll )
{
register int i = 0;
Vector origin;
const char *classname;
CWeapon *pWeapon;
edict_t *pEnt = NULL;

m_theWeapons[iId].setHasWeapon(true);

pWeapon = m_theWeapons[iId].getWeaponInfo();

if ( !pWeapon )
return;

classname = pWeapon->getWeaponName();

origin = m_pBot->getOrigin();

// if entity comes from the engine use the entity
if ( pent )
{
m_theWeapons[iId].setWeaponEntity(pent,bOverrideAll);
}
else // find the weapon entity
{
for ( i = (gpGlobals->maxClients+1); i <= gpGlobals->maxEntities; i ++ )
{
pEnt = INDEXENT(i);

if ( pEnt && CBotGlobals::entityIsValid(pEnt) )
{
if ( CBotGlobals::entityOrigin(pEnt) == origin )
{
if ( strcmp(pEnt->GetClassName(),classname) == 0 )
{
m_theWeapons[iId].setWeaponEntity(pEnt, bOverrideAll);// .setWeaponIndex(ENTINDEX(pEnt));

return;
}
}
}
}
}

}*/
CBotWeapon *CBotWeapons :: getWeapon ( CWeapon *pWeapon )
{
	for ( register unsigned int i = 0; i < MAX_WEAPONS; i ++ )
	{
		if ( m_theWeapons[i].getWeaponInfo() == pWeapon )
			return &(m_theWeapons[i]);
	}

	return NULL;
}

CBotWeapon *CBotWeapons :: getCurrentWeaponInSlot ( int iSlot )
{
	for ( register unsigned int i = 0; i < MAX_WEAPONS; i ++ )
	{
		if (  m_theWeapons[i].hasWeapon() && m_theWeapons[i].getWeaponInfo() && (m_theWeapons[i].getWeaponInfo()->getSlot()==iSlot) )
			return &(m_theWeapons[i]);
	}

	return NULL;
}

const char *szWeaponFlags[] = {
	 "primary_attack" ,
	 "secondary_attack" ,
	 "explosive" ,
	 "melee" ,
	 "underwater" ,
	 "hold_attack" ,
	 "special" ,
	 "can_kill_pipes" ,
	 "can_deflect_rockets" ,
	 "is_grav_gun" ,
	 "has_explosive_secondary" ,
	 "is_zoomable" ,
	 "is_deployable_dods" ,
	 "has_melee_secondary" ,
	 "has_fire_select_mode_dods" ,
	 "cant_be_fired_unzoomed_undeployed_dods" ,
	 "is_grenade" ,
	 "has_high_recoil_dods" ,
	 "has_scope" ,
	 "weapon_fires_projectile" ,
	 "\0"
};

void CWeapons::loadWeapons(const char *szWeaponListName, WeaponsData_t *pDefault)
{
	if ((szWeaponListName != NULL) && (szWeaponListName[0] != 0))
	{
		KeyValues *kv = new KeyValues("Weapons");
		char szFilename[1024];

		CBotGlobals::buildFileName(szFilename, "weapons", BOT_CONFIG_FOLDER, "ini", false);

		if (kv)
		{
			if (kv->LoadFromFile(filesystem, szFilename, NULL))
			{
				kv = kv->FindKey(szWeaponListName);

				if (kv)
				{
					kv = kv->GetFirstSubKey();

					if (0)
						kv = kv->GetFirstTrueSubKey();

					while (kv != NULL)
					{
						WeaponsData_t newWeapon;

						memset(&newWeapon, 0, sizeof(WeaponsData_t));

						const char *szKeyName = kv->GetName();

						char lowered[64];

						strncpy(lowered, szKeyName, 63);
						lowered[63] = 0;

						__strlow(lowered);

						newWeapon.szWeaponName = CStrings::getString(lowered);
						newWeapon.iId = kv->GetInt("id");
						newWeapon.iSlot = kv->GetInt("slot");
						newWeapon.minPrimDist = kv->GetFloat("minPrimDist");
						newWeapon.maxPrimDist = kv->GetFloat("maxPrimDist");
						newWeapon.m_fProjSpeed = kv->GetFloat("m_fProjSpeed");
						newWeapon.m_iAmmoIndex = kv->GetInt("m_iAmmoIndex");
						newWeapon.m_iPreference = kv->GetInt("m_iPreference");

						KeyValues *flags = kv->FindKey("flags");

						if (flags)
						{
							int i = 0;

							while (szWeaponFlags[i][0] != '\0')
							{
								if (flags->FindKey(szWeaponFlags[i]) && (flags->GetInt(szWeaponFlags[i]) == 1))
									newWeapon.m_iFlags |= (1 << i);

								i++;
							}
						}

						addWeapon(new CWeapon(&newWeapon));

						kv = kv->GetNextTrueSubKey();
					}
				}

			}


			kv->deleteThis();

		}
	}

	if (pDefault!=NULL)
	{
		// No weapons from INI file then add default
		if (m_theWeapons.size() == 0)
		{
			while (pDefault->szWeaponName[0] != '\0')
			{
				addWeapon(new CWeapon(pDefault));
				pDefault++;
			}
		}
	}
}


void CBotWeapons::clearWeapons()
{
	for (register unsigned short i = 0; i < MAX_WEAPONS; i++)
	{
		memset(&m_theWeapons[i], 0, sizeof(CBotWeapon));
		//m_theWeapons[i].setHasWeapon(false);
	}
}

// returns weapon with highest priority even if no ammo
CBotWeapon *CBotWeapons :: getPrimaryWeapon ()
{
	CBotWeapon *pBest = NULL;

	for ( register unsigned short i = 0; i < MAX_WEAPONS; i ++ )
	{
		CBotWeapon *pWeap = &(m_theWeapons[i]);

		if ( !pWeap->hasWeapon() )
			continue;

		if ( (pBest == NULL) || (pBest->getPreference() < pWeap->getPreference() ) )
		{
			pBest = pWeap;
		}
	}

	return pBest;
}


CBotWeapon *CBotWeapons::getActiveWeapon(const char *szWeaponName, edict_t *pWeaponUpdate, bool bOverrideAmmoTypes)
{
	CBotWeapon *toReturn = NULL;

	if (szWeaponName && *szWeaponName)
	{
		CWeapon *pWeapon = CWeapons::getWeapon(szWeaponName);

		if (pWeapon)
		{
			register unsigned short int i;

			for (i = 0; i < MAX_WEAPONS; i++)
			{
				CWeapon *p = m_theWeapons[i].getWeaponInfo();

				if (!p)
					continue;

				if (strcmp(p->getWeaponName(), szWeaponName) == 0)
				{
					toReturn = &m_theWeapons[i];
					break;
				}
			}

			if (pWeaponUpdate && toReturn)
			{
				toReturn->setWeaponEntity(pWeaponUpdate, bOverrideAmmoTypes);
			}
		}
	}

	return toReturn;
}
/*
bool CBotWeaponGravGun ::outOfAmmo (CBot *pBot)
{
	if ( m_pEnt )
		(return CClassInterface::gravityGunObject(m_pEnt)==NULL);

	return true;
}
*/
bool CBotWeapon :: outOfAmmo(CBot *pBot)
{
	if ( m_pWeaponInfo->isGravGun() && m_pEnt )
		return (CClassInterface::gravityGunObject(m_pEnt) == NULL);

	// if I have something in my clip now
	// I am okay, otherwise return ammo in list
	if ( m_iClip1 && (*m_iClip1 > 0) )
		return false;

	return getAmmo(pBot)==0;
}
/*
bool CBotWeapon :: needToReload(CBot *pBot)
{
	return getAmmo(pBot)==0;
}*/
////////////////////////////////////////
// CWeapons

class IWeaponFunc
{
public:
	virtual void execute ( CWeapon *pWeapon ) = 0;
};

class CGetWeapID : public IWeaponFunc
{
public:
	CGetWeapID ( int iId )
	{
		m_iId = iId;
		m_pFound = NULL;
	}

	void execute ( CWeapon *pWeapon )
	{
		if ( m_iId == pWeapon->getID() )
			m_pFound = pWeapon;
	}

	CWeapon *get ()
	{
		return m_pFound;
	}

private:
	int m_iId;
	CWeapon *m_pFound;
};

class CGetWeapCName : public IWeaponFunc
{
public:
	CGetWeapCName ( const char *szWeapon )
	{
		m_pFound = NULL;
        m_szWeapon = szWeapon;
	}

	void execute ( CWeapon *pWeapon )
	{
		if ( pWeapon->isWeaponName(m_szWeapon) )
			m_pFound = pWeapon;
	}

	CWeapon *get ()
	{
		return m_pFound;
	}
private:
	const char *m_szWeapon;
	CWeapon *m_pFound;
};

class CGetWeapShortName : public IWeaponFunc
{
public:
	CGetWeapShortName ( const char *szWeapon )
	{
		m_pFound = NULL;
        m_szWeapon = szWeapon;
	}

	void execute ( CWeapon *pWeapon )
	{
		if ( pWeapon->isShortWeaponName(m_szWeapon) )
			m_pFound = pWeapon;
	}

	CWeapon *get ()
	{
		return m_pFound;
	}
private:
	const char *m_szWeapon;
	CWeapon *m_pFound;
};

CWeapon *CWeapons :: getWeapon ( const int iId )
{
	CGetWeapID pFunc = CGetWeapID(iId);
	eachWeapon(&pFunc);
	return pFunc.get();
}

CWeapon *CWeapons :: getWeapon ( const char *szWeapon )
{
	CGetWeapCName pFunc = CGetWeapCName(szWeapon);
	eachWeapon(&pFunc);
	return pFunc.get();
}

CWeapon *CWeapons :: getWeaponByShortName ( const char *szWeapon )
{
	CGetWeapShortName pFunc = CGetWeapShortName(szWeapon);
	eachWeapon(&pFunc);
	return pFunc.get();
}

void CWeapons :: eachWeapon ( IWeaponFunc *pFunc )
{
	for ( unsigned int i = 0; i < m_theWeapons.size(); i ++ )
	{
		pFunc->execute(m_theWeapons[i]);
	}
}

void CWeapons :: freeMemory ()
{
	for ( unsigned int i = 0; i < m_theWeapons.size(); i ++ )
	{
		delete m_theWeapons[i];
		m_theWeapons[i] = NULL;
	}

	m_theWeapons.clear();
}
