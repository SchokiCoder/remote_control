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

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SM_dict.h>
#include "messages.h"
#include "path.h"
#include "config.h"

Config Config_new( void )
{
	Config cfg = {
		.invalid = false,
		.gfx_framerate = CFG_STD_GFX_FRAMERATE,
		.gfx_window_x = CFG_STD_GFX_WINDOW_X,
		.gfx_window_y = CFG_STD_GFX_WINDOW_Y,
		.gfx_window_w = CFG_STD_GFX_WINDOW_W,
		.gfx_window_h = CFG_STD_GFX_WINDOW_H,
		.bg_red = CFG_STD_BG_RED,
		.bg_green = CFG_STD_BG_GREEN,
		.bg_blue = CFG_STD_BG_BLUE,
		.font_red = CFG_STD_FONT_RED,
		.font_green = CFG_STD_FONT_GREEN,
		.font_blue = CFG_STD_FONT_BLUE,
		.font_alpha = CFG_STD_FONT_ALPHA,
		.field_border_red = CFG_STD_FIELD_BORDER_RED,
		.field_border_green = CFG_STD_FIELD_BORDER_GREEN,
		.field_border_blue = CFG_STD_FIELD_BORDER_BLUE,
		.field_border_alpha = CFG_STD_FIELD_BORDER_ALPHA,
	};

	strncpy(cfg.path_font, CFG_STD_PATH_FONT, CFG_SETTING_PATH_FONT_MAX_LEN);

	return cfg;
}

void Config_load( Config *cfg )
{
	SM_String filepath = SM_String_new(16);

	// get path
	if (get_config_path(&filepath) != 0)
	{
		SM_String_clear(&filepath);
		cfg->invalid = true;
		return;
	}

	// read file
	SM_Dict dict = SM_Dict_from_file(filepath.str);

	if (!dict.valid)
	{
		SM_String_clear(&filepath);
		cfg->invalid = true;
		printf(MSG_WARN_CONFIG_LOAD);
		return;
	}

	// convert dict into config
    for (size_t i = 0; i < dict.len; i++)
    {
    	if (SM_strequal(dict.data[i].key.str, CFG_SETTING_PATH_FONT))
			strncpy(cfg->path_font, dict.data[i].value.str, CFG_SETTING_PATH_FONT_MAX_LEN);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_GFX_FRAMERATE))
			cfg->gfx_framerate = strtof(dict.data[i].value.str, NULL);

		// window pos, size
		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_GFX_WINDOW_X))
			cfg->gfx_window_x = strtol(dict.data[i].value.str, NULL, 10);

		else if(SM_strequal(dict.data[i].key.str, CFG_SETTING_GFX_WINDOW_Y))
			cfg->gfx_window_y = strtol(dict.data[i].value.str, NULL, 10);

		else if(SM_strequal(dict.data[i].key.str, CFG_SETTING_GFX_WINDOW_W))
			cfg->gfx_window_w = strtol(dict.data[i].value.str, NULL, 10);

		else if(SM_strequal(dict.data[i].key.str, CFG_SETTING_GFX_WINDOW_H))
			cfg->gfx_window_h = strtol(dict.data[i].value.str, NULL, 10);

		// bg color
		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_BG_RED))
			cfg->bg_red = strtoul(dict.data[i].value.str, NULL, 10);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_BG_GREEN))
			cfg->bg_green = strtoul(dict.data[i].value.str, NULL, 10);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_BG_BLUE))
			cfg->bg_blue = strtoul(dict.data[i].value.str, NULL, 10);

		// font color
		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_FONT_RED))
			cfg->font_red = strtoul(dict.data[i].value.str, NULL, 10);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_FONT_GREEN))
			cfg->font_green = strtoul(dict.data[i].value.str, NULL, 10);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_FONT_BLUE))
			cfg->font_blue = strtoul(dict.data[i].value.str, NULL, 10);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_FONT_ALPHA))
			cfg->font_alpha = strtoul(dict.data[i].value.str, NULL, 10);

		// field border color
		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_FIELD_BORDER_RED))
			cfg->field_border_red = strtoul(dict.data[i].value.str, NULL, 10);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_FIELD_BORDER_GREEN))
			cfg->field_border_green = strtoul(dict.data[i].value.str, NULL, 10);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_FIELD_BORDER_BLUE))
			cfg->field_border_blue = strtoul(dict.data[i].value.str, NULL, 10);

		else if (SM_strequal(dict.data[i].key.str, CFG_SETTING_FIELD_BORDER_ALPHA))
			cfg->field_border_alpha = strtoul(dict.data[i].value.str, NULL, 10);

		// unknown option
		else
			printf(MSG_WARN_UNKNOWN_SETTING, dict.data[i].key.str);
	}

	SM_String_clear(&filepath);
	SM_Dict_clear(&dict);
}

