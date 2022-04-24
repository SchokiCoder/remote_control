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
#include <SGUI_theme.h>
#include "messages.h"
#include "game.h"
#include "config.h"
#include "hud.h"

static const uint_fast8_t HUD_FONT_SIZE = 14;

// area, fields
static const float HUD_AREA_X2 =	0.9f;
static const float HUD_AREA_Y =		0.1f;
static const float HUD_AREA_H =		0.8f;

static const float HUD_FIELD_CONTENT_SIZE =	0.95f;

// bars
static const uint8_t HUD_BAR_COLOR_R = 20;
static const uint8_t HUD_BAR_COLOR_G = 20;
static const uint8_t HUD_BAR_COLOR_B = 20;
static const uint8_t HUD_BAR_COLOR_A = 200;

static const float HUD_BAR_TOP_X = 0.0f;
static const float HUD_BAR_TOP_Y = 0.0f;
static const float HUD_BAR_TOP_W = 1.0f;
static const float HUD_BAR_TOP_H_MARGIN = 0.001f;

static const float HUD_BAR_CMD_W = 1.0f;
static const float HUD_BAR_CMD_H_MARGIN = 0.001;

// hover labels
static const float HUD_LBL_HOVER_X_X = 0.01f;
static const float HUD_LBL_HOVER_X_Y_DIST = 0.005f;

static const float HUD_LBL_HOVER_Y_X_DIST = 0.04f;

static const float HUD_LBL_HOVER_NAME_X_DIST = 0.04f;

// top bar widgets
static const char HUD_LBL_TIME_DAY_TEXT[] =		"day:";
static const char HUD_LBL_TIME_HOUR_TEXT[] =	"at:";
static const char HUD_LBL_MONEY_TEXT[] =		"money:";

static const float HUD_LBL_TIME_DAY_X =				0.005f;
static const float HUD_LBL_TIME_DAY_Y =				HUD_BAR_TOP_H_MARGIN;

static const float HUD_LBL_TIME_DAY_VAL_X_DIST =	0.01f;
static const float HUD_LBL_TIME_DAY_VAL_Y =			HUD_LBL_TIME_DAY_Y;

static const float HUD_LBL_TIME_HOUR_X_DIST =		0.03f;
static const float HUD_LBL_TIME_HOUR_Y =			HUD_BAR_TOP_H_MARGIN;

static const float HUD_LBL_TIME_HOUR_VAL_X_DIST =	HUD_LBL_TIME_DAY_VAL_X_DIST;
static const float HUD_LBL_TIME_HOUR_VAL_Y =		HUD_LBL_TIME_HOUR_Y;

static const float HUD_LBL_MONEY_X_DIST 	=		0.05f;
static const float HUD_LBL_MONEY_Y =				HUD_BAR_TOP_H_MARGIN;

static const float HUD_LBL_MONEY_VAL_X_DIST =		HUD_LBL_TIME_DAY_VAL_X_DIST;
static const float HUD_LBL_MONEY_VAL_Y =			HUD_LBL_MONEY_Y;

static const SGUI_Theme THEME_RC = {
	.menu = {
		.bg_color = {.r = 0, .g = 0, .b = 0, .a = 0},
	},

	.label = {
		.font_color = {.r = 200, .g = 200, .b = 200, .a = 255},
    	.bg_color = {.r = 0, .g = 0, .b = 0, .a = 0},
    	.border_color = {.r = 255, .g = 255, .b = 255, .a = 0},
	},

    .button = {
    	.font_color = {.r = 200, .g = 200, .b = 200, .a = 255},
    	.bg_color = {.r = 50, .g = 50, .b = 50, .a = 255},
    	.border_color = {.r = 70, .g = 70, .b = 70, .a = 255},
    	.disabled_color = {.r = 0, .g = 0, .b = 0, .a = 75},
	},

    .entry = {
    	.font_color = {.r = 200, .g = 200, .b = 200, .a = 255},
    	.bg_color = {.r = 65, .g = 65, .b = 65, .a = 0},
    	.border_color = {.r = 70, .g = 70, .b = 70, .a = 0},
    	.disabled_color = {.r = 0, .g = 0, .b = 0, .a = 75},
	},
};

