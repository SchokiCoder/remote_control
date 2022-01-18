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
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"
#include "town.h"
#include "config.h"
#include "sprite.h"
#include "game_commands.h"
#include "game.h"

void calculate_ui(
    float p_window_w,
    float p_window_h,
    SDL_Rect *p_hover_label_rect,
    SDL_Rect *p_hover_x_rect,
    SDL_Rect *p_hover_y_rect,
    SDL_Rect *p_area_rect,
    float *p_field_width,
    float *p_field_height,
    SDL_Rect p_field_rects[TOWN_WIDTH][TOWN_HEIGHT],
    SDL_Rect p_field_content_rects[TOWN_WIDTH][TOWN_HEIGHT])
{
    //calc hover text rects
    p_hover_label_rect->x = p_window_w * UI_TEXT_HOVER_LABEL_X;
    p_hover_label_rect->y = p_window_h * UI_TEXT_HOVER_LABEL_Y;
    p_hover_label_rect->w = p_window_w * UI_TEXT_HOVER_LABEL_W;
    p_hover_label_rect->h = p_window_h * UI_TEXT_HOVER_LABEL_H;

    p_hover_x_rect->x = p_window_w * UI_TEXT_HOVER_X_X;
    p_hover_x_rect->y = p_window_h * UI_TEXT_HOVER_X_Y;
    p_hover_x_rect->w = p_window_w * UI_TEXT_HOVER_X_W;
    p_hover_x_rect->h = p_window_h * UI_TEXT_HOVER_X_H;

    p_hover_y_rect->x = p_window_w * UI_TEXT_HOVER_Y_X;
    p_hover_y_rect->y = p_window_h * UI_TEXT_HOVER_Y_Y;
    p_hover_y_rect->w = p_window_w * UI_TEXT_HOVER_Y_W;
    p_hover_y_rect->h = p_window_h * UI_TEXT_HOVER_Y_H;

    /* calculate area pos and size
        -as wide as high
        -stay on top and right
    */
    p_area_rect->h = p_window_h * UI_AREA_H;
    p_area_rect->w = p_area_rect->h;
    p_area_rect->y = p_window_h * UI_AREA_Y;
    p_area_rect->x = (p_window_w * UI_AREA_X2) - p_area_rect->w;
   
    //calc field size
    *p_field_width = (float) p_area_rect->w / (float) TOWN_WIDTH;
    *p_field_height = (float) p_area_rect->h / (float) TOWN_HEIGHT;

    //calc field rects
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
        {
            //full rect
            p_field_rects[x][y].x = p_area_rect->x + (x * *p_field_width);
            p_field_rects[x][y].y = p_area_rect->y + (y * *p_field_height);
            p_field_rects[x][y].w = *p_field_width;
            p_field_rects[x][y].h = *p_field_height;

            //content texture rect
            p_field_content_rects[x][y].w = (float) p_field_rects[x][y].w * UI_FIELD_CONTENT_SIZE;
            p_field_content_rects[x][y].h = (float) p_field_rects[x][y].h * UI_FIELD_CONTENT_SIZE;
            p_field_content_rects[x][y].x = 
                (float) p_field_rects[x][y].x + 
                (((float) p_field_rects[x][y].w - (float) p_field_content_rects[x][y].w) / 2.0f);
            p_field_content_rects[x][y].y = 
                (float) p_field_rects[x][y].y + 
                (((float) p_field_rects[x][y].h - (float) p_field_content_rects[x][y].h) / 2.0f);
        }
    }
}

