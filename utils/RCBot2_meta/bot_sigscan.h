#ifndef __BOT_SIGSCAN_H__
#define __BOT_SIGSCAN_H__

#include "bot_const.h"

struct DynLibInfo
{
	void *baseAddress;
	size_t memorySize;
};

class CRCBotKeyValueList;

class CSignatureFunction
{
public:
	CSignatureFunction() { m_func = nullptr; }
private:
	static size_t decodeHexString(unsigned char *buffer, size_t maxlength, const char *hexstr);

	static bool getLibraryInfo(const void *libPtr, DynLibInfo &lib);

	void *findPattern(const void *libPtr, const char *pattern, size_t len);

	void *findSignature (const void* addrInBase, const char* signature);
protected:
	void findFunc (CRCBotKeyValueList& kv, const char* pKey, const void* pAddrBase, const char* defaultsig);

	void *m_func;
};

class CGameRulesObject : public CSignatureFunction
{
public:
	CGameRulesObject(CRCBotKeyValueList &list, void *pAddrBase);

	bool found() const { return m_func != nullptr; }

	void **getGameRules() const { return static_cast<void **>(m_func); }
};

class CCreateGameRulesObject : public CSignatureFunction
{
public:
	CCreateGameRulesObject(CRCBotKeyValueList &list, const void *pAddrBase);

	bool found() const { return m_func != nullptr; }

	void **getGameRules() const;
};

extern CGameRulesObject *g_pGameRules_Obj;
extern CCreateGameRulesObject *g_pGameRules_Create_Obj;

void *GetGameRules();
#endif