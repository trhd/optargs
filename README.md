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
			{
				.description = "Be quiet.",
				.short_option = 'q',
				.argument = { .mandatory = optargs_no},
			},
			{
				.description = "Be verbose.",
				.long_option = "verbose",
				.argument = { .mandatory = optargs_no},
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

		printf("Do the hustle!\n");

		return 0;
	}

and get something like this:

	$ b/tst/readme -h
	Usage: b/tst/readme [OPTIONS] [COMMAND]

	OPTIONS:
	  -h,--help           Help text
	  -q                  Be quiet.
	  --verbose           Be verbose.

	COMMAND:
	  start               Start doing stuff.
	  stop                Stop doing stuff.

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
