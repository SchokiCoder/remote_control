/*
	remote_control
	Copyright (C) 2021	Andy Frank Schoknecht

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct CommandData
{
	char *name;
	bool has_abbr;
	char *abbr;
	char *desc;
	bool has_args;
	char *args;
} CommandData ;

typedef enum Command
{
	CMD_HELP,
	CMD_LIST_ADMINS,
	CMD_HIRE_ADMIN,
	CMD_LIST_TOWNS,
	CMD_CONNECT,
	CMD_DELETE_TOWN,

	CMD_FIRST = CMD_HELP,
	CMD_LAST = CMD_DELETE_TOWN,
} Command ;

typedef enum CommandDJB2
{
    DJB2_HELP = 2900064876,
    DJB2_HELP_ABBR = 183058,
    DJB2_LIST_ADMINS = 2152484539,
    DJB2_LIST_ADMINS_ABBR = 6224205,
    DJB2_HIRE_ADMIN = 1600892068,
    DJB2_HIRE_ADMIN_ABBR = 6224069,
    DJB2_LIST_TOWNS = 468110531,
    DJB2_LIST_TOWNS_ABBR = 6224224,
    DJB2_CONNECT = 1335798814,
    DJB2_CONNECT_ABBR = 183053,
    DJB2_DELETE = 2218755409,
} CommandDJB2 ;

static const CommandData DATA_CMDS[] = {
	{"help", true, "h", "shows this message", false, ""},
	{"list-admins", true, "la", "list all available administrators and their attributes", false, ""},
	{"hire-admin", true, "ha", "hire an admin to watch over another place", true, "ADMIN_ID TOWN_NAME"},
	{"list-towns", true, "lt", "list towns that are already controlled by you", false, ""},
	{"connect", true, "c", "connect to a towns administrator and get to work", true, "TOWN_NAME"},
	{"delete", false, "", "delete all files of a given town", true, "TOWN_NAME"},
};

void cmd_help_menu( void );

void cmd_help_full( void );

void cmd_list_admins( void );

void cmd_hire_admin( const int32_t admin_id, const char *town_name );

void cmd_list_towns( void );

void cmd_connect( const char *town_name );

void cmd_delete( const char *town_name );

#endif /* COMMANDS_H */
