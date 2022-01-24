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

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"
#include "town.h"
#include "config.h"
#include "sprite.h"
#include "game_commands.h"
#include "game.h"

struct Widget
{
	char text[64];
	SDL_Rect rect;
	struct Sprite sprite;
};

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
	return Sprite_from_text(&self->sprite, p_renderer, self->text, p_font, p_font_color);
}

void Widget_clear(struct Widget *self)
{
	Sprite_clear(&self->sprite);
}

struct Hud
{
	/* widgets */
	struct Widget time_day_label;
	struct Widget time_day;
	struct Widget time_hour_label;
	struct Widget time_hour;
	struct Widget hover_label;
	struct Widget hover_x;
	struct Widget hover_y;

	/* graphical data for area and fields */
	uint32_t field_width;
	uint32_t field_height;
	SDL_Rect rect_area;
	SDL_Rect rects_field[TOWN_WIDTH][TOWN_HEIGHT];
	SDL_Rect rects_field_content[TOWN_WIDTH][TOWN_HEIGHT];
	SDL_Texture* textures_field_ground[TOWN_WIDTH][TOWN_HEIGHT];
	SDL_Texture* textures_field_content[TOWN_WIDTH][TOWN_HEIGHT];
	SDL_RendererFlip flips_field[TOWN_WIDTH][TOWN_HEIGHT];

	/* shared sprites */
	struct Sprite spr_ground;
	struct Sprite spr_hidden;
	struct Sprite spr_hq;
	struct Sprite spr_trees[TOWN_TREE_VARIETY_COUNT];
};

void Hud_new(struct Hud *self)
{
	//init widgets
	Widget_new(&self->time_day_label);
	Widget_new(&self->time_day);
	Widget_new(&self->time_hour_label);
	Widget_new(&self->time_hour);
	Widget_new(&self->hover_label);
	Widget_new(&self->hover_x);
	Widget_new(&self->hover_y);

	//init sprites
	Sprite_new(&self->spr_ground);
	Sprite_new(&self->spr_hidden);
	Sprite_new(&self->spr_hq);

	for (uint32_t i = 0; i < TOWN_TREE_VARIETY_COUNT; i++)
		Sprite_new(&self->spr_trees[i]);

	//set widget texts
	strcpy(self->hover_label.text, UI_TEXT_HOVER_LABEL);
	strcpy(self->hover_x.text, "0");
	strcpy(self->hover_y.text, "0");
	strcpy(self->time_day_label.text, HUD_TIME_DAY_LABEL_TEXT);
	strcpy(self->time_hour_label.text, HUD_TIME_HOUR_LABEL_TEXT);
}

void Hud_update_time(struct Hud *self, uint32_t p_round)
{
	//create widget strings
	sprintf(self->time_day.text, "%u", (p_round / 24));
	sprintf(self->time_hour.text, "%02u:00", p_round);
}

