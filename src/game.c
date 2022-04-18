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
#include <SM_crypto.h>

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

void print_gmcmd_djb2( void )
{
	FILE *file = fopen("djb2_gmhash.txt", "w");

	fprintf(file, "%-32s| %-10s | %-10s\n", "cmd", "name", "abbr");

	for (uint32_t i = 0; i <= GM_CMD_LAST; i++)
	{
		if (DATA_GM_CMDS[i].has_abbr)
		{
			fprintf(file,
				"%-32s| %-10u | %-10u\n",
				DATA_GM_CMDS[i].name,
				SM_djb2_encode(DATA_GM_CMDS[i].name),
				SM_djb2_encode(DATA_GM_CMDS[i].abbr));
		}
		else
		{
			fprintf(file,
				"%-32s| %-10u |\n",
				DATA_GM_CMDS[i].name,
				SM_djb2_encode(DATA_GM_CMDS[i].name));
		}
	}

	fclose(file);
}

void Game_issue_command( Game *game, Hud *hud, const char *str )
{
	uint32_t gm_cmd_djb2;
	bool valid_gm_cmd = true;
	const char *splits[2];
	uint_fast32_t argc = 0;
	char argv[MAX_ARGS][MAX_ARG_LEN];
	SDL_Point coord;
	SDL_Point dest_coord;
	uint_fast32_t weapon_slot;
	Field field;
	bool valid_field;
	uint_fast32_t min_constr_args = 4;

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

	gm_cmd_djb2 = SM_djb2_encode(argv[0]);

	// exec command
	switch (gm_cmd_djb2)
	{
	/*case DJB2_GM_HELP:
	case DJB2_GM_HELP_ABBR:
		//check arg max
		if (argc > 1)
		{
			printf(MSG_WARN_ARG_MAX);
		}

		printf(HELP_TEXT_INGAME);
		break;*/

	case DJB2_GM_SAVE:
		/*// check arg max
		if (argc > 1)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		gm_cmd_save(hud, game->town_name, game->town);
		break;

	case DJB2_GM_SAVE_AS:
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
		break;

	case DJB2_GM_EXIT:
		/*// check arg max
		if (argc > 1)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		game->game_state = GS_CLOSE;
		break;

	case DJB2_GM_CONFIG_SET:
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
		break;

	/*case DJB2_GM_CONFIG_SHOW:
		// check arg max
		if (argc > 1)
		{
			printf(MSG_WARN_ARG_MAX);
		}

		gm_cmd_show_config(lbl_feedback, data->cfg);
		break;*/

	case DJB2_GM_PASS:
		/*// check arg max
		if (argc > 3)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		gm_cmd_pass(game, hud);
		break;

	case DJB2_GM_MERC_MOVE:
	case DJB2_GM_MERC_MOVE_ABBR:
		// check arg min
		if (argc < 5)
		{
			Hud_update_feedback(hud, GM_MSG_ERR_MIN_ARG);
			return;
		}

		// parse args
		coord.x = strtol(argv[1], NULL, 10);
		coord.y = strtol(argv[2], NULL, 10);
		dest_coord.x = strtol(argv[3], NULL, 10);
		dest_coord.y = strtol(argv[4], NULL, 10);

		gm_cmd_merc_move(game, hud, coord, dest_coord);
		break;

	case DJB2_GM_MERC_ATTACK:
	case DJB2_GM_MERC_ATTACK_ABBR:
		// check arg min
		if (argc < 6)
		{
			Hud_update_feedback(hud, GM_MSG_ERR_MIN_ARG);
			return;
		}

		// parse args
		coord.x = strtol(argv[1], NULL, 10);
		coord.y = strtol(argv[2], NULL, 10);
		weapon_slot = strtol(argv[3], NULL, 10);
		dest_coord.x = strtol(argv[4], NULL, 10);
		dest_coord.y = strtol(argv[5], NULL, 10);

		gm_cmd_merc_attack(game, hud, coord, weapon_slot, dest_coord);
		break;

	case DJB2_GM_DESTRUCT:
	case DJB2_GM_DESTRUCT_ABBR:
		min_constr_args = 3;

		// parse destr args
		valid_field = true;
		field = FIELD_EMPTY;
		coord.x = strtol(argv[1], NULL, 10);
		coord.y = strtol(argv[2], NULL, 10);
		goto CONSTRUCT;

	case DJB2_GM_CONSTRUCT:
	case DJB2_GM_CONSTRUCT_ABBR:
		CONSTRUCT:

		// check arg min
		if (argc < min_constr_args)
		{
			Hud_update_feedback(hud, GM_MSG_ERR_MIN_ARG);
			return;
		}

		/*// check arg max
		else if (argc > min_constr_args)
		{
			printf(MSG_WARN_ARG_MAX);
		}*/

		// parse constr args
		valid_field = str_to_field(argv[1], &field);
		coord.x = strtol(argv[2], NULL, 10);
		coord.y = strtol(argv[3], NULL, 10);

		// check values
		if (valid_field == false)
		{
			Hud_update_feedback(hud, GM_MSG_CONSTRUCT_INVALID);
			return;
		}

		if (coord.x < 0 || coord.y < 0 ||
			coord.x > TOWN_WIDTH || coord.y > TOWN_HEIGHT)
		{
			Hud_update_feedback(hud, GM_MSG_CONSTRUCT_COORD_INVALID);
			return;
		}

		// exec
		gm_cmd_construct(game, hud, coord, field);
		break;

	default:
		Hud_update_feedback(hud, GM_MSG_ERR_UNKNOWN_CMD);
		valid_gm_cmd = false;
		break;
	}

	// if issued command was valid
	if (valid_gm_cmd)
	{
		// add to command history
		Hud_add_to_command_history(hud, str);
	}
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

void Game_construct( Game *game, Hud *hud, const SDL_Point coords, const Field field )
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
	Hud_set_field(hud, coords, hud->spr_fields[FIELD_CONSTRUCTION].texture);
}

