/*
    remote_control
    Copyright (C) 2021  Andy Frank Schoknecht

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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define APP_NAME "remote_control"
#define APP_MAJOR 0
#define APP_MINOR 0
#define APP_PATCH 0

#ifdef _WIN32
    #define SLASH "\\"
#else
    #define SLASH "/"
#endif

#define DIR_BASE                    "." APP_NAME
#define DIR_TOWNS                   "towns"

#define PATH_ASSETS                 "assets" SLASH
#define PATH_TEXTURES               PATH_ASSETS "textures" SLASH
#define PATH_TEXTURE_HQ             PATH_TEXTURES "headquarter.png"
#define PATH_TEXTURE_TREE_0         PATH_TEXTURES "tree_0.png"

#define FILETYPE_TOWN "twn"

#define MSG_ERR     "ERROR: "
#define MSG_WARN    "WARNING: "
#define MSG_SDL_ERR "SDL-Err: %s\n"

#define MSG_ERR_ARG_MIN                 MSG_ERR "Too few arguments passed.\nStopped.\n"
#define MSG_WARN_ARG_MAX                MSG_WARN "Too many arguments passed.\nAdditional arguments will be ignored.\n"
#define MSG_ERR_FILE_SAVE               MSG_ERR "File could not be saved.\nMake sure you have permissions to write and read.\n"
#define MSG_ERR_FILE_LOAD               MSG_ERR "File could not be loaded.\nMake sure the file exists and permissions to read are given.\n"
#define MSG_ERR_DIR_BASE                MSG_ERR "Game file directory does not exist and could not be created.\n"
#define MSG_ERR_DIR_TOWNS_CREATE        MSG_ERR "Town file directory does not exist and could not be created.\n"
#define MSG_ERR_DIR_TOWNS               MSG_ERR "Town file directory does not exist.\n"
#define MSG_ERR_FILE_TOWN_CORRUPT       MSG_ERR "Town file has corrupt information.\n"
#define MSG_ERR_FILE_TOWN_SAVE          MSG_ERR "New town was not successfully created.\n"
#define MSG_FILE_TOWN_SAVE              "New town was successfully created and is awaiting your commands.\n" 
#define MSG_ERR_FILE_TOWN_LOAD          MSG_ERR "Town could not be loaded.\n"
#define MSG_FILE_TOWN_LOAD              "Town loaded.\n"
#define MSG_WARN_FILE_TOWN_EXIST        MSG_WARN "The town already exists, type 'y' if you are sure to proceed.\n"
#define MSG_ERR_ADMIN_ID                MSG_ERR "Given admin id does not exist.\nUse \"" CMD_LIST_ADMINS_LONG "\" command to make your decision.\n"
#define MSG_TOWN_CREATION_STOPPED       "Town creation stopped.\n"
#define MSG_ERR_SDL_INIT                MSG_ERR "SDL could not be initialized.\n" MSG_SDL_ERR
#define MSG_ERR_SDL_WINDOW              MSG_ERR "SDL could not open a window.\n" MSG_SDL_ERR
#define MSG_ERR_SDL_RENDERER            MSG_ERR "SDL could not create a renderer.\n" MSG_SDL_ERR
#define MSG_ERR_FILE_READ               MSG_ERR "File \"%s\" could not be read.\n"
#define MSG_ERR_IMAGE_LOAD              MSG_ERR "Texture \"%s\" could not be loaded.\n"
#define MSG_ERR_TEXTURE_CREATE          MSG_ERR "Texture \"%s\" can not be used.\n"

#define CMD_HELP                "h"     /* context dependent: behaving different in-game and outside of game */
#define CMD_HELP_LONG           "help"
#define CMD_LIST_ADMINS         "la"
#define CMD_LIST_ADMINS_LONG    "list-admins"
#define CMD_HIRE_ADMIN          "ha"    /* create new game */
#define CMD_HIRE_ADMIN_LONG     "hire-admin"
#define CMD_LIST_TOWNS          "lt"
#define CMD_LIST_TOWNS_LONG     "list-towns"
#define CMD_CONNECT             "c"    /* start playing game, inits the mainloop */
#define CMD_CONNECT_LONG        "connect"

#define TOWN_WIDTH                  15
#define TOWN_HEIGHT                 15
#define TOWN_EXPOSURE_AREA_BEGIN_X  4
#define TOWN_EXPOSURE_AREA_BEGIN_Y  4
#define TOWN_EXPOSURE_AREA_END_X    11
#define TOWN_EXPOSURE_AREA_END_Y    11
#define TOWN_TREEFREE_AREA_BEGIN_X  5
#define TOWN_TREEFREE_AREA_BEGIN_Y  5
#define TOWN_TREEFREE_AREA_END_X    10
#define TOWN_TREEFREE_AREA_END_Y    10
#define TOWN_GEN_TREE_THRESHOLD     40  /* from 0 to 100 */
#define TOWN_HQ_SPAWN_X             7
#define TOWN_HQ_SPAWN_Y             7

#define HELP_TEXT_OUTSIDE "Usage " APP_NAME " COMMAND [ARGS]\n" \
"\n" \
"Commands to prepare your work:\n" \
"  show this message:\n" \
"  " CMD_HELP ",\t" CMD_HELP_LONG "\n" \
"\n" \
"  list all available administrators and their attributes:\n" \
"  " CMD_LIST_ADMINS ",\t" CMD_LIST_ADMINS_LONG "\n" \
"\n" \
"  hire a new admin to assist you watching over another place:\n" \
"  " CMD_HIRE_ADMIN ",\t" CMD_HIRE_ADMIN_LONG "\tADMIN_ID TOWN_NAME\n" \
"\n" \
"  list towns that are already controlled by you:\n" \
"  " CMD_LIST_TOWNS ",\t" CMD_LIST_TOWNS_LONG "\n" \
"\n" \
"  connect to a towns administrator and get to work:\n" \
"  " CMD_CONNECT ",\t" CMD_CONNECT_LONG "\t\tTOWN_NAME\n"

#define WINDOW_WIDTH    600.0f
#define WINDOW_HEIGHT   600.0f

#define COLOR_BG_RED    20
#define COLOR_BG_GREEN  20
#define COLOR_BG_BLUE   80

#define COLOR_FIELD_HIDDEN_RED      0
#define COLOR_FIELD_HIDDEN_GREEN    0
#define COLOR_FIELD_HIDDEN_BLUE     0
#define COLOR_FIELD_HIDDEN_ALPHA    255

#define COLOR_FIELD_EXPOSED_RED     100
#define COLOR_FIELD_EXPOSED_GREEN   255
#define COLOR_FIELD_EXPOSED_BLUE    100
#define COLOR_FIELD_EXPOSED_ALPHA   255

#define SIZE_AREA_X 0.9f
#define SIZE_AREA_Y 0.9f

#endif
