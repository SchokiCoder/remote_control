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

#ifndef MESSAGES_H
#define MESSAGES_H

#include "commands.h"

/* help text */
static const char HELP_1ST_STEPS[] =
	"\n" \
	"If you are new to this post, you should follow this 1st steps guide to quickly get started.\n" \
	"1. In the main menu list all administrators, " \
	"to take a glance at the characteristics of our loyal administrators.\n" \
	"   Remember them well and you should be good to go.\n" \
	"2. Use the hire admin option, to start a new town.\n" \
	"3. Use the connect option to begin your work in your town.\n" \
	"\n";

static const char HELP_APP[] =
	"Usage:\n  %s [COMMAND] [ARGS]\n";

static const char HELP_LICENSE[] =
	"%s %u.%u.%u is licensed under the %s.\n" \
	"%s" \
	"The source code of this program is available at\n%s\n";

/* messages */
#define MSG_ERR 	"ERROR: "
#define MSG_WARN	"WARNING: "
#define MSG_SDL_ERR	"SDL-Err: %s\n"

/* outside of the game */
static const char MSG_ERR_UNKNOWN_COMMAND[] =
	MSG_ERR "Command not recognised.\nUse \"%s\" command for information on usage.\n";

static const char MSG_ERR_ARG_MIN[] =
	MSG_ERR "Too few arguments passed.\nStopped.\n";

static const char MSG_WARN_ARG_MAX[] =
	MSG_WARN "Too many arguments passed.\nAdditional arguments will be ignored.\n";

static const char MSG_ERR_FILE_SAVE[] =
	MSG_ERR "File could not be saved.\nMake sure you have permissions to write and read.\n";

static const char MSG_ERR_FILE_LOAD[] =
	MSG_ERR "File could not be loaded.\nMake sure the file exists and permissions to read are given.\n";

static const char MSG_ERR_DIR_BASE[] =
	MSG_ERR "Game file directory does not exist and could not be created.\n";

static const char MSG_ERR_DIR_TOWNS_CREATE[] =
	MSG_ERR "Town file directory does not exist and could not be created.\n";

static const char MSG_WARN_CONFIG_SAVE[] =
	MSG_WARN "Config file could not be saved.\n";

static const char MSG_WARN_CONFIG_LOAD[] =
	MSG_WARN "Config file could not be loaded.\n";

static const char MSG_WARN_UNKNOWN_SETTING[] =
	MSG_WARN "Unrecognised setting \"%s\".\n";

static const char MSG_CONFIG_CREATED[] =
	"The config was missing and a default config has been created.\n";

static const char MSG_ERR_DIR_TOWNS[] =
	MSG_ERR "Town file directory does not exist.\n";

static const char MSG_ERR_FILE_TOWN_CORRUPT[] =
	MSG_ERR "Town file has corrupt information.\n";

static const char MSG_ERR_FILE_TOWN_SAVE[] =
	MSG_ERR "Town was not successfully saved.\n";

static const char MSG_FILE_TOWN_SAVE[] =
	"Town was successfully saved.\n";

static const char MSG_FILE_TOWN_SAVE_AS[] =
	"Town was successfully saved as \"%s\".\n";

static const char MSG_FILE_TOWN_CREATE[] =
	"New town was successfully created and is awaiting your commands.\n";

static const char MSG_FILE_TOWN_DELETE[] =
	"Depending on how bad this case was, you should also get rid of the physical drive.\n";

static const char MSG_ERR_FILE_TOWN_DELETE[] =
	MSG_ERR "Town files couldn't be deleted.\n";

static const char MSG_ERR_FILE_TOWN_LOAD[] =
	MSG_ERR "Town could not be loaded.\n";

static const char MSG_FILE_TOWN_LOAD[] =
	"Town loaded.\n";

static const char MSG_WARN_FILE_TOWN_EXIST[] =
	MSG_WARN "The town already exists, type 'y' if you are sure to proceed.\n";

static const char MSG_WARN_FILE_TOWN_BACKUP[] =
	MSG_WARN "Creating backup files of this town failed.\n";

static const char MSG_ERR_ADMIN_ID[] =
	MSG_ERR "Given admin id does not exist.\nUse \"%s\" command to make your decision.\n";

static const char MSG_TOWN_CREATION_STOPPED[] =
	"Town creation stopped.\n";

static const char MSG_ERR_SDL_INIT[] =
	MSG_SDL_ERR "SDL could not be initialized.\n%s";

static const char MSG_ERR_SDL_WINDOW[] =
	MSG_SDL_ERR "SDL could not open a window.\n%s";

static const char MSG_ERR_SDL_RENDERER[] =
	MSG_SDL_ERR "SDL could not create a renderer.\n%s";

static const char MSG_ERR_TTF_INIT[] =
	MSG_SDL_ERR "SDL_ttf could not be initialized.\n";

static const char MSG_ERR_FILE_READ[] =
	MSG_ERR "File \"%s\" could not be read.\n";

static const char MSG_ERR_IMAGE_LOAD[] =
	MSG_ERR "Texture \"%s\" could not be loaded.\n";

static const char MSG_ERR_TEXT_CREATE[] =
	MSG_ERR "Texture could no be created from text \"%s\".\n";

static const char MSG_ERR_FONT_LOAD[] =
	MSG_ERR "Font \"%s\" could not be loaded.\nYou should change your config.\n";

static const char MSG_ERR_TEXTURE_CREATE[] =
	MSG_ERR "Texture \"%s\" can not be used.\n";

static const char MSG_WARN_WIN_ICON[] =
	MSG_WARN "Window icon could not be opened.\n";

static const char MSG_CONNECTION_CLOSED[] =
	"-- Connection terminated --\n";

static const char MSG_PLAYER_FAILURE[] =
	"You have failed this town, overseer.\n";

static const char MSG_FAILURE_COST[] =
	"Your headquarter reported critical financial instabilities.\n";

/* ingame messages */
static const char GM_MSG_ERR_MIN_ARG[] =
	MSG_ERR "Not enough arguments provided.";

static const char GM_MSG_ERR_UNKNOWN_CMD[] =
	MSG_ERR "Command not recognised.";

static const char GM_MSG_CFG_SET[] =
	"Config value set.";

static const char GM_MSG_ERR_CONFIG_UNKNOWN_SETTING[] =
	MSG_ERR "Unrecognised setting in config.";

static const char GM_MSG_ERR_TOWN_SAVE[] =
	"Town could not be saved.";

static const char GM_MSG_TOWN_SAVE[] =
	"Town saved";

static const char GM_MSG_PASS[] =
	"Round ended.";

static const char GM_MSG_CONSTRUCT[] =
	"Construction order accepted.";

static const char GM_MSG_CONSTRUCT_INVALID[] =
	"Construction order invalid. (Construction unknown)";

static const char GM_MSG_CONSTRUCT_COORD_INVALID[] =
	"Construction order invalid. (Coordinates invalid)";

static const char GM_MSG_DESTRUCT[] =
	"Destruction order accepted.";

#endif /* MESSAGES_H */
