#ifndef __RCBOT_TF2_POINTS_H__
#define __RCBOT_TF2_POINTS_H__

#include "utlmap.h"

class CTeamControlPoint;

class CTeamControlPointRound
{
public:

	bool isPointInRound ( edict_t *point_pent );

	CUtlVector< CBaseHandle > m_ControlPoints;

	bool m_bDisabled;

	string_t	m_iszCPNames;
	int			m_nPriority;
	int			m_iInvalidCapWinner;
	string_t	m_iszPrintName;
};


class CTFGameRulesProxy
{
	MyEHandle m_Resource;

	float m_flCapturePointEnableTime;
};


#define TEAM_ARRAY( index, team )		(index + (team * MAX_CONTROL_POINTS))

typedef enum ePointAttackDefend_s
{
	TF2_POINT_DEFEND = 0,
	TF2_POINT_ATTACK
}ePointAttackDefend;

typedef struct
{
	float fProb;
	float fProbMultiplier;
	bool bValid;
	bool bNextPoint;
	bool bPrev;
	int iPrev[MAX_PREVIOUS_POINTS];
}TF2PointProb_t;

class CTFObjectiveResource
{
public:
	CTFObjectiveResource()
	{
		reset();
	}

	void reset ()
	{
		memset(this,0,sizeof(CTFObjectiveResource));
		memset(m_iControlPointWpt,0xFF,sizeof(int)*MAX_CONTROL_POINTS);
		m_iMonitorPoint[0] = -1;
		m_iMonitorPoint[1] = -1;
	}


	bool isWaypointAreaValid ( int wptarea, int waypointflags = 0 );


	bool testProbWptArea ( int iWptArea, int iTeam );

	void debugprint ( void );
	void updatePoints();
	bool TeamCanCapPoint( int index, int team )
	{
		AssertValidIndex(index);
		return m_bTeamCanCap[ TEAM_ARRAY( index, team ) ];
	}

	// Is the point visible in the objective display
	bool	IsCPVisible( int index )
	{
		return (m_bCPIsVisible[index] == 1);
	}

	bool	IsCPBlocked( int index )
	{
		return m_bBlocked[index];
	}

	void think ();

	int getControlPointArea ( edict_t *pPoint );

	// Get the world location of this control point
	Vector& GetCPPosition( int index )
	{
		return m_vCPPositions[index];
	}

	int getControlPointWaypoint ( int index )
	{
		return m_iControlPointWpt[index];
	}

	int NearestArea ( Vector vOrigin );

	int GetCappingTeam( int index )
	{
		if ( index >= *m_iNumControlPoints )
			return TEAM_UNASSIGNED;

		return m_iCappingTeam[index];
	}

	int GetTeamInZone( int index )
	{
		if ( index >= *m_iNumControlPoints )
			return TEAM_UNASSIGNED;

		return m_iTeamInZone[index];
	}

	// Icons
	int GetCPCurrentOwnerIcon( int index, int iOwner )
	{
		return GetIconForTeam( index, iOwner );
	}

	int GetCPCappingIcon( int index )
	{
		int iCapper = GetCappingTeam(index);

		return GetIconForTeam( index, iCapper );
	}

	// Icon for the specified team
	int GetIconForTeam( int index, int team )
	{		
		return m_iTeamIcons[ TEAM_ARRAY(index,team) ];
	}

	// Overlay for the specified team
	int GetOverlayForTeam( int index, int team )
	{
		return m_iTeamOverlays[ TEAM_ARRAY(index,team) ];
	}

	// Number of players in the area
	int GetNumPlayersInArea( int index, int team )
	{
		return m_iNumTeamMembers[ TEAM_ARRAY(index,team) ];
	}
	
	// get the required cappers for the passed team
	int GetRequiredCappers( int index, int team )
	{
		return m_iTeamReqCappers[ TEAM_ARRAY(index,team) ];
	}

	// Base Icon for the specified team
	int GetBaseIconForTeam( int team )
	{
		return m_iTeamBaseIcons[ team ];
	}

	int GetBaseControlPointForTeam( int iTeam ) 
	{ 
		return m_iBaseControlPoints[iTeam]; 
	}

	// Data functions, called to set up the state at the beginning of a round
	inline int	 GetNumControlPoints( void ) 
	{ 
		if ( m_iNumControlPoints==NULL )
			return 0;
		return *m_iNumControlPoints;
	}

	int GetPreviousPointForPoint( int index, int team, int iPrevIndex )
	{
		AssertValidIndex(index);
		Assert( iPrevIndex >= 0 && iPrevIndex < MAX_PREVIOUS_POINTS );
		int iIntIndex = iPrevIndex + (index * MAX_PREVIOUS_POINTS) + (team * MAX_CONTROL_POINTS * MAX_PREVIOUS_POINTS);
		return m_iPreviousPoints[ iIntIndex ];
	}

