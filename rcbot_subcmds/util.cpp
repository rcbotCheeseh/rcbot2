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
 */

CBotCommandInline SearchCommand("search", CMD_ACCESS_UTIL, [](CClient *pClient, BotCommandArgs args)
{
	int i = 0;

	edict_t *pPlayer = pClient->getPlayer();
	edict_t *pEdict;
	float fDistance;
	string_t model;

	for ( i = 0; i < gpGlobals->maxEntities; i ++ )
	{
		pEdict = INDEXENT(i);

		if ( pEdict )
		{
			if ( !pEdict->IsFree() )
			{
				if ( pEdict->m_pNetworkable && pEdict->GetIServerEntity() )
				{				
					if ( (fDistance=(CBotGlobals::entityOrigin(pEdict) - CBotGlobals::entityOrigin(pPlayer)).Length()) < 128 )
					{
						float fVelocity;
						Vector v;

						if ( CClassInterface::getVelocity(pEdict,&v) )
							fVelocity = v.Length();
						else
							fVelocity = 0;

						model = pEdict->GetIServerEntity()->GetModelName();
				
						CBotGlobals::botMessage(pPlayer,0,"(%d) D:%0.2f C:'%s', Mid:%d, Mn:'%s' Health=%d, Tm:%d, Fl:%d, Spd=%0.2f",i,fDistance,pEdict->GetClassName(),pEdict->GetIServerEntity()->GetModelIndex(),model.ToCStr(),(int)CClassInterface::getPlayerHealth(pEdict),(int)CClassInterface::getTeam(pEdict),pEdict->m_fStateFlags,fVelocity );
					}
				}
			}
		}
	}

	return COMMAND_ACCESSED;

});

CBotCommandInline SetTeleportUtilCommand("set_teleport", CMD_ACCESS_UTIL, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		pClient->setTeleportVector();
		engine->ClientPrintf(pClient->getPlayer(),"Teleport Position Remembered!");
		return COMMAND_ACCESSED;
	}

	return COMMAND_ERROR;
}, "usage: remembers where you want to teleport");

CBotCommandInline TeleportUtilCommand("teleport", CMD_ACCESS_UTIL, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		Vector *vTeleport;

		vTeleport = pClient->getTeleportVector();

		if ( vTeleport != NULL )
		{
			CBotGlobals::teleportPlayer(pClient->getPlayer(),*vTeleport);
			//CRCBotPlugin::HudTextMessage(pClient->getPlayer(),"teleported to your remembered location");
			CBotGlobals::botMessage(pClient->getPlayer(),0,"teleported to your remembered location");

			return COMMAND_ACCESSED;
		}
	}

	return COMMAND_ERROR;
}, "usage: first use set_teleport, then this command to go there");

CBotCommandInline NoClipCommand("noclip", CMD_ACCESS_UTIL, [](CClient *pClient, BotCommandArgs args)
{

	edict_t *pEntity = NULL;

	if ( pClient )
		pEntity = pClient->getPlayer();

	if ( pEntity )
    {
		char msg[256];
		byte *movetype = CClassInterface::getMoveTypePointer(pEntity);

		
       if ( (*movetype & 15) != MOVETYPE_NOCLIP )
	   {
           *movetype &= ~15;
		   *movetype |= MOVETYPE_NOCLIP;
	   }
       else
	   {
		   *movetype &= ~15;
		   *movetype |= MOVETYPE_WALK;
	   }

	   sprintf(msg,"%s used no_clip %d on self\n",pClient->getName(),((*movetype & 15) == MOVETYPE_NOCLIP));
           
	  // CRCBotPlugin::HudTextMessage(pEntity,msg);
	   CBotGlobals::botMessage(pEntity,0,msg);
	   return COMMAND_ACCESSED;
    }

	return COMMAND_ERROR;
}, "fly through walls , yeah!");

CBotCommandInline GodModeUtilCommand("god", CMD_ACCESS_UTIL, [](CClient *pClient, BotCommandArgs args)
{
	if ( pClient )
	{
		edict_t *pEntity = pClient->getPlayer();

		if ( pEntity )
		{
			int *playerflags = CClassInterface::getPlayerFlagsPointer(pEntity);

			if ( playerflags )
			{
				char msg[256];

				if ( *playerflags & FL_GODMODE )
					*playerflags &= ~FL_GODMODE;
				else
					*playerflags |= FL_GODMODE;

				sprintf(msg,"god mode %s",(*playerflags & FL_GODMODE)?"enabled":"disabled");
				
				//CRCBotPlugin::HudTextMessage(pEntity,msg);
				CBotGlobals::botMessage(pEntity,0,msg);

				return COMMAND_ACCESSED;

			}
		}
	}

	return COMMAND_ERROR;
}, "usage: toggle for invulnerability!");

CBotCommandInline NoTouchCommand("notouch", CMD_ACCESS_UTIL, [](CClient *pClient, BotCommandArgs args)
{

	if ( pClient )
	{
		edict_t *pEntity = pClient->getPlayer();

		if ( pEntity )
		{
			int *playerflags = CClassInterface::getPlayerFlagsPointer(pEntity);

			if ( playerflags )
			{
				char msg[256];

				if ( *playerflags & FL_DONTTOUCH )
					*playerflags &= ~FL_DONTTOUCH;
				else
					*playerflags |= FL_DONTTOUCH;

				sprintf(msg,"notouch mode %s",(*playerflags & FL_DONTTOUCH)?"enabled":"disabled");
				CBotGlobals::botMessage(NULL,0,msg);
				//CRCBotPlugin::HudTextMessage(pEntity,msg);

				return COMMAND_ACCESSED;

			}
		}
	}

	return COMMAND_ERROR;
}, "don't set off capture points etc");

CBotSubcommands UtilSubcommands("util", CMD_ACCESS_DEDICATED, {
	&SearchCommand,
	&SetTeleportUtilCommand,
	&TeleportUtilCommand,
	&NoClipCommand,
	&GodModeUtilCommand,
	&NoTouchCommand
});
