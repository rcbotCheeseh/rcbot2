#ifndef __RCBOT_GETPROP_H__
#define __RCBOT_GETPROP_H__

typedef enum
{
	TELE_ENTRANCE = 0,
	TELE_EXIT
}eTeleMode;

typedef enum
{
	GETPROP_UNDEF = -1,
	GETPROP_TF2SCORE = 0,
	GETPROP_ENTITY_FLAGS,
	GETPROP_TEAM,
	GETPROP_PLAYERHEALTH,
	GETPROP_EFFECTS,
	GETPROP_AMMO,
	GETPROP_TF2_NUMHEALERS,
	GETPROP_TF2_CONDITIONS,
	GETPROP_VELOCITY,
	GETPROP_TF2CLASS,
	GETPROP_TF2SPYMETER,// CTFPlayer::
	GETPROP_TF2SPYDISGUISED_TEAM,//CTFPlayer::m_nDisguiseTeam
	GETPROP_TF2SPYDISGUISED_CLASS,//CTFPlayer::m_nDisguiseClass
	GETPROP_TF2SPYDISGUISED_TARGET_INDEX,//CTFPlayer::m_iDisguiseTargetIndex
	GETPROP_TF2SPYDISGUISED_DIS_HEALTH,//CTFPlayer::m_iDisguiseHealth
 	GETPROP_TF2MEDIGUN_HEALING,
	GETPROP_TF2MEDIGUN_TARGETTING,
	//SETPROP_SET_TICK_BASE,
	GETPROP_TF2TELEPORTERMODE,
	GETPROP_CURRENTWEAPON,
	GETPROP_TF2UBERCHARGE_LEVEL,
	GETPROP_TF2SENTRYHEALTH,
	GETPROP_TF2DISPENSERHEALTH,
	GETPROP_TF2TELEPORTERHEALTH,
	GETPROP_TF2OBJECTCARRIED,
	GETPROP_TF2OBJECTUPGRADELEVEL,
	GETPROP_TF2OBJECTUPGRADEMETAL,
	GETPROP_TF2OBJECTMAXHEALTH,
	GETPROP_TF2DISPMETAL,
	GETPROP_TF2MINIBUILDING,
	GETPROP_MAXSPEED,
	GETPROP_CONSTRAINT_SPEED,
	GETPROP_TF2OBJECTBUILDING,
	GETPROP_HL2DM_PHYSCANNON_ATTACHED,
	GETPROP_HL2DM_PHYSCANNON_OPEN,
	GETPROP_HL2DM_PLAYER_AUXPOWER,
	GETPROP_HL2DM_LADDER_ENT,
	GETPROP_WEAPONLIST,
	GETPROP_WEAPONSTATE,
	GETPROP_WEAPONCLIP1,
	GETPROP_WEAPONCLIP2,
	GETPROP_WEAPON_AMMOTYPE1,
	GETPROP_WEAPON_AMMOTYPE2,
	GETPROP_DOD_PLAYERCLASS,
	GETPROP_DOD_DES_PLAYERCLASS,
	GETPROP_DOD_STAMINA,
	GETPROP_DOD_PRONE,
	GETPROP_SEQUENCE,
	GETPROP_CYCLE,
	GETPROP_ENTITYFLAGS,
	GETPROP_DOD_CP_NUMCAPS,
	GETPROP_DOD_CP_POSITIONS,
	GETPROP_DOD_CP_ALLIES_REQ_CAP,
	GETPROP_DOD_CP_AXIS_REQ_CAP,
	GETPROP_DOD_CP_NUM_AXIS,
	GETPROP_DOD_CP_NUM_ALLIES,
	GETPROP_DOD_CP_OWNER,
	GETPROP_DOD_SNIPER_ZOOMED,
	GETPROP_DOD_MACHINEGUN_DEPLOYED,
	GETPROP_DOD_ROCKET_DEPLOYED,
	GETPROP_DOD_SEMI_AUTO,
	GETPROP_MOVETYPE,
	GETPROP_DOD_GREN_THROWER,
	GETPROP_DOD_SCORE,
	GETPROP_DOD_OBJSCORE,
	GETPROP_DOD_DEATHS,
	GETPROP_DOD_SMOKESPAWN_TIME,
	GETPROP_DOD_ROUNDTIME,
	GETPROP_DOD_K98ZOOM,
	GETPROP_DOD_GARANDZOOM,
	GETPROP_DOD_ALLIESBOMBING,
	GETPROP_DOD_AXISBOMBING,
	GETPROP_DOD_BOMBSPLANTED,
	GETPROP_DOD_BOMBSREQ,
	GETPROP_DOD_BOMBSDEFUSED,
	GETPROP_DOD_BOMBSREMAINING,
	GETPROP_DOD_PLANTINGBOMB,
	GETPROP_DOD_DEFUSINGBOMB,
	GETPROP_ALL_ENTOWNER,
	GETPROP_DOD_BOMB_STATE,
	GETPROP_DOD_BOMB_TEAM,
	GETPROP_DOD_CP_VISIBLE,
	GETPROP_GROUND_ENTITY,
	GETPROP_ORIGIN,
	GETPROP_TAKEDAMAGE,
	GETPROP_SENTRY_ENEMY,
	GETPROP_WATERLEVEL,
	GETPROP_TF2OBJECTSHELLS,
	GETPROP_TF2OBJECTROCKETS,
	GETPROP_TF2_TELEPORT_RECHARGETIME,
	GETPROP_TF2_TELEPORT_RECHARGEDURATION,
	GETPROP_TF2_OBJTR_m_vCPPositions,
	GETPROP_TF2_OBJTR_m_bCPIsVisible,
	GETPROP_TF2_OBJTR_m_iTeamIcons,
	GETPROP_TF2_OBJTR_m_iTeamOverlays,
	GETPROP_TF2_OBJTR_m_iTeamReqCappers,
	GETPROP_TF2_OBJTR_m_flTeamCapTime,
	GETPROP_TF2_OBJTR_m_iPreviousPoints,
	GETPROP_TF2_OBJTR_m_bTeamCanCap,
	GETPROP_TF2_OBJTR_m_iTeamBaseIcons,
	GETPROP_TF2_OBJTR_m_iBaseControlPoints,
	GETPROP_TF2_OBJTR_m_bInMiniRound,
	GETPROP_TF2_OBJTR_m_iWarnOnCap,
	GETPROP_TF2_OBJTR_m_iCPGroup,
	GETPROP_TF2_OBJTR_m_bCPLocked,
	GETPROP_TF2_OBJTR_m_bTrackAlarm,
	GETPROP_TF2_OBJTR_m_flUnlockTimes,
	GETPROP_TF2_OBJTR_m_flCPTimerTimes,
	GETPROP_TF2_OBJTR_m_iNumTeamMembers,
	GETPROP_TF2_OBJTR_m_iCappingTeam,
	GETPROP_TF2_OBJTR_m_iTeamInZone,
	GETPROP_TF2_OBJTR_m_bBlocked,
	GETPROP_TF2_OBJTR_m_iOwner,
	GETPROP_TF2_OBJTR_m_bCPCapRateScalesWithPlayers,
	GETPROP_TF2_OBJTR_m_iNumControlPoints,
	GETPROP_TF2_OBJTR_m_bPlayingMiniRounds,
	GETPROP_TF2_RNDTM_m_flTimerEndTime,
	GETPROP_TF2_RNDTM_m_nSetupTimeLength,
	GETPROP_TF2_RNDTM_m_bInSetup,
	GETPROP_PIPEBOMB_OWNER,
	GETPROP_TF2_TAUNTYAW,
	GETPROP_TF2_HIGHFIVE,
	GETPROP_TF2_HIGHFIVE_PARTNER,
	GETPROP_SENTRYGUN_PLACING,
	GETPROP_TF2_ISCARRYINGOBJ,
	GETPROP_TF2_GETCARRIEDOBJ,
	GETPROP_TF2_ITEMDEFINITIONINDEX,
	GETPROP_TF2_DISGUISEWEARABLE,
	GETPROP_TF2_RAGEMETER,
	GETPROP_TF2_RAGEDRAINING,
	GETPROP_SIMULATIONTIME,
	GETPROP_TF2_INUPGRADEZONE,
	GETPROP_TF2_ENERGYDRINKMETER,
	GETPROP_TF2_MEDIEVALMODE,
	GETPROP_TF2_ACTIVEWEAPON,
	GETPROP_TF2_BUILDER_TYPE,
	GETPROP_TF2_BUILDER_MODE,
	GETPROP_TF2_CHARGE_RESIST_TYPE,
	GETPROP_TF2_ROUNDSTATE,
	GETPROP_TF2DESIREDCLASS, //Jrob
	GET_PROPDATA_MAX
}getpropdata_id;

