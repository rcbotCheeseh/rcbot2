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
	virtual ~RCBotSourceModExt() = default;
	bool OnExtensionLoad(IExtension *me, IShareSys *sys, char* error, size_t maxlength, bool late) override;
	void OnExtensionUnload() override;
	void OnExtensionsAllLoaded() override;
	void OnExtensionPauseChange(bool pause) override;
	bool QueryRunning(char *error, size_t maxlength) override;
	bool IsMetamodExtension() override;
	const char *GetExtensionName() override;
	const char *GetExtensionURL() override;
	const char *GetExtensionTag() override;
	const char *GetExtensionAuthor() override;
	const char *GetExtensionVerString() override;
	const char *GetExtensionDescription() override;
	const char *GetExtensionDateString() override;

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
