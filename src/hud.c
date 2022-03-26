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
#include "definitions/def_files.h"
#include "definitions/def_messages.h"
#include "definitions/def_hud.h"
#include "game.h"
#include "hud.h"

static const SGUI_Theme THEME_RC = {
	.menu_bg_color = {.r = 30, .g = 30, .b = 30, .a = 255},

	.label_font_color = {.r = 200, .g = 200, .b = 200, .a = 255},
    .label_bg_color = {.r = 0, .g = 0, .b = 0, .a = 0},
    .label_border_color = {.r = 0, .g = 0, .b = 0, .a = 0},

    .button_font_color = {.r = 200, .g = 200, .b = 200, .a = 255},
    .button_bg_color = {.r = 50, .g = 50, .b = 50, .a = 255},
    .button_border_color = {.r = 70, .g = 70, .b = 70, .a = 255},
    .button_disabled_color = {.r = 0, .g = 0, .b = 0, .a = 75},

    .entry_font_color = {.r = 200, .g = 200, .b = 200, .a = 255},
    .entry_bg_color = {.r = 65, .g = 65, .b = 65, .a = 255},
    .entry_border_color = {.r = 70, .g = 70, .b = 70, .a = 255},
    .entry_disabled_color = {.r = 0, .g = 0, .b = 0, .a = 75}
};

Hud Hud_new(SDL_Renderer *p_renderer, char *p_path_font)
{
	SMString temp;
	Hud result = {
		.invalid = false,
		.state = HS_NORMAL,
		.renderer = p_renderer
	};

	// load sprites
	((Sprite_from_image(
			&self->spr_hud_construct,
			self->renderer,
			PATH_TEXTURE_HUD_CONSTRUCT) != 0) ||
		(Sprite_from_image(
			&self->spr_hud_deconstruct,
			self->renderer,
			PATH_TEXTURE_HUD_DECONSTRUCT) != 0) ||

		(Sprite_from_image(&self->spr_ground, self->renderer, PATH_TEXTURE_GROUND) != 0) ||
		(Sprite_from_image(&self->spr_hidden, self->renderer, PATH_TEXTURE_HIDDEN) != 0) ||

		(Sprite_from_image(&self->spr_merc_base, self->renderer, PATH_TEXTURE_MERC_BASE) != 0) ||
		(Sprite_from_image(&self->spr_merc_tint_green, self->renderer, PATH_TEXTURE_MERC_GREEN) != 0) ||
		(Sprite_from_image(&self->spr_merc_tint_purple, self->renderer, PATH_TEXTURE_MERC_PURPLE) != 0) ||
		(Sprite_from_image(&self->spr_mercs[0], self->renderer, PATH_TEXTURE_MERC_SOLDIER) != 0) ||
		(Sprite_from_image(&self->spr_mercs[1], self->renderer, PATH_TEXTURE_MERC_PYRO) != 0) ||
		(Sprite_from_image(&self->spr_mercs[2], self->renderer, PATH_TEXTURE_MERC_ANCHOR) != 0) ||
		(Sprite_from_image(&self->spr_mercs[3], self->renderer, PATH_TEXTURE_MERC_MEDIC) != 0) ||

		(Sprite_from_image(&self->spr_trees[0], self->renderer, PATH_TEXTURE_TREE_0) != 0) ||
		(Sprite_from_image(&self->spr_trees[1], self->renderer, PATH_TEXTURE_TREE_1) != 0) ||
		(Sprite_from_image(&self->spr_trees[2], self->renderer, PATH_TEXTURE_TREE_2) != 0) ||
		(Sprite_from_image(&self->spr_trees[3], self->renderer, PATH_TEXTURE_TREE_3) != 0) ||
		(Sprite_from_image(&self->spr_trees[4], self->renderer, PATH_TEXTURE_TREE_4) != 0) ||

		(Sprite_from_image(&self->spr_hq, self->renderer, PATH_TEXTURE_HQ) != 0) ||
		(Sprite_from_image(&self->spr_construction, self->renderer, PATH_TEXTURE_CONSTRUCTION) != 0) ||
		(Sprite_from_image(&self->spr_quarry, self->renderer, PATH_TEXTURE_QUARRY) != 0))

	// make menu
	TTF_Font *font = TTF_OpenFont(p_path_font, HUD_FONT_SIZE);

	result.mnu_hud = SGUI_Menu(p_renderer, font, &THEME_RC);
	SGUI_Label_new(&result.lbl_time_day, &result.mnu_hud, &THEME_RC);
	SGUI_Label_new(&result.lbl_time_day_val, &result.mnu_hud, &THEME_RC);
	SGUI_Label_new(&result.lbl_time_hour, &result.mnu_hud, &THEME_RC);
	SGUI_Label_new(&result.lbl_time_hour_val, &result.mnu_hud, &THEME_RC);
	SGUI_Label_new(&result.lbl_money, &result.mnu_hud, &THEME_RC);
	SGUI_Label_new(&result.lbl_money_val, &result.mnu_hud, &THEME_RC);

	// define menu




	strncpy(self->lbl_time_day.text, HUD_LBL_TIME_DAY_TEXT, HUD_WIDGET_TEXT_MAX_LEN);
	strncpy(self->lbl_time_hour.text, HUD_LBL_TIME_HOUR_TEXT, HUD_WIDGET_TEXT_MAX_LEN);
	strncpy(self->lbl_money.text, HUD_LBL_MONEY_TEXT, HUD_WIDGET_TEXT_MAX_LEN);
	strncpy(self->lbl_money_val.text, "0", HUD_WIDGET_TEXT_MAX_LEN);
	strncpy(self->btn_pass.text, HUD_BTN_PASS_TEXT, HUD_WIDGET_TEXT_MAX_LEN);

	// check
	if (font == NULL)
	{
		result.invalid = true;
	}

	dont forget other checks!;

	return result;
}

