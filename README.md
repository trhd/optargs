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

	#include "optargs.h"

	int
	main(int ac, char ** av)
	{
		struct optargs_option opts[] =
		{
			{
				.long_option = "help",
				.description = "Print help."
			},
			optargs_option_eol
		};

		if (optargs_parse_options(ac, (char const **)av, opts) < 0)
			return -1;

		if (optargs_option_type(opts, 0))
			optargs_print_help(av[0], 0, opts, 0);

		return 0;
	}

If your needs are a bit more demanding something more like the example
below might be more to your interest.

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
			_OPTION_COUNT
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

			[_OPTION_COUNT] = optargs_option_eol
		};
		enum
		{
			COMMAND,
			COMMAND_START,
			COMMAND_STOP,
			_COMMAND_COUNT
		};
		struct optargs_argument args[] =
		{
			[COMMAND] =
			{
				.name = "COMMAND",
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

			[_COMMAND_COUNT] = optargs_argument_eol
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

In the above case you would then get something like this:

	$ b/tst/readme -h
	Usage: b/tst/readme [OPTIONS] COMMAND

	OPTIONS:
	  -h,--help              Help text
	  --quiet                Be quiet.
	  --socket FILE          Path to an imaginary socket file.
	  -v,--verbose[=LEVEL]   Be verbose.
	    LEVEL                 The level of the desired verbosity.
	  -d                     Output debug info (can be given several times).

	COMMAND:
	  start [WHEN]           Start doing stuff.
	   WHEN:                  When to start doing stuff.
	    now                    Start doing stuff now.
	    then                   Start doing stuff later.
	  stop                   Stop doing stuff.

	About this program. This is the most awesomest program ever written and you
	ought to know that.
	$ b/tst/readme -v=13 -ddd --socket demo.socket start nwo
	Invalid argument 'nwo' for 'WHEN'.
	Something went wrong when parsing arguments.
	$ b/tst/readme -v=13 -ddd --socket demo.socket start now
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

If used as a meson subproject one should use the declared optargs
dependency object from the variable `dep`.

It should be pretty straightforward to just include the `optargs.c` in
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
