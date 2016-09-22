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
	int verbosity, debug;
	const char *tmp;
	struct optargs_opt opts[] =
	{
		{
			.description = "Help text",
			.long_option = "help",
			.short_option = 'h',
		},
		{
			.description = "Be quiet.",
			.long_option = "quiet",
		},
		{
			.description = "Path to imaginary socket file.",
			.long_option = "socket",
			.argument = {
				.name = "file",
				.mandatory = optargs_yes
			}
		},
		{
			.description = "Be verbose.",
			.long_option = "verbose",
			.short_option = 'v',
			.argument = {
				.name = "level",
				.description = "The level of the desired verbosity.",
				.mandatory = optargs_maybe
			}
		},
		{
			.description = "Output debug info (can be given several times).",
			.short_option = 'd',
		},
		{
			.long_option = "secret-option",
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

	debug = optargs_is_default(optargs_opt_by_short(opts, 'd'));

	tmp = optargs_opt_by_long(opts, "verbose");
	if (!tmp)
		verbosity = 0;
	else if (optargs_is_default(tmp))
		verbosity = 100;
	else
		verbosity = atoi(tmp);

	if (!optargs_opt_by_long(opts, "quiet"))
	{
		printf("Doing stuff with %d%% verbosity.\n", verbosity);
		printf("Debug level defined to %d.\n", debug);
	}

	return 0;
}