void Hud_update_time(struct Hud *self, uint32_t p_round)
{
	/* create widget strings */
	sprintf(self->lbl_time_day_val.text, "%u", (p_round / 24));
	sprintf(self->lbl_time_hour_val.text, "%02u:00", (p_round % 24));

	/* gen widgets */
	Widget_generate_sprite(&self->lbl_time_day_val, self->renderer, self->font, self->font_color);
	Widget_generate_sprite(&self->lbl_time_hour_val, self->renderer, self->font, self->font_color);
}

void Hud_update_money(struct Hud *self, uint32_t p_money)
{
	/* create widget strings */
	sprintf(self->lbl_money_val.text, "%u", p_money);

	/* gen widget */
	Widget_generate_sprite(&self->lbl_money_val, self->renderer, self->font, self->font_color);
}

void Hud_calc(struct Hud *self,	int32_t p_window_w,	int32_t p_window_h)
{
	/* calc bars */
	self->rect_bar_top.x = p_window_w * HUD_BAR_TOP_X;
	self->rect_bar_top.y = p_window_h * HUD_BAR_TOP_Y;
	self->rect_bar_top.w = p_window_w * HUD_BAR_TOP_W;
	self->rect_bar_top.h =
		((float) p_window_h * HUD_BAR_TOP_H_MARGIN * 2.0f) +
		self->lbl_time_day.sprite.surface->h;

	self->rect_bar_side.x = p_window_w * HUD_BAR_SIDE_X;
	self->rect_bar_side.y = p_window_h * HUD_BAR_SIDE_Y;
	self->rect_bar_side.w = p_window_w * HUD_BAR_SIDE_W;
	self->rect_bar_side.h = p_window_h * HUD_BAR_SIDE_H;

	/* calc time widgets position */
	self->lbl_time_day.rect.x = p_window_w * HUD_LBL_TIME_DAY_X;
	self->lbl_time_day.rect.y = p_window_h * HUD_LBL_TIME_DAY_Y;

	self->lbl_time_day_val.rect.x = (self->lbl_time_day.rect.x + self->lbl_time_day.rect.w) +
		(p_window_w * HUD_LBL_TIME_DAY_VAL_X_DIST);
	self->lbl_time_day_val.rect.y = p_window_h * HUD_LBL_TIME_DAY_VAL_Y;

	self->lbl_time_hour.rect.x = (self->lbl_time_day_val.rect.x + self->lbl_time_day_val.rect.w) +
		(p_window_w * HUD_LBL_TIME_HOUR_X_DIST);
	self->lbl_time_hour.rect.y = p_window_h * HUD_LBL_TIME_HOUR_Y;

	self->lbl_time_hour_val.rect.x = (self->lbl_time_hour.rect.x + self->lbl_time_hour.rect.w) +
		(p_window_w * HUD_LBL_TIME_HOUR_VAL_X_DIST);
	self->lbl_time_hour_val.rect.y = p_window_h * HUD_LBL_TIME_HOUR_VAL_Y;

	/* money widgets */
	self->lbl_money.rect.x = (self->lbl_time_hour_val.rect.x + self->lbl_time_hour_val.rect.w) +
		(p_window_w * HUD_LBL_MONEY_X_DIST);
	self->lbl_money.rect.y = p_window_h * HUD_LBL_MONEY_Y;

	self->lbl_money_val.rect.x = (self->lbl_money.rect.x + self->lbl_money.rect.w) +
		(p_window_w * HUD_LBL_MONEY_VAL_X_DIST);
	self->lbl_money_val.rect.y = p_window_h * HUD_LBL_MONEY_VAL_Y;

	/* pass button */
	self->btn_pass.rect.x = p_window_w * HUD_BTN_PASS_X;
	self->btn_pass.rect.y = p_window_h * HUD_BTN_PASS_Y;
	self->btn_pass.rect.w = p_window_w * HUD_BTN_PASS_W;
	self->btn_pass.rect.h = p_window_h * HUD_BTN_PASS_H;

	/* construct btn */
	self->btn_construct.rect.x = p_window_w * HUD_BTN_CONSTRUCT_X;
	self->btn_construct.rect.y = p_window_h * HUD_BTN_CONSTRUCT_Y;
	self->btn_construct.rect.w = p_window_w * HUD_BTN_CONSTRUCT_W;
	self->btn_construct.rect.h = p_window_h * HUD_BTN_CONSTRUCT_H;

	/* deconstruct button */
	self->btn_deconstruct.rect.x = p_window_w * HUD_BTN_DECONSTRUCT_X;
	self->btn_deconstruct.rect.y = p_window_h * HUD_BTN_DECONSTRUCT_Y;
	self->btn_deconstruct.rect.w = p_window_w * HUD_BTN_DECONSTRUCT_W;
	self->btn_deconstruct.rect.h = p_window_h * HUD_BTN_DECONSTRUCT_H;

	/* construct widgets */
	self->btn_construct_quarry.rect.x = p_window_w * HUD_BTN_CONSTRUCT_QUARRY_X;
	self->btn_construct_quarry.rect.y = p_window_h * HUD_BTN_CONSTRUCT_QUARRY_Y;
	self->btn_construct_quarry.rect.w = p_window_w * HUD_BTN_CONSTRUCT_QUARRY_W;
	self->btn_construct_quarry.rect.h = p_window_h * HUD_BTN_CONSTRUCT_QUARRY_H;

	/* calculate area pos and size
		-as wide as high
		-stay on top and right
	*/
	self->rect_area.h = p_window_h * HUD_AREA_H;
	self->rect_area.h -= (self->rect_area.h % TOWN_HEIGHT);  /* remove modulo, to fix mouse hitbox */
	self->rect_area.w = self->rect_area.h;
	self->rect_area.y = p_window_h * HUD_AREA_Y;
	self->rect_area.x = (p_window_w * HUD_AREA_X2) - self->rect_area.w;

	/* calc field size */
	self->field_width = self->rect_area.w / TOWN_WIDTH;
	self->field_height = self->rect_area.h / TOWN_HEIGHT;

	/* calc field rects */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			/* full rect */
			self->rects_field[x][y].x = self->rect_area.x + (x * self->field_width);
			self->rects_field[x][y].y = self->rect_area.y + (y * self->field_height);
			self->rects_field[x][y].w = self->field_width;
			self->rects_field[x][y].h = self->field_height;

			/* content texture rect */
			self->rects_field_content[x][y].w = self->rects_field[x][y].w * HUD_FIELD_CONTENT_SIZE;
			self->rects_field_content[x][y].h = self->rects_field[x][y].h * HUD_FIELD_CONTENT_SIZE;
			self->rects_field_content[x][y].x =
				self->rects_field[x][y].x +
				((self->rects_field[x][y].w - self->rects_field_content[x][y].w) / 2);
			self->rects_field_content[x][y].y =
				self->rects_field[x][y].y +
				((self->rects_field[x][y].h - self->rects_field_content[x][y].h) / 2);
		}
	}
}

