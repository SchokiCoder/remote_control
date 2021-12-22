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

#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include <GL/glew.h>
#define STBI_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb/stb_image.h>

struct Vertex
{
    float x;
    float y;
    float z;
};

struct Triangle
{
    struct Vertex a;
    struct Vertex b;
    struct Vertex c;
};

struct Rectangle
{
    struct Vertex a;
    struct Vertex b;
    struct Vertex c;
    struct Vertex d;
};

static const uint32_t FIELD_NUM_VERTS = sizeof(struct Rectangle) / sizeof(struct Vertex);

struct VertexBuffer
{
    GLuint vao;         /* vertex array object */
    GLuint buffer_id;
    uint32_t num_verts;
};

void VertexBuffer_new(struct VertexBuffer* self, void* p_data, uint32_t p_num_verts);
void VertexBuffer_delete(struct VertexBuffer* self);
void VertexBuffer_bind(struct VertexBuffer* self);
void VertexBuffer_unbind(struct VertexBuffer* self);

struct Texture
{
    int32_t width;
    int32_t height;
    int32_t colordepth;
    stbi_uc* buffer;
};

int32_t Texture_load(struct Texture* self, const char* p_filepath);
void Texture_free(struct Texture* self);

#endif
