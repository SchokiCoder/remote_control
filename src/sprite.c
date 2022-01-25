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

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "constants.h"
#include "sprite.h"

void Sprite_new(struct Sprite *self)
{
	self->surface = NULL;
	self->texture = NULL;
}

int32_t Sprite_from_image(struct Sprite *self, SDL_Renderer *p_renderer, const char *p_filepath)
{
	//clear
	Sprite_clear(self);
	
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

int32_t Sprite_from_text(
	struct Sprite *self,
	SDL_Renderer *p_renderer,
	const char *p_text,
	TTF_Font *p_font,
	SDL_Color p_color)
{
	//clear
	Sprite_clear(self);
	
	//create text
	self->surface = TTF_RenderText_Solid(p_font, p_text, p_color);
	
	if (self->surface == NULL)
	{
		printf(MSG_ERR_TEXT_CREATE, p_text);
		return 1;
	}
	
	//create texture
	self->texture = SDL_CreateTextureFromSurface(p_renderer, self->surface);

	if (self->texture == NULL)
	{
		printf(MSG_ERR_TEXTURE_CREATE, p_text);
		return 2;
	}

	return 0;
}

void Sprite_clear(struct Sprite *self)
{
	if (self->surface != NULL)
	{
		SDL_FreeSurface(self->surface);
		self->surface = NULL;
	}

	if (self->texture != NULL)
	{
		SDL_DestroyTexture(self->texture);
		self->texture = NULL;
	}
}
