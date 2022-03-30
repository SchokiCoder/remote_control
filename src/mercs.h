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

#ifndef MERCS_H
#define MERCS_H

#include <stdint.h>

typedef enum MercRole {
	MR_OFFENSE,
	MR_DEFENSE,
	MR_SUPPORT
} MercRole ;

typedef enum MercWeapon {
	MW_NONE,
	MW_SHOTGUN,
	MW_GRENADE,
	MW_KNIFE,
	MW_FLAMETHROWER,
	MW_MINIGUN,
	MW_SYRINGE_GUN,
	MW_PISTOL,
	MW_SWORD
} MercWeapon ;

typedef enum MercFraction {
	MF_GREEN,
	MF_PURPLE
} MercFraction ;

typedef struct MercenaryData
{
	char name[10];
	MercRole role;
	uint32_t max_hp;
	uint32_t speed;
	MercWeapon loadout[3];
} MercenaryData ;

static const MercenaryData DATA_MERCENARIES[] = {
    {"Soldier",	MR_OFFENSE,	125, 	2,	{MW_SHOTGUN, MW_GRENADE, MW_KNIFE}},
    {"Pyro",	MR_OFFENSE,	75,		3,	{MW_FLAMETHROWER, MW_NONE, MW_NONE}},
    {"Anchor",	MR_DEFENSE, 300,	1,	{MW_MINIGUN, MW_NONE, MW_NONE}},
    {"Medic",	MR_SUPPORT, 75,		2,	{MW_SYRINGE_GUN, MW_PISTOL, MW_SWORD}}
};
#define MERCENARY_COUNT sizeof(DATA_MERCENARIES) / sizeof(DATA_MERCENARIES[0])

#endif /* MERCS_H */
