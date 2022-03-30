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

#ifndef ADMINS_H
#define ADMINS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct AdminData
{
	char first_name[10];
	char last_name[10];
	int32_t age;
	int32_t salary;
	bool male;
	char bio[256];
} AdminData ;

static const AdminData DATA_ADMINS[] = {
	{"Debbie", "Inkwell", 25, 10, false,
	"Nerdy new-coming assistant.\n" \
	"She just began and thus focuses on doing her job correctly.\n" \
	"Because of that she brings a stable environment, in which to work with no surprises."},

	{"Dennis", "Ritch", 33, true, 11,
	"Suited business oriented guy.\n" \
	"Focuses on thorough planning and preparation.\n" \
	"His execution is slow but once things get going, it will pay off."},

	{"Helen", "McLain", 63, false, 12,
	"Old chainsmoking lady.\n" \
	"Experienced, inducing fear amongst enemies and allies alike and should not be disappointed.\n" \
	"For the right payment, she can effectively turn tides in dire situations, with her methods of motivation."},
};

#endif /* ADMINS_H */
