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
    struct Sprite sprite_hq;
    struct Sprite sprite_trees[1];

    if ((Sprite_from(&sprite_hq, PATH_TEXTURE_HQ, renderer) != 0) |
        (Sprite_from(&sprite_trees[0], PATH_TEXTURE_TREE_0, renderer) != 0))
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

    //mainloop
    while (active)
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
                    SDL_SetRenderDrawColor(
                        renderer,
                        COLOR_FIELD_EXPOSED_RED,
                        COLOR_FIELD_EXPOSED_GREEN, 
                        COLOR_FIELD_EXPOSED_BLUE,
                        COLOR_FIELD_EXPOSED_ALPHA);
                    SDL_RenderFillRect(renderer, &coords_fields[x][y]);

                    //determine which texture to draw onto the ground
                    switch (data->town->area_content[x][y])
                    {
                    case FIELD_ADMINISTRATION:
                       SDL_RenderCopy(renderer, sprite_hq.texture, NULL, &coords_fields[x][y]); 
                    break;

                    case FIELD_TREE:
                        SDL_RenderCopy(renderer, sprite_trees[0].texture, NULL, &coords_fields[x][y]);
                    break;

                    case FIELD_EMPTY:
                    break;
                    }
                }
            }
        }

        //show image
        SDL_RenderPresent(renderer);
        
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
    bool active = true;
    
    //mainloop
    while (active)
    {
        switch (data->rsp)
        {
        case GRSP_NONE:
        break;

        //gfx window stopped
        case GRSP_STOPPED:
            //stop
            active = false;
        break;
        }
    }

    return 0;
}
