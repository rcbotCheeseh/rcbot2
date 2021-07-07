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

#ifndef __linux__
// for file stuff
#include <windows.h>
#define WIN32_LEAN_AND_MEAN

#include <conio.h>

#endif

#include "bot.h"
#include "bot_cvars.h"
#include "bot_globals.h"
#include "bot_strings.h"
#include "bot_waypoint_locations.h"
#include "bot_getprop.h"
#include "bot_weapons.h"

#include "ndebugoverlay.h"

#ifndef __linux__
#include <direct.h> // for mkdir
#include <sys/stat.h>
#else
#include <fcntl.h>
#include <sys/stat.h>
#endif

extern IServerGameEnts *servergameents;

///////////
trace_t CBotGlobals :: m_TraceResult;
char * CBotGlobals :: m_szModFolder = NULL;
eModId CBotGlobals :: m_iCurrentMod = MOD_UNSUPPORTED;
CBotMod *CBotGlobals :: m_pCurrentMod = NULL;
bool CBotGlobals :: m_bMapRunning = false;
int CBotGlobals :: m_iMaxClients = 0;
int CBotGlobals :: m_iEventVersion = 1;
int CBotGlobals :: m_iWaypointDisplayType = 0;
char CBotGlobals :: m_szMapName[MAX_MAP_STRING_LEN];
bool CBotGlobals :: m_bTeamplay = false;
char *CBotGlobals :: m_szRCBotFolder = NULL;

///////////

extern IVDebugOverlay *debugoverlay;

class CTraceFilterVis : public CTraceFilter
{
public:
	CTraceFilterVis(edict_t *pPlayer, edict_t *pHit = NULL )
	{
		m_pPlayer = pPlayer;
		m_pHit = pHit;
	}

	virtual bool ShouldHitEntity( IHandleEntity *pServerEntity, int contentsMask )
	{ 
		if ( m_pPlayer && (pServerEntity == (IHandleEntity*)m_pPlayer->GetIServerEntity()) )
			return false;

		if ( m_pHit && (pServerEntity == (IHandleEntity*)m_pHit->GetIServerEntity()) )
			return false;

		return true; 
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
private:
	edict_t *m_pPlayer;
	edict_t *m_pHit;
};

CBotGlobals :: CBotGlobals ()
{
	init();
}

void CBotGlobals :: init ()
{
	m_iCurrentMod = MOD_UNSUPPORTED;
	m_szModFolder[0] = 0;
}

bool CBotGlobals ::isAlivePlayer ( edict_t *pEntity )
{
	return pEntity && ENTINDEX(pEntity) && (ENTINDEX(pEntity) <= gpGlobals->maxClients) && (entityIsAlive(pEntity));
}

//new map
void CBotGlobals :: setMapName ( const char *szMapName ) 
{ 
	strncpy(m_szMapName,szMapName,MAX_MAP_STRING_LEN-1); 
	m_szMapName[MAX_MAP_STRING_LEN-1] = 0; 	
}

char *CBotGlobals :: getMapName () 
{ 
	return m_szMapName; 
}

bool CBotGlobals :: isCurrentMod ( eModId modid )
{
	return m_pCurrentMod->getModId() == modid;
}

int CBotGlobals ::numPlayersOnTeam(int iTeam, bool bAliveOnly)
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
				if ( bAliveOnly )
				{
					if ( CBotGlobals::entityIsAlive(pEdict) )
						num++;
				}
				else 
					num++;
			}
		}
	}
	return num;
}