int32_t Hud_generate_widget_sprites(
	struct Hud *self,
	SDL_Renderer *p_renderer,
	TTF_Font *p_font,
	SDL_Color p_font_color)
{
	//create widget sprites
	if ((Widget_generate_sprite(&self->time_day_label, p_renderer, p_font, p_font_color) != 0) ||
		(Widget_generate_sprite(&self->time_day, p_renderer, p_font, p_font_color) != 0) ||
		(Widget_generate_sprite(&self->time_hour_label, p_renderer, p_font, p_font_color) != 0) ||
		(Widget_generate_sprite(&self->time_hour, p_renderer, p_font, p_font_color) != 0) ||
		(Widget_generate_sprite(&self->hover_label, p_renderer, p_font, p_font_color) != 0) ||
		(Widget_generate_sprite(&self->hover_x, p_renderer, p_font, p_font_color) != 0) ||
		(Widget_generate_sprite(&self->hover_y, p_renderer, p_font, p_font_color) != 0))
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
	//calc time widgets
	self->time_day_label.rect.x = p_window_w * HUD_TIME_DAY_LABEL_X;
	self->time_day_label.rect.y = p_window_h * HUD_TIME_DAY_LABEL_Y;
	self->time_day_label.rect.w = p_window_w * HUD_TIME_DAY_LABEL_W;
	self->time_day_label.rect.h = p_window_h * HUD_TIME_DAY_LABEL_H;

	/* x is calculated with distance from previous widget in mind */
	self->time_day.rect.x = p_window_w * (HUD_TIME_DAY_LABEL_X + HUD_TIME_DAY_LABEL_W + HUD_TIME_DAY_X_DIST);
	self->time_day.rect.y = p_window_h * HUD_TIME_DAY_Y;
	self->time_day.rect.w = p_window_w * HUD_TIME_DAY_W;
	self->time_day.rect.h = p_window_h * HUD_TIME_DAY_H;

	/* x is calculated with distance from previous widget in mind */
	self->time_hour_label.rect.x = self->time_day.rect.x +
		(p_window_w * (HUD_TIME_DAY_W + HUD_TIME_HOUR_LABEL_X_DIST));
	self->time_hour_label.rect.y = p_window_h * HUD_TIME_HOUR_LABEL_Y;
	self->time_hour_label.rect.w = p_window_w * HUD_TIME_HOUR_LABEL_W;
	self->time_hour_label.rect.h = p_window_h * HUD_TIME_HOUR_LABEL_H;
	
	/* x is calculated with distance from previous widget in mind */
	self->time_hour.rect.x = self->time_hour_label.rect.x +
		(p_window_w * (HUD_TIME_HOUR_LABEL_W + HUD_TIME_HOUR_X_DIST));
	self->time_hour.rect.y = p_window_h * HUD_TIME_HOUR_Y;
	self->time_hour.rect.w = p_window_w * HUD_TIME_HOUR_W;
	self->time_hour.rect.h = p_window_h * HUD_TIME_HOUR_H;

	//calc hover widgets
	self->hover_label.rect.x = p_window_w * UI_TEXT_HOVER_LABEL_X;
	self->hover_label.rect.y = p_window_h * UI_TEXT_HOVER_LABEL_Y;
	self->hover_label.rect.w = p_window_w * UI_TEXT_HOVER_LABEL_W;
	self->hover_label.rect.h = p_window_h * UI_TEXT_HOVER_LABEL_H;

	self->hover_x.rect.x = p_window_w * UI_TEXT_HOVER_X_X;
	self->hover_x.rect.y = p_window_h * UI_TEXT_HOVER_X_Y;
	self->hover_x.rect.w = p_window_w * UI_TEXT_HOVER_X_W;
	self->hover_x.rect.h = p_window_h * UI_TEXT_HOVER_X_H;

	self->hover_y.rect.x = p_window_w * UI_TEXT_HOVER_Y_X;
	self->hover_y.rect.y = p_window_h * UI_TEXT_HOVER_Y_Y;
	self->hover_y.rect.w = p_window_w * UI_TEXT_HOVER_Y_W;
	self->hover_y.rect.h = p_window_h * UI_TEXT_HOVER_Y_H;

	/* calculate area pos and size
		-as wide as high
		-stay on top and right
	*/
	self->rect_area.h = p_window_h * UI_AREA_H;
	self->rect_area.h -= (self->rect_area.h % TOWN_HEIGHT);  /* remove modulo, to fix mouse hitbox */
	self->rect_area.w = self->rect_area.h;
	self->rect_area.y = p_window_h * UI_AREA_Y;
	self->rect_area.x = (p_window_w * UI_AREA_X2) - self->rect_area.w;

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
			self->rects_field_content[x][y].w = self->rects_field[x][y].w * UI_FIELD_CONTENT_SIZE;
			self->rects_field_content[x][y].h = self->rects_field[x][y].h * UI_FIELD_CONTENT_SIZE;
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
	SDL_Renderer *p_renderer,
	uint8_t p_field_border_red,
	uint8_t p_field_border_green,
	uint8_t p_field_border_blue,
	uint8_t p_field_border_alpha)
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
				p_field_border_red,
				p_field_border_green,
				p_field_border_blue,
				p_field_border_alpha);
			SDL_RenderDrawRect(p_renderer, &self->rects_field[x][y]);
		}
	}

	//draw time widgets
	SDL_RenderCopy(p_renderer, self->time_day_label.sprite.texture, NULL, &self->time_day_label.rect);
	SDL_RenderCopy(p_renderer, self->time_day.sprite.texture, NULL, &self->time_day.rect);
	SDL_RenderCopy(p_renderer, self->time_hour_label.sprite.texture, NULL, &self->time_hour_label.rect);
	SDL_RenderCopy(p_renderer, self->time_hour.sprite.texture, NULL, &self->time_hour.rect);
	
	//if mouse hovers on area, draw hover widgets
	if (self->hover_x.text[0] != '\0' && self->hover_y.text[0] != '\0')
	{
		SDL_RenderCopy(p_renderer, self->hover_label.sprite.texture, NULL, &self->hover_label.rect);
		SDL_RenderCopy(p_renderer, self->hover_x.sprite.texture, NULL, &self->hover_x.rect);
		SDL_RenderCopy(p_renderer, self->hover_y.sprite.texture, NULL, &self->hover_y.rect);
	}
}

