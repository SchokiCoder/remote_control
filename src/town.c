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
#include "constants.h"
#include "path.h"
#include "town.h"

void print_town(char* p_town_name, struct Town* p_in)
{
	//name
	printf("%s\n\n", p_town_name);
	
	//exposure
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			printf("%i", p_in->area_hidden[x][y]);
		}

		printf("\n");
	}

	printf("\n");

	//content
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			printf("%i", p_in->area_content[x][y]);
		}

		printf("\n");
	}
}

int32_t save_town(char* p_town_name, struct Town* p_in)
{
	char filepath_save[FILEPATH_MAX_LEN] = "";
	char filepath_bkp[FILEPATH_MAX_LEN] = "";
	FILE* f;
	uint32_t town_width = TOWN_WIDTH;
	uint32_t town_height = TOWN_HEIGHT;

	//get path
	if (get_town_path(filepath_save) != 0)
		return 1;
	
	//glue file part to path
	strcat(filepath_save, p_town_name);
	strcat(filepath_save, ".");

	strcpy(filepath_bkp, filepath_save);

	strcat(filepath_save, FILETYPE_TOWN);
	strcat(filepath_bkp, FILETYPE_BACKUP);

	//if save already exists, move to backup
	f = fopen(filepath_save, "r");

	if (f != NULL)
	{
		if (rename(filepath_save, filepath_bkp) != 0)
		{
			printf(MSG_WARN_FILE_TOWN_BACKUP);
		}

		fclose(f);
	}

	//open file
	f = fopen(filepath_save, "w");

	if (f == NULL)
	{
		printf(MSG_ERR_FILE_SAVE);
		return 2;
	}

	//write header
	fwrite(&p_in->admin_id, sizeof(p_in->admin_id), 1, f);
	fwrite(&p_in->round, sizeof(p_in->round), 1, f);
	fwrite(&town_width, sizeof(uint32_t), 1, f);
	fwrite(&town_height, sizeof(uint32_t), 1, f);
	fputc('\n', f);

	//write exposure data
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fwrite(p_in->area_hidden[x], sizeof(p_in->area_hidden[x][0]), TOWN_HEIGHT, f);
	}

	//write content data
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fwrite(p_in->area_content[x], sizeof(p_in->area_content[x][0]), TOWN_HEIGHT, f);
	}

	//check and done
	if (ferror(f))
	{
		printf(MSG_ERR_FILE_TOWN_SAVE);
		fclose(f);
		return 3;
	}
	
	fclose(f);
	return 0;
}

int32_t load_town(char* p_town_name, struct Town* p_out)
{
	FILE *f;
	char filepath[FILEPATH_MAX_LEN] = "";
	uint32_t town_width, town_height;
	
	//get path
	if (get_town_path(filepath) != 0)
		return 1;

	//glue file part to path
	strcat(filepath, p_town_name);
	strcat(filepath, ".");
	strcat(filepath, FILETYPE_TOWN);

	//open
	f = fopen(filepath, "r");

	if (f == NULL)
	{
		printf(MSG_ERR_FILE_LOAD);
		return 2;
	}

	//read header
	fread(&p_out->admin_id, sizeof(p_out->admin_id), 1, f);
	fread(&p_out->round, sizeof(p_out->round), 1, f);
	fread(&town_width, sizeof(town_width), 1, f);
	fread(&town_height, sizeof(town_height), 1, f);
	fgetc(f);

	//check header info
	if (
		!((town_width == TOWN_WIDTH) &
		(town_height == TOWN_HEIGHT))
	   )
	{
		printf(MSG_ERR_FILE_TOWN_CORRUPT);
		return 3;
	}

	//read exposure data
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fread(p_out->area_hidden[x], sizeof(p_out->area_hidden[x][0]), TOWN_HEIGHT, f);
	}

	//read content data
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		fread(p_out->area_content[x], sizeof(p_out->area_content[x][0]), TOWN_HEIGHT, f);
	}

	//check and done
	if (ferror(f))
	{
		printf(MSG_ERR_FILE_TOWN_LOAD);
		fclose(f);
		return 4;
	}
	
	printf(MSG_FILE_TOWN_LOAD);
	fclose(f);
	return 0;
}
