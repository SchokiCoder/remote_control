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
static const char HELP_1ST_STEPS[] = "\n" \
"If you are new to this post, you should follow this 1st steps guide to quickly get started.\n" \
"1. In the main menu list all administrators, " \
"to take a glance at the characteristics of our loyal administrators.\n" \
"   Remember them well and you should be good to go.\n" \
"2. Use the hire admin option, to start a new town.\n" \
"3. Use the connect option to begin your work in your town.\n" \
"\n";

static const char HELP_COMMANDS[] = "\n" \
"Usage %s [COMMAND] [ARGS]\n" \
"\n" \
"Commands to prepare your work:\n" \
"  show this message:\n" \
"  %s,\t%s\n" \
"\n" \
"  list all available administrators and their attributes:\n" \
"  %s,\t%s\n" \
"\n" \
"  hire a new admin to assist you watching over another place:\n" \
"  %s,\t%s\tADMIN_ID TOWN_NAME\n" \
"\n" \
"  list towns that are already controlled by you:\n" \
"  %s,\t%s\n" \
"\n" \
"  connect to a towns administrator and get to work:\n" \
"  %s,\t%s\tTOWN_NAME\n" \
"\n" \
"  delete all files of a given town:\n" \
"  %s\t\tTOWN_NAME\n" \
"\n";

static const char HELP_LICENSE[] = "%s %u.%u.%u is licensed under the %s.\n" \
"%s" \
"The source code of this program is available at\n%s\n";

/* messages */
static const char MSG_ERR[] =		"ERROR: ";
static const char MSG_WARN[] =		"WARNING: ";
static const char MSG_SDL_ERR[] =	"SDL-Err: %s\n";

/* outside of the game */
static const char MSG_ERR_UNKNOWN_COMMAND[] =
	"%sCommand not recognised.\nUse \"%s\" command for information on usage.\n";

static const char MSG_ERR_ARG_MIN[] =
	"%sToo few arguments passed.\nStopped.\n";

static const char MSG_WARN_ARG_MAX[] =
	"%sToo many arguments passed.\nAdditional arguments will be ignored.\n";

static const char MSG_ERR_FILE_SAVE[] =
	"%sFile could not be saved.\nMake sure you have permissions to write and read.\n";

static const char MSG_ERR_FILE_LOAD[] =
	"%sFile could not be loaded.\nMake sure the file exists and permissions to read are given.\n";

static const char MSG_ERR_DIR_BASE[] =
	"%sGame file directory does not exist and could not be created.\n";

static const char MSG_ERR_DIR_TOWNS_CREATE[] =
	"%sTown file directory does not exist and could not be created.\n";

static const char MSG_WARN_CONFIG_SAVE[] =
	"%sConfig file could not be saved.\n";

static const char MSG_WARN_CONFIG_LOAD[] =
	"%sConfig file could not be loaded.\n";

static const char MSG_WARN_CONFIG_UNKNOWN_SETTING[] =
	"%sUnrecognised setting in config \"%s\".\n";

static const char MSG_ERR_UNKNOWN_SETTING[] =
	"%sUnrecognised setting \"%s\".\n";

static const char MSG_CONFIG_CREATED[] =
	"The config was missing and a default config has been created.\n";

static const char MSG_ERR_DIR_TOWNS[] =
	"%sTown file directory does not exist.\n";

static const char MSG_ERR_FILE_TOWN_CORRUPT[] =
	"%sTown file has corrupt information.\n";

static const char MSG_ERR_FILE_TOWN_SAVE[] =
	"%sTown was not successfully saved.\n";

static const char MSG_FILE_TOWN_SAVE[] =
	"Town was successfully saved.\n";

static const char MSG_FILE_TOWN_SAVE_AS[] =
	"Town was successfully saved as \"%s\".\n";

static const char MSG_FILE_TOWN_CREATE[] =
	"New town was successfully created and is awaiting your commands.\n";

static const char MSG_FILE_TOWN_DELETE[] =
	"Depending on how bad this case was, you should also get rid of the physical drive.\n";

static const char MSG_ERR_FILE_TOWN_DELETE[] =
	"%sTown files couldn't be deleted.\n";

static const char MSG_ERR_FILE_TOWN_LOAD[] =
	"%sTown could not be loaded.\n";

static const char MSG_FILE_TOWN_LOAD[] =
	"Town loaded.\n";

static const char MSG_WARN_FILE_TOWN_EXIST[] =
	"%sThe town already exists, type 'y' if you are sure to proceed.\n";

static const char MSG_WARN_FILE_TOWN_BACKUP[] =
	"%sCreating backup files of this town failed.\n";

static const char MSG_ERR_ADMIN_ID[] =
	"%sGiven admin id does not exist.\nUse \"%s\" command to make your decision.\n";

static const char MSG_TOWN_CREATION_STOPPED[] =
	"Town creation stopped.\n";

static const char MSG_ERR_SDL_INIT[] =
	"%sSDL could not be initialized.\n%s";

static const char MSG_ERR_SDL_WINDOW[] =
	"%sSDL could not open a window.\n%s";

static const char MSG_ERR_SDL_RENDERER[] =
	"%sSDL could not create a renderer.\n%s";

static const char MSG_ERR_TTF_INIT[] =
	"%sSDL_ttf could not be initialized.\n";

static const char MSG_ERR_FILE_READ[] =
	"%sFile \"%s\" could not be read.\n";

static const char MSG_ERR_IMAGE_LOAD[] =
	"%sTexture \"%s\" could not be loaded.\n";

static const char MSG_ERR_TEXT_CREATE[] =
	"%sTexture could no be created from text \"%s\".\n";

static const char MSG_ERR_FONT_LOAD[] =
	"%sFont \"%s\" could not be loaded.\nYou should change your config.\n";

static const char MSG_ERR_TEXTURE_CREATE[] =
	"%sTexture \"%s\" can not be used.\n";

/* ingame messages */
static const char MSG_CONNECTION_CLOSED[] =
	"-- Connection terminated --\n";

static const char MSG_PLAYER_FAILURE[] =
	"You have failed this town, overseer.\n";

static const char MSG_FAILURE_COST[] =
	"Your headquarter reported critical financial instabilities.\n";

#endif /* MESSAGES_H */
