/*
    remote_control
    Copyright (C) 2021  Andy Frank Schoknecht

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
#include "constants.h"

enum Field
{
    FIELD_EMPTY,
    FIELD_TREE,
    FIELD_ADMINISTRATION
};

struct Town
{
    uint8_t admin_id;
    enum Field area_content[TOWN_WIDTH][TOWN_HEIGHT];
    bool area_hidden[TOWN_WIDTH][TOWN_HEIGHT];
};

#endif
