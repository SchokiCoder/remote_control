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
#include "hud.h"

int32_t Hud_new(struct Hud *self, char *p_path_font)
{
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
	Widget_new(&self->lbl_hover);
	Widget_new(&self->lbl_hover_x_val);
	Widget_new(&self->lbl_hover_y_val);
	Widget_new(&self->btn_pass);

	//init sprites
	Sprite_new(&self->spr_ground);
	Sprite_new(&self->spr_hidden);
	Sprite_new(&self->spr_hq);

	for (uint32_t i = 0; i < TOWN_TREE_VARIETY_COUNT; i++)
		Sprite_new(&self->spr_trees[i]);

	//set widget texts
	strcpy(self->lbl_time_day.text, HUD_LBL_TIME_DAY_TEXT);
	strcpy(self->lbl_time_hour.text, HUD_LBL_TIME_HOUR_TEXT);
	strcpy(self->lbl_money.text, HUD_LBL_MONEY_TEXT);
	strcpy(self->lbl_money_val.text, "0");
	strcpy(self->lbl_hover.text, HUD_LBL_HOVER_TEXT);
	strcpy(self->lbl_hover_x_val.text, "0");
	strcpy(self->lbl_hover_y_val.text, "0");
	strcpy(self->btn_pass.text, HUD_BTN_PASS_TEXT);

	return 0;
}

void Hud_update_time(
	struct Hud *self,
	uint32_t p_round,
	SDL_Renderer *p_renderer)
{
	//create widget strings
	sprintf(self->lbl_time_day_val.text, "%u", (p_round / 24));
	sprintf(self->lbl_time_hour_val.text, "%02u:00", p_round);

	//gen widgets
	Widget_generate_sprite(&self->lbl_time_day_val, p_renderer, self->font, self->font_color);
	Widget_generate_sprite(&self->lbl_time_hour_val, p_renderer, self->font, self->font_color);
}

void Hud_update_money(
	struct Hud *self,
	uint32_t p_money,
	SDL_Renderer *p_renderer)
{
	//create widget strings
	sprintf(self->lbl_money_val.text, "%u", p_money);

	//gen widget
	Widget_generate_sprite(&self->lbl_money_val, p_renderer, self->font, self->font_color);
}

int32_t Hud_init_widgets(
	struct Hud *self,
	SDL_Renderer *p_renderer)
{
	//create sprites of widgets, that wont change their sprite
	if ((Widget_generate_sprite(&self->lbl_time_day, p_renderer, self->font, self->font_color) != 0) ||
		(Widget_generate_sprite(&self->lbl_time_hour, p_renderer, self->font, self->font_color) != 0) ||
		(Widget_generate_sprite(&self->lbl_money, p_renderer, self->font, self->font_color) != 0) ||
		(Widget_generate_sprite(&self->lbl_hover, p_renderer, self->font, self->font_color) != 0) ||
		(Widget_generate_sprite(&self->btn_pass, p_renderer, self->font, self->font_color) != 0))
	{
		return 1;
	}

	return 0;
}

int32_t Hud_load_sprites(struct Hud *self, SDL_Renderer *p_renderer)
{
	//load sprites
	if ((Sprite_from_image(&self->spr_ground, p_renderer, PATH_TEXTURE_GROUND) != 0) ||
		(Sprite_from_image(&self->spr_hidden, p_renderer, PATH_TEXTURE_HIDDEN) != 0) ||
		(Sprite_from_image(&self->spr_hq, p_renderer, PATH_TEXTURE_HQ) != 0) ||
		(Sprite_from_image(&self->spr_trees[0], p_renderer, PATH_TEXTURE_TREE_0) != 0) ||
		(Sprite_from_image(&self->spr_trees[1], p_renderer, PATH_TEXTURE_TREE_1) != 0) ||
		(Sprite_from_image(&self->spr_trees[2], p_renderer, PATH_TEXTURE_TREE_2) != 0) ||
		(Sprite_from_image(&self->spr_trees[3], p_renderer, PATH_TEXTURE_TREE_3) != 0) ||
		(Sprite_from_image(&self->spr_trees[4], p_renderer, PATH_TEXTURE_TREE_4) != 0))
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

	//calc hover widgets
	self->lbl_hover.rect.x = p_window_w * HUD_LBL_HOVER_X;
	self->lbl_hover.rect.y = p_window_h * HUD_LBL_HOVER_Y;

	self->lbl_hover_x_val.rect.x = (self->lbl_hover.rect.x + self->lbl_hover.rect.w) +
		(p_window_w * HUD_LBL_HOVER_X_VAL_X_DIST);
	self->lbl_hover_x_val.rect.y = p_window_h * HUD_LBL_HOVER_X_VAL_Y;

	self->lbl_hover_y_val.rect.x = (self->lbl_hover_x_val.rect.x + self->lbl_hover_x_val.rect.w) +
		(p_window_w * HUD_LBL_HOVER_Y_VAL_X_DIST);
	self->lbl_hover_y_val.rect.y = p_window_h * HUD_LBL_HOVER_Y_VAL_Y;

	//calc pass button
	self->btn_pass.rect.x = p_window_w * HUD_BTN_PASS_X;
	self->btn_pass.rect.y = p_window_h * HUD_BTN_PASS_Y;
	self->btn_pass.rect.w = p_window_w * HUD_BTN_PASS_W;
	self->btn_pass.rect.h = p_window_h * HUD_BTN_PASS_H;

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
					case FIELD_ADMINISTRATION:
						self->textures_field_content[x][y] = self->spr_hq.texture;
						break;

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

					case FIELD_EMPTY:
						self->textures_field_content[x][y] = NULL;
						break;
				}
			}
		}
	}
}

