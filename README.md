[![Build Status](https://travis-ci.org/trhd/optargs.svg)](https://travis-ci.org/trhd/optargs)

optargs
=======

What Is Optargs
---------------

Optargs is an option and argument management library with the goal of
being easy-to-use and able of providing visually pleasant, informative and
useful help texts for command line applications with as little input from
the developer as possible.


Why Optargs
-----------

Existing argument parsing/management solutions were seen as unnecessarily
complicated or lacking on the help text generation side.


How To Use Optargs
------------------

To use, write something like this:

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

			optargs_arg_eol
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

and get something like this:

	$ readme -h
	Usage: b/tst/readme [OPTIONS] COMMAND

	OPTIONS:
	  -h,--help              Help text
	  --quiet                Be quiet.
	  --socket file          Path to an imaginary socket file.
	  -v,--verbose[=level]   Be verbose.
	    level                 The level of the desired verbosity.
	  -d                     Output debug info (can be given several times).

	COMMAND:
	  start [WHEN]           Start doing stuff.
	   WHEN:                  When to start doing stuff.
	    now                    Start doing stuff now.
	    then                   Start doing stuff later.
	  stop                   Stop doing stuff.

	About this program. This is the most awesomest program ever written and you
	ought to know that.
	$ readme -v=13 -ddd --socket demo.socket start nwo
	Invalid argument 'nwo' for WHEN.
	Something went wrong when parsing arguments.
	$ readme -v=13 -ddd --socket demo.socket start now
	Doing stuff with 13% verbosity.
	Debug level defined to 3.
	Socket file: demo.socket.
	Start doing stuff 'now'.

See optargs' tests for more complex usage examples. The optargs.h header
file should provide all the needed documentation.


Building Optargs
----------------

Optargs supports the Meson build tool.

To build and install optargs shared library (using ninja backend):

	# To "configure" and build.
	$ meson build
	$ ninja -C build

	# To run optargs' test suite.
	$ ninja -C build test

	# To install the library to the system.
	$ sudo ninja -C build install

If used as a meson subproject one can use the variables `OPTARGS_BIN` and
`OPTARGS_LIB` to access optargs as a static and dynamic library,
`OPTARGS_HDR` to locate the optargs' header files' directory and
`OPTARGS_SRC` to access the optargs' main source file.

It should be pretty straight forward to just include the `optargs.c` in
one's own project as well if needed.


Reporting Bugs, Sending Patches and Giving Feedback
---------------------------------------------------

Contact the author e.g. via email.


Authors
-------

[Hemmo Nieminen](mailto:hemmo.nieminen@iki.fi)


Licensing
---------

See the file called LICENSE.