void Hud_update_hover(struct Hud *self, SDL_Renderer *p_renderer, TTF_Font *p_font, SDL_Color p_font_color)
{
	SDL_Point mouse;
	
	//if mouse hovers in field
	SDL_GetMouseState(&mouse.x, &mouse.y);

	if (SDL_PointInRect(&mouse, &self->rect_area) == true)
	{
		//calc in which
		sprintf(self->hover_x.text, "%u", ((mouse.x - self->rect_area.x) / self->field_width));
		sprintf(self->hover_y.text, "%u", ((mouse.y - self->rect_area.y) / self->field_height));

		//reset sprites
		Widget_generate_sprite(&self->hover_x, p_renderer, p_font, p_font_color);
		Widget_generate_sprite(&self->hover_y, p_renderer, p_font, p_font_color);
	}
	else
	{
		self->hover_x.text[0] = '\0';
	}
}

void Hud_clear(struct Hud *self)
{
	Widget_clear(&self->time_day_label);
	Widget_clear(&self->time_day);
	Widget_clear(&self->time_hour_label);
	Widget_clear(&self->time_hour);
	Widget_clear(&self->hover_label);
	Widget_clear(&self->hover_x);
	Widget_clear(&self->hover_y);

	Sprite_clear(&self->spr_ground);
	Sprite_clear(&self->spr_hidden);
	Sprite_clear(&self->spr_hq);

	for (uint32_t i = 0; i < TOWN_TREE_VARIETY_COUNT; i++)
		Sprite_clear(&self->spr_trees[i]);
}

void exit_game(
	enum GameResponse *p_response,
	SDL_Window *p_window,
	SDL_Renderer *p_renderer,
	TTF_Font *p_font,
	struct Hud *p_hud)
{
	//send stop response
	*p_response = GRSP_STOPPED;

	//clear hud
	Hud_clear(p_hud);

	//free font
	TTF_CloseFont(p_font);

	//if given, destroy window and renderer
	if (p_window != NULL)
	{
		SDL_DestroyWindow(p_window);
	}

	if (p_renderer != NULL)
	{
		SDL_DestroyRenderer(p_renderer);
	}

	//quit sdl ttf
	TTF_Quit();

	//quit sdl
	SDL_Quit();
}

