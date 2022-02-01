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

#include <string.h>
#include "widget.h"

void Widget_new(struct Widget *self)
{
	strcpy(self->text, "");
	Sprite_new(&self->sprite);
}

int32_t Widget_generate_sprite(
	struct Widget *self,
	SDL_Renderer *p_renderer,
	TTF_Font *p_font,
	SDL_Color p_font_color)
{
	int32_t rc_gen = 0;

	/* gen sprite */
	rc_gen = Sprite_from_text(&self->sprite, p_renderer, self->text, p_font, p_font_color);

	/* if successful, update width and height of widget */
	if (rc_gen == 0)
	{
		self->rect.w = self->sprite.surface->w;
		self->rect.h = self->sprite.surface->h;
	}
	else
	{
		return rc_gen;
	}

	return 0;
}

void Widget_draw(struct Widget *self, SDL_Renderer *p_renderer)
{
	SDL_RenderCopy(p_renderer, self->sprite.texture, NULL, &self->rect);
}

void Widget_clear(struct Widget *self)
{
	Sprite_clear(&self->sprite);
}
