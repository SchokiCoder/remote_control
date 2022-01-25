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
#include "hud.h"
#include "game.h"

void exit_game(
	volatile enum GameResponse *p_response,
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

int32_t gfx_game(void *p_data)
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	TTF_Font *font;

	bool active = true;
	uint32_t ts_now = 0;
	uint32_t ts_render = 0;

	struct GameData *data = (struct GameData*) (p_data);

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
	font = TTF_OpenFont(data->cfg->path_font, HUD_FONT_SIZE);

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

	//update some widgets
	Hud_update_hover(&hud, renderer, font, font_color);
	Hud_update_money(&hud, data->town->money, renderer, font, font_color);

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
