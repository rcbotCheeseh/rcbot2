// EXCLUDED
#ifndef __RCBOT_CONFIG_H__
#define __RCBOT_CONFIG_H__

class CRCBotConfig //: public CConfigFile 
{
public:
	void loadConfig ();
private:
	int m_iMaxBots = 0;
	int m_iMinBots = 0;
};

#endif
