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

#include <SDL_video.h>

//application
#define APP_NAME "remote_control"
#define APP_LICENSE "GPLv3"
#define APP_LICENSE_NOTICE "You should have received a copy of the GNU General Public License\n" \
"along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
#define APP_SOURCE "https://github.com/SchokiCoder/remote_control"
#define APP_MAJOR "0"
#define APP_MINOR "1"
#define APP_PATCH "0"

#ifdef _WIN32
    #define SLASH "\\"
#else
    #define SLASH "/"
#endif

#define FILEPATH_MAX_LEN 1024

//userfile paths
#define PATH_BASE       "." APP_NAME    //appended onto the home path
#define PATH_TOWNS      "towns"         //from here on: appended onto the base path
#define PATH_CONFIG     "config.cfg"

//config definitions
#define CONFIG_MAX_LINES        255
#define CONFIG_MAX_SETTING_LEN  64
#define CONFIG_MAX_VALUE_LEN    64

//config setting defintions
#define CFG_SETTING_PATH_FONT_MAX_LEN   255

//config setting names
#define CFG_SETTING_PATH_FONT           "path_font"
#define CFG_SETTING_GFX_FRAMERATE       "gfx_framerate"
#define CFG_SETTING_GFX_WINDOW_X        "gfx_window_x"
#define CFG_SETTING_GFX_WINDOW_Y        "gfx_window_y"
#define CFG_SETTING_GFX_WINDOW_W        "gfx_window_w"
#define CFG_SETTING_GFX_WINDOW_H        "gfx_window_h"
#define CFG_SETTING_BG_RED              "bg_red"
#define CFG_SETTING_BG_GREEN            "bg_green"
#define CFG_SETTING_BG_BLUE             "bg_blue"
#define CFG_SETTING_FONT_RED            "font_red"
#define CFG_SETTING_FONT_GREEN          "font_green"
#define CFG_SETTING_FONT_BLUE           "font_blue"
#define CFG_SETTING_FONT_ALPHA          "font_alpha"
#define CFG_SETTING_FIELD_BORDER_RED    "field_border_red"
#define CFG_SETTING_FIELD_BORDER_GREEN  "field_border_green"
#define CFG_SETTING_FIELD_BORDER_BLUE   "field_border_blue"
#define CFG_SETTING_FIELD_BORDER_ALPHA  "field_border_alpha"

//default config values
#ifdef _WIN32
    #define CFG_STD_PATH_FONT       "C:\\Windows\\Fonts\\Arial.ttf"    
#else
    #define CFG_STD_PATH_FONT       "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
#endif

#define CFG_STD_GFX_FRAMERATE       2.5f
#define CFG_STD_GFX_WINDOW_X        SDL_WINDOWPOS_CENTERED
#define CFG_STD_GFX_WINDOW_Y        SDL_WINDOWPOS_CENTERED
#define CFG_STD_GFX_WINDOW_W        600.0f
#define CFG_STD_GFX_WINDOW_H        600.0f
#define CFG_STD_BG_RED              20
#define CFG_STD_BG_GREEN            20
#define CFG_STD_BG_BLUE             80
#define CFG_STD_FONT_RED            255
#define CFG_STD_FONT_GREEN          255
#define CFG_STD_FONT_BLUE           255
#define CFG_STD_FONT_ALPHA          255
#define CFG_STD_FIELD_BORDER_RED    100
#define CFG_STD_FIELD_BORDER_GREEN  100
#define CFG_STD_FIELD_BORDER_BLUE   255
#define CFG_STD_FIELD_BORDER_ALPHA  50

//asset paths
#define PATH_TEXTURE_ICON           PATH_TEXTURES APP_NAME ".png"
#define PATH_TEXTURE_GROUND         PATH_TEXTURES "ground.png"
#define PATH_TEXTURE_HIDDEN         PATH_TEXTURES "hidden.png"
#define PATH_TEXTURE_HQ             PATH_TEXTURES "headquarter.png"
#define PATH_TEXTURE_TREE_0         PATH_TEXTURES "tree_0.png"
#define PATH_TEXTURE_TREE_1         PATH_TEXTURES "tree_1.png"
#define PATH_TEXTURE_TREE_2         PATH_TEXTURES "tree_2.png"
#define PATH_TEXTURE_TREE_3         PATH_TEXTURES "tree_3.png"
#define PATH_TEXTURE_TREE_4         PATH_TEXTURES "tree_4.png"
#define FILETYPE_TOWN   "twn"
#define FILETYPE_BACKUP "bkp"

//messages
#define MSG_ERR     "ERROR: "
#define MSG_WARN    "WARNING: "
#define MSG_SDL_ERR "SDL-Err: %s\n"

