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

#include "constants.h"
#include "draw.h"

void VertexBuffer_new(struct VertexBuffer* self, void* p_data, uint32_t p_num_verts)
{
    self->num_verts = p_num_verts;
    
    //create vartex array object and vert buffer
    glGenVertexArrays(1, &self->vao);
    VertexBuffer_bind(self);
    
    glGenBuffers(1, &self->id);
    glBindBuffer(GL_ARRAY_BUFFER, self->id);
    glBufferData(GL_ARRAY_BUFFER, self->num_verts * sizeof(struct Vertex), p_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), offsetof(struct Vertex, x));

    //unbind new vao
    VertexBuffer_unbind(self);
}

void VertexBuffer_delete(struct VertexBuffer* self)
{
    glDeleteBuffers(1, &self->id);
}

void VertexBuffer_bind(struct VertexBuffer* self)
{
    glBindVertexArray(self->vao);
}

void VertexBuffer_unbind(struct VertexBuffer* self)
{
    glBindVertexArray(0);
}

int32_t Texture_load(struct Texture* self, const char* p_filepath)
{
    //load texure
    self->buffer = stbi_load(p_filepath, &self->width, &self->height, &self->colordepth, 4);

    if (self->buffer == NULL)
    {
        printf(MSG_ERR_TEXTURE_LOAD, p_filepath);
        return 1;
    }

    //gl create texture
    glGenTextures(1, &self->id);
    glBindTexture(GL_TEXTURE_2D, self->id);

    //set texture attributes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //apply and unbind
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, self->width, self->height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, self->buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

void Texture_free(struct Texture* self)
{
    stbi_image_free(self->buffer);
    glDeleteTextures(1, &self->id);
}
