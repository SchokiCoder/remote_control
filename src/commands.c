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

void cmd_help_outgame()
{
	printf("%s", HELP_TEXT_OUTSIDE);
}

void print_admin(
	uint32_t p_admin_id,
	char *p_lastname,
	char *p_firstname,
	uint8_t p_age,
	bool p_male,
	char *p_bio)
{
	printf("Admin ID:\t%i\nLastname:\t%s\nFirstname:\t%s\nAge:\t\t%i\nGender:\t\t%s\nBio:\n%s\n\n",
		p_admin_id, p_lastname, p_firstname, p_age, (p_male == true ? "male" : "female"), p_bio);
}

void cmd_list_admins()
{
	print_admin(ADMIN_1_ID, ADMIN_1_LASTNAME, ADMIN_1_FIRSTNAME, ADMIN_1_AGE, ADMIN_1_MALE, ADMIN_1_BIO);
	print_admin(ADMIN_2_ID, ADMIN_2_LASTNAME, ADMIN_2_FIRSTNAME, ADMIN_2_AGE, ADMIN_2_MALE, ADMIN_2_BIO);
	print_admin(ADMIN_3_ID, ADMIN_3_LASTNAME, ADMIN_3_FIRSTNAME, ADMIN_3_AGE, ADMIN_3_MALE, ADMIN_3_BIO);
}

void cmd_hire_admin(int32_t p_admin_id, char *p_town_name)
{
	struct Town new_game;
	uint32_t tree_chance;
	uint32_t tree_variance;
	time_t rng_seed;
	char filepath[FILEPATH_MAX_LEN] = "";
	FILE *f;
	char confirmation;

	time(&rng_seed);
	srand(rng_seed);

	//check given admin id
	if ((p_admin_id != ADMIN_1_ID) &
		(p_admin_id != ADMIN_2_ID) &
		(p_admin_id != ADMIN_3_ID))
	{
		printf(MSG_ERR_ADMIN_ID);
		return;
	}

	//check if town file already exists (by trying to read it)
	if (get_town_path(filepath) != 0)
		return;

	strcat(filepath, p_town_name);
	strcat(filepath, ".");
	strcat(filepath, FILETYPE_TOWN);

	f = fopen(filepath, "r");

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
		return;
	}

	//set values
	new_game.admin_id = p_admin_id;
	new_game.round = TOWN_TIME_BEGIN;
	new_game.money = TOWN_START_MONEY;

	//hide fields, generate trees
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			tree_chance = rand() % 100;

			//chance to generate tree
			if (tree_chance > TOWN_GEN_TREE_THRESHOLD)
			{
				//chance for species
				tree_variance = rand() % TOWN_TREE_VARIETY_COUNT;
				new_game.area_content[x][y] = (FIELD_TREE_0 + tree_variance);
			}
			else
			{
				new_game.area_content[x][y] = FIELD_EMPTY;
			}

			//hide field
			new_game.area_hidden[x][y] = true;
		}
	}

	//expose starting area
	for (uint32_t x = TOWN_EXPOSURE_AREA_BEGIN_X; x < TOWN_EXPOSURE_AREA_END_X; x++)
	{
		for (uint32_t y = TOWN_EXPOSURE_AREA_BEGIN_Y; y < TOWN_EXPOSURE_AREA_END_Y; y++)
		{
			new_game.area_hidden[x][y] = false;
		}
	}

	//clear tree-free zone from trees
	for (uint32_t x = TOWN_TREEFREE_AREA_BEGIN_X; x < TOWN_TREEFREE_AREA_END_X; x++)
	{
		for (uint32_t y = TOWN_TREEFREE_AREA_BEGIN_Y; y < TOWN_TREEFREE_AREA_END_Y; y++)
		{
			new_game.area_content[x][y] = FIELD_EMPTY;
		}
	}

	//set headquarter
	new_game.area_content[TOWN_HQ_SPAWN_X][TOWN_HQ_SPAWN_Y] = FIELD_ADMINISTRATION;

	//save town to file
	if(save_town(p_town_name, &new_game) == 0)
		printf(MSG_FILE_TOWN_CREATE);
}

void cmd_list_towns()
{
	char path[FILEPATH_MAX_LEN] = "";
	DIR *dir;
	struct dirent *d_ent;
	char *substr = NULL;

	//open town dir
	if (get_town_path(path) != 0)
		return;

	dir = opendir(path);

	if (dir == NULL)
	{
		printf(MSG_ERR_DIR_TOWNS);
	}

	//for all dirents
	while ((d_ent = readdir(dir)) != NULL)
	{
		//see if name contains filetype
		substr = strstr(d_ent->d_name, FILETYPE_TOWN);

		//if so, print and reset substr
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
	struct Town town;
	struct Config cfg;

	//load game
	if (load_town(p_town_name, &town) != 0)
		return;

	//set cfg to std values (set values will overwrite std)
	strcpy(cfg.path_font, CFG_STD_PATH_FONT);
	cfg.gfx_framerate = CFG_STD_GFX_FRAMERATE;
	cfg.gfx_window_x = CFG_STD_GFX_WINDOW_X;
	cfg.gfx_window_y = CFG_STD_GFX_WINDOW_Y;
	cfg.gfx_window_w = CFG_STD_GFX_WINDOW_W;
	cfg.gfx_window_h = CFG_STD_GFX_WINDOW_H;
	cfg.bg_red = CFG_STD_BG_RED;
	cfg.bg_green = CFG_STD_BG_GREEN;
	cfg.bg_blue = CFG_STD_BG_BLUE;
	cfg.font_red = CFG_STD_FONT_RED;
	cfg.font_green = CFG_STD_FONT_GREEN;
	cfg.font_blue = CFG_STD_FONT_BLUE;
	cfg.font_alpha = CFG_STD_FONT_ALPHA;
	cfg.field_border_red = CFG_STD_FIELD_BORDER_RED;
	cfg.field_border_green = CFG_STD_FIELD_BORDER_GREEN;
	cfg.field_border_blue = CFG_STD_FIELD_BORDER_BLUE;
	cfg.field_border_alpha = CFG_STD_FIELD_BORDER_ALPHA;

	//read config
	load_config(&cfg);

	//start game part
	gp_main(p_town_name, &town, &cfg);

	//end print
	printf(MSG_CONNECTION_CLOSED);
}

void cmd_delete(char *p_town_name)
{
	char filepath[FILEPATH_MAX_LEN] = "";

	//get path
	if (get_town_path(filepath) != 0)
		return;

	//glue file part to path
	strcat(filepath, p_town_name);
	strcat(filepath, ".");
	strcat(filepath, FILETYPE_TOWN);

	if (remove(filepath) != 0)
		printf(MSG_ERR_FILE_TOWN_DELETE);

	else
		printf(MSG_FILE_TOWN_DELETE);
}
