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
#include "draw.h"
#include "shader.h"
#include "game.h"

static const uint32_t TOWN_NUM_VERTS = FIELD_NUM_VERTS * TOWN_WIDTH * TOWN_DEPTH;

#ifdef _DEBUG
void opengl_debug_callback(
    GLenum p_source,
    GLenum p_type,
    GLuint p_id,
    GLenum p_severity,
    GLsizei p_length,
    const GLchar* p_message,
    const void* p_param)
{
    if (p_severity == GL_DEBUG_SEVERITY_HIGH || p_severity == GL_DEBUG_SEVERITY_MEDIUM)
        printf("OpenGL Debug (%u): %s\n", p_severity, p_message);
}
#endif

int32_t gfx_game(void* p_data)
{
    bool active = true;
    SDL_Window* window;
    SDL_GLContext* glc;
    GLenum gl_err = GL_NO_ERROR;
    struct VertexBuffer vtb_terrain;
    SDL_Event event;
    struct GameData* data = (struct GameData*) (p_data);
    float area_size_x, area_size_y;
    float field_size_x, field_size_y;
    struct Vertex base;
    struct Rectangle faces[TOWN_WIDTH][TOWN_DEPTH];
    uint32_t vert_offset;

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

    #ifdef _DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    #endif

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

    //enable gl debug output
    #ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, NULL);
    #endif

    //set gl clear color
    glClearColor(COLOR_BG_RED, COLOR_BG_GREEN, COLOR_BG_BLUE, 1.0f);

    //calculate area scale
    if (WINDOW_WIDTH == WINDOW_HEIGHT)
    {
        area_size_x = 1.0f;
        area_size_y = 1.0f;
    }
    else if (WINDOW_WIDTH / WINDOW_HEIGHT > 1.0f)
    {
        area_size_x = WINDOW_HEIGHT / WINDOW_WIDTH;
        area_size_y = 1.0f;
    }
    else
    {
        area_size_x = 1.0f;
        area_size_y = WINDOW_WIDTH / WINDOW_HEIGHT;
    }

    //calculate field size
    field_size_x = area_size_x / (float) TOWN_WIDTH;
    field_size_y = area_size_y / (float) TOWN_DEPTH;
    
    //calculate base vertex
    base.x = (-1.0f + (area_size_x / 2.0f));
    base.y = (-1.0f + (area_size_x / 2.0f));
    base.z = 0.0f;

    //set terrain vertices from base vertex
    for (uint32_t x = 0; x < TOWN_WIDTH; x++)
    {
        for (uint32_t y = 0; y < TOWN_DEPTH; y++)
        {
            faces[x][y].a.x = base.x + (field_size_x * x);
            faces[x][y].a.y = base.y + (field_size_y * y);
            faces[x][y].a.z = base.z;

            faces[x][y].b.x = faces[x][y].a.x + field_size_x;
            faces[x][y].b.y = faces[x][y].a.y;
            faces[x][y].b.z = faces[x][y].a.z;

            faces[x][y].c.x = faces[x][y].a.x;
            faces[x][y].c.y = faces[x][y].a.y + field_size_y;
            faces[x][y].c.z = faces[x][y].a.z;

            faces[x][y].d.x = faces[x][y].a.x + field_size_x;
            faces[x][y].d.y = faces[x][y].a.y + field_size_y;
            faces[x][y].d.z = faces[x][y].a.z;
       }
    }

    //create vertex buffer
    VertexBuffer_new(&vtb_terrain, &faces, TOWN_NUM_VERTS);

    //bind vert buf
    VertexBuffer_bind(&vtb_terrain);

    //load shader
    struct Shader shdr_hidden;
    struct Shader shdr_exposed;

    Shader_new(&shdr_hidden, PATH_VERT_SHADER, PATH_FRAG_SHADER_HIDDEN);
    Shader_new(&shdr_exposed, PATH_VERT_SHADER, PATH_FRAG_SHADER_EXPOSED);

    //debug wireframe mode
    #ifdef _DEBUG
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif

    //load textures
    struct Texture buildings[2];

    stbi_set_flip_vertically_on_load(true);

    if (Texture_load(&buildings[0], PATH_TEXTURE_HQ) != 0 ||
        Texture_load(&buildings[1], PATH_TEXTURE_TREE_0) != 0)
        return 666; //TODO resort return codes

    //mainloop
    while (active)
    {
        //draw to window
        glClear(GL_COLOR_BUFFER_BIT);
        vert_offset = 0;

        for (uint32_t x = 0; x < TOWN_WIDTH; x++)
        {
            for (uint32_t y = 0; y < TOWN_DEPTH; y++)
            {
                if (data->town->area_hidden[x][y] == true)
                {
                    Shader_bind(&shdr_hidden);
                }
                else
                {
                    Shader_bind(&shdr_exposed);
                }

                glDrawArrays(GL_TRIANGLE_STRIP, vert_offset, FIELD_NUM_VERTS);

                vert_offset += FIELD_NUM_VERTS;
            }
        }

        SDL_GL_SwapWindow(window);
        
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
