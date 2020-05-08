#ifndef __RCBOT_PROFILE_H__
#define __RCBOT_PROFILE_H__

#include <vector>

class CBotProfile
{
public:
	CBotProfile()
	{
		memset(this, 0, sizeof(CBotProfile));
	}
	CBotProfile(CBotProfile& other);
	// setup profile
	CBotProfile(
		const char* szName,
		const char* szModel,
		int iTeam,
		int iVisionTicks,
		int iPathTicks,
		int iVisionTicksClients,
		int iSensitivity,
		float fBraveness,
		float fAimSkill,
		int iClass = 0);

	// bot's name
	char* m_szName;
	char* m_szModel;
	// bot's team
	int m_iTeam;				// preferred player team
	int m_iVisionTicks;			// speed of finding non players (npcs/teleporters etc)
	int m_iPathTicks;			// speed of finding a path
	int m_iClass;				// preferred player class
	int m_iVisionTicksClients;	// speed of finding other players and enemy players
	int m_iSensitivity;		// 1 to 20 sensitivity of bot's "mouse" (angle speed)
	float m_fBraveness;			// 0.0 to 1.0 sensitivity to danger (brave = less sensitive)
	float m_fAimSkill;			// 0.0 to 1.0 ability to predict players movements (aim skill)
};

class CBotProfiles
{
public:
	static void deleteProfiles();

	// find profiles and setup list
	static void setupProfiles();

	// return a profile unused by a bot
	static CBotProfile* getRandomFreeProfile();

	static CBotProfile* getDefaultProfile();

private:
	static std::vector <CBotProfile*> m_Profiles;
	static CBotProfile* m_pDefaultProfile;
};

#endif