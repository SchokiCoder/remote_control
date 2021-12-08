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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "town.h"
#include "constants.h"
#include "path.h"
#include "tools.h"

void print_town(char* p_town_name, struct Town* p_in)
{
    //name
    printf("%s\n\n", p_town_name);
    
    //exposure
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_DEPTH; y++)
        {
            printf("%i", p_in->area_hidden[x][y]);
        }

        printf("\n");
    }

    printf("\n");

    //content
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_DEPTH; y++)
        {
            printf("%i", p_in->area_content[x][y]);
        }

        printf("\n");
    }
}

int32_t save_town(char* p_town_name, struct Town* p_in)
{
    char filepath[1024] = "";
    FILE* f;
    uint32_t town_width = TOWN_WIDTH;
    uint32_t town_depth = TOWN_DEPTH;

    //get path
    if (get_town_path(filepath) != 0)
        return 1;
    
    //glue file part to path
    strcat(filepath, p_town_name);
    strcat(filepath, ".");
    strcat(filepath, FILETYPE_TOWN);

    //open file
    f = fopen(filepath, "w");

    if (f == NULL)
    {
        printf(MSG_ERR_FILE_SAVE);
        return 2;
    }

    //write header
    fwrite(&town_width, sizeof(town_width), 1, f);
    fwrite(&town_depth, sizeof(town_depth), 1, f);
    fputc('\n', f);

    //write exposure data
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        fwrite(p_in->area_hidden[x], sizeof(p_in->area_hidden[x][0]), TOWN_DEPTH, f);
    }

    //write content data
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        fwrite(p_in->area_content[x], sizeof(p_in->area_content[x][0]), TOWN_DEPTH, f);
    }

    //check and done
    if (ferror(f))
    {
        printf(MSG_ERR_FILE_TOWN_SAVE);
        fclose(f);
        return 3;
    }
    
    printf(MSG_FILE_TOWN_SAVE);
    fclose(f);
    return 0;
}

int32_t load_town(char* p_town_name, struct Town* p_out)
{
    FILE *f;
    char filepath[1024] = "";
    uint32_t town_width, town_depth;
    
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
    fread(&town_width, sizeof(town_width), 1, f);
    fread(&town_depth, sizeof(town_depth), 1, f);
    fgetc(f);

    //check header info
    if (
        !((town_width == TOWN_WIDTH) &
        (town_depth == TOWN_DEPTH))
       )
    {
        printf(MSG_ERR_FILE_TOWN_CORRUPT);
        return 3;
    }

    //read exposure data
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        fread(p_out->area_hidden[x], sizeof(p_out->area_hidden[x][0]), TOWN_DEPTH, f);
    }

    //read content data
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        fread(p_out->area_content[x], sizeof(p_out->area_content[x][0]), TOWN_DEPTH, f);
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
