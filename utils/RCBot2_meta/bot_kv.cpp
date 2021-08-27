#include "bot.h"
#include "bot_kv.h"
#include "bot_globals.h"

#include "logging.h"

void CRCBotKeyValueList::parseFile(std::fstream& fp)
{
	char buffer[2* RCBOT_MAX_KV_LEN];
	char szKey[RCBOT_MAX_KV_LEN];
	char szValue[RCBOT_MAX_KV_LEN];

	int iLine = 0;

	// parse profile ini
	while (fp.getline(buffer, 255))
	{
		iLine++;

		if ( buffer[0] == '#' ) // skip comment
			continue;

		int iLen = strlen(buffer);

		if ( iLen == 0 )
			continue;

		if ( buffer[iLen-1] == '\n' )
			buffer[--iLen] = 0;

		if ( buffer[iLen-1] == '\r' )
			buffer[--iLen] = 0;

		bool bHaveKey = false;

		int iKi = 0;
		int iVi = 0;

		for ( int iCi = 0; iCi < iLen; iCi ++ )
		{
			// ignore spacing
			if ( buffer[iCi] == ' ' )
				continue;

			if ( !bHaveKey )
			{
				if ( buffer[iCi] == '=' )
				{
					bHaveKey = true;
					continue;
				}

				// parse key

				if ( iKi < RCBOT_MAX_KV_LEN )
					szKey[iKi++] = buffer[iCi];													
			}
			else if ( iVi < RCBOT_MAX_KV_LEN )
				szValue[iVi++] = buffer[iCi];
			else
				break;
		}      

		szKey[iKi] = 0;
		szValue[iVi] = 0;

		logger->Log(LogLevel::TRACE, "m_KVs.emplace_back(%s,%s)", szKey, szValue);

		m_KVs.emplace_back(new CRCBotKeyValue(szKey,szValue));

	}

}

CRCBotKeyValueList :: ~CRCBotKeyValueList()
{
	for ( unsigned int i = 0; i < m_KVs.size(); i ++ )
	{
		delete m_KVs[i];
		m_KVs[i] = NULL;
	}

	m_KVs.clear();
}

CRCBotKeyValue *CRCBotKeyValueList :: getKV ( const char *key )
{
	for ( unsigned int i = 0; i < m_KVs.size(); i ++ )
	{
		if ( FStrEq(m_KVs[i]->getKey(),key) )
			return m_KVs[i];
	}

	return NULL;
}

bool CRCBotKeyValueList :: getFloat ( const char *key, float *val )
{
	CRCBotKeyValue* pKV = getKV(key);

	if ( !pKV )
		return false;
	
	*val = atof(pKV->getValue());

	return true;
}

	
bool CRCBotKeyValueList :: getInt ( const char *key, int *val )
{
	CRCBotKeyValue* pKV = getKV(key);

	if ( !pKV )
		return false;
	
	*val = atoi(pKV->getValue());

	return true;
}


bool CRCBotKeyValueList :: getString ( const char *key, char **val )
{
	CRCBotKeyValue* pKV = getKV(key);

	if ( !pKV )
		return false;

	*val = pKV->getValue();

	return true;
}

CRCBotKeyValue :: CRCBotKeyValue ( const char *szKey, char *szValue )
{
	strncpy(m_szKey,szKey,RCBOT_MAX_KV_LEN-1);
	m_szKey[RCBOT_MAX_KV_LEN-1] = 0;
	strncpy(m_szValue,szValue,RCBOT_MAX_KV_LEN-1);
	m_szValue[RCBOT_MAX_KV_LEN-1] = 0;
}