SGUI_Sprite Hud_load_sprite( Hud *hud, const char *filepath )
{
	SGUI_Sprite result;

	result = SGUI_Sprite_from_file(hud->renderer, filepath);

	if (result.invalid)
		hud->invalid = true;

	return result;
}

void Hud_new( Hud *hud, const SDL_Renderer *renderer, const Config *cfg )
{
	// init values
	hud->invalid = false;
	hud->renderer = (SDL_Renderer*) renderer;
	hud->cmd_history_cursor = -1;

	for (uint_fast32_t i = 0; i < HUD_CMD_HISTORY_LEN; i++)
	{
        hud->cmd_history[i] = SM_String_new(8);
	}

	// get values from cfg
	hud->field_border_color.r = cfg->field_border_red;
	hud->field_border_color.g = cfg->field_border_green;
	hud->field_border_color.b = cfg->field_border_blue;
	hud->field_border_color.a = cfg->field_border_alpha;

	// load sprites
	hud->spr_ground = Hud_load_sprite(hud, PATH_TEXTURE_GROUND);
	hud->spr_hidden = Hud_load_sprite(hud, PATH_TEXTURE_HIDDEN);
	hud->spr_merc_base = Hud_load_sprite(hud, PATH_TEXTURE_MERC_BASE);
	hud->spr_merc_tint_green = Hud_load_sprite(hud, PATH_TEXTURE_MERC_GREEN);
	hud->spr_merc_tint_purple = Hud_load_sprite(hud, PATH_TEXTURE_MERC_PURPLE);

	for (uint_fast32_t i = 0; i < MERCENARY_SPRITE_COUNT; i++)
		hud->spr_mercs[i] = Hud_load_sprite(hud, PATH_TEXTURE_MERCENARIES[i]);

	for (uint_fast32_t i = 0; i < FIELD_SPRITE_OFFSET; i++)
		hud->spr_fields[i] = SGUI_Sprite_new();

	for (uint_fast32_t i = FIELD_SPRITE_OFFSET; i < (FIELD_SPRITE_COUNT + FIELD_SPRITE_OFFSET); i++)
		hud->spr_fields[i] = Hud_load_sprite(hud, PATH_TEXTURE_FIELDS[i - FIELD_SPRITE_OFFSET]);

	// load font
	TTF_Font *font = TTF_OpenFont(cfg->path_font, HUD_FONT_SIZE);

	if (font == NULL)
	{
		hud->invalid = true;
		return;
	}

	// make menu
	hud->mnu_hud = SGUI_Menu_new((SDL_Renderer*) renderer, THEME_RC.menu);
	SGUI_Label_new(&hud->lbl_hover_x, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_hover_y, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_hover_name, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_time_day, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_time_day_val, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_time_hour, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_time_hour_val, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_money, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_money_val, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_feedback, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Label_new(&hud->lbl_pointer, &hud->mnu_hud, font, THEME_RC.label);
	SGUI_Entry_new(&hud->txt_command, &hud->mnu_hud, font, THEME_RC.entry);

	// define menu
	hud->mnu_hud.rect.x = 0;
	hud->mnu_hud.rect.y = 0;
	hud->mnu_hud.rect.w = cfg->gfx_window_w;
	hud->mnu_hud.rect.h = cfg->gfx_window_h;
	hud->mnu_hud.focused_entry = &hud->txt_command;

	SM_String_copy_cstr(&hud->lbl_time_day.text, HUD_LBL_TIME_DAY_TEXT);
	SGUI_Label_update_sprite(&hud->lbl_time_day);

	SM_String_copy_cstr(&hud->lbl_time_hour.text, HUD_LBL_TIME_HOUR_TEXT);
	SGUI_Label_update_sprite(&hud->lbl_time_hour);

	SM_String_copy_cstr(&hud->lbl_money.text, HUD_LBL_MONEY_TEXT);
	SGUI_Label_update_sprite(&hud->lbl_money);

	SM_String_copy_cstr(&hud->lbl_pointer.text, ">");
	SGUI_Label_update_sprite(&hud->lbl_pointer);
}

void Hud_update_hover( Hud *hud, const SDL_Point coord, const char *name )
{
    // if hover values are out of town, make invisible and stop
    if (coord.x >= TOWN_WIDTH || coord.y >= TOWN_HEIGHT)
	{
        hud->lbl_hover_x.visible = false;
        hud->lbl_hover_y.visible = false;
        hud->lbl_hover_name.visible = false;
        return;
	}
	else
	{
		hud->lbl_hover_x.visible = true;
        hud->lbl_hover_y.visible = true;
        hud->lbl_hover_name.visible = true;
	}

    sprintf(hud->lbl_hover_x.text.str, "%i", coord.x);
    hud->lbl_hover_x.text.len = strlen(hud->lbl_hover_x.text.str);
    SGUI_Label_update_sprite(&hud->lbl_hover_x);
    hud->lbl_hover_x.rect.w = hud->lbl_hover_x.sprite.surface->w;
    hud->lbl_hover_x.rect.h = hud->lbl_hover_x.sprite.surface->h;

    sprintf(hud->lbl_hover_y.text.str, "%i", coord.y);
    hud->lbl_hover_y.text.len = strlen(hud->lbl_hover_y.text.str);
    SGUI_Label_update_sprite(&hud->lbl_hover_y);
    hud->lbl_hover_y.rect.w = hud->lbl_hover_y.sprite.surface->w;
    hud->lbl_hover_y.rect.h = hud->lbl_hover_y.sprite.surface->h;

    SM_String_copy_cstr(&hud->lbl_hover_name.text, name);
    SGUI_Label_update_sprite(&hud->lbl_hover_name);
    hud->lbl_hover_name.rect.w = hud->lbl_hover_name.sprite.surface->w;
    hud->lbl_hover_name.rect.h = hud->lbl_hover_name.sprite.surface->h;
}

void Hud_update_time( Hud *hud, const uint32_t round )
{
	sprintf(hud->lbl_time_day_val.text.str, "%u", (round / 24));
	hud->lbl_time_day_val.text.len = strlen(hud->lbl_time_day_val.text.str);
	SGUI_Label_update_sprite(&hud->lbl_time_day_val);

	hud->lbl_time_day_val.rect.w = hud->lbl_time_day_val.sprite.surface->w;
	hud->lbl_time_day_val.rect.h = hud->lbl_time_day_val.sprite.surface->h;

	sprintf(hud->lbl_time_hour_val.text.str, "%02u:00", (round % 24));
	hud->lbl_time_hour_val.text.len = strlen(hud->lbl_time_hour_val.text.str);
	SGUI_Label_update_sprite(&hud->lbl_time_hour_val);

	hud->lbl_time_hour_val.rect.w = hud->lbl_time_hour_val.sprite.surface->w;
	hud->lbl_time_hour_val.rect.h = hud->lbl_time_hour_val.sprite.surface->h;
}

void Hud_update_money( Hud *hud, const uint32_t money )
{
	sprintf(hud->lbl_money_val.text.str, "%u", money);
	hud->lbl_money_val.text.len = strlen(hud->lbl_money_val.text.str);
	SGUI_Label_update_sprite(&hud->lbl_money_val);

	hud->lbl_money_val.rect.w = hud->lbl_money_val.sprite.surface->w;
	hud->lbl_money_val.rect.h = hud->lbl_money_val.sprite.surface->h;
}

void Hud_update_feedback( Hud *hud, const char *str )
{
	SM_String_copy_cstr(&hud->lbl_feedback.text, str);
	SGUI_Label_update_sprite(&hud->lbl_feedback);
	hud->lbl_feedback.rect.w = hud->lbl_feedback.sprite.surface->w;
}

void Hud_calc( Hud *hud, const int32_t window_w, const int32_t window_h )
{
	// calc bar
	hud->rect_bar_top.x = window_w * HUD_BAR_TOP_X;
	hud->rect_bar_top.y = window_h * HUD_BAR_TOP_Y;
	hud->rect_bar_top.w = window_w * HUD_BAR_TOP_W;
	hud->rect_bar_top.h =
		((float) window_h * HUD_BAR_TOP_H_MARGIN * 2.0f) +
		hud->lbl_time_day.sprite.surface->h;

	// hover rect
    hud->lbl_hover_x.rect.x = window_w * HUD_LBL_HOVER_X_X;
    hud->lbl_hover_x.rect.y = hud->rect_bar_top.y + hud->rect_bar_top.h +
    	(window_h * HUD_LBL_HOVER_X_Y_DIST);

    hud->lbl_hover_y.rect.x = hud->lbl_hover_x.rect.x + hud->lbl_hover_x.rect.w +
    	(window_w * HUD_LBL_HOVER_Y_X_DIST);
	hud->lbl_hover_y.rect.y = hud->lbl_hover_x.rect.y;

	hud->lbl_hover_name.rect.x = hud->lbl_hover_y.rect.x + hud->lbl_hover_y.rect.w +
		(window_w * HUD_LBL_HOVER_NAME_X_DIST);
	hud->lbl_hover_name.rect.y = hud->lbl_hover_y.rect.y;

	// time widgets rect
	hud->lbl_time_day.rect.x = window_w * HUD_LBL_TIME_DAY_X;
	hud->lbl_time_day.rect.y = window_h * HUD_LBL_TIME_DAY_Y;
	hud->lbl_time_day.rect.w = hud->lbl_time_day.sprite.surface->w;
	hud->lbl_time_day.rect.h = hud->lbl_time_day.sprite.surface->h;

	hud->lbl_time_day_val.rect.x = (hud->lbl_time_day.rect.x + hud->lbl_time_day.rect.w) +
		(window_w * HUD_LBL_TIME_DAY_VAL_X_DIST);
	hud->lbl_time_day_val.rect.y = window_h * HUD_LBL_TIME_DAY_VAL_Y;

	hud->lbl_time_hour.rect.x = (hud->lbl_time_day_val.rect.x + hud->lbl_time_day_val.rect.w) +
		(window_w * HUD_LBL_TIME_HOUR_X_DIST);
	hud->lbl_time_hour.rect.y = window_h * HUD_LBL_TIME_HOUR_Y;
	hud->lbl_time_hour.rect.w = hud->lbl_time_hour.sprite.surface->w;
	hud->lbl_time_hour.rect.h = hud->lbl_time_hour.sprite.surface->h;

	hud->lbl_time_hour_val.rect.x = (hud->lbl_time_hour.rect.x + hud->lbl_time_hour.rect.w) +
		(window_w * HUD_LBL_TIME_HOUR_VAL_X_DIST);
	hud->lbl_time_hour_val.rect.y = window_h * HUD_LBL_TIME_HOUR_VAL_Y;

	// money widgets rect
	hud->lbl_money.rect.x = (hud->lbl_time_hour_val.rect.x + hud->lbl_time_hour_val.rect.w) +
		(window_w * HUD_LBL_MONEY_X_DIST);
	hud->lbl_money.rect.y = window_h * HUD_LBL_MONEY_Y;
	hud->lbl_money.rect.w = hud->lbl_money.sprite.surface->w;
	hud->lbl_money.rect.h = hud->lbl_money.sprite.surface->h;

	hud->lbl_money_val.rect.x = (hud->lbl_money.rect.x + hud->lbl_money.rect.w) +
		(window_w * HUD_LBL_MONEY_VAL_X_DIST);
	hud->lbl_money_val.rect.y = window_h * HUD_LBL_MONEY_VAL_Y;

	// command line widgets
	hud->lbl_feedback.rect.x = 0;
	hud->lbl_feedback.rect.y = window_h - ((HUD_FONT_SIZE + (0.01f * window_h)) * 2);
	hud->lbl_feedback.rect.w = window_w;
	hud->lbl_feedback.rect.h = HUD_FONT_SIZE + (0.01f * window_h);

	hud->lbl_pointer.rect.x = 0;
	hud->lbl_pointer.rect.y = window_h - (HUD_FONT_SIZE + (0.01f * window_h));
	hud->lbl_pointer.rect.w = hud->lbl_pointer.sprite.surface->w;
	hud->lbl_pointer.rect.h = HUD_FONT_SIZE + (0.01f * window_h);

	hud->txt_command.rect.x = hud->lbl_pointer.rect.w + 0;
	hud->txt_command.rect.y = window_h - (HUD_FONT_SIZE + (0.01f * window_h));
	hud->txt_command.rect.w = window_w;
	hud->txt_command.rect.h = HUD_FONT_SIZE + (0.01f * window_h);

	// cmd bar
    hud->rect_bar_cmd.w = window_w * HUD_BAR_CMD_W;
    hud->rect_bar_cmd.h =
    	hud->lbl_feedback.rect.h + hud->txt_command.rect.h +
    	(window_h * HUD_BAR_CMD_H_MARGIN);
    hud->rect_bar_cmd.x = 0;
    hud->rect_bar_cmd.y = window_h - hud->rect_bar_cmd.h;

	/* calculate area pos and size
		-as wide as high
		-stay on top and right
	*/
	hud->rect_area.h = window_h * HUD_AREA_H;
	hud->rect_area.h -= (hud->rect_area.h % TOWN_HEIGHT);  /* remove modulo, to fix mouse hitbox */
	hud->rect_area.w = hud->rect_area.h;
	hud->rect_area.y = window_h * HUD_AREA_Y;
	hud->rect_area.x = (window_w * HUD_AREA_X2) - hud->rect_area.w;

	// calc field size
	hud->field_width = hud->rect_area.w / TOWN_WIDTH;
	hud->field_height = hud->rect_area.h / TOWN_HEIGHT;

	// calc field rects
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			// full rect
			hud->rects_field[x][y].x = hud->rect_area.x + (x * hud->field_width);
			hud->rects_field[x][y].y = hud->rect_area.y + (y * hud->field_height);
			hud->rects_field[x][y].w = hud->field_width;
			hud->rects_field[x][y].h = hud->field_height;

			// content texture rect
			hud->rects_field_content[x][y].w = hud->rects_field[x][y].w * HUD_FIELD_CONTENT_SIZE;
			hud->rects_field_content[x][y].h = hud->rects_field[x][y].h * HUD_FIELD_CONTENT_SIZE;
			hud->rects_field_content[x][y].x =
				hud->rects_field[x][y].x +
				((hud->rects_field[x][y].w - hud->rects_field_content[x][y].w) / 2);
			hud->rects_field_content[x][y].y =
				hud->rects_field[x][y].y +
				((hud->rects_field[x][y].h - hud->rects_field_content[x][y].h) / 2);
		}
	}
}

