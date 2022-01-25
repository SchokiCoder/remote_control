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

#ifndef CONFIG_H
#define CONFIG_H

#include "constants.h"

struct Config
{
	char path_font[CFG_SETTING_PATH_FONT_MAX_LEN];
	float gfx_framerate;
	int32_t gfx_window_x;
	int32_t gfx_window_y;
	int32_t gfx_window_w;
	int32_t gfx_window_h;
	uint8_t bg_red;
	uint8_t bg_green;
	uint8_t bg_blue;
	uint8_t font_red;
	uint8_t font_green;
	uint8_t font_blue;
	uint8_t font_alpha;
	uint8_t field_border_red;
	uint8_t field_border_green;
	uint8_t field_border_blue;
	uint8_t field_border_alpha;
};

int32_t load_config(struct Config *p_cfg);
int32_t save_config(struct Config *p_cfg);

#endif /* CONFIG_H */
