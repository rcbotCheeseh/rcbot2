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
#include "engine_wrappers.h"

#include "bot.h"
#include "bot_utility.h"
#include "bot_getprop.h"
#include "bot_configfile.h"
#include "bot_mods.h"
#include "bot_fortress.h"

const char *g_szUtils[BOT_UTIL_MAX+1] =
{
	"BOT_UTIL_BUILDSENTRY",
	"BOT_UTIL_BUILDDISP",
	"BOT_UTIL_BUILDTELENT",
	"BOT_UTIL_BUILDTELEXT",
	"BOT_UTIL_UPGSENTRY",
	"BOT_UTIL_UPGDISP",
	"BOT_UTIL_UPGTELENT",
	"BOT_UTIL_UPGTELEXT",
	"BOT_UTIL_UPGTMSENTRY",
	"BOT_UTIL_UPGTMDISP",
	"BOT_UTIL_UPGTMTELENT",
	"BOT_UTIL_UPGTMTELEXT",
	"BOT_UTIL_GOTODISP",
	"BOT_UTIL_GOTORESUPPLY_FOR_HEALTH",
	"BOT_UTIL_GETAMMOKIT",
	"BOT_UTIL_GETAMMOTMDISP",
	"BOT_UTIL_GETAMMODISP",
	"BOT_UTIL_GETFLAG",
	"BOT_UTIL_GETHEALTHKIT",
	"BOT_UTIL_GETFLAG_LASTKNOWN",
	"BOT_UTIL_SNIPE",
	"BOT_UTIL_ROAM",
	"BOT_UTIL_CAPTURE_FLAG",
	"BOT_UTIL_GOTORESUPPLY_FOR_AMMO",
	"BOT_UTIL_FIND_NEAREST_HEALTH",
	"BOT_UTIL_FIND_NEAREST_AMMO",
	"BOT_UTIL_ATTACK_POINT",
	"BOT_UTIL_DEFEND_POINT",
	"BOT_UTIL_DEFEND_FLAG",
    "BOT_UTIL_ENGI_LOOK_AFTER_SENTRY",
    "BOT_UTIL_DEFEND_FLAG_LASTKNOWN",
    "BOT_UTIL_PUSH_PAYLOAD_BOMB",
    "BOT_UTIL_DEFEND_PAYLOAD_BOMB",
	"BOT_UTIL_MEDIC_HEAL",
	"BOT_UTIL_MEDIC_HEAL_LAST",
	"BOT_UTIL_MEDIC_FINDPLAYER",
	"BOT_UTIL_SAP_NEAREST_SENTRY",
    "BOT_UTIL_SAP_ENEMY_SENTRY",
    "BOT_UTIL_SAP_LASTENEMY_SENTRY",
	"BOT_UTIL_SAP_DISP",
	"BOT_UTIL_BACKSTAB",
	"BOT_UTIL_REMOVE_SENTRY_SAPPER",
	"BOT_UTIL_REMOVE_DISP_SAPPER",
	"BOT_UTIL_REMOVE_TMSENTRY_SAPPER",
	"BOT_UTIL_REMOVE_TMDISP_SAPPER",
	"BOT_UTIL_DEMO_STICKYTRAP_LASTENEMY",
	"BOT_UTIL_DEMO_STICKYTRAP_POINT",
	"BOT_UTIL_DEMO_STICKYTRAP_FLAG",
	"BOT_UTIL_DEMO_STICKYTRAP_FLAG_LASTKNOWN",
	"BOT_UTIL_DEMO_STICKYTRAP_PL",
	"BOT_UTIL_REMOVE_TMTELE_SAPPER",
    "BOT_UTIL_SAP_NEAREST_TELE",
    "BOT_UTIL_SAP_ENEMY_TELE",
    "BOT_UTIL_SAP_LASTENEMY_TELE",
	"BOT_UTIL_GOTO_NEST",
	"BOT_UTIL_MESSAROUND",
	 "BOT_UTIL_ENGI_MOVE_SENTRY",
	 "BOT_UTIL_ENGI_MOVE_DISP",
	 "BOT_UTIL_ENGI_MOVE_ENTRANCE",
	 "BOT_UTIL_ENGI_MOVE_EXIT",
 "BOT_UTIL_ENGI_DESTROY_SENTRY",
 "BOT_UTIL_ENGI_DESTROY_DISP",
 "BOT_UTIL_ENGI_DESTROY_ENTRANCE",
 "BOT_UTIL_ENGI_DESTROY_EXIT",
	 "BOT_UTIL_HIDE_FROM_ENEMY",
	 "BOT_UTIL_MEDIC_FINDPLAYER_AT_SPAWN",
	 "BOT_UTIL_HL2DM_GRAVIGUN_PICKUP",
	 "BOT_UTIL_HL2DM_FIND_ARMOR",
	 "BOT_UTIL_FIND_LAST_ENEMY",
	 "BOT_UTIL_HL2DM_USE_CHARGER",
	 "BOT_UTIL_HL2DM_USE_HEALTH_CHARGER",
	 "BOT_UTIL_THROW_GRENADE",
	 "BOT_UTIL_PICKUP_WEAPON",
	 "BOT_UTIL_ATTACK_NEAREST_POINT",
	 "BOT_UTIL_DEFEND_NEAREST_POINT",
	 "BOT_UTIL_DEFEND_BOMB",
	 "BOT_UTIL_DEFUSE_BOMB",
	 "BOT_UTIL_PLANT_BOMB",
	 "BOT_UTIL_PLANT_NEAREST_BOMB",
	 "BOT_UTIL_DEFUSE_NEAREST_BOMB",
	 "BOT_UTIL_DEFEND_NEAREST_BOMB",
	 "BOT_UTIL_PICKUP_BOMB",
 "BOT_UTIL_PIPE_NEAREST_SENTRY",
 "BOT_UTIL_PIPE_LAST_ENEMY",
 "BOT_UTIL_PIPE_LAST_ENEMY_SENTRY",
 "BOT_UTIL_DOD_PICKUP_OBJ",
 "BOT_UTIL_HL2DM_USE_CRATE",
 "BOT_UTIL_PLACE_BUILDING",
 "BOT_UTIL_SPYCHECK_AIR",
 "BOT_UTIL_FIND_MEDIC_FOR_HEALTH",
 "BOT_UTIL_FIND_SQUAD_LEADER",
 "BOT_UTIL_FOLLOW_SQUAD_LEADER",
 "BOT_UTIL_ATTACK_SENTRY",
 "BOT_UTIL_SPAM_LAST_ENEMY",
 "BOT_UTIL_SPAM_NEAREST_SENTRY",
 "BOT_UTIL_SPAM_LAST_ENEMY_SENTRY",
 "BOT_UTIL_SAP_NEAREST_DISP",
 "BOT_UTIL_SAP_ENEMY_DISP",
 "BOT_UTIL_SAP_LASTENEMY_DISP",
 "BOT_UTIL_BUILDTELENT_SPAWN",
 "BOT_UTIL_INVESTIGATE_POINT",
 "BOT_UTIL_COVER_POINT",
 "BOT_UTIL_SNIPE_POINT",
 "BOT_UTIL_MOVEUP_MG",
 "BOT_UTIL_SNIPE_CROSSBOW",
"BOT_UTIL_MAX"
};

