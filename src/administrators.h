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

#ifndef ADMINISTRATORS_H
#define ADMINISTRATORS_H

#include <stdint.h>
#include <stdbool.h>

//nerdy new assistant
//she just began and just focuses on not messing up things, can you blame her
//no real up- or downsides because of that
#define ADMIN_1_ID 54
#define ADMIN_1_FIRSTNAME "Robin"
#define ADMIN_1_LASTNAME "Inkwell"
#define ADMIN_1_AGE 25
#define ADMIN_1_MALE false
#define ADMIN_1_BIO "Nerdy new-coming assistant.\n" \
"She just began and thus focuses on doing her job correctly.\n" \
"Because of that she brings a stable environment, in which to work with no surprises."

//suity business guy
//focuses on thorough planning
//causing preparation to be slow but execution to be fast and or effective
#define ADMIN_2_ID 22
#define ADMIN_2_FIRSTNAME "Dennis"
#define ADMIN_2_LASTNAME "Ritch"
#define ADMIN_2_AGE 33
#define ADMIN_2_MALE true
#define ADMIN_2_BIO "Suited business oriented guy.\n" \
"Focuses on thorough planning and preparation.\n" \
"His execution is slow but once things get going, it will pay off."

//old expirienced chainsmoker seductress
//inducing fear in your coworkers and should not be disappointed
//when things look bad she can motivate those under her to perform well, but requires higher salary
#define ADMIN_3_ID 4
#define ADMIN_3_FIRSTNAME "Helen"
#define ADMIN_3_LASTNAME "McLain"
#define ADMIN_3_AGE 63
#define ADMIN_3_MALE false
#define ADMIN_3_BIO "Old chainsmoking lady.\n" \
"Experienced, inducing fear amongst enemies and friends alike and should not be disappointed.\n" \
"For the right payment, she can effectively turn tides in dire situations, with her methods of motivation."

#endif /* ADMINISTRATORS_H */