#define MSG_ERR_UNKNOWN_COMMAND         MSG_ERR "Command not recognised.\nUse " CMD_HELP_LONG " command for information on usage.\n"
#define MSG_ERR_ARG_MIN                 MSG_ERR "Too few arguments passed.\nStopped.\n"
#define MSG_WARN_ARG_MAX                MSG_WARN "Too many arguments passed.\nAdditional arguments will be ignored.\n"
#define MSG_ERR_FILE_SAVE               MSG_ERR "File could not be saved.\nMake sure you have permissions to write and read.\n"
#define MSG_ERR_FILE_LOAD               MSG_ERR "File could not be loaded.\nMake sure the file exists and permissions to read are given.\n"
#define MSG_ERR_DIR_BASE                MSG_ERR "Game file directory does not exist and could not be created.\n"
#define MSG_ERR_DIR_TOWNS_CREATE        MSG_ERR "Town file directory does not exist and could not be created.\n"
#define MSG_WARN_CONFIG_SAVE            MSG_WARN "Config file could not be saved.\n"
#define MSG_WARN_CONFIG_LOAD            MSG_WARN "Config file could not be loaded.\n"
#define MSG_WARN_CONFIG_UNKNOWN_SETTING MSG_WARN "Unrecognised setting in config \"%s\".\n"
#define MSG_ERR_UNKNOWN_SETTING         MSG_ERR "Unrecognised setting \"%s\".\n"
#define MSG_CONFIG_CREATED              "The config was missing and a default config has been created.\n"
#define MSG_ERR_DIR_TOWNS               MSG_ERR "Town file directory does not exist.\n"
#define MSG_ERR_FILE_TOWN_CORRUPT       MSG_ERR "Town file has corrupt information.\n"
#define MSG_ERR_FILE_TOWN_SAVE          MSG_ERR "Town was not successfully saved.\n"
#define MSG_FILE_TOWN_SAVE              "Town was successfully saved.\n"
#define MSG_FILE_TOWN_SAVE_AS           "Town was successfully saved as \"%s\".\n"
#define MSG_FILE_TOWN_CREATE            "New town was successfully created and is awaiting your commands.\n" 
#define MSG_FILE_TOWN_DELETE            "Depending on how bad this case was, you should also get rid of the physical drive.\n"
#define MSG_ERR_FILE_TOWN_DELETE        MSG_ERR "Town files couldn't be deleted.\n"
#define MSG_ERR_FILE_TOWN_LOAD          MSG_ERR "Town could not be loaded.\n"
#define MSG_FILE_TOWN_LOAD              "Town loaded.\n"
#define MSG_WARN_FILE_TOWN_EXIST        MSG_WARN "The town already exists, type 'y' if you are sure to proceed.\n"
#define MSG_WARN_FILE_TOWN_BACKUP       MSG_WARN "Creating backup files of this town failed.\n"
#define MSG_ERR_ADMIN_ID                MSG_ERR "Given admin id does not exist.\nUse \"" CMD_LIST_ADMINS_LONG "\" command to make your decision.\n"
#define MSG_TOWN_CREATION_STOPPED       "Town creation stopped.\n"
#define MSG_ERR_SDL_INIT                MSG_ERR "SDL could not be initialized.\n" MSG_SDL_ERR
#define MSG_ERR_SDL_WINDOW              MSG_ERR "SDL could not open a window.\n" MSG_SDL_ERR
#define MSG_ERR_SDL_RENDERER            MSG_ERR "SDL could not create a renderer.\n" MSG_SDL_ERR
#define MSG_ERR_TTF_INIT                MSG_ERR "SDL_ttf could not be initialized.\n"
#define MSG_ERR_FILE_READ               MSG_ERR "File \"%s\" could not be read.\n"
#define MSG_ERR_IMAGE_LOAD              MSG_ERR "Texture \"%s\" could not be loaded.\n"
#define MSG_ERR_TEXT_CREATE             MSG_ERR "Texture could no be created fromn text \"%s\".\n"
#define MSG_ERR_FONT_LOAD               MSG_ERR "Font \"%s\" could not be loaded.\nYou should change your config.\n"
#define MSG_ERR_TEXTURE_CREATE          MSG_ERR "Texture \"%s\" can not be used.\n"

//commands
#define CMD_HELP                "h"
#define CMD_HELP_LONG           "help"
#define CMD_LIST_ADMINS         "la"
#define CMD_LIST_ADMINS_LONG    "list-admins"
#define CMD_HIRE_ADMIN          "ha"    /* create new game */
#define CMD_HIRE_ADMIN_LONG     "hire-admin"
#define CMD_LIST_TOWNS          "lt"
#define CMD_LIST_TOWNS_LONG     "list-towns"
#define CMD_CONNECT             "c"    /* start playing game, inits the mainloop */
#define CMD_CONNECT_LONG        "connect"
#define CMD_DELETE_TOWN_LONG    "delete"

