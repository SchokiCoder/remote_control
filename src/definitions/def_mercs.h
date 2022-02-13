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

#ifndef DEF_MERCS_H
#define DEF_MERCS_H

#include <stdint.h>

enum MercClass {
	MC_OFFENSE,
	MC_DEFENSE,
	MC_SUPPORT
};

enum MercWeapon {
	MW_NONE,
	MW_SHOTGUN,
	MW_GRENADE,
	MW_KNIFE,
	MW_FLAMETHROWER,
	MW_MINIGUN,
	MW_SYRINGE_GUN,
	MW_PISTOL,
	MW_SWORD
};

enum MercFraction {
	MF_GREEN,
	MF_PURPLE
};

static const uint32_t MERCS_ID[] = {
	0,
	1,
	2,
	3
};

static const char MERCS_NAME[][10] = {
	"Soldier",
	"Pyro",
	"Anchor",
	"Medic"
};

static const enum MercClass MERCS_CLASS[] = {
	MC_OFFENSE,
	MC_OFFENSE,
	MC_DEFENSE,
	MC_SUPPORT
};

static const uint32_t MERCS_HP[] = {
	125,
	75,
	300,
	75
};

static const uint32_t MERCS_SPEED[] = {
	2,
	3,
	1,
	2
};

static const enum MercWeapon MERCS_LOADOUT[][3] = {
	{
		MW_SHOTGUN,
		MW_GRENADE,
		MW_KNIFE
	},

	{
		MW_FLAMETHROWER,
		MW_NONE,
		MW_NONE
	},

	{
		MW_MINIGUN,
		MW_NONE,
		MW_NONE
	},

	{
		MW_SYRINGE_GUN,
		MW_PISTOL,
		MW_SWORD
	}
};

#endif /* DEF_MERCS_H */
