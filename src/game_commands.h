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

typedef struct Config Config;
typedef struct Town Town;
typedef struct Hud Hud;

//static const char GM_CMD_HELP[] = "help";
static const char GM_CMD_SAVE[] = "save";
static const char GM_CMD_SAVE_AS[] = "save-as";
static const char GM_CMD_EXIT[] = "exit";
static const char GM_CMD_CONFIG_SET[] = "config-set";
//static const char GM_CMD_CONFIG_SHOW[] = "config-show";
static const char GM_CMD_PASS[] = "pass";
static const char GM_CMD_CONSTRUCT[] = "construct";
static const char GM_CMD_DESTRUCT[] = "destruct";

void gm_cmd_save( Hud *hud, const char *town_name, Town *town );

void gm_cmd_save_as( Hud *hud, const char *town_name, Town *town );

void gm_cmd_config_set(Hud *hud, Config *cfg, const char *setting_name, const char *setting_value );

//void gm_cmd_config_show( const Config *p_cfg );

void gm_cmd_pass( Game *game, Hud *hud );

void gm_cmd_construct( Game *game, Hud *hud, const SDL_Point coord, const Field field );

void gm_cmd_destruct( Game *game, Hud *hud, const SDL_Point coord );

#endif /* GAME_COMMANDS_H */
