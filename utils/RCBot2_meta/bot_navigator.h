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
#ifndef __RCBOT_NAVIGATOR_H__
#define __RCBOT_NAVIGATOR_H__

#include <vector>
#include <queue>
#include <stack>

#include "bot.h"
#include "bot_waypoint.h"

#include "bot_belief.h"

class CNavMesh;
class CWaypointVisibilityTable;

#define MAX_BELIEF 200.0f

class INavigatorNode
{
public:
	inline Vector getOrigin () { return m_vOrigin; }
protected:
	Vector m_vOrigin;
};

class IBotNavigator
{
public:
	virtual void init () = 0;

	// returns true when working out route finishes, not if successful
	virtual bool workRoute ( Vector vFrom, Vector vTo, bool *bFail, bool bRestart = true, bool bNoInterruptions = false, int iGoalId = -1, int iConditions = 0, int iDangerId = -1 ) = 0;

	virtual void rollBackPosition () = 0;

	virtual void failMove () = 0;

	virtual float getNextYaw () = 0;

	virtual bool getNextRoutePoint ( Vector *vPoint ) = 0;

	inline Vector getPreviousPoint () { return m_vPreviousPoint; }

	virtual bool hasNextPoint () = 0;

	virtual int getCurrentWaypointID () = 0;

	virtual int getCurrentGoalID () = 0;

	virtual Vector getNextPoint () = 0;

	virtual void updatePosition () = 0;

	virtual bool canGetTo ( Vector vOrigin ) = 0;

	virtual int getCurrentFlags () { return 0; }
	virtual int getPathFlags ( int iPath ) { return 0; }

	virtual float distanceTo ( Vector vOrigin ) = 0;

	virtual float distanceTo ( CWaypoint *pWaypoint ) = 0;

	//virtual void goBack () = 0;

	virtual void freeMapMemory () = 0;		

	virtual void freeAllMemory () = 0;

	virtual bool routeFound () = 0;

	virtual void clear () = 0;

	virtual void getFailedGoals (WaypointList **goals) = 0;

	inline Vector getGoalOrigin () { return m_vGoal; }

	virtual bool nextPointIsOnLadder () { return false; }

	virtual bool beliefLoad ( ) { return false; };

	virtual bool beliefSave ( bool bOverride = false ) { return false; };

	virtual void belief ( Vector origin, Vector vOther, float fBelief, float fStrength, BotBelief iType ) = 0;

	// nearest cover position to vOrigin only
	virtual bool getCoverPosition ( Vector vCoverOrigin, Vector *vCover ) = 0;
	// nearest cover postion to both vectors
	virtual bool getHideSpotPosition ( Vector vCoverOrigin, Vector *vCover ) = 0;

	virtual float getCurrentBelief ( ) { return 0; }

	virtual float getBelief ( int index ) { return 0; }

	virtual void beliefOne ( int iWptIndex, BotBelief iBeliefType, float fDist ) { return; }

	virtual int numPaths ( ) { return 0; }

	virtual Vector getPath ( int pathid ) { return Vector(0,0,0); }

	virtual bool randomDangerPath (Vector *vec) { return false; }

	bool getDangerPoint ( Vector *vec ) { *vec = m_bDangerPoint ? m_vDangerPoint : Vector(0,0,0); return m_bDangerPoint; }

	bool wantToLoadBelief () { return m_bLoadBelief; }
	virtual bool wantToSaveBelief () { return false; }
	float getGoalDistance () { return m_fGoalDistance; }

	static const int MAX_PATH_TICKS = 200;

protected:
	Vector m_vGoal;
	float m_fGoalDistance;
	Vector m_vPreviousPoint;
	Vector m_vDangerPoint;
	bool m_bDangerPoint;
	short int m_iBeliefTeam;
	bool m_bBeliefChanged;
	bool m_bLoadBelief;
};

#define FL_ASTAR_CLOSED		1
#define FL_ASTAR_PARENT		2
#define FL_ASTAR_OPEN		4
#define FL_HEURISTIC_SET	8

class AStarNode
{
public:
	AStarNode() { memset(this,0,sizeof(AStarNode)); }
	///////////////////////////////////////////////////////
	inline void close () { setFlag(FL_ASTAR_CLOSED); }
	inline void unClose () { removeFlag(FL_ASTAR_CLOSED); }
	inline bool isOpen () { return hasFlag(FL_ASTAR_OPEN); }
	inline void unOpen () { removeFlag(FL_ASTAR_OPEN); }
	inline bool isClosed () { return hasFlag(FL_ASTAR_CLOSED); }
	inline void open () { setFlag(FL_ASTAR_OPEN); }
	//////////////////////////////////////////////////////	
	inline void setHeuristic ( float fHeuristic ) { m_fHeuristic = fHeuristic; setFlag(FL_HEURISTIC_SET); }
	inline bool heuristicSet () { return hasFlag(FL_HEURISTIC_SET); }
	inline const float getHeuristic () { return m_fHeuristic; } const
	
