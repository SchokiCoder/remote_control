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

//generic path definitions
#ifdef _WIN32
	#define SLASH "\\"
#else
	#define SLASH "/"
#endif /* _WIN32 */

#define FILEPATH_MAX_LEN 1024

//userfile paths
#define PATH_BASE		"." APP_NAME	//appended onto the home path
#define PATH_TOWNS		"towns"			//from here on: appended onto the base path
#define PATH_CONFIG		"config.cfg"

//asset paths (and some data)
#define PATH_TEXTURE_ICON			PATH_TEXTURES APP_NAME ".png"
#define PATH_TEXTURE_GROUND			PATH_TEXTURES "ground.png"
#define PATH_TEXTURE_HIDDEN			PATH_TEXTURES "hidden.png"
#define PATH_TEXTURE_HQ				PATH_TEXTURES "headquarter.png"
#define PATH_TEXTURE_CONSTRUCTION	PATH_TEXTURES "construction.png"
#define PATH_TEXTURE_QUARRY			PATH_TEXTURES "quarry.png"

#define TOWN_TREE_VARIETY_COUNT		5
#define PATH_TEXTURE_TREE_0			PATH_TEXTURES "tree_0.png"
#define PATH_TEXTURE_TREE_1			PATH_TEXTURES "tree_1.png"
#define PATH_TEXTURE_TREE_2			PATH_TEXTURES "tree_2.png"
#define PATH_TEXTURE_TREE_3			PATH_TEXTURES "tree_3.png"
#define PATH_TEXTURE_TREE_4			PATH_TEXTURES "tree_4.png"

//filetypes
#define FILETYPE_TOWN	"twn"
#define FILETYPE_BACKUP "bkp"

#endif /* DEF_FILES_H */