void Game_spawn_merc( Game *game, Hud *hud, const TownMerc merc )
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

bool Game_move_merc( Game *game, Hud *hud, const SDL_Point src_coord, const SDL_Point dest_coord )
{
	// check if src_coord has mercenary
	if (game->town->field[src_coord.x][src_coord.y] != FIELD_MERC)
		return false;

    // check if destination is empty
    if (game->town->field[dest_coord.x][dest_coord.y] != FIELD_EMPTY)
    	return false;

	// move merc field
    game->town->field[src_coord.x][src_coord.y] = FIELD_EMPTY;
    game->town->field[dest_coord.x][dest_coord.y] = FIELD_MERC;

    // update merc list
    for (uint_fast32_t i = 0; i < game->town->merc_count; i++)
    {
    	if (game->town->mercs[i].coords.x == src_coord.x &&
    		game->town->mercs[i].coords.y == src_coord.y)
    	{
    		game->town->mercs[i].coords.x = dest_coord.x;
    		game->town->mercs[i].coords.y = dest_coord.y;
    		break;
		}
    }

    // update hud
    Hud_set_field(hud, src_coord, hud->spr_fields[FIELD_EMPTY].texture);
    Hud_set_field(hud, dest_coord, hud->spr_merc_base.texture);

	return true;
}

int_fast32_t Game_merc_attack(
	Game *game,
	const SDL_Point src_coord,
	const uint_fast8_t weapon_slot,
	const SDL_Point dest_coord )
{
	MercWeapon weapon;
	int_fast32_t damage;
	float dmg_falloff;

	// check if src_coord has mercenary
	if (game->town->field[src_coord.x][src_coord.y] != FIELD_MERC)
		return 0;

	// check if destination is valid target
	if (game->town->field[dest_coord.x][dest_coord.y] != FIELD_MERC)
		return 0;

	// find used weapon
	for (uint_fast32_t i = 0; i < game->town->merc_count; i++)
    {
    	if (game->town->mercs[i].coords.x == src_coord.x &&
    		game->town->mercs[i].coords.y == src_coord.y)
    	{
			weapon = DATA_MERCENARIES[game->town->mercs[i].id].loadout[weapon_slot];
			break;
    	}
	}

    // calculate damage
	damage = DATA_WEAPONS[weapon].damage;

	if (DATA_WEAPONS[weapon].damage_falloff)
		damage *= dmg_falloff;

	// apply damage
	for (uint_fast32_t i = 0; i < game->town->merc_count; i++)
    {
    	if (game->town->mercs[i].coords.x == dest_coord.x &&
    		game->town->mercs[i].coords.y == dest_coord.y)
    	{
    		game->town->mercs[i].hp -= damage;
    		break;
		}
	}

	return damage;
}

int32_t Game_main( Game *game )
{print_gmcmd_djb2();return 0;
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
	SDL_Point hover_coord;
	Field hover_field;

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
			SGUI_Menu_handle_event(&hud.mnu_hud, &event);

			switch (event.type)
			{
			// keyboard
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_RETURN:
					// isuue command
					Game_issue_command(game, &hud, hud.txt_command.text.str);
					hud.txt_command.text.str[0] = '\0';
					hud.txt_command.text.len = 0;

					// reset command history cursor
					hud.cmd_history_cursor = -1;
					break;

				case SDLK_UP:
					if (hud.cmd_history_cursor < (HUD_CMD_HISTORY_LEN - 1))
					{
						hud.cmd_history_cursor++;
						SM_String_copy(&hud.txt_command.text, &hud.cmd_history[hud.cmd_history_cursor]);
						SGUI_Entry_update_sprites(&hud.txt_command);
					}
					break;

				case SDLK_DOWN:
					if (hud.cmd_history_cursor > 0)
					{
						hud.cmd_history_cursor--;
						SM_String_copy(&hud.txt_command.text, &hud.cmd_history[hud.cmd_history_cursor]);
						SGUI_Entry_update_sprites(&hud.txt_command);
					}
					else
					{
						hud.cmd_history_cursor = -1;
						SM_String_empty(&hud.txt_command.text);
						SGUI_Entry_update_sprites(&hud.txt_command);
					}
					break;
				}
				break;

			// mouse hover
			case SDL_MOUSEMOTION:
				hover_coord.x = (mouse.x - hud.rect_area.x) / hud.field_width;
				hover_coord.y = (mouse.y - hud.rect_area.y) / hud.field_height;

				if (hover_coord.x >= 0 && hover_coord.y >= 0 &&
					hover_coord.x < TOWN_WIDTH && hover_coord.y < TOWN_HEIGHT)
				{
					hover_field = game->town->field[hover_coord.x][hover_coord.y];
				}

				Hud_update_hover(&hud, hover_coord, DATA_FIELDS[hover_field].name);
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
