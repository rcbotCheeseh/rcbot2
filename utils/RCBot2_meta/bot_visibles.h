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
#ifndef __BOT_VISIBLES_H__
#define __BOT_VISIBLES_H__

#include "bot.h"
#include "bot_globals.h"

#include <set>

class CVisibleFunc
{
public:
	virtual void execute ( edict_t *pEntity ) { return; }
};

class CTF2FindFlagFunc : public CVisibleFunc
{
public:
	CTF2FindFlagFunc ( CBot *pBot ) 
	{ 
		m_pFlag = NULL;
	}

	edict_t *getFlag ( Vector &vOrigin )
	{
		return m_pFlag;
	}

	void init ();

	void execute ( edict_t *pEntity ) override;
private:
	CBot *m_pBot;
	edict_t *m_pFlag;
};


class CFindEnemyFunc : public CVisibleFunc
{
public:
	CFindEnemyFunc ( CBot *pBot ) 
	{ 
		m_pBot = pBot; 
		m_fBestFactor = 0;
		m_pBest = NULL;
	}

	edict_t *getBestEnemy ()
	{
		return m_pBest;
	}

	float getFactor ( edict_t *pEntity );
	void setOldEnemy ( edict_t *pEntity );
	void init ();

	void execute ( edict_t *pEntity ) override;
private:
	CBot *m_pBot;
	float m_fBestFactor;
	edict_t *m_pBest;
};

class CBotVisibles
{	
public:
	CBotVisibles ( CBot *pBot );
	~CBotVisibles ();

	void reset ();
	void updateVisibles ();

	bool isVisible (const edict_t* pEdict);
	void setVisible ( edict_t *pEdict, bool bVisible );

	void eachVisible ( CVisibleFunc *pFunc );

	void checkVisible (edict_t* pEntity, int* iTicks, bool* bVisible, const int& iIndex, bool bCheckHead = false);

	static void debugString ( char *string );

	static const int DEFAULT_MAX_TICKS = 10; // max number of PVS checks fired every visible check

private:
	//static const int NUM_BYTES = 4; // 32 entities
	//static const int MAX_INDEX = NUM_BYTES*8;
	
	static byte m_bPvs[MAX_MAP_CLUSTERS/8];

	CBot *m_pBot;
	// current entity index we are checking
	int m_iCurrentIndex;
	// current player index we are checking -- updated more often
	int m_iCurPlayer;
	unsigned char *m_iIndicesVisible;//[NUM_BYTES];
	int m_iMaxSize;
	int m_iMaxIndex;

	std::set<edict_t*> m_VisibleSet;
};

#endif