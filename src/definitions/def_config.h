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

#ifndef DEF_CONFIG_H
#define DEF_CONFIG_H

#include <SDL.h>

/* config definitions */
#define CONFIG_MAX_LINES		256
#define CONFIG_MAX_SETTING_LEN	64
#define CONFIG_MAX_VALUE_LEN	64

/* config setting defintions */
#define CFG_SETTING_PATH_FONT_MAX_LEN	256

/* config setting names */
static const char CFG_SETTING_PATH_FONT[] = 			"path_font";
static const char CFG_SETTING_GFX_FRAMERATE[] =			"gfx_framerate";
static const char CFG_SETTING_GFX_WINDOW_X[] =			"gfx_window_x";
static const char CFG_SETTING_GFX_WINDOW_Y[] =			"gfx_window_y";
static const char CFG_SETTING_GFX_WINDOW_W[] =			"gfx_window_w";
static const char CFG_SETTING_GFX_WINDOW_H[] =			"gfx_window_h";
static const char CFG_SETTING_BG_RED[] =				"bg_red";
static const char CFG_SETTING_BG_GREEN[] =				"bg_green";
static const char CFG_SETTING_BG_BLUE[] =				"bg_blue";
static const char CFG_SETTING_FONT_RED[] =				"font_red";
static const char CFG_SETTING_FONT_GREEN[] =			"font_green";
static const char CFG_SETTING_FONT_BLUE[] =				"font_blue";
static const char CFG_SETTING_FONT_ALPHA[] =			"font_alpha";
static const char CFG_SETTING_FIELD_BORDER_RED[] =		"field_border_red";
static const char CFG_SETTING_FIELD_BORDER_GREEN[] =	"field_border_green";
static const char CFG_SETTING_FIELD_BORDER_BLUE[] =		"field_border_blue";
static const char CFG_SETTING_FIELD_BORDER_ALPHA[] =	"field_border_alpha";

/* default config values */
#ifdef _WIN32
	static const char CFG_STD_PATH_FONT[] =	"C:\\Windows\\Fonts\\Arial.ttf";
#else
	static const char CFG_STD_PATH_FONT[] =	"/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
#endif /* _WIN32 */

static const float CFG_STD_GFX_FRAMERATE =			10.0f;
static const uint32_t CFG_STD_GFX_WINDOW_X =		SDL_WINDOWPOS_CENTERED;
static const uint32_t CFG_STD_GFX_WINDOW_Y =		SDL_WINDOWPOS_CENTERED;
static const float CFG_STD_GFX_WINDOW_W =			600.0f;
static const float CFG_STD_GFX_WINDOW_H =			600.0f;
static const uint8_t CFG_STD_BG_RED =				29;
static const uint8_t CFG_STD_BG_GREEN =				25;
static const uint8_t CFG_STD_BG_BLUE =				6;
static const uint8_t CFG_STD_FONT_RED =				255;
static const uint8_t CFG_STD_FONT_GREEN =			255;
static const uint8_t CFG_STD_FONT_BLUE =			255;
static const uint8_t CFG_STD_FONT_ALPHA =			255;
static const uint8_t CFG_STD_FIELD_BORDER_RED =		100;
static const uint8_t CFG_STD_FIELD_BORDER_GREEN =	100;
static const uint8_t CFG_STD_FIELD_BORDER_BLUE	=	255;
static const uint8_t CFG_STD_FIELD_BORDER_ALPHA =	50;

#endif /* DEF_CONFIG_H */
