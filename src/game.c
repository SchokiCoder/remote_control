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
#include <SGUI_sprite.h>

#ifdef _DEBUG
#include <time.h>
#endif

#include "commands.h"
#include "messages.h"
#include "path.h"
#include "admins.h"
#include "town.h"
#include "config.h"
#include "hud.h"
#include "game.h"

void Game_exit( SDL_Window *window, SDL_Renderer *renderer, Hud *hud )
{
	/* clear hud */
	Hud_clear(hud);

	/* if given, destroy window and renderer */
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	if (renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}

	/* quit sdl ttf */
	TTF_Quit();

	/* quit sdl */
	SDL_Quit();
}

void Game_end_round( Game *game, Hud *hud )
{
	uint32_t cost = 0;

	/* get running cost for admin */
	cost += DATA_ADMINS[game->town->admin_id].salary;

	/* for each building, add cost */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			cost += DATA_FIELDS[game->town->field[x][y]].running_cost;
		}
	}

	/* if cost is not higher than current money */
	if (cost < game->town->money)
	{
		/* subtract running cost from players money */
		game->town->money -= cost;
	}
	else
	{
		/* gameover */
		game->game_state = GS_FAILURE_COST;
		return;
	}

	/* for all constructions */
	for (uint32_t i = 0; i < game->town->construction_count; i++)
	{
		/* increment progress */
		game->town->constructions[i].progress++;

		/* if building is done */
		if (game->town->constructions[i].progress >=
			DATA_FIELDS[game->town->constructions[i].field].construction_time)
		{
            /* set field to actual building */
            game->town->field
            	[game->town->constructions[i].coords.x]
            	[game->town->constructions[i].coords.y]
            	= game->town->constructions[i].field;

            /* update hud */
            Hud_set_field(
            	hud,
            	game->town->constructions[game->town->construction_count - 1].coords,
            	hud->spr_fields[game->town->constructions[i].field].texture);

            /* remove entry from construction list */
            Town_construction_list_remove(game->town, i);
		}
	}

	/* increment time */
	game->town->round++;

	/* save file */
	Town_save(game->town, game->town_name);

	/* update hud */
	Hud_update_time(hud, game->town->round);
	Hud_update_money(hud, game->town->money);
}

void Game_construct( Game *game, SDL_Point coords, Field field, Hud *hud )
{
	/* change field */
	game->town->field[coords.x][coords.y] = FIELD_CONSTRUCTION;

	/* add building to construction list */
	game->town->construction_count++;
	game->town->constructions[game->town->construction_count - 1].field = field;
	game->town->constructions[game->town->construction_count - 1].coords = coords;
	game->town->constructions[game->town->construction_count - 1].progress = 0;

	/* subtract cost of building */
	game->town->money -= DATA_FIELDS[field].construction_cost;

	/* update hud */
	Hud_update_money(hud, game->town->money);
}

void Game_spawn_merc( Game *game, Hud *hud, Mercenary merc )
{
	// if merc already exists, stop
	for (uint32_t i = 0; i < game->town->merc_count; i++)
	{
		if (merc.id == game->town->mercs[i].id)
		{
			return;
		}
	}

	// add to merc list
	game->town->merc_count++;
	game->town->mercs[game->town->merc_count - 1] = merc;

    // update town
    game->town->field[merc.coords.x][merc.coords.y] = FIELD_MERC;

    // update hud
    Hud_set_field(hud, merc.coords, hud->spr_merc_base.texture);
}

int32_t Game_main( Game *game )
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	Hud hud;
	SGUI_Sprite spr_icon;

#ifdef _DEBUG
	// init random gen
	srand(time(NULL));
