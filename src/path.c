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

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "messages.h"
#include "app.h"
#include "path.h"

int32_t get_base_path( char *out )
{
	int32_t rc;

	/* get path */
	sprintf(out, PATH_BASE, getenv("HOME"), SLASH, APP_NAME);
	strncat(out, SLASH, (FILEPATH_MAX_LEN - strlen(out)));

	/* in case, create dir */
	errno = 0;

	#ifdef _WIN32
		rc = mkdir(out);
	#else
		rc = mkdir(out, S_IRWXU);
	#endif

	if (rc == -1)
	{
		if (errno != EEXIST)
		{
			printf(MSG_ERR_DIR_BASE, MSG_ERR);
			return 1;
		}
	}

	return 0;
}

int32_t get_town_path( char *out )
{
	int32_t rc;

	/* get base path */
	rc = get_base_path(out);

	if (rc != 0)
		return rc;

	/* get path */
	strncat(out, PATH_TOWNS, (FILEPATH_MAX_LEN - strlen(out)));
	strncat(out, SLASH, (FILEPATH_MAX_LEN - strlen(out)));

	/* in case, create dir */
	errno = 0;

	#ifdef _WIN32
		rc = mkdir(out);
	#else
		rc = mkdir(out, S_IRWXU);
	#endif

	if (rc == -1)
	{
		if (errno != EEXIST)
		{
			printf(MSG_ERR_DIR_TOWNS_CREATE, MSG_ERR);
			return 1;
		}
	}

	return 0;
}

int32_t get_config_path( char *out )
{
	int32_t rc;

	/* get base path */
	rc = get_base_path(out);

	if (rc != 0)
		return rc;

	/* get path */
	strncat(out, PATH_CONFIG, (FILEPATH_MAX_LEN - strlen(out)));

	return 0;
}
