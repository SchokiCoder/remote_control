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

#ifndef WIDGET_H
#define WIDGET_H

#include <SDL.h>
#include "definitions/def_hud.h"
#include "sprite.h"

struct Widget
{
	char text[HUD_WIDGET_TEXT_MAX_LEN];
	SDL_Rect rect;
	struct Sprite sprite;
};

void Widget_new(struct Widget *self);

int32_t Widget_generate_sprite(
	struct Widget *self,
	SDL_Renderer *p_renderer,
	TTF_Font *p_font,
	SDL_Color p_font_color);

void Widget_draw(struct Widget *self, SDL_Renderer *p_renderer);

void Widget_clear(struct Widget *self);

#endif /* WIDGET_H */
