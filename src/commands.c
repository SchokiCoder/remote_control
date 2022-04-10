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
#include <SM_string.h>
#include "admins.h"
#include "messages.h"
#include "app.h"
#include "town.h"
#include "config.h"
#include "path.h"
#include "game.h"
#include "commands.h"

void print_cmd_help( const Command cmd )
{
	printf("  %s:\n", DATA_CMDS[cmd].desc);

	SM_String cmd_naming = SM_String_from(DATA_CMDS[cmd].name);

	if (DATA_CMDS[cmd].has_abbr)
	{
		SM_String_append_cstr(&cmd_naming, ", ");
		SM_String_append_cstr(&cmd_naming, DATA_CMDS[cmd].abbr);
	}

	printf("  %-32s%s\n", cmd_naming.str, DATA_CMDS[cmd].args);

	printf("\n");

	SM_String_clear(&cmd_naming);
}

void cmd_help_menu( void )
{
	printf(HELP_1ST_STEPS);
}

void cmd_help_full( void )
{
	cmd_help_menu();

	printf(HELP_APP, APP_NAME);
	printf("\n");

	for (uint_fast32_t i = 0; i <= CMD_LAST; i++)
	{
		print_cmd_help(i);
	}

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

void cmd_list_admins( void )
{
	for (uint32_t i = 0; i < (sizeof(DATA_ADMINS) / sizeof(DATA_ADMINS[0])); i++)
	{
		printf(
			"Admin ID:\t%i\nLastname:\t%s\nFirstname:\t%s\nAge:\t\t%i\nGender:\t\t%s\nBio:\n%s\n\n",
			i,
			DATA_ADMINS[i].last_name,
			DATA_ADMINS[i].first_name,
			DATA_ADMINS[i].age,
			(DATA_ADMINS[i].male == true ? "male" : "female"),
			DATA_ADMINS[i].bio);
	}
}

void cmd_hire_admin( const int32_t admin_id, const char *town_name )
{
	Town new_town = Town_new();
	uint32_t tree_chance;
	uint32_t tree_variance;
	SM_String filepath = SM_String_new(16);
	FILE *f;
	char confirmation;

	// init random gen
	srand(time(NULL));

	/* check given admin id */
	if (admin_id < 0 ||
		(uint32_t) admin_id > (sizeof(DATA_ADMINS) / sizeof(DATA_ADMINS[0])))
	{
		SM_String_clear(&filepath);
		printf(MSG_ERR_ADMIN_ID, DATA_CMDS[CMD_LIST_ADMINS].desc);
		return;
	}

	/* check if town file already exists (by trying to read it) */
	if (get_town_path(&filepath) != 0)
	{
		SM_String_clear(&filepath);
		return;
	}

	SM_String_append_cstr(&filepath, town_name);
	SM_String_append_cstr(&filepath, ".");
	SM_String_append_cstr(&filepath, FILETYPE_TOWN);

	f = fopen(filepath.str, "r");

	if (f != NULL)
	{
		printf(MSG_WARN_FILE_TOWN_EXIST);
		confirmation = getchar();
	}
	else
	{
		confirmation = 'y';
	}

	if (confirmation != 'y')
	{
		printf(MSG_TOWN_CREATION_STOPPED);
		SM_String_clear(&filepath);
		return;
	}

	/* set values */
	new_town.admin_id = admin_id;

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
				tree_variance = rand() % FIELD_TREE_COUNT;
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
	Town_save(&new_town, town_name);

	if(new_town.invalid == false)
		printf(MSG_FILE_TOWN_CREATE);

	// clear
	SM_String_clear(&filepath);
}

void cmd_list_towns( void )
{
	SM_String filepath = SM_String_new(16);
	DIR *dir;
	struct dirent *d_ent;
	char *substr = NULL;

	/* open town dir */
	if (get_town_path(&filepath) != 0)
	{
		SM_String_clear(&filepath);
		return;
	}

	dir = opendir(filepath.str);

	if (dir == NULL)
	{
		printf(MSG_ERR_DIR_TOWNS);
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
	printf("\n");

	closedir(dir);
	SM_String_clear(&filepath);
}

void cmd_connect( const char *town_name )
{
	Town town = Town_new();
	Config cfg = Config_new();
	Game game = {
		.town_name = town_name,
		.town = &town,
		.cfg = &cfg,
		.game_state = GS_ACTIVE
	};

	// load game
	Town_load(&town, town_name);

	if (town.invalid)
		return;

	// read config
	Config_load(&cfg);

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

void cmd_delete( const char *town_name )
{
	SM_String filepath = SM_String_new(16);

	// get path
	if (get_town_path(&filepath) != 0)
	{
		SM_String_clear(&filepath);
		return;
	}

	// glue file part to path
	SM_String_append_cstr(&filepath, town_name);
	SM_String_append_cstr(&filepath, ".");
	SM_String_append_cstr(&filepath, FILETYPE_TOWN);

	// delete and check
	if (remove(filepath.str) != 0)
		printf(MSG_ERR_FILE_TOWN_DELETE);
	else
		printf(MSG_FILE_TOWN_DELETE);

	SM_String_clear(&filepath);
}
