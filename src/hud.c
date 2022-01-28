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

int32_t Hud_new(struct Hud *self, SDL_Renderer *p_renderer, char *p_path_font)
{
	//set data
	self->state = HS_NORMAL;
	self->hover_mode = HHM_NONE;
	self->texture_hover_construct = NULL;

	//set renderer
	self->renderer = p_renderer;

	//load font
	self->font = TTF_OpenFont(p_path_font, HUD_FONT_SIZE);

	if (self->font == NULL)
	{
		printf(MSG_ERR_FONT_LOAD, p_path_font);
		return 1;
	}

	//init widgets
	Widget_new(&self->lbl_time_day);
	Widget_new(&self->lbl_time_day_val);
	Widget_new(&self->lbl_time_hour);
	Widget_new(&self->lbl_time_hour_val);
	Widget_new(&self->lbl_money);
	Widget_new(&self->lbl_money_val);
	Widget_new(&self->btn_construct);
	Widget_new(&self->btn_pass);

	Widget_new(&self->btn_construct_quarry);

	//init sprites
	Sprite_new(&self->spr_ground);
	Sprite_new(&self->spr_hidden);

	for (uint32_t i = 0; i < TOWN_TREE_VARIETY_COUNT; i++)
		Sprite_new(&self->spr_trees[i]);

	Sprite_new(&self->spr_hq);
	Sprite_new(&self->spr_construction);
	Sprite_new(&self->spr_quarry);

	//set widget texts
	strcpy(self->lbl_time_day.text, HUD_LBL_TIME_DAY_TEXT);
	strcpy(self->lbl_time_hour.text, HUD_LBL_TIME_HOUR_TEXT);
	strcpy(self->lbl_money.text, HUD_LBL_MONEY_TEXT);
	strcpy(self->lbl_money_val.text, "0");
	strcpy(self->btn_construct.text, "C");
	strcpy(self->btn_pass.text, HUD_BTN_PASS_TEXT);

	strcpy(self->btn_construct_quarry.text, "Q");

	return 0;
}

void Hud_update_time(struct Hud *self, uint32_t p_round)
{
	//create widget strings
	sprintf(self->lbl_time_day_val.text, "%u", (p_round / 24));
	sprintf(self->lbl_time_hour_val.text, "%02u:00", p_round);

	//gen widgets
	Widget_generate_sprite(&self->lbl_time_day_val, self->renderer, self->font, self->font_color);
	Widget_generate_sprite(&self->lbl_time_hour_val, self->renderer, self->font, self->font_color);
}

void Hud_update_money(struct Hud *self, uint32_t p_money)
{
	//create widget strings
	sprintf(self->lbl_money_val.text, "%u", p_money);

	//gen widget
	Widget_generate_sprite(&self->lbl_money_val, self->renderer, self->font, self->font_color);
}

int32_t Hud_init_widgets(struct Hud *self)
{
	//create sprites of widgets, that wont change their sprite
	if ((Widget_generate_sprite(&self->lbl_time_day, self->renderer, self->font, self->font_color) != 0) ||
		(Widget_generate_sprite(&self->lbl_time_hour, self->renderer, self->font, self->font_color) != 0) ||
		(Widget_generate_sprite(&self->lbl_money, self->renderer, self->font, self->font_color) != 0) ||
		(Widget_generate_sprite(&self->btn_construct, self->renderer, self->font, self->font_color) != 0) ||
		(Widget_generate_sprite(&self->btn_pass, self->renderer, self->font, self->font_color) != 0) ||

		(Widget_generate_sprite(
			&self->btn_construct_quarry,
			self->renderer,
			self->font,
			self->font_color) != 0))
	{
		return 1;
	}

	return 0;
}

