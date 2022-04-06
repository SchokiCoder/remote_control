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
#include "messages.h"
#include "path.h"
#include "config.h"

Config Config_new( void )
{
	Config result = {
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

	strncpy(result.path_font, CFG_STD_PATH_FONT, CFG_SETTING_PATH_FONT_MAX_LEN);

	return result;
}

void Config_load( Config *cfg )
{
	FILE *f;
	SM_String filepath = SM_String_new(16);
	int32_t buf;
	uint32_t line = 0;
	uint32_t setting_len = 0;
	uint32_t value_len = 0;
	bool read_setting = true;
	char cfg_settings[CONFIG_MAX_LINES][CONFIG_MAX_SETTING_LEN];
	char cfg_values[CONFIG_MAX_LINES][CONFIG_MAX_VALUE_LEN];

	/* get path */
	if (get_config_path(&filepath) != 0)
	{
		SM_String_clear(&filepath);
		cfg->invalid = true;
		return;
	}

	/* open file */
	f = fopen(filepath.str, "r");

	if (f == NULL)
	{
		SM_String_clear(&filepath);
		cfg->invalid = true;
		printf(MSG_WARN_CONFIG_LOAD);
		return;
	}

	/* read */
	while ((buf = fgetc(f)) != EOF)
	{
		switch (buf)
		{
		case ' ':
			/* ignore spaces */
			break;

		case '=':
			/* start reading value, add '\0' to setting */
			read_setting = false;
			cfg_settings[line][setting_len] = '\0';
			setting_len++;
			break;

		case '\n':
			/* add '\0' to value */
			cfg_values[line][value_len] = '\0';
			value_len++;

			/* jump to next line, reset values */
			line++;
			read_setting = true;
			setting_len = 0;
			value_len = 0;
			break;

		default:
			/* if comma did not yet occur */
			if (read_setting == true)
			{
				/* read setting-name */
				cfg_settings[line][setting_len] = (unsigned char) buf;
				setting_len++;
			}
			else
			{
				/* else read value */
				cfg_values[line][value_len] = (unsigned char) buf;
				value_len++;
			}
			break;
		}
	}

	/* if last line is empty, decrement line counter */
	if (strcmp(cfg_settings[line], "") == 0)
	{
		line--;
	}

	/* convert strings into values */
	for (uint32_t i = 0; i < (line + 1); i++)
	{
		/* etc */
		if (strcmp(cfg_settings[i], CFG_SETTING_PATH_FONT) == 0)
		{
			strncpy(cfg->path_font, cfg_values[i], CFG_SETTING_PATH_FONT_MAX_LEN);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_GFX_FRAMERATE) == 0)
		{
			cfg->gfx_framerate = strtof(cfg_values[i], NULL);
		}

		/* window pos, size */
		else if (strcmp(cfg_settings[i], CFG_SETTING_GFX_WINDOW_X) == 0)
		{
			cfg->gfx_window_x = strtol(cfg_values[i], NULL, 10);
		}
		else if(strcmp(cfg_settings[i], CFG_SETTING_GFX_WINDOW_Y) == 0)
		{
			cfg->gfx_window_y = strtol(cfg_values[i], NULL, 10);
		}
		else if(strcmp(cfg_settings[i], CFG_SETTING_GFX_WINDOW_W) == 0)
		{
			cfg->gfx_window_w = strtol(cfg_values[i], NULL, 10);
		}
		else if(strcmp(cfg_settings[i], CFG_SETTING_GFX_WINDOW_H) == 0)
		{
			cfg->gfx_window_h = strtol(cfg_values[i], NULL, 10);
		}

		/* bg color */
		else if (strcmp(cfg_settings[i], CFG_SETTING_BG_RED) == 0)
		{
			cfg->bg_red = strtoul(cfg_values[i], NULL, 10);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_BG_GREEN) == 0)
		{
			cfg->bg_green = strtoul(cfg_values[i], NULL, 10);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_BG_BLUE) == 0)
		{
			cfg->bg_blue = strtoul(cfg_values[i], NULL, 10);
		}

		/* font color */
		else if (strcmp(cfg_settings[i], CFG_SETTING_FONT_RED) == 0)
		{
			cfg->font_red = strtoul(cfg_values[i], NULL, 10);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_FONT_GREEN) == 0)
		{
			cfg->font_green = strtoul(cfg_values[i], NULL, 10);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_FONT_BLUE) == 0)
		{
			cfg->font_blue = strtoul(cfg_values[i], NULL, 10);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_FONT_ALPHA) == 0)
		{
			cfg->font_alpha = strtoul(cfg_values[i], NULL, 10);
		}

		/* field border color */
		else if (strcmp(cfg_settings[i], CFG_SETTING_FIELD_BORDER_RED) == 0)
		{
			cfg->field_border_red = strtoul(cfg_values[i], NULL, 10);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_FIELD_BORDER_GREEN) == 0)
		{
			cfg->field_border_green = strtoul(cfg_values[i], NULL, 10);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_FIELD_BORDER_BLUE) == 0)
		{
			cfg->field_border_blue = strtoul(cfg_values[i], NULL, 10);
		}
		else if (strcmp(cfg_settings[i], CFG_SETTING_FIELD_BORDER_ALPHA) == 0)
		{
			cfg->field_border_alpha = strtoul(cfg_values[i], NULL, 10);
		}

		/* unknown option */
		else
		{
			printf(MSG_WARN_UNKNOWN_SETTING, cfg_settings[i]);
		}
	}

	fclose(f);
	SM_String_clear(&filepath);
}