void Hud_generate_flips(struct Hud *self)
{
	uint32_t flip;

	/* generate texture flip */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			/* random */
			flip = rand() % 3;

			switch (flip)
			{
			case 0:
				self->flips_field[x][y] = SDL_FLIP_NONE;
				break;

			case 1:
				self->flips_field[x][y] = SDL_FLIP_VERTICAL;
				break;

			case 2:
				self->flips_field[x][y] = SDL_FLIP_HORIZONTAL;
				break;

			case 3:
				self->flips_field[x][y] = SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL;
				break;
			}
		}
	}
}

void Hud_map_textures(
	struct Hud *self,
	bool p_fields_hidden[TOWN_WIDTH][TOWN_HEIGHT],
	enum Field p_fields_content[TOWN_WIDTH][TOWN_HEIGHT])
{
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			/* if field hidden */
			if (p_fields_hidden[x][y] == true)
			{
				/* assign hidden ground texture */
				self->textures_field_ground[x][y] = self->spr_hidden.texture;

				/* and null content texture */
				self->textures_field_content[x][y] = NULL;
			}
			else
			{
				/* assign exposed ground texture */
				self->textures_field_ground[x][y] = self->spr_ground.texture;

				/* map textures to area content */
				self->textures_field_content[x][y] = Hud_get_field_texture(self, p_fields_content[x][y]);
			}
		}
	}
}

