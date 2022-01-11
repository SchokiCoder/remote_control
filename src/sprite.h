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

#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <SDL2/SDL.h>

struct Sprite
{
    SDL_Surface* surface;
    SDL_Texture* texture;
};

int32_t Sprite_init(struct Sprite* self, const char* p_filepath, SDL_Renderer* p_renderer);
void Sprite_clear(struct Sprite* self);

#endif