//in-game comands
#define GM_CMD_HELP             "help"
#define GM_CMD_SAVE             "save"
#define GM_CMD_SAVE_AS          "save-as"
#define GM_CMD_EXIT             "exit"
#define GM_CMD_SHOW_CONFIG      "show-config"
#define GM_CMD_SET              "set"

//town generation
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

//help text
#define HELP_TEXT_OUTSIDE "\n" \
"If you are new to this post, you should follow this 1st steps guide to quickly get started.\n" \
"1. Use the " CMD_LIST_ADMINS_LONG " or in short " CMD_LIST_ADMINS " command, to take a glance at our loyal administrators.\n" \
"   Remember their strengths well and you should be good to go.\n" \
"2. Use the " CMD_HIRE_ADMIN_LONG " or " CMD_HIRE_ADMIN " command, to start a new town.\n" \
"3. Use the " CMD_CONNECT_LONG " / " CMD_CONNECT " command to begin your work in this town.\n" \
"\n" \
"PS. It is advisable to read through the commands description, before using it.\n" \
"\n" \
"\n" \
"Usage " APP_NAME " COMMAND [ARGS]\n" \
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
"  " CMD_CONNECT ",\t" CMD_CONNECT_LONG "\t\tTOWN_NAME\n" \
"\n" \
"  delete all files of a given town:\n" \
"  " CMD_DELETE_TOWN_LONG "\t\tTOWN_NAME\n" \
"\n" \
APP_NAME " " APP_MAJOR "." APP_MINOR "." APP_PATCH " is licensed under the " APP_LICENSE ".\n" \
APP_LICENSE_NOTICE \
"The source code of this program is available at\n" \
APP_SOURCE "\n"

#define HELP_TEXT_INGAME "List of commands:\n" \
"\n" \
"  show this message:\n" \
"  " GM_CMD_HELP "\n" \
"\n" \
"  save current game:\n" \
"  " GM_CMD_SAVE "\n" \
"\n" \
"  save game with different name:\n" \
"  " GM_CMD_SAVE_AS "\tTOWN_NAME\n" \
"\n" \
"  exit the game:\n" \
"  " GM_CMD_EXIT "\n" \
"\n" \
"  list all configuration values:\n" \
"  " GM_CMD_SHOW_CONFIG "\n" \
"\n" \
"  set configuration values:\n" \
"  " GM_CMD_SET "\tSETTING_NAME\tSETTING_VALUE\n" \

//terminal in-game
#define TRM_MAX_ARGS    8
#define TRM_MAX_ARG_LEN 32
#define TRM_MAX_IN_LEN  (TRM_MAX_ARGS * TRM_MAX_ARG_LEN)

//colors
#define COLOR_FIELD_HIDDEN_RED      0
#define COLOR_FIELD_HIDDEN_GREEN    0
#define COLOR_FIELD_HIDDEN_BLUE     0
#define COLOR_FIELD_HIDDEN_ALPHA    255

//gfx in-game sizes of displayed content in percent
#define UI_AREA_X2              0.9f
#define UI_AREA_Y               0.1f
#define UI_AREA_H               0.8f

#define UI_FIELD_CONTENT_SIZE   0.95f

#define UI_FONT_SIZE            12.0f

#define UI_TEXT_HOVER_LABEL     "field:"
#define UI_TEXT_HOVER_LABEL_X   0.005f
#define UI_TEXT_HOVER_LABEL_Y   0.005f
#define UI_TEXT_HOVER_LABEL_W   0.175f
#define UI_TEXT_HOVER_LABEL_H   0.05f
#define UI_TEXT_HOVER_X_X       (UI_TEXT_HOVER_LABEL_X + UI_TEXT_HOVER_LABEL_W + 0.025f)
#define UI_TEXT_HOVER_X_Y       UI_TEXT_HOVER_LABEL_Y
#define UI_TEXT_HOVER_X_W       0.065f
#define UI_TEXT_HOVER_X_H       0.05f
#define UI_TEXT_HOVER_Y_X       (UI_TEXT_HOVER_X_X + UI_TEXT_HOVER_X_W + 0.025f)
#define UI_TEXT_HOVER_Y_Y       UI_TEXT_HOVER_X_Y
#define UI_TEXT_HOVER_Y_W       UI_TEXT_HOVER_X_W
#define UI_TEXT_HOVER_Y_H       UI_TEXT_HOVER_X_H

#endif
