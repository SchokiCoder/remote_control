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

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

struct Texture;

struct Shader
{
    GLuint id;
    int32_t texture_uniform_location;

};

void Shader_new(struct Shader* self, const char* p_vert_shader_filename, const char* p_frag_shader_filename);

void Shader_destroy(struct Shader* self);

void Shader_bind(struct Shader* self);

void Shader_unbind();

void Shader_prepare_texture_slot(struct Shader* self, int32_t p_texture_slot);

void Shader_bind_texture(int32_t p_texture_slot, struct Texture* p_texture);

#endif
