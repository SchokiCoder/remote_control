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

#ifndef TOWN_H
#define TOWN_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "definitions/def_gameplay.h"

enum Field
{
	FIELD_EMPTY,
	FIELD_TREE_0,
	FIELD_TREE_1,
	FIELD_TREE_2,
	FIELD_TREE_3,
	FIELD_TREE_4,
	FIELD_ADMINISTRATION,
	FIELD_CONSTRUCTION,
	FIELD_QUARRY

};

struct Construction
{
	enum Field building;
	SDL_Point field;
	uint32_t progress;
	uint32_t time;		/* how long does it take to build */
	SDL_Texture *texture;
};

struct Town
{
	uint8_t admin_id;
	uint32_t round;
	uint32_t money;
	enum Field area_content[TOWN_WIDTH][TOWN_HEIGHT];
	bool area_hidden[TOWN_WIDTH][TOWN_HEIGHT];

	uint32_t construction_count;
	struct Construction constructions[TOWN_MAX_CONSTRUCTIONS];
};

void Town_print(struct Town *self, char *p_town_name);

void Town_new(struct Town *self);

int32_t Town_save(struct Town *self, char *p_town_name);

int32_t Town_load(struct Town *self, char *p_town_name);

void Town_construction_list_remove(struct Town *self, uint32_t p_index);

uint32_t get_construction_cost(enum Field p_field);

uint32_t get_construction_time(enum Field p_field);

#endif /* TOWN_H */