bool UTIL_FindSendPropInfo(ServerClass *pInfo, const char *szType, unsigned int *offset);
ServerClass *UTIL_FindServerClass(const char *name);
void UTIL_FindServerClassPrint(const char*name_cmd);
void UTIL_FindServerClassnamePrint(const char *name_cmd);
void UTIL_FindPropPrint(const char *prop_name);

class CClassInterfaceValue
{
public:
	CClassInterfaceValue ()
	{
		m_data = NULL; 
		m_class = NULL;
		m_value = NULL;
		m_offset = 0;
	}

	CClassInterfaceValue ( char *key, char *value, unsigned int preoffset )
	{
		init(key,value,preoffset);
	}

	void init ( char *key, char *value, unsigned int preoffset = 0 );

	void findOffset ( );

	void getData ( void *edict, bool bIsEdict = true );

	edict_t *getEntity ( edict_t *edict );

	CBaseHandle *getEntityHandle ( edict_t *edict );

	inline bool getBool(void *edict, bool defaultvalue, bool bIsEdict = true)
	{ 
		getData(edict, bIsEdict);
		
		if ( !m_data ) 
			return defaultvalue; 
		
		try
		{
			return *((bool*)m_data); 
		}

		catch(...)
		{
			return defaultvalue;
		}
	}

