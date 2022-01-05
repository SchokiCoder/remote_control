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

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include "constants.h"
#include "administrators.h"
#include "town.h"
#include "tools.h"
#include "path.h"
#include "game.h"
#include "commands.h"

void cmd_help_outgame()
{
    printf("%s", HELP_TEXT_OUTSIDE);
}

void print_admin(uint32_t p_admin_id, char* p_lastname, char* p_firstname, uint8_t p_age, bool p_male, char* p_bio)
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

void cmd_hire_admin(int32_t p_admin_id, char* p_town_name)
{
    struct Town new_game;
    float tree_chance;
    time_t rng_seed;
    char filepath[1024] = "";
    FILE* f;
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

    //get values
    new_game.admin_id = p_admin_id;
    
    //hide fields, generate trees
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
        {
            tree_chance = rand() % 100;
            
            new_game.area_content[x][y] = (tree_chance > TOWN_GEN_TREE_THRESHOLD ? FIELD_EMPTY : FIELD_TREE);
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
    save_town(p_town_name, &new_game);
}

void cmd_list_towns()
{
    char path[1024] = "";
    DIR* dir;
    struct dirent* d_ent;
    char* substr = NULL;

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

void cmd_connect(char* p_town_name)
{
    struct Town town;
    struct GameData game_data;
    SDL_Thread* gfx_thread;
    int32_t gfx_rc;
    
    //load game
    if (load_town(p_town_name, &town) != 0)
        return;

    //prepare data of gfx part
    game_data.window_title = p_town_name;
    game_data.town = &town;
    game_data.cmd = GCMD_NONE;
    game_data.rsp = GRSP_NONE;

    //start gfx part in seperate thread
    gfx_thread = SDL_CreateThread(gfx_game, "thread_gfx", &game_data);

    //start terminal game part
    terminal_game(&game_data);

    //wait for gfx thread to finish
    SDL_WaitThread(gfx_thread, &gfx_rc);
    gfx_thread = NULL;
}
