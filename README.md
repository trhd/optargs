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

In a nutshell:

	int main(int argc, char **argv)
	{
		struct optargs_opt opts[] =
		{
			{
				.description = "Print this help text.",
				.long_option = "help",
				.short_option = 'h',
				.argument = { .mandatory = optargs_no},
			},
			optargs_opt_eol
		};

		if (optargs_parse(argc, argv, opts))
		{
			printf("Some sort of an error occurred.");
			return -1;
		}

		if (optargs_opt_by_long(opts, "help"))
		{
			optargs_print_help(argv[0], "This is a really simple example.", opts, NULL);
			return 0;
		}

		// Do the boogie.
	}

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
optargs as a static library and OPTARGS_HDR to locate the optargs.h header
file's directory.

One can also compile optargs into their own program by just including the
optargs.c file in it. The compiler define NDEBUG can be used disable
optarg's internal assertions (see "man assert").


Reporting Bugs, Sending Patches and Giving Feedback
---------------------------------------------------

Contact the author e.g. via email.


Authors
-------

[Hemmo Nieminen](mailto:hemmo.nieminen@iki.fi)


Licensing
---------

See the file called LICENSE.