	inline bool *getBoolPointer ( edict_t *edict ) 
	{ 
		getData(edict);  
				
		if ( !m_data ) 
			return NULL; 

		return ((bool*)m_data); 
	}

	inline void *getVoidPointer ( edict_t *edict ) 
	{ 
		getData(edict);  
				
		if ( !m_data ) 
			return NULL; 

		return m_data; 
	}

	inline float getFloat ( edict_t *edict, float defaultvalue ) 
	{ 
		getData(edict); 
		
		if ( !m_data ) 
			return defaultvalue; 
		
		return *((float*)m_data); 
	}

	inline float *getFloatPointer ( edict_t *edict ) 
	{ 
		getData(edict); 
		
		if ( !m_data ) 
			return NULL; 
		
		return ((float*)m_data); 
	}

	inline char *getString (edict_t *edict ) 
	{ 
		getData(edict); 

		return (char*)m_data; 
	}

	inline Vector *getVectorPointer ( edict_t *edict )
	{
		getData(edict);

		if ( m_data )
		{
			return (Vector*)m_data;
		}

		return NULL;
	}

	inline bool getVector ( edict_t *edict, Vector *v )
	{
		static float *x;

		getData(edict);

		if ( m_data )
		{
			x = (float*)m_data;
			*v = Vector(*x,*(x+1),*(x+2));

			return true;
		}

		return false;
	}

	inline int getInt(void *edict, int defaultvalue, bool bIsEdict = true)
	{ 
		getData(edict, bIsEdict);
		
		if ( !m_data ) 
			return defaultvalue; 

		try
		{
			return *((int*)m_data);
		}

		catch ( ... )
		{
			return defaultvalue;
		}
	}

	inline int *getIntPointer ( edict_t *edict ) 
	{ 
		getData(edict); 

		return (int*)m_data; 
	}

	inline byte *getBytePointer ( edict_t *edict ) 
	{ 
		getData(edict); 

		return (byte*)m_data; 
	}

	inline float getFloatFromInt ( edict_t *edict, float defaultvalue )
	{
		getData(edict); 

		if ( !m_data ) 
			return defaultvalue; 

		return (float)(*(int *)m_data);
	}

	static void resetError () { m_berror = false; }
	static bool isError () { return m_berror; }

	int getOffset()
	{
		return m_offset;
	}
private:
	unsigned int m_offset;
	unsigned int m_preoffset;
	void *m_data;
	char *m_class;
	char *m_value;

	static bool m_berror;
};


extern CClassInterfaceValue g_GetProps[GET_PROPDATA_MAX];
class CTFObjectiveResource;
class CTeamRoundTimer;
#define DEFINE_GETPROP(id,classname,value,preoffs)\
 g_GetProps[id] = CClassInterfaceValue( CClassInterfaceValue ( classname, value, preoffs ) )

class CClassInterface
{
public:
	static void init ();

	static const char *FindEntityNetClass(int start, const char *classname);
	static edict_t *FindEntityByNetClass(int start, const char *classname);
	static edict_t *FindEntityByNetClassNearest(Vector vstart, const char *classname);
	static edict_t *FindEntityByClassnameNearest(Vector vstart, const char *classname, float fMinDist = 8192.0f, edict_t *pOwner = NULL );

