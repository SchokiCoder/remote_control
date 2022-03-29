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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "definitions/def_app.h"
#include "definitions/def_gameplay.h"
#include "definitions/def_files.h"
#include "definitions/def_messages.h"
#include "path.h"
#include "town.h"

Town Town_new( void )
{
	Town result = {
		.admin_id = 0,
		.construction_count = 0,
		.merc_count = 0,
		.money = TOWN_START_MONEY,
		.round = TOWN_START_TIME,
	};

	return result;
}

void Town_print( Town *town, char *town_name )
{
	/* name */
	printf("%s\n\n", town_name);

	/* exposure */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			printf("%i", town->hidden[x][y]);
		}

		printf("\n");
	}

	printf("\n");

	/* content */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			printf("%i", town->field[x][y]);
		}

		printf("\n");
	}
}

int32_t Town_save( Town *town, char *town_name )
{
	char filepath_save[FILEPATH_MAX_LEN] = "";
	char filepath_bkp[FILEPATH_MAX_LEN] = "";
	FILE *f;
	uint32_t town_width = TOWN_WIDTH;
	uint32_t town_height = TOWN_HEIGHT;

	/* get path */
	if (get_town_path(filepath_save) != 0)
		return 1;

	/* glue file part to path */
	strncat(filepath_save, town_name, (FILEPATH_MAX_LEN - strlen(filepath_save)));
	strncat(filepath_save, ".", (FILEPATH_MAX_LEN - strlen(filepath_save)));

	strncpy(filepath_bkp, filepath_save, FILEPATH_MAX_LEN);

	strncat(filepath_save, FILETYPE_TOWN, (FILEPATH_MAX_LEN - strlen(filepath_save)));
	strncat(filepath_bkp, FILETYPE_BACKUP, (FILEPATH_MAX_LEN - strlen(filepath_save)));

	/* if save already exists, move to backup */
	f = fopen(filepath_save, "r");

	if (f != NULL)
	{
		if (rename(filepath_save, filepath_bkp) != 0)
		{
			printf(MSG_WARN_FILE_TOWN_BACKUP, MSG_WARN);
		}

		fclose(f);
	}

	/* open file */
	f = fopen(filepath_save, "w");

	if (f == NULL)
	{
		printf(MSG_ERR_FILE_SAVE, MSG_ERR);
		return 2;
	}

	/* write header */
	fwrite(&APP_MAJOR, sizeof(APP_MAJOR), 1, f);
	fwrite(&APP_MINOR, sizeof(APP_MINOR), 1, f);
	fwrite(&APP_PATCH, sizeof(APP_PATCH), 1, f);
	fwrite(&town->admin_id, sizeof(town->admin_id), 1, f);
	fwrite(&town->round, sizeof(town->round), 1, f);
	fwrite(&town->money, sizeof(town->money), 1, f);
	fwrite(&town_width, sizeof(uint32_t), 1, f);
	fwrite(&town_height, sizeof(uint32_t), 1, f);
	fputc('\n', f);

	/* write exposure data */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fwrite(town->hidden[x], sizeof(town->hidden[x][0]), TOWN_HEIGHT, f);
	}

	/* write content data */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fwrite(town->field[x], sizeof(town->field[x][0]), TOWN_HEIGHT, f);
	}

	fputc('\n', f);

	/* write construction list data */
	fwrite(&town->construction_count, sizeof(town->construction_count), 1, f);
	fputc('\n', f);

	/* write construction list */
	for (uint32_t i = 0; i < town->construction_count; i++)
	{
		fwrite(&town->constructions[i].field, sizeof(town->constructions[i].field), 1, f);
		fwrite(&town->constructions[i].coords.x, sizeof(town->constructions[i].coords.x), 1, f);
		fwrite(&town->constructions[i].coords.y, sizeof(town->constructions[i].coords.y), 1, f);
		fwrite(&town->constructions[i].progress, sizeof(town->constructions[i].progress), 1, f);
	}

	// write merc list data
	fwrite(&town->merc_count, sizeof(town->merc_count), 1, f);
	fputc('\n', f);

	// write merc list
	for (uint32_t i = 0; i < town->merc_count; i++)
	{
		fwrite(&town->mercs[i].id, sizeof(town->mercs[i].id), 1, f);
		fwrite(&town->mercs[i].coords.x, sizeof(town->mercs[i].coords.x), 1, f);
		fwrite(&town->mercs[i].coords.y, sizeof(town->mercs[i].coords.y), 1, f);
		fwrite(&town->mercs[i].hp, sizeof(town->mercs[i].hp), 1, f);
		fwrite(&town->mercs[i].fraction, sizeof(town->mercs[i].fraction), 1, f);
	}

	/* check and done */
	if (ferror(f))
	{
		printf(MSG_ERR_FILE_TOWN_SAVE, MSG_ERR);
		fclose(f);
		return 3;
	}

	fclose(f);
	return 0;
}

