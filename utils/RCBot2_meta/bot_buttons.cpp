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
#include "bot.h"
#include "bot_buttons.h"
#include "in_buttons.h"

void CBotButtons :: attack (float fFor, float fFrom)
{	
	holdButton(IN_ATTACK,fFrom,fFor,0.1);
}

void CBotButtons :: jump (float fFor, float fFrom)
{
	holdButton(IN_JUMP,fFrom,fFor,0.25);
}

void CBotButtons :: duck (float fFor, float fFrom)
{
	holdButton(IN_DUCK,fFrom,fFor);
}

void CBotButton :: hold ( float fFrom, float fFor, float fLetGoTime )
{
	fFrom += engine->Time();
	m_fTimeStart = fFrom;
	m_fTimeEnd = fFrom + fFor;
	m_fLetGoTime = m_fTimeEnd+fLetGoTime;
}

CBotButtons :: CBotButtons()
{
	add(new CBotButton(IN_ATTACK));
	add(new CBotButton(IN_ATTACK2));
	add(new CBotButton(IN_DUCK));
	add(new CBotButton(IN_JUMP));
	add(new CBotButton(IN_RELOAD));
	add(new CBotButton(IN_SPEED)); // for sprint
	add(new CBotButton(IN_FORWARD)); // for ladders
	add(new CBotButton(IN_USE)); // for chargers
	add(new CBotButton(IN_ALT1)); // for proning
	add(new CBotButton(IN_RUN)); // ????

	m_bLetGoAll = false;
}

void CBotButtons :: holdButton ( int iButtonId, float fFrom, float fFor, float fLetGoTime )
{
	for (unsigned int i = 0; i < m_theButtons.size(); i ++ )
	{			
		if ( m_theButtons[i]->getID() == iButtonId )
		{
			m_theButtons[i]->hold(fFrom,fFor,fLetGoTime);
			return;
		}
	}
}

void CBotButtons :: letGo (int iButtonId)
{
	for (unsigned int i = 0; i < m_theButtons.size(); i ++ )
	{			
		if ( m_theButtons[i]->getID() == iButtonId )
		{
			m_theButtons[i]->letGo();
			return;
		}
	}
}

int CBotButtons :: getBitMask ()
{
	if ( m_bLetGoAll )
		return 0;
	else
	{

        int iBitMask = 0;

        float fTime = engine->Time();

        for (unsigned int i = 0; i < m_theButtons.size(); i ++ )
        {
            if ( m_theButtons[i]->held(fTime) )
            {
                m_theButtons[i]->unTap();
                iBitMask |= m_theButtons[i]->getID();
            }
        }

        return iBitMask;

    }
}

bool CBotButtons :: canPressButton ( int iButtonId )
{
	for (unsigned int i = 0; i < m_theButtons.size(); i ++ )
	{			
		if ( m_theButtons[i]->getID() == iButtonId )
			return m_theButtons[i]->canPress(engine->Time());
	}
	return false;		
}

void CBotButtons :: add ( CBotButton *theButton )
{
	m_theButtons.push_back(theButton);
}

bool CBotButtons :: holdingButton ( int iButtonId )
{
	for ( unsigned int i = 0; i < m_theButtons.size(); i ++ )
	{
		if ( m_theButtons[i]->getID() == iButtonId )
			return m_theButtons[i]->held(engine->Time());
	}

	return false;
}

void CBotButtons :: tap ( int iButtonId )
{
	for ( unsigned int i = 0; i < m_theButtons.size(); i ++ )
	{
		if ( m_theButtons[i]->getID() == iButtonId )
		{
			 m_theButtons[i]->tap();

			return;
		}
	}
}
