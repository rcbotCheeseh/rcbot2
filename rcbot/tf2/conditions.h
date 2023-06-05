/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod (C)2004-2008 AlliedModders LLC.  All rights reserved.
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
 * Version: $Id$
 */
#ifndef __RCBOT2_TF2_CONDITIONS_H__
#define __RCBOT2_TF2_CONDITIONS_H__

typedef enum
{
	TFCond_Slowed = 0, //0: Revving Minigun, Sniper Rifle. Gives zoomed/revved pose
	TFCond_Zoomed, //1: Sniper Rifle zooming
	TFCond_Disguising, //2: Disguise smoke
	TFCond_Disguised, //3: Disguise
	TFCond_Cloaked, //4: Cloak effect
	TFCond_Ubercharged, //5: Invulnerability, removed when being healed or by another Uber effect
	TFCond_TeleportedGlow, //6: Teleport trail effect
	TFCond_Taunting, //7: Used for taunting, can remove to stop taunting
	TFCond_UberchargeFading, //8: Invulnerability expiration effect
	TFCond_Unknown1, //9
	TFCond_CloakFlicker = 9, //9: Cloak flickering effect
	TFCond_Teleporting, //10: Used for teleporting, does nothing applying
	TFCond_Kritzkrieged, //11: Crit boost, removed when being healed or another Uber effect
	TFCond_Unknown2, //12
	TFCond_TmpDamageBonus = 12, //12: Temporary damage buff, something along with attribute 19
	TFCond_DeadRingered, //13: Dead Ringer damage resistance, gives TFCond_Cloaked
	TFCond_Bonked, //14: Bonk! Atomic Punch effect
	TFCond_Dazed, //15: Slow effect, can remove to remove stun effects
	TFCond_Buffed, //16: Buff Banner mini-crits, icon, and glow
	TFCond_Charging, //17: Forced forward, charge effect
	TFCond_DemoBuff, //18: Eyelander eye glow
	TFCond_CritCola, //19: Mini-crit effect
	TFCond_InHealRadius, //20: Ring effect, rings disappear after a taunt ends
	TFCond_Healing, //21: Used for healing, does nothing applying
	TFCond_OnFire, //22: Ignite sound and vocals, can remove to remove afterburn
	TFCond_Overhealed, //23: Used for overheal, does nothing applying
	TFCond_Jarated, //24: Jarate effect
	TFCond_Bleeding, //25: Bleed effect
	TFCond_DefenseBuffed, //26: Battalion's Backup's defense, icon, and glow
	TFCond_Milked, //27: Mad Milk effect
	TFCond_MegaHeal, //28: Quick-Fix Ubercharge's knockback/stun immunity and visual effect
	TFCond_RegenBuffed, //29: Concheror's speed boost, heal on hit, icon, and glow
	TFCond_MarkedForDeath, //30: Fan o' War marked-for-death effect
	TFCond_NoHealingDamageBuff, //31: Mini-crits, blocks healing, glow, no weapon mini-crit effects
	TFCond_SpeedBuffAlly, //32: Disciplinary Action speed boost
	TFCond_HalloweenCritCandy, //33: Halloween pumpkin crit-boost
	TFCond_CritCanteen, //34: Crit-boost and doubles Sentry Gun fire-rate
	TFCond_CritDemoCharge, //35: Crit glow, adds TFCond_Charging when charge meter is below 75%
	TFCond_CritHype, //36: Soda Popper multi-jump effect
	TFCond_CritOnFirstBlood, //37: Arena first blood crit-boost
	TFCond_CritOnWin, //38: End-of-round crit-boost (May not remove correctly?)
	TFCond_CritOnFlagCapture, //39: Intelligence capture crit-boost
	TFCond_CritOnKill, //40: Crit-boost from crit-on-kill weapons
	TFCond_RestrictToMelee, //41: Prevents switching once melee is out
	TFCond_DefenseBuffNoCritBlock, //42: MvM Bomb Carrier defense buff (TFCond_DefenseBuffed without crit resistance)
	TFCond_Reprogrammed, //43: No longer functions
	TFCond_CritMmmph, //44: Phlogistinator crit-boost
	TFCond_DefenseBuffMmmph, //45: Old Phlogistinator defense buff
	TFCond_FocusBuff, //46: Hitman's Heatmaker no-unscope and faster Sniper charge
	TFCond_DisguiseRemoved, //47: Enforcer damage bonus removed
	TFCond_MarkedForDeathSilent, //48: Marked-for-death without sound effect
	TFCond_DisguisedAsDispenser, //49: Dispenser disguise when crouching, max movement speed, sentries ignore player
	TFCond_Sapped, //50: Sapper sparkle effect in MvM
	TFCond_UberchargedHidden, //51: Out-of-bounds robot invulnerability effect
	TFCond_UberchargedCanteen, //52: Invulnerability effect and Sentry Gun damage resistance
	TFCond_HalloweenBombHead, //53: Bomb head effect (does not explode)
	TFCond_HalloweenThriller, //54: Forced Thriller taunting
	TFCond_RadiusHealOnDamage, //55: Radius healing, adds TFCond_InHealRadius, TFCond_Healing. Removed when a taunt ends, but this condition stays but does nothing
	TFCond_CritOnDamage, //56: Miscellaneous crit-boost
	TFCond_UberchargedOnTakeDamage, //57: Miscellaneous invulnerability
	TFCond_UberBulletResist, //58: Vaccinator Uber bullet resistance
	TFCond_UberBlastResist, //59: Vaccinator Uber blast resistance
	TFCond_UberFireResist, //60: Vaccinator Uber fire resistance
	TFCond_SmallBulletResist, //61: Vaccinator healing bullet resistance
	TFCond_SmallBlastResist, //62: Vaccinator healing blast resistance
	TFCond_SmallFireResist, //63: Vaccinator healing fire resistance
	TFCond_Stealthed, //64: Cloaked until next attack
	TFCond_MedigunDebuff, //65: Unknown
	TFCond_StealthedUserBuffFade, //66: Cloaked, will appear for a few seconds on attack and cloak again
	TFCond_BulletImmune, //67: Full bullet immunity
	TFCond_BlastImmune, //68: Full blast immunity
	TFCond_FireImmune, //69: Full fire immunity
	TFCond_PreventDeath, //70: Survive to 1 health, then the condition is removed
	TFCond_MVMBotRadiowave, //71: Stuns bots and applies radio effect
	TFCond_HalloweenSpeedBoost, //72: Speed boost, non-melee fire rate and reload, infinite air jumps
	TFCond_HalloweenQuickHeal, //73: Healing effect, adds TFCond_Healing along with TFCond_MegaHeal temporarily
	TFCond_HalloweenGiant, //74: Double size, x10 max health increase, ammo regeneration, and forced thirdperson
	TFCond_HalloweenTiny, //75: Half size and increased head size
	TFCond_HalloweenInHell, //76: Applies TFCond_HalloweenGhostMode when the player dies
	TFCond_HalloweenGhostMode, //77: Becomes a ghost unable to attack but can fly
	TFCond_MiniCritOnKill, //78: Mini-crits effect
	TFCond_DodgeChance, //79
	TFCond_ObscuredSmoke = 79, //79: 75% chance to dodge an attack
	TFCond_Parachute, //80: Parachute effect, removed when touching the ground
	TFCond_BlastJumping, //81: Player is blast jumping
	TFCond_HalloweenKart, //82: Player forced into a Halloween kart
	TFCond_HalloweenKartDash, //83: Forced forward if in TFCond_HalloweenKart, zoom in effect, and dash animations
	TFCond_BalloonHead, //84: Big head and lowered gravity
	TFCond_MeleeOnly, //85: Forced melee, along with TFCond_SpeedBuffAlly and TFCond_HalloweenTiny
	TFCond_SwimmingCurse, //86: Swim in the air with Jarate overlay
	TFCond_HalloweenKartNoTurn, //87
	TFCond_FreezeInput = 87, //87: Prevents player from using controls
	TFCond_HalloweenKartCage, //88: Puts a cage around the player if in TFCond_HalloweenKart, otherwise crashes
	TFCond_HasRune, //89: Has a powerup
	TFCond_RuneStrength, //90: Double damage and no damage falloff
	TFCond_RuneHaste, //91: Double fire rate, reload speed, clip and ammo size, and 30% faster movement speed
	TFCond_RuneRegen, //92: Regen ammo, health, and metal
	TFCond_RuneResist, //93: Takes 1/2 damage and critical immunity
	TFCond_RuneVampire, //94: Takes 3/4 damage, gain health on damage, and 40% increase in max health
	TFCond_RuneWarlock, //95: Attacker takes damage and knockback on hitting the player and 50% increase in max health
	TFCond_RunePrecision, //96: Less bullet spread, no damage falloff, 250% faster projectiles, and double damage, faster charge, and faster re-zoom for Sniper Rifles
	TFCond_RuneAgility, //97: Increased movement speed, grappling hook speed, jump height, and instant weapon switch
	TFCond_GrapplingHook, //98: Used when a player fires their grappling hook, no effect applying or removing
	TFCond_GrapplingHookSafeFall, //99: Used when a player is pulled by their grappling hook, no effect applying or removing
	TFCond_GrapplingHookLatched, //100: Used when a player latches onto a wall, no effect applying or removing
	TFCond_GrapplingHookBleeding, //101: Used when a player is hit by attacker's grappling hook
	TFCond_AfterburnImmune, //102: Deadringer afterburn immunity
	TFCond_RuneKnockout, //103: Melee and grappling hook only, increased max health, knockback immunity, x4 more damage against buildings, and knockbacks a powerup off a victim on hit
	TFCond_RuneImbalance, //104: Prevents gaining a crit-boost or Uber powerups
	TFCond_CritRuneTemp, //105: Crit-boost effect
	TFCond_PasstimeInterception, //106: Used when a player intercepts the Jack/Ball
	TFCond_SwimmingNoEffects, //107: Swimming in the air without animations or overlay
	TFCond_EyeaductUnderworld, //108: Refills max health, short Uber, escaped the underworld message on removal
	TFCond_KingRune, //109: Increased max health and applies TFCond_KingAura
	TFCond_PlagueRune, //110: Radius health kit stealing, increased max health, TFCond_Plague on touching a victim
	TFCond_SupernovaRune, //111: Charge meter passively increasing, when charged activiated causes radius Bonk stun
	TFCond_Plague, //112: Plague sound effect and message, blocks King powerup health regen
	TFCond_KingAura, //113: Increased fire rate, reload speed, and health regen to players in a radius
	TFCond_SpawnOutline, //114: Outline and health meter of teammates (and disguised spies)
	TFCond_KnockedIntoAir, //115: Used when a player is airblasted
	TFCond_CompetitiveWinner, //116: Unknown
	TFCond_CompetitiveLoser, //117: Unknown
	TFCond_NoTaunting_DEPRECATED, //118
	TFCond_HealingDebuff = 118, //118: Healing debuff from Medics and dispensers
	TFCond_PasstimePenaltyDebuff, //119: Marked-for-death effect
	TFCond_GrappledToPlayer, //120: Prevents taunting and some Grappling Hook actions
	TFCond_GrappledByPlayer, //121: Unknown
	TFCond_ParachuteDeployed, //122: Parachute deployed, prevents reopening it
	TFCond_Gas, //123: Gas Passer effect
	TFCond_BurningPyro, //124: Dragon's Fury afterburn on Pyros
	TFCond_RocketPack, //125: Thermal Thruster launched effects, prevents reusing
	TFCond_LostFooting, //126: Less ground friction
	TFCond_AirCurrent, //127: Reduced air control and friction
	TFCond_HalloweenHellHeal, // 128: Used when a player gets teleported to hell
	TFCond_PowerupModeDominant // 129: Reduces effects of certain powerups
}TFCond;

class CTF2Conditions
{
public:
	bool TF2_IsPlayerInCondition(int client, TFCond cond);
};

extern CTF2Conditions *tf2_conditions;

#endif