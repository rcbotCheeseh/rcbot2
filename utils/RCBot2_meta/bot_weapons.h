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
#ifndef __BOT_WEAPONS_H__
#define __BOT_WEAPONS_H__

#include <vector>

extern const char *g_szTF2Weapons[];

#include "shareddefs.h"

class CBot;


extern int m_TF2AmmoIndices[];

typedef struct
{
	int iSlot;
	int iId;
	const char *szWeaponName;
	int m_iFlags;
	float minPrimDist;
	float maxPrimDist;
	int m_iAmmoIndex;
	int m_iPreference;
	float m_fProjSpeed;
}WeaponsData_t;

enum 
{
	TF2_WEAPON_BAT = 0,
	TF2_WEAPON_BOTTLE, //1
	TF2_WEAPON_FIREAXE, //2
	TF2_WEAPON_CLUB, //3 
	TF2_WEAPON_KNIFE, //4 
	TF2_WEAPON_FISTS, //5 
	TF2_WEAPON_SHOVEL, //6 
	TF2_WEAPON_WRENCH,//7
	TF2_WEAPON_BONESAW,//8
	TF2_WEAPON_SHOTGUN_PRIMARY,//9
	TF2_WEAPON_SHOTGUN_SOLDIER,//10
	TF2_WEAPON_SHOTGUN_HWG,//11
	TF2_WEAPON_SHOTGUN_PYRO,//12
	TF2_WEAPON_SCATTERGUN,//13
	TF2_WEAPON_SNIPERRIFLE,//14
	TF2_WEAPON_MINIGUN,//15
	TF2_WEAPON_SMG,//16
	TF2_WEAPON_SYRINGEGUN,//17
	TF2_WEAPON_ROCKETLAUNCHER,//18
	TF2_WEAPON_GRENADELAUNCHER,//19
	TF2_WEAPON_PIPEBOMBS,//20
	TF2_WEAPON_FLAMETHROWER,//21
	TF2_WEAPON_PISTOL,//22
	TF2_WEAPON_PISTOL_SCOUT,//23
	TF2_WEAPON_REVOLVER,//24
	TF2_WEAPON_POMSON6000, //TF2_WEAPON_PDA_ENGI_BUILD,//25
	TF2_WEAPON_SHOTGUN,//TF2_WEAPON_PDA_ENGI_DESTROY,//26
	TF2_WEAPON_FRONTIERJUSTICE,//TF2_WEAPON_PDA_SPY,//27
	TF2_WEAPON_BUILDER,//28
	TF2_WEAPON_MEDIGUN,//29
	TF2_WEAPON_INVIS,//30
	TF2_WEAPON_FLAREGUN,//31
	TF2_WEAPON_BUFF_ITEM,//32
	TF2_WEAPON_SAXXY,//33
	TF2_WEAPON_SENTRYGUN,//34
	TF2_WEAPON_BAT_WOOD,//=44,
	TF2_WEAPON_LUNCHBOX_DRINK,//=46,
	TF2_WEAPON_BOW,//=56,
	TF2_WEAPON_JAR,//=58,
	TF2_WEAPON_DIRECTHIT,//=127,
	TF2_WEAPON_SWORD,//=132,
	TF2_WEAPON_BAT_FISH,//=221,
	TF2_WEAPON_KATANA,//=357,
	TF2_WEAPON_COWMANGLER,
	TF2_WEAPON_CROSSBOW,
	TF2_WEAPON_CLEAVER,
	TF2_WEAPON_BAT_GIFTWRAP,
	TF2_WEAPON_RAYGUN,
	TF2_WEAPON_MAX
};
/*
enum 
{
	TF2_WEAPON_BAT = 0,
	TF2_WEAPON_BONESAW,
	TF2_WEAPON_BOTTLE,
	TF2_WEAPON_BUILDER,
	TF2_WEAPON_CLUB,
	TF2_WEAPON_FIREAXE,
	TF2_WEAPON_FISTS,
	TF2_WEAPON_FLAMETHROWER,
	TF2_WEAPON_GRENADELAUNCHER,
	TF2_WEAPON_INVIS,
	TF2_WEAPON_KNIFE,
	TF2_WEAPON_MEDIGUN,
	TF2_WEAPON_MINIGUN,
	TF2_WEAPON_OBJECTSSELECTION,
	TF2_WEAPON_PDA_ENGI_BUILD,
	TF2_WEAPON_PDA_ENGI_DESTROY,
	TF2_WEAPON_PDA_SPY,
	TF2_WEAPON_PIPEBOMBS,
	TF2_WEAPON_PISTOL,
	TF2_WEAPON_PISTOL_SCOUT,
	TF2_WEAPON_REVOLVER,
	TF2_WEAPON_ROCKETLAUNCHER,
	TF2_WEAPON_SCATTERGUN,
	TF2_WEAPON_SHOTGUN_HWG,
	TF2_WEAPON_SHOTGUN_PRIMARY,
	TF2_WEAPON_SHOTGUN_PYRO,
	TF2_WEAPON_SHOTGUN_SOLDIER,
	TF2_WEAPON_SHOVEL,
	TF2_WEAPON_SMG,
	TF2_WEAPON_SNIPERRIFLE,
	TF2_WEAPON_SYRINGEGUN,
	TF2_WEAPON_WRENCH,
	TF2_WEAPON_ENGIDESTROY,
	TF2_WEAPON_ENGIBUILD,
	TF2_WEAPON_SENTRYGUN,
	TF2_WEAPON_MAX
};
*/
enum
{
	HL2DM_WEAPON_PISTOL = 0,
	HL2DM_WEAPON_CROWBAR,
	HL2DM_WEAPON_357,
	HL2DM_WEAPON_SMG1,
	HL2DM_WEAPON_AR2,
	HL2DM_WEAPON_FRAG,
	HL2DM_WEAPON_STUNSTICK,
	HL2DM_WEAPON_CROSSBOW,
	HL2DM_WEAPON_RPG,
	HL2DM_WEAPON_SLAM,	
	HL2DM_WEAPON_SHOTGUN,
	HL2DM_WEAPON_PHYSCANNON,
	HL2DM_WEAPON_MAX
};

