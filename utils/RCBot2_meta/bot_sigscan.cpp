#ifdef WIN32
#include <Windows.h>
#else

#include "shake.h" //bir3yk
#include "elf.h"

#define PAGE_SIZE 4096
#define PAGE_ALIGN_UP(x) ((x + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#endif

//#include "cbase.h"
//#include "baseentity.h"
#include "filesystem.h"
#include "interface.h"
#include "engine/iserverplugin.h"
#include "tier2/tier2.h"
#ifdef __linux__
#include "shake.h"    //bir3yk
#ifndef sscanf_s
#define sscanf_s sscanf
#endif
#endif
#include "eiface.h"
#include "bot_const.h"
#include "bot.h"
#include "bot_fortress.h"
#include "bot_kv.h"
#include "bot_getprop.h"
#include "bot_sigscan.h"
#include "bot_mods.h"

CGetEconItemSchema *g_pGetEconItemSchema = NULL;
CSetRuntimeAttributeValue *g_pSetRuntimeAttributeValue = NULL;
CGetAttributeDefinitionByName *g_pGetAttributeDefinitionByName = NULL;
CAttributeList_GetAttributeByID *g_pAttribList_GetAttributeByID = NULL;
CGameRulesObject *g_pGameRules_Obj = NULL;
CCreateGameRulesObject *g_pGameRules_Create_Obj = NULL;
CGetAttributeDefinitionByID *g_pGetAttributeDefinitionByID = NULL;

void **g_pGameRules = NULL;

void *GetGameRules()
{
	if (!g_pGameRules)
		return NULL;

	return *g_pGameRules;
}

size_t CSignatureFunction::decodeHexString(unsigned char *buffer, size_t maxlength, const char *hexstr)
{
	size_t written = 0;
	size_t length = strlen(hexstr);

	for (size_t i = 0; i < length; i++)
	{
		if (written >= maxlength)
			break;
		buffer[written++] = hexstr[i];
		if (hexstr[i] == '\\' && hexstr[i + 1] == 'x')
		{
			if (i + 3 >= length)
				continue;
			// Get the hex part. 
			char s_byte[3];
			int r_byte;
			s_byte[0] = hexstr[i + 2];
			s_byte[1] = hexstr[i + 3];
			s_byte[2] = '\0';
			// Read it as an integer 
			sscanf_s(s_byte, "%x", &r_byte);
			
			// Save the value 
			buffer[written - 1] = r_byte;
			// Adjust index 
			i += 3;
		}
	}

	return written;
}

bool CSignatureFunction::getLibraryInfo(const void *libPtr, DynLibInfo &lib)
{
	uintptr_t baseAddr;

	if (libPtr == NULL)
	{
		return false;
	}

#ifdef _WIN32


	MEMORY_BASIC_INFORMATION info;
	IMAGE_DOS_HEADER *dos;
	IMAGE_NT_HEADERS *pe;
	IMAGE_FILE_HEADER *file;
	IMAGE_OPTIONAL_HEADER *opt;

	if (!VirtualQuery(libPtr, &info, sizeof(MEMORY_BASIC_INFORMATION)))
	{
		return false;
	}

	baseAddr = reinterpret_cast<uintptr_t>(info.AllocationBase);

	// All this is for our insane sanity checks :o 
	dos = reinterpret_cast<IMAGE_DOS_HEADER *>(baseAddr);
	pe = reinterpret_cast<IMAGE_NT_HEADERS *>(baseAddr + dos->e_lfanew);
	file = &pe->FileHeader;
	opt = &pe->OptionalHeader;

	// Check PE magic and signature 
	if (dos->e_magic != IMAGE_DOS_SIGNATURE || pe->Signature != IMAGE_NT_SIGNATURE || opt->Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		return false;
	}

	// Check architecture, which is 32-bit/x86 right now
	// Should change this for 64-bit if Valve gets their act together

	if (file->Machine != IMAGE_FILE_MACHINE_I386)
	{
		return false;
	}

	//For our purposes, this must be a dynamic library 
	if ((file->Characteristics & IMAGE_FILE_DLL) == 0)
	{
		return false;
	}

	//Finally, we can do this
	lib.memorySize = opt->SizeOfImage;

#else
	Dl_info info;
	Elf32_Ehdr *file;
	Elf32_Phdr *phdr;
	uint16_t phdrCount;

	if (!dladdr(libPtr, &info))
	{
		return false;
	}

	if (!info.dli_fbase || !info.dli_fname)
	{
		return false;
	}

	// This is for our insane sanity checks :o 
	baseAddr = reinterpret_cast<uintptr_t>(info.dli_fbase);
	file = reinterpret_cast<Elf32_Ehdr *>(baseAddr);

	// Check ELF magic 
	if (memcmp(ELFMAG, file->e_ident, SELFMAG) != 0)
	{
		return false;
	}

	// Check ELF version 
	if (file->e_ident[EI_VERSION] != EV_CURRENT)
	{
		return false;
	}

	// Check ELF architecture, which is 32-bit/x86 right now
	// Should change this for 64-bit if Valve gets their act together

	if (file->e_ident[EI_CLASS] != ELFCLASS32 || file->e_machine != EM_386 || file->e_ident[EI_DATA] != ELFDATA2LSB)
	{
		return false;
	}

	// For our purposes, this must be a dynamic library/shared object 
	if (file->e_type != ET_DYN)
	{
		return false;
	}

	phdrCount = file->e_phnum;
	phdr = reinterpret_cast<Elf32_Phdr *>(baseAddr + file->e_phoff);

	for (uint16_t i = 0; i < phdrCount; i++)
	{
		Elf32_Phdr &hdr = phdr[i];

		// We only really care about the segment with executable code 
		if (hdr.p_type == PT_LOAD && hdr.p_flags == (PF_X | PF_R))
		{
			// From glibc, elf/dl-load.c:
			// c->mapend = ((ph->p_vaddr + ph->p_filesz + GLRO(dl_pagesize) - 1) 
			//             & ~(GLRO(dl_pagesize) - 1));
			//
			// In glibc, the segment file size is aligned up to the nearest page size and
			// added to the virtual address of the segment. We just want the size here.

			lib.memorySize = PAGE_ALIGN_UP(hdr.p_filesz);
			break;
		}
	}
#endif

	lib.baseAddress = reinterpret_cast<void *>(baseAddr);

	return true;
}