	// TF2
	static int getTF2Score ( edict_t *edict );
	static void setupCTeamRoundTimer ( CTeamRoundTimer *pTimer );
	inline static float getRageMeter ( edict_t *edict ) { return g_GetProps[GETPROP_TF2_RAGEMETER].getFloat(edict,0); }
	inline static int getFlags ( edict_t *edict ) { return g_GetProps[GETPROP_ENTITY_FLAGS].getInt(edict,0); }
	inline static int getTeam ( edict_t *edict ) { return g_GetProps[GETPROP_TEAM].getInt(edict,0); }
	inline static float getPlayerHealth ( edict_t *edict ) { return g_GetProps[GETPROP_PLAYERHEALTH].getFloatFromInt(edict,0); }
	inline static int getEffects ( edict_t *edict ) { return g_GetProps[GETPROP_EFFECTS].getInt(edict,0); }
	inline static int *getAmmoList ( edict_t *edict ) { return g_GetProps[GETPROP_AMMO].getIntPointer(edict); }
	//static unsigned int findOffset(const char *szType,const char *szClass);
	inline static int getTF2NumHealers ( edict_t *edict ) { return g_GetProps[GETPROP_TF2_NUMHEALERS].getInt(edict,0); }
	inline static int getTF2Conditions ( edict_t *edict ) { return g_GetProps[GETPROP_TF2_CONDITIONS].getInt(edict,0); }
	inline static bool getVelocity ( edict_t *edict, Vector *v ) {return g_GetProps[GETPROP_VELOCITY].getVector(edict,v); }
	inline static int getTF2Class ( edict_t *edict ) { return g_GetProps[GETPROP_TF2CLASS].getInt(edict,0); }
	inline static float TF2_getEnergyDrinkMeter(edict_t * edict) { return g_GetProps[GETPROP_TF2_ENERGYDRINKMETER].getFloat(edict, 0); }
	inline static edict_t *TF2_getActiveWeapon(edict_t *edict) { return g_GetProps[GETPROP_TF2_ACTIVEWEAPON].getEntity(edict); }
	// set weapon
	static bool TF2_setActiveWeapon(edict_t *edict, edict_t *pWeapon)
	{
		CBaseHandle *pHandle = g_GetProps[GETPROP_TF2_ACTIVEWEAPON].getEntityHandle(edict);
		pHandle->Set(pWeapon->GetNetworkable()->GetEntityHandle());
	}
	inline static void TF2_SetBuilderType(edict_t *pBuilder, int itype)
	{
		int *pitype = g_GetProps[GETPROP_TF2_BUILDER_TYPE].getIntPointer(pBuilder);

		*pitype = itype;
			//, ]
	}
	inline static int getChargeResistType(edict_t *pMedigun)
	{
		return g_GetProps[GETPROP_TF2_CHARGE_RESIST_TYPE].getInt(pMedigun, 0);
	}
	inline static void TF2_SetBuilderMode(edict_t *pBuilder, int imode)
	{
		int *pitype = g_GetProps[GETPROP_TF2_BUILDER_MODE].getIntPointer(pBuilder);

		*pitype = imode;
		//GETPROP_TF2_BUILDER_MODE, ]
	}
	//Jrob
	inline static int getTF2DesiredClass(edict_t *edict) { return g_GetProps[GETPROP_TF2DESIREDCLASS].getInt(edict, 0); }
	inline static void setTF2Class(edict_t *edict, int _class)
	{
		int* p = g_GetProps[GETPROP_TF2DESIREDCLASS].getIntPointer(edict);
		if (p != NULL) *p = _class;
	}
	//end Jrob
	inline static bool TF2_IsMedievalMode(void*gamerules) { return g_GetProps[GETPROP_TF2_MEDIEVALMODE].getBool(gamerules, false, false);}
	inline static int TF2_getRoundState(void *gamerules) { return g_GetProps[GETPROP_TF2_ROUNDSTATE].getInt(gamerules, 0, 0); }
	inline static float getTF2SpyCloakMeter ( edict_t *edict ) { return g_GetProps[GETPROP_TF2SPYMETER].getFloat(edict,0); }
	inline static int getWaterLevel ( edict_t *edict ) { return g_GetProps[GETPROP_WATERLEVEL].getInt(edict,0); }
	inline static void updateSimulationTime ( edict_t *edict )
	{
		float *m_flSimulationTime = g_GetProps[GETPROP_SIMULATIONTIME].getFloatPointer(edict);

		if ( m_flSimulationTime )
			*m_flSimulationTime = gpGlobals->curtime;
	}

