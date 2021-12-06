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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "constants.h"
#include "commands.h"

int main(int argc, char *argv[])
{
    //check amount of args
    if (argc < 2)
    {
        printf("No commands issued.\nUse \"%s\" command for information on usage.\n", CMD_HELP_LONG);
        return 0;
    }

    //interpret command
    if ((strcmp(argv[1], CMD_HELP) == 0) |
        (strcmp(argv[1], CMD_HELP_LONG) == 0))
    {
        //check argc max
        if (argc > 2)
        {
            printf(MSG_WARN_ARG_MAX);
        }
        
        cmd_help_outgame();
    }
    else if ((strcmp(argv[1], CMD_LIST_ADMINS) == 0) |
        (strcmp(argv[1], CMD_LIST_ADMINS_LONG) == 0))
    {
        //check argc max
        if (argc > 2)
        {
            printf(MSG_WARN_ARG_MAX);
        }

        cmd_list_admins();
    }
    else if ((strcmp(argv[1], CMD_HIRE_ADMIN) == 0) |
        (strcmp(argv[1], CMD_HIRE_ADMIN_LONG) == 0))
    {
        //check argc min
        if (argc < 4)
        {
            printf(MSG_ERR_ARG_MIN);
            return 0;
        }

        //check argc max
        if (argc > 4)
        {
            printf(MSG_WARN_ARG_MAX);
        }
        
        //parse args
        int32_t admin_id = strtol(argv[2], NULL, 10);
        
        cmd_hire_admin(admin_id, argv[3]);
    }
    else
    {
        printf("Command not recognised.\nUse \"%s\" command for information on usage.\n", CMD_HELP_LONG);
    }
    
    return 0;
}
