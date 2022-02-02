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

void Town_print(struct Town *self, char *p_town_name)
{
	/* name */
	printf("%s\n\n", p_town_name);

	/* exposure */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			printf("%i", self->hidden[x][y]);
		}

		printf("\n");
	}

	printf("\n");

	/* content */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			printf("%i", self->field[x][y]);
		}

		printf("\n");
	}
}

void Town_new(struct Town *self)
{
	self->admin_id = 0;
	self->construction_count = 0;
	self->money = TOWN_START_MONEY;
	self->round = TOWN_TIME_BEGIN;
}

int32_t Town_save(struct Town *self, char *p_town_name)
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
	strncat(filepath_save, p_town_name, (FILEPATH_MAX_LEN - strlen(filepath_save)));
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
	fwrite(&self->admin_id, sizeof(self->admin_id), 1, f);
	fwrite(&self->round, sizeof(self->round), 1, f);
	fwrite(&self->money, sizeof(self->money), 1, f);
	fwrite(&town_width, sizeof(uint32_t), 1, f);
	fwrite(&town_height, sizeof(uint32_t), 1, f);
	fputc('\n', f);

	/* write exposure data */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fwrite(self->hidden[x], sizeof(self->hidden[x][0]), TOWN_HEIGHT, f);
	}

	/* write content data */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fwrite(self->field[x], sizeof(self->field[x][0]), TOWN_HEIGHT, f);
	}

	fputc('\n', f);

	/* write construction list data */
	fwrite(&self->construction_count, sizeof(self->construction_count), 1, f);
	fputc('\n', f);

	/* write construction list */
	for (uint32_t i = 0; i < self->construction_count; i++)
	{
		fwrite(&self->constructions[i].field, sizeof(self->constructions[i].field), 1, f);
		fwrite(&self->constructions[i].coords.x, sizeof(self->constructions[i].coords.x), 1, f);
		fwrite(&self->constructions[i].coords.y, sizeof(self->constructions[i].coords.y), 1, f);
		fwrite(&self->constructions[i].progress, sizeof(self->constructions[i].progress), 1, f);
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

int32_t Town_load(struct Town *self, char *p_town_name)
{
	FILE *f;
	char filepath[FILEPATH_MAX_LEN] = "";
	uint32_t town_width, town_height;
	uint32_t file_major, file_minor, file_patch;

	/* get path */
	if (get_town_path(filepath) != 0)
		return 1;

	/* glue file part to path */
	strncat(filepath, p_town_name, (FILEPATH_MAX_LEN - strlen(filepath)));
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
	fread(&self->admin_id, sizeof(self->admin_id), 1, f);
	fread(&self->round, sizeof(self->round), 1, f);
	fread(&self->money, sizeof(self->money), 1, f);
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
		fread(self->hidden[x], sizeof(self->hidden[x][0]), TOWN_HEIGHT, f);
	}

	/* read content data */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fread(self->field[x], sizeof(self->field[x][0]), TOWN_HEIGHT, f);
	}
	fgetc(f);

	/* read construction list data */
	fread(&self->construction_count, sizeof(self->construction_count), 1, f);
	fgetc(f);

	/* read construction list */
	for (uint32_t i = 0; i < self->construction_count; i++)
	{
		fread(&self->constructions[i].field, sizeof(self->constructions[i].field), 1, f);
		fread(&self->constructions[i].coords.x, sizeof(self->constructions[i].coords.x), 1, f);
		fread(&self->constructions[i].coords.y, sizeof(self->constructions[i].coords.y), 1, f);
		fread(&self->constructions[i].progress, sizeof(self->constructions[i].progress), 1, f);
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

void Town_construction_list_remove(struct Town *self, uint32_t p_index)
{
	/* beginning at index, for each entry overwrite with next entry */
	for (uint32_t i = p_index; i < self->construction_count; i++)
	{
		self->constructions[i] = self->constructions[i + 1];
	}

	/* decrement count */
	self->construction_count--;
}
