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

#include "draw.h"

void VertexBuffer_new(struct VertexBuffer* self, void* p_data, uint32_t p_num_verts)
{
    self->num_verts = p_num_verts;
    
    //create vartex array object and vert buffer
    glGenVertexArrays(1, &self->vao);
    VertexBuffer_bind(self);
    
    glGenBuffers(1, &self->buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, self->buffer_id);
    glBufferData(GL_ARRAY_BUFFER, self->num_verts * sizeof(struct Vertex), p_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), offsetof(struct Vertex, x));

    //unbind new vao
    VertexBuffer_unbind(self);
}

void VertexBuffer_delete(struct VertexBuffer* self)
{
    glDeleteBuffers(1, &self->buffer_id);
}

void VertexBuffer_bind(struct VertexBuffer* self)
{
    glBindVertexArray(self->vao);
}

void VertexBuffer_unbind(struct VertexBuffer* self)
{
    glBindVertexArray(0);
}
