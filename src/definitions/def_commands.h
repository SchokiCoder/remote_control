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

#ifndef DEF_COMMANDS_H
#define DEF_COMMANDS_H

//commands
#define CMD_HELP				"h"
#define CMD_HELP_LONG			"help"
#define CMD_LIST_ADMINS			"la"
#define CMD_LIST_ADMINS_LONG	"list-admins"
#define CMD_HIRE_ADMIN			"ha"	/* create new game */
#define CMD_HIRE_ADMIN_LONG		"hire-admin"
#define CMD_LIST_TOWNS			"lt"
#define CMD_LIST_TOWNS_LONG		"list-towns"
#define CMD_CONNECT				"c"		/* start playing game, inits the mainloop */
#define CMD_CONNECT_LONG		"connect"
#define CMD_DELETE_TOWN_LONG	"delete"

//technical limitations of terminal in-game commands
#define TRM_MAX_ARGS	8
#define TRM_MAX_ARG_LEN 32
#define TRM_MAX_IN_LEN	(TRM_MAX_ARGS * TRM_MAX_ARG_LEN)

//in-game comands
#define GM_CMD_HELP				"help"
#define GM_CMD_SAVE				"save"
#define GM_CMD_SAVE_AS			"save-as"
#define GM_CMD_EXIT				"exit"
#define GM_CMD_SHOW_CONFIG		"show-config"
#define GM_CMD_SET				"set"

//in-game commands (gameplay)
#define GM_CMD_PASS				"pass"

#endif /* DEF_COMMANDS_H */