int32_t Hud_load_sprites(struct Hud *self)
{
	//load sprites
	if ((Sprite_from_image(&self->spr_ground, self->renderer, PATH_TEXTURE_GROUND) != 0) ||
		(Sprite_from_image(&self->spr_hidden, self->renderer, PATH_TEXTURE_HIDDEN) != 0) ||
		(Sprite_from_image(&self->spr_trees[0], self->renderer, PATH_TEXTURE_TREE_0) != 0) ||
		(Sprite_from_image(&self->spr_trees[1], self->renderer, PATH_TEXTURE_TREE_1) != 0) ||
		(Sprite_from_image(&self->spr_trees[2], self->renderer, PATH_TEXTURE_TREE_2) != 0) ||
		(Sprite_from_image(&self->spr_trees[3], self->renderer, PATH_TEXTURE_TREE_3) != 0) ||
		(Sprite_from_image(&self->spr_trees[4], self->renderer, PATH_TEXTURE_TREE_4) != 0) ||
		(Sprite_from_image(&self->spr_hq, self->renderer, PATH_TEXTURE_HQ) != 0) ||
		(Sprite_from_image(&self->spr_construction, self->renderer, PATH_TEXTURE_CONSTRUCTION) != 0) ||
		(Sprite_from_image(&self->spr_quarry, self->renderer, PATH_TEXTURE_QUARRY) != 0))
	{
		return 1;
	}

	return 0;
}

void Hud_calc(struct Hud *self,	int32_t p_window_w,	int32_t p_window_h)
{
	//calc time widgets position
	self->lbl_time_day.rect.x = p_window_w * HUD_LBL_TIME_DAY_X;
	self->lbl_time_day.rect.y = p_window_h * HUD_LBL_TIME_DAY_Y;

	self->lbl_time_day_val.rect.x = (self->lbl_time_day.rect.x + self->lbl_time_day.rect.w) +
		(p_window_w * HUD_LBL_TIME_DAY_VAL_X_DIST);
	self->lbl_time_day_val.rect.y = p_window_h * HUD_LBL_TIME_DAY_VAL_Y;

	self->lbl_time_hour.rect.x = p_window_w * HUD_LBL_TIME_HOUR_X;
	self->lbl_time_hour.rect.y = p_window_h * HUD_LBL_TIME_HOUR_Y;

	self->lbl_time_hour_val.rect.x = (self->lbl_time_hour.rect.x + self->lbl_time_hour.rect.w) +
		(p_window_w * HUD_LBL_TIME_HOUR_VAL_X_DIST);
	self->lbl_time_hour_val.rect.y = p_window_h * HUD_LBL_TIME_HOUR_VAL_Y;

	//money widgets
	self->lbl_money.rect.x = p_window_w * HUD_LBL_MONEY_X;
	self->lbl_money.rect.y = p_window_h * HUD_LBL_MONEY_Y;

	self->lbl_money_val.rect.x = (self->lbl_money.rect.x + self->lbl_money.rect.w) +
		(p_window_w * HUD_LBL_MONEY_VAL_X_DIST);
	self->lbl_money_val.rect.y = p_window_h * HUD_LBL_MONEY_VAL_Y;

	//construct btn
	self->btn_construct.rect.x = p_window_w * HUD_BTN_CONSTRUCT_X;
	self->btn_construct.rect.y = p_window_h * HUD_BTN_CONSTRUCT_Y;
	self->btn_construct.rect.w = p_window_w * HUD_BTN_CONSTRUCT_W;
	self->btn_construct.rect.h = p_window_h * HUD_BTN_CONSTRUCT_H;

	//pass button
	self->btn_pass.rect.x = p_window_w * HUD_BTN_PASS_X;
	self->btn_pass.rect.y = p_window_h * HUD_BTN_PASS_Y;
	self->btn_pass.rect.w = p_window_w * HUD_BTN_PASS_W;
	self->btn_pass.rect.h = p_window_h * HUD_BTN_PASS_H;

	//construct widgets
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

	//calc field size
	self->field_width = self->rect_area.w / TOWN_WIDTH;
	self->field_height = self->rect_area.h / TOWN_HEIGHT;

	//calc field rects
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			//full rect
			self->rects_field[x][y].x = self->rect_area.x + (x * self->field_width);
			self->rects_field[x][y].y = self->rect_area.y + (y * self->field_height);
			self->rects_field[x][y].w = self->field_width;
			self->rects_field[x][y].h = self->field_height;

			//content texture rect
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

	//generate texture flip
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			//random
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
			//if field hidden
			if (p_fields_hidden[x][y] == true)
			{
				//assign hidden ground texture
				self->textures_field_ground[x][y] = self->spr_hidden.texture;

				//and null content texture
				self->textures_field_content[x][y] = NULL;
			}
			else
			{
				//assign exposed ground texture
				self->textures_field_ground[x][y] = self->spr_ground.texture;

				//map textures to area content
				switch (p_fields_content[x][y])
				{
					case FIELD_TREE_0:
						self->textures_field_content[x][y] = self->spr_trees[0].texture;
						break;

					case FIELD_TREE_1:
						self->textures_field_content[x][y] = self->spr_trees[1].texture;
						break;

					case FIELD_TREE_2:
						self->textures_field_content[x][y] = self->spr_trees[2].texture;
						break;

					case FIELD_TREE_3:
						self->textures_field_content[x][y] = self->spr_trees[3].texture;
						break;

					case FIELD_TREE_4:
						self->textures_field_content[x][y] = self->spr_trees[4].texture;
						break;

					case FIELD_ADMINISTRATION:
						self->textures_field_content[x][y] = self->spr_hq.texture;
						break;

					case FIELD_CONSTRUCTION:
						self->textures_field_content[x][y] = self->spr_construction.texture;
						break;

					case FIELD_QUARRY:
						self->textures_field_content[x][y] = self->spr_quarry.texture;
						break;

					case FIELD_EMPTY:
						self->textures_field_content[x][y] = NULL;
						break;
				}
			}
		}
	}
}