void *CSignatureFunction::findPattern(const void *libPtr, const char *pattern, size_t len)
{
	DynLibInfo lib;
	bool found;
	char *ptr, *end;

	memset(&lib, 0, sizeof(DynLibInfo));

	if (!getLibraryInfo(libPtr, lib))
	{
		return NULL;
	}

	ptr = reinterpret_cast<char *>(lib.baseAddress);
	end = ptr + lib.memorySize - len;

	while (ptr < end)
	{
		found = true;
		for (register size_t i = 0; i < len; i++)
		{
			if (pattern[i] != '\x2A' && pattern[i] != ptr[i])
			{
				found = false;
				break;
			}
		}

		if (found)
			return ptr;

		ptr++;
	}

	return NULL;
}
// Sourcemod - Metamod - Allied Modders.net
void *CSignatureFunction::findSignature(void *addrInBase, const char *signature)
{
	// First, preprocess the signature 
	unsigned char real_sig[511];

	size_t real_bytes;

	real_bytes = decodeHexString(real_sig, sizeof(real_sig), signature);

	if (real_bytes >= 1)
	{
		return findPattern(addrInBase, (char*)real_sig, real_bytes);
	}

	return NULL;
}


void CSignatureFunction::findFunc(CRCBotKeyValueList *kv, const char*pKey, void *pAddrBase, const char *defaultsig)
{
	char *sig = NULL;

	if (kv->getString(pKey, &sig) && sig)
		m_func = findSignature(pAddrBase, sig);
	else
		m_func = findSignature(pAddrBase, defaultsig);
}

CGameRulesObject::CGameRulesObject(CRCBotKeyValueList *list, void *pAddrBase)
{
#ifdef _WIN32
	m_func = NULL;
#else
	findFunc(list, "g_pGameRules", pAddrBase, "@g_pGameRules");
#endif
}

CCreateGameRulesObject::CCreateGameRulesObject(CRCBotKeyValueList *list, void *pAddrBase)
{
#ifdef _WIN32
	findFunc(list, "create_gamerules_object_win", pAddrBase, "\\x55\\x8B\\xEC\\x8B\\x0D\\x2A\\x2A\\x2A\\x2A\\x85\\xC9\\x74\\x07");
#else
	m_func = NULL;
#endif
}

void **CCreateGameRulesObject::getGameRules()
{
	char *addr = reinterpret_cast<char*>(m_func);
	extern ConVar rcbot_gamerules_offset;

	return *reinterpret_cast<void ***>(addr + rcbot_gamerules_offset.GetInt());
}

CGetEconItemSchema::CGetEconItemSchema(CRCBotKeyValueList *list, void *pAddrBase)
{
#ifdef _WIN32
	findFunc(list, "get_item_schema_win", pAddrBase, "\\xE8\\x2A\\x2A\\x2A\\x2A\\x83\\xC0\\x04\\xC3");
#else
	findFunc(list, "get_item_schema_linux", pAddrBase, "@_Z15GEconItemSchemav");
#endif
}

