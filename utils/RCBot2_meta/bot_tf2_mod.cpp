/*
 *    This file is part of RCBot.
 *
 *    RCBot by Paul Murphy adapted from Botman's HPB Bot 2 template.
 *
 *    RCBot is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    RCBot is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RCBot; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */
#include "engine_wrappers.h"

 //#include "server_class.h"

#include "bot.h"

#include "in_buttons.h"

#include "bot_mods.h"
#include "bot_globals.h"
#include "bot_fortress.h"
#include "bot_weapons.h"
#include "bot_configfile.h"
#include "bot_getprop.h"
#include "bot_navigator.h"
#include "bot_waypoint.h"
#include "bot_waypoint_locations.h"
#include "bot_perceptron.h"
#include "bot_tf2_points.h"
#include "bot_sigscan.h"

eTFMapType CTeamFortress2Mod::m_MapType = TF_MAP_CTF;
tf_tele_t CTeamFortress2Mod::m_Teleporters[MAX_PLAYERS];
int CTeamFortress2Mod::m_iArea = 0;
float CTeamFortress2Mod::m_fSetupTime = 0.0f;
float CTeamFortress2Mod::m_fRoundTime = 0.0f;
MyEHandle CTeamFortress2Mod::m_pFlagCarrierRed = MyEHandle(NULL);
MyEHandle CTeamFortress2Mod::m_pFlagCarrierBlue = MyEHandle(NULL);
float CTeamFortress2Mod::m_fArenaPointOpenTime = 0.0f;
float CTeamFortress2Mod::m_fPointTime = 0.0f;
tf_sentry_t CTeamFortress2Mod::m_SentryGuns[MAX_PLAYERS];	// used to let bots know if sentries have been sapped or not
tf_disp_t  CTeamFortress2Mod::m_Dispensers[MAX_PLAYERS];	// used to let bots know where friendly/enemy dispensers are
MyEHandle CTeamFortress2Mod::m_pResourceEntity = MyEHandle(NULL);
MyEHandle CTeamFortress2Mod::m_pGameRules = MyEHandle(NULL);
bool CTeamFortress2Mod::m_bAttackDefendMap = false;
int CTeamFortress2Mod::m_Cappers[MAX_CONTROL_POINTS];
int CTeamFortress2Mod::m_iCapDefenders[MAX_CONTROL_POINTS];
bool CTeamFortress2Mod::m_bHasRoundStarted = true;
bool CTeamFortress2Mod::m_bDontClearPoints = false;
int CTeamFortress2Mod::m_iFlagCarrierTeam = 0;
MyEHandle CTeamFortress2Mod::m_pBoss = MyEHandle(NULL);
bool CTeamFortress2Mod::m_bBossSummoned = false;
MyEHandle CTeamFortress2Mod::pMediGuns[MAX_PLAYERS];
CTFObjectiveResource CTeamFortress2Mod::m_ObjectiveResource = CTFObjectiveResource();
CTeamControlPointMaster* CTeamFortress2Mod::m_PointMaster = NULL;
CTeamRoundTimer CTeamFortress2Mod::m_Timer;
MyEHandle CTeamFortress2Mod::m_PointMasterResource = MyEHandle(NULL);
CTeamControlPointRound* CTeamFortress2Mod::m_pCurrentRound = NULL;
bool CTeamFortress2Mod::bFlagStateDefault = true;
MyEHandle CTeamFortress2Mod::m_pPayLoadBombBlue = MyEHandle(NULL);
MyEHandle CTeamFortress2Mod::m_pPayLoadBombRed = MyEHandle(NULL);
bool CTeamFortress2Mod::m_bRoundOver = false;
int CTeamFortress2Mod::m_iWinningTeam = 0;
int CTeamFortress2Mod::m_iLastWinningTeam = 0;
Vector CTeamFortress2Mod::m_vFlagLocationBlue = Vector(0, 0, 0);
Vector CTeamFortress2Mod::m_vFlagLocationRed = Vector(0, 0, 0);
bool CTeamFortress2Mod::m_bFlagLocationValidBlue = false;
bool CTeamFortress2Mod::m_bFlagLocationValidRed = false;
bool CTeamFortress2Mod::m_bMVMFlagStartValid = false;
Vector CTeamFortress2Mod::m_vMVMFlagStart = Vector(0, 0, 0);
bool CTeamFortress2Mod::m_bMVMCapturePointValid = false;
Vector CTeamFortress2Mod::m_vMVMCapturePoint = Vector(0, 0, 0);
bool CTeamFortress2Mod::m_bMVMAlarmSounded = false;
float CTeamFortress2Mod::m_fMVMCapturePointRadius = 0.0f;
int CTeamFortress2Mod::m_iCapturePointWptID = -1;
int CTeamFortress2Mod::m_iFlagPointWptID = -1;
vector<CTF2Loadout*> CTeamFortress2Mod::m_pLoadoutWeapons[TF2_SLOT_MAX][9];
MyEHandle CTeamFortress2Mod::m_pNearestTankBoss = NULL;
float CTeamFortress2Mod::m_fNearestTankDistance = 0.0f;
Vector CTeamFortress2Mod::m_vNearestTankLocation = Vector(0, 0, 0);
vector<CAttributeID*> CAttributeLookup::attributes;
extern ConVar bot_use_disp_dist;

int UTIL_FindAttributeID(vector<CAttributeID*>* list, const char* name)
{
	unsigned int i;

	for (i = 0; i < list->size(); i++)
	{
		CAttributeID* attrib = list->at(i);

		if (attrib && (strcmp(attrib->m_attribute, name) == 0))
		{
			return attrib->m_id;
		}
	}

	return -1;
}

CTF2Loadout::CTF2Loadout(const char* pszClassname, const int iIndex, const int iQuality, const int iMinLevel,
	const int iMaxLevel)
{
	m_pszClassname = CStrings::getString(pszClassname);
	m_iIndex = iIndex;
	m_iQuality = iQuality;
	m_iMinLevel = iMinLevel;
	m_iMaxLevel = iMaxLevel;
	m_bCanBeUsedInMedieval = false;
}
void CSCICopy(CEconItemView* olditem, CEconItemView* newitem)
{
	memset(newitem, 0, sizeof(CEconItemView));

	//#define copymember(a) newitem->a = olditem->a
#define copymember(a) memcpy(&newitem->a, &olditem->a, sizeof(newitem->a));

	copymember(m_pVTable);

	copymember(m_iItemDefinitionIndex);

	copymember(m_iEntityQuality);
	copymember(m_iEntityLevel);

	copymember(m_iItemID);
	copymember(m_iItemIDHigh);
	copymember(m_iItemIDLow);
	copymember(m_iAccountID);
	copymember(m_iInventoryPosition);

	copymember(m_ItemHandle);

	copymember(m_bColorInit);
	copymember(m_bPaintOverrideInit);
	copymember(m_bHasPaintOverride);

	copymember(m_flOverrideIndex);
	copymember(m_unRGB);
	copymember(m_unAltRGB);

	copymember(m_iTeamNumber);

	copymember(m_bInitialized);

	// copy ctor so the CUtlVector is copied correctly
	newitem->m_AttributeList = olditem->m_AttributeList;
	newitem->m_NetworkedDynamicAttributesForDemos = olditem->m_NetworkedDynamicAttributesForDemos;

	copymember(m_bDoNotIterateStaticAttributes);

	/*
	META_CONPRINTF("Copying attributes...\n");
	int nCount = olditem->m_Attributes.Count();
	META_CONPRINTF("Count: %d\n", nCount);
	newitem->m_Attributes.SetSize( nCount );
	for ( int i = 0; i < nCount; i++ )
	{
	META_CONPRINTF("Copying %d...\n", i+1);
	newitem->m_Attributes[ i ] = olditem->m_Attributes[ i ];
	}
	*/
}

void CAttributeLookup::addAttribute(const char* pszAttrib, const int id)
{
	attributes.push_back(new CAttributeID(id, pszAttrib));
}

void CAttributeLookup::freeMemory()
{
	for (unsigned int i = 0; i < attributes.size(); i++)
	{
		CAttributeID* attrib = attributes[i];

		delete attrib;

		attributes[i] = NULL;
	}
}

int CAttributeLookup::findAttributeID(const char* name)
{
	unsigned int i;

	for (i = 0; i < attributes.size(); i++)
	{
		CAttributeID* attrib = attributes.at(i);

		if (attrib && (strcmp(attrib->m_attribute, name) == 0))
		{
			return attrib->m_id;
		}
	}

	return -1;
}

