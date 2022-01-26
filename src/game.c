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

struct GamePlayData
{
	uint32_t admin_salary;
};

/* gameplay functions */
void gp_end_round(
	struct Town *p_town,
	struct GamePlayData *p_gpd,
	volatile enum RoundReport *p_rpt,
	struct Hud *p_hud,
	SDL_Renderer *p_renderer)
{
	uint32_t cost = 0;
	
	//get running cost for admin and hq
	cost += p_gpd->admin_salary;
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
	if (cost < p_town->money)
	{
		//subtract running cost from players money
		p_town->money -= cost;
	}
	else
	{
		//gameover
		*p_rpt = RPT_FAILURE_COST;
		return;
	}
	
	//increment time
	p_town->round++;

	//update hud
	Hud_update_time(p_hud, p_town->round, p_renderer);
	Hud_update_money(p_hud, p_town->money, p_renderer);
}

void exit_game(
	volatile enum GameResponse *p_response,
	SDL_Window *p_window,
	SDL_Renderer *p_renderer,
	struct Hud *p_hud)
{
	//send stop response
	*p_response = GRSP_STOPPED;

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

int32_t gfx_game(void *p_data)
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	bool active = true;
	uint32_t ts_now = 0;
	uint32_t ts_render = 0;

	struct GameData *data = (struct GameData*) (p_data);
	struct GamePlayData gameplay_data;

	SDL_Event event;
	int32_t border_t, border_l;
	int32_t window_x, window_y;
	int32_t window_w, window_h;

	struct Hud hud;

	struct Sprite spr_icon;

	//init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf(MSG_ERR_SDL_INIT, SDL_GetError());
		exit_game(&data->rsp, NULL, NULL, NULL);
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
		exit_game(&data->rsp, NULL, NULL, NULL);
		return 2;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		printf(MSG_ERR_SDL_RENDERER, SDL_GetError());
		exit_game(&data->rsp, window, NULL, NULL);
		return 3;
	}

	//set alpha channel
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//init ttf
	if (TTF_Init() != 0)
	{
		printf(MSG_ERR_TTF_INIT);
		exit_game(&data->rsp, window, renderer, NULL);
		return 4;
	}

	//init hud
	if (Hud_new(&hud, data->cfg->path_font) != 0)
	{
		exit_game(&data->rsp, window, renderer, &hud);
		return 5;
	}

	//set font color
	hud.font_color.r = data->cfg->font_red;
	hud.font_color.g = data->cfg->font_green;
	hud.font_color.b = data->cfg->font_blue;
	hud.font_color.a = data->cfg->font_alpha;

	//update some widgets
	Hud_update_time(&hud, data->town->round, renderer);
	Hud_update_money(&hud, data->town->money, renderer);
	Hud_update_hover(&hud, renderer);

	//make all sprites
	if ((Sprite_from_image(&spr_icon, renderer, PATH_TEXTURE_ICON) != 0) ||
		(Hud_load_sprites(&hud, renderer) != 0) ||
		(Hud_init_widgets(&hud, renderer) != 0))
	{
		exit_game(&data->rsp, window, renderer, &hud);
		return 6;
	}

	//calculate ui sizes and positions
	Hud_calc(&hud, data->cfg->gfx_window_w,	data->cfg->gfx_window_h);
	Hud_generate_flips(&hud);
	Hud_map_textures(&hud, data->town->area_hidden, data->town->area_content);

	//set window icon, and clear icon sprite
	SDL_SetWindowIcon(window, spr_icon.surface);
	Sprite_clear(&spr_icon);

	//prepare gameplay data		
	switch (data->town->admin_id)
	{
	case ADMIN_1_ID:
		gameplay_data.admin_salary = ADMIN_1_SALARY;
		break;

	case ADMIN_2_ID:
		gameplay_data.admin_salary = ADMIN_2_SALARY;
		break;

	case ADMIN_3_ID:
		gameplay_data.admin_salary = ADMIN_3_SALARY;
		break;
	}

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
			Hud_draw(&hud, renderer);

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

		case GCMD_PASS:
			//function to process end of round
			gp_end_round(data->town, &gameplay_data, &data->rpt, &hud, renderer);
			
			//check if player failed the game
			if (data->rpt != RPT_NONE)
			{
				//player lost, stop gfx mainloop
				active = false;
			}
			
			data->cmd = GCMD_NONE;	/* TODO yes i know, i'll fix it later */
			break;
		}

		//handle sdl-events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				Hud_update_hover(&hud, renderer);
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
	exit_game(&data->rsp, window, renderer, &hud);

	return 0;
}

int32_t terminal_game(struct GameData *data)
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

		//game initialized
		case GRSP_INIT:
			enabled = true;
			break;

		//game stopped
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
			else if (strcmp(argv[0], GM_CMD_PASS) == 0)
			{
				//check arg max
				if (argc > 3)
				{
					printf(MSG_WARN_ARG_MAX);
				}

				//send signal
				data->cmd = GCMD_PASS;
			}
			else
			{
				printf(MSG_ERR_UNKNOWN_COMMAND);
			}
		}
	}

	//if player failed, print
	if (data->rpt != RPT_NONE)
	{
		//"you failed"
		printf(MSG_PLAYER_FAILURE);

		//reason
		switch (data->rpt)
		{
		case RPT_NONE:
			break;
			
		case RPT_FAILURE_COST:
			printf(MSG_FAILURE_COST);
		}
	}

	//print
	printf(MSG_CONNECTION_CLOSED);

	return 0;
}
