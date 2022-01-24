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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "commands.h"

int main(int argc, char *argv[])
{		
	//if no args given
	if (argc < 2)
	{
		uint32_t option = 0;
		char input[64];
		int32_t num_arg;

		printf("Welcome to duty overseer, ");

		while (option != 7)
		{
			//ask
			printf("\nwhat do you wish to do?\n"
				"1) help\n"
				"2) list available administrators\n"
				"3) hire administrator\n"
				"4) list current towns\n"
				"5) connect to a town\n"
				"6) delete a town's documents\n"
				"7) exit\n");
			scanf("%s", input);

			//parse
			option = strtol(input, NULL, 10);

			switch(option)
			{
			case 1:
				cmd_help_outgame();
				break;

			case 2:
				cmd_list_admins();
				break;

			case 3:
				printf("Please enter the admin-id now: ");
				scanf("%s", input);
				num_arg = strtol(input, NULL, 10);

				printf("Please enter the town name now: ");
				scanf("%s", input);

				cmd_hire_admin(num_arg, input);
				break;

			case 4:
				cmd_list_towns();
				break;

			case 5:
				printf("Please enter the town name now: ");
				scanf("%s", input);
				while (getchar() != '\n');
				
				cmd_connect(input);
				break;

			case 6:
				//confirm deletion
				printf("Are you sure, to delete the town's files? (y)\n");
				scanf("%s", input);

				if (input[0] != 'y')
					break;

				//delete
				printf("Please enter the town name now: ");
				scanf("%s", input);				  

				cmd_delete(input);
				break;

			case 7:
				printf("Goodbye.\n");
				break;

			default:
				printf("Option not recognised.\n");
				break;
			}
		}

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
	else if ((strcmp(argv[1], CMD_LIST_TOWNS) == 0) |
		(strcmp(argv[1], CMD_LIST_TOWNS_LONG) == 0))
	{
		//check argc max
		if (argc > 2)
		{
			printf(MSG_WARN_ARG_MAX);
		}

		cmd_list_towns();
	}
	else if ((strcmp(argv[1], CMD_CONNECT) == 0) |
		(strcmp(argv[1], CMD_CONNECT_LONG) == 0))
	{
		//check argc min
		if (argc < 3)
		{
			printf(MSG_ERR_ARG_MIN);
			return 0;
		}

		//check argc max
		if (argc > 3)
		{
			printf(MSG_WARN_ARG_MAX);
		}

		cmd_connect(argv[2]);
	}
	else if (strcmp(argv[1], CMD_DELETE_TOWN_LONG) == 0)
	{
		//check argc min
		if (argc < 3)
		{
			printf(MSG_ERR_ARG_MIN);
			return 0;
		}

		//check argc max
		if (argc > 3)
		{
			printf(MSG_WARN_ARG_MAX);
		}
		
		cmd_delete(argv[2]);
	}
	else
	{
		printf(MSG_ERR_UNKNOWN_COMMAND);
	}
	
	return 0;
}
