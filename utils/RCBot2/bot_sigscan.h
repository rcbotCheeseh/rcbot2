#ifndef __BOT_SIGSCAN_H__
#define __BOT_SIGSCAN_H__

#include "bot_const.h"

/* From SOURCEMOD */
class CAttributeList;
class CEconItemAttribute;
class CEconItemAttributeDefinition;
class CEconItemSchema;
class CEconWearable;

typedef CEconItemAttribute* (*FUNC_ATTRIBLIST_GET_ATTRIB_BY_ID)(CAttributeList*,int);
typedef CEconItemSchema* (*FUNC_GET_ECON_ITEM_SCHEMA)(void);
typedef CEconItemAttributeDefinition* (*FUNC_GET_ATTRIB_BY_NAME)(CEconItemSchema*,const char*);
typedef int (*FUNC_SET_ATTRIB_VALUE)(CAttributeList*,const CEconItemAttributeDefinition*, float);

void UTIL_ApplyAttribute ( edict_t *pEdict, const char *name, float fVal );

struct DynLibInfo
{
	void *baseAddress;
	size_t memorySize;
};

class CRCBotKeyValueList;

class CSignatureFunction
{
public:
	CSignatureFunction() { m_func = 0x0; }
private:
	size_t decodeHexString(unsigned char *buffer, size_t maxlength, const char *hexstr);

	bool getLibraryInfo(const void *libPtr, DynLibInfo &lib);

	void *findPattern(const void *libPtr, const char *pattern, size_t len);

	void *findSignature ( void *addrInBase, const char *signature );
protected:
	void findFunc ( CRCBotKeyValueList *kv, const char *pKey, void *pAddrBase, const char *defaultsig );

	void *m_func;
};


class CGetEconItemSchema : public CSignatureFunction
{
public:
	CGetEconItemSchema ( CRCBotKeyValueList *list, void *pAddrBase );

	CEconItemSchema *callme();
};

class CSetRuntimeAttributeValue : public CSignatureFunction
{
public:
	CSetRuntimeAttributeValue ( CRCBotKeyValueList *list, void *pAddrBase );

	bool callme(edict_t *pEnt, CAttributeList *list, CEconItemAttributeDefinition *attrib,int value);
};

class CGetAttributeDefinitionByName : public CSignatureFunction
{
public:
	CGetAttributeDefinitionByName ( CRCBotKeyValueList *list, void *pAddrBase );

	CEconItemAttributeDefinition *callme(CEconItemSchema *schema, const char *attrib);
};

class CAttributeList_GetAttributeByID : public CSignatureFunction
{
public:
	CAttributeList_GetAttributeByID ( CRCBotKeyValueList *list, void *pAddrBase );

	CEconItemAttributeDefinition *callme(CAttributeList *list, int id);
};

/*
CEconItemAttribute *UTIL_AttributeList_GetAttributeByID ( CAttributeList *list, int id )
{
	void *pret = NULL;

	if ( list && AttributeList_GetAttributeByID )
	{
#ifdef _WIN32
		__asm
	   {
		  mov ecx, list;
		  push id;
		  call AttributeList_GetAttributeByID;
		  mov pret, eax;
	   };
#else
	   FUNC_ATTRIBLIST_GET_ATTRIB_BY_ID func = (FUNC_ATTRIBLIST_GET_ATTRIB_BY_ID)AttributeList_GetAttributeByID;

	   pret = (void*)func(list,id);
#endif
	}

	return (CEconItemAttribute*)pret;
}
*/


extern CGetEconItemSchema *g_pGetEconItemSchema;
extern CSetRuntimeAttributeValue *g_pSetRuntimeAttributeValue;
extern CGetAttributeDefinitionByName *g_pGetAttributeDefinitionByName;
extern CAttributeList_GetAttributeByID *g_pAttribList_GetAttributeByID;

#endif