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

#ifdef _DEBUG
#include <time.h>
#endif

#include "definitions/def_commands.h"
#include "definitions/def_messages.h"
#include "definitions/def_files.h"
#include "definitions/def_admins.h"
#include "town.h"
#include "config.h"
#include "sprite.h"
#include "hud.h"
#include "game.h"

void Game_exit(SDL_Window *p_window, SDL_Renderer *p_renderer, struct Hud *p_hud)
{
	/* clear hud */
	Hud_clear(p_hud);

	/* if given, destroy window and renderer */
	if (p_window != NULL)
	{
		SDL_DestroyWindow(p_window);
	}

	if (p_renderer != NULL)
	{
		SDL_DestroyRenderer(p_renderer);
	}

	/* quit sdl ttf */
	TTF_Quit();

	/* quit sdl */
	SDL_Quit();
}

void Game_end_round(struct Game *self, struct Hud *p_hud)
{
	uint32_t cost = 0;

	/* get running cost for admin */
	cost += ADMIN_SALARY[self->town->admin_id];

	/* for each building, add cost */
	for (uint32_t x = 0; x < TOWN_WIDTH; x++)
	{
		for (uint32_t y = 0; y < TOWN_HEIGHT; y++)
		{
			cost += FIELD_RUNNING_COST[self->town->field[x][y]];
		}
	}

	/* if cost is not higher than current money */
	if (cost < self->town->money)
	{
		/* subtract running cost from players money */
		self->town->money -= cost;
	}
	else
	{
		/* gameover */
		self->game_state = GS_FAILURE_COST;
		return;
	}

	/* for all constructions */
	for (uint32_t i = 0; i < self->town->construction_count; i++)
	{
		/* increment progress */
		self->town->constructions[i].progress++;

		/* if building is done */
		if (self->town->constructions[i].progress >=
			FIELD_CONSTRUCTION_TIME[self->town->constructions[i].field])
		{
            /* set field to actual building */
            self->town->field
            	[self->town->constructions[i].coords.x]
            	[self->town->constructions[i].coords.y]
            	= self->town->constructions[i].field;

            /* update hud */
            Hud_set_field(
            	p_hud,
            	self->town->constructions[self->town->construction_count - 1].coords,
            	Hud_get_field_texture(p_hud, self->town->constructions[i].field));

            /* remove entry from construction list */
            Town_construction_list_remove(self->town, i);
		}
	}

	/* increment time */
	self->town->round++;

	/* save file */
	Town_save(self->town, self->town_name);

	/* update hud */
	Hud_update_time(p_hud, self->town->round);
	Hud_update_money(p_hud, self->town->money);
}

void Game_construct(
	struct Game *self,
	SDL_Point p_coords,
	enum Field p_field,
	struct Hud *p_hud,
	SDL_Texture *p_texture)
{
	/* change field */
	self->town->field[p_coords.x][p_coords.y] = FIELD_CONSTRUCTION;

	/* add building to construction list */
	self->town->construction_count++;
	self->town->constructions[self->town->construction_count - 1].field = p_field;
	self->town->constructions[self->town->construction_count - 1].coords = p_coords;
	self->town->constructions[self->town->construction_count - 1].progress = 0;

	/* subtract cost of building */
	self->town->money -= FIELD_CONSTRUCTION_COST[p_field];

	/* update hud */
	Hud_update_money(p_hud, self->town->money);
}

void Game_spawn_merc(struct Game *self, struct Hud *hud, struct Mercenary p_merc)
{
	//if merc already exists, stop
	for (uint32_t i = 0; i < self->town->merc_count; i++)
	{
		if (p_merc.id == self->town->mercs[i].id)
		{
			return;
		}
	}

	//add to merc list
	self->town->merc_count++;
	self->town->mercs[self->town->merc_count - 1] = p_merc;

    //update town
    self->town->field[p_merc.coords.x][p_merc.coords.y] = FIELD_MERC;

    //update hud
    Hud_set_field(hud, p_merc.coords, Hud_get_field_texture(hud, FIELD_MERC));
}