int32_t gfx_game(void* p_data)
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	TTF_Font *font;

	bool active = true;
	uint32_t ts_now = 0;
	uint32_t ts_render = 0;

	struct GameData* data = (struct GameData*) (p_data);

	SDL_Event event;
	int32_t border_t, border_l;
	int32_t window_x, window_y;
	int32_t window_w, window_h;

	SDL_Color font_color;
	struct Hud hud;

	struct Sprite spr_icon;

	//init values
	Hud_new(&hud);

	//init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf(MSG_ERR_SDL_INIT, SDL_GetError());
		exit_game(&data->rsp, NULL, NULL, NULL, NULL);
		return 1;
	}

	//create window
	window = SDL_CreateWindow(
		data->town_name,
		data->cfg->gfx_window_x,
		data->cfg->gfx_window_y,
		data->cfg->gfx_window_w,
		data->cfg->gfx_window_h,
		(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

	if (window == NULL)
	{
		printf(MSG_ERR_SDL_WINDOW, SDL_GetError());
		exit_game(&data->rsp, NULL, NULL, NULL, NULL);
		return 2;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		printf(MSG_ERR_SDL_RENDERER, SDL_GetError());
		exit_game(&data->rsp, window, NULL, NULL, NULL);
		return 3;
	}

	//set alpha channel
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//init ttf
	if (TTF_Init() != 0)
	{
		printf(MSG_ERR_TTF_INIT);
		exit_game(&data->rsp, window, renderer, NULL, NULL);
		return 4;
	}

	//load font
	font = TTF_OpenFont(data->cfg->path_font, UI_FONT_SIZE);

	if (font == NULL)
	{
		printf(MSG_ERR_FONT_LOAD, data->cfg->path_font);
		exit_game(&data->rsp, window, renderer, NULL, NULL);
		return 5;
	}

	//set font color
	font_color.r = data->cfg->font_red;
	font_color.g = data->cfg->font_green;
	font_color.b = data->cfg->font_blue;
	font_color.a = data->cfg->font_alpha;

	//make all sprites
	Hud_update_time(&hud, data->town->round);
	
	if ((Sprite_from_image(&spr_icon, renderer, PATH_TEXTURE_ICON) != 0) ||
		(Hud_load_sprites(&hud, renderer) != 0) ||
		(Hud_generate_widget_sprites(&hud, renderer, font, font_color) != 0))
	{
		exit_game(&data->rsp, window, renderer, font, &hud);
		return 6;
	}

	//calculate ui sizes and positions
	Hud_calc(&hud, data->cfg->gfx_window_w,	data->cfg->gfx_window_h);
	Hud_generate_flips(&hud);
	Hud_map_textures(&hud, data->town->area_hidden, data->town->area_content);

	//set window icon, and clear icon sprite
	SDL_SetWindowIcon(window, spr_icon.surface);
	Sprite_clear(&spr_icon);

	//send signal (init successful)
	data->rsp = GRSP_INIT;

	//mainloop
	while (active)
	{
		if (ts_now > (ts_render + (1000.0f / data->cfg->gfx_framerate)))
		{
			//clear screen
			SDL_SetRenderDrawColor(renderer, data->cfg->bg_red, data->cfg->bg_green, data->cfg->bg_blue, 255);
			SDL_RenderClear(renderer);

			//draw hud
			Hud_draw(
				&hud,
				renderer,
				data->cfg->field_border_red,
				data->cfg->field_border_green,
				data->cfg->field_border_blue,
				data->cfg->field_border_alpha);

			//show image, save time
			SDL_RenderPresent(renderer);
			ts_render = ts_now;
		}

		//update time
		ts_now = SDL_GetTicks();

		//handle terminal command-signals
		switch (data->cmd)
		{
		case GCMD_NONE:
			break;

		case GCMD_STOP:
			active = false;
			break;
		}

		//handle sdl-events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				Hud_update_hover(&hud, renderer, font, font_color);
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					//set config window size
					SDL_GetWindowSize(window, &window_w, &window_h);

					data->cfg->gfx_window_w = window_w;
					data->cfg->gfx_window_h = window_h;

					//recalc ui sizes and pos
					Hud_calc(
						&hud,
						data->cfg->gfx_window_w,
						data->cfg->gfx_window_h);
				}
				else if (event.window.event == SDL_WINDOWEVENT_MOVED)
				{
					//set config window pos
					SDL_GetWindowBordersSize(window, &border_t, &border_l, NULL, NULL);
					SDL_GetWindowPosition(window, &window_x, &window_y);

					data->cfg->gfx_window_x = window_x - border_l;
					data->cfg->gfx_window_y = window_y - border_t;
				}
				break;

			case SDL_QUIT:
				active = false;
				break;
			}
		}
	}

	//save config
	save_config(data->cfg);

	//quit routine
	exit_game(&data->rsp, window, renderer, font, &hud);

	return 0;
}

