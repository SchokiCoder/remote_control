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

#ifndef DEF_MESSAGES_H
#define DEF_MESSAGES_H

#include "def_commands.h"

//help text
#define HELP_TEXT_OUTSIDE "\n" \
"If you are new to this post, you should follow this 1st steps guide to quickly get started.\n" \
"1. Use the " CMD_LIST_ADMINS_LONG " or in short " CMD_LIST_ADMINS " command, to take a glance at our loyal administrators.\n" \
"	Remember their strengths well and you should be good to go.\n" \
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
"\n" \
"  end round:\n" \
"  " GM_CMD_PASS "\n"

//short messages
#define MSG_ERR		"ERROR: "
#define MSG_WARN	"WARNING: "
#define MSG_SDL_ERR "SDL-Err: %s\n"

#define MSG_ERR_UNKNOWN_COMMAND			MSG_ERR "Command not recognised.\nUse " CMD_HELP_LONG " command for information on usage.\n"
#define MSG_ERR_ARG_MIN					MSG_ERR "Too few arguments passed.\nStopped.\n"
#define MSG_WARN_ARG_MAX				MSG_WARN "Too many arguments passed.\nAdditional arguments will be ignored.\n"
#define MSG_ERR_FILE_SAVE				MSG_ERR "File could not be saved.\nMake sure you have permissions to write and read.\n"
#define MSG_ERR_FILE_LOAD				MSG_ERR "File could not be loaded.\nMake sure the file exists and permissions to read are given.\n"
#define MSG_ERR_DIR_BASE				MSG_ERR "Game file directory does not exist and could not be created.\n"
#define MSG_ERR_DIR_TOWNS_CREATE		MSG_ERR "Town file directory does not exist and could not be created.\n"
#define MSG_WARN_CONFIG_SAVE			MSG_WARN "Config file could not be saved.\n"
#define MSG_WARN_CONFIG_LOAD			MSG_WARN "Config file could not be loaded.\n"
#define MSG_WARN_CONFIG_UNKNOWN_SETTING MSG_WARN "Unrecognised setting in config \"%s\".\n"
#define MSG_ERR_UNKNOWN_SETTING			MSG_ERR "Unrecognised setting \"%s\".\n"
#define MSG_CONFIG_CREATED				"The config was missing and a default config has been created.\n"
#define MSG_ERR_DIR_TOWNS				MSG_ERR "Town file directory does not exist.\n"
#define MSG_ERR_FILE_TOWN_CORRUPT		MSG_ERR "Town file has corrupt information.\n"
#define MSG_ERR_FILE_TOWN_SAVE			MSG_ERR "Town was not successfully saved.\n"
#define MSG_FILE_TOWN_SAVE				"Town was successfully saved.\n"
#define MSG_FILE_TOWN_SAVE_AS			"Town was successfully saved as \"%s\".\n"
#define MSG_FILE_TOWN_CREATE			"New town was successfully created and is awaiting your commands.\n"
#define MSG_FILE_TOWN_DELETE			"Depending on how bad this case was, you should also get rid of the physical drive.\n"
#define MSG_ERR_FILE_TOWN_DELETE		MSG_ERR "Town files couldn't be deleted.\n"
#define MSG_ERR_FILE_TOWN_LOAD			MSG_ERR "Town could not be loaded.\n"
#define MSG_FILE_TOWN_LOAD				"Town loaded.\n"
#define MSG_WARN_FILE_TOWN_EXIST		MSG_WARN "The town already exists, type 'y' if you are sure to proceed.\n"
#define MSG_WARN_FILE_TOWN_BACKUP		MSG_WARN "Creating backup files of this town failed.\n"
#define MSG_ERR_ADMIN_ID				MSG_ERR "Given admin id does not exist.\nUse \"" CMD_LIST_ADMINS_LONG "\" command to make your decision.\n"
#define MSG_TOWN_CREATION_STOPPED		"Town creation stopped.\n"
#define MSG_ERR_SDL_INIT				MSG_ERR "SDL could not be initialized.\n" MSG_SDL_ERR
#define MSG_ERR_SDL_WINDOW				MSG_ERR "SDL could not open a window.\n" MSG_SDL_ERR
#define MSG_ERR_SDL_RENDERER			MSG_ERR "SDL could not create a renderer.\n" MSG_SDL_ERR
#define MSG_ERR_TTF_INIT				MSG_ERR "SDL_ttf could not be initialized.\n"
#define MSG_ERR_FILE_READ				MSG_ERR "File \"%s\" could not be read.\n"
#define MSG_ERR_IMAGE_LOAD				MSG_ERR "Texture \"%s\" could not be loaded.\n"
#define MSG_ERR_TEXT_CREATE				MSG_ERR "Texture could no be created from text \"%s\".\n"
#define MSG_ERR_FONT_LOAD				MSG_ERR "Font \"%s\" could not be loaded.\nYou should change your config.\n"
#define MSG_ERR_TEXTURE_CREATE			MSG_ERR "Texture \"%s\" can not be used.\n"

#define MSG_CONNECTION_CLOSED	"-- Connection terminated --\n"
#define MSG_PLAYER_FAILURE		"You have failed this town, overseer.\n"
#define MSG_FAILURE_COST		"Your headquarter reported critical financial instabilities.\n"

#endif /* DEF_MESSAGES_H */