int32_t Game_main(struct Game *self)
{
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

#ifdef _DEBUG
	// init random gen
	srand(time(NULL));
#endif /* _DEBUG */

	/* init SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf(MSG_ERR_SDL_INIT, MSG_ERR, SDL_GetError());
		Game_exit(NULL, NULL, NULL);
		return 1;
	}

	/* create window */
	window = SDL_CreateWindow(
		self->town_name,
		self->cfg->gfx_window_x,
		self->cfg->gfx_window_y,
		self->cfg->gfx_window_w,
		self->cfg->gfx_window_h,
		(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

	if (window == NULL)
	{
		printf(MSG_ERR_SDL_WINDOW, MSG_ERR, SDL_GetError());
		Game_exit(NULL, NULL, NULL);
		return 2;
	}

	/* create renderer */
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		printf(MSG_ERR_SDL_RENDERER, MSG_ERR, SDL_GetError());
		Game_exit(window, NULL, NULL);
		return 3;
	}

	/* set alpha channel */
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	/* init ttf */
	if (TTF_Init() != 0)
	{
		printf(MSG_ERR_TTF_INIT, MSG_ERR);
		Game_exit(window, renderer, NULL);
		return 4;
	}

	/* init hud */
	if (Hud_new(&hud, renderer, self->cfg->path_font) != 0)
	{
		Game_exit(window, renderer, &hud);
		return 5;
	}

	/* set font color */
	hud.font_color.r = self->cfg->font_red;
	hud.font_color.g = self->cfg->font_green;
	hud.font_color.b = self->cfg->font_blue;
	hud.font_color.a = self->cfg->font_alpha;

	/* update some widgets */
	Hud_update_time(&hud, self->town->round);
	Hud_update_money(&hud, self->town->money);

	/* make all sprites */
	if ((Sprite_from_image(&spr_icon, renderer, PATH_TEXTURE_ICON) != 0) ||
		(Hud_load_sprites(&hud) != 0) ||
		(Hud_init_widgets(&hud) != 0))
	{
		Game_exit(window, renderer, &hud);
		return 6;
	}

	/* calculate ui sizes and positions */
	Hud_calc(&hud, self->cfg->gfx_window_w, self->cfg->gfx_window_h);
	Hud_generate_flips(&hud);
	Hud_map_textures(&hud, self->town->hidden, self->town->field);

	/* set window icon, and clear icon sprite */
	SDL_SetWindowIcon(window, spr_icon.surface);
	Sprite_clear(&spr_icon);

	/* mainloop */
	while (self->game_state == GS_ACTIVE)
	{
		if (ts_now > (ts_render + (1000.0f / self->cfg->gfx_framerate)))
		{
			/* clear screen */
			SDL_SetRenderDrawColor(renderer,
				self->cfg->bg_red,
				self->cfg->bg_green,
				self->cfg->bg_blue,
				255);
			SDL_RenderClear(renderer);

			/* draw hud */
			Hud_draw(&hud, self->town);

			/* show image, save time */
			SDL_RenderPresent(renderer);
			ts_render = ts_now;
		}

		/* update time */
		ts_now = SDL_GetTicks();

		/* update mouse coords */
		SDL_GetMouseState(&mouse.x, &mouse.y);

		/* handle sdl-events */
		while (SDL_PollEvent(&event))
		{
			/* mouse */
			switch (event.type)
			{
			case SDL_MOUSEBUTTONUP:
				Hud_handle_click(&hud, mouse, self);
				break;

			case SDL_MOUSEMOTION:
				Hud_handle_hover(&hud, mouse);
				break;

			/* keyboard */
			case SDL_KEYUP:
				if (event.key.keysym.sym == self->cfg->kb_pass)
				{
					Game_end_round(self, &hud);
				}

#ifdef _DEBUG
				//in case of debug mode, add debug keybinds
				else if (event.key.keysym.sym == SDLK_o)
				{
					// spawn random friendly merc
					struct Mercenary merc = {
						.id = rand() % TOWN_MAX_MERCS,
						.hp = MERCS_HP[merc.id],
						.coords = {.x = 8, .y = 7},
						.fraction = MF_PURPLE
					};

					Game_spawn_merc(self, &hud, merc);
				}
#endif /* _DEBUG */

				else if (event.key.keysym.sym == self->cfg->kb_build_quarry)
				{
					Hud_construct_mode(&hud, FIELD_QUARRY, hud.spr_quarry.texture);
					Hud_handle_hover(&hud, mouse);
				}
				else if (event.key.keysym.sym == self->cfg->kb_deconstruct)
				{
					Hud_deconstruct_mode(&hud);
					Hud_handle_hover(&hud, mouse);
				}
				break;

			/* window */
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					/* set config window size */
					SDL_GetWindowSize(window, &window_w, &window_h);

					self->cfg->gfx_window_w = window_w;
					self->cfg->gfx_window_h = window_h;

					/* recalc ui sizes and pos */
					Hud_calc(&hud, self->cfg->gfx_window_w, self->cfg->gfx_window_h);
				}
				else if (event.window.event == SDL_WINDOWEVENT_MOVED)
				{
					/* set config window pos */
					SDL_GetWindowBordersSize(window, &border_t, &border_l, NULL, NULL);
					SDL_GetWindowPosition(window, &window_x, &window_y);

					self->cfg->gfx_window_x = window_x - border_l;
					self->cfg->gfx_window_y = window_y - border_t;
				}
				break;

			case SDL_QUIT:
				/* save and close */
				Town_save(self->town, self->town_name);
				self->game_state = GS_CLOSE;
				break;
			}
		}
	}

	/* save config */
	Config_save(self->cfg);

	/* quit routine */
	Game_exit(window, renderer, &hud);

	return 0;
}