void Hud_generate_flips( Hud *hud )
{
	uint_fast32_t flip;

	/* generate texture flip */
	for (uint_fast32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint_fast32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			/* random */
			flip = rand() % 3;

			switch (flip)
			{
			case 0:
				hud->flips_field[x][y] = SDL_FLIP_NONE;
				break;

			case 1:
				hud->flips_field[x][y] = SDL_FLIP_VERTICAL;
				break;

			case 2:
				hud->flips_field[x][y] = SDL_FLIP_HORIZONTAL;
				break;

			case 3:
				hud->flips_field[x][y] = SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL;
				break;
			}
		}
	}
}

void Hud_map_textures(
	Hud *hud,
	const bool fields_hidden[TOWN_WIDTH][TOWN_HEIGHT],
	const Field fields_content[TOWN_WIDTH][TOWN_HEIGHT] )
{
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			// if field hidden
			if (fields_hidden[x][y] == true)
			{
				// assign hidden ground texture
				hud->textures_field_ground[x][y] = hud->spr_hidden.texture;

				// and null content texture
				hud->textures_field_content[x][y] = NULL;
			}
			else
			{
				// assign exposed ground texture
				hud->textures_field_ground[x][y] = hud->spr_ground.texture;

				// map textures to area content
				hud->textures_field_content[x][y] = hud->spr_fields[fields_content[x][y]].texture;
			}
		}
	}
}

