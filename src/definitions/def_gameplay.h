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

#ifndef DEF_GAMEPLAY_H
#define DEF_GAMEPLAY_H

/* town generation */
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

/* building system */
#define TOWN_MAX_CONSTRUCTIONS		(TOWN_WIDTH * TOWN_HEIGHT)
#define TOWN_MAX_MERCS				4

/* fields */
enum Field
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
	FIELD_QUARRY
};

/* construction costs */
static const uint32_t FIELD_CONSTRUCTION_COST[] =
{
	0,	/* empty */
	0,	/* merc */
	0,	/* trees */
	0,
	0,
	0,
	0,	/* trees */
	0,	/* headquarter */
	0,	/* construction */
	20	/* quarry */
};

/* construction time */
static const uint32_t FIELD_CONSTRUCTION_TIME[] =
{
	1,	/* empty */
	0,	/* merc */
	0,	/* trees */
	0,
	0,
	0,
	0,	/* trees */
	0,	/* headquarter */
	0,	/* construction */
	2	/* quarry */
};

/* running costs */
static const uint32_t FIELD_RUNNING_COST[] =
{
	0,	/* empty */
	10,	/* merc */
	0,	/* trees */
	0,
	0,
	0,
	0,	/* trees */
	20,	/* headquarter */
	0,	/* construction */
	10	/* quarry */
};

#endif /* DEF_GAMEPLAY_H */