void Hud_draw(struct Hud *self, struct Town *p_town)
{
	/* draw fields */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			/* draw stored ground texture */
			SDL_RenderCopyEx(self->renderer, self->textures_field_ground[x][y], NULL, &self->rects_field[x][y],
				0.0f,
				NULL,
				self->flips_field[x][y]);

			/* draw given content texture */
			SDL_RenderCopy(
				self->renderer,
				self->textures_field_content[x][y],
				NULL,
				&self->rects_field_content[x][y]);

			/* draw field border */
			SDL_SetRenderDrawColor(
				self->renderer,
				self->field_border_color.r,
				self->field_border_color.g,
				self->field_border_color.b,
				self->field_border_color.a);
			SDL_RenderDrawRect(self->renderer, &self->rects_field[x][y]);
		}
	} /* draw fields */

	for (uint32_t i = 0; i < p_town->merc_count; i++)
	{
		// draw mercenary fields
		SDL_RenderCopy(
			self->renderer,
        	self->spr_merc_base.texture,
        	NULL,
        	&self->rects_field_content[p_town->mercs[i].coords.x][p_town->mercs[i].coords.y]);

		// draw fraction color tint
        if (p_town->mercs[i].fraction == MF_GREEN)
        {
			SDL_RenderCopy(
				self->renderer,
	        	self->spr_merc_tint_green.texture,
        		NULL,
        		&self->rects_field_content[p_town->mercs[i].coords.x][p_town->mercs[i].coords.y]);
        }
        else
        {
        	SDL_RenderCopy(
				self->renderer,
        		self->spr_merc_tint_purple.texture,
	        	NULL,
    	    	&self->rects_field_content[p_town->mercs[i].coords.x][p_town->mercs[i].coords.y]);
        }

		// draw mercenary class icon
		SDL_RenderCopy(
			self->renderer,
			self->spr_mercs[p_town->mercs[i].id].texture,
			NULL,
			&self->rects_field_content[p_town->mercs[i].coords.x][p_town->mercs[i].coords.y]);
	}

	/* draw hud bars */
	SDL_SetRenderDrawColor(
		self->renderer,
		HUD_BAR_COLOR_R,
		HUD_BAR_COLOR_G,
		HUD_BAR_COLOR_B,
		HUD_BAR_COLOR_A);
	SDL_RenderFillRect(self->renderer, &self->rect_bar_top);

	SDL_RenderFillRect(self->renderer, &self->rect_bar_side);

	/* draw time widgets */
	Widget_draw(&self->lbl_time_day, self->renderer);
	Widget_draw(&self->lbl_time_day_val, self->renderer);
	Widget_draw(&self->lbl_time_hour, self->renderer);
	Widget_draw(&self->lbl_time_hour_val, self->renderer);

	/* draw money widgets */
	Widget_draw(&self->lbl_money, self->renderer);
	Widget_draw(&self->lbl_money_val, self->renderer);

	/* draw pass button */
	Widget_draw(&self->btn_pass, self->renderer);

	/* construct btn */
	Widget_draw(&self->btn_construct, self->renderer);

	/* deconstruct btn */
	Widget_draw(&self->btn_deconstruct, self->renderer);

	/* draw optional menus */
	switch (self->state)
	{
	case HS_NORMAL:
		break;

	case HS_CONSTRUCT:
		Widget_draw(&self->btn_construct_quarry, self->renderer);
		break;
	}

	/* draw depending on hover mode */
	switch (self->hover_mode)
	{
	case HHM_NONE:
		break;

	case HHM_CONSTRUCT:
        /* draw to be constructed building on hover field */
        SDL_RenderCopy(self->renderer, self->texture_hover_construct, NULL, &self->rect_hover_construct);
		break;

	case HHM_DECONSTRUCT:
		/* draw red rect on hovered field */
		SDL_SetRenderDrawColor(
			self->renderer,
			HUD_DECONSTRUCT_HOVER_R,
			HUD_DECONSTRUCT_HOVER_G,
			HUD_DECONSTRUCT_HOVER_B,
			HUD_DECONSTRUCT_HOVER_A);
		SDL_RenderFillRect(self->renderer, &self->rect_hover_deconstruct);
	}
}