enum
{
	DOD_WEAPON_AMERKNIFE = 0,
	DOD_WEAPON_SPADE,
	DOD_WEAPON_COLT,
	DOD_WEAPON_P38,
	DOD_WEAPON_M1,
	DOD_WEAPON_C96,
	DOD_WEAPON_GARAND,
	DOD_WEAPON_K98,
	DOD_WEAPON_THOMPSON,
	DOD_WEAPON_MP40,
	DOD_WEAPON_BAR,
	DOD_WEAPON_MP44,
	DOD_WEAPON_SPRING,
	DOD_WEAPON_K98_SCOPED,
	DOD_WEAPON_20CAL,
	DOD_WEAPON_MG42,
	DOD_WEAPON_BAZOOKA,
	DOD_WEAPON_PSCHRECK,
	DOD_WEAPON_RIFLEGREN_US,
	DOD_WEAPON_RIFLEGREN_GER,
	DOD_WEAPON_FRAG_US,
	DOD_WEAPON_FRAG_GER, 
	DOD_WEAPON_SMOKE_US,
	DOD_WEAPON_SMOKE_GER,
	DOD_WEAPON_BOMB,
	DOD_WEAPON_MAX
};


#define WEAP_FL_NONE			0
#define WEAP_FL_PRIM_ATTACK		1
#define WEAP_FL_SEC_ATTACK		2
#define WEAP_FL_EXPLOSIVE		4 // weapon is an explosive weapon eg. rpg
#define WEAP_FL_MELEE			8 //
#define WEAP_FL_UNDERWATER		16 // weapon can be used under water
#define WEAP_FL_HOLDATTACK		32 // weapon must hold attack (e.g. minigun)
#define WEAP_FL_SPECIAL			64 //
#define WEAP_FL_KILLPIPEBOMBS	128 // weapon can destroy pipe bombs (tf2)
#define WEAP_FL_DEFLECTROCKETS	256 // weapon can deflect rocekts (tf2)
#define WEAP_FL_GRAVGUN			512 // weapon is a grav gun
#define WEAP_FL_EXPLOSIVE_SEC	1024 // weapon has an explosive secondary attack
#define WEAP_FL_ZOOMABLE		2048 // weapon can be zoomed
#define WEAP_FL_DEPLOYABLE		4096 // weapon can be deployed
#define WEAP_FL_MELEE_SEC_ATT	8192 // weapon has a melee secondary attack
#define WEAP_FL_FIRE_SELECT		16384 // weapon can choose fire mode
#define WEAP_FL_CANTFIRE_NORM	32768 // weapon can't be fired normally, needs to be zoomed/deployed
#define WEAP_FL_GRENADE			65536
#define WEAP_FL_HIGH_RECOIL		131072 // can't be fired at long distance, but ok when deployed
#define WEAP_FL_SCOPE			262144 // has a scope . i.e. sniper rifle
#define WEAP_FL_PROJECTILE		524288 // affected by gravity

