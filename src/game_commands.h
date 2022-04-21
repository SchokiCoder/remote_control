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
#include "commands.h"

typedef struct Config Config;
typedef struct Town Town;
typedef struct Hud Hud;

typedef enum GameCommand
{
	//GM_CMD_HELP,
	GM_CMD_SAVE,
	GM_CMD_SAVE_AS,
	GM_CMD_EXIT,
	GM_CMD_CONFIG_SET,
	//GM_CMD_CONFIG_SHOW,
	GM_CMD_PASS,

#ifdef _DEBUG
	GM_CMD_PRINT_DJB2,
	GM_CMD_SPAWN_MERC,
	GM_CMD_HURT_MERC,
#endif

	GM_CMD_MERC_MOVE,
	GM_CMD_MERC_ATTACK,
	GM_CMD_CONSTRUCT,
	GM_CMD_DESTRUCT,

	GM_CMD_FIRST = GM_CMD_SAVE,
	GM_CMD_LAST = GM_CMD_DESTRUCT
} GameCommand ;

typedef enum GameCommandDJB2
{
	//DJB2_GM_HELP = ,
	DJB2_GM_SAVE = 2900492925,
	DJB2_GM_SAVE_AS = 3952009201,
	DJB2_GM_EXIT = 2899968830,
	DJB2_GM_CONFIG_SET = 1267258658,
	//DJB2_GM_CONFIG_SHOW = ,
	DJB2_GM_PASS = 2900374925,

#ifdef _DEBUG
	DJB2_GM_PRINT_DJB2 = 2572032878,
	DJB2_GM_SPAWN_MERC = 2983389811,
	DJB2_GM_HURT_MERC = 1484431347,
#endif

	DJB2_GM_MERC_MOVE = 1817727845,
	DJB2_GM_MERC_MOVE_ABBR = 6224251,
	DJB2_GM_MERC_ATTACK = 515757909,
	DJB2_GM_MERC_ATTACK_ABBR = 6224239,
	DJB2_GM_CONSTRUCT = 2537986078,
	DJB2_GM_CONSTRUCT_ABBR = 183053,
	DJB2_GM_DESTRUCT = 1123813470,
	DJB2_GM_DESTRUCT_ABBR = 183054,
} GameCommandDJB2 ;

static const CommandData DATA_GM_CMDS[] = {
    //{"help", true, "h", "shows this message", false, ""},
    {"save", false, "", "save current town file", false, ""},
    {"save-as", false, "", "save current town with name", true, "TOWN_NAME"},
    {"exit", false, "", "close connection to town", false, ""},
    {"config-set", false, "", "set config value", true, "VARIABLE_NAME VALUE"},
    //{"config-show", false, "", "show all current config values", false, ""},
    {"pass", false, "", "pass time", false, ""},

#ifdef _DEBUG
	{"print_djb2", false, "", "", false, ""},
	{"spawn_merc", false, "", "", true, "X Y MERC_ID FRACTION_ID"},
	{"hurt_merc", false, "", "", true, "X Y HP"},
#endif

    {"merc-move", true, "mm", "move a mercenary", true, "SRC_X SRC_Y DEST_X DEST_Y"},
    {"merc-attack", true, "ma", "attack a coordinate", true, "SRC_X SRC_Y SLOT DEST_Y DEST_Y"},
    {"construct", true, "c", "start construction", true, "X Y CONSTRUCTION"},
    {"destruct", true, "d", "start destruction", true, "X Y"},
};

void gm_cmd_save( Hud *hud, const char *town_name, Town *town );

void gm_cmd_save_as( Hud *hud, const char *town_name, Town *town );

void gm_cmd_config_set(Hud *hud, Config *cfg, const char *setting_name, const char *setting_value );

//void gm_cmd_config_show( const Config *p_cfg );

void gm_cmd_pass( Game *game, Hud *hud );

#ifdef _DEBUG
void gm_cmd_print_djb2( void );

void gm_cmd_spawn_merc(
	Game *game, Hud *hud,
	const SDL_Point coord,
	const Mercenary merc_id,
	const MercFraction frac_id );

void gm_cmd_hurt_merc( Game *game, Hud *hud, const SDL_Point coord, const uint_fast32_t hp );
#endif

void gm_cmd_merc_move( Game *game, Hud *hud, const SDL_Point src_coord, const SDL_Point dest_coord );

void gm_cmd_merc_attack(
	Game *game, Hud *hud,
	const SDL_Point src_coord,
	const uint_fast8_t weapon_slot,
	const SDL_Point dest_coord );

void gm_cmd_construct( Game *game, Hud *hud, const SDL_Point coord, const Field field );

void gm_cmd_destruct( Game *game, Hud *hud, const SDL_Point coord );

#endif /* GAME_COMMANDS_H */
