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
		int verbosity, debug;
		char const * str;
		struct optargs_result const * res;
		enum option
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
				.argument =
				{
					.name = "file",
					.mandatory = optargs_yes
				}
			},

			[OPTION_VERBOSE] =
			{
				.description = "Be verbose.",
				.long_option = "verbose",
				.short_option = 'v',
				.argument =
				{
					.name = "level",
					.description = "The level of the desired verbosity.",
					.mandatory = optargs_maybe
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
		struct optargs_arg args[] =
		{
			{
				.name = "COMMAND",
				.mandatory = optargs_yes
			}, {
				.name = "start",
				.description = "Start doing stuff.",
				.mandatory = optargs_no
			}, {
				.name = "stop",
				.description = "Stop doing stuff.",
				.mandatory = optargs_no
			},
			optargs_arg_eol
		};

		if (optargs_parse(ac, (char const **)av, opts) < 0)
		{
			printf("Something went wrong.\n");
			return EXIT_FAILURE;
		}

		if (optargs_count_by_long(opts, "help"))
		{
			optargs_print_help(av[0], ABOUT, opts, args);
			return EXIT_SUCCESS;
		}

		debug = optargs_count_by_short(opts, 'd');

		res = optargs_result_by_index(opts, OPTION_VERBOSE);
		if (!res)
			verbosity = 0;
		else if (optargs_result_type(res) == optargs_count)
			verbosity = 100;
		else
			verbosity = atoi(res->value.string);

		if (!optargs_count_by_index(opts, OPTION_QUIET))
		{
			printf("Doing stuff with %d%% verbosity.\n", verbosity);
			printf("Debug level defined to %d.\n", debug);

			str = optargs_string_by_index(opts, OPTION_SOCKET);
			if (str)
				printf("Socket file: %s.\n", str);
		}

		return EXIT_SUCCESS;
	}

and get something like this:

	Usage: ./readme [OPTIONS] COMMAND

	OPTIONS:
	  -h,--help             Help text
	  --quiet               Be quiet.
	  --socket file         Path to imaginary socket file.
	  -v,--verbose[=level]  Be verbose.
	                        level: The level of the desired verbosity.
	  -d                    Output debug info (can be given several times).

	COMMAND:
	  start                 Start doing stuff.
	  stop                  Stop doing stuff.

	About this program. This is the most awesomest program ever written and you
	ought to know that.

See optargs' tests for more complex usage examples. The optargs.h header
file should provide all the needed documentation.


Building Optargs
----------------

Optargs supports the Meson build tool.

To build and install optargs shared library (using ninja backend):

	$ mkdir build
	$ meson build
	$ ninja -C build

	# To run optargs' test suite.
	$ ninja -C build test

	# To install the library to the system.
	$ sudo ninja -C build install

If used as a meson subproject one can use the variables OPTARGS_BIN to get
optargs as a static library, OPTARGS_HDR to locate the optargs.h header
file's directory and OPTARGS_SRC to access the optargs' main source file.

One can also compile optargs into their own program by just including the
optargs.c file in it. The compiler define NDEBUG can be used disable
optarg's internal assertions (see "man 3 assert").


Reporting Bugs, Sending Patches and Giving Feedback
---------------------------------------------------

Contact the author e.g. via email.


Authors
-------

[Hemmo Nieminen](mailto:hemmo.nieminen@iki.fi)


Licensing
---------

See the file called LICENSE.