#endif // _DEBUG

	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf(MSG_ERR_SDL_INIT, MSG_ERR, SDL_GetError());
		Game_exit(NULL, NULL, NULL);
		return 1;
	}

	// create window
	window = SDL_CreateWindow(
		game->town_name,
		game->cfg->gfx_window_x,
		game->cfg->gfx_window_y,
		game->cfg->gfx_window_w,
		game->cfg->gfx_window_h,
		(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

	if (window == NULL)
	{
		printf(MSG_ERR_SDL_WINDOW, MSG_ERR, SDL_GetError());
		Game_exit(NULL, NULL, NULL);
		return 2;
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		printf(MSG_ERR_SDL_RENDERER, MSG_ERR, SDL_GetError());
		Game_exit(window, NULL, NULL);
		return 3;
	}

	// set alpha channel
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// init ttf
	if (TTF_Init() != 0)
	{
		printf(MSG_ERR_TTF_INIT, MSG_ERR);
		Game_exit(window, renderer, NULL);
		return 4;
	}

	// init hud
	hud = Hud_new(renderer, game->cfg->path_font);

	if (hud.invalid)
	{
		Game_exit(window, renderer, &hud);
		return 5;
	}

	// update some widgets
	Hud_update_time(&hud, game->town->round);
	Hud_update_money(&hud, game->town->money);

	// load window icon
	spr_icon = SGUI_Sprite_from_file(renderer, PATH_TEXTURE_ICON);

	if (spr_icon.invalid)
	{
		Game_exit(window, renderer, &hud);
		return 6;
	}

	/* calculate ui sizes and positions */
	Hud_calc(&hud, game->cfg->gfx_window_w, game->cfg->gfx_window_h);
	Hud_generate_flips(&hud);
	Hud_map_textures(&hud, game->town->hidden, game->town->field);

	/* set window icon, and clear icon sprite */
	SDL_SetWindowIcon(window, spr_icon.surface);
	SGUI_Sprite_clear(&spr_icon);

	// mainloop
	uint32_t ts_now = 0;
	uint32_t ts_render = 0;

	SDL_Point mouse;
	SDL_Event event;

	int32_t border_t, border_l;
	int32_t window_x, window_y;
	int32_t window_w, window_h;
	SDL_Point hover;

	while (game->game_state == GS_ACTIVE)
	{
		if (ts_now > (ts_render + (1000.0f / game->cfg->gfx_framerate)))
		{
			/* clear screen */
			SDL_SetRenderDrawColor(renderer,
				game->cfg->bg_red,
				game->cfg->bg_green,
				game->cfg->bg_blue,
				255);
			SDL_RenderClear(renderer);

			/* draw hud */
			Hud_draw(&hud, game->town);

			/* show image, save time */
			SDL_RenderPresent(renderer);
			ts_render = ts_now;
		}

		/* update time */
		ts_now = SDL_GetTicks();

		/* update mouse coords */
		SDL_GetMouseState(&mouse.x, &mouse.y);

		// handle sdl-events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			// mouse hover
			case SDL_MOUSEMOTION:
				hover.x = (mouse.x - hud.rect_area.x) / hud.field_width;
				hover.y = (mouse.y - hud.rect_area.y) / hud.field_height;

				Hud_update_hover(&hud, hover);
				break;

			// window
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					// set config window size
					SDL_GetWindowSize(window, &window_w, &window_h);

					game->cfg->gfx_window_w = window_w;
					game->cfg->gfx_window_h = window_h;

					// recalc ui sizes and pos
					Hud_calc(&hud, game->cfg->gfx_window_w, game->cfg->gfx_window_h);
				}
				else if (event.window.event == SDL_WINDOWEVENT_MOVED)
				{
					// set config window pos
					SDL_GetWindowBordersSize(window, &border_t, &border_l, NULL, NULL);
					SDL_GetWindowPosition(window, &window_x, &window_y);

					game->cfg->gfx_window_x = window_x - border_l;
					game->cfg->gfx_window_y = window_y - border_t;
				}
				break;

			case SDL_QUIT:
				// save and close
				Town_save(game->town, game->town_name);
				game->game_state = GS_CLOSE;
				break;
			}
		}
	}

	/* save config */
	Config_save(game->cfg);

	/* quit routine */
	Game_exit(window, renderer, &hud);

	return 0;
}