/*
int TF2II_GetItemLevelByID(int iItemDefinitionIndex)
{
	int iLevel = 1;
	//new iSlot = TF2II_GetItemSlotByID(iItemDefinitionIndex);
	//if (iSlot >= TF2ItemSlot_Hat && iSlot <= TF2ItemSlot_Misc)
	//	iLevel = GetRandomInt(1, 100);
	switch (iItemDefinitionIndex)
	{
	case 125;	262; 335, 336, 360, 434, 435: iLevel = 1;
	case 164, 343: iLevel = 5;
	case 115, 116, 126, 165, 240, 261, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 263, 279, 346, 347, 486, 408, 409, 410, 470, 473, 490, 491, 492, 514, 515, 516, 517, 518, 519, 520, 537: iLevel = 10;
	case 422: iLevel = 13;
	case 166, 333, 392, 443, 483, 484: iLevel = 15;
	case 170, 189, 295, 299, 296, 345, 420, 432, 454, 1899: iLevel = 20;
	case 334: iLevel = 28;
	case 332: iLevel = 30;
	case 278, 287, 289, 290, 291: iLevel = 31;
	case 292, 471: iLevel = 50;
	}
	return iLevel;
}

*/
/*
void CTF2Loadout::getScript ( CEconItemView *cscript )
{
	static CEconItemAttribute m_Attributes[16];
	extern ConVar rcbot_enable_attributes;

	cscript->m_iEntityLevel = m_iLevel;
	cscript->m_iEntityQuality = m_iQuality;
	cscript->m_iItemDefinitionIndex = m_iIndex;
	cscript->m_bInitialized = true;

	if ( rcbot_enable_attributes.GetBool() )
	{
		unsigned int size = copyAttributesIntoArray(m_Attributes,cscript->m_pVTable_Attributes);

		if ( size > 0 )
		{
			cscript->m_Attributes.CopyArray(m_Attributes,size);

			cscript->m_bDoNotIterateStaticAttributes = true;

			if ( cscript->m_iEntityQuality == 0 )
				cscript->m_iEntityQuality = 6;
		}
	}
}*/
/*
const char *CTF2Loadout :: getScript ( CEconItemView *script )
{
	script->m_iEntityLevel = m_iLevel;
	script->m_iItemDefinitionIndex = m_iIndex;
	script->m_iEntityQuality = m_iQuality;
	script->m_bInitialized = true;

	return m_pszClassname;
}*/

CTF2Loadout* CTeamFortress2Mod::getRandomHat(const int iClass)
{
	int iSize;

	if ((iSize = m_pLoadoutWeapons[TF2_SLOT_HAT][iClass].size()) > 0)
	{
		return m_pLoadoutWeapons[TF2_SLOT_HAT][iClass].at(randomInt(0, iSize - 1));
	}

	return NULL;
}

bool CTeamFortress2Mod::isSuddenDeath()
{
	extern ConVar* mp_stalemate_enable;

	// Bot weapon Randomizer -- leonardo
	if (!mp_stalemate_enable || !mp_stalemate_enable->GetBool() || isMapType(TF_MAP_ARENA))
		return false;

	void* pGameRules = GetGameRules();

	if (pGameRules)
	{
		const int iRoundState = CClassInterface::TF2_getRoundState(pGameRules);

		return iRoundState == RoundState_Stalemate;
	}

	return false;
}

#define CHECK_PREFAB_INT(val) ((prefab==NULL)||(prefab->GetInt(val,-1)==-1))?kv->GetInt(val):prefab->GetInt(val)
#define CHECK_PREFAB_STRING(val) ((prefab==NULL)||(prefab->GetString(val,NULL)==NULL))?kv->GetString(val):prefab->GetString(val)
#define CHECK_PREFAB_FINDKEY(keyname) ((prefab==NULL)||(prefab->FindKey(keyname)==NULL))?kv->FindKey(keyname):prefab->FindKey(keyname)

