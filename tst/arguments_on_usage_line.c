/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016-2018 Hemmo Nieminen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

#define _STR(a) #a
#define STR(a) _STR(a)

static char const about[] = "skibidiwaynow!";

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_argument const args[] =
	{
		{
			.type = optargs_argument_group
		},
		{
			.name = "name:" STR(__LINE__),
			.description = "desc:" STR(__LINE__),
			.type = optargs_argument_group_member
		},
		{
			.name = "name:" STR(__LINE__),
			.description = "desc:" STR(__LINE__),
			.type = optargs_argument_group_member
		},
		{
			.name = "jee",
			.type = optargs_argument_group
		},
		{
			.name = "name:" STR(__LINE__),
			.description = "desc:" STR(__LINE__),
			.type = optargs_argument_group_member
		},
		{
			.name = "name:" STR(__LINE__),
			.description = "desc:" STR(__LINE__),
			.type = optargs_argument_group_member
		},
		{
			.type = optargs_argument_group_opt
		},
		{
			.name = "name:" STR(__LINE__),
			.description = "desc:" STR(__LINE__),
			.type = optargs_argument_group_member
		},
		{
			.name = "name:" STR(__LINE__),
			.description = "desc:" STR(__LINE__),
			.type = optargs_argument_group_member
		},
		{
			.name = "jee2",
			.type = optargs_argument_group_opt
		},
		{
			.name = "name:" STR(__LINE__),
			.description = "desc:" STR(__LINE__),
			.type = optargs_argument_group_member
		},
		{
			.name = "name:" STR(__LINE__),
			.description = "desc:" STR(__LINE__),
			.type = optargs_argument_group_member
		},
		optargs_argument_eol
	};

	optargs_print_help(av[0], about, NULL, args);

	return EXIT_SUCCESS;
}