SDL_Point Hud_mouse_to_field(struct Hud *self, SDL_Point p_mouse)
{
	SDL_Point result;

	result.x = (p_mouse.x - self->rect_area.x) / self->field_width;
	result.y = (p_mouse.y - self->rect_area.y) / self->field_height;

	return result;
}

void Hud_handle_click(struct Hud *self, SDL_Point p_mouse, struct Game *p_game)
{
	SDL_Point temp;

	switch (self->hover_mode)
	{
	case HHM_NONE:
		/* if pass button pressed */
		if (SDL_PointInRect(&p_mouse, &self->btn_pass.rect) == true)
		{
			Game_end_round(p_game, self);
		}

		/* if construct button pressed */
		else if (SDL_PointInRect(&p_mouse, &self->btn_construct.rect) == true)
		{
			self->state = HS_CONSTRUCT;
		}

		/* if deconstruct button pressed */
		else if (SDL_PointInRect(&p_mouse, &self->btn_deconstruct.rect) == true)
		{
			Hud_deconstruct_mode(self);
		}

		/* if hud has construct state, check construct widgets */
		else if (self->state == HS_CONSTRUCT)
		{
			if (SDL_PointInRect(&p_mouse, &self->btn_construct_quarry.rect) == true)
			{
				Hud_construct_mode(self, FIELD_QUARRY, self->spr_quarry.texture);
			}
		}
		break;

	case HHM_CONSTRUCT:
		/* if field clicked */
		if (SDL_PointInRect(&p_mouse, &self->rect_area))
		{
			/* get field coord */
			temp = Hud_mouse_to_field(self, p_mouse);

			/* if field is exposed and empty */
			if ((p_game->town->hidden[temp.x][temp.y] == false) &&
				(p_game->town->field[temp.x][temp.y] == FIELD_EMPTY))
			{
				/* update game and hud */
				Game_construct(p_game, temp, self->hover_construct, self, self->texture_hover_construct);
				Hud_set_field(self, temp, self->spr_construction.texture);
			}
		}

		/* stop construct mode */
		self->hover_mode = HHM_NONE;
		break;

	case HHM_DECONSTRUCT:
        /* if field clicked */
		if (SDL_PointInRect(&p_mouse, &self->rect_area))
		{
			/* get field coord */
			temp = Hud_mouse_to_field(self, p_mouse);

			/* if field is exposed and not empty */
			if ((p_game->town->hidden[temp.x][temp.y] == false) &&
				(p_game->town->field[temp.x][temp.y] != FIELD_EMPTY))
			{
				/* update game and hud */
				Game_construct(p_game, temp, FIELD_EMPTY, self, NULL);
				Hud_set_field(self, temp, self->spr_construction.texture);
			}
		}

		/* stop deconstruct mode */
		self->hover_mode = HHM_NONE;
		break;
	}
}

