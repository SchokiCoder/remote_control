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
#include <SDL_image.h>
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
#include "game_commands.h"
#include "game.h"

static const uint_fast8_t MAX_ARGS = 16;
static const uint_fast8_t MAX_ARG_LEN = 32;

void Game_issue_command( Game *game, Hud *hud, const char *str )
{
	const char *splits[2];
	uint_fast32_t argc = 0;
	char argv[MAX_ARGS][MAX_ARG_LEN];

	// parse input
	splits[0] = &str[0];

	for (uint_fast32_t i = 0; i < MAX_ARGS; i++)
	{
		// increment arg counter
		argc++;

		// split
		splits[1] = strchr(splits[0], ' ');

		argv[i][0] = '\0';
		strncat(argv[i], splits[0], (size_t) (splits[1] - splits[0]));

		// if not end, move pointers, else stop
		if (splits[1] != NULL)
			splits[0] = splits[1] + 1;
		else
			break;
	}

	// parse command
	/*if (strcmp(argv[0], GM_CMD_HELP) == 0)
	{
		//check arg max
		if (argc > 1)
		{
			printf(MSG_WARN_ARG_MAX);
		}

		printf(HELP_TEXT_INGAME);
	}
	else*/

	if (strcmp(argv[0], GM_CMD_SAVE) == 0)
	{
		/*// check arg max
		if (argc > 1)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		gm_cmd_save(hud, game->town_name, game->town);
	}
	else if (strcmp(argv[0], GM_CMD_SAVE_AS) == 0)
	{
		// check arg min
		if (argc < 2)
		{
			Hud_update_feedback(hud, GM_MSG_ERR_MIN_ARG);
			return;
		}

		/*// check arg max
		else if (argc > 2)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		gm_cmd_save_as(hud, argv[1], game->town);
	}
	else if (strcmp(argv[0], GM_CMD_EXIT) == 0)
	{
		/*// check arg max
		if (argc > 1)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		game->game_state = GS_CLOSE;
	}
	/*else if (strcmp(argv[0], GM_CMD_CONFIG_SHOW) == 0)
	{
		// check arg max
		if (argc > 1)
		{
			printf(MSG_WARN_ARG_MAX);
		}

		gm_cmd_show_config(lbl_feedback, data->cfg);
	}*/
	else if (strcmp(argv[0], GM_CMD_CONFIG_SET) == 0)
	{
		// check arg min
		if (argc < 3)
		{
			Hud_update_feedback(hud, GM_MSG_ERR_MIN_ARG);
			return;
		}

		/*// check arg max
		else if (argc > 3)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		gm_cmd_config_set(hud, game->cfg, argv[1], argv[2]);
	}
	else if (strcmp(argv[0], GM_CMD_PASS) == 0)
	{
		/*// check arg max
		if (argc > 3)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		// end round
		Game_end_round(game, hud);
	}
	else
		Hud_update_feedback(hud, GM_MSG_ERR_UNKNOWN_CMD);
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

void Game_construct( Game *game, const SDL_Point coords, const Field field, Hud *hud )
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

void Game_spawn_merc( Game *game, Hud *hud, const Mercenary merc )
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

#ifdef _DEBUG
	// init random gen
	srand(time(NULL));
#endif // _DEBUG

	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf(MSG_ERR_SDL_INIT, MSG_ERR, SDL_GetError());
		goto game_clear;
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
		goto game_clear;
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		printf(MSG_ERR_SDL_RENDERER, MSG_ERR, SDL_GetError());
		goto game_clear;
	}

	// set alpha channel
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// init ttf
	if (TTF_Init() != 0)
	{
		printf(MSG_ERR_TTF_INIT, MSG_ERR);
		goto game_clear;
	}

	// init hud
	Hud_new(&hud, renderer, game->cfg);

	if (hud.invalid)
		goto game_clear;

	// update some widgets
	Hud_update_time(&hud, game->town->round);
	Hud_update_money(&hud, game->town->money);

	// load window icon
	SDL_Surface *win_icon = NULL;

	win_icon = IMG_Load(PATH_TEXTURE_ICON);

	// if icon loaded, set for window
	if (win_icon != NULL)
	{
		SDL_SetWindowIcon(window, win_icon);
	}
	else
	{
		printf(MSG_WARN_WIN_ICON);
	}

	// handle hud field textures
	Hud_generate_flips(&hud);
	Hud_map_textures(&hud, game->town->hidden, game->town->field);

	// set hud values and recalculate ui sizes
	Hud_update_time(&hud, game->town->round);
	Hud_update_money(&hud, game->town->money);

	Hud_calc(&hud, game->cfg->gfx_window_w, game->cfg->gfx_window_h);

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
			// clear screen
			SDL_SetRenderDrawColor(renderer,
				game->cfg->bg_red,
				game->cfg->bg_green,
				game->cfg->bg_blue,
				255);
			SDL_RenderClear(renderer);

			// draw hud
			Hud_draw(&hud, game->town);

			// show image, save time
			SDL_RenderPresent(renderer);
			ts_render = ts_now;
		}

		// update time
		ts_now = SDL_GetTicks();

		// update mouse coords
		SDL_GetMouseState(&mouse.x, &mouse.y);

		// handle sdl-events
		while (SDL_PollEvent(&event))
		{
			// menu
			SGUI_Menu_handle_events(&hud.mnu_hud, &event);

			switch (event.type)
			{
			// keyboard
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_RETURN)
					Game_issue_command(game, &hud, hud.txt_command.text.str);
				break;

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

	// save config
	Config_save(game->cfg);

	game_clear:

	// clear hud
	Hud_clear(&hud);

	// if given, destroy window and renderer
	if (window != NULL)
		SDL_DestroyWindow(window);

	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);

	// free win icon
	SDL_FreeSurface(win_icon);

	// quit sdl ttf
	TTF_Quit();

	// quit sdl
	SDL_Quit();

	return 0;
}