CEconItemSchema *CGetEconItemSchema::callme()
{
	void *thefunc = m_func;
	CEconItemSchema *pret = NULL;

	if (thefunc == NULL)
		return NULL;
#ifdef _WIN32
	__asm
	{
		call thefunc;
		mov pret, eax;
	};
#else
	FUNC_GET_ECON_ITEM_SCHEMA func = (FUNC_GET_ECON_ITEM_SCHEMA)thefunc;

	pret = func();
#endif
	return pret;
}



CSetRuntimeAttributeValue::CSetRuntimeAttributeValue(CRCBotKeyValueList *list, void *pAddrBase)
{
#ifdef _WIN32
	findFunc(list, "set_attribute_value_win", pAddrBase, "\\x55\\x8B\\xEC\\x83\\xEC\\x14\\x33\\xD2\\x53\\x8B\\xD9\\x56\\x57\\x8B\\x73\\x10\\x85\\xF6");
#else
	findFunc(list, "set_attribute_value_linux", pAddrBase, "@_ZN14CAttributeList24SetRuntimeAttributeValueEPK28CEconItemAttributeDefinitionf");
#endif
}

bool CSetRuntimeAttributeValue::callme(edict_t *pEnt, CAttributeList *list, CEconItemAttributeDefinition *attrib, float value)
{
	union {
		int (CAttributeList::*SetRunTimeAttributeValue)(CEconItemAttributeDefinition*, float);
		void* addr;
	} u;

	int bret = 0;
	void *thefunc = m_func;

	int iEntityIndex = ENTINDEX(pEnt);

	if (list && attrib && thefunc)
	{
		u.addr = m_func;

		bret = (reinterpret_cast<CAttributeList*>(list)->*u.SetRunTimeAttributeValue)(attrib, value);
		/*
		#ifdef _WIN32
		__asm
		{
		mov ecx, list;
		push attrib;
		push value;
		call thefunc;
		mov bret, eax;
		};
		#else
		FUNC_SET_ATTRIB_VALUE func = (FUNC_SET_ATTRIB_VALUE)thefunc;

		bret = func(list,attrib,value);
		#endif*/
	}

	return (bret == 1) || ((bret & 0x1FFF) == ((iEntityIndex + 4) * 4));
}


CGetAttributeDefinitionByID::CGetAttributeDefinitionByID(CRCBotKeyValueList *list, void *pAddrBase)
{
#ifdef _WIN32
	findFunc(list, "get_attrib_def_id_win", pAddrBase, "\\x55\\x8B\\xEC\\x83\\xEC\\x2A\\x53\\x56\\x8B\\xD9\\x8D\\x2A\\x2A\\x57");
#else
	findFunc(list, "get_attrib_def_id_linux", pAddrBase, "@_ZN15CEconItemSchema22GetAttributeDefinitionEi");
#endif
}

CEconItemAttributeDefinition *CGetAttributeDefinitionByID::callme(CEconItemSchema *schema, int id)
{
	void *pret = NULL;

	if (schema && m_func)
	{
		void *thefunc = m_func;
#ifdef _WIN32
		__asm
		{
			mov ecx, schema;
			push id;
			call thefunc;
			mov pret, eax;
		};
#else
		FUNC_GET_ATTRIB_BY_NAME func = (FUNC_GET_ATTRIB_BY_NAME)thefunc;

		pret = (void*)func(schema, id);
#endif
	}

	return (CEconItemAttributeDefinition*)pret;
}


CGetAttributeDefinitionByName::CGetAttributeDefinitionByName(CRCBotKeyValueList *list, void *pAddrBase)
{
#ifdef _WIN32
	findFunc(list, "get_attrib_definition_win", pAddrBase, "\\x55\\x8B\\xEC\\x83\\xEC\\x18\\x83\\x7D\\x08\\x00\\x53\\x56\\x57\\x8B\\xD9\\x75\\x2A\\x33\\xC0\\x5F");
#else
	findFunc(list, "get_attrib_definition_linux", pAddrBase, "@_ZN15CEconItemSchema28GetAttributeDefinitionByNameEPKc");
#endif
}

CEconItemAttributeDefinition *CGetAttributeDefinitionByName::callme(CEconItemSchema *schema, const char *attrib)
{
	void *pret = NULL;

	if (schema && m_func)
	{
		void *thefunc = m_func;
#ifdef _WIN32
		__asm
		{
			mov ecx, schema;
			push attrib;
			call thefunc;
			mov pret, eax;
		};
#else
		FUNC_GET_ATTRIB_BY_NAME func = (FUNC_GET_ATTRIB_BY_NAME)thefunc;

		pret = (void*)func(schema, attrib);
#endif
	}

	return (CEconItemAttributeDefinition*)pret;
}