	inline static bool *getDODCPVisible ( edict_t *pResource ) { return g_GetProps[GETPROP_DOD_CP_VISIBLE].getBoolPointer(pResource); }
	static bool getTF2SpyDisguised( edict_t *edict, int *_class, int *_team, int *_index, int *_health ) 
	{ 
		CClassInterfaceValue::resetError();
		if ( _team )
		*_team = g_GetProps[GETPROP_TF2SPYDISGUISED_TEAM].getInt(edict,0); 

		if ( _class )
		*_class = g_GetProps[GETPROP_TF2SPYDISGUISED_CLASS].getInt(edict,0); 

		if  ( _index )
		*_index = g_GetProps[GETPROP_TF2SPYDISGUISED_TARGET_INDEX].getInt(edict,0); 

		if ( _health )
		*_health = g_GetProps[GETPROP_TF2SPYDISGUISED_DIS_HEALTH].getInt(edict,0);

		return !CClassInterfaceValue::isError();
	}

	inline static int TF2_getItemDefinitionIndex(edict_t *edict)
	{
		return g_GetProps[GETPROP_TF2_ITEMDEFINITIONINDEX].getInt(edict, 0);
	}
	
	inline static bool isCarryingObj ( edict_t *edict ) { return g_GetProps[GETPROP_TF2_ISCARRYINGOBJ].getBool(edict,false); }
	inline static edict_t *getCarriedObj ( edict_t *edict ) { return g_GetProps[GETPROP_TF2_GETCARRIEDOBJ].getEntity(edict); }
	inline static bool getMedigunHealing ( edict_t *edict ) { return g_GetProps[GETPROP_TF2MEDIGUN_HEALING].getBool(edict,false); }
	inline static edict_t *getMedigunTarget ( edict_t *edict ) { return g_GetProps[GETPROP_TF2MEDIGUN_TARGETTING].getEntity(edict); }
	inline static edict_t *getSentryEnemy ( edict_t *edict ) { return g_GetProps[GETPROP_SENTRY_ENEMY].getEntity(edict); }
	inline static edict_t *getOwner ( edict_t *edict ) { return g_GetProps[GETPROP_ALL_ENTOWNER].getEntity(edict); }
	inline static bool isMedigunTargetting ( edict_t *pgun, edict_t *ptarget) { return (g_GetProps[GETPROP_TF2MEDIGUN_TARGETTING].getEntity(pgun) == ptarget); }
	//static void setTickBase ( edict_t *edict, int tickbase ) { return ;
	inline static int isTeleporterMode (edict_t *edict, eTeleMode mode ) { return (g_GetProps[GETPROP_TF2TELEPORTERMODE].getInt(edict,-1) == (int)mode); }
	inline static edict_t *getCurrentWeapon (edict_t *player) { return g_GetProps[GETPROP_CURRENTWEAPON].getEntity(player); }
	inline static int getUberChargeLevel (edict_t *pWeapon) { return (int)(g_GetProps[GETPROP_TF2UBERCHARGE_LEVEL].getFloat(pWeapon,0)*100.0); }
	//static void test ();
	inline static float getSentryHealth ( edict_t *edict ) { return g_GetProps[GETPROP_TF2SENTRYHEALTH].getFloatFromInt(edict,100); }
	inline static float getDispenserHealth ( edict_t *edict ) { return g_GetProps[GETPROP_TF2DISPENSERHEALTH].getFloatFromInt(edict,100); }
	inline static float getTeleporterHealth ( edict_t *edict ) { return g_GetProps[GETPROP_TF2TELEPORTERHEALTH].getFloatFromInt(edict,100); }
	inline static bool isObjectCarried ( edict_t *edict ) { return g_GetProps[GETPROP_TF2OBJECTCARRIED].getBool(edict,false); }
	inline static int getTF2UpgradeLevel ( edict_t *edict ) { return g_GetProps[GETPROP_TF2OBJECTUPGRADELEVEL].getInt(edict,0); }
	inline static int getTF2SentryUpgradeMetal ( edict_t *edict ) { return g_GetProps[GETPROP_TF2OBJECTUPGRADEMETAL].getInt(edict,0); }
	inline static int getTF2SentryShells ( edict_t *edict ) { return g_GetProps[GETPROP_TF2OBJECTSHELLS].getInt(edict,0); }
	inline static int getTF2SentryRockets ( edict_t *edict ) { return g_GetProps[GETPROP_TF2OBJECTROCKETS].getInt(edict,0); }
	
	static bool getTF2ObjectiveResource ( CTFObjectiveResource *pResource );