int32_t Town_load( Town *town, char *town_name )
{
	FILE *f;
	char filepath[FILEPATH_MAX_LEN] = "";
	uint32_t town_width, town_height;
	uint32_t file_major, file_minor, file_patch;

	/* get path */
	if (get_town_path(filepath) != 0)
		return 1;

	/* glue file part to path */
	strncat(filepath, town_name, (FILEPATH_MAX_LEN - strlen(filepath)));
	strncat(filepath, ".", (FILEPATH_MAX_LEN - strlen(filepath)));
	strncat(filepath, FILETYPE_TOWN, (FILEPATH_MAX_LEN - strlen(filepath)));

	/* open */
	f = fopen(filepath, "r");

	if (f == NULL)
	{
		printf(MSG_ERR_FILE_LOAD, MSG_ERR);
		return 2;
	}

	/* read header */
	fread(&file_major, sizeof(file_major), 1, f);
	fread(&file_minor, sizeof(file_minor), 1, f);
	fread(&file_patch, sizeof(file_patch), 1, f);
	fread(&town->admin_id, sizeof(town->admin_id), 1, f);
	fread(&town->round, sizeof(town->round), 1, f);
	fread(&town->money, sizeof(town->money), 1, f);
	fread(&town_width, sizeof(town_width), 1, f);
	fread(&town_height, sizeof(town_height), 1, f);
	fgetc(f);

	/* check header info */
	if (
		!((town_width == TOWN_WIDTH) &
		(town_height == TOWN_HEIGHT))
	   )
	{
		printf(MSG_ERR_FILE_TOWN_CORRUPT, MSG_ERR);
		return 3;
	}

	/* read exposure data */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fread(town->hidden[x], sizeof(town->hidden[x][0]), TOWN_HEIGHT, f);
	}

	/* read content data */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fread(town->field[x], sizeof(town->field[x][0]), TOWN_HEIGHT, f);
	}
	fgetc(f);

	/* read construction list data */
	fread(&town->construction_count, sizeof(town->construction_count), 1, f);
	fgetc(f);

	/* read construction list */
	for (uint32_t i = 0; i < town->construction_count; i++)
	{
		fread(&town->constructions[i].field, sizeof(town->constructions[i].field), 1, f);
		fread(&town->constructions[i].coords.x, sizeof(town->constructions[i].coords.x), 1, f);
		fread(&town->constructions[i].coords.y, sizeof(town->constructions[i].coords.y), 1, f);
		fread(&town->constructions[i].progress, sizeof(town->constructions[i].progress), 1, f);
	}

	// read merc list data
	fread(&town->merc_count, sizeof(town->merc_count), 1, f);
	fgetc(f);

	// read merc list
	for (uint32_t i = 0; i < town->merc_count; i++)
	{
		fread(&town->mercs[i].id, sizeof(town->mercs[i].id), 1, f);
		fread(&town->mercs[i].coords.x, sizeof(town->mercs[i].coords.x), 1, f);
		fread(&town->mercs[i].coords.y, sizeof(town->mercs[i].coords.y), 1, f);
		fread(&town->mercs[i].hp, sizeof(town->mercs[i].hp), 1, f);
		fread(&town->mercs[i].fraction, sizeof(town->mercs[i].fraction), 1, f);
	}

	/* check and done */
	if (ferror(f))
	{
		printf(MSG_ERR_FILE_TOWN_LOAD, MSG_ERR);
		fclose(f);
		return 4;
	}

	printf(MSG_FILE_TOWN_LOAD);
	fclose(f);
	return 0;
}

void Town_construction_list_remove( Town *town, uint32_t index )
{
	/* beginning at index, for each entry overwrite with next entry */
	for (uint32_t i = index; i < town->construction_count; i++)
	{
		town->constructions[i] = town->constructions[i + 1];
	}

	/* decrement count */
	town->construction_count--;
}