int32_t gfx_game(void* p_data)
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    TTF_Font *font;

    bool active = true;
    uint32_t ts_now = 0;
    uint32_t ts_render = 0;

    struct GameData* data = (struct GameData*) (p_data);

    SDL_Event event;
    SDL_Point mouse;
    int32_t border_t, border_l;
    int32_t window_x, window_y;
    int32_t window_w, window_h;

    SDL_Rect hover_label_rect;
    SDL_Rect hover_x_rect;
    SDL_Rect hover_y_rect;
    SDL_Rect area_rect;
    float field_width, field_height;
    SDL_Rect field_rects[TOWN_WIDTH][TOWN_HEIGHT];
    SDL_Rect field_content_rects[TOWN_WIDTH][TOWN_HEIGHT];

    char hover_x[3] = "";
    char hover_y[3] = "";

    struct Sprite sprite_ground;
    struct Sprite sprite_hq;
    struct Sprite sprite_trees[TOWN_TREE_VARIETY_COUNT];
    struct Sprite sprite_hover_label;
    struct Sprite sprite_hover_x;
    struct Sprite sprite_hover_y;

    uint32_t ground_angles[TOWN_WIDTH][TOWN_HEIGHT];
    SDL_RendererFlip ground_flip[TOWN_WIDTH][TOWN_HEIGHT];
    uint32_t angle, flip; 
    SDL_Texture* area_content_textures[TOWN_WIDTH][TOWN_HEIGHT];
    SDL_Color font_color;

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
        data->cfg->gfx_window_x,
        data->cfg->gfx_window_y,
        data->cfg->gfx_window_w,
        data->cfg->gfx_window_h,
        (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

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

    //init ttf
    if (TTF_Init() != 0)
    {
        printf(MSG_ERR_TTF_INIT);
        return 4;
    }

    //load font
    font = TTF_OpenFont(data->cfg->path_font, UI_FONT_SIZE);

    if (font == NULL)
    {
        printf(MSG_ERR_FONT_LOAD, data->cfg->path_font);
        return 5;
    }

    //set font color
    font_color.r = data->cfg->font_red;
    font_color.g = data->cfg->font_green;
    font_color.b = data->cfg->font_blue;
    font_color.a = data->cfg->font_alpha;

    //load sprites and create text sprite
    if ((Sprite_from_image(&sprite_ground, renderer, PATH_TEXTURE_GROUND) != 0) ||
        (Sprite_from_image(&sprite_hq, renderer, PATH_TEXTURE_HQ) != 0) ||
        (Sprite_from_image(&sprite_trees[0], renderer, PATH_TEXTURE_TREE_0) != 0) ||
        (Sprite_from_image(&sprite_trees[1], renderer, PATH_TEXTURE_TREE_1) != 0) ||
        (Sprite_from_image(&sprite_trees[2], renderer, PATH_TEXTURE_TREE_2) != 0) ||
        (Sprite_from_image(&sprite_trees[3], renderer, PATH_TEXTURE_TREE_3) != 0) ||
        (Sprite_from_image(&sprite_trees[4], renderer, PATH_TEXTURE_TREE_4) != 0) ||
        (Sprite_from_text(&sprite_hover_label, renderer, UI_TEXT_HOVER_LABEL, font, font_color) != 0) ||
        (Sprite_from_text(&sprite_hover_x, renderer, "0", font, font_color) != 0) ||
        (Sprite_from_text(&sprite_hover_y, renderer, "0", font, font_color) != 0))
    {
        data->rsp = GRSP_STOPPED;
        return 6;
    }

    //calculate ui sizes and positions
    calculate_ui(
        data->cfg->gfx_window_w,
        data->cfg->gfx_window_h,
        &hover_label_rect,
        &hover_x_rect,
        &hover_y_rect,
        &area_rect,
        &field_width,
        &field_height,
        field_rects,
        field_content_rects);

    //generate texture rotation and flip
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
        {
            //ground texture angles
            angle = rand() % 3;
            ground_angles[x][y] = angle * 90;

            //ground texture flip
            flip = rand() % 3;

            switch (flip)
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
        if (ts_now > (ts_render + (1000.0f / data->cfg->gfx_framerate)))
        {
            //clear screen
            SDL_SetRenderDrawColor(renderer, data->cfg->bg_red, data->cfg->bg_green, data->cfg->bg_blue, 255);
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
                        SDL_RenderFillRect(renderer, &field_rects[x][y]);
                    }
                    else
                    {
                        //draw ground
                        SDL_RenderCopyEx(renderer, sprite_ground.texture, NULL, &field_rects[x][y],
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
                                &field_content_rects[x][y]);
                        }
                    }

                    //draw field border
                    SDL_SetRenderDrawColor(
                        renderer,
                        data->cfg->field_border_red,
                        data->cfg->field_border_green,
                        data->cfg->field_border_blue,
                        data->cfg->field_border_alpha);
                    SDL_RenderDrawRect(renderer, &field_rects[x][y]);
                }
            }

            //if mouse hovers on area, draw hover text fields
            if (hover_x[0] != '\0' && hover_y[0] != '\0')
            {
                SDL_RenderCopy(renderer, sprite_hover_label.texture, NULL, &hover_label_rect);
                SDL_RenderCopy(renderer, sprite_hover_x.texture, NULL, &hover_x_rect);
                SDL_RenderCopy(renderer, sprite_hover_y.texture, NULL, &hover_y_rect);
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
                    sprintf(hover_x, "%u", (uint32_t) ((float) (mouse.x - area_rect.x) / field_width));
                    sprintf(hover_y, "%u", (uint32_t) ((float) (mouse.y - area_rect.y) / field_height));

                    //reset sprites
                    Sprite_from_text(&sprite_hover_x, renderer, hover_x, font, font_color);
                    Sprite_from_text(&sprite_hover_y, renderer, hover_y, font, font_color);
                }
                else
                {
                    hover_x[0] = '\0';
                }
                break;
            
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    //set config window size and pos
                    SDL_GetWindowBordersSize(window, &border_t, &border_l, NULL, NULL);
                    SDL_GetWindowPosition(window, &window_x, &window_y);
                    SDL_GetWindowSize(window, &window_w, &window_h);

                    data->cfg->gfx_window_x = (float) window_x - (float) border_l;
                    data->cfg->gfx_window_y = (float) window_y - (float) border_t;
                    data->cfg->gfx_window_w = (float) window_w;
                    data->cfg->gfx_window_h = (float) window_h;

                    //recalc ui sizes and pos
                    calculate_ui(
                        data->cfg->gfx_window_w,
                        data->cfg->gfx_window_h,
                        &hover_label_rect,
                        &hover_x_rect,
                        &hover_y_rect,
                        &area_rect,
                        &field_width,
                        &field_height,
                        field_rects,
                        field_content_rects);
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

    //save config
    save_config(data->cfg);

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
                //check arg max
                if (argc > 1)
                {
                    printf(MSG_WARN_ARG_MAX);
                }

                printf(HELP_TEXT_INGAME);
            }
            else if (strcmp(argv[0], GM_CMD_SAVE) == 0)
            {
                //check arg max
                if (argc > 1)
                {
                    printf(MSG_WARN_ARG_MAX);
                }

                gm_cmd_save(data->town_name, data->town);
            }
            else if (strcmp(argv[0], GM_CMD_SAVE_AS) == 0)
            {
                //check arg min
                if (argc < 2)
                {
                    printf(MSG_ERR_ARG_MIN);
                    break;
                }

                //check arg max
                else if (argc > 2)
                {
                    printf(MSG_WARN_ARG_MAX);
                }

                gm_cmd_save_as(argv[1], data->town);
            }
            else if (strcmp(argv[0], GM_CMD_EXIT) == 0)
            {
                //check arg max
                if (argc > 1)
                {
                    printf(MSG_WARN_ARG_MAX);
                }

                data->cmd = GCMD_STOP;
                enabled = false;
                active = false;
            }
            else if (strcmp(argv[0], GM_CMD_SHOW_CONFIG) == 0)
            {
                //check arg max
                if (argc > 1)
                {
                    printf(MSG_WARN_ARG_MAX);
                }

                gm_cmd_show_config(data->cfg);
            }
            else if (strcmp(argv[0], GM_CMD_SET) == 0)
            {
                //check arg min
                if (argc < 3)
                {
                    printf(MSG_ERR_ARG_MIN);
                    break;
                }

                //check arg max
                else if (argc > 3)
                {
                    printf(MSG_WARN_ARG_MAX);
                }

                gm_cmd_set(data->cfg, argv[1], argv[2]);
            }
            else
            {
                printf(MSG_ERR_UNKNOWN_COMMAND);
            }
        }
    }

    return 0;
} 