void Hud_draw( Hud *hud, const Town *town )
{
	// draw fields
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			// draw stored ground texture
			SDL_RenderCopyEx(hud->renderer, hud->textures_field_ground[x][y], NULL, &hud->rects_field[x][y],
				0.0f,
				NULL,
				hud->flips_field[x][y]);

			// draw stored content texture
			SDL_RenderCopy(
				hud->renderer,
				hud->textures_field_content[x][y],
				NULL,
				&hud->rects_field_content[x][y]);

			// draw field border
			SDL_SetRenderDrawColor(
				hud->renderer,
				hud->field_border_color.r,
				hud->field_border_color.g,
				hud->field_border_color.b,
				hud->field_border_color.a);
			SDL_RenderDrawRect(hud->renderer, &hud->rects_field[x][y]);
		}
	}

	for (uint32_t i = 0; i < town->merc_count; i++)
	{
		// if merc is dead, skip drawing
		if (town->mercs[i].hp == 0)
			continue;

		// draw mercenary fields
		SDL_RenderCopy(
			hud->renderer,
        	hud->spr_merc_base.texture,
        	NULL,
        	&hud->rects_field_content[town->mercs[i].coords.x][town->mercs[i].coords.y]);

		// draw fraction color tint
        if (town->mercs[i].fraction == MF_GREEN)
        {
			SDL_RenderCopy(
				hud->renderer,
	        	hud->spr_merc_tint_green.texture,
        		NULL,
        		&hud->rects_field_content[town->mercs[i].coords.x][town->mercs[i].coords.y]);
        }
        else
        {
        	SDL_RenderCopy(
				hud->renderer,
        		hud->spr_merc_tint_purple.texture,
	        	NULL,
    	    	&hud->rects_field_content[town->mercs[i].coords.x][town->mercs[i].coords.y]);
        }

		// draw mercenary class icon
		SDL_RenderCopy(
			hud->renderer,
			hud->spr_mercs[town->mercs[i].id].texture,
			NULL,
			&hud->rects_field_content[town->mercs[i].coords.x][town->mercs[i].coords.y]);
	}

	// draw hud bars
	SDL_SetRenderDrawColor(
		hud->renderer,
		HUD_BAR_COLOR_R,
		HUD_BAR_COLOR_G,
		HUD_BAR_COLOR_B,
		HUD_BAR_COLOR_A);
	SDL_RenderFillRect(hud->renderer, &hud->rect_bar_top);
	SDL_RenderFillRect(hud->renderer, &hud->rect_bar_cmd);

	// draw hud menu
	SGUI_Menu_draw(&hud->mnu_hud);
}

