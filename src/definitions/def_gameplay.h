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

//town generation
#define TOWN_WIDTH					15
#define TOWN_HEIGHT					15
#define TOWN_EXPOSURE_AREA_BEGIN_X	4
#define TOWN_EXPOSURE_AREA_BEGIN_Y	4
#define TOWN_EXPOSURE_AREA_END_X	11
#define TOWN_EXPOSURE_AREA_END_Y	11
#define TOWN_TREEFREE_AREA_BEGIN_X	5
#define TOWN_TREEFREE_AREA_BEGIN_Y	5
#define TOWN_TREEFREE_AREA_END_X	10
#define TOWN_TREEFREE_AREA_END_Y	10
#define TOWN_GEN_TREE_THRESHOLD		40	/* from 0 to 100 */
#define TOWN_HQ_SPAWN_X				7
#define TOWN_HQ_SPAWN_Y				7
#define TOWN_TIME_BEGIN				6	/* round 0 plays at 06:00 am */
#define TOWN_START_MONEY			50

#endif /* DEF_GAMEPLAY_H */