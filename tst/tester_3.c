/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "optargs.h"
#include <stdio.h>

int
main(int ac, char **av)
{
	struct optargs_opt opts[] =
	{
		{
			.description = "Help text",
			.long_option = "help",
			.short_option = 'h',
			.argument = { .mandatory = optargs_no},
		},
		optargs_opt_eol
	};

	struct optargs_arg args[] =
	{
		{ "ARGS1", NULL, optargs_maybe },
		{ "arg1a", "argument 1a", optargs_no },
		{ "arg1b", "argument 1b", optargs_no },
		{ "arg1c", "argument 1c", optargs_no },
		{ "ARGS2", NULL, optargs_maybe },
		{ "arg2a", "argument 2a", optargs_no },
		{ "arg2b", "argument 2b", optargs_no },
		{ "arg2c", "argument 2c", optargs_no },
		{ "ARGS3", NULL, optargs_maybe },
		{ "arg3a", "argument 3a", optargs_no },
		{ "arg3b", "argument 3b", optargs_no },
		{ "arg3c", "argument 3c", optargs_no },
		{ "ARGS4", NULL, optargs_maybe },
		{ "arg4a foo", "argument 4a", optargs_no },
		{ "arg4b bar", "argument 4b", optargs_no },
		{ "arg4c keke", "argument 4c", optargs_no },
		optargs_arg_eol
	};
	int i, j, k;

	if ((k = optargs_parse(ac, (const char **)av, opts)) < 0)
	{
		printf("Error\n");
		return -1;
	}

	if (optargs_opt_by_index(opts, 0))
	{
		optargs_print_help(av[0], "Foobar", opts, args);
		return 0;
	}

	for (i = 1; i < ac; ++i)
	{
		if ((j = optargs_arg_index(args, av[i])) >= 0)
			printf("%s,", args[j].name);
	}

	printf("\n");

	return 0;
}
