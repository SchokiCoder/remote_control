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
#include <SDL_ttf.h>
#include <SGUI_menu.h>
#include <SGUI_entry.h>
#include <SGUI_label.h>
#include "town.h"

typedef struct Game Game;
typedef struct Config Config;

static const char PATH_TEXTURE_GROUND[] =		PATH_TEXTURES "ground.png";
static const char PATH_TEXTURE_HIDDEN[] =		PATH_TEXTURES "hidden.png";
static const char PATH_TEXTURE_MERC_BASE[] =	PATH_TEXTURES "mercenary_base.png";
static const char PATH_TEXTURE_MERC_GREEN[] =	PATH_TEXTURES "mercenary_green_tint.png";
static const char PATH_TEXTURE_MERC_PURPLE[] =	PATH_TEXTURES "mercenary_purple_tint.png";

static const char *PATH_TEXTURE_MERCENARIES[] = {
	PATH_TEXTURES "mercenary_soldier.png",
	PATH_TEXTURES "mercenary_pyro.png",
	PATH_TEXTURES "mercenary_anchor.png",
	PATH_TEXTURES "mercenary_medic.png"
};
#define MERCENARY_SPRITE_COUNT sizeof(PATH_TEXTURE_MERCENARIES) / sizeof(PATH_TEXTURE_MERCENARIES[0])

static const char *PATH_TEXTURE_FIELDS[] = {
	PATH_TEXTURES "tree_0.png",
	PATH_TEXTURES "tree_1.png",
	PATH_TEXTURES "tree_2.png",
	PATH_TEXTURES "tree_3.png",
	PATH_TEXTURES "tree_4.png",
	PATH_TEXTURES "headquarter.png",
	PATH_TEXTURES "construction.png",
	PATH_TEXTURES "quarry.png"
};
#define FIELD_SPRITE_OFFSET 2
#define FIELD_SPRITE_COUNT (sizeof(PATH_TEXTURE_FIELDS) / sizeof(PATH_TEXTURE_FIELDS[0]))

typedef struct Hud
{
	SDL_Renderer *renderer;

	// data
	bool invalid;
	SDL_Point hover_field;

	/* bars */
	SDL_Rect rect_bar_top;
	SDL_Rect rect_bar_cmd;

	/* hud widgets */
	SGUI_Menu mnu_hud;
	SGUI_Label lbl_hover_x;
	SGUI_Label lbl_hover_y;
	SGUI_Label lbl_hover_name;
	SGUI_Label lbl_time_day;
	SGUI_Label lbl_time_day_val;
	SGUI_Label lbl_time_hour;
	SGUI_Label lbl_time_hour_val;
	SGUI_Label lbl_money;
	SGUI_Label lbl_money_val;
	SGUI_Label lbl_feedback;
	SGUI_Label lbl_pointer;
	SGUI_Entry txt_command;

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
	SGUI_Sprite spr_ground;
	SGUI_Sprite spr_hidden;

	SGUI_Sprite spr_merc_base;
	SGUI_Sprite spr_merc_tint_green;
	SGUI_Sprite spr_merc_tint_purple;
	SGUI_Sprite spr_mercs[MERCENARY_SPRITE_COUNT];

	SGUI_Sprite spr_fields[FIELD_SPRITE_COUNT + FIELD_SPRITE_OFFSET];
} Hud ;

void Hud_new( Hud *hud, const SDL_Renderer *renderer, const Config *cfg );

void Hud_update_hover( Hud *hud, const SDL_Point coord, const char *name );

void Hud_update_time( Hud *hud, const uint32_t round );

void Hud_update_money( Hud *hud, const uint32_t money );

void Hud_update_feedback( Hud *hud, const char *str );

void Hud_calc( Hud *hud, const int32_t window_w, const int32_t window_h );

void Hud_generate_flips( Hud *hud );

void Hud_map_textures(
	Hud *hud,
	const bool fields_hidden[TOWN_WIDTH][TOWN_HEIGHT],
	const Field fields_content[TOWN_WIDTH][TOWN_HEIGHT] );

void Hud_draw( Hud *hud, const Town *town );

void Hud_set_field( Hud *hud, const SDL_Point field, const SDL_Texture *texture );

void Hud_clear( Hud *hud );

#endif /* HUD_H */