void Hud_draw(
	struct Hud *self)
{
	//draw fields
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			//draw stored ground texture
			SDL_RenderCopyEx(self->renderer, self->textures_field_ground[x][y], NULL, &self->rects_field[x][y],
				0.0f,
				NULL,
				self->flips_field[x][y]);

			//draw given content texture
			SDL_RenderCopy(
				self->renderer,
				self->textures_field_content[x][y],
				NULL,
				&self->rects_field_content[x][y]);

			//draw field border
			SDL_SetRenderDrawColor(
				self->renderer,
				self->field_border_color.r,
				self->field_border_color.g,
				self->field_border_color.b,
				self->field_border_color.a);
			SDL_RenderDrawRect(self->renderer, &self->rects_field[x][y]);
		}
	} /* draw fields */

	//draw time widgets
	Widget_draw(&self->lbl_time_day, self->renderer);
	Widget_draw(&self->lbl_time_day_val, self->renderer);
	Widget_draw(&self->lbl_time_hour, self->renderer);
	Widget_draw(&self->lbl_time_hour_val, self->renderer);

	//draw money widgets
	Widget_draw(&self->lbl_money, self->renderer);
	Widget_draw(&self->lbl_money_val, self->renderer);

	//construct btn
	Widget_draw(&self->btn_construct, self->renderer);

	//draw pass button
	Widget_draw(&self->btn_pass, self->renderer);

	//draw optional menus
	switch (self->state)
	{
	case HS_NORMAL:
		break;

	case HS_CONSTRUCT:
		Widget_draw(&self->btn_construct_quarry, self->renderer);
		break;
	}

	//draw depending on hover mode
	switch (self->hover_mode)
	{
	case HHM_NONE:
		break;

	case HHM_CONSTRUCT:
        //draw to be constructed building on hover field
        SDL_RenderCopy(self->renderer, self->texture_hover_construct, NULL, &self->rect_hover_construct);
	}
}

SDL_Point Hud_mouse_to_field(struct Hud *self, SDL_Point p_mouse)
{
	SDL_Point result;

	result.x = (p_mouse.x - self->rect_area.x) / self->field_width;
	result.y = (p_mouse.y - self->rect_area.y) / self->field_height;

	return result;
}

