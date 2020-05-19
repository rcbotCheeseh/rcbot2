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
#ifndef __BOT_GA_INDIVIDUAL_H__
#define __BOT_GA_INDIVIDUAL_H__

#include "bot_ga.h"

class CBotGAValues : public IIndividual
{
public:
	CBotGAValues();

	CBotGAValues( std::vector<float> values );

	void init (void);

	// crossover with other individual
	void crossOver ( IIndividual *other );

	// mutate some values
	void mutate ();

	// get new copy of this
	// sub classes return their class with own values
	virtual IIndividual *copy ();

	void setVector ( std::vector<float> values );

	float get ( int iIndex );

	void set ( int iIndex, float fVal );

	void clear ();

	inline void add ( float val ) { m_theValues.push_back(val); }

	void addRnd ();

	void setup ( int iValues )
	{
		for ( int i = 0; i < iValues; i ++ )
			addRnd();
	}

	void freeMemory ();

protected:
	std::vector<float> m_theValues;
};

class CBotStuckValues : public CBotGAValues
{
public:
	CBotStuckValues()
	{
		init();
		setup(5);
	}

	float getJumpTime ()
	{
		return m_theValues[0];
	}

	float getFailTime ()
	{
		return m_theValues[1];
	}

	virtual IIndividual *copy ()
	{
		CBotStuckValues *p = new CBotStuckValues();
		p->setVector(m_theValues);
		p->setFitness(getFitness());
		return p;
	}

	void getStuckWeights ( std::vector<ga_nn_value> *weights )
	{
		weights->clear();
		weights->push_back(m_theValues[2]);
		weights->push_back(m_theValues[3]);
		weights->push_back(m_theValues[4]);
	}

};

#endif