	////////////////////////////////////////////////////////
	inline void setFlag ( int iFlag ) { m_iFlags |= iFlag; }
	inline bool hasFlag ( int iFlag ) { return ((m_iFlags & iFlag) == iFlag); }
	inline void removeFlag ( int iFlag ) { m_iFlags &= ~iFlag; }
	/////////////////////////////////////////////////////////
	inline int getParent () { if ( hasFlag(FL_ASTAR_PARENT) ) return m_iParent; else return -1; }
	inline void setParent ( short int iParent ) 
	{ 
		m_iParent = iParent; 

		if ( m_iParent == -1 )
			removeFlag(FL_ASTAR_PARENT); // no parent
		else
			setFlag(FL_ASTAR_PARENT);
	}
	////////////////////////////////////////////////////////
	inline const float getCost () { return m_fCost; } const
	inline void setCost ( float fCost ) { m_fCost = fCost; }
	////////////////////////////////////////////////////////
	// for comparison
	bool precedes ( AStarNode *other ) const
	{
		return (m_fCost+m_fHeuristic) < (other->getCost() + other->getHeuristic());
	}
	void setWaypoint ( int iWpt ) { m_iWaypoint = iWpt; }
	inline int getWaypoint () { return m_iWaypoint; }
private:
	float m_fCost;
	float m_fHeuristic;
	unsigned char m_iFlags;
	short int m_iParent;
	int m_iWaypoint;
};
// Insertion sorted list
class AStarListNode
{
public:
	AStarListNode ( AStarNode *data )
	{
		m_Data = data;
		m_Next = NULL;
	}
	AStarNode *m_Data;
	AStarListNode *m_Next;
};

class AStarOpenList
{
public:
	AStarOpenList()
	{
		m_Head = NULL;
	}

	bool empty ()
	{
		return (m_Head==NULL);
	}

	AStarNode *top ()
	{
		if ( m_Head == NULL )
			return NULL;
		
		return m_Head->m_Data;
	}

	void pop ()
	{
		if ( m_Head != NULL )
		{
			AStarListNode *t = m_Head;

			m_Head = m_Head->m_Next;

			delete t;
		}
	}


	void add ( AStarNode *data )
	{
		AStarListNode *newNode = new AStarListNode(data);
		AStarListNode *t;
		AStarListNode *p;

		if ( m_Head == NULL )
			m_Head = newNode;
		else
		{
			if ( data->precedes(m_Head->m_Data) )
			{
				newNode->m_Next = m_Head;
				m_Head = newNode;
			}
			else
			{
				p = m_Head;
				t = m_Head->m_Next;

				while ( t != NULL )
				{
					if ( data->precedes(t->m_Data) )
					{
						p->m_Next = newNode;
						newNode->m_Next = t;
						break;
					}

					p = t;
					t = t->m_Next;
				}

				if ( t == NULL )
					p->m_Next = newNode;

			}
		}
	}

	void destroy ()
	{
		AStarListNode *t;

		while ( m_Head != NULL )
		{
			t = m_Head;
			m_Head = m_Head->m_Next;
			delete t;
			t = NULL;
		}

		m_Head = NULL;
	}
	
private:
	AStarListNode *m_Head;
};

/*
struct AstarNodeCompare : binary_function<AStarNode*, AStarNode*, bool> 
{
  // Other stuff...
  bool operator()(AStarNode* x, AStarNode* y) const 
  {
    return y->betterCost(x);
  }
};

class AStarOpenList : public vector<AStarNode*> 
{
  AstarNodeCompare comp;
public:
  AStarOpenList(AstarNodeCompare cmp = AstarNodeCompare()) : comp(cmp) {
    make_heap(begin(), end(), comp);
  }
  AStarNode* top() { return front(); }
  void push(AStarNode* x) {
    push_back(x);
    push_heap(begin(), end(), comp);
  }
  void pop() {
    pop_heap(begin(), end(), comp);
    pop_back();
  }  
};*/


/*
bool operator<( const AStarNode & A, const AStarNode & B )
{
    return A.betterCost(&B);
}

bool operator<( const AStarNode * A, const AStarNode * B )
{
    return A->betterCost(B);
}*/

#define WPT_SEARCH_AVOID_SENTRIES 1
#define WPT_SEARCH_AVOID_SNIPERS 2
#define WPT_SEARCH_AVOID_TEAMMATE 4

typedef struct
{
	short int iFrom;
	short int iTo;
	bool bValid;
	bool bSkipped;
}failedpath_t;