void Config_save( Config *cfg )
{
	FILE *f;
	SM_String filepath = SM_String_new(16);
	const char *delim = " = ";

	/* get path */
	if (get_config_path(&filepath) != 0)
	{
		SM_String_clear(&filepath);
		cfg->invalid = true;
		return;
	}

	/* open */
	f = fopen(filepath.str, "w");

	if (f == NULL)
	{
		SM_String_clear(&filepath);
		cfg->invalid = true;
		printf(MSG_WARN_CONFIG_SAVE);
		return;
	}

	/* write - etc */
	fputs(CFG_SETTING_PATH_FONT, f);
	fputs(delim, f);
	fprintf(f, "%s", cfg->path_font);
	fputc('\n', f);

	fputs(CFG_SETTING_GFX_FRAMERATE, f);
	fputs(delim, f);
	fprintf(f, "%f", cfg->gfx_framerate);
	fputc('\n', f);

	/* window pos, size */
	fputs(CFG_SETTING_GFX_WINDOW_X, f);
	fputs(delim, f);
	fprintf(f, "%i", cfg->gfx_window_x);
	fputc('\n', f);

	fputs(CFG_SETTING_GFX_WINDOW_Y, f);
	fputs(delim, f);
	fprintf(f, "%i", cfg->gfx_window_y);
	fputc('\n', f);

	fputs(CFG_SETTING_GFX_WINDOW_W, f);
	fputs(delim, f);
	fprintf(f, "%i", cfg->gfx_window_w);
	fputc('\n', f);

	fputs(CFG_SETTING_GFX_WINDOW_H, f);
	fputs(delim, f);
	fprintf(f, "%i", cfg->gfx_window_h);
	fputc('\n', f);

	/* bg color */
	fputs(CFG_SETTING_BG_RED, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->bg_red);
	fputc('\n', f);

	fputs(CFG_SETTING_BG_GREEN, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->bg_green);
	fputc('\n', f);

	fputs(CFG_SETTING_BG_BLUE, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->bg_blue);
	fputc('\n', f);

	/* font color */
	fputs(CFG_SETTING_FONT_RED, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->font_red);
	fputc('\n', f);

	fputs(CFG_SETTING_FONT_GREEN, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->font_green);
	fputc('\n', f);

	fputs(CFG_SETTING_FONT_BLUE, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->font_blue);
	fputc('\n', f);

	fputs(CFG_SETTING_FONT_ALPHA, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->font_alpha);
	fputc('\n', f);

	/* field border color */
	fputs(CFG_SETTING_FIELD_BORDER_RED, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->field_border_red);
	fputc('\n', f);

	fputs(CFG_SETTING_FIELD_BORDER_GREEN, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->field_border_green);
	fputc('\n', f);

	fputs(CFG_SETTING_FIELD_BORDER_BLUE, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->field_border_blue);
	fputc('\n', f);

	fputs(CFG_SETTING_FIELD_BORDER_ALPHA, f);
	fputs(delim, f);
	fprintf(f, "%u", cfg->field_border_alpha);
	fputc('\n', f);

	fclose(f);
	SM_String_clear(&filepath);
}
