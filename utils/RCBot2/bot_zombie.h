#ifndef _BOT_ZOMBIE_H_
#define _BOT_ZOMBIE_H_

class CBotZombie : public CBot
{
	bool isEnemy ( edict_t *pEdict,bool bCheckWeapons = true );

	void modThink ( void );

	void getTasks (unsigned int iIgnore);
};

#endif