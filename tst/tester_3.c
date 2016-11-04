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
		},
		optargs_opt_eol
	};

	struct optargs_arg args[] =
	{
		{ .name = "ARGS1", .mandatory = optargs_maybe },
		{ .name = "arg1a", .description = "argument 1a", .mandatory = optargs_no },
		{ .name = "arg1b", .description = "argument 1b", .mandatory = optargs_no },
		{ .name = "arg1c", .description = "argument 1c", .mandatory = optargs_no },
		{ .name = "ARGS2", .mandatory = optargs_maybe },
		{ .name = "arg2a", .description = "argument 2a", .mandatory = optargs_no },
		{ .name = "arg2b", .description = "argument 2b", .mandatory = optargs_no },
		{ .name = "arg2c", .description = "argument 2c", .mandatory = optargs_no },
		{ .name = "ARGS3", .mandatory = optargs_maybe },
		{ .name = "arg3a", .description = "argument 3a", .mandatory = optargs_no },
		{ .name = "arg3b", .description = "argument 3b", .mandatory = optargs_no },
		{ .name = "arg3c", .description = "argument 3c", .mandatory = optargs_no },
		{ .name = "ARGS4", .mandatory = optargs_maybe },
		{ .name = "arg4a foo", .description = "argument 4a", .mandatory = optargs_no },
		{ .name = "arg4b bar", .description = "argument 4b", .mandatory = optargs_no },
		{ .name = "arg4c keke", .description = "argument 4c", .mandatory = optargs_no },
		optargs_arg_eol
	};
	int i, j, k;

	if ((k = optargs_parse(ac, (const char **)av, opts)) < 0)
	{
		printf("Error\n");
		return -1;
	}

	if (optargs_count_by_index(opts, 0))
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
