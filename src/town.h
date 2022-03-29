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
#include "definitions/def_mercs.h"

typedef struct Construction
{
	Field field;
	SDL_Point coords;
	uint32_t progress;
} Construction ;

typedef struct Mercenary
{
	uint32_t id;
	SDL_Point coords;
	uint32_t hp;
	enum MercFraction fraction;
} Mercenary ;

typedef struct Town
{
	uint8_t admin_id;
	uint32_t round;
	uint32_t money;
	Field field[TOWN_WIDTH][TOWN_HEIGHT];
	bool hidden[TOWN_WIDTH][TOWN_HEIGHT];

	uint32_t construction_count;
	Construction constructions[TOWN_WIDTH * TOWN_HEIGHT];

	uint32_t merc_count;
	Mercenary mercs[MERCENARY_COUNT];
} Town ;

Town Town_new( void );

void Town_print( Town *town, char *town_name );

int32_t Town_save( Town *town, char *town_name );

int32_t Town_load( Town *town, char *town_name );

void Town_construction_list_remove( Town *town, uint32_t index );

#endif /* TOWN_H */
