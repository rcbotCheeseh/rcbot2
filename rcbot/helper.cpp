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

#include "helper.h"
#include "IEngineTrace.h"
#include "toolframework/itoolentity.h"
#include <server_class.h>

static CBotHelper s_bot_helper;
CBotHelper *bot_helper = &s_bot_helper;

extern IEngineTrace *enginetrace;
extern IServerTools *servertools;

/* Given an entity reference or index, fill out a CBaseEntity and/or edict.
   If lookup is successful, returns true and writes back the two parameters.
   If lookup fails, returns false and doesn't touch the params.  */
bool CBotHelper::IndexToAThings(int num, CBaseEntity **pEntData, edict_t **pEdictData)
{
	CBaseEntity *pEntity = sm_gamehelpers->ReferenceToEntity(num);

	if (!pEntity)
	{
		return false;
	}

	int index = sm_gamehelpers->ReferenceToIndex(num);
	if (index > 0 && index <= sm_players->GetMaxClients())
	{
		SourceMod::IGamePlayer *pPlayer = sm_players->GetGamePlayer(index);
		if (!pPlayer || !pPlayer->IsConnected())
		{
			return false;
		}
	}

	if (pEntData)
	{
		*pEntData = pEntity;
	}

	if (pEdictData)
	{
		edict_t *pEdict = BaseEntityToEdict(pEntity);
		if (!pEdict || pEdict->IsFree())
		{
			pEdict = nullptr;
		}

		*pEdictData = pEdict;
	}

	return true;
}

/**
 * Clone of CCollisionProperty::OBBCenter( ) --- see game/shared/collisionproperty.h
 * 
 * @param pEntity		Entity to get OBB center
 **/
Vector CBotHelper::getOBBCenter( edict_t *pEntity )
{
	Vector result = Vector(0,0,0);
	VectorLerp(pEntity->GetCollideable()->OBBMins(), pEntity->GetCollideable()->OBBMaxs(), 0.5f, result);
	return result;
}

Vector CBotHelper::collisionToWorldSpace( const Vector &in, edict_t *pEntity )
{
	Vector result = Vector(0,0,0);

	if(!isBoundsDefinedInEntitySpace(pEntity) || pEntity->GetCollideable()->GetCollisionAngles() == vec3_angle)
	{
		VectorAdd(in, pEntity->GetCollideable()->GetCollisionOrigin(), result);
	}
	else
	{
		VectorTransform(in, pEntity->GetCollideable()->CollisionToWorldTransform(), result);
	}

	return result;
}

/**
 * Gets the entity world center. Clone of WorldSpaceCenter()
 * @param pEntity	The entity to get the center from
 * @return			Center vector
 **/
Vector CBotHelper::worldCenter( edict_t *pEntity )
{
	Vector result = getOBBCenter(pEntity);
	result = collisionToWorldSpace(result, pEntity);
	return result;
}

/**
 * Checks if a point is within a trigger
 * 
 * @param pEntity	The trigger entity
 * @param vPoint	The point to be tested
 * @return			True if the given point is within pEntity
 **/
bool CBotHelper::pointIsWithin( edict_t *pEntity, const Vector &vPoint )
{
	Ray_t ray;
	trace_t tr;
	ICollideable *pCollide = pEntity->GetCollideable();
	ray.Init(vPoint, vPoint);
	enginetrace->ClipRayToCollideable(ray, MASK_ALL, pCollide, &tr);
	return (tr.startsolid);
}

/// @brief Checks if the given edict is a brush model
/// @param pEntity Edict to check
/// @return True if brush model
bool CBotHelper::isBrushEntity( edict_t *pEntity )
{
	const char* szModel = pEntity->GetIServerEntity()->GetModelName().ToCStr();
	return szModel[0] == '*';
}

/// @brief Searches for entities by classname
/// @return Entity index/reference or INVALID_EHANDLE_INDEX if none is found
int CBotHelper::FindEntityByClassname(int start,const char *classname)
{
	CBaseEntity *pEntity = servertools->FindEntityByClassname(GetEntity(start), classname);
	return sm_gamehelpers->EntityToBCompatRef(pEntity);
}

/// @brief Searches for entities in a sphere
/// @return Entity index/reference or INVALID_EHANDLE_INDEX if none is found
int CBotHelper::FindEntityInSphere(int start, const Vector& center, float radius)
{
	CBaseEntity *pEntity = servertools->FindEntityInSphere(GetEntity(start), center, radius);
	return sm_gamehelpers->EntityToBCompatRef(pEntity);
}

/// @brief Searches for entities by their networkable class
/// @return Entity index or INVALID_EHANDLE_INDEX if none is found
int CBotHelper::FindEntityByNetClass(int start, const char *classname)
{
	for (int i = ((start != -1) ? start : 0); i < gpGlobals->maxEntities; i++)
	{
		edict_t* current = engine->PEntityOfEntIndex(i);
		if (current == nullptr || current->IsFree())
		{
			continue;
		}

		IServerNetworkable *network = current->GetNetworkable();

		if (network == nullptr)
		{
			continue;
		}

		ServerClass *sClass = network->GetServerClass();
		const char *name = sClass->GetName();
		

		if (strcmp(name, classname) == 0)
		{
			return i;
		}
	}

	return INVALID_EHANDLE_INDEX;
}

/// @brief check if a point is in the field of a view of an object. supports up to 180 degree fov.
/// @param vecSrcPosition Source position of the view.
/// @param vecTargetPosition Point to check if within view angle.
/// @param vecLookDirection The direction to look towards.  Note that this must be a forward angle vector.
/// @param flCosHalfFOV The width of the forward view cone as a dot product result.
/// @return True if the point is within view from the source position at the specified FOV.
/// @note https://github.com/ValveSoftware/source-sdk-2013/blob/beaae8ac45a2f322a792404092d4482065bef7ef/sp/src/public/mathlib/vector.h#L462-L477
bool CBotHelper::PointWithinViewAngle(Vector const &vecSrcPosition, Vector const &vecTargetPosition, Vector const &vecLookDirection, float flCosHalfFOV)
{
	const Vector vecDelta = vecTargetPosition - vecSrcPosition;
	const float cosDiff = DotProduct( vecLookDirection, vecDelta );

	if ( cosDiff < 0 ) 
		return false;

	const float flLen2 = vecDelta.LengthSqr();

	// a/sqrt(b) > c  == a^2 > b * c ^2
	return ( cosDiff * cosDiff > flLen2 * flCosHalfFOV * flCosHalfFOV );
	
}

/// @brief Calculates the width of the forward view cone as a dot product result from the given angle.
/// This manually calculates the value of CBaseCombatCharacter's `m_flFieldOfView` data property.
/// @param angle The FOV value in degree
/// @return Width of the forward view cone as a dot product result
float CBotHelper::GetForwardViewCone(float angle)
{
	return cosf(DEG2RAD(angle) / 2.0f);
}