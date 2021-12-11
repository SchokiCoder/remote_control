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
#include <GL/glu.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"
#include "town.h"
#include "game.h"

int32_t gfx_game(void* p_data)
{
    bool active = true;
    SDL_Window* window;
    SDL_GLContext* glc;
    GLenum gl_err = GL_NO_ERROR;
    SDL_Event event;
    struct GameData* data = (struct GameData*) (p_data);
    float area_scale_x, area_scale_y;
    float field_size_x, field_size_y;
    struct Vertex verts[TOWN_WIDTH][TOWN_DEPTH];
    struct Color colrs[TOWN_WIDTH][TOWN_DEPTH];

    //init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf(MSG_ERR_SDL_INIT, SDL_GetError());
        return 1;
    }

    //configure OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    //create window
    window = SDL_CreateWindow(
        data->window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));

    if (window == NULL)
    {
        printf(MSG_ERR_SDL_WINDOW, SDL_GetError());
        return 2;
    }

    //create gl context
    glc = SDL_GL_CreateContext(window);

    if (glc == NULL)
    {
        printf(MSG_ERR_GL_CONTEXT, SDL_GetError());
        return 3;
    }

    //init GL projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gl_err = glGetError();

    if (gl_err != GL_NO_ERROR)
    {
        printf(MSG_ERR_GL_PROJECTION_MATRIX, gluErrorString(gl_err));
        return 4;
    }

    //init GL modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gl_err = glGetError();

    if (gl_err != GL_NO_ERROR)
    {
        printf(MSG_ERR_GL_MODELVIEW_MATRIX, gluErrorString(gl_err));
        return 5;
    }

    //set gl clear color
    glClearColor(COLOR_BG_RED, COLOR_BG_GREEN, COLOR_BG_BLUE, 1.0f);

    gl_err = glGetError();

    if (gl_err != GL_NO_ERROR)
    {
        printf(MSG_ERR_GL_CLEAR_COLOR, gluErrorString(gl_err));
        return 6;
    }

    //set perspective
    //glRotatef(45.0f, 1.0f, 0.0f, 1.0f);

    //calculate area scale
    if (WINDOW_WIDTH == WINDOW_HEIGHT)
    {
        area_scale_x = 1.0f;
        area_scale_y = 1.0f;
    }
    else if (WINDOW_WIDTH / WINDOW_HEIGHT > 1.0f)
    {
        area_scale_x = WINDOW_HEIGHT / WINDOW_WIDTH;
        area_scale_y = 1.0f;
    }
    else
    {
        area_scale_x = 1.0f;
        area_scale_y = WINDOW_WIDTH / WINDOW_HEIGHT;
    }

    //calculate field size
    field_size_x = area_scale_x / (float) TOWN_WIDTH;
    field_size_y = area_scale_y / (float) TOWN_DEPTH;
    
    //calculate vertices and colors
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_DEPTH; y++)
        {
            verts[x][y].x1 = (area_scale_x / 2.0f) + (float) x * -1.0f * field_size_x;
            verts[x][y].x2 = verts[x][y].x1 + field_size_x;
            verts[x][y].y1 = (area_scale_y / 2.0f) + (float) y * -1.0f * field_size_y;
            verts[x][y].y2 = verts[x][y].y1 + field_size_y;

            if (data->town->area_hidden[x][y] == true)
            {
                colrs[x][y].r = 0;
                colrs[x][y].g = 0;
                colrs[x][y].b = 0;
            }
            else
            {
                colrs[x][y].r = 100;
                colrs[x][y].g = 255;
                colrs[x][y].b = 100;
            }
        }
    }

    //mainloop
    while (active)
    {
        //draw to window
        {
            glClear(GL_COLOR_BUFFER_BIT);

            /*glBegin(GL_QUADS);
            
            glVertex2f(area_scale_x * -1.0f, area_scale_y * -1.0f);
            glVertex2f(area_scale_x * -1.0f, area_scale_y);
            glVertex2f(area_scale_x, area_scale_y);
            glVertex2f(area_scale_x, area_scale_y * -1.0f);

            glEnd();*/

            for (uint32_t x = 0; x < TOWN_WIDTH; x++)
            {
                for (uint32_t y = 0; y < TOWN_DEPTH; y++)
                {
                    glBegin(GL_QUADS);
                    
                    glColor3ub(colrs[x][y].r, colrs[x][y].g, colrs[x][y].b);
                    glVertex2f(verts[x][y].x1, verts[x][y].y1);
                    glVertex2f(verts[x][y].x2, verts[x][y].y1);
                    glVertex2f(verts[x][y].x2, verts[x][y].y2);
                    glVertex2f(verts[x][y].x1, verts[x][y].y2);

                    glEnd();
                }
            }

            SDL_GL_SwapWindow(window);
        }
        
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

    //destroy window
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
