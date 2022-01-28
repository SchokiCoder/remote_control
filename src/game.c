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
#include "definitions/def_commands.h"
#include "definitions/def_messages.h"
#include "definitions/def_files.h"
#include "definitions/def_admins.h"
#include "town.h"
#include "config.h"
#include "sprite.h"
#include "game_commands.h"
#include "hud.h"
#include "game.h"

/* technical functions */
void exit_game(
	SDL_Window *p_window,
	SDL_Renderer *p_renderer,
	struct Hud *p_hud)
{
	//clear hud
	Hud_clear(p_hud);

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

/* gameplay functions */
void gp_end_round(struct GameData *p_game_data, struct Hud *p_hud)
{
	uint32_t cost = 0;

	//get running cost for admin and hq
	cost += p_game_data->admin_salary;
	cost += COST_HQ;

	/* per building
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			switch (p_town->field[x][y])
			{
			}
		}
	}*/

	//if cost is not higher than current money
	if (cost < p_game_data->town->money)
	{
		//subtract running cost from players money
		p_game_data->town->money -= cost;
	}
	else
	{
		//gameover
		p_game_data->game_state = GS_FAILURE_COST;
	}

	//increment time
	p_game_data->town->round++;

	//update hud
	Hud_update_time(p_hud, p_game_data->town->round);
	Hud_update_money(p_hud, p_game_data->town->money);
}

int32_t gp_main(struct GameData *p_game_data)
{
	struct GameData *game_data = p_game_data;	/* just for now out of laziness */
												/* TODO remove later and use actual parameter */

	SDL_Window *window;
	SDL_Renderer *renderer;

	uint32_t ts_now = 0;
	uint32_t ts_render = 0;

	SDL_Event event;
	int32_t border_t, border_l;
	int32_t window_x, window_y;
	int32_t window_w, window_h;
	SDL_Point mouse;

	struct Hud hud;

	struct Sprite spr_icon;

	//init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf(MSG_ERR_SDL_INIT, SDL_GetError());
		exit_game(NULL, NULL, NULL);
		return 1;
	}

	//create window
	window = SDL_CreateWindow(
		game_data->town_name,
		game_data->cfg->gfx_window_x,
		game_data->cfg->gfx_window_y,
		game_data->cfg->gfx_window_w,
		game_data->cfg->gfx_window_h,
		(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

	if (window == NULL)
	{
		printf(MSG_ERR_SDL_WINDOW, SDL_GetError());
		exit_game(NULL, NULL, NULL);
		return 2;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		printf(MSG_ERR_SDL_RENDERER, SDL_GetError());
		exit_game(window, NULL, NULL);
		return 3;
	}

	//set alpha channel
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//init ttf
	if (TTF_Init() != 0)
	{
		printf(MSG_ERR_TTF_INIT);
		exit_game(window, renderer, NULL);
		return 4;
	}

	//init hud
	if (Hud_new(&hud, renderer, game_data->cfg->path_font) != 0)
	{
		exit_game(window, renderer, &hud);
		return 5;
	}

	//set font color
	hud.font_color.r = game_data->cfg->font_red;
	hud.font_color.g = game_data->cfg->font_green;
	hud.font_color.b = game_data->cfg->font_blue;
	hud.font_color.a = game_data->cfg->font_alpha;

	//update some widgets
	Hud_update_time(&hud, game_data->town->round);
	Hud_update_money(&hud, game_data->town->money);

	//make all sprites
	if ((Sprite_from_image(&spr_icon, renderer, PATH_TEXTURE_ICON) != 0) ||
		(Hud_load_sprites(&hud) != 0) ||
		(Hud_init_widgets(&hud) != 0))
	{
		exit_game(window, renderer, &hud);
		return 6;
	}

	//calculate ui sizes and positions
	Hud_calc(&hud, game_data->cfg->gfx_window_w, game_data->cfg->gfx_window_h);
	Hud_generate_flips(&hud);
	Hud_map_textures(&hud, game_data->town->area_hidden, game_data->town->area_content);

	//set window icon, and clear icon sprite
	SDL_SetWindowIcon(window, spr_icon.surface);
	Sprite_clear(&spr_icon);

	//mainloop
	while (game_data->game_state == GS_ACTIVE)
	{
		if (ts_now > (ts_render + (1000.0f / game_data->cfg->gfx_framerate)))
		{
			//clear screen
			SDL_SetRenderDrawColor(renderer,
				game_data->cfg->bg_red,
				game_data->cfg->bg_green,
				game_data->cfg->bg_blue,
				255);
			SDL_RenderClear(renderer);

			//draw hud
			Hud_draw(&hud);

			//show image, save time
			SDL_RenderPresent(renderer);
			ts_render = ts_now;
		}

		//update time
		ts_now = SDL_GetTicks();

		//update mouse coords
		SDL_GetMouseState(&mouse.x, &mouse.y);

		//handle sdl-events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEBUTTONUP:
				Hud_handle_click(&hud, mouse, game_data);
				break;

			case SDL_MOUSEMOTION:
				Hud_handle_hover(&hud, mouse);
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					//set config window size
					SDL_GetWindowSize(window, &window_w, &window_h);

					game_data->cfg->gfx_window_w = window_w;
					game_data->cfg->gfx_window_h = window_h;

					//recalc ui sizes and pos
					Hud_calc(
						&hud,
						game_data->cfg->gfx_window_w,
						game_data->cfg->gfx_window_h);
				}
				else if (event.window.event == SDL_WINDOWEVENT_MOVED)
				{
					//set config window pos
					SDL_GetWindowBordersSize(window, &border_t, &border_l, NULL, NULL);
					SDL_GetWindowPosition(window, &window_x, &window_y);

					game_data->cfg->gfx_window_x = window_x - border_l;
					game_data->cfg->gfx_window_y = window_y - border_t;
				}
				break;

			case SDL_QUIT:
				game_data->game_state = GS_CLOSE;
				break;
			}
		}
	}

	//save config
	save_config(game_data->cfg);

	//quit routine
	exit_game(window, renderer, &hud);

	return 0;
}
