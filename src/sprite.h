/*
	remote_control
	Copyright (C) 2021	Andy Frank Schoknecht

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
#include <SDL.h>

struct TTF_Font;

struct Sprite
{
	SDL_Surface* surface;
	SDL_Texture* texture;
};

void Sprite_new(struct Sprite *self);

int32_t Sprite_from_image(struct Sprite *self, SDL_Renderer *p_renderer, const char *p_filepath);

int32_t Sprite_from_text(
	struct Sprite *self,
	SDL_Renderer *p_renderer,
	const char *p_text,
	TTF_Font *p_font,
	SDL_Color p_color);

void Sprite_clear(struct Sprite* self);

#endif
