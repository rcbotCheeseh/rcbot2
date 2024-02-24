#ifndef __BOT_EXT_SOURCEMOD_H__
#define __BOT_EXT_SOURCEMOD_H__

#include <IExtensionSys.h>
#include <smsdk_config.h>
//#include <IBinTools.h>
//#include <ISDKHooks.h>
#include <ISDKTools.h>

#include "bot_plugin_meta.h"

using SourceMod::IExtension;
using SourceMod::IShareSys;
using SourceMod::IExtensionManager;

class RCBotSourceModExt : public SourceMod::IExtensionInterface
{
	public:
	virtual bool OnExtensionLoad(IExtension *me, IShareSys *sys, char* error, size_t maxlength, bool late);
	virtual void OnExtensionUnload();
	virtual void OnExtensionsAllLoaded();
	virtual void OnExtensionPauseChange(bool pause);
	virtual bool QueryRunning(char *error, size_t maxlength);
	virtual bool IsMetamodExtension();
	virtual const char *GetExtensionName();
	virtual const char *GetExtensionURL();
	virtual const char *GetExtensionTag();
	virtual const char *GetExtensionAuthor();
	virtual const char *GetExtensionVerString();
	virtual const char *GetExtensionDescription();
	virtual const char *GetExtensionDateString();

	virtual void LateLoadExtensions();
};

bool SM_AcquireInterfaces(char *error, size_t maxlength);
bool SM_LoadExtension(char *error, size_t maxlength);
void SM_UnloadExtension();
void SM_UnsetInterfaces();

extern RCBotSourceModExt g_RCBotSourceMod;

extern SourceMod::IExtensionManager *smexts;

extern SourceMod::IShareSys *sharesys;
extern SourceMod::IExtension *myself;

//extern SourceMod::IBinTools *sm_bintools;
extern SourceMod::ISDKTools *sm_sdktools;
//extern SourceMod::ISDKHooks *sm_sdkhooks;

#endif