extern WeaponsData_t TF2Weaps[];
extern WeaponsData_t HL2DMWeaps[];
extern WeaponsData_t DODWeaps[];

class CWeapon
{
public:
	CWeapon ( WeaponsData_t *data )
	{
		m_iSlot = data->iSlot;
		setID(data->iId);
		setName(data->szWeaponName);

		setFlags(data->m_iFlags);

		// shoot distance (default)
		m_fPrimMinWeaponShootDist = data->minPrimDist;
		m_fPrimMaxWeaponShootDist = data->maxPrimDist;

		m_fSecMinWeaponShootDist = 0.0f;
		m_fSecMaxWeaponShootDist = 512.0f;

		m_fProjectileSpeed = data->m_fProjSpeed;

		m_iAmmoIndex1 = data->m_iAmmoIndex;
		m_iAmmoIndex2 = -1;

		m_iPreference = data->m_iPreference;
	}

	/*CWeapon( int iSlot, const char *szWeaponName, int iId, int iFlags = 0, int iAmmoIndex = -1, float minPrim =0.0f, float maxPrim = 4096.0f, int iPref = 0, int iAmmoIndex2 = -1 )
	{
		m_iSlot = iSlot;
		setID(iId);
		setName(szWeaponName);

		setFlags(iFlags);

		// shoot distance (default)
		m_fPrimMinWeaponShootDist = minPrim;
		m_fPrimMaxWeaponShootDist = maxPrim;

		m_fSecMinWeaponShootDist = 0.0f;
		m_fSecMaxWeaponShootDist = 512.0f;
		m_iAmmoIndex1 = iAmmoIndex;
		m_iAmmoIndex2 = iAmmoIndex2;

		m_iPreference = iPref;
	}*/

	inline void setName ( const char *szWeaponName )
	{
		m_szWeaponName = szWeaponName;
	}

	inline bool isWeaponName ( const char *szWeaponName )
	{
		return !strcmp(szWeaponName,getWeaponName());
	}

	inline bool isShortWeaponName ( const char *szWeaponName )
	{
		static int start;
		
		start = strlen(m_szWeaponName) - strlen(szWeaponName);
		
		if ( start < 0 )
			return false;

		return !strcmp(&m_szWeaponName[start],szWeaponName);
	}

	inline bool canDestroyPipeBombs()
	{
		return hasAllFlags(WEAP_FL_KILLPIPEBOMBS);
	}

	inline bool isScoped ()
	{
		return hasAllFlags(WEAP_FL_SCOPE);
	}
					
	inline bool canDeflectRockets()
	{
		return hasAllFlags(WEAP_FL_DEFLECTROCKETS);
	}

	inline void setID ( const int iId )
	{
		m_iWeaponId = iId;
	}

	inline void setFlags ( const int iFlags )
	{
		m_iFlags = iFlags;
	}

