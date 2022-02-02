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

#ifndef DEF_FILES_H
#define DEF_FILES_H

#include "def_app.h"

/* generic path definitions */
#ifdef _WIN32
	static const char SLASH[] = "\\";
#else
	static const char SLASH[] = "/";
#endif /* _WIN32 */

#define FILEPATH_MAX_LEN 1024

static const char PATH_BASE[] = "%s%s.%s";

/* userfile paths (appended onto the base path) */
static const char PATH_TOWNS[] = "towns";
static const char PATH_CONFIG[] = "config.cfg";

/* asset paths (and some data) */
static const char PATH_TEXTURE_ICON[] =	PATH_TEXTURES "icon.png";

static const char PATH_TEXTURE_HUD_CONSTRUCT[] =	PATH_TEXTURES "hud_construct.png";
static const char PATH_TEXTURE_HUD_DECONSTRUCT[] =	PATH_TEXTURES "hud_deconstruct.png";

static const char PATH_TEXTURE_GROUND[] =		PATH_TEXTURES "ground.png";
static const char PATH_TEXTURE_HIDDEN[] =		PATH_TEXTURES "hidden.png";
static const char PATH_TEXTURE_HQ[] =			PATH_TEXTURES "headquarter.png";
static const char PATH_TEXTURE_CONSTRUCTION[] =	PATH_TEXTURES "construction.png";
static const char PATH_TEXTURE_QUARRY[] =		PATH_TEXTURES "quarry.png";

#define TOWN_TREE_VARIETY_COUNT					5
static const char PATH_TEXTURE_TREE_0[] =		PATH_TEXTURES "tree_0.png";
static const char PATH_TEXTURE_TREE_1[] =		PATH_TEXTURES "tree_1.png";
static const char PATH_TEXTURE_TREE_2[] =		PATH_TEXTURES "tree_2.png";
static const char PATH_TEXTURE_TREE_3[] =		PATH_TEXTURES "tree_3.png";
static const char PATH_TEXTURE_TREE_4[] =		PATH_TEXTURES "tree_4.png";

/* filetypes */
static const char FILETYPE_TOWN[] = "twn";
static const char FILETYPE_BACKUP[] = "bkp";

#endif /* DEF_FILES_H */
