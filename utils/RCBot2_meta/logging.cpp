/**
 * This file is part of RCBot2.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "logging.h"

#include <cstdarg>
#include <cstdio>

#include "convar.h"

#include "tier0/dbg.h"

static CBotLogger s_Logger;
CBotLogger *logger = &s_Logger;

ConVar rcbot_loglevel("rcbot_loglevel", "2", 0, "Display logging messages with increasing verbosity (higher number = more messages)");

const char* LOGLEVEL_STRINGS[] = {
	"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"
};

void CBotLogger::Log(LogLevel level, const char* fmt, ...) {
	if (level > static_cast<LogLevel>(rcbot_loglevel.GetInt())) {
		return;
	}
	
	char buf[1024];
	
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buf, fmt, argptr); 
	va_end(argptr);
	
	if (level <= LogLevel::WARN) {
		Warning("[RCBot] %s: %s\n", LOGLEVEL_STRINGS[level], buf);
	} else {
		Msg("[RCBot] %s: %s\n", LOGLEVEL_STRINGS[level], buf);
	}
}