	inline bool primaryInRange ( float fDistance )
	{
		return (fDistance>m_fPrimMinWeaponShootDist)&&(fDistance<m_fPrimMaxWeaponShootDist);
	}

	inline bool primaryGreaterThanRange ( float fDistance )
	{
		return (fDistance<m_fPrimMaxWeaponShootDist);
	}
	
	inline float primaryMaxRange ( )
	{
		return (m_fPrimMaxWeaponShootDist);
	}

	inline bool hasHighRecoil ()
	{
		return hasAllFlags(WEAP_FL_HIGH_RECOIL);
	}

	inline bool isZoomable ()
	{
		return hasAllFlags(WEAP_FL_ZOOMABLE);
	}

	inline bool isProjectile ()
	{
		return hasAllFlags(WEAP_FL_PROJECTILE);
	}

	inline bool isExplosive ()
	{
		return hasAllFlags(WEAP_FL_EXPLOSIVE);
	}

	inline bool isDeployable ()
	{
		return hasAllFlags(WEAP_FL_DEPLOYABLE);
	}

	inline bool canUseUnderWater ()
	{
		return hasAllFlags(WEAP_FL_UNDERWATER);
	}

	inline bool isGravGun()
	{
		return hasAllFlags(WEAP_FL_GRAVGUN);
	}

	inline bool mustHoldAttack ()
	{
		return hasAllFlags(WEAP_FL_HOLDATTACK);
	}

	inline bool isGrenade ()
	{
		return hasAllFlags(WEAP_FL_GRENADE);
	}

	inline bool isMelee ()
	{
		return hasAllFlags(WEAP_FL_MELEE);
	}

	inline bool isMeleeSecondary ()
	{
		return hasAllFlags(WEAP_FL_MELEE_SEC_ATT);
	}

	inline bool needsDeployedOrZoomed ()
	{
		return hasAllFlags(WEAP_FL_CANTFIRE_NORM);
	}

	inline bool canAttack()
	{
		return hasAllFlags(WEAP_FL_PRIM_ATTACK);
	}

	inline bool isSpecial ()
	{
		return hasAllFlags(WEAP_FL_SPECIAL);
	}

	inline bool secondaryInRange ( float fDistance )
	{
		return (fDistance>m_fSecMinWeaponShootDist)&&(fDistance<m_fSecMaxWeaponShootDist);
	}

	inline int getPreference ()
	{
		return m_iPreference;
	}

	inline const char *getWeaponName () const
	{
		return m_szWeaponName;
	}

	inline const int getID () const
	{
		return m_iWeaponId;
	}

	inline void setPrimaryRange ( float fMinRange, float fMaxRange )
	{
		m_fPrimMinWeaponShootDist = fMinRange; 
		m_fPrimMaxWeaponShootDist = fMaxRange;
	}

	inline void setSecondaryRange ( float fMinRange, float fMaxRange )
	{
		m_fSecMinWeaponShootDist = fMinRange;
		m_fSecMaxWeaponShootDist = fMaxRange;
	}

	inline int getAmmoIndex1 ()
	{
		return m_iAmmoIndex1;
	}

	inline int getAmmoIndex2 ()
	{
		return m_iAmmoIndex2;
	}

	inline int getSlot ()
	{
		return m_iSlot;
	}

	void setAmmoIndex ( int iAmmoIndex1, int iAmmoIndex2 = -1)
	{
		m_iAmmoIndex1 = iAmmoIndex1;
		m_iAmmoIndex2 = iAmmoIndex2;
	}

	inline bool canUseSecondary ()
	{
		return hasSomeFlags(WEAP_FL_SEC_ATTACK);
	}

	inline float getProjectileSpeed ()
	{
		return m_fProjectileSpeed;
	}

private:

	inline bool hasAllFlags ( int iFlags ) const
	{
		return (m_iFlags & iFlags)==iFlags;
	}

	inline bool hasSomeFlags ( int iFlags ) const
	{
		return (m_iFlags & iFlags)>0;
	}

	const char *m_szWeaponName; // classname

