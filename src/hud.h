#/*
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
#include <SDL_ttf.h>
#include "definitions/def_gameplay.h"
#include "widget.h"
#include "sprite.h"
#include "town.h"

struct GameData;

struct Hud
{
	/* font */
	TTF_Font *font;
	SDL_Color font_color;

	/* widgets */
	struct Widget lbl_time_day;
	struct Widget lbl_time_day_val;
	struct Widget lbl_time_hour;
	struct Widget lbl_time_hour_val;
	struct Widget lbl_money;
	struct Widget lbl_money_val;
	struct Widget lbl_hover;
	struct Widget lbl_hover_x_val;
	struct Widget lbl_hover_y_val;
	struct Widget btn_pass;

	/* graphical data for area and fields */
	SDL_Color field_border_color;
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

int32_t Hud_new(struct Hud *self, char *p_path_font);

void Hud_update_time(
	struct Hud *self,
	uint32_t p_round,
	SDL_Renderer *p_renderer);

void Hud_update_money(
	struct Hud *self,
	uint32_t p_money,
	SDL_Renderer *p_renderer);

int32_t Hud_init_widgets(
	struct Hud *self,
	SDL_Renderer *p_renderer);

int32_t Hud_load_sprites(struct Hud *self, SDL_Renderer *p_renderer);

void Hud_calc(struct Hud *self,	int32_t p_window_w,	int32_t p_window_h);

void Hud_generate_flips(struct Hud *self);

void Hud_map_textures(
	struct Hud *self,
	bool p_fields_hidden[TOWN_WIDTH][TOWN_HEIGHT],
	enum Field p_fields_content[TOWN_WIDTH][TOWN_HEIGHT]);

void Hud_draw(struct Hud *self, SDL_Renderer *p_renderer);

void Hud_update_hover(struct Hud *self, SDL_Renderer *p_renderer);

void Hud_clear(struct Hud *self);

#endif /* HUD_H */