int32_t terminal_game(struct GameData* data)
{
	bool active = true;		/* mainloop active */
	bool enabled = false;	/* describes if the console accepts input */
	char input[TRM_MAX_IN_LEN];
	char *split1, *split2;
	uint32_t argc;
	char argv[TRM_MAX_ARGS][TRM_MAX_ARG_LEN];

	//mainloop
	while (active)
	{
		//handle responses
		switch (data->rsp)
		{
		case GRSP_NONE:
			break;

		case GRSP_INIT:
			enabled = true;
			break;

		case GRSP_STOPPED:
			enabled = false;
			active = false;
			break;
		}

		//if enabled, handle input
		if (enabled)
		{
			//reset argc
			argc = 0;

			//provide somy eyecandy for input, take input
			printf("\n > ");
			fgets(input, TRM_MAX_IN_LEN, stdin);

			//replace linebreak with \0
			split1 = strchr(input, '\n');
			*split1 = '\0';

			//split
			split1 = &input[0];

			for (uint32_t i = 0; i < TRM_MAX_ARGS; i++)
			{
				//increment arg counter
				argc++;

				//split
				split2 = strchr(split1, ' ');

				argv[i][0] = '\0';
				strncat(argv[i], split1, (size_t) (split2 - split1));

				//if not end, move pointers, else stop
				if (split2 != NULL)
					split1 = split2 + 1;
				else
					break;
			}

			//parse
			if (strcmp(argv[0], GM_CMD_HELP) == 0)
			{
				//check arg max
				if (argc > 1)
				{
					printf(MSG_WARN_ARG_MAX);
				}

				printf(HELP_TEXT_INGAME);
			}
			else if (strcmp(argv[0], GM_CMD_SAVE) == 0)
			{
				//check arg max
				if (argc > 1)
				{
					printf(MSG_WARN_ARG_MAX);
				}

				gm_cmd_save(data->town_name, data->town);
			}
			else if (strcmp(argv[0], GM_CMD_SAVE_AS) == 0)
			{
				//check arg min
				if (argc < 2)
				{
					printf(MSG_ERR_ARG_MIN);
					break;
				}

				//check arg max
				else if (argc > 2)
				{
					printf(MSG_WARN_ARG_MAX);
				}

				gm_cmd_save_as(argv[1], data->town);
			}
			else if (strcmp(argv[0], GM_CMD_EXIT) == 0)
			{
				//check arg max
				if (argc > 1)
				{
					printf(MSG_WARN_ARG_MAX);
				}

				data->cmd = GCMD_STOP;
				enabled = false;
				active = false;
			}
			else if (strcmp(argv[0], GM_CMD_SHOW_CONFIG) == 0)
			{
				//check arg max
				if (argc > 1)
				{
					printf(MSG_WARN_ARG_MAX);
				}

				gm_cmd_show_config(data->cfg);
			}
			else if (strcmp(argv[0], GM_CMD_SET) == 0)
			{
				//check arg min
				if (argc < 3)
				{
					printf(MSG_ERR_ARG_MIN);
					break;
				}

				//check arg max
				else if (argc > 3)
				{
					printf(MSG_WARN_ARG_MAX);
				}

				gm_cmd_set(data->cfg, argv[1], argv[2]);
			}
			else
			{
				printf(MSG_ERR_UNKNOWN_COMMAND);
			}
		}
	}

	return 0;
}