	int m_iWeaponId;			// identification
	int m_iFlags;				// flags
	int m_iAmmoIndex1;
	int m_iAmmoIndex2;
	int m_iPreference;
	int m_iSlot;

	// shoot distance
	float m_fPrimMinWeaponShootDist;
	float m_fPrimMaxWeaponShootDist;

	float m_fSecMinWeaponShootDist;
	float m_fSecMaxWeaponShootDist;

	float m_fProjectileSpeed;
};

class IWeaponFunc;

class CWeapons
{
public:
	CWeapons ()
	{
		m_theWeapons.clear();
	}

	static inline void addWeapon ( CWeapon *pWeapon ) { m_theWeapons.push_back(pWeapon); }

	static CWeapon *getWeapon ( const int iId );

	static CWeapon *getWeapon ( const char *szWeapon );

	static CWeapon *getWeaponByShortName ( const char *pszWeapon );

	static void eachWeapon ( IWeaponFunc *pFunc );

	static void freeMemory ();

	static edict_t *findWeapon ( edict_t *pPlayer, const char *szWeaponName );

	static void loadWeapons(const char *szWeaponListName, WeaponsData_t *pDefault);

private:
	// available weapons in game
	static std::vector<CWeapon*> m_theWeapons;
};
#define AMMO_PRIM 1
#define AMMO_SEC 2

////////////////////////////////////////////////////////////
// Weapon but with bot holding it and ammo information etc
////////////////////////////////////////////////////////////
class CBotWeapon
{
public:
	CBotWeapon ()
	{
		m_pWeaponInfo = NULL;
		m_bHasWeapon = false;		
		m_iWeaponIndex = 0;
		m_pEnt = NULL;
		m_iClip1 = NULL;
		m_iClip2 = NULL;
	}

	inline void setWeapon ( CWeapon *pWeapon )
	{
		m_pWeaponInfo = pWeapon;
	}

	inline float getPrimaryMaxRange()
	{
		return m_pWeaponInfo->primaryMaxRange();
	}

	inline bool primaryInRange ( float fDistance )
	{
		return m_pWeaponInfo->primaryInRange(fDistance);
	}

	inline bool primaryGreaterThanRange ( float fDistance )
	{
		return m_pWeaponInfo->primaryGreaterThanRange(fDistance); 
	}

	inline bool needsDeployedOrZoomed ()
	{
		return m_pWeaponInfo->needsDeployedOrZoomed();
	}

	inline bool isGravGun ()
	{
		return m_pWeaponInfo->isGravGun();
	}


	inline bool isExplosive ()
	{
		return m_pWeaponInfo->isExplosive();
	}

	inline bool isZoomable () 
	{
		return m_pWeaponInfo->isZoomable();
	}

	inline bool canUseUnderWater ()
	{
		return m_pWeaponInfo->canUseUnderWater();
	}

	inline bool hasHighRecoil ()
	{
		return m_pWeaponInfo->hasHighRecoil();
	}

	inline int getID ()
	{
		return m_pWeaponInfo->getID();
	}

	inline bool isSpecial ()
	{
		return m_pWeaponInfo->isSpecial();
	}

	inline float primaryMaxRange ( )
	{
		return (m_pWeaponInfo->primaryMaxRange());
	}

	inline bool isDeployable ()
	{
		return m_pWeaponInfo->isDeployable();
	}

	inline bool mustHoldAttack ()
	{
		return m_pWeaponInfo->mustHoldAttack();
	}

	inline bool canDestroyPipeBombs()
	{
		return m_pWeaponInfo->canDestroyPipeBombs();
	}

	inline bool isProjectile ()
	{
		return m_pWeaponInfo->isProjectile();
	}

	inline float getProjectileSpeed ()
	{
		return m_pWeaponInfo->getProjectileSpeed();
	}
					
	inline bool canDeflectRockets()
	{
		return m_pWeaponInfo->canDeflectRockets();
	}

	inline bool canUseSecondary ()
	{
		return m_pWeaponInfo->canUseSecondary();
	}

