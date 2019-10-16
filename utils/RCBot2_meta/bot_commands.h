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
#ifndef __BOT_COMMANDS_H__
#define __BOT_COMMANDS_H__

#include <vector>
using namespace std;

class CClient;

typedef enum
{
	COMMAND_NOT_FOUND,     // command not found
	COMMAND_ACCESSED,      // okay
	COMMAND_ERROR,		   // accessed but error occurred
	COMMAND_REQUIRE_ACCESS // dont have access to command
}eBotCommandResult;

#define NEED_ARG(x) if ( !(x) || !*(x) ) return COMMAND_ERROR;

#define CMD_ACCESS_NONE				0
#define CMD_ACCESS_WAYPOINT			(1<<0)
#define CMD_ACCESS_BOT				(1<<1)
#define CMD_ACCESS_UTIL				(1<<2)
#define CMD_ACCESS_CONFIG			(1<<3)
#define CMD_ACCESS_DEBUG			(1<<4)
#define CMD_ACCESS_USERS            (1<<5)

#define CMD_ACCESS_ALL (CMD_ACCESS_WAYPOINT|CMD_ACCESS_UTIL|CMD_ACCESS_BOT|CMD_ACCESS_CONFIG|CMD_ACCESS_DEBUG|CMD_ACCESS_USERS)

class CBotCommand
{
protected:
	CBotCommand()
	{
		m_iAccessLevel = 0;
		m_szCommand = NULL;
		m_szHelp = NULL;
	}
public:
	// initialise
	CBotCommand(char* szCommand, int iAccessLevel = 0);

	// check command name
	bool isCommand(const char* szCommand);

	// execute command
	virtual eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

	// delete command
	virtual void freeMemory();

	virtual void showStatus() { return; }
	virtual void showHelp() { return; }

	bool hasAccess(CClient* pClient);

	virtual void printCommand(edict_t* pPrintTo, int indent = 0);

	virtual void printHelp(edict_t* pPrintTo);

	virtual bool isContainer() { return false; }

	virtual bool canbeUsedDedicated() { return false; }
protected:
	void setName(char* szName);
	void setAccessLevel(int iAccessLevel);
	void setHelp(char* pszHelp);

	int m_iAccessLevel;
	char* m_szCommand;
	char* m_szHelp;
};

// container of commands
class CBotCommandContainer : public CBotCommand
{
public:
	CBotCommandContainer() {};

	// call execute command
	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

	void freeMemory();

	void add(CBotCommand* newCommand) { m_theCommands.push_back(newCommand); }

	void printCommand(edict_t* pPrintTo, int indent = 0);

	bool isContainer() { return true; }

	virtual void printHelp(edict_t* pPrintTo);

	virtual bool canbeUsedDedicated() { return true; }
private:
	vector<CBotCommand*> m_theCommands;
};

/////////////////////////////////////////////////
class CRCBotCommand : public CBotCommandContainer
{
public:
	CRCBotCommand();
};

class CUsersCommand : public CBotCommandContainer
{
public:
	CUsersCommand();
};

class CWaypointCommand : public CBotCommandContainer
{
public:
	CWaypointCommand();
};

class CPathWaypointCommand : public CBotCommandContainer
{
public:
	CPathWaypointCommand();
};

class CConfigCommand : public CBotCommandContainer
{
public:
	CConfigCommand();
};

class CUtilCommand : public CBotCommandContainer
{
public:
	CUtilCommand();
};