void Hud_set_field( Hud *hud, const SDL_Point field, const SDL_Texture *texture )
{
	hud->textures_field_content[field.x][field.y] = (SDL_Texture*) texture;
}

void Hud_add_to_command_history( Hud *hud, const char *cmd )
{
    // push old ones back
    for (uint_fast32_t i = HUD_CMD_HISTORY_LEN - 1; i > 0; i--)
    {
    	SM_String_copy(&hud->cmd_history[i], &hud->cmd_history[i - 1]);
    }

    // add new cmd
    SM_String_copy_cstr(&hud->cmd_history[0], cmd);
}

void Hud_clear( Hud *hud )
{
	SGUI_Menu_clear(&hud->mnu_hud);

	SGUI_Sprite_clear(&hud->spr_ground);
	SGUI_Sprite_clear(&hud->spr_hidden);

	SGUI_Sprite_clear(&hud->spr_merc_base);
	SGUI_Sprite_clear(&hud->spr_merc_tint_green);
	SGUI_Sprite_clear(&hud->spr_merc_tint_purple);

	for (uint32_t i = 0; i < MERCENARY_SPRITE_COUNT; i++)
		SGUI_Sprite_clear(&hud->spr_mercs[i]);

	for (uint32_t i = 0; i < FIELD_SPRITE_COUNT; i++)
		SGUI_Sprite_clear(&hud->spr_fields[i]);
}
