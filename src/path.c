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

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "constants.h"
#include "path.h"

int32_t get_base_path(char* p_out)
{  
    int32_t rc;
    
    //get path
    strcat(p_out, getenv("HOME"));
    strcat(p_out, SLASH);
    strcat(p_out, PATH_BASE);
    strcat(p_out, SLASH);

    //in case, create dir
    errno = 0;

    #ifdef _WIN32
        rc = mkdir(p_out);
    #else
        rc = mkdir(p_out, S_IRWXU);
    #endif

    if (rc == -1)
    {
        if (errno != EEXIST)
        {
            printf(MSG_ERR_DIR_BASE);
            return 1;
        }
    }

    return 0;
}

int32_t get_town_path(char* p_out)
{
    int32_t rc;

    //get base path
    rc = get_base_path(p_out);

    if (rc != 0)
        return rc;

    //get path
    strcat(p_out, PATH_TOWNS);
    strcat(p_out, SLASH);

    //in case, create dir
    errno = 0;
    
    #ifdef _WIN32
        rc = mkdir(p_out);
    #else
        rc = mkdir(p_out, S_IRWXU);
    #endif
    
    if (rc == -1)
    {
        if (errno != EEXIST)
        {
            printf(MSG_ERR_DIR_TOWNS_CREATE);
            return 1;
        }
    }

    return 0;
}

int32_t get_config_path(char* p_out)
{
    int32_t rc;

    //get base path
    rc = get_base_path(p_out);

    if (rc != 0)
        return rc;

    //get path
    strcat(p_out, PATH_CONFIG);

    return 0;
}
