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

struct Town;
struct Config;
struct Hud;

enum GameState
{
	GS_ACTIVE,
	GS_CLOSE,
	GS_FAILURE_COST
};

struct GameData
{
	char *town_name;
	struct Town *town;
	struct Config *cfg;

	enum GameState game_state;
	uint32_t admin_salary;
};

void gp_end_round(struct GameData *p_game_data, struct Hud *p_hud);

int32_t gp_main(struct GameData *p_game_data);

#endif /* GAME_H */
