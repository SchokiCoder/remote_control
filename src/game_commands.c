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
#include "constants.h"
#include "town.h"
#include "config.h"
#include "game_commands.h"

void gm_cmd_save(char *p_town_name, struct Town *p_in)
{
	if (save_town(p_town_name, p_in) == 0)
		printf(MSG_FILE_TOWN_SAVE);
}

void gm_cmd_save_as(char *p_town_name, struct Town *p_in)
{
	if (save_town(p_town_name, p_in) == 0)
		printf(MSG_FILE_TOWN_SAVE_AS, p_town_name);
}

void gm_cmd_set(struct Config *p_cfg, char *p_setting_name, char *p_setting_value)
{
	//check which setting should be changed -etc
	if (strcmp(p_setting_name, CFG_SETTING_PATH_FONT) == 0)
	{
		strcpy(p_cfg->path_font, p_setting_value);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_GFX_FRAMERATE) == 0)
	{
		p_cfg->gfx_framerate = strtof(p_setting_value, NULL);
	}

	//window pos size
	else if (strcmp(p_setting_name, CFG_SETTING_GFX_WINDOW_X) == 0)
	{
		p_cfg->gfx_window_x = strtol(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_GFX_WINDOW_Y) == 0)
	{
		p_cfg->gfx_window_y = strtol(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_GFX_WINDOW_W) == 0)
	{
		p_cfg->gfx_window_w = strtol(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_GFX_WINDOW_H) == 0)
	{
		p_cfg->gfx_window_h = strtol(p_setting_value, NULL, 10);
	}

	//bg color
	else if (strcmp(p_setting_name, CFG_SETTING_BG_RED) == 0)
	{
		p_cfg->bg_red = strtoul(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_BG_GREEN) == 0)
	{
		p_cfg->bg_green = strtoul(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_BG_BLUE) == 0)
	{
		p_cfg->bg_blue = strtoul(p_setting_value, NULL, 10);
	}

	//font color
	else if (strcmp(p_setting_name, CFG_SETTING_FONT_RED) == 0)
	{
		p_cfg->font_red = strtoul(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_FONT_GREEN) == 0)
	{
		p_cfg->font_green = strtoul(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_FONT_BLUE) == 0)
	{
		p_cfg->font_blue = strtoul(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_FONT_ALPHA) == 0)
	{
		p_cfg->font_alpha = strtoul(p_setting_value, NULL, 10);
	}

	//field border color
	else if (strcmp(p_setting_name, CFG_SETTING_FIELD_BORDER_RED) == 0)
	{
		p_cfg->field_border_red = strtoul(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_FIELD_BORDER_GREEN) == 0)
	{
		p_cfg->field_border_green = strtoul(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_FIELD_BORDER_BLUE) == 0)
	{
		p_cfg->field_border_blue = strtoul(p_setting_value, NULL, 10);
	}
	else if (strcmp(p_setting_name, CFG_SETTING_FIELD_BORDER_ALPHA) == 0)
	{
		p_cfg->field_border_alpha = strtoul(p_setting_value, NULL, 10);
	}

	//unknown
	else
	{
		printf(MSG_ERR_UNKNOWN_SETTING, p_setting_name);
		return;
	}

	save_config(p_cfg);
}

void gm_cmd_show_config(struct Config *p_cfg)
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
		p_cfg->path_font,
		p_cfg->gfx_framerate,
		p_cfg->gfx_window_x,
		p_cfg->gfx_window_y,
		p_cfg->gfx_window_w,
		p_cfg->gfx_window_h,
		p_cfg->bg_red,
		p_cfg->bg_green,
		p_cfg->bg_blue,
		p_cfg->font_red,
		p_cfg->font_green,
		p_cfg->font_blue,
		p_cfg->font_alpha,
		p_cfg->field_border_red,
		p_cfg->field_border_green,
		p_cfg->field_border_blue,
		p_cfg->field_border_alpha);
}
