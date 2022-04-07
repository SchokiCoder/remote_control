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
#include "hud.h"
#include "messages.h"
#include "town.h"
#include "config.h"
#include "game_commands.h"
#include "game.h"

void gm_cmd_save( Hud *hud, const char *town_name, Town *town )
{
	SM_String msg = SM_String_contain(MSG_FILE_TOWN_SAVE);

	Town_save(town, town_name);

	if (town->invalid)
		Hud_update_feedback(hud, msg.str);
}

void gm_cmd_save_as( Hud *hud, const char *town_name, Town *town )
{
	SM_String msg = SM_String_new(16);
	SM_String temp = SM_String_contain(MSG_FILE_TOWN_SAVE_AS);

	SM_String_copy(&msg, &temp);
	temp = SM_String_contain(town_name);
	SM_String_copy(&msg, &temp);

	Town_save(town, town_name);

	if (town->invalid)
		Hud_update_feedback(hud, msg.str);

	SM_String_clear(&msg);
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

void gm_cmd_construct( Game *game, Hud *hud, const SDL_Point coord, const Field field )
{
	Game_construct(game, hud, coord, field);

	Hud_update_feedback(hud, GM_MSG_CONSTRUCT);
}

void gm_cmd_destruct( Game *game, Hud *hud, const SDL_Point coord )
{
	Game_construct(game, hud, coord, FIELD_EMPTY);

	Hud_update_feedback(hud, GM_MSG_DESTRUCT);
}
