/**
 * vim: set ts=4 :
 * =============================================================================
 * Copyright (C) 2004-2010 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 */
#ifndef __RCBOT2_ENTPROPS_H__
#define __RCBOT2_ENTPROPS_H__

#include "bot_plugin_meta.h"
#include "bot_const.h"
#include <dt_send.h>
#include <server_class.h>

enum
{
	INVALID_ENT_REFERENCE = 0xFFFFFFFF
};

enum PropType
{
	Prop_Send = 0,
	Prop_Data
};

class CBotEntProp
{
public:
	void Init(bool reset = false);
	int GetEntProp(int entity, PropType proptype, char *prop, int size = 4, int element = 0);
	int *GetEntPropPointer(int entity, PropType proptype, char *prop, int size = 4, int element = 0);
	bool GetEntPropBool(int entity, PropType proptype, char *prop, int element = 0);
	bool *GetEntPropBoolPointer(int entity, PropType proptype, char *prop, int element = 0);
	bool SetEntProp(int entity, PropType proptype, char *prop, int value, int size = 4, int element = 0);
	float GetEntPropFloat(int entity, PropType proptype, char *prop, int element = 0);
	float *GetEntPropFloatPointer(int entity, PropType proptype, char *prop, int element = 0);
	bool SetEntPropFloat(int entity, PropType proptype, char *prop, float value, int element = 0);
	int GetEntPropEnt(int entity, PropType proptype, char *prop, int element = 0);
	bool SetEntPropEnt(int entity, PropType proptype, char *prop, int other, int element = 0);
	Vector GetEntPropVector(int entity, PropType proptype, char *prop, int element = 0);
	Vector *GetEntPropVectorPointer(int entity, PropType proptype, char *prop, int element = 0);
	bool SetEntPropVector(int entity, PropType proptype, char *prop, Vector value, int element = 0);
	char *GetEntPropString(int entity, PropType proptype, char *prop, int maxlen, int *len, int element = 0);
	bool SetEntPropString(int entity, PropType proptype, char *prop, char *value, int element = 0);
	int GetEntData(int entity, int offset, int size = 4);
	bool SetEntData(int entity, int offset, int value, int size = 4, bool changeState = false);
	float GetEntDataFloat(int entity, int offset);
	bool SetEntDataFloat(int entity, int offset, float value, bool changeState = false);
	int GetEntDataEnt(int entity, int offset);
	bool SetEntDataEnt(int entity, int offset, int value, bool changeState = false);
	Vector GetEntDataVector(int entity, int offset);
	bool SetEntDataVector(int entity, int offset, Vector value, bool changeState = false);
	char *GetEntDataString(int entity, int offset, int maxlen, int *len);
	bool SetEntDataString(int entity, int offset, char *value, int maxlen, bool changeState = false);
	int GameRules_GetProp(char *prop, int size = 4, int element = 0);
	float GameRules_GetPropFloat(char *prop, int element = 0);
	int GameRules_GetPropEnt(char *prop, int element = 0);
	Vector GameRules_GetPropVector(char *prop, int element = 0);
	char *GameRules_GetPropString(char *prop, int *len, int maxlen, int element = 0);
	RoundState GameRules_GetRoundState();

private:
	bool IsNetworkedEntity(CBaseEntity *pEntity);
	edict_t *BaseEntityToEdict(CBaseEntity *pEntity);
	bool FindSendProp(SourceMod::sm_sendprop_info_t *info, CBaseEntity *pEntity, char *prop, int entity);
	int MatchTypeDescAsInteger(_fieldtypes type, int flags);
	bool IndexToAThings(int num, CBaseEntity **pEntData, edict_t **pEdictData);
	CBaseEntity *GetEntity(int entity);
	CBaseEntity *GetGameRulesProxyEntity();

	const char* grclassname = nullptr; // game rules proxy net class
	bool initialized = false;
};

inline int CBotEntProp::MatchTypeDescAsInteger(_fieldtypes type, int flags)
{
	switch (type)
	{
	case FIELD_TICK:
	case FIELD_MODELINDEX:
	case FIELD_MATERIALINDEX:
	case FIELD_INTEGER:
	case FIELD_COLOR32:
		return 32;
	case FIELD_CUSTOM:
		if ((flags & FTYPEDESC_OUTPUT) == FTYPEDESC_OUTPUT)
		{
			// Variant, read as int32.
			return 32;
		}
		break;
	case FIELD_SHORT:
		return 16;
	case FIELD_CHARACTER:
		return 8;
	case FIELD_BOOLEAN:
		return 1;
	default:
		return 0;
	}

	return 0;
}

/// @brief Converts a CBaseEntity to an edict_t
/// @param pEntity CBaseEntity pointer
/// @return edict_t pointer
inline edict_t *CBotEntProp::BaseEntityToEdict(CBaseEntity *pEntity)
{
	IServerUnknown *pUnk = reinterpret_cast<IServerUnknown*>(pEntity);
	const IServerNetworkable *pNet = pUnk->GetNetworkable();

	if (!pNet)
	{
		return nullptr;
	}

	return pNet->GetEdict();
}

/// @brief Gets a CBaseEntity from an entity index
/// @param entity Entity/Edict index
/// @return CBaseEntity pointer
inline CBaseEntity *CBotEntProp::GetEntity(int entity)
{
	CBaseEntity *pEntity;
	if (!IndexToAThings(entity, &pEntity, nullptr))
	{
		return nullptr;
	}

	return pEntity;
}

extern CBotEntProp *entprops;

#endif