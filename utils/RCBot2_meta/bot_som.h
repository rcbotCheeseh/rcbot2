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
#ifndef __RCBOT_SOM_H__
#define __RCBOT_SOM_H__

#include <cmath>

#include <vector>

class CSomNeuron
{
public:
	~CSomNeuron ();

	CSomNeuron ();

	CSomNeuron ( unsigned short id, int iInp, int iX, int iY );

	inline float getX()
	{
		return m_iX;
	}

	inline float getY()	
	{
		return m_iY;
	}

	void update ( std::vector<float> *inputs, float inf );

	float distance ( std::vector <float> *inputs );
	
	std::vector <float> *weights ();

	void displayWeights ();
	
	float neighbourDistance ( CSomNeuron *other );

	inline void setClassID ( unsigned short iId ) { m_iId = iId; }

	inline unsigned short getClassID () { return m_iId; }

private:
	std::vector<float> fWeights;
	float m_iX;
	float m_iY;
	unsigned short m_iId;
};

class CSom
{
public:
	static float m_fLearnRate;

	CSom ( int iW, int iH, int iIn );

	~CSom ();

	CSomNeuron *getBMU ( std::vector <float> *inputs );

	void updateAround ( std::vector<float> *inputs, CSomNeuron *bmu );

	CSomNeuron *input ( std::vector < std::vector <float> > *inputs );

	CSomNeuron *inputOne ( std::vector < float > *inputs );

	void input ( std::vector < std::vector <float> > *inputs, int epochs );

	void display ();

	inline unsigned int epochs ()
	{
		return m_iEpochs;
	}

private:
	std::vector<CSomNeuron*> m_Neurons;
	int m_iH;
	int m_iW;
	float m_fNSize;
	unsigned int m_iEpochs;
};
#endif