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

typedef struct Config Config;
typedef struct Hud Hud;

enum GameState
{
	GS_ACTIVE,
	GS_CLOSE,
	GS_FAILURE_COST
};

typedef struct Game
{
	const char *town_name;
	Town *town;
	Config *cfg;

	enum GameState game_state;
} Game ;

void Game_end_round( Game *game, Hud *hud );

bool Game_construct( Game *game, Hud *hud, const SDL_Point field, const Field building );

bool Game_spawn_merc( Game *game, Hud *hud, const TownMerc merc );

bool Game_move_merc( Game *game, Hud *hud, const SDL_Point src_coord, const SDL_Point dest_coord );

int_fast32_t Game_merc_attack(
	Game *game,
	const SDL_Point src_coord,
	const uint_fast8_t weapon_slot,
	const SDL_Point dest_coord );

int32_t Game_main( Game *game );

#endif /* GAME_H */
