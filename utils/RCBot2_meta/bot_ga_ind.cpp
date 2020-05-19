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
#include "bot_mtrand.h"
#include "bot_ga.h"
#include "bot_ga_ind.h"
#include "bot_mtrand.h"

CBotGAValues :: CBotGAValues()
{
	init();
}

void CBotGAValues :: init (void)
{
	clear();	
	setFitness(0);
}

CBotGAValues :: CBotGAValues( std::vector<float> values )
{
	clear();	
	setFitness(0);

	setVector(values);
}

void CBotGAValues :: clear ()
{
	m_theValues.clear();
}

// crossover with other individual
void CBotGAValues :: crossOver ( IIndividual *other )
{
	unsigned int iPoint = randomInt(0,m_theValues.size());
	float fTemp;

	CBotGAValues *vother = (CBotGAValues*)other;

	unsigned int i;

	for ( i = 0; i < iPoint; i ++ )
	{
		fTemp = get(i);
		set(i,vother->get(i));
		vother->set(i,fTemp);
	}

	for (  i = iPoint; i < m_theValues.size(); i ++ )
	{
		fTemp = vother->get(i);
		vother->set(i,get(i));
		set(i,fTemp);
	}
}

// mutate some values
void CBotGAValues :: mutate ()
{
	for ( unsigned int i = 0; i < m_theValues.size(); i ++ )
	{
		if ( randomFloat(0,1) < CGA::g_fMutateRate )
		{
			float fCurrentVal = get(i);

			set(i,fCurrentVal + ((fCurrentVal * (-1+randomFloat(0,2))) * CGA::g_fMaxPerturbation));
		}
	}
}


float CBotGAValues :: get ( int iIndex )
{
	return m_theValues[iIndex];
}

void CBotGAValues :: set ( int iIndex, float fVal )
{
	m_theValues[iIndex] = fVal;
}

void CBotGAValues :: addRnd()
{
	m_theValues.push_back(randomFloat(0,1));
}

// get new copy of this
// sub classes return their class with own values
IIndividual *CBotGAValues :: copy ()
{
	IIndividual *individual = new CBotGAValues (m_theValues);

	individual->setFitness(getFitness());

	return individual;
}

void CBotGAValues :: setVector ( std::vector<float> values )
{
	for ( unsigned int i = 0; i < values.size(); i ++ )
		m_theValues.push_back(values[i]);
}

void CBotGAValues :: freeMemory ()
{
	m_theValues.clear();
}