	inline static float getTF2TeleRechargeTime(edict_t *edict) { return g_GetProps[GETPROP_TF2_TELEPORT_RECHARGETIME].getFloat(edict,0); } 
	inline static float getTF2TeleRechargeDuration(edict_t *edict) { return g_GetProps[GETPROP_TF2_TELEPORT_RECHARGEDURATION].getFloat(edict,0); } 

	inline static int getTF2GetBuildingMaxHealth ( edict_t *edict ) { return g_GetProps[GETPROP_TF2OBJECTMAXHEALTH].getInt(edict,0); }
	inline static int getTF2DispMetal ( edict_t *edict ) { return g_GetProps[GETPROP_TF2DISPMETAL].getInt(edict,0); }
	inline static bool getTF2BuildingIsMini ( edict_t *edict ) { return g_GetProps[GETPROP_TF2MINIBUILDING].getBool(edict,false); }
	inline static float getMaxSpeed(edict_t *edict) { return g_GetProps[GETPROP_MAXSPEED].getFloat(edict,0); }
	inline static float getSpeedFactor(edict_t *edict) { return g_GetProps[GETPROP_CONSTRAINT_SPEED].getFloat(edict,0); } 
	inline static bool isObjectBeingBuilt(edict_t *edict) { return g_GetProps[GETPROP_TF2OBJECTBUILDING].getBool(edict,false); }
	inline static edict_t *getGroundEntity(edict_t *edict) { return g_GetProps[GETPROP_GROUND_ENTITY].getEntity(edict); }
	inline static edict_t *gravityGunObject(edict_t *pgun) { return g_GetProps[GETPROP_HL2DM_PHYSCANNON_ATTACHED].getEntity(pgun); }
	inline static bool gravityGunOpen(edict_t *pgun) { return g_GetProps[GETPROP_HL2DM_PHYSCANNON_OPEN].getBool(pgun,false); }
	inline static float auxPower (edict_t *player) { return g_GetProps[GETPROP_HL2DM_PLAYER_AUXPOWER].getFloat(player,0);} 
	inline static edict_t *onLadder ( edict_t *player ) { return g_GetProps[GETPROP_HL2DM_LADDER_ENT].getEntity(player);}
	inline static CBaseHandle *getWeaponList ( edict_t *player ) { return g_GetProps[GETPROP_WEAPONLIST].getEntityHandle(player);}
	inline static int getWeaponState ( edict_t *pgun ) { return g_GetProps[GETPROP_WEAPONSTATE].getInt(pgun,0); }

	inline static edict_t *getPipeBombOwner ( edict_t *pPipeBomb ) { return g_GetProps[GETPROP_PIPEBOMB_OWNER].getEntity(pPipeBomb); }

	inline static int getDODBombState ( edict_t *pBombTarget ) { return g_GetProps[GETPROP_DOD_BOMB_STATE].getInt(pBombTarget,0); }
	inline static int getDODBombTeam ( edict_t *pBombTarget ) { return g_GetProps[GETPROP_DOD_BOMB_TEAM].getInt(pBombTarget,0); }
	inline static int *getWeaponClip1Pointer ( edict_t *pgun ) { return g_GetProps[GETPROP_WEAPONCLIP1].getIntPointer(pgun); }
	inline static int *getWeaponClip2Pointer ( edict_t *pgun ) { return g_GetProps[GETPROP_WEAPONCLIP2].getIntPointer(pgun); }
	inline static int getOffset(int id) { return g_GetProps[id].getOffset(); }
	inline static void getWeaponClip ( edict_t *pgun, int *iClip1, int *iClip2 ) { *iClip1 = g_GetProps[GETPROP_WEAPONCLIP1].getInt(pgun,0); *iClip2 = g_GetProps[GETPROP_WEAPONCLIP2].getInt(pgun,0); }
	inline static void getAmmoTypes ( edict_t *pgun, int *iAmmoType1, int *iAmmoType2 ) { *iAmmoType1 = g_GetProps[GETPROP_WEAPON_AMMOTYPE1].getInt(pgun,-1); *iAmmoType2 = g_GetProps[GETPROP_WEAPON_AMMOTYPE2].getInt(pgun,-1);} 

	inline static int getPlayerClassDOD(edict_t *player) { return g_GetProps[GETPROP_DOD_PLAYERCLASS].getInt(player,0); }
	inline static void getPlayerInfoDOD(edict_t *player, bool *m_bProne, float *m_flStamina)
	{
		*m_bProne = g_GetProps[GETPROP_DOD_PRONE].getBool(player,false);
		if ( m_flStamina )
			*m_flStamina = g_GetProps[GETPROP_DOD_STAMINA].getFloat(player,0);
	}