///////////////////////
// command
class CPrintProps : public CBotCommand
{
public:
	CPrintProps()
	{
		setName("printprops");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};
///////////////////////
// command

class CFindProp : public CBotCommand
{
public:
	CFindProp()
	{
		setName("findprop");
		setHelp("Usage: findprop <propname>");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CFindClass : public CBotCommand
{
public:
	CFindClass()
	{
		setName("findclass");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CFindClassname : public CBotCommand
{
public:
	CFindClassname()
	{
		setName("findclassname");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};
//clear bots schedules
class CBotTaskCommand : public CBotCommand
{
public:
	CBotTaskCommand()
	{
		setName("givetask");
		setAccessLevel(CMD_ACCESS_DEBUG);
		setHelp("gives a bot a task : usage <id> <entity name - for reference>");
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

//clear bots schedules
class CBotFlush : public CBotCommand
{
public:
	CBotFlush()
	{
		setName("bot_flush");
		setAccessLevel(CMD_ACCESS_DEBUG);
		setHelp("flush bot tasks");
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};
//bot goto
class CBotGoto : public CBotCommand
{
public:
	CBotGoto()
	{
		setName("bot_goto");
		setAccessLevel(CMD_ACCESS_DEBUG);
		setHelp("set a debug bot first and then stand near a waypoint to force your bot to go there");
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};
///////////////////////
// command
class CGetProp : public CBotCommand
{
public:
	CGetProp()
	{
		setName("getprop");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

///////////////////////
// command
class CSetProp : public CBotCommand
{
public:
	CSetProp()
	{
		setName("setprop");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};
class CGameEventVersion : public CBotCommand
{
public:
	CGameEventVersion()
	{
		setName("event_version");
		setAccessLevel(CMD_ACCESS_CONFIG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CMaxBotsCommand : public CBotCommand
{
public:
	CMaxBotsCommand()
	{
		setName("max_bots");
		setAccessLevel(CMD_ACCESS_CONFIG);
	}

	virtual bool canbeUsedDedicated() { return true; }

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CMinBotsCommand : public CBotCommand
{
public:
	CMinBotsCommand()
	{
		setName("min_bots");
		setAccessLevel(CMD_ACCESS_CONFIG);
	}

	virtual bool canbeUsedDedicated() { return true; }

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

///////////////////////
// waypoint
class CWaypointSetRadiusCommand : public CBotCommand
{
public:
	CWaypointSetRadiusCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointShowVisCommand : public CBotCommand
{
public:
	CWaypointShowVisCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointSetAreaCommand : public CBotCommand
{
public:
	CWaypointSetAreaCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointShiftAreas : public CBotCommand
{
public:
	CWaypointShiftAreas();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointCopy : public CBotCommand
{
public:
	CWaypointCopy();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointCut : public CBotCommand
{
public:
	CWaypointCut();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointPaste : public CBotCommand
{
public:
	CWaypointPaste();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointMenuCommand : public CBotCommand
{
public:
	CWaypointMenuCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointCheckCommand : public CBotCommand
{
public:
	CWaypointCheckCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointAreaSetToNearest : public CBotCommand
{
public:
	CWaypointAreaSetToNearest();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointAutoFix : public CBotCommand
{
public:
	CWaypointAutoFix();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointShowCommand : public CBotCommand
{
public:
	CWaypointShowCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointSetAngleCommand : public CBotCommand
{
public:
	CWaypointSetAngleCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointAngleCommand : public CBotCommand
{
public:
	CWaypointAngleCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointInfoCommand : public CBotCommand
{
public:
	CWaypointInfoCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointDrawTypeCommand : public CBotCommand
{
public:
	CWaypointDrawTypeCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointGiveTypeCommand : public CBotCommand
{
public:
	CWaypointGiveTypeCommand()
	{
		setName("givetype");
		setAccessLevel(CMD_ACCESS_WAYPOINT);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointOnCommand : public CBotCommand
{
public:
	CWaypointOnCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointOffCommand : public CBotCommand
{
public:
	CWaypointOffCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointAddCommand : public CBotCommand
{
public:
	CWaypointAddCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointTeleportCommand : public CBotCommand
{
public:
	CWaypointTeleportCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointDeleteCommand : public CBotCommand
{
public:
	CWaypointDeleteCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

///////////////
// control bot
class CControlCommand : public CBotCommand
{
public:
	CControlCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

	virtual bool canbeUsedDedicated() { return true; }
};

///////////////
// addbot
class CAddBotCommand : public CBotCommand
{
public:
	CAddBotCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

	virtual bool canbeUsedDedicated() { return true; }
};

///////////////
// pathwaypoint

class CPathWaypointDeleteToCommand : public CBotCommand
{
public:
	CPathWaypointDeleteToCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointDeleteFromCommand : public CBotCommand
{
public:
	CPathWaypointDeleteFromCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointOnCommand : public CBotCommand
{
public:
	CPathWaypointOnCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointOffCommand : public CBotCommand
{
public:
	CPathWaypointOffCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointAutoWaypointCommand : public CBotCommand
{
public:
	CWaypointAutoWaypointCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointAutoOnCommand : public CBotCommand
{
public:
	CPathWaypointAutoOnCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointAutoOffCommand : public CBotCommand
{
public:
	CPathWaypointAutoOffCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointCreateFromToCommand : public CBotCommand
{
public:
	CPathWaypointCreateFromToCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointRemoveFromToCommand : public CBotCommand
{
public:
	CPathWaypointRemoveFromToCommand();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointCreate1Command : public CBotCommand
{
public:
	CPathWaypointCreate1Command();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointCreate2Command : public CBotCommand
{
public:
	CPathWaypointCreate2Command();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointRemove1Command : public CBotCommand
{
public:
	CPathWaypointRemove1Command();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CPathWaypointRemove2Command : public CBotCommand
{
public:
	CPathWaypointRemove2Command();

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

////////

class CWaypointClearCommand : public CBotCommand
{
public:
	CWaypointClearCommand()
	{
		setName("clear");
		setAccessLevel(CMD_ACCESS_WAYPOINT);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointSaveCommand : public CBotCommand
{
public:
	CWaypointSaveCommand()
	{
		setName("save");
		setAccessLevel(CMD_ACCESS_WAYPOINT);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CWaypointLoadCommand : public CBotCommand
{
public:
	CWaypointLoadCommand()
	{
		setName("load");
		setAccessLevel(CMD_ACCESS_WAYPOINT);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};
/////////////////////////////////////////////////////
class CDebugCommand : public CBotCommandContainer
{
public:
	CDebugCommand();
};

class CDebugProfilingCommand : public CBotCommand
{
public:
	CDebugProfilingCommand()
	{
		setName("profiling");
		setHelp("usage \"profiling 1 or 0, 1 on, 0 off\" : shows performance profiling");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugMemoryCheckCommand : public CBotCommand
{
public:
	CDebugMemoryCheckCommand()
	{
		setName("memorycheck");
		setHelp("usage \"memorycheck <classname> <offset> <type>\"");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

#define MAX_MEM_SEARCH 8192

typedef union
{
	struct
	{
		unsigned searched : 1; // already searched
		unsigned found : 1; // offset found
		unsigned unused : 6;
	}b1;

	byte data;
}u_MEMSEARCH;

class CDebugMemoryScanCommand : public CBotCommand
{
public:
	CDebugMemoryScanCommand()
	{
		setName("memoryscan");
		setHelp("usage \"memoryscan <classname> <value> <type = 'bool/int/byte/float'> [store last = 1]\"");
		setAccessLevel(CMD_ACCESS_DEBUG);
		memset(stored_offsets, 0, sizeof(u_MEMSEARCH) * MAX_MEM_SEARCH);
		m_size = 0;
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
private:

	u_MEMSEARCH stored_offsets[MAX_MEM_SEARCH];
	unsigned int m_size;
};

class CDebugNavCommand : public CBotCommand
{
public:
	CDebugNavCommand()
	{
		setName("nav");
		setHelp("usage \"nav 1 or 0, 1 on, 0 off\" : shows navigation output");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugTaskCommand : public CBotCommand
{
public:
	CDebugTaskCommand()
	{
		setName("task");
		setHelp("usage \"task 1 or 0, 1 on, 0 off\" : shows task output");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CGodModeUtilCommand : public CBotCommand
{
public:
	CGodModeUtilCommand()
	{
		setName("god");
		setHelp("usage: toggle for invulnerability!");
		setAccessLevel(CMD_ACCESS_UTIL);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CSetTeleportUtilCommand : public CBotCommand
{
public:
	CSetTeleportUtilCommand()
	{
		setName("set_teleport");
		setHelp("usage: remembers where you want to teleport");
		setAccessLevel(CMD_ACCESS_UTIL);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CTeleportUtilCommand : public CBotCommand
{
public:
	CTeleportUtilCommand()
	{
		setName("teleport");
		setHelp("usage: first use set_teleport, then this command to go there");
		setAccessLevel(CMD_ACCESS_UTIL);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CNoTouchCommand : public CBotCommand
{
public:
	CNoTouchCommand()
	{
		setName("notouch");
		setHelp("don't set off capture points etc");
		setAccessLevel(CMD_ACCESS_UTIL);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CNoClipCommand : public CBotCommand
{
public:
	CNoClipCommand()
	{
		setName("noclip");
		setHelp("fly through walls , yeah!");
		setAccessLevel(CMD_ACCESS_UTIL);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugUtilCommand : public CBotCommand
{
public:
	CDebugUtilCommand()
	{
		setName("util");
		setHelp("usage \"util 1 or 0, 1 on, 0 off\" : shows utility/action output");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugUsercmdCommand : public CBotCommand
{
public:
	CDebugUsercmdCommand()
	{
		setName("usercmd");
		setHelp("usage \"usercmd 1 or 0, 1 on, 0 off\" : shows last user command output");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugEdictsCommand : public CBotCommand
{
public:
	CDebugEdictsCommand()
	{
		setName("edicts");
		setHelp("usage \"edicts 1 or 0, 1 on, 0 off\" : shows allocated/freed edicts");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugSpeedCommand : public CBotCommand
{
public:
	CDebugSpeedCommand()
	{
		setName("speed");
		setHelp("usage \"speed 1 or 0, 1 on, 0 off\" : shows speed");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugAimCommand : public CBotCommand
{
public:
	CDebugAimCommand()
	{
		setName("aim");
		setHelp("usage \"aim 1 or 0, 1 on, 0 off\" : displays aiming accuracy info on the hud");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugChatCommand : public CBotCommand
{
public:
	CDebugChatCommand()
	{
		setName("chat");
		setHelp("usage \"chat 1 or 0, 1 on, 0 off\" : displays logs in chat");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugHudCommand : public CBotCommand
{
public:
	CDebugHudCommand()
	{
		setName("hud");
		setHelp("usage \"hud 1 or 0, 1 on, 0 off\" : displays most important info about bot on the hud");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugButtonsCommand : public CBotCommand
{
public:
	CDebugButtonsCommand()
	{
		setName("buttons");
		setHelp("usage \"buttons 1 or 0, 1 on, 0 off\" : shows buttons bitmask");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugLookCommand : public CBotCommand
{
public:
	CDebugLookCommand()
	{
		setName("look");
		setHelp("usage \"look 1 or 0, 1 on, 0 off\" : shows bot look output");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugThinkCommand : public CBotCommand
{
public:
	CDebugThinkCommand()
	{
		setName("think");
		setHelp("usage \"think 1 or 0, 1 on, 0 off\" : shows bot thinking output");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugVisCommand : public CBotCommand
{
public:
	CDebugVisCommand()
	{
		setName("vis");
		setHelp("usage \"vis 1 or 0, 1 on, 0 off\" : shows bot visibility output");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugGameEventCommand : public CBotCommand
{
public:
	CDebugGameEventCommand()
	{
		setName("gameevent");
		setHelp("usage \"gameevent 1 or 0, 1 on, 0 off\" : shows event output");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class CDebugBotCommand : public CBotCommand
{
public:
	CDebugBotCommand()
	{
		setName("bot");
		setHelp("usage \"bot <partial bot name>, or just bot to switch off : shows bot debug output on listen server");
		setAccessLevel(CMD_ACCESS_DEBUG);
	}

	bool canbeUsedDedicated() { return false; }

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};
//////////////////////////////////

class CSearchCommand : public CBotCommand
{
public:
	CSearchCommand()
	{
		setName("search");
		setAccessLevel(CMD_ACCESS_UTIL);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

/////////////////////////////////
class CKickBotCommand : public CBotCommand
{
public:
	CKickBotCommand()
	{
		setName("kickbot");
		setHelp("usage \"kickbot\" or \"kickbot <team>\" : kicks random bot or bot on team: <team>");
		setAccessLevel(CMD_ACCESS_BOT);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

	virtual bool canbeUsedDedicated() { return true; }
};
////////////////////////////////////////
class CShowUsersCommand : public CBotCommand
{
public:
	CShowUsersCommand()
	{
		setName("show");
		setAccessLevel(CMD_ACCESS_USERS);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

	virtual bool canbeUsedDedicated() { return true; }
};
///////////////////////////////////////
class CPrintCommands : public CBotCommand
{
public:
	CPrintCommands()
	{
		setName("printcommands");
		setAccessLevel(0);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

	virtual bool canbeUsedDedicated() { return true; }
};

class CTestCommand : public CBotCommand
{
public:
	CTestCommand()
	{
		setName("test");
		setAccessLevel(0);
	}

	eBotCommandResult execute(CClient* pClient, const char* pcmd, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

	virtual bool canbeUsedDedicated() { return false; }
};
#endif
