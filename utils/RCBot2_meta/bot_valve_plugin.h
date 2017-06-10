#ifndef __BOT_VALVE_PLUGIN_H__
#define __BOT_VALVE_PLUGIN_H__

#include "GameEventListener.h"
#include "iserverplugin.h"
//---------------------------------------------------------------------------------
// Purpose: a sample 3rd party plugin class
//---------------------------------------------------------------------------------
class CRCBotPlugin : public IServerPluginCallbacks, public IGameEventListener
{
public:
	CRCBotPlugin();
	~CRCBotPlugin();

	// IServerPluginCallbacks methods
	virtual bool			Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory );
	virtual void			Unload( void );
	virtual void			Pause( void );
	virtual void			UnPause( void );
	virtual const char     *GetPluginDescription( void );      
	virtual void			LevelInit( char const *pMapName );
	virtual void			ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
	virtual void			GameFrame( bool simulating );
		// Called once per simulation frame on the final tick
	virtual void			PreClientUpdate( bool simulating );
	virtual void			LevelShutdown( void );
	virtual void			ClientActive( edict_t *pEntity );
	virtual void			ClientDisconnect( edict_t *pEntity );
	virtual void			ClientPutInServer( edict_t *pEntity, char const *playername );
	virtual void			SetCommandClient( int index );
	virtual void			ClientSettingsChanged( edict_t *pEdict );
	virtual PLUGIN_RESULT	ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );
	virtual PLUGIN_RESULT	ClientCommand( edict_t *pEntity, const CCommand &args );
	virtual PLUGIN_RESULT	NetworkIDValidated( const char *pszUserName, const char *pszNetworkID );
	virtual void			OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue );

	// added with version 3 of the interface.
	virtual void			OnEdictAllocated( edict_t *edict );
	virtual void			OnEdictFreed( const edict_t *edict  );	

	// IGameEventListener Interface
	virtual void FireGameEvent( KeyValues * pevent );
	//virtual void FireGameEvent( IGameEvent * event );

	virtual int GetCommandIndex() { return m_iClientCommandIndex; }

	static void HudTextMessage ( edict_t *pEntity, const char *szMessage );

	static void ShowLicense ( void );

	CRCBotEventListener *getEventListener ( void );
private:
	int m_iClientCommandIndex;
	CRCBotEventListener *eventListener2;
};



extern CRCBotPlugin g_RCBOTServerPlugin;

#endif