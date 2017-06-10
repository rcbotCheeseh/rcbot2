#ifndef __BOT_BASEENTITY_HACK_H__
#define __BOT_BASEENTITY_HACK_H__

struct PVSInfo_t
{
	// number of clusters or -1 if too many
	int			m_nClusterCount;		
	
	// cluster indices
	int			m_pClusters[ MAX_ENT_CLUSTERS ];	

	// headnode for the entity's bounding box
	int			m_nHeadNode;			

	// For dynamic "area portals"
	int			m_nAreaNum;
	int			m_nAreaNum2;

	// current position
	float		m_vCenter[3];
};

class CServerNetworkProperty : public IServerNetworkable, public IEventRegisterCallback
{
public:
	DECLARE_CLASS_NOBASE( CServerNetworkProperty );

public:
	CServerNetworkProperty();
	virtual	~CServerNetworkProperty();

public:
// IServerNetworkable implementation.
	virtual IHandleEntity  *GetEntityHandle( );
	virtual int				GetEFlags() const;
	virtual void			AddEFlags( int iEFlags );
	virtual edict_t			*GetEdict() const;
	virtual CBaseNetworkable* GetBaseNetworkable();
	virtual CBaseEntity*	GetBaseEntity();
	virtual ServerClass*	GetServerClass();
	virtual const char*		GetClassName() const;
	virtual void			Release();
	virtual int				AreaNum() const;
	virtual PVSInfo_t*		GetPVSInfo();

public:
	// Other public methods
	void Init( CBaseEntity *pEntity );

	void AttachEdict( edict_t *pRequiredEdict = NULL );
	
	// Methods to get the entindex + edict
	int	entindex() const;
	edict_t *edict();
	const edict_t *edict() const;

	// Sets the edict pointer (for swapping edicts)
	void SetEdict( edict_t *pEdict );

	// All these functions call through to CNetStateMgr. 
	// See CNetStateMgr for details about these functions.
	void NetworkStateForceUpdate();
	void NetworkStateChanged();

	// This is useful for entities that don't change frequently or that the client
	// doesn't need updates on very often. If you use this mode, the server will only try to
	// detect state changes every N seconds, so it will save CPU cycles and bandwidth.
	//
	// Note: N must be less than AUTOUPDATE_MAX_TIME_LENGTH.
	//
	// Set back to zero to disable the feature.
	//
	// This feature works on top of manual mode. 
	// - If you turn it on and manual mode is off, it will autodetect changes every N seconds.
	// - If you turn it on and manual mode is on, then every N seconds it will only say there
	//   is a change if you've called NetworkStateChanged.
	void			SetUpdateInterval( float N );

	// You can use this to override any entity's ShouldTransmit behavior.
	// void SetTransmitProxy( CBaseTransmitProxy *pProxy );

	// This version does a PVS check which also checks for connected areas
	bool IsInPVS( const CCheckTransmitInfo *pInfo );

	// This version doesn't do the area check
	bool IsInPVS( const edict_t *pRecipient, const void *pvs, int pvssize );

	// Called by the timed event manager when it's time to detect a state change.
	virtual void FireEvent();

private:
	// Detaches the edict.. should only be called by CBaseNetworkable's destructor.
	void DetachEdict();
	CBaseEntity *GetOuter();

	// Recomputes PVS information
	void RecomputePVSInformation();

	// Marks the networkable that it will should transmit
	void SetTransmit( CCheckTransmitInfo *pInfo );

private:
	CBaseEntity *m_pOuter;
	// CBaseTransmitProxy *m_pTransmitProxy;
	edict_t	*m_pPev;
	PVSInfo_t m_PVSInfo;

	// Counters for SetUpdateInterval.
	CEventRegister	m_TimerEvent;
	bool m_bPendingStateChange;

//	friend class CBaseTransmitProxy;
};

// IServerNetworkable is the interface the engine uses for all networkable data.
class IServerNetworkable
{
// These functions are handled automatically by the server_class macros and CBaseNetworkable.
public:
	// Gets at the entity handle associated with the collideable
	virtual IHandleEntity	*GetEntityHandle() = 0;

	// Tell the engine which class this object is.
	virtual ServerClass*	GetServerClass() = 0;

	// Return a combo of the EFL_ flags.
	virtual int				GetEFlags() const = 0;
	virtual void			AddEFlags( int iEFlags ) = 0;

	virtual edict_t			*GetEdict() const = 0;

	virtual const char*		GetClassName() const = 0;
	virtual void			Release() = 0;

	virtual int				AreaNum() const = 0;

	// In place of a generic QueryInterface.
	virtual CBaseNetworkable* GetBaseNetworkable() = 0;
	virtual CBaseEntity*	GetBaseEntity() = 0; // Only used by game code.
	virtual PVSInfo_t*		GetPVSInfo() = 0; // get current visibilty data

protected:
	// Should never call delete on this! 
	virtual					~IServerNetworkable() {}
};


#endif