	inline bool isMelee ()
	{
		return m_pWeaponInfo->isMelee();
	}

	inline bool isMeleeSecondary () 
	{
		return m_pWeaponInfo->isMeleeSecondary();
	}

	inline bool secondaryInRange ( float fDistance )
	{
		return m_pWeaponInfo->secondaryInRange(fDistance);
	}

	inline int getPreference ()
	{
		return m_pWeaponInfo->getPreference();
	}

	bool outOfAmmo (CBot *pBot);

	bool needToReload (CBot *pBot);

	inline void setHasWeapon ( bool bHas )
	{
		m_bHasWeapon = bHas;
	}

	inline bool hasWeapon ()
	{
		return m_bHasWeapon;
	}

	inline bool canAttack ()
	{
		return m_pWeaponInfo->canAttack();
	}

	int getAmmo ( CBot *pBot, int type = AMMO_PRIM );

	int getClip1 ( CBot *pBot ) 
	{ 
		if ( m_iClip1 ) 
			return *m_iClip1; 
		
		return 0; 
	}

	int getClip2 ( CBot *pBot ) 
	{ 
		if ( m_iClip2 ) 
			return *m_iClip2; 
		
		return 0; 
	}

	CWeapon *getWeaponInfo () { return m_pWeaponInfo; }

	inline int getWeaponIndex () { return m_iWeaponIndex; }

	inline void setWeaponIndex (int iIndex) { m_iWeaponIndex = iIndex; } // Entity Index

	void setWeaponEntity (edict_t *pent, bool bOverrideAmmoTypes = true );

	inline edict_t *getWeaponEntity () { return m_pEnt; }


private:

	// link to weapon info
	CWeapon *m_pWeaponInfo;

	int m_iWeaponIndex;

	bool m_bHasWeapon;

	edict_t *m_pEnt;

	int *m_iClip1;
	int *m_iClip2;
};

// Weapons that
class CBotWeapons 
{
public:
/////////////////////////////////////
	CBotWeapons ( CBot *pBot );    // // constructor
/////////////////////////////////////
	CBotWeapon *getBestWeapon ( edict_t *pEnemy, bool bAllowMelee = true, bool bAllowMeleeFallback = true, bool bMeleeOnly = false, bool bExplosivesOnly = false, bool bIgnorePrimaryMinimum = false );

	CBotWeapon *addWeapon(CWeapon *pWeaponInfo, int iId, edict_t *pent, bool bOverrideAll = true);

	CBotWeapon *getWeapon(CWeapon *pWeapon);

	CBotWeapon *getActiveWeapon(const char *szWeaponName, edict_t *pWeaponUpdate = NULL, bool bOverrideAmmoTypes = true);

	CBotWeapon *getCurrentWeaponInSlot ( int iSlot );

	CBotWeapon *getGrenade ()
	{
		for ( int i = 0; i < MAX_WEAPONS; i ++ )
		{
			if ( m_theWeapons[i].hasWeapon() )
			{
				if ( m_theWeapons[i].getWeaponInfo() )
				{
					if ( m_theWeapons[i].getWeaponInfo()->isGrenade() )
						return &(m_theWeapons[i]);
				}
			}
		}

		return NULL;
	}

	void clearWeapons ();

	bool hasWeapon ( int id );

	bool hasExplosives ( void );

	// returns true if there is a change to the weapons
	bool update ( bool bOverrideAllFromEngine = true ); // update from sendprop

	CBotWeapon *getPrimaryWeapon (); // return the most important weapon bot is holding if if out o ammo
	inline void resetSignature() { m_iWeaponsSignature = 0; }
private:
	// bot that has these weapons
	CBot *m_pBot;

	// checksum mask of weapons bot already has so we know if we need to update or not
	unsigned int m_iWeaponsSignature;

	// weapons local to the bot only 
	// (holds ammo/preference etc and link to actual weapon)
	CBotWeapon m_theWeapons[MAX_WEAPONS];//[MAX_WEAPONS];

	float m_fUpdateWeaponsTime;
};

#endif