CAttributeList_GetAttributeByID::CAttributeList_GetAttributeByID(CRCBotKeyValueList *list, void *pAddrBase)
{
#ifdef _WIN32
	findFunc(list, "attributelist_get_attrib_by_id_win", pAddrBase, "\\x55\\x8B\\xEC\\x51\\x8B\\xC1\\x53\\x56\\x33\\xF6\\x89\\x45\\xFC\\x8B\\x58\\x10");
#else
	findFunc(list, "attributelist_get_attrib_by_id_linux", pAddrBase, "@_ZNK14CAttributeList16GetAttributeByIDEi");
#endif
}

CEconItemAttribute *CAttributeList_GetAttributeByID::callme(CAttributeList *list, int id)
{
	void *pret = NULL;

	if (list && m_func)
	{
		void *thefunc = m_func;
#ifdef _WIN32
		__asm
		{
			mov ecx, list;
			push id;
			call thefunc;
			mov pret, eax;
		};
#else
		FUNC_ATTRIBLIST_GET_ATTRIB_BY_ID func = (FUNC_ATTRIBLIST_GET_ATTRIB_BY_ID)thefunc;

		pret = (void*)func(list, id);
#endif
	}

	return (CEconItemAttribute*)pret;
}

// TF2 Attributes - Flamin Sarge
bool TF2_SetAttrib(edict_t *pedict, const char *strAttrib, float flVal)
{
	//CBaseEntity *pEntity;

	if (!pedict || pedict->IsFree())
		return false;

	CAttributeList *pList = CClassInterface::getAttributeList(pedict);

	CEconItemSchema *pSchema = g_pGetEconItemSchema->callme();

	if (pSchema == NULL)
		return false;

	int id = CAttributeLookup::findAttributeID(strAttrib);

	if (id == -1)
		return false;

	CEconItemAttributeDefinition *pAttribDef = g_pGetAttributeDefinitionByID->callme(pSchema, id);

	if ((unsigned int)pAttribDef < 0x10000)
	{
		return false;
	}

	bool bSuccess = g_pSetRuntimeAttributeValue->callme(pedict, pList, pAttribDef, flVal);

	//Just a note, the above SDKCall returns ((entindex + 4) * 4) | 0xA000), and you can AND it with 0x1FFF to get back the entindex if you want, though it's pointless)
	//I don't know any other specifics, such as if the highest 3 bits actually matter
	//And I don't know what happens when you hit ent index 2047
	//	ClearAttributeCache(GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity"));
	//	decl String:strClassname[64];
	//	GetEntityClassname(entity, strClassname, sizeof(strClassname));
	//	if (strncmp(strClassname, "tf_wea", 6, false) == 0 || StrEqual(strClassname, "tf_powerup_bottle", false))
	//	{
	//		new client = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
	//		if (client > 0 && client <= MaxClients && IsClientInGame(client)) ClearAttributeCache(client);
	//	}

	return bSuccess;
}

CEconItemAttribute *TF2Attrib_GetByName(edict_t *entity, const char *strAttrib)
{
	if (entity->IsFree())
	{
		return NULL;
	}

	CAttributeList *pList = CClassInterface::getAttributeList(entity);

	if (pList == NULL)
		return NULL;

	if (*(int*)((unsigned long)pList + 4) == 0x0)
	{
		throw "Invalid Attribute List?";

		return NULL;
	}

	if (!g_pGetEconItemSchema)
		return NULL;

	CEconItemSchema *pSchema = g_pGetEconItemSchema->callme();

	if (pSchema == NULL)
		return NULL;
	CEconItemAttributeDefinition *pAttribDef = g_pGetAttributeDefinitionByName->callme(pSchema, strAttrib);

	if ((unsigned int)pAttribDef < 0x10000)
		return NULL;

	unsigned short int iDefIndex = *(unsigned short int*)(((unsigned long)pAttribDef) + 4);

	CEconItemAttribute *pAttrib = g_pAttribList_GetAttributeByID->callme(pList, iDefIndex);

	if ((unsigned int)pAttrib < 0x10000)
		pAttrib = NULL;

	return pAttrib;
}

bool TF2_setAttribute(edict_t *pEdict, const char *szName, float flVal)
{
	// Creates the new Attribute
	CEconItemAttribute *pAttrib = NULL;

	try
	{
		pAttrib = TF2Attrib_GetByName(pEdict, szName);
	}

	catch (const char *str)
	{
		if (str && *str)
			return false;
	}

	if (((unsigned int)pAttrib) < 0x10000)
	{
		return TF2_SetAttrib(pEdict, szName, flVal);
	}
	else
		pAttrib->m_flValue = flVal;

	return true;
}

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
