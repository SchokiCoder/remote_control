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
#include "mercs.h"

#define TOWN_WIDTH	15
#define TOWN_HEIGHT	15

static const uint32_t TOWN_EXPOSURE_AREA_BEGIN_X =	4;
static const uint32_t TOWN_EXPOSURE_AREA_BEGIN_Y =	4;
static const uint32_t TOWN_EXPOSURE_AREA_END_X =	11;
static const uint32_t TOWN_EXPOSURE_AREA_END_Y =	11;
static const uint32_t TOWN_TREEFREE_AREA_BEGIN_X =	5;
static const uint32_t TOWN_TREEFREE_AREA_BEGIN_Y =	5;
static const uint32_t TOWN_TREEFREE_AREA_END_X =	10;
static const uint32_t TOWN_TREEFREE_AREA_END_Y =	10;
static const uint32_t TOWN_GEN_TREE_THRESHOLD =		40;		/* from 0 to 100 */
static const uint32_t TOWN_HQ_SPAWN_X =				7;
static const uint32_t TOWN_HQ_SPAWN_Y =				7;
static const uint32_t TOWN_START_TIME =				6;		/* round 0 plays at 06:00 am */
static const uint32_t TOWN_START_MONEY =			2000;

typedef enum Field
{
	FIELD_EMPTY,
	FIELD_MERC,
	FIELD_TREE_0,
	FIELD_TREE_1,
	FIELD_TREE_2,
	FIELD_TREE_3,
	FIELD_TREE_4,
	FIELD_ADMINISTRATION,
	FIELD_CONSTRUCTION,
	FIELD_QUARRY,

	FIELD_FIRST = FIELD_EMPTY,
	FIELD_LAST = FIELD_QUARRY
} Field ;
static const uint_fast32_t FIELD_TREE_COUNT = 5;

typedef struct FieldData
{
	char name[15];
	uint32_t construction_cost;
	uint32_t construction_time;
	uint32_t running_cost;
} FieldData ;

static const FieldData DATA_FIELDS[] = {
	{"empty", 0, 1, 0},
	{"merc", 0, 0, 10},
	{"tree0", 0, 0, 0},
	{"tree1", 0, 0, 0},
	{"tree2", 0, 0, 0},
	{"tree3", 0, 0, 0},
	{"tree4", 0, 0, 0},
	{"administration", 0, 0, 20},
	{"construct", 0, 0, 0},
	{"quarry", 20, 2, 10},
};

typedef struct Construction
{
	Field field;
	SDL_Point coords;
	uint32_t progress;
} Construction ;

typedef struct TownMerc
{
	Mercenary id;
	SDL_Point coords;
	uint32_t hp;
	enum MercFraction fraction;
} TownMerc ;

bool str_to_field( const char *str, Field *field );

uint_fast32_t get_distance( const SDL_Point pt1, const SDL_Point pt2 );

typedef struct Town
{
	bool invalid;
	uint8_t admin_id;
	uint32_t round;
	uint32_t money;
	Field field[TOWN_WIDTH][TOWN_HEIGHT];
	bool hidden[TOWN_WIDTH][TOWN_HEIGHT];

	uint32_t construction_count;
	Construction constructions[TOWN_WIDTH * TOWN_HEIGHT];

	uint32_t merc_count;
	TownMerc mercs[MERCENARY_COUNT];
} Town ;

Town Town_new( void );

void Town_print( const Town *town, const char *town_name );

void Town_save( Town *town, const char *town_name );

void Town_load( Town *town, const char *town_name );

void Town_construction_list_remove( Town *town, const uint32_t index );

#endif /* TOWN_H */
