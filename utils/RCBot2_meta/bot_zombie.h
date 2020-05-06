#ifndef _BOT_ZOMBIE_H_
#define _BOT_ZOMBIE_H_

class CBotZombie : public CBot
{
	bool isEnemy(edict_t* pEdict, bool bCheckWeapons = true) override;

	void modThink(void) override;

	void getTasks(unsigned int iIgnore) override;
};

#endif