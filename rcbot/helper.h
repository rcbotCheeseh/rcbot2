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
#ifndef __RCBOT2_UTIL_FUNC_H__
#define __RCBOT2_UTIL_FUNC_H__

#include "bot_plugin_meta.h"

// General Utility functions


/// @brief General utility/helper class, alternative to CBotGlobals
class CBotHelper
{
public:
	bool IndexToAThings(int num, CBaseEntity **pEntData, edict_t **pEdictData);
	CBaseEntity *GetEntity(int entity);
	edict_t *BaseEntityToEdict(CBaseEntity *pEntity);
	Vector worldCenter( edict_t *pEntity );
	bool pointIsWithin( edict_t *pEntity, const Vector &vPoint );
	bool isBrushEntity( edict_t *pEntity );
	int FindEntityByClassname(int start, const char *classname);
	int FindEntityInSphere(int start, Vector center, float radius);
	int FindEntityByNetClass(int start, const char *classname);
	bool PointWithinViewAngle(Vector const &vecSrcPosition, Vector const &vecTargetPosition, Vector const &vecLookDirection, float flCosHalfFOV);
	float GetForwardViewCone(float angle);

private:
    inline bool isBoundsDefinedInEntitySpace( edict_t *pEntity )
	{
		return ((pEntity->GetCollideable()->GetSolidFlags() & FSOLID_FORCE_WORLD_ALIGNED) == 0 &&
		pEntity->GetCollideable()->GetSolid() != SOLID_BBOX && pEntity->GetCollideable()->GetSolid() != SOLID_NONE);
	}
	Vector getOBBCenter( edict_t *pEntity );
	Vector collisionToWorldSpace( const Vector &in, edict_t *pEntity );
};

/// @brief Converts a CBaseEntity to an edict_t
/// @param pEntity CBaseEntity pointer
/// @return edict_t pointer
inline edict_t *CBotHelper::BaseEntityToEdict(CBaseEntity *pEntity)
{
	IServerUnknown *pUnk = (IServerUnknown *)pEntity;
	IServerNetworkable *pNet = pUnk->GetNetworkable();

	if (!pNet)
	{
		return nullptr;
	}

	return pNet->GetEdict();
}

/// @brief Gets a CBaseEntity from an entity index
/// @param entity Entity/Edict index
/// @return CBaseEntity pointer
inline CBaseEntity *CBotHelper::GetEntity(int entity)
{
	CBaseEntity *pEntity;
	if (!IndexToAThings(entity, &pEntity, nullptr))
	{
		return nullptr;
	}

	return pEntity;
}

extern CBotHelper *bot_helper;

#endif