void Hud_draw(
	struct Hud *self,
	SDL_Renderer *p_renderer)
{
	//draw fields
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			//draw stored ground texture
			SDL_RenderCopyEx(p_renderer, self->textures_field_ground[x][y], NULL, &self->rects_field[x][y],
				0.0f,
				NULL,
				self->flips_field[x][y]);

			//draw given content texture
			SDL_RenderCopy(
				p_renderer,
				self->textures_field_content[x][y],
				NULL,
				&self->rects_field_content[x][y]);


			//draw field border
			SDL_SetRenderDrawColor(
				p_renderer,
				self->field_border_color.r,
				self->field_border_color.g,
				self->field_border_color.b,
				self->field_border_color.a);
			SDL_RenderDrawRect(p_renderer, &self->rects_field[x][y]);
		}
	}

	//draw time widgets
	Widget_draw(&self->lbl_time_day, p_renderer);
	Widget_draw(&self->lbl_time_day_val, p_renderer);
	Widget_draw(&self->lbl_time_hour, p_renderer);
	Widget_draw(&self->lbl_time_hour_val, p_renderer);

	//draw money widgets
	Widget_draw(&self->lbl_money, p_renderer);
	Widget_draw(&self->lbl_money_val, p_renderer);

	//draw pass button
	Widget_draw(&self->btn_pass, p_renderer);

	//if mouse hovers on area, draw hover widgets
	if (self->lbl_hover_x_val.text[0] != '\0' && self->lbl_hover_y_val.text[0] != '\0')
	{
		Widget_draw(&self->lbl_hover, p_renderer);
		Widget_draw(&self->lbl_hover_x_val, p_renderer);
		Widget_draw(&self->lbl_hover_y_val, p_renderer);
	}
}

void Hud_update_hover(struct Hud *self, SDL_Renderer *p_renderer)
{
	SDL_Point mouse;

	//if mouse hovers in field
	SDL_GetMouseState(&mouse.x, &mouse.y);

	if (SDL_PointInRect(&mouse, &self->rect_area) == true)
	{
		//calc in which
		sprintf(self->lbl_hover_x_val.text, "%u", ((mouse.x - self->rect_area.x) / self->field_width));
		sprintf(self->lbl_hover_y_val.text, "%u", ((mouse.y - self->rect_area.y) / self->field_height));

		//reset sprites
		Widget_generate_sprite(&self->lbl_hover_x_val, p_renderer, self->font, self->font_color);
		Widget_generate_sprite(&self->lbl_hover_y_val, p_renderer, self->font, self->font_color);
	}
	else
	{
		self->lbl_hover_x_val.text[0] = '\0';
	}
}

void Hud_clear(struct Hud *self)
{
	TTF_CloseFont(self->font);

	Widget_clear(&self->lbl_time_day);
	Widget_clear(&self->lbl_time_day_val);
	Widget_clear(&self->lbl_time_hour);
	Widget_clear(&self->lbl_time_hour_val);
	Widget_clear(&self->lbl_hover);
	Widget_clear(&self->lbl_hover_x_val);
	Widget_clear(&self->lbl_hover_y_val);

	Sprite_clear(&self->spr_ground);
	Sprite_clear(&self->spr_hidden);
	Sprite_clear(&self->spr_hq);

	for (uint32_t i = 0; i < TOWN_TREE_VARIETY_COUNT; i++)
		Sprite_clear(&self->spr_trees[i]);
}
