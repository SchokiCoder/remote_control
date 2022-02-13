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

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <SDL.h>
#include "definitions/def_gameplay.h"
#include "definitions/def_admins.h"
#include "definitions/def_messages.h"
#include "definitions/def_files.h"
#include "town.h"
#include "config.h"
#include "path.h"
#include "game.h"
#include "commands.h"

void cmd_help_menu()
{
	printf(HELP_1ST_STEPS);
}

void cmd_help_full()
{
	cmd_help_menu();

	printf(
		HELP_COMMANDS,
		APP_NAME,
		CMD_HELP_LONG, CMD_HELP,
		CMD_LIST_ADMINS_LONG, CMD_LIST_ADMINS,
		CMD_HIRE_ADMIN_LONG, CMD_HIRE_ADMIN,
		CMD_LIST_TOWNS_LONG, CMD_LIST_TOWNS,
		CMD_CONNECT_LONG, CMD_CONNECT,
		CMD_DELETE_TOWN_LONG);

	printf(
		HELP_LICENSE,
		APP_NAME,
		APP_MAJOR,
		APP_MINOR,
		APP_PATCH,
		APP_LICENSE,
		APP_LICENSE_NOTICE,
		APP_SOURCE);
}

void cmd_list_admins()
{
	for (uint32_t i = 0; i < (sizeof(ADMIN_ID) / sizeof(ADMIN_ID[0])); i++)
	{
		printf(
			"Admin ID:\t%i\nLastname:\t%s\nFirstname:\t%s\nAge:\t\t%i\nGender:\t\t%s\nBio:\n%s\n\n",
			ADMIN_ID[i],
			ADMIN_LASTNAME[i],
			ADMIN_FIRSTNAME[i],
			ADMIN_AGE[i],
			(ADMIN_MALE[i] == true ? "male" : "female"),
			ADMIN_BIO[i]);
	}
}

void cmd_hire_admin(int32_t p_admin_id, char *p_town_name)
{
	struct Town new_town;
	uint32_t tree_chance;
	uint32_t tree_variance;
	char filepath[FILEPATH_MAX_LEN] = "";
	FILE *f;
	char confirmation;

	// init random gen
	srand(time(NULL));

	/* check given admin id */
	if ((p_admin_id != ADMIN_ID[0]) &
		(p_admin_id != ADMIN_ID[1]) &
		(p_admin_id != ADMIN_ID[2]))
	{
		printf(MSG_ERR_ADMIN_ID, MSG_ERR, CMD_LIST_ADMINS_LONG);
		return;
	}

	/* check if town file already exists (by trying to read it) */
	if (get_town_path(filepath) != 0)
		return;

	strncat(filepath, p_town_name, (FILEPATH_MAX_LEN - strlen(filepath)));
	strncat(filepath, ".", (FILEPATH_MAX_LEN - strlen(filepath)));
	strncat(filepath, FILETYPE_TOWN, (FILEPATH_MAX_LEN - strlen(filepath)));

	f = fopen(filepath, "r");

	if (f != NULL)
	{
		printf(MSG_WARN_FILE_TOWN_EXIST, MSG_WARN);
		confirmation = getchar();
	}
	else
	{
		confirmation = 'y';
	}

	if (confirmation != 'y')
	{
		printf(MSG_TOWN_CREATION_STOPPED);
		return;
	}

	/* set values */
	Town_new(&new_town);
	new_town.admin_id = p_admin_id;

	/* hide fields, generate trees */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			tree_chance = rand() % 100;

			/* chance to generate tree */
			if (tree_chance > TOWN_GEN_TREE_THRESHOLD)
			{
				/* chance for species */
				tree_variance = rand() % TOWN_TREE_VARIETY_COUNT;
				new_town.field[x][y] = (FIELD_TREE_0 + tree_variance);
			}
			else
			{
				new_town.field[x][y] = FIELD_EMPTY;
			}

			/* hide field */
			new_town.hidden[x][y] = true;
		}
	}

	/* expose starting area */
	for (uint32_t x = TOWN_EXPOSURE_AREA_BEGIN_X; x < TOWN_EXPOSURE_AREA_END_X; x++)
	{
		for (uint32_t y = TOWN_EXPOSURE_AREA_BEGIN_Y; y < TOWN_EXPOSURE_AREA_END_Y; y++)
		{
			new_town.hidden[x][y] = false;
		}
	}

	/* clear tree-free zone from trees */
	for (uint32_t x = TOWN_TREEFREE_AREA_BEGIN_X; x < TOWN_TREEFREE_AREA_END_X; x++)
	{
		for (uint32_t y = TOWN_TREEFREE_AREA_BEGIN_Y; y < TOWN_TREEFREE_AREA_END_Y; y++)
		{
			new_town.field[x][y] = FIELD_EMPTY;
		}
	}

	/* set headquarter */
	new_town.field[TOWN_HQ_SPAWN_X][TOWN_HQ_SPAWN_Y] = FIELD_ADMINISTRATION;

	/* save town to file */
	if(Town_save(&new_town, p_town_name) == 0)
		printf(MSG_FILE_TOWN_CREATE);
}

void cmd_list_towns()
{
	char path[FILEPATH_MAX_LEN] = "";
	DIR *dir;
	struct dirent *d_ent;
	char *substr = NULL;

	/* open town dir */
	if (get_town_path(path) != 0)
		return;

	dir = opendir(path);

	if (dir == NULL)
	{
		printf(MSG_ERR_DIR_TOWNS, MSG_ERR);
	}

	/* for all dirents */
	while ((d_ent = readdir(dir)) != NULL)
	{
		/* see if name contains filetype */
		substr = strstr(d_ent->d_name, FILETYPE_TOWN);

		/* if so, print and reset substr */
		if (substr != NULL)
		{
			printf("%s\n", d_ent->d_name);
			substr = NULL;
		}
	}

	closedir(dir);

	printf("\n");
}

void cmd_connect(char *p_town_name)
{
	struct Game game;
	struct Town town;
	struct Config cfg;

	/* load game */
	Town_new(&town);

	if (Town_load(&town, p_town_name) != 0)
		return;

	/* set cfg to std values (set values will overwrite std) */
	Config_new(&cfg);

	/* read config */
	Config_load(&cfg);

	/* prepare gameplay data */
	game.town_name = p_town_name;
	game.town = &town;
	game.cfg = &cfg;
	game.game_state = GS_ACTIVE;

	/* start game part */
	Game_main(&game);

	/* if gameover print reason */
	switch (game.game_state)
	{
	case GS_ACTIVE:
		break;
	case GS_CLOSE:
		break;

	case GS_FAILURE_COST:
		printf(MSG_FAILURE_COST);
		break;
	}

	/* end print */
	printf(MSG_CONNECTION_CLOSED);
}

void cmd_delete(char *p_town_name)
{
	char filepath[FILEPATH_MAX_LEN] = "";

	/* get path */
	if (get_town_path(filepath) != 0)
		return;

	/* glue file part to path */
	strncat(filepath, p_town_name, (FILEPATH_MAX_LEN - strlen(filepath)));
	strncat(filepath, ".", (FILEPATH_MAX_LEN - strlen(filepath)));
	strncat(filepath, FILETYPE_TOWN, (FILEPATH_MAX_LEN - strlen(filepath)));

	if (remove(filepath) != 0)
		printf(MSG_ERR_FILE_TOWN_DELETE, MSG_ERR);

	else
		printf(MSG_FILE_TOWN_DELETE);
}