bool CBotGlobals::dirExists(const char *path)
{
#ifdef _WIN32

	struct _stat info;

	if (_stat(path, &info) != 0)
		return false;
	else if (info.st_mode & _S_IFDIR)
		return true;
	else
		return false;

#else

	struct stat info;

	if (stat(path, &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else
		return false;

#endif
}

void CBotGlobals::readRCBotFolder()
{
	KeyValues *mainkv = new KeyValues("Metamod Plugin");

	if (mainkv->LoadFromFile(filesystem, "addons/metamod/rcbot2.vdf", "MOD")) {
		char folder[256] = "\0";
		const char *szRCBotFolder = mainkv->GetString("rcbot2path");

		if (szRCBotFolder && *szRCBotFolder) {
			CBotGlobals::botMessage(NULL, 0, "RCBot Folder -> trying %s", szRCBotFolder);

			if (!dirExists(szRCBotFolder)) {
				snprintf(folder, sizeof(folder), "%s/%s", CBotGlobals::modFolder(), szRCBotFolder);

				szRCBotFolder = CStrings::getString(folder);
				CBotGlobals::botMessage(NULL, 0, "RCBot Folder -> trying %s", szRCBotFolder);

				if (!dirExists(szRCBotFolder)) {
					CBotGlobals::botMessage(NULL, 0, "RCBot Folder -> not found ...");
				}
			}

			m_szRCBotFolder = CStrings::getString(szRCBotFolder);
		}
	}

	mainkv->deleteThis();
}

float CBotGlobals :: grenadeWillLand ( Vector vOrigin, Vector vEnemy, float fProjSpeed, float fGrenadePrimeTime, float *fAngle )
{
	static float g;
	Vector v_comp = vEnemy-vOrigin;
	float fDistance = v_comp.Length();

	v_comp = v_comp/fDistance;

	g = sv_gravity.IsValid()? sv_gravity.GetFloat() : 800.f;

	if ( fAngle == NULL )
	{

		return false;
	}
	else
	{
		// use angle -- work out time
				// work out angle
		float vhorz;
		float vvert;

		SinCos(DEG2RAD(*fAngle),&vvert,&vhorz);

		vhorz *= fProjSpeed;
		vvert *= fProjSpeed;

		float t = fDistance/vhorz;

		// within one second of going off
		if ( fabs(t-fGrenadePrimeTime) < 1.0f )
		{
			float ffinaly =  vOrigin.z + (vvert*t) - ((g*0.5)*(t*t));

			return ( fabs(ffinaly - vEnemy.z) < BLAST_RADIUS ); // ok why not
		}
	}

	return false;
}

// TO DO :: put in CClients ?
edict_t *CBotGlobals :: findPlayerByTruncName ( const char *name )
// find a player by a truncated name "name".
// e.g. name = "Jo" might find a player called "John"
{
	edict_t *pent = NULL;
	IPlayerInfo *pInfo;
	int i;

	for( i = 1; i <= maxClients(); i ++ )
	{
		pent = INDEXENT(i);

		if( pent && CBotGlobals::isNetworkable(pent) )
		{
			int length = strlen(name);						 

			char arg_lwr[128];
			char pent_lwr[128];

			strcpy(arg_lwr,name);

			pInfo = playerinfomanager->GetPlayerInfo( pent );
			
			if ( pInfo == NULL )
				continue;

			strcpy(pent_lwr,pInfo->GetName());

			__strlow(arg_lwr);
			__strlow(pent_lwr);

			if( strncmp( arg_lwr,pent_lwr,length) == 0 )
			{
				return pent;
			}
		}
	}

	return NULL;
}

class CTraceFilterHitAllExceptPlayers : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity( IHandleEntity *pServerEntity, int contentsMask )
	{ 
		return pServerEntity->GetRefEHandle().GetEntryIndex() <= gpGlobals->maxClients; 
	}
};

//-----------------------------------------------------------------------------
// traceline methods
//-----------------------------------------------------------------------------
class CTraceFilterSimple : public CTraceFilter
{
public:
	
	CTraceFilterSimple( const IHandleEntity *passentity1, const IHandleEntity *passentity2, int collisionGroup )
	{
		m_pPassEnt1 = passentity1;
		
		if ( passentity2 )
			m_pPassEnt2 = passentity2;

		m_collisionGroup = collisionGroup;
	}
	virtual bool ShouldHitEntity( IHandleEntity *pHandleEntity, int contentsMask )
	{
		if ( m_pPassEnt1 == pHandleEntity )
			return false;
		if ( m_pPassEnt2 == pHandleEntity )
			return false;
#if defined(_DEBUG) && !defined(__linux__)
		if ( CClients::clientsDebugging(BOT_DEBUG_VIS) )
		{
			static edict_t *edict;
			
			edict = INDEXENT(pHandleEntity->GetRefEHandle().GetEntryIndex());

			debugoverlay->AddTextOverlayRGB(CBotGlobals::entityOrigin(edict),0,2.0f,255,100,100,200,"Traceline hit %s",edict->GetClassName());
		}
#endif
		return true;
	}
	//virtual void SetPassEntity( const IHandleEntity *pPassEntity ) { m_pPassEnt = pPassEntity; }
	//virtual void SetCollisionGroup( int iCollisionGroup ) { m_collisionGroup = iCollisionGroup; }

	//const IHandleEntity *GetPassEntity( void ){ return m_pPassEnt;}

private:
	const IHandleEntity *m_pPassEnt1;
	const IHandleEntity *m_pPassEnt2;
	int m_collisionGroup;
};

bool CBotGlobals :: checkOpensLater ( Vector vSrc, Vector vDest )
{
	CTraceFilterSimple traceFilter( NULL, NULL, MASK_PLAYERSOLID );

	traceLine (vSrc,vDest,MASK_PLAYERSOLID,&traceFilter);

	return (traceVisible(NULL));
}


bool CBotGlobals :: isVisibleHitAllExceptPlayer ( edict_t *pPlayer, Vector vSrc, Vector vDest, edict_t *pDest )
{
	const IHandleEntity *ignore = pPlayer->GetIServerEntity();

	CTraceFilterSimple traceFilter( ignore, ((pDest==NULL)?NULL:pDest->GetIServerEntity()), MASK_ALL );

	traceLine (vSrc,vDest,MASK_SHOT|MASK_VISIBLE,&traceFilter);

	return (traceVisible(pDest));
}

bool CBotGlobals :: isVisible ( edict_t *pPlayer, Vector vSrc, Vector vDest)
{
	CTraceFilterWorldAndPropsOnly filter;

	traceLine (vSrc,vDest,MASK_SOLID_BRUSHONLY|CONTENTS_OPAQUE,&filter);

	return (traceVisible(NULL));
}

bool CBotGlobals :: isVisible ( edict_t *pPlayer, Vector vSrc, edict_t *pDest )
{
	//CTraceFilterWorldAndPropsOnly filter;//	CTraceFilterHitAll filter;

	CTraceFilterWorldAndPropsOnly filter;

	traceLine (vSrc,entityOrigin(pDest),MASK_SOLID_BRUSHONLY|CONTENTS_OPAQUE,&filter);

	return (traceVisible(pDest));
}

bool CBotGlobals :: isShotVisible ( edict_t *pPlayer, Vector vSrc, Vector vDest, edict_t *pDest )
{
	//CTraceFilterWorldAndPropsOnly filter;//	CTraceFilterHitAll filter;

	CTraceFilterVis filter = CTraceFilterVis(pPlayer,pDest);

	traceLine (vSrc,vDest,MASK_SHOT,&filter);

	return (traceVisible(pDest));
}

bool CBotGlobals :: isVisible (Vector vSrc, Vector vDest)
{
	CTraceFilterWorldAndPropsOnly filter;

	traceLine (vSrc,vDest,MASK_SOLID_BRUSHONLY|CONTENTS_OPAQUE,&filter);

	return traceVisible(NULL);
}

void CBotGlobals :: traceLine (Vector vSrc, Vector vDest, unsigned int mask, ITraceFilter *pFilter)
{
	Ray_t ray;
	memset(&m_TraceResult,0,sizeof(trace_t));
	ray.Init( vSrc, vDest );
	enginetrace->TraceRay( ray, mask, pFilter, &m_TraceResult );
}

float CBotGlobals :: quickTraceline (edict_t *pIgnore,Vector vSrc, Vector vDest)
{
	CTraceFilterVis filter = CTraceFilterVis(pIgnore);

	Ray_t ray;
	memset(&m_TraceResult,0,sizeof(trace_t));
	ray.Init( vSrc, vDest );
	enginetrace->TraceRay( ray, MASK_NPCSOLID_BRUSHONLY, &filter, &m_TraceResult );
	return m_TraceResult.fraction;
}

float CBotGlobals :: DotProductFromOrigin ( edict_t *pEnemy, Vector pOrigin )
{
	static Vector vecLOS;
	static float flDot;
	IPlayerInfo *p;
	
	Vector vForward;
	QAngle eyes;

	p = playerinfomanager->GetPlayerInfo(pEnemy);

	if (!p )
		return 0;

	eyes = p->GetAbsAngles();

	// in fov? Check angle to edict
	AngleVectors(eyes,&vForward);
	
	vecLOS = pOrigin - CBotGlobals::entityOrigin(pEnemy);
	vecLOS = vecLOS/vecLOS.Length();
	
	flDot = DotProduct (vecLOS , vForward );
	
	return flDot; 
}


float CBotGlobals :: DotProductFromOrigin ( Vector vPlayer, Vector vFacing, QAngle eyes )
{
	static Vector vecLOS;
	static float flDot;

	Vector vForward;

	// in fov? Check angle to edict
	AngleVectors(eyes,&vForward);
	
	vecLOS = vFacing - vPlayer;
	vecLOS = vecLOS/vecLOS.Length();
	
	flDot = DotProduct (vecLOS , vForward );
	
	return flDot; 
}

bool CBotGlobals :: traceVisible (edict_t *pEnt)
{
	return (m_TraceResult.fraction >= 1.0)||(m_TraceResult.m_pEnt && pEnt && (m_TraceResult.m_pEnt==pEnt->GetUnknown()->GetBaseEntity()));
}

bool CBotGlobals::initModFolder() {
	char szGameFolder[512];
	engine->GetGameDir(szGameFolder, 512);

	int iLength = strlen(CStrings::getString(szGameFolder));
	int pos = iLength - 1;

	while ((pos > 0) && (szGameFolder[pos] != '\\') && (szGameFolder[pos] != '/')) {
		pos--;
	}
	pos++;

	m_szModFolder = CStrings::getString(&szGameFolder[pos]);
	return true;
}

bool CBotGlobals :: gameStart ()
{
	char szGameFolder[512];
	engine->GetGameDir(szGameFolder,512);
	/*
	CFileSystemPassThru a;
	a.InitPassThru(filesystem,true);
	a.GetCurrentDirectoryA(szSteamFolder,512);
*/
	//filesystem->GetCurrentDirectory(szSteamFolder,512);

	size_t iLength = strlen(CStrings::getString(szGameFolder));

	size_t pos = iLength-1;

	while ( (pos > 0) && (szGameFolder[pos] != '\\') && (szGameFolder[pos] != '/') )
	{
		pos--;
	}
	pos++;
	
	m_szModFolder = CStrings::getString(&szGameFolder[pos]);

	CBotMods::readMods();
	
	m_pCurrentMod = CBotMods::getMod(m_szModFolder);

	if ( m_pCurrentMod != NULL )
	{
		m_iCurrentMod = m_pCurrentMod->getModId();

		m_pCurrentMod->initMod();

		CBots::init();

		return true;
	}
	else
	{
		Msg("[BOT ERROR] Mod not found. Please edit the bot_mods.ini in the bot config folder\n\ngamedir = %s\n",m_szModFolder);

		return false;
	}
}

void CBotGlobals :: levelInit ()
{

}

int CBotGlobals :: countTeamMatesNearOrigin ( Vector vOrigin, float fRange, int iTeam, edict_t *pIgnore )
{
	int iCount = 0;
	IPlayerInfo *p;

	for ( int i = 1; i <= CBotGlobals::maxClients(); i ++ )
	{
		edict_t *pEdict = INDEXENT(i);

		if ( pEdict->IsFree() )
			continue;

		if ( pEdict == pIgnore )
			continue;

		p = playerinfomanager->GetPlayerInfo(pEdict);

		if ( !p || !p->IsConnected() || p->IsDead() || p->IsObserver() || !p->IsPlayer() )
			continue;

		if ( CClassInterface::getTeam(pEdict) == iTeam )
		{
			Vector vPlayer = entityOrigin(pEdict);

			if ( (vOrigin - vPlayer).Length() <= fRange )
				iCount++;
		}
	}

	return iCount;
}

int CBotGlobals :: numClients ()
{
	int iCount = 0;

	for ( int i = 1; i <= CBotGlobals::maxClients(); i ++ )
	{
		edict_t *pEdict = INDEXENT(i);

		if ( !pEdict )
			continue;
		
		IPlayerInfo *p = playerinfomanager->GetPlayerInfo(pEdict);
		if (!p || p->IsHLTV())
			continue;
		
		if ( engine->GetPlayerUserId(pEdict) > 0 )
			iCount++;
	}

	return iCount;
}

bool CBotGlobals :: entityIsAlive ( edict_t *pEntity )
{
	static short int index;

	index = ENTINDEX(pEntity);

	if ( index && (index <= gpGlobals->maxClients) )
	{
		IPlayerInfo *p = playerinfomanager->GetPlayerInfo(pEntity);

		if ( !p )
			return false;

		return (!p->IsDead() && (p->GetHealth()>0));
	}

	return ( pEntity->GetIServerEntity() && pEntity->GetClassName() && *pEntity->GetClassName() );
	//CBaseEntity *pBaseEntity = CBaseEntity::Instance(pEntity);
	//return pBaseEntity->IsAlive();
}

edict_t *CBotGlobals :: playerByUserId(int iUserId)
{
	for ( int i = 1; i <= maxClients(); i ++ )
	{
		edict_t *pEdict = INDEXENT(i);

		if ( pEdict )
		{
			if ( engine->GetPlayerUserId(pEdict) == iUserId )
				return pEdict;
		}
	}

	return NULL;
}

int CBotGlobals :: getTeam ( edict_t *pEntity )
{
	IPlayerInfo *p = playerinfomanager->GetPlayerInfo(pEntity);
	return p->GetTeamIndex();
}

bool CBotGlobals :: isNetworkable ( edict_t *pEntity )
{
	static IServerEntity *pServerEnt;

	pServerEnt = pEntity->GetIServerEntity();

	return (pServerEnt && (pServerEnt->GetNetworkable() != NULL));
}

/*
inline Vector CBotGlobals :: entityOrigin ( edict_t *pEntity )
{
	return pEntity->GetIServerEntity()->GetCollideable()->GetCollisionOrigin();
	
	Vector vOrigin;

	if ( pEntity && pEntity->GetIServerEntity() && pEntity->GetIServerEntity()->GetCollideable() )//fix?
		vOrigin = pEntity->GetIServerEntity()->GetCollideable()->GetCollisionOrigin();
	else
		vOrigin = Vector(0,0,0);

	return vOrigin;
}*/

void CBotGlobals :: serverSay ( char *fmt, ... )
{
	va_list argptr; 
	static char string[1024];

	va_start (argptr, fmt);
	
	strcpy(string,"say \"");

	vsprintf (&string[5], fmt, argptr); 

	va_end (argptr); 

	strcat(string,"\"");

	engine->ServerCommand(string);
}

// TO DO :: put into CClient
bool CBotGlobals :: setWaypointDisplayType ( int iType )
{
	if ( (iType >= 0) && (iType <= 1) )
	{
		m_iWaypointDisplayType = iType;
		return true;
	}

	return false;
}
// work on this
bool CBotGlobals :: walkableFromTo (edict_t *pPlayer, Vector v_src, Vector v_dest)
{
	CTraceFilterVis filter = CTraceFilterVis(pPlayer);
	float fDistance = sqrt((v_dest - v_src).LengthSqr());
	CClient *pClient = CClients::get(pPlayer);
	Vector vcross = v_dest - v_src;
	Vector vleftsrc,vleftdest, vrightsrc,vrightdest;
	float fWidth = rcbot_wptplace_width.GetFloat();

	if ( v_dest == v_src )
		return true;

	// minimum
	if ( fWidth < 2.0f )
		fWidth = 2.0f;

	if ( pClient->autoWaypointOn() )
		fWidth = 4.0f;

	vcross = vcross / vcross.Length();
	vcross = vcross.Cross(Vector(0,0,1));
	vcross = vcross * (fWidth*0.5f);

	vleftsrc = v_src - vcross;
	vrightsrc = v_src + vcross;

	vleftdest = v_dest - vcross;
	vrightdest = v_dest + vcross;

	if ( fDistance > CWaypointLocations::REACHABLE_RANGE )
		return false;

	//if ( !CBotGlobals::isVisible(v_src,v_dest) )
	//	return false;

	// can swim there?
	if ((enginetrace->GetPointContents( v_src ) == CONTENTS_WATER) &&
		(enginetrace->GetPointContents( v_dest ) == CONTENTS_WATER))
	{
		return true;
	}

	// find the ground
	CBotGlobals::traceLine(v_src,v_src-Vector(0,0,256.0),MASK_NPCSOLID_BRUSHONLY,&filter);
#ifndef __linux__
	debugoverlay->AddLineOverlay(v_src,v_src-Vector(0,0,256.0),255,0,255,false,3);
#endif
	Vector v_ground_src = CBotGlobals::getTraceResult()->endpos + Vector(0,0,1);

	CBotGlobals::traceLine(v_dest,v_dest-Vector(0,0,256.0),MASK_NPCSOLID_BRUSHONLY,&filter);
#ifndef __linux__
	debugoverlay->AddLineOverlay(v_dest,v_dest-Vector(0,0,256.0),255,255,0,false,3);
#endif
	Vector v_ground_dest = CBotGlobals::getTraceResult()->endpos + Vector(0,0,1);

	if ( !CBotGlobals::isVisible(pPlayer,v_ground_src,v_ground_dest) )
	{
#ifndef __linux__
		debugoverlay->AddLineOverlay(v_ground_src,v_ground_dest,0,255,255,false,3);		
#endif
		trace_t *tr = CBotGlobals::getTraceResult();

		// no slope there
		if ( tr->endpos.z > v_src.z )
		{
#ifndef __linux__
			debugoverlay->AddTextOverlay((v_ground_src+v_ground_dest)/2,0,3,"ground fail");
#endif

			CBotGlobals::traceLine(tr->endpos,tr->endpos-Vector(0,0,45),MASK_NPCSOLID_BRUSHONLY,&filter);

			Vector v_jsrc = tr->endpos;

#ifndef __linux__
			debugoverlay->AddLineOverlay(v_jsrc,v_jsrc-Vector(0,0,45),255,255,255,false,3);	
#endif
			// can't jump there
			if ( ((v_jsrc.z - tr->endpos.z) + (v_dest.z-v_jsrc.z)) > 45.0f )
			{
				//if ( (tr->endpos.z > (v_src.z+45)) && (fDistance > 64.0f) )
				//{
#ifndef __linux__
					debugoverlay->AddTextOverlay(tr->endpos,0,3,"jump fail");
#endif
					// check for slope or stairs
					Vector v_norm = v_dest-v_src;
					v_norm = v_norm/sqrt(v_norm.LengthSqr());

					for ( float fDistCheck = 45.0f; fDistCheck < fDistance; fDistCheck += 45.0f )
					{
						Vector v_checkpoint = v_src + (v_norm * fDistCheck);

						// check jump height again
						CBotGlobals::traceLine(v_checkpoint,v_checkpoint-Vector(0,0,45.0f),MASK_NPCSOLID_BRUSHONLY,&filter);

						if ( CBotGlobals::traceVisible(NULL) )
						{
#ifndef __linux__
							debugoverlay->AddTextOverlay(tr->endpos,0,3,"step/jump fail");
#endif
							return false;
						}
					}
				//}
			}
		}
	}

	return CBotGlobals::isVisible(pPlayer,vleftsrc,vleftdest) && CBotGlobals::isVisible(pPlayer,vrightsrc,vrightdest);

	//return true;
}

#ifdef _LINUX
// kludge for linux
using std::min;
using std::max;
#endif

bool CBotGlobals :: boundingBoxTouch2d ( 
										const Vector2D &a1, const Vector2D &a2,
										const Vector2D &bmins, const Vector2D &bmaxs )
{
	Vector2D amins = Vector2D(min(a1.x,a2.x),min(a1.y,a2.y));
	Vector2D amaxs = Vector2D(max(a1.x,a2.x),max(a1.y,a2.y));

	return (((bmins.x >= amins.x) && (bmins.y >= amins.y)) && ((bmins.x <= amaxs.x) && (bmins.y <= amaxs.y)) ||
		((bmaxs.x >= amins.x) && (bmaxs.y >= amins.y)) && ((bmaxs.x <= amaxs.x) && (bmaxs.y <= amaxs.y)));
}

bool CBotGlobals :: boundingBoxTouch3d (
										const Vector &a1, const Vector &a2,
										const Vector &bmins, const Vector &bmaxs )
{
	Vector amins = Vector(min(a1.x,a2.x),min(a1.y,a2.y),min(a1.z,a2.z));
	Vector amaxs = Vector(max(a1.x,a2.x),max(a1.y,a2.y),max(a1.z,a2.z));

	return (((bmins.x >= amins.x) && (bmins.y >= amins.y) && (bmins.z >= amins.z)) && ((bmins.x <= amaxs.x) && (bmins.y <= amaxs.y) && (bmins.z <= amaxs.z)) ||
		    ((bmaxs.x >= amins.x) && (bmaxs.y >= amins.y) && (bmaxs.z >= amins.z)) && ((bmaxs.x <= amaxs.x) && (bmaxs.y <= amaxs.y) && (bmaxs.z <= amaxs.z)));	
}
bool CBotGlobals :: onOppositeSides2d (
		const Vector2D &amins, const Vector2D &amaxs,
		const Vector2D &bmins, const Vector2D &bmaxs )
{
  float g = (amaxs.x - amins.x) * (bmins.y - amins.y) - 
	        (amaxs.y - amins.y) * (bmins.x - amins.x);

  float h = (amaxs.x - amins.x) * (bmaxs.y - amins.y) - 
	        (amaxs.y - amins.y) * (bmaxs.x - amins.x);

  return (g * h) <= 0.0f;
}

bool CBotGlobals :: onOppositeSides3d (
		const Vector &amins, const Vector &amaxs,
		const Vector &bmins, const Vector &bmaxs )
{
	amins.Cross(bmins);
	amaxs.Cross(bmaxs);

  float g = (amaxs.x - amins.x) * (bmins.y - amins.y) * (bmins.z - amins.z) - 
	        (amaxs.z - amins.z) * (amaxs.y - amins.y) * (bmins.x - amins.x);

  float h = (amaxs.x - amins.x) * (bmaxs.y - amins.y) * (bmaxs.z - amins.z) - 
	        (amaxs.z - amins.z) * (amaxs.y - amins.y) * (bmaxs.x - amins.x);

  return (g * h) <= 0.0f;
}

bool CBotGlobals :: linesTouching2d (
		const Vector2D &amins, const Vector2D &amaxs,
		const Vector2D &bmins, const Vector2D &bmaxs )
{
	return onOppositeSides2d(amins,amaxs,bmins,bmaxs) && boundingBoxTouch2d(amins,amaxs,bmins,bmaxs);
}

bool CBotGlobals :: linesTouching3d (
		const Vector &amins, const Vector &amaxs,
		const Vector &bmins, const Vector &bmaxs )
{
	return onOppositeSides3d(amins,amaxs,bmins,bmaxs) && boundingBoxTouch3d(amins,amaxs,bmins,bmaxs);
}

void CBotGlobals :: botMessage ( edict_t *pEntity, int iErr, const char *fmt, ... )
{
	va_list argptr; 
	static char string[1024];

	va_start (argptr, fmt);
	vsprintf (string, fmt, argptr); 
	va_end (argptr); 

	const char *bot_tag = BOT_TAG;
	int len = strlen(string);
	int taglen = strlen(BOT_TAG);
	// add tag -- push tag into string
	for ( int i = len + taglen; i >= taglen; i -- )
		string[i] = string[i-taglen];

	string[len+taglen+1] = 0;

	for ( int i = 0; i < taglen; i ++ )
		string[i] = bot_tag[i];

	strcat(string,"\n");

	if ( pEntity )
	{
		engine->ClientPrintf(pEntity,string);
	}
	else
	{
		if ( iErr )
		{
			Warning(string);
		}
		else
			Msg(string);
	}
}

bool CBotGlobals :: makeFolders ( char *szFile )
{
#ifndef __linux__
	char *delimiter = "\\";
#else
	char *delimiter = "/";
#endif

	char szFolderName[1024];
	int folderNameSize = 0;
	szFolderName[0] = 0;

	int iLen = strlen(szFile);

	int i = 0;

	while ( i < iLen )
	{
		while ( (i < iLen) && (szFile[i] != *delimiter) )
		{
			szFolderName[folderNameSize++]=szFile[i];
			i++;
		}

		if ( i == iLen )
			return true;

		i++;
		szFolderName[folderNameSize++]=*delimiter;//next
        szFolderName[folderNameSize] = 0;
        
#ifndef __linux__
        mkdir(szFolderName);
#else
		if ( mkdir(szFolderName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 ) {
			botMessage(NULL,0,"Trying to create folder '%s' successful",szFolderName);
		} else {
			if (dirExists(szFolderName)) {
				botMessage(NULL,0,"Folder '%s' already exists", szFolderName);
			} else {
				botMessage(NULL,0,"Trying to create folder '%s' failed",szFolderName);
			}
		}
#endif   
	}

	return true;
}

void CBotGlobals :: addDirectoryDelimiter ( char *szString )
{
#ifndef __linux__
	strcat(szString,"\\");
#else
	strcat(szString,"/");
#endif
}

bool CBotGlobals :: isBreakableOpen ( edict_t *pBreakable )
{
	return ((CClassInterface::getEffects(pBreakable) & EF_NODRAW) == EF_NODRAW);
}

Vector CBotGlobals:: getVelocity ( edict_t *pPlayer )
{
	CClient *pClient = CClients::get(pPlayer);

	if ( pClient )
		return pClient->getVelocity();

	return Vector(0,0,0);
}

FILE *CBotGlobals :: openFile ( char *szFile, char *szMode )
{
	FILE *fp = fopen(szFile,szMode);

	if ( fp == NULL )
	{
		botMessage ( NULL, 0, "file not found/opening error '%s' mode %s", szFile, szMode );

		makeFolders(szFile);

		// try again
		fp = fopen(szFile,szMode);

		if ( fp == NULL )
			botMessage ( NULL, 0, "failed to make folders for %s",szFile);
	}

	return fp;
}

void CBotGlobals :: buildFileName ( char *szOutput, const char *szFile, const char *szFolder, const char *szExtension, bool bModDependent )
{
	if (m_szRCBotFolder == NULL)
	{
#ifdef HOMEFOLDER
		char home[512];
		home[0] = 0;
#endif
		szOutput[0] = 0;

#if defined(HOMEFOLDER) && defined(__linux)
		char *lhome = getenv ("HOME");

		if (lhome != NULL) 
		{
			strncpy(home,lhome,511);
			home[511] = 0; 
		}
		else
			strcpy(home,".");
#endif

#if defined(HOMEFOLDER) && defined(WIN32)
		ExpandEnvironmentStringsA("%userprofile%", home, 511);
#endif

#ifdef HOMEFOLDER
		strcat(szOutput, home);
		addDirectoryDelimiter(szOutput);
#endif

		/*#ifndef HOMEFOLDER
			strcat(szOutput,"..");
			#endif HOMEFOLDER*/

		strcat(szOutput, BOT_FOLDER);
	}
	else
		strcpy(szOutput, m_szRCBotFolder);

	if ( (szOutput[strlen(szOutput)-1] != '\\') && (szOutput[strlen(szOutput)-1] != '/') )
		addDirectoryDelimiter(szOutput);

	if ( szFolder )
	{
		strcat(szOutput,szFolder);
		addDirectoryDelimiter(szOutput);
	}

	if ( bModDependent )
	{
		strcat(szOutput,CBotGlobals::modFolder());
		addDirectoryDelimiter(szOutput);
	}

	strcat(szOutput,szFile);

	if ( szExtension )
	{
		strcat(szOutput,".");
		strcat(szOutput,szExtension);
	}
}

QAngle CBotGlobals::playerAngles ( edict_t *pPlayer )
{
	IPlayerInfo *pPlayerInfo = playerinfomanager->GetPlayerInfo(pPlayer);
	CBotCmd lastCmd = pPlayerInfo->GetLastUserCommand();
	return lastCmd.viewangles;
}

QAngle CBotGlobals :: entityEyeAngles ( edict_t *pEntity )
{
	return playerinfomanager->GetPlayerInfo(pEntity)->GetAbsAngles();
	//CBaseEntity *pBaseEntity = CBaseEntity::Instance(pEntity);

	//return pBaseEntity->EyeAngles();
}

void CBotGlobals :: fixFloatAngle ( float *fAngle ) 
{ 
	if ( *fAngle > 180 ) 
	{
		*fAngle = *fAngle - 360;
	} 
	else if ( *fAngle < -180 )
	{
		*fAngle = *fAngle + 360;
	}
}

void CBotGlobals :: fixFloatDegrees360 ( float *pFloat )
{
	if ( *pFloat > 360 )
		*pFloat -= 360;
	else if ( *pFloat < 0 )
		*pFloat += 360;
}

float CBotGlobals :: yawAngleFromEdict (edict_t *pEntity,Vector vOrigin)
{
	/*
	float fAngle;
	QAngle qBotAngles = entityEyeAngles(pEntity);
	Vector v2;
	Vector v1 = (vOrigin - entityOrigin(pEntity));
	Vector t;

	v1 = v1 / v1.Length();

	AngleVectors(qBotAngles,&v2);

	fAngle = atan2((v1.x*v2.y) - (v1.y*v2.x), (v1.x*v2.x) + (v1.y * v2.y));

	fAngle = RAD2DEG(fAngle);

	return (float)fAngle;*/

	float fAngle;
	float fYaw;
	QAngle qBotAngles = playerAngles(pEntity);
	QAngle qAngles;
	Vector vAngles;
	Vector vPlayerOrigin;

	gameclients->ClientEarPosition(pEntity,&vPlayerOrigin);

	vAngles = vOrigin - vPlayerOrigin;

	VectorAngles(vAngles/vAngles.Length(),qAngles);

	fYaw = qAngles.y;
	CBotGlobals::fixFloatAngle(&fYaw);

	fAngle = qBotAngles.y - fYaw;

	CBotGlobals::fixFloatAngle(&fAngle);

	return fAngle;

}

void CBotGlobals::teleportPlayer ( edict_t *pPlayer, Vector v_dest )
{
	CClient *pClient = CClients::get(pPlayer);
	
	if ( pClient )
		pClient->teleportTo(v_dest);
}
/*

static void TeleportEntity( CBaseEntity *pSourceEntity, TeleportListEntry_t &entry, const Vector *newPosition, const QAngle *newAngles, const Vector *newVelocity )
{
	CBaseEntity *pTeleport = entry.pEntity;
	Vector prevOrigin = entry.prevAbsOrigin;
	QAngle prevAngles = entry.prevAbsAngles;

	int nSolidFlags = pTeleport->GetSolidFlags();
	pTeleport->AddSolidFlags( FSOLID_NOT_SOLID );

	// I'm teleporting myself
	if ( pSourceEntity == pTeleport )
	{
		if ( newAngles )
		{
			pTeleport->SetLocalAngles( *newAngles );
			if ( pTeleport->IsPlayer() )
			{
				CBasePlayer *pPlayer = (CBasePlayer *)pTeleport;
				pPlayer->SnapEyeAngles( *newAngles );
			}
		}

		if ( newVelocity )
		{
			pTeleport->SetAbsVelocity( *newVelocity );
			pTeleport->SetBaseVelocity( vec3_origin );
		}

		if ( newPosition )
		{
			pTeleport->AddEffects( EF_NOINTERP );
			UTIL_SetOrigin( pTeleport, *newPosition );
		}
	}
	else
	{
		// My parent is teleporting, just update my position & physics
		pTeleport->CalcAbsolutePosition();
	}
	IPhysicsObject *pPhys = pTeleport->VPhysicsGetObject();
	bool rotatePhysics = false;

	// handle physics objects / shadows
	if ( pPhys )
	{
		if ( newVelocity )
		{
			pPhys->SetVelocity( newVelocity, NULL );
		}
		const QAngle *rotAngles = &pTeleport->GetAbsAngles();
		// don't rotate physics on players or bbox entities
		if (pTeleport->IsPlayer() || pTeleport->GetSolid() == SOLID_BBOX )
		{
			rotAngles = &vec3_angle;
		}
		else
		{
			rotatePhysics = true;
		}

		pPhys->SetPosition( pTeleport->GetAbsOrigin(), *rotAngles, true );
	}

	g_pNotify->ReportTeleportEvent( pTeleport, prevOrigin, prevAngles, rotatePhysics );

	pTeleport->SetSolidFlags( nSolidFlags );
}
*/
