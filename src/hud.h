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

#ifndef HUD_H
#define HUD_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "constants.h"
#include "widget.h"
#include "sprite.h"
#include "town.h"

struct Hud
{
	/* widgets */
	struct Widget time_day_label;
	struct Widget time_day;
	struct Widget time_hour_label;
	struct Widget time_hour;
	struct Widget money_label;
	struct Widget money;
	struct Widget hover_label;
	struct Widget hover_x;
	struct Widget hover_y;

	/* graphical data for area and fields */
	uint32_t field_width;
	uint32_t field_height;
	SDL_Rect rect_area;
	SDL_Rect rects_field[TOWN_WIDTH][TOWN_HEIGHT];
	SDL_Rect rects_field_content[TOWN_WIDTH][TOWN_HEIGHT];
	SDL_Texture *textures_field_ground[TOWN_WIDTH][TOWN_HEIGHT];
	SDL_Texture *textures_field_content[TOWN_WIDTH][TOWN_HEIGHT];
	SDL_RendererFlip flips_field[TOWN_WIDTH][TOWN_HEIGHT];

	/* shared sprites */
	struct Sprite spr_ground;
	struct Sprite spr_hidden;
	struct Sprite spr_hq;
	struct Sprite spr_trees[TOWN_TREE_VARIETY_COUNT];
};

void Hud_new(struct Hud *self);

void Hud_update_time(
	struct Hud *self,
	uint32_t p_round);

void Hud_update_money(
	struct Hud *self,
	uint32_t p_money,
	SDL_Renderer *p_renderer,
	TTF_Font *p_font,
	SDL_Color p_font_color);

void Hud_update_hover(
	struct Hud *self,
	SDL_Renderer *p_renderer,
	TTF_Font *p_font,
	SDL_Color p_font_color);

int32_t Hud_generate_widget_sprites(
	struct Hud *self,
	SDL_Renderer *p_renderer,
	TTF_Font *p_font,
	SDL_Color p_font_color);

int32_t Hud_load_sprites(struct Hud *self, SDL_Renderer *p_renderer);

void Hud_calc(struct Hud *self,	int32_t p_window_w,	int32_t p_window_h);

void Hud_generate_flips(struct Hud *self);

void Hud_map_textures(
	struct Hud *self,
	bool p_fields_hidden[TOWN_WIDTH][TOWN_HEIGHT],
	enum Field p_fields_content[TOWN_WIDTH][TOWN_HEIGHT]);

void Hud_draw(
	struct Hud *self,
	SDL_Renderer *p_renderer,
	uint8_t p_field_border_red,
	uint8_t p_field_border_green,
	uint8_t p_field_border_blue,
	uint8_t p_field_border_alpha);

void Hud_clear(struct Hud *self);

#endif /* HUD_H */