CBotUtility :: CBotUtility ( CBot *pBot, eBotAction id, bool bCanDo, float fUtil, CBotWeapon *pWeapon, int iData, Vector vec )
{
	m_iData = iData;
	m_fUtility = fUtil;
	m_id = id;
	m_bCanDo = bCanDo;
	m_pBot = pBot;
	m_pWeapon = pWeapon;
	m_vVector = vec;

	if ( m_pBot && m_pBot->isTF2() )
	{
		int iClass = CClassInterface::getTF2Class(pBot->getEdict());

		if ( CTeamFortress2Mod::isAttackDefendMap() && (m_pBot->getTeam() == TF2_TEAM_BLUE) )
			m_fUtility += randomFloat(CRCBotTF2UtilFile::m_fUtils[BOT_ATT_UTIL][id][iClass].min,CRCBotTF2UtilFile::m_fUtils[BOT_ATT_UTIL][id][iClass].max);
		else
			m_fUtility += randomFloat(CRCBotTF2UtilFile::m_fUtils[BOT_NORM_UTIL][id][iClass].min,CRCBotTF2UtilFile::m_fUtils[BOT_NORM_UTIL][id][iClass].max);
	}
}

// Execute a list of possible actions and put them into order of available actions against utility
void CBotUtilities :: execute ()
{
	unsigned int i = 0;
	CBotUtility *pUtil;
	float fUtil;

	util_node_t *temp;
	util_node_t *pnew;
	util_node_t *prev;

	m_pBest.head = NULL;

	for ( i = 0; i < m_Utilities.size(); i ++ )
	{
		pUtil = &(m_Utilities[i]);
		fUtil = pUtil->getUtility();

		// if bot can do this action
		if ( pUtil->canDo() )
		{			
			// add to list
			temp = m_pBest.head;

			// put in correct order by making a linked list
			pnew = (util_node_t*)malloc(sizeof(util_node_t));

			if ( pnew != NULL )
			{
				pnew->util = pUtil;
				pnew->next = NULL;
				prev = NULL;

				if ( temp )
				{
					while ( temp )
					{
						// put into correct position
						if ( fUtil > temp->util->getUtility() )
						{
							if ( temp == m_pBest.head )
							{
								pnew->next = temp;
								m_pBest.head = pnew;
								break;
							}
							else
							{
								prev->next = pnew;
								pnew->next = temp;
								break;
							}
						}

						prev = temp;
						temp = temp->next;
					}

					if ( pnew->next == NULL )
						prev->next = pnew;
				}
				else
					m_pBest.head = pnew;
			}
		}
	}

	//return pBest;
}

void CBotUtilities :: freeMemory ()
{
	util_node_t *temp;
	m_Utilities.clear();

	// FREE LIST
	while ( (temp = m_pBest.head) != NULL )
	{
		temp = m_pBest.head;
		m_pBest.head = m_pBest.head->next;
		free(temp);		
	}
}

CBotUtility *CBotUtilities :: nextBest ()
{
	CBotUtility *pBest;
	util_node_t *temp;

	if ( m_pBest.head == NULL )
		return NULL;

	pBest = m_pBest.head->util;

	temp = m_pBest.head;

	m_pBest.head = m_pBest.head->next;

	free(temp);

	return pBest;
	
}