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
#ifndef __BOT_GA_H__
#define __BOT_GA_H__

#include "bot_ga_nn_const.h"
#include <vector>

class IIndividual
{
public:
	// get fitness for this individual
	inline ga_nn_value getFitness () { return m_fFitness; }
	inline void setFitness ( float fVal ) { m_fFitness = fVal; }

	// crossover with other individual
	virtual void crossOver ( IIndividual *other ) = 0;

	// mutate some values
	virtual void mutate () = 0;

	// get new copy of this
	// sub classes return their class with own values
	virtual IIndividual *copy () = 0;
private:
	ga_nn_value m_fFitness;
};

class CGA;

class CPopulation
{
public:

	void freeMemory ();

	inline void setGA ( CGA *ga ) { m_ga = ga; }
	// size of population
	inline unsigned int size () { return m_theIndividuals.size(); };

	// get from population index
	IIndividual *get ( int iIndex );

	// add individual to population
	void add ( IIndividual *individual );

	void clear ();

	ga_nn_value totalFitness ();

	ga_nn_value bestFitness ();

	ga_nn_value averageFitness ();

	// get back individual
	IIndividual *pick ();

private:
	std::vector<IIndividual*> m_theIndividuals;
	CGA *m_ga;
};

// selection function interface
class ISelection
{
public:
	virtual IIndividual *select ( CPopulation *population ) = 0;
};

class CRouletteSelection : public ISelection
{
	IIndividual *select ( CPopulation *population );
};

class CGA
{
public:

	CGA (int iMaxPopSize=0)
	{
		init(iMaxPopSize);
	}

	void init (int iMaxPopSize=0)
	{
		m_theSelectFunction = new CRouletteSelection();

		m_thePopulation.setGA(this);
		m_theNewPopulation.setGA(this);

		m_iNumGenerations = 0;
		m_fPrevAvgFitness = 0;

		m_iMaxPopSize = iMaxPopSize;
		
		if ( m_iMaxPopSize == 0 )
			m_iMaxPopSize = g_iDefaultMaxPopSize;
	}

	// give GA a custom selection function
	CGA ( ISelection *selectFunction );

	void freeLocalMemory();
	void freeGlobalMemory();

	// make new generation
	void epoch ();

	void addToPopulation ( IIndividual *individual );

	// can get an individual off new population
	bool canPick ();

	IIndividual *pick ();

	unsigned int m_iMaxPopSize;
	static const int g_iDefaultMaxPopSize;
	static const float g_fCrossOverRate;
	static const float g_fMutateRate;
	static const float g_fMaxPerturbation;

private:
	
	CPopulation m_thePopulation;
	CPopulation m_theNewPopulation;

	unsigned int m_iNumGenerations;
	float m_fPrevAvgFitness;

	ISelection *m_theSelectFunction;
};
#endif
