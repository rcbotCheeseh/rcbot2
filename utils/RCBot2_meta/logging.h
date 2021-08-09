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

#ifndef __RCBOT2_LOGGER_H__
#define __RCBOT2_LOGGER_H__

/**
 * Log levels in ascending order.
 */
enum LogLevel {
	FATAL, ERROR, WARN, INFO, DEBUG, TRACE
};

class CBotLogger {
	public:
	void Log(LogLevel level, const char* fmt, ...);
};

extern CBotLogger *logger;

#endif
