/*
 * THIS IS A CODE EXCERPT FROM CODEGURU.COM by KevinHall
 *
 * http://www.codeguru.com/forum/showthread.php?t=291294
 *
 * Last Accessed : 07 July 2009
 *
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
#ifndef __BOT_PROFILING_H__
#define __BOT_PROFILING_H__

class CBot;

class CProfileTimer
{
public:

	CProfileTimer(const char* szFunction);

	const char* getFunction() const
	{ 
		return m_szFunction; 
	}

    void Start();

    void Stop();

	void print(const double* high);

#ifndef __linux__
	__int64 getOverall() const
#else
	inline long long getOverall()
#endif
    {
        return m_overall;
    }
    
private:
#ifndef __linux__
    unsigned __int64  start_cycle;
    unsigned __int64  end_cycle;
    unsigned __int64  m_average;
    unsigned __int64  m_min;
    unsigned __int64  m_max;
    unsigned __int64  m_last;
    unsigned __int64  m_overall;
#else    
    unsigned long long  start_cycle;
    unsigned long long  end_cycle;
    unsigned long long  m_average;
    unsigned long long  m_min;
    unsigned long long  m_max;
    unsigned long long  m_last;
    unsigned long long  m_overall;
#endif
	
	const char* m_szFunction;

	// helps us to know if the timer has been used recently
	// is also used to reset the minimum value
	// and reset the maximum values after
	// if it is not "invoked" it is not displayed
	int m_iInvoked; 
};

enum
{
	BOTS_THINK_TIMER = 0,
	BOT_THINK_TIMER = 1,
	BOT_ROUTE_TIMER = 2,
	BOT_VISION_TIMER = 3,
	PROFILING_TIMERS = 4
};

class CProfileTimers
{
public:

	static void reset();

	static void updateAndDisplay();

	static CProfileTimer* getTimer(int id);
private:
	static CProfileTimer m_Timers[PROFILING_TIMERS];

	static float m_fNextUpdate;
};

#endif