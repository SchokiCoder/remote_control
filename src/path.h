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

#ifndef PATH_H
#define PATH_H

#include <stdint.h>
#include <SM_string.h>

#ifdef _WIN32
	static const char SLASH[] = "\\";
#else
	static const char SLASH[] = "/";
#endif /* _WIN32 */

static const char PATH_TOWNS[] = "towns";
static const char PATH_CONFIG[] = "config.cfg";
static const char PATH_TEXTURE_ICON[] =	PATH_TEXTURES "icon.png";

static const char FILETYPE_TOWN[] = "twn";
static const char FILETYPE_BACKUP[] = "bkp";

int32_t get_base_path( SM_String *out );

int32_t get_town_path( SM_String *out );

int32_t get_config_path( SM_String *out );

#endif /* PATH_H */
