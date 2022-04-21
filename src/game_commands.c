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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SM_string.h>
#include <SM_crypto.h>
#include "hud.h"
#include "messages.h"
#include "town.h"
#include "config.h"
#include "game_commands.h"
#include "game.h"

void gm_cmd_save( Hud *hud, const char *town_name, Town *town )
{
	Town_save(town, town_name);

	if (town->invalid)
		Hud_update_feedback(hud, GM_MSG_ERR_TOWN_SAVE);
	else
	{
		SM_String msg = SM_String_from(GM_MSG_TOWN_SAVE);
		SM_String_append_cstr(&msg, ".");

		Hud_update_feedback(hud, msg.str);
		SM_String_clear(&msg);
	}
}

void gm_cmd_save_as( Hud *hud, const char *town_name, Town *town )
{
	Town_save(town, town_name);

	if (town->invalid)
		Hud_update_feedback(hud, GM_MSG_ERR_TOWN_SAVE);
	else
	{
		SM_String msg = SM_String_from(GM_MSG_TOWN_SAVE);
		SM_String_append_cstr(&msg, " as ");
		SM_String_append_cstr(&msg, town_name);
		SM_String_append_cstr(&msg, ".");

		Hud_update_feedback(hud, msg.str);
		SM_String_clear(&msg);
	}
}

void gm_cmd_config_set( Hud *hud, Config *cfg, const char *setting_name, const char *setting_value )
{
	//check which setting should be changed -etc
	if (strcmp(setting_name, CFG_SETTING_PATH_FONT) == 0)
	{
		strcpy(cfg->path_font, setting_value);
	}
	else if (strcmp(setting_name, CFG_SETTING_GFX_FRAMERATE) == 0)
	{
		cfg->gfx_framerate = strtof(setting_value, NULL);
	}

	//window pos size
	else if (strcmp(setting_name, CFG_SETTING_GFX_WINDOW_X) == 0)
	{
		cfg->gfx_window_x = strtol(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_GFX_WINDOW_Y) == 0)
	{
		cfg->gfx_window_y = strtol(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_GFX_WINDOW_W) == 0)
	{
		cfg->gfx_window_w = strtol(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_GFX_WINDOW_H) == 0)
	{
		cfg->gfx_window_h = strtol(setting_value, NULL, 10);
	}

	//bg color
	else if (strcmp(setting_name, CFG_SETTING_BG_RED) == 0)
	{
		cfg->bg_red = strtoul(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_BG_GREEN) == 0)
	{
		cfg->bg_green = strtoul(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_BG_BLUE) == 0)
	{
		cfg->bg_blue = strtoul(setting_value, NULL, 10);
	}

	//font color
	else if (strcmp(setting_name, CFG_SETTING_FONT_RED) == 0)
	{
		cfg->font_red = strtoul(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_FONT_GREEN) == 0)
	{
		cfg->font_green = strtoul(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_FONT_BLUE) == 0)
	{
		cfg->font_blue = strtoul(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_FONT_ALPHA) == 0)
	{
		cfg->font_alpha = strtoul(setting_value, NULL, 10);
	}

	//field border color
	else if (strcmp(setting_name, CFG_SETTING_FIELD_BORDER_RED) == 0)
	{
		cfg->field_border_red = strtoul(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_FIELD_BORDER_GREEN) == 0)
	{
		cfg->field_border_green = strtoul(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_FIELD_BORDER_BLUE) == 0)
	{
		cfg->field_border_blue = strtoul(setting_value, NULL, 10);
	}
	else if (strcmp(setting_name, CFG_SETTING_FIELD_BORDER_ALPHA) == 0)
	{
		cfg->field_border_alpha = strtoul(setting_value, NULL, 10);
	}

	//unknown
	else
	{
		Hud_update_feedback(hud, GM_MSG_ERR_CONFIG_UNKNOWN_SETTING);
		return;
	}

	Hud_update_feedback(hud, GM_MSG_CFG_SET);

	Config_save(cfg);
}

