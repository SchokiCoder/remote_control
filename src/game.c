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
#include "game_commands.h"
#include "game.h"

int32_t gfx_game(void* p_data)
{
    bool active = true;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_Point mouse;
    struct GameData* data = (struct GameData*) (p_data);
    uint32_t ts_now = 0;
    uint32_t ts_render = 0;
   
    //init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf(MSG_ERR_SDL_INIT, SDL_GetError());
        data->rsp = GRSP_STOPPED;
        return 1;
    }

    //create window
    window = SDL_CreateWindow(
        data->town_name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf(MSG_ERR_SDL_WINDOW, SDL_GetError());
        data->rsp = GRSP_STOPPED;
        return 2;
    }

    //create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL)
    {
        printf(MSG_ERR_SDL_RENDERER, SDL_GetError());
        data->rsp = GRSP_STOPPED;
        return 3;
    }

    //set alpha channel
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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
        data->rsp = GRSP_STOPPED;
        return 4;
    }

    //calculate area rect and field size
    SDL_Rect area_rect;
    area_rect.w = WINDOW_WIDTH * SIZE_AREA_X;
    area_rect.h = WINDOW_HEIGHT * SIZE_AREA_Y;
    area_rect.x = (WINDOW_WIDTH - (float) area_rect.w) / 2.0f;
    area_rect.y = (WINDOW_HEIGHT - (float) area_rect.h) / 2.0f;

    float size_field_x = (float) area_rect.w / (float) TOWN_WIDTH;
    float size_field_y = (float) area_rect.h / (float) TOWN_HEIGHT;

    //calc field coords
    SDL_Rect field_rect[TOWN_WIDTH][TOWN_HEIGHT];

    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
        {
            field_rect[x][y].x = area_rect.x + (x * size_field_x);
            field_rect[x][y].y = area_rect.y + (y * size_field_y);
            field_rect[x][y].w = size_field_x;
            field_rect[x][y].h = size_field_y;
        }
    }

    //generate graphical information
    uint32_t ground_angles[TOWN_WIDTH][TOWN_HEIGHT];
    SDL_RendererFlip ground_flip[TOWN_WIDTH][TOWN_HEIGHT];
    uint32_t angle, flip;
    SDL_Texture* area_content_textures[TOWN_WIDTH][TOWN_HEIGHT];
    SDL_Rect field_content_rect[TOWN_WIDTH][TOWN_HEIGHT];

    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
        {
            //content texture rects
            field_content_rect[x][y].w = (float) field_rect[x][y].w * SIZE_FIELD_CONTENT;
            field_content_rect[x][y].h = (float) field_rect[x][y].h * SIZE_FIELD_CONTENT;
            field_content_rect[x][y].x = 
                (float) field_rect[x][y].x + 
                (((float) field_rect[x][y].w - (float) field_content_rect[x][y].w) / 2.0f);
            field_content_rect[x][y].y = 
                (float) field_rect[x][y].y + 
                (((float) field_rect[x][y].h - (float) field_content_rect[x][y].h) / 2.0f);

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
                    //if hidden draw black, else ground with content
                    if (data->town->area_hidden[x][y] == true)
                    {
                        SDL_SetRenderDrawColor(
                            renderer,
                            COLOR_FIELD_HIDDEN_RED,
                            COLOR_FIELD_HIDDEN_GREEN, 
                            COLOR_FIELD_HIDDEN_BLUE,
                            COLOR_FIELD_HIDDEN_ALPHA);
                        SDL_RenderFillRect(renderer, &field_rect[x][y]);
                    }
                    else
                    {
                        //draw ground
                        SDL_RenderCopyEx(renderer, sprite_ground.texture, NULL, &field_rect[x][y],
                            ground_angles[x][y],
                            NULL,
                            ground_flip[x][y]);

                        //if given, draw content texture
                        if (area_content_textures[x][y] != NULL)
                        {                        
                            SDL_RenderCopy(
                                renderer,
                                area_content_textures[x][y],
                                NULL,
                                &field_content_rect[x][y]);
                        }
                    }

                    //draw field border
                    SDL_SetRenderDrawColor(
                        renderer,
                        COLOR_FIELD_BORDER_RED,
                        COLOR_FIELD_BORDER_GREEN,
                        COLOR_FIELD_BORDER_BLUE,
                        COLOR_FIELD_BORDER_ALPHA);
                    SDL_RenderDrawRect(renderer, &field_rect[x][y]);
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

        case GCMD_STOP:
            active = false;
        break;
        }

        //handle sdl-events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEMOTION:
                //if mouse hovers in field
                SDL_GetMouseState(&mouse.x, &mouse.y);

                if (SDL_PointInRect(&mouse, &area_rect) == true)
                {
                    //calc in which
                    SDL_Point mouse_mapped;
                    mouse_mapped.x = mouse.x - area_rect.x;
                    mouse_mapped.y = mouse.y - area_rect.y;

                    SDL_Point field_hover;
                    field_hover.x = (float) mouse_mapped.x / size_field_x;
                    field_hover.y = (float) mouse_mapped.y / size_field_y;

                    //print TODO this is just for now
                    printf("Field hover: %i, %i\n", field_hover.x, field_hover.y);
                }
            break;
            
            case SDL_QUIT:
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
    char input[TRM_MAX_IN_LEN];
    char *split1, *split2;
    uint32_t argc;
    char argv[TRM_MAX_ARGS][TRM_MAX_ARG_LEN];
    
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
            //reset argc
            argc = 0;
            
            //take
            fgets(input, TRM_MAX_IN_LEN, stdin);

            //replace linebreak with \0
            split1 = strchr(input, '\n');
            *split1 = '\0';

            //split
            split1 = &input[0];

            for (uint32_t i = 0; i < TRM_MAX_ARGS; i++)
            {
                //increment arg counter
                argc++;

                //split
                split2 = strchr(split1, ' ');

                argv[i][0] = '\0';
                strncat(argv[i], split1, (size_t) (split2 - split1));

                //if not end, move pointers, else stop
                if (split2 != NULL)
                    split1 = split2 + 1;
                else
                    break;
            }

            //parse
            if (strcmp(argv[0], GM_CMD_HELP) == 0)
            {
                printf(HELP_TEXT_INGAME);
            }
            else if (strcmp(argv[0], GM_CMD_SAVE) == 0)
            {
                gm_cmd_save(data->town_name, data->town);
            }
            else if (strcmp(argv[0], GM_CMD_SAVE_AS) == 0)
            {
                gm_cmd_save_as(argv[1], data->town);
            }
            else if (strcmp(argv[0], GM_CMD_EXIT) == 0)
            {
                data->cmd = GCMD_STOP;
                enabled = false;
                active = false;
            }
        }
    }

    return 0;
}
