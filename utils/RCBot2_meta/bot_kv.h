#ifndef __RCBOT_KEY_VAL__
#define __RCBOT_KEY_VAL__

#define RCBOT_MAX_KV_LEN 256

#include <vector>

class CRCBotKeyValue
{
public:
	CRCBotKeyValue(const char *szKey, char *szValue);

	char *getKey ()
	{
		return m_szKey;
	}

	char *getValue ()
	{
		return m_szValue;
	}

private:
	char m_szKey[RCBOT_MAX_KV_LEN];
	char m_szValue[RCBOT_MAX_KV_LEN];
};

class CRCBotKeyValueList
{
public:
	~CRCBotKeyValueList();

	void parseFile ( FILE *fp );

	//unsigned int size ();

	//CRCBotKeyValue *getKV ( unsigned int iIndex );

	bool getInt ( const char *key, int *val );

	bool getString ( const char *key, char **val );

	bool getFloat ( const char *key, float *val );

private:

	CRCBotKeyValue *getKV ( const char *key );

	std::vector <CRCBotKeyValue*> m_KVs;
};

#endif