	inline static float getAnimCycle ( edict_t *edict) 
	{	
		return g_GetProps[GETPROP_CYCLE].getFloat(edict,0);
	}

	inline static void getAnimatingInfo ( edict_t *edict, float *flCycle, int *iSequence ) 
	{	
		*flCycle = g_GetProps[GETPROP_CYCLE].getFloat(edict,0);
		*iSequence = g_GetProps[GETPROP_SEQUENCE].getInt(edict,false);
	}

	inline static int getPlayerFlags (edict_t *player) { return g_GetProps[GETPROP_ENTITYFLAGS].getInt(player,0);}
	inline static int *getPlayerFlagsPointer (edict_t *player) { return g_GetProps[GETPROP_ENTITYFLAGS].getIntPointer(player);}

	inline static int getDODNumControlPoints ( edict_t *pResource )
	{
		return g_GetProps[GETPROP_DOD_CP_NUMCAPS].getInt(pResource,0);
	}

	inline static Vector *getOrigin ( edict_t *pPlayer )
	{
		return g_GetProps[GETPROP_ORIGIN].getVectorPointer(pPlayer);
	}

	inline static void setOrigin ( edict_t *pPlayer, Vector vOrigin )
	{
		Vector *vEntOrigin = g_GetProps[GETPROP_ORIGIN].getVectorPointer(pPlayer);

		*vEntOrigin = vOrigin;
	}

	inline static Vector *getDODCP_Positions ( edict_t *pResource )
	{
		return g_GetProps[GETPROP_DOD_CP_POSITIONS].getVectorPointer(pResource);
	}

	inline static void getDODFlagInfo (edict_t *pResource, int **m_iNumAxis, int **m_iNumAllies, int **m_iOwner, int **m_iNumAlliesReq, int **m_iNumAxisReq )
	{
		*m_iNumAxis = g_GetProps[GETPROP_DOD_CP_NUM_AXIS].getIntPointer(pResource);
		*m_iNumAllies = g_GetProps[GETPROP_DOD_CP_NUM_ALLIES].getIntPointer(pResource);
		*m_iOwner = g_GetProps[GETPROP_DOD_CP_OWNER].getIntPointer(pResource);
		*m_iNumAlliesReq = g_GetProps[GETPROP_DOD_CP_ALLIES_REQ_CAP].getIntPointer(pResource);
		*m_iNumAxisReq = g_GetProps[GETPROP_DOD_CP_AXIS_REQ_CAP].getIntPointer(pResource);
	}

	inline static void 	getDODBombInfo ( edict_t *pResource, bool **m_bBombPlanted, int **m_iBombsRequired, int **m_iBombsRemaining, bool **m_bBombBeingDefused)
	{
		*m_bBombPlanted = g_GetProps[GETPROP_DOD_BOMBSPLANTED].getBoolPointer(pResource);
		*m_iBombsRequired = g_GetProps[GETPROP_DOD_BOMBSREQ].getIntPointer(pResource);
		*m_iBombsRemaining = g_GetProps[GETPROP_DOD_BOMBSREMAINING].getIntPointer(pResource);
		*m_bBombBeingDefused = g_GetProps[GETPROP_DOD_BOMBSDEFUSED].getBoolPointer(pResource);
	}

	inline static float getTF2TauntYaw ( edict_t *edict ) { return g_GetProps[GETPROP_TF2_TAUNTYAW].getFloat(edict,0); }
	inline static bool getTF2HighFiveReady ( edict_t *edict ) { return g_GetProps[GETPROP_TF2_HIGHFIVE].getBool(edict,false); }
	inline static edict_t *getHighFivePartner ( edict_t *edict ) { return g_GetProps[GETPROP_TF2_HIGHFIVE_PARTNER].getEntity(edict); }

	inline static int getDesPlayerClassDOD(edict_t *player) { return g_GetProps[GETPROP_DOD_DES_PLAYERCLASS].getInt(player,0); }

	inline static bool isSniperWeaponZoomed (edict_t *weapon) { return g_GetProps[GETPROP_DOD_SNIPER_ZOOMED].getBool(weapon,false); }
	inline static bool isMachineGunDeployed (edict_t *weapon) { return g_GetProps[GETPROP_DOD_MACHINEGUN_DEPLOYED].getBool(weapon,false); }
	inline static bool isRocketDeployed ( edict_t *weapon ) { return g_GetProps[GETPROP_DOD_ROCKET_DEPLOYED].getBool(weapon,false); }

	inline static bool isMoveType ( edict_t *pent, int movetype )
	{
		return ((g_GetProps[GETPROP_MOVETYPE].getInt(pent,0) & 15) == movetype);
	}

