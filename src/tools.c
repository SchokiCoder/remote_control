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
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "town.h"
#include "constants.h"
#include "tools.h"

void print_town(char* p_town_name, struct Town* p_in)
{
    //name
    printf("%s\n\n", p_town_name);
    
    //exposure
    for (uint32_t x = 0; x < 15; x++)
    {
        for (uint32_t y = 0; y < 15; y++)
        {
            printf("%i", p_in->area_hidden[x][y]);
        }

        printf("\n");
    }

    printf("\n");

    //content
    for (uint32_t x = 0; x < 15; x++)
    {
        for (uint32_t y = 0; y < 15; y++)
        {
            printf("%i", p_in->area_content[x][y]);
        }

        printf("\n");
    }
}

void save_town(char* p_town_name, struct Town* p_in)
{
    char filepath[1024] = "";
    FILE* f;
    DIR* dir;

    //glue path to save dir
    strcat(filepath, getenv("HOME"));
    strcat(filepath, "/.remote_control/");

    //check if save dir exists
    dir = opendir(filepath);

    if (dir)
        closedir(dir);
    else if (ENOENT == errno)
    {
        //create dir
        mkdir(filepath, S_IRWXU);
    }
    else
    {
        printf("Directory to saves is not available.\nMake sure you have enough permissions.\n");
        return;
    }

    //glue file part to path
    strcat(filepath, p_town_name);
    strcat(filepath, ".");
    strcat(filepath, FILETYPE_TOWN);

    //open file
    f = fopen(filepath, "w");

    if (f == NULL)
    {
        printf(MSG_ERR_FILE_SAVE);
        return;
    }

    //write exposure data
    fputc(1, f);
    fputc(0, f);
    fputc(2000000000, f);

    //write content data


    fclose(f);
}

void load_town(char* p_town_name, struct Town* p_out)
{}
