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

/* filetypes */
static const char FILETYPE_TOWN[] = "twn";
static const char FILETYPE_BACKUP[] = "bkp";

#endif /* DEF_FILES_H */