	inline static byte getTakeDamage ( edict_t *pent )
	{
		return (byte)(g_GetProps[GETPROP_TAKEDAMAGE].getInt(pent,0));
	}

	inline static byte *getTakeDamagePointer ( edict_t *pent )
	{
		return (g_GetProps[GETPROP_TAKEDAMAGE].getBytePointer(pent));
	}

	inline static int getMoveType ( edict_t *pent )
	{
		return (g_GetProps[GETPROP_MOVETYPE].getInt(pent,0) & 15);
	}

	inline static byte *getMoveTypePointer ( edict_t *pent )
	{
		return (g_GetProps[GETPROP_MOVETYPE].getBytePointer(pent));
	}

	inline static edict_t *getGrenadeThrower ( edict_t *gren )
	{
		return g_GetProps[GETPROP_DOD_GREN_THROWER].getEntity(gren);
	}

	inline static int getPlayerScoreDOD ( edict_t *resource, edict_t *pPlayer )
	{
		int *score_array = g_GetProps[GETPROP_DOD_SCORE].getIntPointer(resource);

		return (score_array!=NULL) ? score_array[ENTINDEX(pPlayer)] : 0;
	}

	inline static int getPlayerObjectiveScoreDOD ( edict_t *resource, edict_t *pPlayer )
	{
		int *score_array = g_GetProps[GETPROP_DOD_OBJSCORE].getIntPointer(resource);

		return (score_array!=NULL) ? score_array[ENTINDEX(pPlayer)] : 0;
	}

	inline static int getPlayerDeathsDOD ( edict_t *resource, edict_t *pPlayer )
	{
		int *score_array = g_GetProps[GETPROP_DOD_DEATHS].getIntPointer(resource);

		return (score_array!=NULL) ? score_array[ENTINDEX(pPlayer)] : 0;
	}

	inline static float getSmokeSpawnTime ( edict_t *pSmoke )
	{
		return g_GetProps[GETPROP_DOD_SMOKESPAWN_TIME].getFloat(pSmoke,0);
	}
	
	inline static float getRoundTime ( edict_t *pGamerules )
	{
		return g_GetProps[GETPROP_DOD_ROUNDTIME].getFloat(pGamerules,0);
	}

	inline static bool isGarandZoomed ( edict_t *pGarand )
	{
		return g_GetProps[GETPROP_DOD_GARANDZOOM].getBool(pGarand,false);
	}

	inline static bool isK98Zoomed( edict_t *pK98 )
	{
		return g_GetProps[GETPROP_DOD_K98ZOOM].getBool(pK98,false);
	}
	// HL2DM
	//static void 

	inline static bool areAlliesBombing (edict_t *pRes) 
	{
		return g_GetProps[GETPROP_DOD_ALLIESBOMBING].getBool(pRes,false);
	}
	inline static bool areAxisBombing (edict_t *pRes) 
	{
		return g_GetProps[GETPROP_DOD_AXISBOMBING].getBool(pRes,false);
	}
	inline static int *isBombPlantedList (edict_t *pRes) 
	{
		return g_GetProps[GETPROP_DOD_BOMBSPLANTED].getIntPointer(pRes);
	}
	inline static int *getNumBombsRequiredList (edict_t *pRes) 
	{
		return g_GetProps[GETPROP_DOD_BOMBSREQ].getIntPointer(pRes);
	}
	inline static int *isBombDefusingList (edict_t *pRes) 
	{
		return g_GetProps[GETPROP_DOD_BOMBSDEFUSED].getIntPointer(pRes);
	}
	inline static int *getNumBombsRemaining ( edict_t *pRes )
	{
		return g_GetProps[GETPROP_DOD_BOMBSREMAINING].getIntPointer(pRes);
	}

	inline static bool isPlayerDefusingBomb_DOD(edict_t *pPlayer)
	{
		return g_GetProps[GETPROP_DOD_DEFUSINGBOMB].getBool(pPlayer,false);
	}

	inline static bool isPlayerPlantingBomb_DOD(edict_t *pPlayer)
	{
		return g_GetProps[GETPROP_DOD_PLANTINGBOMB].getBool(pPlayer,false);
	}

	inline static bool isSentryGunBeingPlaced (edict_t *pSentry )
	{
		return g_GetProps[GETPROP_SENTRYGUN_PLACING].getBool(pSentry,false);
	}

private:
	static CClassInterfaceValue g_GetProps[GET_PROPDATA_MAX];

};

#endif