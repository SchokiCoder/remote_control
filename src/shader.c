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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "draw.h"
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
    char* gl_err;
    int32_t gl_err_length;

    //compile
    id = glCreateShader(p_type);
    glShaderSource(id, 1, &p_shader_source, 0);
    glCompileShader(id);

    //check compilation
    glGetShaderiv(id, GL_COMPILE_STATUS, &rc_c);

    if (rc_c != GL_TRUE)
    {
        //get gl error
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &gl_err_length);
        gl_err = malloc(gl_err_length * sizeof(char));
        glGetShaderInfoLog(id, gl_err_length, &gl_err_length, gl_err);

        //print
        printf(MSG_ERR_SHADER_COMPILE, gl_err);

        free(gl_err);
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
    self->id = glCreateProgram();
    vert_shader = compile_shader(vert_shader_source, GL_VERTEX_SHADER);
    frag_shader = compile_shader(frag_shader_source, GL_FRAGMENT_SHADER);

    //check for errors
    if (vert_shader == 0 || frag_shader == 0)
    {
        printf(MSG_ERR_SHADER_COMPILE_FILES,
            p_vert_shader_filename, vert_shader,
            p_frag_shader_filename, frag_shader);
    }

    //link shader
    glAttachShader(self->id, vert_shader);
    glAttachShader(self->id, frag_shader);
    glLinkProgram(self->id);

    //delete shaders
    glDetachShader(self->id, vert_shader);
    glDetachShader(self->id, frag_shader);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

void Shader_destroy(struct Shader* self)
{
    glDeleteProgram(self->id);
}

void Shader_bind(struct Shader* self)
{
    glUseProgram(self->id);
}

void Shader_unbind()
{
    glUseProgram(0);
}

void Shader_prepare_texture_slot(struct Shader* self, int32_t p_texture_slot)
{
    Shader_bind(self);

    self->texture_uniform_location = glGetUniformLocation(self->id, "u_texture");

    if (self->texture_uniform_location != -1)
    {
        glUniform1i(self->texture_uniform_location, p_texture_slot);
    }

    Shader_unbind();
}

void Shader_bind_texture(int32_t p_texture_slot, struct Texture* p_texture)
{
    switch(p_texture_slot)
    {
    case 0:
        glActiveTexture(GL_TEXTURE0);
    break;

    case 1:
        glActiveTexture(GL_TEXTURE1);
    break;

    case 2:
        glActiveTexture(GL_TEXTURE2);
    break;

    case 4:
        glActiveTexture(GL_TEXTURE3);
    break;

    glBindTexture(GL_TEXTURE_2D, p_texture->id);
    }
}
