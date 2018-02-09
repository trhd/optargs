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

#define ABOUT "About this program. This is the most awesomest program " \
	"ever written and you ought to know that."

int
main(int ac, char ** av)
{
	int verbosity, debug, t;
	char const * str, * when;
	enum
	{
		OPTION_HELP,
		OPTION_QUIET,
		OPTION_SOCKET,
		OPTION_VERBOSE,
		OPTION_DEBUG,
		OPTION_SECRET,
		_OPTION_EOL
	};
	struct optargs_option opts[] =
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
			.argument = (struct optargs_argument [])
			{
				{
					.name = "file",
					.type = optargs_argument_any
				},
				optargs_argument_eol
			}
		},

		[OPTION_VERBOSE] =
		{
			.description = "Be verbose.",
			.long_option = "verbose",
			.short_option = 'v',
			.argument = (struct optargs_argument [])
			{
				{
					.name = "level",
					.description = "The level of the desired verbosity.",
					.type = optargs_argument_any_opt
				},
				optargs_argument_eol
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

		[_OPTION_EOL] = optargs_option_eol
	};
	enum
	{
		COMMAND,
		COMMAND_START,
		COMMAND_STOP,
		_COMMAND_EOL
	};
	struct optargs_argument args[] =
	{
		[COMMAND] =
		{
			.name = "command",
			.type = optargs_argument_group
		},

		[COMMAND_START] =
		{
			.name = "start",
			.description = "Start doing stuff.",
			.type = optargs_argument_group_member,
			.subargument = (struct optargs_argument [])
			{
				{
					.type = optargs_argument_group_opt,
					.name = "when",
					.description = "When to start doing stuff."
				},
				{
					.type = optargs_argument_group_member,
					.name = "now",
					.description = "Start doing stuff now."
				},
				{
					.type = optargs_argument_group_member,
					.name = "then",
					.description = "Start doing stuff later."
				},
				optargs_argument_eol
			}
		},

		[COMMAND_STOP] = {
			.name = "stop",
			.description = "Stop doing stuff.",
			.type = optargs_argument_group_member
		},

		[_COMMAND_EOL] = optargs_argument_eol
	};

	if ((t = optargs_parse_options(ac, (char const **)av, opts)) < 0)
	{
		printf("Something went wrong when parsing options.\n");
		return EXIT_FAILURE;
	}

	if (optargs_option_count(opts, OPTION_HELP))
	{
		optargs_print_help(av[0], ABOUT, opts, args);
		return EXIT_SUCCESS;
	}

	if (optargs_parse_arguments(ac - t, (char const **)av + t, args) < 0)
	{
		printf("Something went wrong when parsing arguments.\n");
		return EXIT_FAILURE;
	}

	debug = optargs_option_count(opts, OPTION_DEBUG);

	switch (optargs_option_type(opts, OPTION_VERBOSE))
	{
		case optargs_argument:
			verbosity = atoi(optargs_option_string(opts, OPTION_VERBOSE));
			break;
		case optargs_flag:
			verbosity = 100;
			break;
		default:
			verbosity = 0;
	}

	if (!optargs_option_count(opts, OPTION_QUIET))
	{
		printf("Doing stuff with %d%% verbosity.\n", verbosity);
		printf("Debug level defined to %d.\n", debug);

		str = optargs_option_string(opts, OPTION_SOCKET);
		if (str)
			printf("Socket file: %s.\n", str);
	}

	switch (optargs_argument_index(args, COMMAND))
	{
		case COMMAND_START:
			when = optargs_argument_value(&args[COMMAND_START].subargument[0]);
			if (when)
				printf("Start doing stuff '%s'.\n", when);
			else
				printf("Casually starting stuff whenever...\n");
			break;
		case COMMAND_STOP:
			printf("Stopping stuff.\n");
			break;
		default:
			printf("Nope, optargs_parse_arguments() would have returned an error.\n");
			break;
	}

	return EXIT_SUCCESS;
}
