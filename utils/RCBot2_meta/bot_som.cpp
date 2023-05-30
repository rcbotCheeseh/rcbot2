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

#ifdef __linux__
#include <cstdio>
#include <cstdlib>
#endif
#include "bot_som.h"
#include "bot_mtrand.h"

float CSom :: m_fLearnRate = 1.0f;

CSom :: CSom ( int iW, int iH, int iIn )
{       
	unsigned short id = 0;

	m_iW = iW;
	m_iH = iH;

	// neighbourhood size
	m_fNSize = static_cast<float>(static_cast<int>(static_cast<float>(iW) / 2));

	for ( int i = 0; i < iH; i ++ )
	{
		for ( int j = 0; j < iW; j ++ )
			m_Neurons.emplace_back(new CSomNeuron(id++,iIn,j,i));
	}

	m_iEpochs = 0;
}

CSom :: ~CSom ()
{
	m_Neurons.clear();
}

CSomNeuron *CSom :: getBMU (const std::vector<float>* inputs) const
{       
	CSomNeuron *winner = nullptr;
	float bestdistance = 0;

	for ( unsigned int i = 0; i < m_Neurons.size(); i ++ )
	{
		const float dist = m_Neurons[i]->distance(inputs);

		if ( !winner || dist < bestdistance )
		{
			winner = m_Neurons[i];
			bestdistance = dist;
		}
	}

	return winner;
}

void CSom :: updateAround (const std::vector<float>* inputs, CSomNeuron* bmu) const
{
	float dist;
	const float nsiz = m_fNSize*m_fNSize;

	for ( unsigned int i = 0; i < m_Neurons.size(); i ++ )
	{
		CSomNeuron *current = m_Neurons[i];

		if ( (dist = bmu->neighbourDistance(current)) <= nsiz )
		{
			bmu->update(inputs,exp(-dist / (2*nsiz)));    
		}           
	}
}

CSomNeuron *CSom :: inputOne ( std::vector <float> *inputs )
{
	CSomNeuron *winner = getBMU(inputs);

	updateAround(inputs,winner);

	m_fNSize *= 0.75f;
	m_fLearnRate *= 0.75f;
	m_iEpochs++;

	return winner;
}

void CSom::input(std::vector<std::vector<float>>* inputs, int epochs)// Experimental [APG]RoboCop[CL]
{
	for (int i = 0; i < epochs; i++)
	{
		for (unsigned int j = 0; j < inputs->size(); j++)
		{
			inputOne(&(*inputs)[j]);
		}
	}
}

CSomNeuron *CSom :: input ( std::vector < std::vector <float> > *inputs )
{
	return inputOne(&(*inputs)[randomInt(0,static_cast<int>(inputs->size())-1)]);
}

void CSom :: display () const
{
	//printf("\nDisplaying...\n");

	for ( unsigned int i = 0; i < m_Neurons.size(); i ++ )
	{
		//printf("%d -- ",i);
		m_Neurons[i]->displayWeights();
		//printf("\n");
	}
}

void CSomNeuron :: update (const std::vector<float>* inputs, float inf)
{
	for ( unsigned int i = 0; i < inputs->size(); i ++ )
	{
		const float change = (*inputs)[i] - fWeights[i];

		fWeights[i] += change*CSom::m_fLearnRate*inf;
	}
}

CSomNeuron :: ~CSomNeuron ()
{
	fWeights.clear();
}

CSomNeuron :: CSomNeuron ()
{
	m_iX = 0;
	m_iY = 0;
	m_iId = 0;
	return;
}

CSomNeuron :: CSomNeuron ( unsigned short iId, int iInp, int iX, int iY )
{				
	m_iX = static_cast<float>(iX);
	m_iY = static_cast<float>(iY);
	m_iId = iId;
	
	for ( int i = 0; i < iInp; i ++ )
		fWeights.emplace_back(randomFloat(0,1));
}

float CSomNeuron :: distance (const std::vector<float>* inputs) const
{
	float dist = 0;

	for ( unsigned int i = 0; i < inputs->size(); i ++ )
	{
		const float comp = fWeights[i] - (*inputs)[i];
		
		dist += comp*comp;
	}
	
	return dist;
}

std::vector <float> *CSomNeuron :: weights ()
{
	return &fWeights;
}

void CSomNeuron :: displayWeights () const
{
	for ( unsigned int i = 0; i < fWeights.size(); i ++ )
	{
		printf("%0.4f,",fWeights[i]);
	}
}

float CSomNeuron :: neighbourDistance ( CSomNeuron *other ) const
{
	const float distx = getX()-other->getX();
	const float disty = getY()-other->getY();
	
	return distx*distx+disty*disty;
}
