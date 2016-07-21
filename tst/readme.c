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

#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

#define ABOUT "About this program. This is the most awesomest program " \
	"ever written and you ought to know that."

int
main(int ac, char **av)
{
	int verbosity;
	const char *tmp;
	struct optargs_opt opts[] =
	{
		{
			.description = "Help text",
			.long_option = "help",
			.short_option = 'h',
			.argument = { .mandatory = optargs_no},
		},
		{
			.description = "Be quiet.",
			.short_option = 'q',
			.argument = { .mandatory = optargs_no},
		},
		{
			.description = "Be verbose.",
			.long_option = "verbose",
			.argument = {
				.name = "level",
				.description = "The level of the desired verbosity.",
				.mandatory = optargs_maybe
			}
		},
		{
			.long_option = "secret-option",
			.argument = { .mandatory = optargs_no},
		},
		optargs_opt_eol
	};
	struct optargs_arg args[] =
	{
		{ "COMMAND", NULL, optargs_yes },
		{ "start", "Start doing stuff.", optargs_no },
		{ "stop", "Stop doing stuff.", optargs_no },
		optargs_arg_eol
	};

	if (optargs_parse(ac, (const char **)av, opts) < 0)
	{
		printf("Something went wrong.\n");
		return EXIT_FAILURE;
	}

	if (optargs_opt_by_long(opts, "help"))
	{
		optargs_print_help(av[0], ABOUT, opts, args);
		return EXIT_SUCCESS;
	}

	tmp = optargs_opt_by_long(opts, "verbose");
	if (!tmp)
		verbosity = 0;
	else if (tmp == &optargs_default_result)
		verbosity = 100;
	else
		verbosity = atoi(tmp);

	printf("Doing stuff with %d%% verbosity.\n", verbosity);

	return 0;
}
