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

#include <stdint.h>

struct Town;
struct Config;
struct Hud;

struct GameData
{
	char *town_name;
	struct Town *town;
	struct Config *cfg;

	uint32_t admin_salary;
};

void gp_end_round(
	struct GameData *p_game_data,
	struct Hud *p_hud,
	SDL_Renderer *p_renderer);

int32_t gp_main(char *p_town_name, struct Town *p_town, struct Config *p_cfg);

#endif /* GAME_H */