class CWaypointNavigator : public IBotNavigator
{
public:
	CWaypointNavigator ( CBot *pBot ) 
	{ 
		init();
		m_pBot = pBot; 
		m_fNextClearFailedGoals = 0;
		m_bDangerPoint = false;
		m_iBeliefTeam = -1;
		m_bLoadBelief = true;
		m_bBeliefChanged = false;
		memset(&m_lastFailedPath,0,sizeof(failedpath_t));
	}

	void init ();

	CWaypoint *chooseBestFromBelief ( std::vector<CWaypoint*> &goals, bool bHighDanger = false, int iSearchFlags = 0, int iTeam = 0);
	CWaypoint *chooseBestFromBeliefBetweenAreas ( std::vector<AStarNode*> &goals, bool bHighDanger = false, bool bIgnoreBelief = false );

	float getNextYaw ();

	bool workRoute ( Vector vFrom, Vector vTo, bool *bFail, bool bRestart = true, bool bNoInterruptions = false, int iGoalId = -1, int iConditions = 0, int iDangerId = -1 );

	bool getNextRoutePoint ( Vector *vPoint );

	void clear ();

	Vector getNextPoint ();

	void updatePosition ();

    float getBelief ( int index ) { if ( index >= 0 ) return m_fBelief[index]; return 0; }

	void failMove ();

	bool hasNextPoint ();

	void freeMapMemory ();

	void freeAllMemory ();

	bool canGetTo ( Vector vOrigin );

	bool routeFound ();

	void rollBackPosition ();

	bool nextPointIsOnLadder ();

	void open ( AStarNode *pNode );

	AStarNode *nextNode ();

	float distanceTo ( Vector vOrigin );

	float distanceTo ( CWaypoint *pWaypoint );

	Vector getCoverOrigin ( Vector vCover );

	void clearOpenList ();

	float getCurrentBelief ( );

	//virtual void goBack();
	
	void belief ( Vector origin, Vector vOther, float fBelief, float fStrength, BotBelief iType );

	void beliefOne ( int iWptIndex, BotBelief iBeliefType, float fDist );

	// nearest cover position to vOrigin only
	bool getCoverPosition ( Vector vCoverOrigin, Vector *vCover );
	// nearest cover postion to both vectors
	bool getHideSpotPosition ( Vector vCoverOrigin, Vector *vCover );

	void getFailedGoals (WaypointList **goals) { *goals = &m_iFailedGoals; }

	int numPaths ( );

	Vector getPath ( int pathid );

	bool randomDangerPath (Vector *vec);

	bool beliefLoad ( );

	bool beliefSave ( bool bOverride = false );

	bool wantToSaveBelief ();

	inline int getCurrentWaypointID ()
	{
		return m_iCurrentWaypoint;
	}

	inline int getCurrentGoalID ()
	{
		return m_iGoalWaypoint;
	}

	int getCurrentFlags ();
	int getPathFlags ( int iPath );

private:
	CBot *m_pBot;

	//CWaypointVisibilityTable *m_pDangerNodes;

	//int m_iPrevWaypoint;
	int m_iCurrentWaypoint;
	int m_iPrevWaypoint;
	int m_iNextWaypoint;
	int m_iGoalWaypoint;
	bool m_bWorkingRoute;

	failedpath_t m_lastFailedPath;

	std::stack<int> m_currentRoute;
	std::queue<int> m_oldRoute;

	int m_iLastFailedWpt;

	AStarNode paths[CWaypoints::MAX_WAYPOINTS];
	AStarNode *curr;
	AStarNode *succ;

	WaypointList m_iFailedGoals;
	float m_fNextClearFailedGoals;

	float m_fBelief [CWaypoints::MAX_WAYPOINTS];

	AStarOpenList m_theOpenList;

	Vector m_vOffset;
	bool m_bOffsetApplied;
};

class CNavMeshNavigator : public IBotNavigator
{
public:
	virtual bool workRoute ( Vector vFrom, Vector vTo, bool *bFail, bool bRestart = true, bool bNoInterruptions = false, int iGoalId = -1, int iConditions = 0, int iDangerId = -1  );

	virtual Vector getNextPoint ();

	virtual void updatePosition ();

	void freeMapMemory ();

	void freeAllMemory ();

	bool routeFound ();

	bool hasNextPoint ();

	void rollBackPosition () {};

    void init ();

    void belief ( Vector origin, Vector facing, float fBelief, float fStrength, BotBelief iType ){}; //bir3yk

	//void rememberEnemyPosition ( Vector vOrigin );

	//Vector getEnemyPositionPinchPoint ( Vector vOrigin );
private:
	CNavMesh *m_pNavMesh;
};

#endif