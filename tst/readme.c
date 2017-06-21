/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016-2017 Hemmo Nieminen
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
main(int ac, char ** av)
{
	int verbosity, debug, t;
	char const * str, * when;
	struct optargs_res const * res;
	enum
	{
		OPTION_HELP,
		OPTION_QUIET,
		OPTION_SOCKET,
		OPTION_VERBOSE,
		OPTION_DEBUG,
		OPTION_SECRET,
		_OPTION_COUNT
	};
	struct optargs_opt opts[] =
	{
		[OPTION_HELP] =
		{
			.description = "Help text",
			.long_option = "help",
			.short_option = 'h',
		},

		[OPTION_QUIET] =
		{
			.description = "Be quiet.",
			.long_option = "quiet",
		},

		[OPTION_SOCKET] =
		{
			.description = "Path to an imaginary socket file.",
			.long_option = "socket",
			.argument = (struct optargs_arg [])
			{
				{
					.name = "file",
					.type = optargs_arg_any
				},
				optargs_arg_eol
			}
		},

		[OPTION_VERBOSE] =
		{
			.description = "Be verbose.",
			.long_option = "verbose",
			.short_option = 'v',
			.argument = (struct optargs_arg [])
			{
				{
					.name = "level",
					.description = "The level of the desired verbosity.",
					.type = optargs_arg_any_opt
				},
				optargs_arg_eol
			}
		},

		[OPTION_DEBUG] =
		{
			.description = "Output debug info (can be given several times).",
			.short_option = 'd',
		},

		[OPTION_SECRET] =
		{
			.long_option = "secret-option",
		},

		[_OPTION_COUNT] = optargs_opt_eol
	};
	enum
	{
		COMMAND,
		COMMAND_START,
		COMMAND_STOP,
		_COMMAND_COUNT
	};
	struct optargs_arg args[] =
	{
		[COMMAND] =
		{
			.name = "COMMAND",
			.type = optargs_arg_group
		},

		[COMMAND_START] =
		{
			.name = "start",
			.description = "Start doing stuff.",
			.type = optargs_arg_group_member,
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group_opt,
					.name = "WHEN",
					.description = "When to start doing stuff."
				},
				{
					.type = optargs_arg_group_member,
					.name = "now",
					.description = "Start doing stuff now."
				},
				{
					.type = optargs_arg_group_member,
					.name = "then",
					.description = "Start doing stuff later."
				},
				optargs_arg_eol
			}
		},

		[COMMAND_STOP] = {
			.name = "stop",
			.description = "Stop doing stuff.",
			.type = optargs_arg_group_member
		},

		[_COMMAND_COUNT] = optargs_arg_eol
	};

	if ((t = optargs_parse_opts(ac, (char const **)av, opts)) < 0)
	{
		printf("Something went wrong when parsing options.\n");
		return EXIT_FAILURE;
	}

	if (optargs_opt_count_by_long(opts, "help"))
	{
		optargs_print_help(av[0], ABOUT, opts, args);
		return EXIT_SUCCESS;
	}

	if (optargs_parse_args(ac - t, (char const **)av + t, args) < 0)
	{
		printf("Something went wrong when parsing arguments.\n");
		return EXIT_FAILURE;
	}

	debug = optargs_opt_count_by_short(opts, 'd');

	res = optargs_opt_res_by_index(opts, OPTION_VERBOSE);
	if (!res)
		verbosity = 0;
	else if (optargs_res_type(res) == optargs_count)
		verbosity = 100;
	else
		verbosity = atoi(res->value.string);

	if (!optargs_opt_count_by_index(opts, OPTION_QUIET))
	{
		printf("Doing stuff with %d%% verbosity.\n", verbosity);
		printf("Debug level defined to %d.\n", debug);

		str = optargs_opt_value_by_index(opts, OPTION_SOCKET);
		if (str)
			printf("Socket file: %s.\n", str);
	}

	switch (optargs_arg_value_index(args, COMMAND))
	{
		case COMMAND_START:
			when = optargs_arg_value(&args[COMMAND_START].subargument[0]);
			if (when)
				printf("Start doing stuff '%s'.\n", when);
			else
				printf("Casually starting stuff whenever...\n");
			break;
		case COMMAND_STOP:
			printf("Stopping stuff.\n");
			break;
		default:
			printf("Nope, optargs_parse_args() would have returned an error.\n");
			break;
	}

	return EXIT_SUCCESS;
}
