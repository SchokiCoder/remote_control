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

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>
#include "town.h"

struct Config;
struct Hud;

enum GameState
{
	GS_ACTIVE,
	GS_CLOSE,
	GS_FAILURE_COST
};

struct Game
{
	char *town_name;
	struct Town *town;
	struct Config *cfg;

	enum GameState game_state;
};

void Game_end_round(struct Game *self, struct Hud *p_hud);

void Game_construct(
	struct Game *self,
	SDL_Point p_field,
	enum Field p_building,
	struct Hud *p_hud,
	SDL_Texture *p_texture);

void Game_spawn_merc(struct Game *self, struct Hud *hud, struct Mercenary p_merc);

int32_t Game_main(struct Game *self);

#endif /* GAME_H */