	int GetOwningTeam( int index )
	{
		if ( index >= *m_iNumControlPoints )
			return TEAM_UNASSIGNED;

		return m_iOwner[index];
	}	

	void AssertValidIndex( int index )
	{
		Assert( (0 <= index) && (index <= MAX_CONTROL_POINTS) && (index < *m_iNumControlPoints) );
	}

	float getLastCaptureTime(int index);

	bool isCPValidWptArea ( int iWptArea, int iTeam, ePointAttackDefend_s type);
	bool isCPValid ( int iCPIndex, int iTeam, ePointAttackDefend_s type);


	// Mini-rounds data
	bool PlayingMiniRounds( void ){ return *m_bPlayingMiniRounds; }
	bool IsInMiniRound( int index ) { return m_bInMiniRound[index]; }
	void updateCaptureTime(int index);
	void setup ();
	bool isInitialised() { return m_bInitialised == true; }
	MyEHandle m_ObjectiveResource;

	int getRandomValidPointForTeam ( int team, ePointAttackDefend_s type );

	// [team][type][point]
	TF2PointProb_t m_ValidPoints[2][2][MAX_CONTROL_POINTS];
	// [team][type] -- if changed, bots must also be updated
	int m_PointSignature[2][2]; 

	// this is a union set of the above array
	bool m_ValidAreas[MAX_CONTROL_POINTS];
	 // For compatibility with old waypoints, need this
	int m_IndexToWaypointAreaTranslation[MAX_CONTROL_POINTS];
	int m_WaypointAreaToIndexTranslation[MAX_CONTROL_POINTS+1]; // add one because 0 is always valid for waypoints

	MyEHandle m_pControlPoints[MAX_CONTROL_POINTS];
	//CTeamControlPoint *m_pControlPointClass[MAX_CONTROL_POINTS];
	int m_iControlPointWpt[MAX_CONTROL_POINTS];
	bool m_iControlPointWptReachable[MAX_CONTROL_POINTS];
	int *m_iNumControlPoints;
	Vector *m_vCPPositions;//[8];
	int *m_bCPIsVisible;//[8];
	int *m_iTeamIcons;
	int *m_iTeamOverlays;
	int *m_iTeamReqCappers;
	float *m_flTeamCapTime;
	int *m_iPreviousPoints;
	bool *m_bTeamCanCap;//;//[64];
	int *m_iTeamBaseIcons;
	int *m_iBaseControlPoints;
	bool *m_bInMiniRound;//[8];
	int *m_iWarnOnCap;//[8];
	int *m_iCPGroup;//[8];
	bool *m_bCPLocked;//[8];
	bool *m_bTrackAlarm;//[4];
	float *m_flUnlockTimes;//[8];
	float *m_flCPTimerTimes;//[8];
	int *m_iNumTeamMembers;//[64];
	int *m_iCappingTeam;//[8];
	int *m_iTeamInZone;//[8];
	bool *m_bBlocked;//[8];
	int *m_iOwner;//[8];
	bool *m_bCPCapRateScalesWithPlayers;//[8];
	bool *m_bPlayingMiniRounds;
	// capindex of point being monitored to check previous points for both teams
	int m_iMonitorPoint[2];
	float m_fNextCheckMonitoredPoint;
	
	float m_fLastCaptureTime[MAX_CONTROL_POINTS];

	float m_fUpdatePointTime;

	private:
	bool m_bInitialised;

	//return a signature of the points structure. Bots will rethink their defend or attack point
	// if the signature changes
	bool updateAttackPoints ( int team );
	bool updateDefendPoints ( int team );

	inline void resetValidWaypointAreas() 
	{ 
		memset(m_ValidAreas,0,sizeof(bool)*MAX_CONTROL_POINTS); 
	}
	void updateValidWaypointAreas ( void )
	{
		resetValidWaypointAreas();

		for ( int i = 0; i < 2; i ++ )
		{
			for ( int j = 0; j < 2; j ++ )
			{
				for ( int k = 0; k < MAX_CONTROL_POINTS; k ++ )
				{
					// OR
					m_ValidAreas[k] = (m_ValidAreas[k] || m_ValidPoints[i][j][k].bValid);
				}
			}
		}
	}
	//bool *m_b
};

class CTeamRoundTimer
{
public:
	CTeamRoundTimer()
	{
		memset(this,0,sizeof(CTeamRoundTimer));
	}

	float getSetupTime ()
	{
		if ((m_Resource.get() != NULL) && m_nSetupTimeLength)
			return (float)*m_nSetupTimeLength;
		return 0.0f;
	}

	void reset ();

	MyEHandle m_Resource;

	float *m_flTimerEndTime;
	int *m_nSetupTimeLength;
	bool *m_bInSetup;
};

