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

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"
#include "town.h"
#include "sprite.h"
#include "game.h"

int32_t gfx_game(void* p_data)
{
    bool active = true;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    struct GameData* data = (struct GameData*) (p_data);
    uint32_t ts_now = 0;
    uint32_t ts_render = 0;
   
    //init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf(MSG_ERR_SDL_INIT, SDL_GetError());
        return 1;
    }

    //create window
    window = SDL_CreateWindow(
        data->window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf(MSG_ERR_SDL_WINDOW, SDL_GetError());
        return 2;
    }

    //create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL)
    {
        printf(MSG_ERR_SDL_RENDERER, SDL_GetError());
        return 3;
    }

    //load sprites
    struct Sprite sprite_ground;
    struct Sprite sprite_hq;
    struct Sprite sprite_trees[TOWN_TREE_VARIETY_COUNT];

    if ((Sprite_init(&sprite_ground, PATH_TEXTURE_GROUND, renderer) != 0) |
        (Sprite_init(&sprite_hq, PATH_TEXTURE_HQ, renderer) != 0) |
        (Sprite_init(&sprite_trees[0], PATH_TEXTURE_TREE_0, renderer) != 0) |
        (Sprite_init(&sprite_trees[1], PATH_TEXTURE_TREE_1, renderer) != 0) |
        (Sprite_init(&sprite_trees[2], PATH_TEXTURE_TREE_2, renderer) != 0) |
        (Sprite_init(&sprite_trees[3], PATH_TEXTURE_TREE_3, renderer) != 0) |
        (Sprite_init(&sprite_trees[4], PATH_TEXTURE_TREE_4, renderer) != 0))
    {
        return 4;
    }

    //calculate size of area and fields
    float size_area_x = WINDOW_WIDTH / 1.0f * SIZE_AREA_X;
    float size_area_y = WINDOW_HEIGHT / 1.0f * SIZE_AREA_Y;
    float size_field_x = size_area_x / TOWN_WIDTH;
    float size_field_y = size_area_y / TOWN_HEIGHT;

    //calc render begin coord
    float coord_begin_x = (WINDOW_WIDTH - size_area_x) / 2.0f;
    float coord_begin_y = (WINDOW_HEIGHT - size_area_y) / 2.0f;

    //calc field coords
    SDL_Rect coords_fields[TOWN_WIDTH][TOWN_HEIGHT];

    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
        {
            coords_fields[x][y].x = coord_begin_x + (x * size_field_x);
            coords_fields[x][y].y = coord_begin_y + (y * size_field_y);
            coords_fields[x][y].w = size_field_x;
            coords_fields[x][y].h = size_field_y;
        }
    }

    //generate graphical information
    uint32_t ground_angles[TOWN_WIDTH][TOWN_HEIGHT];
    SDL_RendererFlip ground_flip[TOWN_WIDTH][TOWN_HEIGHT];
    uint32_t angle, flip;
    SDL_Texture* area_content_textures[TOWN_WIDTH][TOWN_HEIGHT];


    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
        {
            //ground texture angles
            angle = rand() % 3;
            ground_angles[x][y] = angle * 90;

            //ground texture flip
            flip = rand() % 3;

            switch(flip)
            {
            case 0:
                ground_flip[x][y] = SDL_FLIP_NONE;
            break;

            case 1:
                ground_flip[x][y] = SDL_FLIP_VERTICAL;
            break;
            
            case 2:
                ground_flip[x][y] = SDL_FLIP_HORIZONTAL;
            break;
            
            case 3:
                ground_flip[x][y] = SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL;
            break;
            }

            //map textures to area content
            switch (data->town->area_content[x][y])
            {
                case FIELD_ADMINISTRATION:
                     area_content_textures[x][y] = sprite_hq.texture; 
                break;

                case FIELD_TREE_0:
                     area_content_textures[x][y] = sprite_trees[0].texture;
                break;

                case FIELD_TREE_1:
                     area_content_textures[x][y] = sprite_trees[1].texture;
                break;

                case FIELD_TREE_2:
                     area_content_textures[x][y] = sprite_trees[2].texture;
                break;

                case FIELD_TREE_3:
                     area_content_textures[x][y] = sprite_trees[3].texture;
                break;

                case FIELD_TREE_4:
                     area_content_textures[x][y] = sprite_trees[4].texture;
                break;
          
                case FIELD_EMPTY:
                    area_content_textures[x][y] = NULL;
                break;           
            }
        }
    }

    //send signal (init successful)
    data->rsp = GRSP_INIT;

    //mainloop
    while (active)
    {
        if (ts_now > (ts_render + (1000 / FRAMERATE)))
        {
            //clear screen
            SDL_SetRenderDrawColor(renderer, COLOR_BG_RED, COLOR_BG_GREEN, COLOR_BG_BLUE, 255);
            SDL_RenderClear(renderer);
        
            //draw fields
            for (uint32_t x = 0; x < TOWN_WIDTH; x++)
            {
                for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
                {              
                    //determine which ground to draw
                    if (data->town->area_hidden[x][y] == true)
                    {
                        SDL_SetRenderDrawColor(
                            renderer,
                            COLOR_FIELD_HIDDEN_RED,
                            COLOR_FIELD_HIDDEN_GREEN, 
                            COLOR_FIELD_HIDDEN_BLUE,
                            COLOR_FIELD_HIDDEN_ALPHA);
                        SDL_RenderFillRect(renderer, &coords_fields[x][y]);
                    }
                    else
                    {
                        SDL_RenderCopyEx(renderer, sprite_ground.texture, NULL, &coords_fields[x][y],
                            ground_angles[x][y],
                            NULL,
                            ground_flip[x][y]);

                        //if given, draw content texture
                        if (area_content_textures[x][y] != NULL)
                        {                        
                            SDL_RenderCopy(renderer, area_content_textures[x][y], NULL, &coords_fields[x][y]);
                        }
                    }
                }
            }

            //show image, save time
            SDL_RenderPresent(renderer);
            ts_render = ts_now;
        }
        
        //update time
        ts_now = SDL_GetTicks();

        //handle terminal command-signals
        switch (data->cmd)
        {
        case GCMD_NONE:
        break;
        }

        //handle sdl-events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            //window closed
            case SDL_QUIT:
                //stop gfx mainloop
                active = false;
            break;
            }
        }
    }

    //send stop response
    data->rsp = GRSP_STOPPED;

    //destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    //quit sdl
    SDL_Quit();

    return 0;
}

int32_t terminal_game(struct GameData* data)
{
    bool active = true;     /* mainloop active */
    bool enabled = false;   /* describes if the console accepts input */
/*    char input[TRM_MAX_IN_LEN];
    char *split1, *split2;
    char args[TRM_MAX_ARGS][TRM_MAX_ARG_LEN];*/
    
    //mainloop
    while (active)
    {
        //handle responses
        switch (data->rsp)
        {
        case GRSP_NONE:
        break;

        case GRSP_INIT:
            enabled = true;
        break;

        case GRSP_STOPPED:
            enabled = false;
            active = false;
        break;
        }

        //if enabled, handle input
        if (enabled)
        {
            /*//take
            fgets(input, TRM_MAX_IN_LEN, stdin);

            split1 = &input[0];

            //split
            for (
                uint32_t i = 0;
                i < TRM_MAX_ARGS && ((split2 = strchr(split1, ' ')) != NULL);
                i++)
            {
                strncpy(args[i], split1, (size_t) (split2 - split1));

                split1 = split2;
            }

            //parse
            for (uint32_t i = 0; i < TRM_MAX_ARGS; i++)
            {
                printf("%s\n", args[i]);
            }*/
        }
    }

    return 0;
}
