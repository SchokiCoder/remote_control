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

void cmd_help_menu( void );

void cmd_help_full( void );

void cmd_list_admins( void );

void cmd_hire_admin( int32_t admin_id, char *town_name );

void cmd_list_towns( void );

void cmd_connect( char *town_name );

void cmd_delete( char *town_name );

#endif /* COMMANDS_H */
