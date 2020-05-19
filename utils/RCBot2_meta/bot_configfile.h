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
#ifndef __BOT_CONFIGFILE__
#define __BOT_CONFIGFILE__

#include "bot_utility.h"

#include <vector>

typedef enum
{
	BOT_ATT_UTIL = 0,
	BOT_NORM_UTIL,
	UTIL_TYPE_MAX
}eTF2UtilType;

typedef struct
{
	float min;
	float max;
}bot_util_t;

class CRCBotTF2UtilFile
{
public:
	static void addUtilPerturbation (eBotAction iAction, eTF2UtilType iUtil, float fUtility[9][2]);

	static void init ();

	static void loadConfig ();
	// 2 Teams / 2 Types Attack/Defend / 
	static bot_util_t m_fUtils[UTIL_TYPE_MAX][BOT_UTIL_MAX][9];
};


class CBotConfigFile
{
public:
	static void load ();

	static void reset ()
	{
		m_iCmd = 0;
		m_fNextCommandTime = 0.0f;
	}

	static void doNextCommand ();
	static void executeCommands ();

private:
	static std::vector<char *> m_Commands;
	static unsigned int m_iCmd; // current command (time delayed)
	static float m_fNextCommandTime;
};

#endif