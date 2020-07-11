/*
 *    part of https://rcbot2.svn.sourceforge.net/svnroot/rcbot2
 *
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

#ifndef __BOT_UTILITY_H__
#define __BOT_UTILITY_H__

#include <vector>

class CBot;
class CBotWeapon;

typedef enum
{
 BOT_UTIL_BUILDSENTRY = 0,
 BOT_UTIL_BUILDDISP,
 BOT_UTIL_BUILDTELENT,
 BOT_UTIL_BUILDTELEXT,
 BOT_UTIL_UPGSENTRY,
 BOT_UTIL_UPGDISP,
 BOT_UTIL_UPGTELENT,
 BOT_UTIL_UPGTELEXT,
 BOT_UTIL_UPGTMSENTRY,
 BOT_UTIL_UPGTMDISP,
 BOT_UTIL_UPGTMTELENT,
 BOT_UTIL_UPGTMTELEXT,
 BOT_UTIL_GOTODISP,
 BOT_UTIL_GOTORESUPPLY_FOR_HEALTH,
 BOT_UTIL_GETAMMOKIT,
 BOT_UTIL_GETAMMOTMDISP,
 BOT_UTIL_GETAMMODISP,
 BOT_UTIL_GETFLAG,
 BOT_UTIL_GETHEALTHKIT,
 BOT_UTIL_GETFLAG_LASTKNOWN,
 BOT_UTIL_SNIPE,
 BOT_UTIL_ROAM,
 BOT_UTIL_CAPTURE_FLAG,
 BOT_UTIL_GOTORESUPPLY_FOR_AMMO,
 BOT_UTIL_FIND_NEAREST_HEALTH,
 BOT_UTIL_FIND_NEAREST_AMMO,
 BOT_UTIL_ATTACK_POINT,
 BOT_UTIL_DEFEND_POINT,
 BOT_UTIL_DEFEND_FLAG,
 BOT_UTIL_ENGI_LOOK_AFTER_SENTRY,
 BOT_UTIL_DEFEND_FLAG_LASTKNOWN,
 BOT_UTIL_PUSH_PAYLOAD_BOMB,
 BOT_UTIL_DEFEND_PAYLOAD_BOMB,
 BOT_UTIL_MEDIC_HEAL,		// heal someone
 BOT_UTIL_MEDIC_HEAL_LAST,  // find the player I was healing last
 BOT_UTIL_MEDIC_FINDPLAYER, // find player who called medic
 BOT_UTIL_SAP_NEAREST_SENTRY,
 BOT_UTIL_SAP_ENEMY_SENTRY,
 BOT_UTIL_SAP_LASTENEMY_SENTRY,
 BOT_UTIL_SAP_DISP,
 BOT_UTIL_BACKSTAB,
 BOT_UTIL_REMOVE_SENTRY_SAPPER,
 BOT_UTIL_REMOVE_DISP_SAPPER,
 BOT_UTIL_REMOVE_TMSENTRY_SAPPER,
 BOT_UTIL_REMOVE_TMDISP_SAPPER,
 BOT_UTIL_DEMO_STICKYTRAP_LASTENEMY,
 BOT_UTIL_DEMO_STICKYTRAP_POINT,
 BOT_UTIL_DEMO_STICKYTRAP_FLAG,
 BOT_UTIL_DEMO_STICKYTRAP_FLAG_LASTKNOWN,
 BOT_UTIL_DEMO_STICKYTRAP_PL,
 BOT_UTIL_REMOVE_TMTELE_SAPPER,
 BOT_UTIL_SAP_NEAREST_TELE,
 BOT_UTIL_SAP_ENEMY_TELE,
 BOT_UTIL_SAP_LASTENEMY_TELE,
 BOT_UTIL_GOTO_NEST,	// camp , regain health or allies
 BOT_UTIL_MESSAROUND,
 BOT_UTIL_ENGI_MOVE_SENTRY,
 BOT_UTIL_ENGI_MOVE_DISP,
 BOT_UTIL_ENGI_MOVE_ENTRANCE,
 BOT_UTIL_ENGI_MOVE_EXIT,
 BOT_UTIL_ENGI_DESTROY_SENTRY,
 BOT_UTIL_ENGI_DESTROY_DISP,
 BOT_UTIL_ENGI_DESTROY_ENTRANCE,
 BOT_UTIL_ENGI_DESTROY_EXIT,
 BOT_UTIL_HIDE_FROM_ENEMY,
 BOT_UTIL_MEDIC_FINDPLAYER_AT_SPAWN, // find team players around spawn area
 BOT_UTIL_HL2DM_GRAVIGUN_PICKUP,
 BOT_UTIL_HL2DM_FIND_ARMOR,
 BOT_UTIL_FIND_LAST_ENEMY,
 BOT_UTIL_HL2DM_USE_CHARGER,
 BOT_UTIL_HL2DM_USE_HEALTH_CHARGER,
 BOT_UTIL_THROW_GRENADE,
 BOT_UTIL_PICKUP_WEAPON,
 BOT_UTIL_ATTACK_NEAREST_POINT,
 BOT_UTIL_DEFEND_NEAREST_POINT,
 BOT_UTIL_DEFEND_BOMB,
 BOT_UTIL_DEFUSE_BOMB,
 BOT_UTIL_PLANT_BOMB,
 BOT_UTIL_PLANT_NEAREST_BOMB,
 BOT_UTIL_DEFUSE_NEAREST_BOMB,
 BOT_UTIL_DEFEND_NEAREST_BOMB,
 BOT_UTIL_PICKUP_BOMB,
 BOT_UTIL_PIPE_NEAREST_SENTRY,
 BOT_UTIL_PIPE_LAST_ENEMY,
 BOT_UTIL_PIPE_LAST_ENEMY_SENTRY,
 BOT_UTIL_DOD_PICKUP_OBJ,
 BOT_UTIL_HL2DM_USE_CRATE,
 BOT_UTIL_PLACE_BUILDING,
 BOT_UTIL_SPYCHECK_AIR,
 BOT_UTIL_FIND_MEDIC_FOR_HEALTH,
 BOT_UTIL_FIND_SQUAD_LEADER,
 BOT_UTIL_FOLLOW_SQUAD_LEADER,
 BOT_UTIL_ATTACK_SENTRY,
 BOT_UTIL_SPAM_LAST_ENEMY,
 BOT_UTIL_SPAM_NEAREST_SENTRY,
 BOT_UTIL_SPAM_LAST_ENEMY_SENTRY,
 BOT_UTIL_SAP_NEAREST_DISP,
 BOT_UTIL_SAP_ENEMY_DISP,
 BOT_UTIL_SAP_LASTENEMY_DISP,
 BOT_UTIL_BUILDTELENT_SPAWN,
 BOT_UTIL_INVESTIGATE_POINT,
 BOT_UTIL_COVER_POINT,
 BOT_UTIL_SNIPE_POINT,
 BOT_UTIL_MOVEUP_MG,
 BOT_UTIL_SNIPE_CROSSBOW,
 BOT_UTIL_MAX
}eBotAction;

extern const char *g_szUtils[BOT_UTIL_MAX+1];

class CBotUtility
{
public:
	CBotUtility ( CBot *pBot, eBotAction id, bool bCanDo, float fUtil, CBotWeapon *pWeapon = NULL, int iData = 0, Vector vec = Vector(0,0,0) );

	inline float getUtility () { return m_fUtility; }

	inline eBotAction getId () { return m_id; }

	inline bool canDo () { return m_bCanDo; }

	inline CBotWeapon *getWeaponChoice () { return m_pWeapon; }

	inline int getIntData () { return m_iData; }

	inline Vector getVectorData () { return m_vVector; }

private:
	int m_iData;
	float m_fUtility;
	bool m_bCanDo;
	eBotAction m_id;
	CBot *m_pBot;
	CBotWeapon *m_pWeapon;
	Vector m_vVector;
};


typedef struct util_node_s
{
  CBotUtility *util;
  struct util_node_s *next;
}util_node_t;


typedef struct
{
	util_node_t *head;
}util_list;

class CBotUtilities
{
public:

	CBotUtilities ()
	{
		m_pBest.head = NULL;
	}

	void freeMemory ();

	inline void addUtility ( CBotUtility p ) { if ( p.canDo() ) { m_Utilities.push_back(p); } }

	void execute ();

	CBotUtility *nextBest ();

private:
	std::vector<CBotUtility> m_Utilities;

	util_list m_pBest;
};
#define ADD_UTILITY_WEAPON_DATA_VECTOR(utilname,condition,utility,weapon,data,vector) if ( m_fUtilTimes[utilname] < engine->Time()) { if ( condition ) { utils.addUtility(CBotUtility(this,utilname,true,utility,weapon,data,vector)); } }
#define ADD_UTILITY_DATA_VECTOR(utilname,condition,utility,data,vector) if ( m_fUtilTimes[utilname] < engine->Time()) { if ( condition ) { utils.addUtility(CBotUtility(this,utilname,true,utility,NULL,data,vector)); } }
#define ADD_UTILITY_WEAPON_DATA(utilname,condition,utility,weapon,data) if ( m_fUtilTimes[utilname] < engine->Time()) { if ( condition ) { utils.addUtility(CBotUtility(this,utilname,true,utility,weapon,data)); } }
#define ADD_UTILITY_DATA(utilname,condition,utility,data) if ( m_fUtilTimes[utilname] < engine->Time()) { if ( condition ) { utils.addUtility(CBotUtility(this,utilname,true,utility,NULL,data)); } }
#define ADD_UTILITY_WEAPON(utilname,condition,utility,weapon) if ( m_fUtilTimes[utilname] < engine->Time()) { if ( condition ) { utils.addUtility(CBotUtility(this,utilname,true,utility,weapon)); } }
#define ADD_UTILITY(utilname,condition,utility) if ( m_fUtilTimes[utilname] < engine->Time()) { if ( condition ) { utils.addUtility(CBotUtility(this,utilname,true,utility)); } }


#endif