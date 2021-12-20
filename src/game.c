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
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"
#include "town.h"
#include "game.h"

void draw_triangle(struct Vertex *p_a, struct Vertex *p_b, struct Vertex *p_c, struct Color *p_color)
{
    glColor3f(p_color->r, p_color->g, p_color->b);
    
    glBegin(GL_TRIANGLE_STRIP);

    glVertex3f(p_a->x, p_a->y, p_a->z);
    glVertex3f(p_b->x, p_b->y, p_c->z);
    glVertex3f(p_c->x, p_c->y, p_c->z);

    glEnd();
}

void draw_face(struct Face *p_face, struct Color *p_color)
{
    draw_triangle(&p_face->a, &p_face->b, &p_face->c, p_color);
    draw_triangle(&p_face->b, &p_face->c, &p_face->d, p_color);
}

int32_t gfx_game(void* p_data)
{
    bool active = true;
    SDL_Window* window;
    SDL_GLContext* glc;
    GLenum gl_err = GL_NO_ERROR;
    GLuint vert_buf;
    SDL_Event event;
    struct GameData* data = (struct GameData*) (p_data);
    float area_scale_x, area_scale_y;
    float field_size_x, field_size_y;
    struct Face faces[TOWN_WIDTH][TOWN_DEPTH];
    struct Color colrs[TOWN_WIDTH][TOWN_DEPTH];

    //init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf(MSG_ERR_SDL_INIT, SDL_GetError());
        return 1;
    }

    //configure OpenGL
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 16);
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
    /*glMatrixMode(GL_PROJECTION);
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
    }*/

    //set gl clear color
    glClearColor(COLOR_BG_RED, COLOR_BG_GREEN, COLOR_BG_BLUE, 1.0f);

    /*gl_err = glGetError();

    if (gl_err != GL_NO_ERROR)
    {
        printf(MSG_ERR_GL_CLEAR_COLOR, gluErrorString(gl_err));
        return 6;
    }*/

    //init glew
    gl_err = glewInit();

    if (gl_err != GLEW_OK)
    {
        printf(MSG_ERR_GLEW_INIT, glewGetErrorString(gl_err));
        return 7;
    }
    
    //create vertex buffer
    glGenBuffers(1, &vert_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf);

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
            faces[x][y].a.x = (area_scale_x / 2.0f) + (float) x * -1.0f * field_size_x;
            faces[x][y].a.y = (area_scale_y / 2.0f) + (float) y * -1.0f * field_size_y;
            faces[x][y].a.z = 0.0f;
            faces[x][y].b.x = faces[x][y].a.x + field_size_x;
            faces[x][y].b.y = faces[x][y].a.y;
            faces[x][y].b.z = 0.0f;
            faces[x][y].c.x = faces[x][y].a.x;
            faces[x][y].c.y = faces[x][y].a.y + field_size_y;
            faces[x][y].c.z = 0.0f;
            faces[x][y].d.x = faces[x][y].a.x + field_size_x;
            faces[x][y].d.y = faces[x][y].a.y + field_size_y;
            faces[x][y].d.z = 0.0f;

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

    //send vertices into vert buffer
    glBufferData(GL_ARRAY_BUFFER, TOWN_WIDTH * TOWN_DEPTH * sizeof(struct Face), faces, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), offsetof(struct Face, a));

    //mainloop
    while (active)
    {
        //draw to window
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawArrays(GL_TRIANGLES, 0, TOWN_WIDTH * TOWN_DEPTH);
            /*for (uint32_t x = 0; x < TOWN_WIDTH; x++)
            {
                for (uint32_t y = 0; y < TOWN_DEPTH; y++)
                {
                    
                }
            }*/

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