void CTeamFortress2Mod::setupLoadOutWeapons()
{
	extern IFileSystem* filesystem;
	KeyValues* mainkv = new KeyValues("tfweapons");//A possible memory leak? [APG]RoboCop[CL]

	KeyValues* kv;
	KeyValues* usedbyclass;
	KeyValues* attribs;
	KeyValues* prefabs;
	KeyValues* prefab;

	CBotGlobals::botMessage(NULL, 0, "Reading Weapon Loadout details from TF MOD...");
	mainkv->LoadFromFile(filesystem, "scripts/items/items_game.txt", "MOD");

	kv = mainkv->FindKey("items");
	prefabs = mainkv->FindKey("prefabs");

	attribs = mainkv->FindKey("attributes");

	if (attribs)
	{
		attribs = attribs->FindKey("1");

		if (attribs)
		{
			do
			{
				int id;

				const char* keyname = attribs->GetName();

				const char* attrib_name;

				if (keyname && *keyname)
				{
					id = atoi(keyname);

					attrib_name = attribs->GetString("name");

					CAttributeLookup::addAttribute(CStrings::getString(attrib_name), id);
				}
			} while ((attribs = attribs->GetNextTrueSubKey()) != NULL);
		}
	}

	if (kv)
	{
		kv = kv->FindKey("default");

		if (kv)
		{
			while ((kv = kv->GetNextTrueSubKey()) != NULL)
			{
				int iclass = 0;

				CTF2Loadout* added = NULL;

				const char* szPrefab = kv->GetString("prefab");
				prefab = NULL;

				if ((strncmp(szPrefab, "valve", 5) == 0) && (strlen(szPrefab) > 6))
				{
					prefab = prefabs->FindKey(&szPrefab[6]);

					if (prefab == NULL)
					{
						// search prefabs for "valve"
					}
				}
				// NULL used in arithmetic in *szPrefab - [APG]RoboCop[CL]
				else if ((szPrefab != NULL) && (*szPrefab != NULL))
				{
					prefab = prefabs->FindKey(szPrefab);
				}

				const char* szKeyName = kv->GetName();

				int iindex;

				if (szKeyName && *szKeyName)
				{
					if ((*szKeyName >= '0') && (*szKeyName <= '9'))
						iindex = atoi(szKeyName);
					else
						continue; // not a number
				}
				else
					continue;

				const char* pszquality;
				int iquality = 0;
				int iminlevel = 1;
				int imaxlevel = 1;
				bool bIsWeapon = false;
				int islot = 0;

				// These two demoman weapons seem to cause problems
				// skip them
				if (iindex == 1151)
					continue;
				if (iindex == 1150)
					continue;

				KeyValues* loadout;

				if (prefab != NULL)
					loadout = prefab;
				else
					loadout = kv;

				const char* classname = CHECK_PREFAB_STRING("item_class");//loadout->GetString("item_class");

				if ((classname == NULL) || (*classname == 0))
					continue;

				const char* slot = CHECK_PREFAB_STRING("item_slot"); //loadout->GetString("item_slot");

				if ((slot == NULL) || (*slot == 0))
					continue;

				usedbyclass = CHECK_PREFAB_FINDKEY("used_by_classes");// loadout->FindKey("used_by_classes");

				if (usedbyclass)
				{
					if (usedbyclass->FindKey("scout") && ((usedbyclass->GetInt("scout", 0) == 1) || (strcmp(usedbyclass->GetString("scout"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_SCOUT);
					if (usedbyclass->FindKey("sniper") && ((usedbyclass->GetInt("sniper", 0) == 1) || (strcmp(usedbyclass->GetString("sniper"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_SNIPER);
					if (usedbyclass->FindKey("soldier") && ((usedbyclass->GetInt("soldier", 0) == 1) || (strcmp(usedbyclass->GetString("soldier"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_SOLDIER);
					if (usedbyclass->FindKey("demoman") && ((usedbyclass->GetInt("demoman", 0) == 1) || (strcmp(usedbyclass->GetString("demoman"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_DEMOMAN);
					if (usedbyclass->FindKey("medic") && ((usedbyclass->GetInt("medic", 0) == 1) || (strcmp(usedbyclass->GetString("medic"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_MEDIC);;
					if (usedbyclass->FindKey("heavy") && ((usedbyclass->GetInt("heavy", 0) == 1) || (strcmp(usedbyclass->GetString("heavy"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_HWGUY);
					if (usedbyclass->FindKey("pyro") && ((usedbyclass->GetInt("pyro", 0) == 1) || (strcmp(usedbyclass->GetString("pyro"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_PYRO);
					if (usedbyclass->FindKey("spy") && ((usedbyclass->GetInt("spy", 0) == 1) || (strcmp(usedbyclass->GetString("spy"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_SPY);
					if (usedbyclass->FindKey("engineer") && ((usedbyclass->GetInt("engineer", 0) == 1) || (strcmp(usedbyclass->GetString("engineer"), loadout->GetString("item_slot")) == 0)))
						iclass |= (1 << TF_CLASS_ENGINEER);
				}

				if (iclass == 0)
					continue;

				pszquality = CHECK_PREFAB_STRING("item_quality");

				if (pszquality && *pszquality)
					iquality = (strcmp(pszquality, "unique") == 0) ? 6 : 0;

				iminlevel = CHECK_PREFAB_INT("min_ilevel");
				imaxlevel = CHECK_PREFAB_INT("max_ilevel");

				if (strcmp(classname, "tf_wearable") == 0)
				{
					islot = 0;

					// hat
					if (strcmp(slot, "misc") == 0)
					{
						islot = TF2_SLOT_MISC;
					}
					else if (strcmp(slot, "head") == 0)
					{
						islot = TF2_SLOT_HAT;
					}
					if (islot != 0)
					{
						added = new CTF2Loadout(classname, iindex, iquality, iminlevel, imaxlevel);

						// find which hats are available for which class and add them
						for (int i = 1; i <= 9; i++)
						{
							if ((iclass & (1 << i)) > 0)
								m_pLoadoutWeapons[islot][i - 1].push_back(added);
						}

						CBotGlobals::botMessage(NULL, 0, "FOUND hat : %s", loadout->GetString("name"));
					}
				}

				if (added == NULL)
				{
					CWeapon* pWeapon = CWeapons::getWeapon(classname);
					const char* craft_class;

					craft_class = CHECK_PREFAB_STRING("craft_class");

					// check if bots can use this weapon
					if (pWeapon == NULL)
					{
						if (!strcmp(craft_class, "weapon"))
						{
							CBotGlobals::botMessage(NULL, 1, "Weapon not in weapons ini \"%s\"", classname);
						}

						continue;
					}

					if (strcmp(slot, "primary") == 0)
						islot = TF2_SLOT_PRMRY;
					else if (strcmp(slot, "melee") == 0)
						islot = TF2_SLOT_MELEE;
					else if (strcmp(slot, "secondary") == 0)
						islot = TF2_SLOT_SCNDR;
					else
						continue;

					if (!craft_class || !*craft_class)
						continue;

					if (strcmp(craft_class, "weapon"))
						continue;

					if (pWeapon->getSlot() != islot)
					{
						CBotGlobals::botMessage(NULL, 1, "Weapon Slot mismatch for %s - set as %d -- should be %d",
							pWeapon->getWeaponName(), pWeapon->getSlot(), islot);
						continue;
					}

					/*if ( ((iclass == TF_CLASS_ENGINEER) && (islot == TF2_SLOT_MELEE)) || (strcmp(classname,"tf_weapon_wrench")==0) )
					{
					// don't add any wrenches -- may cause bots unable to build sentries
					continue;
					}*/

					CBotGlobals::botMessage(NULL, 0, "FOUND loadout : %s", loadout->GetString("name"));

					//attributes
					added = new CTF2Loadout(classname, iindex, iquality, iminlevel, imaxlevel);

					for (int i = 1; i <= 9; i++)
					{
						if ((iclass & (1 << i)) > 0)
							m_pLoadoutWeapons[islot][i - 1].push_back(added);
					}

					//m_pLoadoutWeapons[islot][iclass-1].push_back(added);
					bIsWeapon = true;
				}

				if (added)
				{
					//if ( prefab != NULL )
					//	loadout = kv;

					attribs = CHECK_PREFAB_FINDKEY("attributes");

					if (attribs)
					{
						attribs = attribs->GetFirstSubKey();

						if (attribs)
						{
							do
							{
								const char* attribname = attribs->GetName();
								float fval = attribs->GetFloat("value");

								if (attribname && *attribname)
								{
									int iId = CAttributeLookup::findAttributeID(attribname);

									if (bIsWeapon && (strcmp(attribname, "damage penalty") == 0))
									{
										// don't add any weapons that don't cause damage e.g. rocket jumper
										if (fval == 0)
										{
											// delete
											for (int i = 1; i <= 9; i++)
											{
												if ((iclass & (1 << i)) > 0)
													m_pLoadoutWeapons[islot][i - 1].pop_back();
											}

											//	m_pLoadoutWeapons[islot][iclass-1].pop_back();

											delete added; // free

											attribs = NULL; // break loop

											iId = -1; // don't add
										}
									}
									else if (strcmp(attribname, "allowed in medieval mode") == 0)
									{
										added->m_bCanBeUsedInMedieval = true;
									}

									if (iId != -1)
										added->addAttribute(iId, fval);
								}
							} while (attribs && ((attribs = attribs->GetNextTrueSubKey()) != NULL));
						}
					}
				}
			}
		}
	}

	mainkv->deleteThis();
	/*
	m_pLoadoutWeapons[TF2_SLOT_PRMRY][TF_CLASS_PYRO].push_back(new CTF2LoadoutWeapon("tf_weapon_flamethrower",40,6,10,"170 ; 2.5 ; 24 ; 1.0 ; 28 ; 0.0",200));
	m_pLoadoutWeapons[TF2_SLOT_SCNDR][TF_CLASS_PYRO].push_back(new CTF2LoadoutWeapon("tf_weapon_flaregun",39,6,10,"25 ; 0.5",16));
	m_pLoadoutWeapons[TF2_SLOT_PRMRY][TF_CLASS_SOLDIER].push_back(new CTF2LoadoutWeapon("tf_weapon_rocketlauncher_directhit",127,6,1,"100 ; 0.3 ; 103 ; 1.8 ; 2 ; 1.25 ; 114 ; 1.0 ; 328 ; 1.0",20));
	*/
}

CTF2Loadout* CTeamFortress2Mod::findRandomWeaponLoadOutInSlot(int iclass, int islot)
{
	vector<CTF2Loadout*>* list = &(m_pLoadoutWeapons[islot][iclass - 1]);
	CTF2Loadout* weap = NULL;

	if (list->size() == 0)
		return NULL;

	if (((islot == TF2_SLOT_PRMRY) || (islot == TF2_SLOT_SCNDR)) && CTeamFortress2Mod::isMedievalMode())
	{
		vector<CTF2Loadout*> newlist;
		unsigned int _size = list->size();

		// add all medieval weapons
		for (unsigned i = 0; i < _size; i++)
		{
			CTF2Loadout* temp = list->at(i);

			if (temp->m_bCanBeUsedInMedieval)
			{
				newlist.push_back(temp);
			}
		}

		if (newlist.size() > 0)
		{
			weap = newlist.at(randomInt(0, newlist.size() - 1));
		}
	}
	else
		weap = list->at(randomInt(0, list->size() - 1));

	return weap;
}

bool CTeamFortress2Mod::isMedievalMode()
{
	return CClassInterface::TF2_IsMedievalMode(GetGameRules());
}
/*
CTF2Loadout *CTeamFortress2Mod::findRandomWeaponLoadOut ( int iclass, const char *classname )
{
	// find weapon slot
	int islot;
	// this is only used to find the slot
	CWeapon *pWeapon = CWeapons::getWeapon(classname);//::getWeapon(classname);

	if ( pWeapon == NULL )
		return NULL;

	islot = pWeapon->getSlot();

	if ( islot == TF2_SLOT_SHOTGUN )
	{
		// find the slot for this shotgun
		if ( iclass == TF_CLASS_ENGINEER )
			islot = TF2_SLOT_PRMRY;
		else
			islot = TF2_SLOT_SCNDR;
	}

	return findRandomWeaponLoadOutInSlot(iclass, islot);
}
*/
bool CTeamFortress2Mod::checkWaypointForTeam(CWaypoint* pWpt, int iTeam)
{
	// Returns true if team can go to waypoint
	return m_bRoundOver || ((!pWpt->hasFlag(CWaypointTypes::W_FL_NOBLU) || (iTeam != TF2_TEAM_BLUE)) && (!pWpt->
		hasFlag(CWaypointTypes::W_FL_NORED) || (iTeam != TF2_TEAM_RED)));
}

bool CTeamFortress2Mod::isWaypointAreaValid(const int iWptArea, const int iWptFlags)
{
	return CTeamFortress2Mod::m_ObjectiveResource.isWaypointAreaValid(iWptArea, iWptFlags);
}
///////////////////////////
bool CTeamFortress2Mod::withinEndOfRound(float fTime)
{
	if (m_Timer.m_flTimerEndTime)
		return (gpGlobals->curtime > (*m_Timer.m_flTimerEndTime - fTime));

	return false;
}

void CTeamFortress2Mod::getTeamOnlyWaypointFlags(int iTeam, int* iOn, int* iOff)
{
	if (iTeam == TF2_TEAM_BLUE)
	{
		*iOn = CWaypointTypes::W_FL_NORED;
		*iOff = CWaypointTypes::W_FL_NOBLU;
	}
	else if (iTeam == TF2_TEAM_RED)
	{
		*iOn = CWaypointTypes::W_FL_NOBLU;
		*iOff = CWaypointTypes::W_FL_NORED;
	}
}

void CTeamFortress2Mod::modFrame()
{
	if (m_bPlayerHasSpawned)
	{
		if (m_ObjectiveResource.m_ObjectiveResource == NULL)
		{
			m_ObjectiveResource.m_ObjectiveResource = CClassInterface::FindEntityByNetClass(gpGlobals->maxClients + 1, "CTFObjectiveResource");

			if (m_ObjectiveResource.m_ObjectiveResource.get() != NULL)
				m_ObjectiveResource.setup();
		}
		else
			CTeamFortress2Mod::m_ObjectiveResource.think();
		/*
		if (m_pGameRules.get() == NULL)
		{
			//m_pGameRules = CClassInterface::FindEntityByClassnameNearest(Vector(0, 0, 0), "tf_gamerules_data", 99999.0f, NULL);
			m_pGameRules = CClassInterface::FindEntityByNetClass(gpGlobals->maxClients + 1, "CTFGameRulesProxy");

			if (m_pGameRules.get())
			{
				const char *classname = m_pGameRules.get()->GetClassName();

				CBotGlobals::botMessage(NULL, 0, "Found gamerules %s", classname);
			}
		}*/
	}
}

void CTeamFortress2Mod::initMod()
{
	//	unsigned int i;
		// Setup Weapons

	CBots::controlBotSetup(true);

	CWeapons::loadWeapons((m_szWeaponListName == NULL) ? "TF2" : m_szWeaponListName, TF2Weaps);
	//CWeapons::loadWeapons("TF2", TF2Weaps);
	/*
	i = 0;
	while ( TF2Weaps[i].szWeaponName[0] != '\0' )  //for ( i = 0; i < TF2_WEAPON_MAX; i ++ )
		CWeapons::addWeapon(new CWeapon(TF2Weaps[i++]));//.iSlot,TF2Weaps[i].szWeaponName,TF2Weaps[i].iId,TF2Weaps[i].m_iFlags,TF2Weaps[i].m_iAmmoIndex,TF2Weaps[i].minPrimDist,TF2Weaps[i].maxPrimDist,TF2Weaps[i].m_iPreference,TF2Weaps[i].m_fProjSpeed));
		*/
	CRCBotTF2UtilFile::loadConfig();

	setupLoadOutWeapons();
	//memset(g_fBotUtilityPerturb,0,sizeof(float)*TF_CLASS_MAX*BOT_UTIL_MAX);
}

void CTeamFortress2Mod::mapInit()
{
	CBotMod::mapInit();

	m_vNearestTankLocation = Vector(0, 0, 0);

	unsigned int i = 0;
	string_t mapname = gpGlobals->mapname;

	const char* szmapname = mapname.ToCStr();

	m_iLastWinningTeam = 0;
	m_iWinningTeam = 0;
	m_pResourceEntity = NULL;
	m_ObjectiveResource.m_ObjectiveResource = NULL;
	m_ObjectiveResource.reset();
	m_PointMaster = NULL;
	m_PointMasterResource = NULL;
	m_pCurrentRound = NULL;
	m_Timer = CTeamRoundTimer();
	bFlagStateDefault = true;
	m_bFlagLocationValidBlue = false;
	m_bFlagLocationValidRed = false;
	m_bMVMAlarmSounded = false;
	m_bMVMFlagStartValid = false;
	m_bMVMCapturePointValid = false;
	m_fMVMCapturePointRadius = 0.0f;
	m_iCapturePointWptID = -1;
	m_iFlagPointWptID = -1;

	if (strncmp(szmapname, "ctf_", 4) == 0)
		m_MapType = TF_MAP_CTF; // capture the flag
	else if (strncmp(szmapname, "cp_", 3) == 0)
		m_MapType = TF_MAP_CP; // control point
	else if (strncmp(szmapname, "tc_", 3) == 0)
		m_MapType = TF_MAP_TC; // territory control
	else if (strncmp(szmapname, "pl_", 3) == 0)
		m_MapType = TF_MAP_CART; // pipeline
	else if (strncmp(szmapname, "plr_", 4) == 0)
		m_MapType = TF_MAP_CARTRACE; // pipeline racing
	else if (strncmp(szmapname, "arena_", 6) == 0 || strncmp(szmapname, "vsh_", 4) == 0) // pongo1321
		m_MapType = TF_MAP_ARENA; // arena mode (also fallback for VS Saxton Hale gamemode)
	else if (strncmp(szmapname, "arena_", 6) == 0)
		m_MapType = TF_MAP_ARENA; // arena mode
	else if (strncmp(szmapname, "koth_", 5) == 0)
		m_MapType = TF_MAP_KOTH; // king of the hill
	else if (strncmp(szmapname, "sd_", 3) == 0)
		m_MapType = TF_MAP_SD; // special delivery
	else if (strncmp(szmapname, "tr_", 3) == 0)
		m_MapType = TF_MAP_TR; // training mode
	else if (strncmp(szmapname, "mvm_", 4) == 0)
		m_MapType = TF_MAP_MVM; // mann vs machine
	else if (strncmp(szmapname, "rd_", 3) == 0)
		m_MapType = TF_MAP_RD; // robot destruction
	else
		m_MapType = TF_MAP_DM;

	m_iArea = 0;

	m_fSetupTime = 5.0f; // 5 seconds normal

	m_fRoundTime = 0.0f;

	m_pFlagCarrierRed = NULL;
	m_pFlagCarrierBlue = NULL;
	m_iFlagCarrierTeam = 0;
	m_bDontClearPoints = false;

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		m_Teleporters[i].m_iWaypoint = -1;
		m_Teleporters[i].m_fLastTeleported = 0.0f;
		m_Teleporters[i].entrance = MyEHandle(NULL);
		m_Teleporters[i].exit = MyEHandle(NULL);
		m_Teleporters[i].sapper = MyEHandle(NULL);
		m_SentryGuns[i].sapper = MyEHandle(NULL);
		m_SentryGuns[i].sentry = MyEHandle(NULL);
		m_Dispensers[i].sapper = MyEHandle(NULL);
		m_Dispensers[i].disp = MyEHandle(NULL);
		pMediGuns[i] = NULL;
	}

	m_bAttackDefendMap = false;
	m_pBoss = NULL;
	m_bBossSummoned = false;

	resetCappers();
	resetDefenders();
	//CPoints::loadMapScript();
}

int CTeamFortress2Mod::getTeleporterWaypoint(edict_t* pTele)
{
	int i;

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if (m_Teleporters[i].exit.get() == pTele)
			return m_Teleporters[i].m_iWaypoint;
	}

	return -1;
}

// Naris @ AlliedModders .net

bool CTeamFortress2Mod::TF2_IsPlayerZoomed(edict_t* pPlayer)
{
	int pcond = CClassInterface::getTF2Conditions(pPlayer);
	return ((pcond & TF2_PLAYER_ZOOMED) == TF2_PLAYER_ZOOMED);
}

bool CTeamFortress2Mod::TF2_IsPlayerSlowed(edict_t* pPlayer)
{
	int pcond = CClassInterface::getTF2Conditions(pPlayer);
	return ((pcond & TF2_PLAYER_SLOWED) == TF2_PLAYER_SLOWED);
}

bool CTeamFortress2Mod::TF2_IsPlayerDisguised(edict_t* pPlayer)
{
	int pcond = CClassInterface::getTF2Conditions(pPlayer);
	return ((pcond & TF2_PLAYER_DISGUISED) == TF2_PLAYER_DISGUISED);
}

bool CTeamFortress2Mod::TF2_IsPlayerTaunting(edict_t* pPlayer)
{
	int pcond = CClassInterface::getTF2Conditions(pPlayer);
	return ((pcond & TF2_PLAYER_TAUNTING) == TF2_PLAYER_TAUNTING);
}

bool CTeamFortress2Mod::TF2_IsPlayerCloaked(edict_t* pPlayer)
{
	int pcond = CClassInterface::getTF2Conditions(pPlayer);
	return ((pcond & TF2_PLAYER_CLOAKED) == TF2_PLAYER_CLOAKED);
}

bool CTeamFortress2Mod::TF2_IsPlayerKrits(edict_t* pPlayer)
{
	int pcond = CClassInterface::getTF2Conditions(pPlayer);
	return ((pcond & TF2_PLAYER_KRITS) == TF2_PLAYER_KRITS);

	return false;
}

bool CTeamFortress2Mod::TF2_IsPlayerInvuln(edict_t* pPlayer)
{
	if (CBotGlobals::isPlayer(pPlayer))
	{
		int pcond = CClassInterface::getTF2Conditions(pPlayer);
		return ((pcond & TF2_PLAYER_INVULN) == TF2_PLAYER_INVULN);
	}

	return false;
}

bool CTeamFortress2Mod::TF2_IsPlayerOnFire(edict_t* pPlayer)
{
	int pcond = CClassInterface::getTF2Conditions(pPlayer);
	return ((pcond & TF2_PLAYER_ONFIRE) == TF2_PLAYER_ONFIRE);
}

int CTeamFortress2Mod::numClassOnTeam(int iTeam, int iClass)
{
	int i = 0;
	int num = 0;
	edict_t* pEdict;

	for (i = 1; i <= CBotGlobals::numClients(); i++)
	{
		pEdict = INDEXENT(i);

		if (CBotGlobals::entityIsValid(pEdict))
		{
			if (getTeam(pEdict) == iTeam)
			{
				if (CClassInterface::getTF2Class(pEdict) == iClass)
					num++;
			}
		}
	}

	return num;
}

edict_t* CTeamFortress2Mod::findResourceEntity()
{
	if (!m_pResourceEntity) // crash fix
		m_pResourceEntity = CClassInterface::FindEntityByNetClass(gpGlobals->maxClients + 1, "CTFPlayerResource");

	return m_pResourceEntity;
}

TF_Class CTeamFortress2Mod::getSpyDisguise(edict_t* pPlayer)
{
	static int iClass;
	static int iTeam;
	static int iIndex;
	static int iHealth;

	CClassInterface::getTF2SpyDisguised(pPlayer, &iClass, &iTeam, &iIndex, &iHealth);

	return (TF_Class)iClass;
}

float CTeamFortress2Mod::TF2_GetClassSpeed(int iClass)
{
	switch (iClass)
	{
	case TF_CLASS_SCOUT: return 133.0f;
	case TF_CLASS_SOLDIER: return 80.0f;
	case TF_CLASS_DEMOMAN: return 93.00;
	case TF_CLASS_MEDIC: return 109.0f;
	case TF_CLASS_PYRO: return 100.0f;
	case TF_CLASS_SPY: return 109.0f;
	case TF_CLASS_ENGINEER: return 100.0f;
	case TF_CLASS_SNIPER: return 100.0f;
	case TF_CLASS_HWGUY: return 77.0f;
	}
	return 0.0;
}

float CTeamFortress2Mod::TF2_GetPlayerSpeed(edict_t* pPlayer, TF_Class iClass)
{
	static float fSpeed;

	fSpeed = CClassInterface::getMaxSpeed(pPlayer);// * CClassInterface::getSpeedFactor(pPlayer);

	if (fSpeed == 0)
	{
		if (TF2_IsPlayerSlowed(pPlayer))
			return 30.0;
		else
			return TF2_GetClassSpeed(iClass) * 1.58;
	}

	return fSpeed;
}

int CTeamFortress2Mod::getTeam(edict_t* pEntity)
{
	return CClassInterface::getTeam(pEntity);
	//return *((int*)(pEntity->GetIServerEntity()->GetBaseEntity())+110);
}

int CTeamFortress2Mod::getSentryLevel(edict_t* pSentry)
{
	string_t model = pSentry->GetIServerEntity()->GetModelName();
	const char* szmodel = model.ToCStr();

	return (szmodel[24] - '1') + 1;
	//if ( pSentry && pSentry->
}

int CTeamFortress2Mod::getDispenserLevel(edict_t* pDispenser)
{
	const string_t model = pDispenser->GetIServerEntity()->GetModelName();
	const char* szmodel = model.ToCStr();

	if (strcmp(szmodel, "models/buildables/dispenser_light.mdl") == 0)
		return 1;

	return (szmodel[31] - '1') + 1;
	//if ( pSentry && pSentry->
}

int CTeamFortress2Mod::getEnemyTeam(int iTeam)
{
	return (iTeam == TF2_TEAM_BLUE) ? TF2_TEAM_RED : TF2_TEAM_BLUE;
}

/*
------------------int : m_nDisguiseTeam
------------------int : m_nDisguiseClass
------------------int : m_iDisguiseTargetIndex
------------------int : m_iDisguiseHealth

*/

bool CTeamFortress2Mod::isDispenser(edict_t* pEntity, const int iTeam, const bool checkcarrying)
{
	return (!iTeam || (iTeam == getTeam(pEntity))) && (strcmp(pEntity->GetClassName(), "obj_dispenser") == 0) && (checkcarrying || !CClassInterface::isSentryGunBeingPlaced(pEntity));
}

bool CTeamFortress2Mod::isFlag(edict_t* pEntity, int iTeam)
{
	return (!iTeam || (getEnemyTeam(iTeam) == getTeam(pEntity))) && (strcmp(pEntity->GetClassName(), "item_teamflag") == 0);
}

bool CTeamFortress2Mod::isBoss(edict_t* pEntity, float* fFactor)
{
	if (m_bBossSummoned)
	{
		if (m_pBoss.get() && CBotGlobals::entityIsAlive(m_pBoss.get()))
			return m_pBoss.get() == pEntity;
		else if ((strcmp(pEntity->GetClassName(), "merasmus") == 0) ||
			(strcmp(pEntity->GetClassName(), "headless_hatman") == 0) ||
			(strcmp(pEntity->GetClassName(), "eyeball_boss") == 0) ||
			(strcmp(pEntity->GetClassName(), "tf_zombie") == 0))
		{
			m_pBoss = pEntity;
			return true;
		}
	}
	else if (CTeamFortress2Mod::isMapType(TF_MAP_CARTRACE))
	{
		if (m_pBoss.get() == pEntity)
			return true;
		// for plr_hightower_event summon event is not called! Boo tf2!!!
		else if (strcmp(pEntity->GetClassName(), "tf_zombie") == 0)
		{
			m_pBoss = pEntity;
			return true;
		}
	}
	else if (CTeamFortress2Mod::isMapType(TF_MAP_MVM))
	{
		if (m_pBoss.get() == pEntity)
			return true;
		else if (isTankBoss(pEntity))
		{
			if (fFactor != NULL)
				*fFactor = 200.0f;

			m_pBoss = pEntity;
			return true;
		}
	}

	return false;
}

void CTeamFortress2Mod::updateTeleportTime(edict_t* pOwner)
{
	m_Teleporters[ENTINDEX(pOwner) - 1].m_fLastTeleported = engine->Time();
}

float CTeamFortress2Mod::getTeleportTime(edict_t* pOwner)
{
	return m_Teleporters[ENTINDEX(pOwner) - 1].m_fLastTeleported;
}

bool CTeamFortress2Mod::isSentry(edict_t* pEntity, int iTeam, const bool checkcarrying)
{
	return (!iTeam || (iTeam == getTeam(pEntity))) && (strcmp(pEntity->GetClassName(), "obj_sentrygun") == 0) && (checkcarrying || !CClassInterface::isSentryGunBeingPlaced(pEntity));
}

bool CTeamFortress2Mod::isTankBoss(edict_t* pEntity)
{
	return (strcmp(pEntity->GetClassName(), "tank_boss") == 0);
}

bool CTeamFortress2Mod::isTeleporter(edict_t* pEntity, int iTeam, const bool checkcarrying)
{
	return (!iTeam || (iTeam == getTeam(pEntity))) && (strcmp(pEntity->GetClassName(), "obj_teleporter") == 0) && (checkcarrying || !CClassInterface::isSentryGunBeingPlaced(pEntity));
}

bool CTeamFortress2Mod::isTeleporterEntrance(edict_t* pEntity, int iTeam, const bool checkcarrying)
{
	return isTeleporter(pEntity, iTeam) && CClassInterface::isTeleporterMode(pEntity, TELE_ENTRANCE) && (checkcarrying || !CClassInterface::isSentryGunBeingPlaced(pEntity));
}

bool CTeamFortress2Mod::isTeleporterExit(edict_t* pEntity, int iTeam, const bool checkcarrying)
{
	return isTeleporter(pEntity, iTeam) && CClassInterface::isTeleporterMode(pEntity, TELE_EXIT) && (checkcarrying || !CClassInterface::isSentryGunBeingPlaced(pEntity));
}

bool CTeamFortress2Mod::isPipeBomb(edict_t* pEntity, int iTeam)
{
	return (!iTeam || (iTeam == getTeam(pEntity))) && (strcmp(pEntity->GetClassName(), "tf_projectile_pipe_remote") == 0);
}

bool CTeamFortress2Mod::isHurtfulPipeGrenade(edict_t* pEntity, edict_t* pPlayer, const bool bCheckOwner)
{
	if (strcmp(pEntity->GetClassName(), "tf_projectile_pipe") == 0)
	{
		if (bCheckOwner && (CClassInterface::getPipeBombOwner(pEntity) == pPlayer))
			return true;

		int iPlayerTeam = getTeam(pPlayer);
		int iGrenTeam = getTeam(pEntity);

		return iPlayerTeam != iGrenTeam;
	}

	return false;
}

bool CTeamFortress2Mod::isRocket(edict_t* pEntity, int iTeam)
{
	return (!iTeam || (iTeam == getTeam(pEntity))) && (strcmp(pEntity->GetClassName(), "tf_projectile_rocket") == 0);
}

edict_t* CTeamFortress2Mod::getMediGun(edict_t* pPlayer)
{
	if (CClassInterface::getTF2Class(pPlayer) == TF_CLASS_MEDIC)
		return pMediGuns[ENTINDEX(pPlayer) - 1];
	return NULL;
}

void CTeamFortress2Mod::findMediGun(edict_t* pPlayer)
{
	static int i;
	static edict_t* pEnt;
	static Vector vOrigin;

	vOrigin = CBotGlobals::entityOrigin(pPlayer);

	for (i = (gpGlobals->maxClients + 1); i < gpGlobals->maxEntities; i++)
	{
		pEnt = INDEXENT(i);

		if (pEnt && CBotGlobals::entityIsValid(pEnt))
		{
			if (strcmp(pEnt->GetClassName(), "tf_weapon_medigun") == 0)
			{
				if (CBotGlobals::entityOrigin(pEnt) == vOrigin)
				{
					pMediGuns[ENTINDEX(pPlayer) - 1] = pEnt;
					break;
				}
			}
		}
	}
}

// get the teleporter exit of an entrance
edict_t* CTeamFortress2Mod::getTeleporterExit(edict_t* pTele)
{
	int i;
	edict_t* pExit;

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if (m_Teleporters[i].entrance.get() == pTele)
		{
			if ((pExit = m_Teleporters[i].exit.get()) != NULL)
			{
				return pExit;
			}

			return 0;
		}
	}

	return 0;
}

// check if the entity is a health kit
bool CTeamFortress2Mod::isHealthKit(edict_t* pEntity)
{
	return strncmp(pEntity->GetClassName(), "item_healthkit", 14) == 0;
}

bool CTeamFortress2Mod::isAreaOwnedByTeam(const int iArea, const int iTeam)
{
	if (CTeamFortress2Mod::m_ObjectiveResource.isInitialised())
	{
		return ((iArea == 0) || (CTeamFortress2Mod::m_ObjectiveResource.GetOwningTeam(CTeamFortress2Mod::m_ObjectiveResource.m_WaypointAreaToIndexTranslation[iArea]) == iTeam));
	}

	return false;
}

// cehc kif the team can pick up a flag in SD mode (special delivery)
bool CTeamFortress2Mod::canTeamPickupFlag_SD(int iTeam, bool bGetUnknown)
{
	if (isArenaPointOpen())
	{
		if (bGetUnknown)
			return (m_iFlagCarrierTeam == iTeam);
		else
			return (m_iFlagCarrierTeam == 0);
	}

	return false;
}
// For MVM only
bool CTeamFortress2Mod::getFlagLocation(int iTeam, Vector* vec)
{
	if (getDroppedFlagLocation(iTeam, vec))
	{
		return true;
	}
	else if (hasRoundStarted() && isFlagCarried(iTeam))
	{
		*vec = CBotGlobals::entityOrigin(getFlagCarrier(iTeam));
		return true;
	}

	return false;
}

// for special delivery mod and MVM
void CTeamFortress2Mod::flagReturned(int iTeam)
{
	m_iFlagCarrierTeam = 0;
	bFlagStateDefault = true;

	if (iTeam == TF2_TEAM_BLUE)
	{
		m_bFlagLocationValidBlue = false;
	}
	else if (iTeam == TF2_TEAM_RED)
	{
		m_bFlagLocationValidRed = false;
	}
}

void CTeamFortress2Mod::flagPickedUp(int iTeam, edict_t* pPlayer)
{
	bFlagStateDefault = false;
	if (iTeam == TF2_TEAM_BLUE)
	{
		m_pFlagCarrierBlue = pPlayer;
		m_bFlagLocationValidBlue = false;
	}
	else if (iTeam == TF2_TEAM_RED)
	{
		m_pFlagCarrierRed = pPlayer;
		m_bFlagLocationValidRed = false;
	}

	m_iFlagCarrierTeam = iTeam;

	CBotTF2FunctionEnemyAtIntel* function = new CBotTF2FunctionEnemyAtIntel(iTeam, CBotGlobals::entityOrigin(pPlayer), EVENT_FLAG_PICKUP);

	CBots::botFunction(function);

	delete function;
}

bool CTeamFortress2Mod::isArenaPointOpen()
{
	return m_fArenaPointOpenTime < engine->Time();
}

void CTeamFortress2Mod::resetSetupTime()
{
	m_fRoundTime = engine->Time() + m_Timer.getSetupTime();
	m_fArenaPointOpenTime = engine->Time() + m_fPointTime;
}

bool CTeamFortress2Mod::hasRoundStarted()
{
	return m_bHasRoundStarted || (!isMapType(TF_MAP_MVM) && (engine->Time() > m_fRoundTime));

	//return (engine->Time() > m_fRoundTime);
}

void CTeamFortress2Mod::setPointOpenTime(int time)
{
	m_fArenaPointOpenTime = 0.0f;
	m_fPointTime = (float)time;
}

void CTeamFortress2Mod::setSetupTime(int time)
{
	m_fRoundTime = 0.0f;
	m_fSetupTime = (float)time;
}

bool CTeamFortress2Mod::isAmmo(edict_t* pEntity)
{
	static const char* szClassname;

	szClassname = pEntity->GetClassName();

	return (strcmp(szClassname, "tf_ammo_pack") == 0) || (strncmp(szClassname, "item_ammopack", 13) == 0);
}

bool CTeamFortress2Mod::isPayloadBomb(edict_t* pEntity, int iTeam)
{
	return ((strncmp(pEntity->GetClassName(), "mapobj_cart_dispenser", 21) == 0) && (CClassInterface::getTeam(pEntity) == iTeam));
}

void CTeamFortress2Mod::checkMVMTankBoss(edict_t* pEntity)
{
	float fTankDistance = CBotGlobals::entityOrigin(pEntity).DistTo(m_vMVMCapturePoint);

	if (m_pNearestTankBoss.get() != NULL)
	{
		if (!CBotGlobals::entityIsAlive(m_pNearestTankBoss))
		{
			m_pNearestTankBoss = NULL;
			m_fNearestTankDistance = 0.0f;
		}
	}

	if (CBotGlobals::entityIsAlive(pEntity) && ((m_pNearestTankBoss.get() == NULL) || ((m_fNearestTankDistance == 0.0f) || (fTankDistance < m_fNearestTankDistance))))
	{
		m_fNearestTankDistance = fTankDistance;
		m_pNearestTankBoss = pEntity;
		m_vNearestTankLocation = CBotGlobals::entityOrigin(pEntity);
	}
}

CWaypoint* CTeamFortress2Mod::getBestWaypointMVM(CBot* pBot, int iFlags)
{
	Vector vFlagLocation;

	bool bFlagLocationValid = CTeamFortress2Mod::getFlagLocation(TF2_TEAM_BLUE, &vFlagLocation);

	float fTankDistance = 0.0f;

	edict_t* pTank;
	// check tank boss is valid
	if ((pTank = m_pNearestTankBoss.get()) != NULL)
	{
		if (CBotGlobals::entityIsAlive(pTank) == false)
			m_pNearestTankBoss = NULL;
	}

	if (hasRoundStarted() && m_bMVMFlagStartValid && m_bMVMCapturePointValid && bFlagLocationValid)
	{
		if (m_bMVMAlarmSounded)
			return CWaypoints::randomWaypointGoalNearestArea(iFlags, TF2_TEAM_RED, 0, false, pBot, true, &m_vMVMCapturePoint, -1, true, m_iCapturePointWptID);
		else if ((m_pNearestTankBoss.get() != NULL) && (m_fNearestTankDistance < vFlagLocation.DistTo(m_vMVMCapturePoint)))
			return CWaypoints::randomWaypointGoalBetweenArea(iFlags, TF2_TEAM_RED, 0, false, pBot, true, &m_vNearestTankLocation, &m_vMVMCapturePoint, true, -1, m_iCapturePointWptID);
		else if (((m_vMVMFlagStart - vFlagLocation).Length() < 1024.0f))
			return CWaypoints::randomWaypointGoalNearestArea(iFlags, TF2_TEAM_RED, 0, false, pBot, true, &vFlagLocation, -1, true);
		else
			return CWaypoints::randomWaypointGoalBetweenArea(iFlags, TF2_TEAM_RED, 0, false, pBot, true, &vFlagLocation, &m_vMVMCapturePoint, true, -1, m_iCapturePointWptID);
	}
	else if (bFlagLocationValid)
	{
		return CWaypoints::randomWaypointGoalNearestArea(iFlags, TF2_TEAM_RED, 0, false, pBot, true, &vFlagLocation, -1, true, m_iFlagPointWptID);
	}

	return NULL;
}

// check voice commands
void CTeamFortress2Mod::clientCommand(edict_t* pEntity, const int argc, const char* pcmd, const char* arg1, const char* arg2)
{
	if (argc > 2)
	{
		if (strcmp(pcmd, "voicemenu") == 0)
		{
			// somebody said a voice command
			u_VOICECMD vcmd;

			vcmd.voicecmd = 0;
			vcmd.b1.v1 = atoi(arg1);
			vcmd.b1.v2 = atoi(arg2);

			CBroadcastVoiceCommand voicecmd = CBroadcastVoiceCommand(pEntity, vcmd.voicecmd);

			CBots::botFunction(&voicecmd);
		}
	}
	else
	{
		/*if ( strcmp(pcmd,"+use_action_slot_item") == 0 )
		{
			CClient *pClient = CClients::get(pEntity);

			if ( pClient != NULL )
			{
				pClient->monitorHighFive();
			}
		}*/
	}
}

// to fixed
void CTeamFortress2Mod::teleporterBuilt(edict_t* pOwner, const eEngiBuild type, edict_t* pBuilding)
{
	int team;

	if ((type != ENGI_TELE)) //(type != ENGI_ENTRANCE) && (type != ENGI_EXIT) )
		return;

	short int iIndex = ENTINDEX(pOwner) - 1;

	if ((iIndex < 0) || (iIndex > gpGlobals->maxClients))
		return;

	team = getTeam(pOwner);

	if (CTeamFortress2Mod::isTeleporterEntrance(pBuilding, team))
		m_Teleporters[iIndex].entrance = MyEHandle(pBuilding);
	else if (CTeamFortress2Mod::isTeleporterExit(pBuilding, team))
		m_Teleporters[iIndex].exit = MyEHandle(pBuilding);

	m_Teleporters[iIndex].sapper = MyEHandle();
	m_Teleporters[iIndex].m_fLastTeleported = 0.0f;
	m_Teleporters[iIndex].m_iWaypoint = CWaypointLocations::NearestWaypoint(CBotGlobals::entityOrigin(pBuilding), 400.0f, -1, true);
}
// used for changing class if I'm doing badly in my team
int CTeamFortress2Mod::getHighestScore()
{
	short int highest = 0;
	short int score;
	short int i = 0;
	edict_t* edict;

	for (i = 1; i <= gpGlobals->maxClients; i++)
	{
		edict = INDEXENT(i);

		if (edict && CBotGlobals::entityIsValid(edict))
		{
			score = static_cast<short int>(CClassInterface::getTF2Score(edict));

			if (score > highest)
			{
				highest = score;
			}
		}
	}

	return highest;
}

// check if there is another building near where I want to build
// check quickly by using the storage of sentryguns etc in the mod class
bool CTeamFortress2Mod::buildingNearby(const int iTeam, const Vector vOrigin)
{
	edict_t* pPlayer;
	short int i;
	short int sentryIndex;

	for (i = 1; i <= gpGlobals->maxClients; i++)
	{
		pPlayer = INDEXENT(i);

		// crash bug fix
		if (!pPlayer || pPlayer->IsFree())
			continue;

		sentryIndex = i - 1;

		if (CClassInterface::getTF2Class(pPlayer) != TF_CLASS_ENGINEER)
			continue;

		if (CClassInterface::getTeam(pPlayer) != iTeam)
			continue;

		if (m_SentryGuns[sentryIndex].sentry.get())
		{
			if ((vOrigin - CBotGlobals::entityOrigin(m_SentryGuns[sentryIndex].sentry.get())).Length() < 100)
				return true;
		}

		if (m_Dispensers[sentryIndex].disp.get())
		{
			if ((vOrigin - CBotGlobals::entityOrigin(m_Dispensers[sentryIndex].disp.get())).Length() < 100)
				return true;
		}

		if (m_Teleporters[sentryIndex].entrance.get())
		{
			if ((vOrigin - CBotGlobals::entityOrigin(m_Teleporters[sentryIndex].entrance.get())).Length() < 100)
				return true;
		}

		if (m_Teleporters[sentryIndex].exit.get())
		{
			if ((vOrigin - CBotGlobals::entityOrigin(m_Teleporters[sentryIndex].exit.get())).Length() < 100)
				return true;
		}
	}

	return false;
}

//get the building
edict_t* CTeamFortress2Mod::getBuilding(eEngiBuild object, edict_t* pOwner)
{
	static short int i;
	static tf_tele_t* tele;

	//index = ENTINDEX(pOwner)-1;
	// i is 1 to 32 ( 0 == worldspawn)
	i = ENTINDEX(pOwner) - 1;

	if (i < 0)
		return NULL;

	//i = ENTINDEX(pOwner)+1;

	switch (object)
	{
	case ENGI_DISP:
		return m_Dispensers[i].disp.get();
	case ENGI_SENTRY:
		return m_SentryGuns[i].sentry.get();
	case ENGI_TELE:
		if (m_Teleporters[i].entrance.get() != NULL)
			return m_Teleporters[i].entrance.get();
		return m_Teleporters[i].exit.get();
	default:;
	}

	return NULL;
}

// get the owner of
edict_t* CTeamFortress2Mod::getBuildingOwner(eEngiBuild object, short index)
{
	static short int i;
	static tf_tele_t* tele;

	switch (object)
	{
	case ENGI_DISP:
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if (m_Dispensers[i].disp.get() && (ENTINDEX(m_Dispensers[i].disp.get()) == index))
				return INDEXENT(i + 1);
		}
		//m_SentryGuns[i].
		break;
	case ENGI_SENTRY:
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if (m_SentryGuns[i].sentry.get() && (ENTINDEX(m_SentryGuns[i].sentry.get()) == index))
				return INDEXENT(i + 1);
		}
		break;
	case ENGI_TELE:
		tele = m_Teleporters;

		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if (tele->entrance.get() && (ENTINDEX(tele->entrance.get()) == index))
				return INDEXENT(i + 1);
			if (tele->exit.get() && (ENTINDEX(tele->exit.get()) == index))
				return INDEXENT(i + 1);

			tele++;
		}
		break;
	default:;
	}

	return NULL;
}

edict_t* CTeamFortress2Mod::nearestDispenser(const Vector vOrigin, const int team)
{
	edict_t* pNearest = NULL;
	edict_t* pDisp;
	float fDist;
	float fNearest = bot_use_disp_dist.GetFloat();

	for (unsigned int i = 0; i < MAX_PLAYERS; i++)
	{
		//m_Dispensers[i]
		pDisp = m_Dispensers[i].disp.get();

		if (pDisp)
		{
			if (CTeamFortress2Mod::getTeam(pDisp) == team)
			{
				fDist = (CBotGlobals::entityOrigin(pDisp) - vOrigin).Length();

				if (fDist < fNearest)
				{
					pNearest = pDisp;
					fNearest = fDist;
				}
			}
		}
	}

	return pNearest;
}

void CTeamFortress2Mod::sapperPlaced(edict_t* pOwner, eEngiBuild type, edict_t* pSapper)
{
	static short int index;

	index = ENTINDEX(pOwner) - 1;

	if ((index >= 0) && (index < MAX_PLAYERS))
	{
		if (type == ENGI_TELE)
			m_Teleporters[index].sapper = MyEHandle(pSapper);
		else if (type == ENGI_DISP)
			m_Dispensers[index].sapper = MyEHandle(pSapper);
		else if (type == ENGI_SENTRY)
			m_SentryGuns[index].sapper = MyEHandle(pSapper);
	}
}

void CTeamFortress2Mod::addWaypointFlags(edict_t* pPlayer, edict_t* pEdict, int* iFlags, int* iArea, float* fMaxDistance)
{
	string_t model = pEdict->GetIServerEntity()->GetModelName();

	if (strncmp(pEdict->GetClassName(), "item_health", 11) == 0)
		*iFlags |= CWaypointTypes::W_FL_HEALTH;
	else if (strncmp(pEdict->GetClassName(), "item_ammo", 9) == 0)
		*iFlags |= CWaypointTypes::W_FL_AMMO;
	else if (strcmp(pEdict->GetClassName(), "prop_dynamic") == 0)
	{
		if (strcmp(model.ToCStr(), "models/props_gameplay/resupply_locker.mdl") == 0)
		{
			*iFlags |= CWaypointTypes::W_FL_RESUPPLY;

			if (CTeamFortress2Mod::getTeam(pPlayer) == TF2_TEAM_BLUE)
				*iFlags |= CWaypointTypes::W_FL_NORED;
			else if (CTeamFortress2Mod::getTeam(pPlayer) == TF2_TEAM_RED)
				*iFlags |= CWaypointTypes::W_FL_NOBLU;
		}
	}
	// do this in the event code
	/*else if ( strcmp(pEdict->GetClassName(),"item_teamflag") == 0 )
	{
		if ( !CTeamFortress2Mod::isFlagCarrier(pPlayer) )
			*iFlags |= CWaypointTypes::W_FL_FLAG;
	}*/
	else if (strcmp(pEdict->GetClassName(), "team_control_point") == 0)
	{
		*iFlags |= CWaypointTypes::W_FL_CAPPOINT;
		*iArea = m_ObjectiveResource.getControlPointArea(pEdict);
		*fMaxDistance = 100;
	}
}

void CTeamFortress2Mod::sapperDestroyed(edict_t* pOwner, eEngiBuild type, edict_t* pSapper)
{
	static short int index;

	for (index = 0; index < MAX_PLAYERS; index++)
	{
		if (type == ENGI_TELE)
		{
			if (m_Teleporters[index].sapper.get_old() == pSapper)
				m_Teleporters[index].sapper = MyEHandle();
		}
		else if (type == ENGI_DISP)
		{
			if (m_Dispensers[index].sapper.get_old() == pSapper)
				m_Dispensers[index].sapper = MyEHandle();
		}
		else if (type == ENGI_SENTRY)
		{
			if (m_SentryGuns[index].sapper.get_old() == pSapper)
				m_SentryGuns[index].sapper = MyEHandle();
		}
	}
}

void CTeamFortress2Mod::updatePointMaster()
{
	if (m_PointMasterResource.get() == NULL)
	{
		edict_t* pMaster = CClassInterface::FindEntityByClassnameNearest(Vector(0, 0, 0), "team_control_point_master", 65535);

		if (pMaster)
		{
#ifdef _WIN32
			extern ConVar rcbot_const_point_master_offset;
			extern IServerGameEnts* servergameents;

			CBaseEntity* pMasterEntity = servergameents->EdictToBaseEntity(pMaster);

			//full_size is initialized but not referenced - [APG]RoboCop[CL]
			unsigned long full_size = sizeof(pMasterEntity);
			unsigned long mempoint = ((unsigned long)pMasterEntity) + rcbot_const_point_master_offset.GetInt();

			m_PointMaster = reinterpret_cast<CTeamControlPointMaster*>(mempoint);
			m_PointMasterResource = pMaster;
#else
			extern ConVar rcbot_const_point_master_offset;
			CBaseEntity* pent = pMaster->GetUnknown()->GetBaseEntity();
			unsigned long mempoint = ((unsigned long)pent) + rcbot_const_point_master_offset.GetInt();

			m_PointMaster = (CTeamControlPointMaster*)mempoint;
			m_PointMasterResource = pMaster;
#endif

			//extern ConVar rcbot_const_round_offset;

			int idx = m_PointMaster->m_iCurrentRoundIndex;
			int size = m_PointMaster->m_ControlPointRounds.Size();

			if (idx >= 0 && size >= 0 && idx < 100 && size < 100)
			{
				int infoCount = 0;

				for (int r = 0; r < m_PointMaster->m_ControlPointRounds.Size(); ++r)
				{
					try
					{
						CBaseEntity* pent = m_PointMaster->m_ControlPointRounds[r];

						CTeamControlPointRound* pointRound = (CTeamControlPointRound*)((unsigned long)pent + (unsigned long)rcbot_const_point_master_offset.GetInt());

						CBotGlobals::botMessage(NULL, 0, "Control Points for Round %d", r);

						for (int i = 0; i < pointRound->m_ControlPoints.Count(); ++i)
						{
							CBaseHandle* handle = &pointRound->m_ControlPoints.Element(i);

							if (handle->IsValid())
							{
								edict_t* edict = INDEXENT(handle->GetEntryIndex());

								if (!edict->IsFree())
								{
									infoCount++;
									CBotGlobals::botMessage(NULL, 0, "%d, %d, %d, %s", r, i, handle->GetSerialNumber(), edict->GetClassName());
								}
							}
						}
					}
					catch (...)
					{
						//error
						break;
					}
				}

				if (infoCount == 0)
				{
					CBotGlobals::botMessage(
						NULL, 0,
						"If you are playing cp_* maps, and you get this message, something might be wrong with your mstr_offset!");
				}
			}
			else
			{
				CBotGlobals::botMessage(
					NULL, 0,
					"If you are playing cp_* maps, and you get this message, something might be wrong with your mstr_offset!");
			}
		}
	}

	if (m_PointMaster != NULL)
	{
		m_pCurrentRound = m_PointMaster->getCurrentRound();
	}
}

edict_t* CTeamFortress2Mod::getPayloadBomb(const int team)
{
	if (team == TF2_TEAM_BLUE)
		return m_pPayLoadBombBlue;
	else if (team == TF2_TEAM_RED)
		return m_pPayLoadBombRed;

	return NULL;
}

void CTeamFortress2Mod::roundReset()
{
	if (m_ObjectiveResource.m_ObjectiveResource.get() == NULL)
	{
		m_ObjectiveResource.m_ObjectiveResource = CClassInterface::FindEntityByNetClass(gpGlobals->maxClients + 1, "CTFObjectiveResource");

		//if ( m_ObjectiveResource.m_ObjectiveResource.get() != NULL )
		//{
		//	m_ObjectiveResource.setup();
		//}
	}

	//always reset on new round
	if (m_ObjectiveResource.m_ObjectiveResource.get() != NULL) //!m_ObjectiveResource.isInitialised() )
		m_ObjectiveResource.setup();

	m_Timer.reset();

	updatePointMaster();

	if (m_ObjectiveResource.isInitialised())
	{
		extern ConVar rcbot_tf2_autoupdate_point_time;
		const int numpoints = m_ObjectiveResource.GetNumControlPoints();
		int i;

		for (i = 0; i < numpoints; i++)
		{
			if (m_ObjectiveResource.GetOwningTeam(i) != TF2_TEAM_RED)
				break;
		}

		// if all points are owned by RED at start up then its an attack defend map
		setAttackDefendMap(i == numpoints);

		m_ObjectiveResource.m_fUpdatePointTime = engine->Time() + rcbot_tf2_autoupdate_point_time.GetFloat();
		m_ObjectiveResource.m_fNextCheckMonitoredPoint = engine->Time() + 0.2f;

		m_ObjectiveResource.updatePoints();
	}

	m_pNearestTankBoss = NULL;
	m_fNearestTankDistance = 0.0f;
	m_iWinningTeam = 0;
	m_bRoundOver = false;
	m_bHasRoundStarted = false;
	m_iFlagCarrierTeam = 0;
	m_pPayLoadBombBlue = NULL;
	m_pPayLoadBombRed = NULL;

	if (isMapType(TF_MAP_MVM))
	{
		if (!m_bMVMFlagStartValid)
		{
			CWaypoint* pGoal = CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_FLAG);

			if (pGoal)
			{
				m_vNearestTankLocation = m_vMVMFlagStart = m_vFlagLocationBlue = pGoal->getOrigin();
				m_bMVMFlagStartValid = m_bFlagLocationValidBlue = true;
				m_iFlagPointWptID = CWaypoints::getWaypointIndex(pGoal);
			}
		}
		else
		{
			// Reset Flag Location
			m_vFlagLocationBlue = m_vMVMFlagStart;
			m_bFlagLocationValidBlue = true;
		}

		if (!m_bMVMCapturePointValid)
		{
			CWaypoint* pGoal = CWaypoints::randomWaypointGoal(CWaypointTypes::W_FL_CAPPOINT);

			if (pGoal)
			{
				m_vMVMCapturePoint = pGoal->getOrigin();
				m_bMVMCapturePointValid = true;
				m_fMVMCapturePointRadius = pGoal->getRadius();
				m_iCapturePointWptID = CWaypoints::getWaypointIndex(pGoal);
			}
		}
	}
}

void CTeamFortress2Mod::sentryBuilt(edict_t* pOwner, const eEngiBuild type, edict_t* pBuilding)
{
	static short int index;
	static tf_sentry_t* temp;

	index = ENTINDEX(pOwner) - 1;

	if ((index >= 0) && (index < MAX_PLAYERS))
	{
		if (type == ENGI_SENTRY)
		{
			temp = &(m_SentryGuns[index]);
			temp->sentry = MyEHandle(pBuilding);
			temp->sapper = MyEHandle();
			//m_SentryGuns[index].builder
		}
	}
}

bool CTeamFortress2Mod::isSentryGun(edict_t* pEdict)
{
	static short int i;
	static tf_sentry_t* temp;

	temp = m_SentryGuns;

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if (temp->sentry == pEdict)
			return true;

		temp++;
	}

	return false;
}

void CTeamFortress2Mod::dispenserBuilt(edict_t* pOwner, const eEngiBuild type, edict_t* pBuilding)
{
	static short int index;
	static tf_disp_t* temp;

	index = ENTINDEX(pOwner) - 1;

	if ((index >= 0) && (index < MAX_PLAYERS))
	{
		if (type == ENGI_DISP)
		{
			temp = &(m_Dispensers[index]);
			temp->disp = MyEHandle(pBuilding);
			temp->sapper = MyEHandle();
			//m_Dispensers[index].builder = userid;
		}
	}
}

void CTeamFortress2Mod::updateRedPayloadBomb(edict_t* pent)
{
	edict_t* cur = m_pPayLoadBombRed.get();

	if (cur != pent)
		m_pPayLoadBombRed = pent;
}

void CTeamFortress2Mod::updateBluePayloadBomb(edict_t* pent)
{
	edict_t* cur = m_pPayLoadBombBlue.get();

	if (cur != pent)
		m_pPayLoadBombBlue = pent;
}

bool CTeamFortress2Mod::isDefending(edict_t* pPlayer)//, int iCapIndex = -1 )
{
	int iIndex = (1 << (ENTINDEX(pPlayer) - 1));

	if (m_ObjectiveResource.GetNumControlPoints() > 0)
	{
		int iTeam = CClassInterface::getTeam(pPlayer);

		//if ( iCapIndex == -1 )
		//{
		for (short int i = 0; i < MAX_CONTROL_POINTS; i++)
		{
			if (m_ObjectiveResource.isCPValid(i, iTeam, TF2_POINT_DEFEND))
			{
				if ((m_iCapDefenders[i] & iIndex) == iIndex)
					return true;
			}
		}
		//}
		//else
		//	return ((m_iCapDefenders[iCapIndex] & iIndex) == iIndex );
	}

	return false;
}

bool CTeamFortress2Mod::isCapping(edict_t* pPlayer)//, int iCapIndex = -1 )
{
	const int index = (1 << (ENTINDEX(pPlayer) - 1));

	if (m_ObjectiveResource.GetNumControlPoints() > 0)
	{
		const int iTeam = CClassInterface::getTeam(pPlayer);

		int i = 0;

		for (i = 0; i < MAX_CAP_POINTS; i++)
		{
			if (m_ObjectiveResource.isCPValid(i, iTeam, TF2_POINT_ATTACK))
			{
				if ((m_Cappers[i] & index) == index)
					return true;
			}
		}
	}

	return false;
}

/*void CTF2Loadout :: addAttribute ( CAttribute *attrib )
{
	m_Attributes.push_back(attrib);
}*/

unsigned int CTF2Loadout::copyAttributesIntoArray(CEconItemAttribute* pArray, void* pVTable)
{
	for (unsigned int i = 0; i < m_Attributes.size(); i++)
	{
		pArray[i] = *(m_Attributes[i]);
		pArray[i].m_pVTable = pVTable;
	}

	return m_Attributes.size();
}

void CTF2Loadout::addAttribute(const int id, const float fval)
{
	m_Attributes.push_back(new CEconItemAttribute(id, fval));
}

void  CTF2Loadout::applyAttributes(CBaseEntity* pEnt)
{/*
	extern IServerGameEnts *servergameents;

	edict_t *pEdict = servergameents->BaseEntityToEdict(pEnt);

	for ( unsigned int i = 0; i < m_Attributes.size(); i ++ )
	{
		int id = m_Attributes[i]->m_iAttributeDefinitionIndex;
		float fval = m_Attributes[i]->m_flValue;

		UTIL_ApplyAttribute(pEdict,g_pszAttributeNames[id],fval);
	}*/
}

void  CTF2Loadout::applyAttributes(CEconItemView* cscript)
{
	for (unsigned int i = 0; i < m_Attributes.size(); i++)
	{
		m_Attributes[i]->m_pVTable = cscript->m_AttributeList.m_pVTable;

		cscript->m_AttributeList.m_Attributes.AddToHead((*m_Attributes[i]));
	}
}

void  CTF2Loadout::freeMemory()
{
	for (unsigned int i = 0; i < m_Attributes.size(); i++)
	{
		CEconItemAttribute* pdel = m_Attributes[i];
		delete pdel;
		m_Attributes[i] = NULL;
	}
}

void CTeamFortress2Mod::freeMemory()
{
	vector<CTF2Loadout*> loadouts_to_free;

	for (unsigned int i = 0; i < TF2_SLOT_MAX; i++)
	{
		for (unsigned int j = 0; j < 9; j++)
		{
			for (unsigned int k = 0; k < m_pLoadoutWeapons[i][j].size(); k++)
			{
				CTF2Loadout* wep = (m_pLoadoutWeapons[i][j]).at(k);

				bool bfound = false;

				for (unsigned int l = 0; l < loadouts_to_free.size(); l++)
				{
					if (loadouts_to_free.at(l) == wep)
					{
						bfound = true;
						break;
					}
				}

				if (bfound == false)
					loadouts_to_free.push_back(wep);

				(m_pLoadoutWeapons[i][j])[k] = NULL;
			}

			(m_pLoadoutWeapons[i][j]).clear();
		}
	}

	for (unsigned int i = 0; i < loadouts_to_free.size(); i++)
	{
		CTF2Loadout* wep = loadouts_to_free.at(i);

		delete wep;
		loadouts_to_free[i] = NULL;
	}

	loadouts_to_free.clear();
}

void CAttribute::applyAttribute(edict_t* pEdict)
{
	//UTIL_ApplyAttribute(pEdict,m_name,m_fval);
}