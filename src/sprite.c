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

#include <SDL2/SDL_image.h>
#include "constants.h"
#include "sprite.h"

int32_t Sprite_init(struct Sprite* self, const char* p_filepath, SDL_Renderer* p_renderer)
{
    //load image
    self->surface = IMG_Load(p_filepath);

    if (self->surface == NULL)
    {
        printf(MSG_ERR_IMAGE_LOAD, p_filepath);
        return 1;
    }

    //create texture
    self->texture = SDL_CreateTextureFromSurface(p_renderer, self->surface);

    if (self->texture == NULL)
    {
        printf(MSG_ERR_TEXTURE_CREATE, p_filepath);
        return 2;
    }

    return 0;
}

void Sprite_clear(struct Sprite* self)
{
    SDL_FreeSurface(self->surface);
    SDL_DestroyTexture(self->texture);
}