class variant_t
{
	union
	{
		bool bVal;
		string_t iszVal;
		int iVal;
		float flVal;
		float vecVal[3];
		color32 rgbaVal;
	};
	CHandle<CBaseEntity> eVal; // this can't be in the union because it has a constructor. 
	fieldtype_t fieldType;
};

class CEventAction;

class COutputEvent
{
	variant_t m_Value;
	CEventAction *m_ActionList;
	DECLARE_SIMPLE_DATADESC();
};

class CTeamControlPointMaster
{
public:

	CUtlMap<int, CBaseEntity *> m_ControlPoints;

	bool m_bFoundPoints;		// true when the control points have been found and the array is initialized

	CTeamControlPointRound *getCurrentRound ( );

	CUtlVector<CBaseEntity *> m_ControlPointRounds;
	int m_iCurrentRoundIndex;

	bool m_bDisabled;

	string_t m_iszTeamBaseIcons[MAX_TEAMS];
	int m_iTeamBaseIcons[MAX_TEAMS];
	string_t m_iszCapLayoutInHUD;

	float m_flCustomPositionX;
	float m_flCustomPositionY;

	int m_iInvalidCapWinner;
	bool m_bSwitchTeamsOnWin;
	bool m_bScorePerCapture;
	bool m_bPlayAllRounds;

	bool m_bFirstRoundAfterRestart;

	COutputEvent m_OnWonByTeam1;
	COutputEvent m_OnWonByTeam2;
	
	float m_flPartialCapturePointsRate;
	float m_flLastOwnershipChangeTime;

};

class CSoundPatch;

class CBaseEntityOutput
{
public:
	variant_t m_Value;
	// end variant_t
	CEventAction *m_ActionList;
};

// From latest GITHUB
class CTeamControlPoint
{
public:
	//static CTeamControlPoint *getPoint ( edict_t *pent );

	int			m_iTeam;			
	int			m_iDefaultOwner;			// Team that initially owns the cap point
	int			m_iIndex;					// The index of this point in the controlpointArray
	int			m_iWarnOnCap;				// Warn the team that owns the control point when the opposing team starts to capture it.
	string_t	m_iszPrintName;
	string_t	m_iszWarnSound;				// Sound played if the team needs to be warned about this point being captured
	bool		m_bRandomOwnerOnRestart;	// Do we want to randomize the owner after a restart?
	bool		m_bLocked;
	float		m_flUnlockTime;				// Time to unlock

	// We store a copy of this data for each team, +1 for the un-owned state.
	struct perteamdata_t
	{
		perteamdata_t()
		{
			iszCapSound = NULL_STRING;
			iszModel = NULL_STRING;
			iModelBodygroup = -1;
			iIcon = 0;
			iszIcon = NULL_STRING;
			iOverlay = 0;
			iszOverlay = NULL_STRING;
			iPlayersRequired = 0;
			iTimedPoints = 0;
			for ( int i = 0; i < MAX_PREVIOUS_POINTS; i++ )
			{
				iszPreviousPoint[i] = NULL_STRING;
			}
			iTeamPoseParam = 0;
		}

		string_t	iszCapSound;
		string_t	iszModel;
		int			iModelBodygroup;
		int			iTeamPoseParam;
		int			iIcon;
		string_t	iszIcon;
		int			iOverlay;
		string_t	iszOverlay;
		int			iPlayersRequired;
		int			iTimedPoints;
		string_t	iszPreviousPoint[MAX_PREVIOUS_POINTS];
	};
	CUtlVector<perteamdata_t>	m_TeamData;

	CBaseEntityOutput	m_OnCapReset;

	CBaseEntityOutput	m_OnCapTeam1;
	CBaseEntityOutput	m_OnCapTeam2;

	CBaseEntityOutput	m_OnOwnerChangedToTeam1;
	CBaseEntityOutput	m_OnOwnerChangedToTeam2;

	CBaseEntityOutput	m_OnRoundStartOwnedByTeam1;
	CBaseEntityOutput	m_OnRoundStartOwnedByTeam2;

	CBaseEntityOutput	m_OnUnlocked;

	int			m_bPointVisible;		//should this capture point be visible on the hud?
	int			m_iPointIndex;			//the mapper set index value of this control point

	int			m_iCPGroup;			//the group that this control point belongs to
	bool		m_bActive;			//

	string_t	m_iszName;				//Name used in cap messages

	bool		m_bStartDisabled;

	float		m_flLastContestedAt;

	CSoundPatch *m_pCaptureInProgressSound;
	string_t	m_iszCaptureStartSound;
	string_t	m_iszCaptureEndSound;
	string_t	m_iszCaptureInProgress;
	string_t	m_iszCaptureInterrupted;
};

#endif