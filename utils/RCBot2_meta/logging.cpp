/**
 * Copyright 2021 nosoop
 * 
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE 
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "logging.h"

#if defined WIN32
	// for SetConsoleTextAttribute and co.
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <cstdarg>
#include <cstdio>

#include "icvar.h"
#include "convar.h"

#include "engine_wrappers.h"

#include "tier0/icommandline.h"
#include "tier0/dbg.h"

static CBotLogger s_Logger;
CBotLogger *logger = &s_Logger;

ConVar rcbot_loglevel("rcbot_loglevel", "2", 0, "Display logging messages with increasing verbosity (higher number = more messages)");

const char* LOGLEVEL_STRINGS[] = {
	"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"
};

const char* LOGLEVEL_ANSI_COLORS[] = {
	"\x1B[1;31m", "\x1B[1;91m", "\x1B[1;33m", "\x1B[1;92m", "\x1B[1;94m", "\x1B[1;96m"
};

#if defined WIN32
#define FOREGROUND_YELLOW (FOREGROUND_GREEN | FOREGROUND_RED)
#define FOREGROUND_WHITE  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
const DWORD LOGLEVEL_WINCON_COLORS[] = {
	FOREGROUND_RED,
	FOREGROUND_RED | FOREGROUND_INTENSITY,
	FOREGROUND_YELLOW,
	FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};
#endif

// colors based on https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
const Color LOGLEVEL_CONSOLE_COLORS[] = {
	{ 197,  15,  31, 255 },
	{ 231,  72,  75, 255 },
	{ 193, 156,   0, 255 },
	{  19, 198,  13, 255 },
	{  59, 120, 255, 255 },
	{  97, 214, 214, 255 },
};

enum MessageColorizationMode {
	Colorize_None,
	
	// ANSI escapes
	Colorize_ANSI,
	
	#if defined WIN32
	// windows console -- uses console text attributes
	Colorize_WinConsole,
	#endif
	
	Colorize_ClientConsole,
};

MessageColorizationMode GetMessageColorizationMode() {
	#if defined _LINUX
		if (!engine->IsDedicatedServer()) {
			return Colorize_ClientConsole;
		} else {
			return Colorize_ANSI;
		}
	#elif defined WIN32
		if (!engine->IsDedicatedServer()) {
			return Colorize_ClientConsole;
		} else if (CommandLine()->CheckParm("-console") != nullptr) {
			return Colorize_WinConsole;
		}
	#endif
	return Colorize_None;
}

void CBotLogger::Log(LogLevel level, const char* fmt, ...) {
	if (level > static_cast<LogLevel>(rcbot_loglevel.GetInt())) {
		return;
	}
	
	char buf[1024];
	
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buf, fmt, argptr); 
	va_end(argptr);
	
	switch (GetMessageColorizationMode()) {
		case Colorize_ANSI:
			if (level <= LogLevel::WARN) {
				Warning("%s[RCBot] %s: %s\x1B[0m\n", LOGLEVEL_ANSI_COLORS[level],
						LOGLEVEL_STRINGS[level], buf);
			} else {
				Msg("%s[RCBot] %s: %s\x1B[0m\n", LOGLEVEL_ANSI_COLORS[level],
						LOGLEVEL_STRINGS[level], buf);
			}
			break;
		#if defined WIN32
		case Colorize_WinConsole:
			HANDLE hConsoleHandle;
			hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			
			SetConsoleTextAttribute(hConsoleHandle, LOGLEVEL_WINCON_COLORS[level]);
			
			Msg("[RCBot] %s: %s\n", LOGLEVEL_STRINGS[level], buf);
			SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_WHITE);
			break;
		#endif
		case Colorize_ClientConsole:
			extern ICvar *icvar;
			icvar->ConsoleColorPrintf(LOGLEVEL_CONSOLE_COLORS[level], "[RCBot] %s: %s\n", LOGLEVEL_STRINGS[level], buf);
			break;
		case Colorize_None:
		default:
			if (level <= LogLevel::WARN) {
				Warning("[RCBot] %s: %s\n", LOGLEVEL_STRINGS[level], buf);
			} else {
				Msg("[RCBot] %s: %s\n", LOGLEVEL_STRINGS[level], buf);
			}
			break;
	}
}
