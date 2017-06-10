#ifdef WIN32
#include <Windows.h>
#else
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
#endif
#include "eiface.h"
#include "bot_const.h"
#include "bot.h"
#include "bot_fortress.h"
#include "bot_kv.h"

#include "bot_sigscan.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CGetEconItemSchema *g_pGetEconItemSchema = NULL;
CSetRuntimeAttributeValue *g_pSetRuntimeAttributeValue = NULL;
CGetAttributeDefinitionByName *g_pGetAttributeDefinitionByName = NULL;
CAttributeList_GetAttributeByID *g_pAttribList_GetAttributeByID = NULL;

size_t CSignatureFunction :: decodeHexString(unsigned char *buffer, size_t maxlength, const char *hexstr)
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
		if (hdr.p_type == PT_LOAD && hdr.p_flags == (PF_X|PF_R))
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

void *CSignatureFunction::findSignature ( void *addrInBase, const char *signature )
{
	// First, preprocess the signature 
	unsigned char real_sig[511];

	size_t real_bytes;
	//size_t length = strlen(signature);

	real_bytes = decodeHexString(real_sig, sizeof(real_sig), signature);

	if (real_bytes >= 1)
	{
		return findPattern(addrInBase, (char*) real_sig, real_bytes);
	}

	return NULL;
}


void CSignatureFunction :: findFunc ( CRCBotKeyValueList *kv, const char*pKey, void *pAddrBase, const char *defaultsig )
{
	char *sig = NULL;

	if ( kv->getString(pKey,&sig) && sig )
		m_func = findSignature(pAddrBase,sig);
	else
		m_func = findSignature(pAddrBase,defaultsig);
}


CGetEconItemSchema::CGetEconItemSchema ( CRCBotKeyValueList *list, void *pAddrBase )
{
#ifdef _WIN32
	findFunc(list,"get_item_schema_win",pAddrBase,"\\xE8\\x2A\\x2A\\x2A\\x2A\\x83\\xC0\\x04\\xC3");
#else
	findFunc(list,"get_item_schema_linux",pAddrBase,"@_Z15GEconItemSchemav");
#endif
}

CEconItemSchema *CGetEconItemSchema::callme()
{
	void *thefunc = m_func;
	CEconItemSchema *pret = NULL;

	if ( thefunc == NULL )
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



CSetRuntimeAttributeValue::CSetRuntimeAttributeValue ( CRCBotKeyValueList *list, void *pAddrBase )
{
#ifdef _WIN32
	findFunc(list,"set_attribute_value_win",pAddrBase,"\\x55\\x8B\\xEC\\x83\\xEC\\x14\\x33\\xD2\\x53\\x8B\\xD9\\x56\\x57\\x8B\\x73\\x10\\x85\\xF6");
#else
	findFunc(list,"set_attribute_value_linux",pAddrBase,"@_ZN14CAttributeList24SetRuntimeAttributeValueEPK28CEconItemAttributeDefinitionf");
#endif
}

bool CSetRuntimeAttributeValue::callme(edict_t *pEnt, CAttributeList *list, CEconItemAttributeDefinition *attrib,int value)
{
	int bret = 0;
	void *thefunc = m_func;

	int iEntityIndex = ENTINDEX(pEnt);

	if ( list && attrib && thefunc )
	{
		//*(DWORD*)&CAttributeListSetValue = (DWORD)SetRuntimeAttributeValue;
		//(*list.*CAttributeListSetValue)(attrib,value);
		//void *preveax1 = 0x0;
		
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
#endif
	}

	return (bret==1) || ((bret & 0x1FFF) == ((iEntityIndex + 4) * 4));
}



CGetAttributeDefinitionByName::CGetAttributeDefinitionByName ( CRCBotKeyValueList *list, void *pAddrBase )
{
#ifdef _WIN32
	findFunc(list,"get_attrib_definition_win",pAddrBase,"\\x55\\x8B\\xEC\\x83\\xEC\\x1C\\x53\\x8B\\xD9\\x8B\\x0D\\x2A\\x2A\\x2A\\x2A\\x56\\x33\\xF6\\x89\\x5D\\xF8\\x89\\x75\\xE4\\x89\\x75\\xE8");
#else
	findFunc(list,"get_attrib_definition_linux",pAddrBase,"@_ZN15CEconItemSchema22GetAttributeDefinitionEi");
#endif
}

CEconItemAttributeDefinition *CGetAttributeDefinitionByName::callme(CEconItemSchema *schema, const char *attrib)
{
	void *pret = NULL;

	if ( schema && m_func )
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

		pret = (void*)func(schema,attrib);
#endif
	}

	return (CEconItemAttributeDefinition*)pret;
}


CAttributeList_GetAttributeByID::CAttributeList_GetAttributeByID ( CRCBotKeyValueList *list, void *pAddrBase )
{
#ifdef _WIN32
	findFunc(list,"attributelist_get_attrib_by_id_win",pAddrBase,"\\x55\\x8B\\xEC\\x51\\x8B\\xC1\\x53\\x56\\x33\\xF6\\x89\\x45\\xFC\\x8B\\x58\\x10");
#else
	findFunc(list,"attributelist_get_attrib_by_id_linux",pAddrBase,"@_ZNK14CAttributeList16GetAttributeByIDEi");
#endif
}

CEconItemAttributeDefinition *CAttributeList_GetAttributeByID::callme(CAttributeList *list, int id)
{
	void *pret = NULL;

	if ( list && m_func )
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

	   pret = (void*)func(list,id);
#endif
	}

	return (CEconItemAttributeDefinition*)pret;
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
