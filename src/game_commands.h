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

#ifndef GAME_COMMANDS_H
#define GAME_COMMANDS_H

#include <stdint.h>

struct Config;

void gm_cmd_save(char* p_town_name, struct Town* p_in);
void gm_cmd_save_as(char* p_town_name, struct Town* p_in);
void gm_cmd_set(struct Config* p_cfg, char* p_setting_name, char* p_setting_value);
void gm_cmd_show_config(struct Config* p_cfg);

#endif