void Hud_handle_hover(struct Hud *self, SDL_Point p_mouse)
{
	/* if mouse not in area, stop */
	if (SDL_PointInRect(&p_mouse, &self->rect_area) == false)
	{
		return;
	}

	/* update hover value */
	self->hover_field = Hud_mouse_to_field(self, p_mouse);

	/* react to hover modes */
	switch (self->hover_mode)
	{
	case HHM_NONE:
		break;

	case HHM_CONSTRUCT:
		self->rect_hover_construct.x =
			self->rects_field_content[self->hover_field.x][self->hover_field.y].x;
		self->rect_hover_construct.y =
			self->rects_field_content[self->hover_field.x][self->hover_field.y].y;
		self->rect_hover_construct.w =
			self->rects_field_content[self->hover_field.x][self->hover_field.y].w;
		self->rect_hover_construct.h =
			self->rects_field_content[self->hover_field.x][self->hover_field.y].h;
		break;

	case HHM_DECONSTRUCT:
		self->rect_hover_deconstruct.x =
			self->rects_field_content[self->hover_field.x][self->hover_field.y].x;
		self->rect_hover_deconstruct.y =
			self->rects_field_content[self->hover_field.x][self->hover_field.y].y;
		self->rect_hover_deconstruct.w =
			self->rects_field_content[self->hover_field.x][self->hover_field.y].w;
		self->rect_hover_deconstruct.h =
			self->rects_field_content[self->hover_field.x][self->hover_field.y].h;
		break;
	}
}

void Hud_construct_mode(struct Hud *self, enum Field p_field, SDL_Texture *p_texture)
{
	self->hover_mode = HHM_CONSTRUCT;
	self->hover_construct = p_field;
	self->texture_hover_construct = p_texture;
}

void Hud_deconstruct_mode(struct Hud *self)
{
	self->hover_mode = HHM_DECONSTRUCT;
}

void Hud_set_field(struct Hud *self, SDL_Point p_field, SDL_Texture *p_texture)
{
	self->textures_field_content[p_field.x][p_field.y] = p_texture;
}

SDL_Texture* Hud_get_field_texture(struct Hud *self, enum Field p_field)
{
	switch (p_field)
	{
	case FIELD_EMPTY:
		break;

	case FIELD_MERC:
		/* the mercenary sprites are handled differently,
		so that they can't be simply drawn by the town_field drawing part */
		break;

	case FIELD_TREE_0:
		return self->spr_trees[0].texture;
		break;

	case FIELD_TREE_1:
		return self->spr_trees[1].texture;
		break;

	case FIELD_TREE_2:
		return self->spr_trees[2].texture;
		break;

	case FIELD_TREE_3:
		return self->spr_trees[3].texture;
		break;

	case FIELD_TREE_4:
		return self->spr_trees[4].texture;
		break;

	case FIELD_ADMINISTRATION:
		return self->spr_hq.texture;
		break;

	case FIELD_CONSTRUCTION:
		return self->spr_construction.texture;
		break;

	case FIELD_QUARRY:
		return self->spr_quarry.texture;
		break;
	}

	return NULL;
}

void Hud_clear(struct Hud *self)
{
	TTF_CloseFont(self->font);

	Widget_clear(&self->lbl_time_day);
	Widget_clear(&self->lbl_time_day_val);
	Widget_clear(&self->lbl_time_hour);
	Widget_clear(&self->lbl_time_hour_val);
	Widget_clear(&self->lbl_money);
	Widget_clear(&self->lbl_money_val);
	Widget_clear(&self->btn_pass);
	Widget_clear(&self->btn_construct);
	Widget_clear(&self->btn_deconstruct);

	Widget_clear(&self->btn_construct_quarry);

	Sprite_clear(&self->spr_hud_deconstruct);

	Sprite_clear(&self->spr_ground);
	Sprite_clear(&self->spr_hidden);

	Sprite_clear(&self->spr_merc_base);
	Sprite_clear(&self->spr_merc_tint_green);
	Sprite_clear(&self->spr_merc_tint_purple);

	for (uint32_t i = 0; i < TOWN_MAX_MERCS; i++)
		Sprite_clear(&self->spr_mercs[i]);

	for (uint32_t i = 0; i < TOWN_TREE_VARIETY_COUNT; i++)
		Sprite_clear(&self->spr_trees[i]);

	Sprite_clear(&self->spr_hq);
	Sprite_clear(&self->spr_construction);
	Sprite_clear(&self->spr_quarry);
}
