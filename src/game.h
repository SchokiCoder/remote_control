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

enum GameCmd
{
	GCMD_NONE,
	GCMD_STOP
};

enum GameResponse
{
	GRSP_NONE,
	GRSP_INIT,
	GRSP_STOPPED
};

struct GameData
{
	char* town_name;
	struct Town* town;
	struct Config* cfg;
	enum GameCmd cmd;			/* 1-way stream to inform gfx-window what to do */
	enum GameResponse rsp;		/* 1-way stream to provide feedback from the gfx-window */
};

int32_t gfx_game(void* p_data);

int32_t terminal_game(struct GameData* data);

#endif