/* due to technical limitations at the moment
void gm_cmd_config_show( const Config *cfg )
{
	printf("" \
		CFG_SETTING_PATH_FONT ": %s\n"
		CFG_SETTING_GFX_FRAMERATE ": %f\n" \
		CFG_SETTING_GFX_WINDOW_X ": %i\n" \
		CFG_SETTING_GFX_WINDOW_Y ": %i\n" \
		CFG_SETTING_GFX_WINDOW_W ": %i\n" \
		CFG_SETTING_GFX_WINDOW_H ": %i\n" \
		CFG_SETTING_BG_RED ": %u\n" \
		CFG_SETTING_BG_GREEN ": %u\n" \
		CFG_SETTING_BG_BLUE ": %u\n" \
		CFG_SETTING_FONT_RED ": %u\n" \
		CFG_SETTING_FONT_GREEN ": %u\n" \
		CFG_SETTING_FONT_BLUE ": %u\n" \
		CFG_SETTING_FONT_ALPHA ": %u\n" \
		CFG_SETTING_FIELD_BORDER_RED ": %u\n" \
		CFG_SETTING_FIELD_BORDER_GREEN ": %u\n" \
		CFG_SETTING_FIELD_BORDER_BLUE ": %u\n" \
		CFG_SETTING_FIELD_BORDER_ALPHA ": %u\n",
		cfg->path_font,
		cfg->gfx_framerate,
		cfg->gfx_window_x,
		cfg->gfx_window_y,
		cfg->gfx_window_w,
		cfg->gfx_window_h,
		cfg->bg_red,
		cfg->bg_green,
		cfg->bg_blue,
		cfg->font_red,
		cfg->font_green,
		cfg->font_blue,
		cfg->font_alpha,
		cfg->field_border_red,
		cfg->field_border_green,
		cfg->field_border_blue,
		cfg->field_border_alpha);
}
*/

void gm_cmd_pass( Game *game, Hud *hud )
{
	Game_end_round(game, hud);

	Hud_update_feedback(hud, GM_MSG_PASS);
}

#ifdef _DEBUG
void gm_cmd_print_djb2( void )
{
	FILE *file = fopen("djb2_gmhash.txt", "w");

	fprintf(file, "%-32s| %-10s | %-10s\n", "cmd", "name", "abbr");

	for (uint32_t i = 0; i <= GM_CMD_LAST; i++)
	{
		if (DATA_GM_CMDS[i].has_abbr)
		{
			fprintf(file,
				"%-32s| %-10u | %-10u\n",
				DATA_GM_CMDS[i].name,
				SM_djb2_encode(DATA_GM_CMDS[i].name),
				SM_djb2_encode(DATA_GM_CMDS[i].abbr));
		}
		else
		{
			fprintf(file,
				"%-32s| %-10u |\n",
				DATA_GM_CMDS[i].name,
				SM_djb2_encode(DATA_GM_CMDS[i].name));
		}
	}

	fclose(file);
}

void gm_cmd_spawn_merc(
	Game *game, Hud *hud,
	const SDL_Point coord,
	const Mercenary merc_id,
	const MercFraction frac_id )
{
	TownMerc merc = {
		.id = merc_id,
		.coords = coord,
		.hp = DATA_MERCENARIES[merc_id].max_hp,
		.fraction = frac_id,
	};

	if (Game_spawn_merc(game, hud, merc))
		Hud_update_feedback(hud, GM_MSG_MERC_SPAWN);

	else
		Hud_update_feedback(hud, GM_MSG_MERC_NO_SPAWN);
}
#endif // _DEBUG

void gm_cmd_merc_move( Game *game, Hud *hud, const SDL_Point src_coord, const SDL_Point dest_coord )
{
    if (Game_move_merc(game, hud, src_coord, dest_coord) == true)
    	Hud_update_feedback(hud, GM_MSG_MERC_MOVE);
	else
		Hud_update_feedback(hud, GM_MSG_MERC_NO_MOVE);
}

void gm_cmd_merc_attack(
	Game *game, Hud *hud,
	const SDL_Point src_coord,
	const uint_fast8_t weapon_slot,
	const SDL_Point dest_coord )
{
	char dmg_no[10];

	SM_String msg = SM_String_from("Mercenary dealt ");
	sprintf(dmg_no, "%li", Game_merc_attack(game, src_coord, weapon_slot, dest_coord));
	SM_String_append_cstr(&msg, dmg_no);
	SM_String_append_cstr(&msg, " damage.");

	Hud_update_feedback(hud, msg.str);

	SM_String_clear(&msg);
}

void gm_cmd_construct( Game *game, Hud *hud, const SDL_Point coord, const Field field )
{
	if (Game_construct(game, hud, coord, field))
		Hud_update_feedback(hud, GM_MSG_CONSTRUCT);

	else
		Hud_update_feedback(hud, GM_MSG_NO_CONSTRUCT);
}

void gm_cmd_destruct( Game *game, Hud *hud, const SDL_Point coord )
{
	if (Game_construct(game, hud, coord, FIELD_EMPTY))
		Hud_update_feedback(hud, GM_MSG_DESTRUCT);

	else
		Hud_update_feedback(hud, GM_MSG_NO_DESTRUCT);
}
