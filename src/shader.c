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
#include <stdlib.h>
#include "constants.h"
#include "shader.h"

uint32_t read_file(const char* p_filename, char* p_out[])
{
    FILE* f;
    size_t filesize;

    //open file
    f = fopen(p_filename, "r");

    if (f == NULL)
    {
        printf(MSG_ERR_FILE_READ, p_filename);
        return 1;
    }

    //get file size
    fseek(f, 0, SEEK_END);
    filesize = ftell(f);
    rewind(f);

    //alloc mem and read
    *p_out = (char*) malloc(filesize);
    fread(*p_out, sizeof(char), filesize, f);

    fclose(f);
    return 0;
}

GLuint compile_shader(const char* p_shader_source, GLenum p_type)
{
    GLuint id;
    int32_t rc_c;

    //compile
    id = glCreateShader(p_type);
    glShaderSource(id, 1, &p_shader_source, 0);
    glCompileShader(id);

    //check compilation
    glGetShaderiv(id, GL_COMPILE_STATUS, &rc_c);

    if (rc_c != GL_TRUE)
    {
        printf(MSG_ERR_SHADER_COMPILE);
        return 0;
    }

    return id;
}

void Shader_new(struct Shader* self, const char* p_vert_shader_filename, const char* p_frag_shader_filename)
{
    char* vert_shader_source;
    char* frag_shader_source;
    GLuint vert_shader;
    GLuint frag_shader;

    //read shader files
    read_file(p_vert_shader_filename, &vert_shader_source);
    read_file(p_frag_shader_filename, &frag_shader_source);

    //compile shader source
    self->shader_id = glCreateProgram();
    vert_shader = compile_shader(vert_shader_source, GL_VERTEX_SHADER);
    frag_shader = compile_shader(frag_shader_source, GL_FRAGMENT_SHADER);

    //link shader
    glAttachShader(self->shader_id, vert_shader);
    glAttachShader(self->shader_id, frag_shader);
    glLinkProgram(self->shader_id);

    //delete shaders
    glDetachShader(self->shader_id, vert_shader);
    glDetachShader(self->shader_id, frag_shader);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

void Shader_destroy(struct Shader* self)
{
    glDeleteProgram(self->shader_id);
}

void Shader_bind(struct Shader* self)
{
    glUseProgram(self->shader_id);
}

void Shader_unbind()
{
    glUseProgram(0);
}
