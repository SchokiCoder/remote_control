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

/* commands */
static const char CMD_HELP[] =				"h";
static const char CMD_HELP_LONG[] =			"help";
static const char CMD_LIST_ADMINS[] =		"la";
static const char CMD_LIST_ADMINS_LONG[] =	"list-admins";
static const char CMD_HIRE_ADMIN[] =		"ha";			/* (create new game) */
static const char CMD_HIRE_ADMIN_LONG[] =	"hire-admin";
static const char CMD_LIST_TOWNS[] =		"lt";
static const char CMD_LIST_TOWNS_LONG[] =	"list-towns";
static const char CMD_CONNECT[] =			"c";			/* start playing game, inits the mainloop */
static const char CMD_CONNECT_LONG[] =		"connect";
static const char CMD_DELETE_TOWN_LONG[] =	"delete";

#endif /* DEF_COMMANDS_H */