void Hud_handle_click(struct Hud *self, SDL_Point p_mouse, struct GameData *p_game_data)
{
	SDL_Point temp;

	switch (self->hover_mode)
	{
	case HHM_NONE:
		//if pass button pressed
		if (SDL_PointInRect(&p_mouse, &self->btn_pass.rect) == true)
		{
			gp_end_round(p_game_data, self);
		}

		//if construct button pressed
		else if (SDL_PointInRect(&p_mouse, &self->btn_construct.rect) == true)
		{
			self->state = HS_CONSTRUCT;
		}

		//if hud has construct state, check construct widgets
		else if (self->state == HS_CONSTRUCT)
		{
			if (SDL_PointInRect(&p_mouse, &self->btn_construct_quarry.rect) == true)
			{
				self->hover_mode = HHM_CONSTRUCT;
				self->hover_construct = FIELD_QUARRY;
				self->texture_hover_construct = self->spr_quarry.texture;
			}
		}
		break;

	case HHM_CONSTRUCT:
		//if field clicked
		if (SDL_PointInRect(&p_mouse, &self->rect_area))
		{
			//construct
			temp = Hud_mouse_to_field(self, p_mouse);
			Hud_set_field(self, p_game_data, temp, self->hover_construct);
		}

		//stop construct mode
		self->hover_mode = HHM_NONE;
		break;
	}
}

void Hud_handle_hover(struct Hud *self, SDL_Point p_mouse)
{
	//react to hover modes
	switch (self->hover_mode)
	{
	case HHM_NONE:
		break;

	case HHM_CONSTRUCT:
		//if mouse in area
		if (SDL_PointInRect(&p_mouse, &self->rect_area) == true)
		{
			//update hover value
			self->hover_field = Hud_mouse_to_field(self, p_mouse);

			self->rect_hover_construct.x =
				self->rects_field_content[self->hover_field.x][self->hover_field.y].x;
			self->rect_hover_construct.y =
				self->rects_field_content[self->hover_field.x][self->hover_field.y].y;
			self->rect_hover_construct.w =
				self->rects_field_content[self->hover_field.x][self->hover_field.y].w;
			self->rect_hover_construct.h =
				self->rects_field_content[self->hover_field.x][self->hover_field.y].h;
		}
	}
}

void Hud_set_field(
	struct Hud *self,
	struct GameData *p_game_data,
	SDL_Point p_field,
	enum Field p_field_content)
{
	//update game
	p_game_data->town->area_content[p_field.x][p_field.y] = p_field_content;

	//update hud
	switch (p_field_content)
	{
	case FIELD_EMPTY:
		self->textures_field_content[p_field.x][p_field.y] = NULL;
		break;

	case FIELD_TREE_0:
		self->textures_field_content[p_field.x][p_field.y] = self->spr_trees[0].texture;
		break;

	case FIELD_TREE_1:
		self->textures_field_content[p_field.x][p_field.y] = self->spr_trees[1].texture;
		break;

	case FIELD_TREE_2:
		self->textures_field_content[p_field.x][p_field.y] = self->spr_trees[2].texture;
		break;

	case FIELD_TREE_3:
		self->textures_field_content[p_field.x][p_field.y] = self->spr_trees[3].texture;
		break;

	case FIELD_TREE_4:
		self->textures_field_content[p_field.x][p_field.y] = self->spr_trees[4].texture;
		break;

	case FIELD_ADMINISTRATION:
		self->textures_field_content[p_field.x][p_field.y] = self->spr_hq.texture;
		break;

	case FIELD_CONSTRUCTION:
		self->textures_field_content[p_field.x][p_field.y] = self->spr_construction.texture;
		break;

	case FIELD_QUARRY:
		self->textures_field_content[p_field.x][p_field.y] = self->spr_quarry.texture;
		break;
	}
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
	Widget_clear(&self->btn_construct);
	Widget_clear(&self->btn_pass);

	Widget_clear(&self->btn_construct_quarry);

	Sprite_clear(&self->spr_ground);
	Sprite_clear(&self->spr_hidden);

	for (uint32_t i = 0; i < TOWN_TREE_VARIETY_COUNT; i++)
		Sprite_clear(&self->spr_trees[i]);

	Sprite_clear(&self->spr_hq);
	Sprite_clear(&self->spr_construction);
	Sprite_clear(&self->spr_quarry);
}
