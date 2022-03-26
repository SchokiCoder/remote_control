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

#ifndef DEF_ADMINS_H
#define DEF_ADMINS_H

#include <stdint.h>
#include <stdbool.h>

static const int32_t ADMIN_ID[] =
{
	0,
	1,
	2
};

static const char ADMIN_FIRSTNAME[][10] =
{
	"Debbie",
	"Dennis",
	"Helen"
};

static const char ADMIN_LASTNAME[][10] =
{
	"Inkwell",
	"Ritch",
	"McLain"
};

static const int32_t ADMIN_AGE[] =
{
	25,
	33,
	63
};

static const bool ADMIN_MALE[] =
{
	false,
	true,
	false
};

static const char ADMIN_BIO[][256] =
{
	"Nerdy new-coming assistant.\n" \
	"She just began and thus focuses on doing her job correctly.\n" \
	"Because of that she brings a stable environment, in which to work with no surprises.",

	"Suited business oriented guy.\n" \
	"Focuses on thorough planning and preparation.\n" \
	"His execution is slow but once things get going, it will pay off.",

	"Old chainsmoking lady.\n" \
	"Experienced, inducing fear amongst enemies and allies alike and should not be disappointed.\n" \
	"For the right payment, she can effectively turn tides in dire situations, with her methods of motivation."
};

static const int32_t ADMIN_SALARY[] =
{
	10,
	11,
	12
};

#endif /* DEF_ADMINS_H */