void Config_save( Config *cfg )
{
	SM_String filepath = SM_String_new(16);

	/* get path */
	if (get_config_path(&filepath) != 0)
	{
		SM_String_clear(&filepath);
		cfg->invalid = true;
		return;
	}

	// convert config into dict
	SM_Dict dict = SM_Dict_new(1);
	char temp[10];

	SM_Dict_add(&dict, CFG_SETTING_PATH_FONT, cfg->path_font);
	sprintf(temp, "%f", cfg->gfx_framerate);
	SM_Dict_add(&dict, CFG_SETTING_GFX_FRAMERATE, temp);

	sprintf(temp, "%i", cfg->gfx_window_x);
	SM_Dict_add(&dict, CFG_SETTING_GFX_WINDOW_X, temp);
	sprintf(temp, "%i", cfg->gfx_window_y);
	SM_Dict_add(&dict, CFG_SETTING_GFX_WINDOW_Y, temp);
	sprintf(temp, "%i", cfg->gfx_window_w);
	SM_Dict_add(&dict, CFG_SETTING_GFX_WINDOW_W, temp);
	sprintf(temp, "%i", cfg->gfx_window_h);
	SM_Dict_add(&dict, CFG_SETTING_GFX_WINDOW_H, temp);

	sprintf(temp, "%u", cfg->bg_red);
	SM_Dict_add(&dict, CFG_SETTING_BG_RED, temp);
	sprintf(temp, "%u", cfg->bg_green);
	SM_Dict_add(&dict, CFG_SETTING_BG_GREEN, temp);
	sprintf(temp, "%u", cfg->bg_blue);
	SM_Dict_add(&dict, CFG_SETTING_BG_BLUE, temp);

	sprintf(temp, "%u", cfg->font_red);
	SM_Dict_add(&dict, CFG_SETTING_FONT_RED, temp);
	sprintf(temp, "%u", cfg->font_green);
	SM_Dict_add(&dict, CFG_SETTING_FONT_GREEN, temp);
	sprintf(temp, "%u", cfg->font_blue);
	SM_Dict_add(&dict, CFG_SETTING_FONT_BLUE, temp);
	sprintf(temp, "%u", cfg->font_alpha);
	SM_Dict_add(&dict, CFG_SETTING_FONT_ALPHA, temp);

	sprintf(temp, "%u", cfg->field_border_red);
	SM_Dict_add(&dict, CFG_SETTING_FIELD_BORDER_RED, temp);
	sprintf(temp, "%u", cfg->field_border_green);
	SM_Dict_add(&dict, CFG_SETTING_FIELD_BORDER_GREEN, temp);
	sprintf(temp, "%u", cfg->field_border_blue);
	SM_Dict_add(&dict, CFG_SETTING_FIELD_BORDER_BLUE, temp);
	sprintf(temp, "%u", cfg->field_border_alpha);
	SM_Dict_add(&dict, CFG_SETTING_FIELD_BORDER_ALPHA, temp);

	// save
	if (!SM_Dict_write(&dict, filepath.str))
		cfg->invalid = true;

	// clear
	SM_String_clear(&filepath);
	SM_Dict_clear(&dict);
}
