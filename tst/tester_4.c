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

static const char about[] = "skibidiwaynow!";

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_opt opts[] =
	{
		{ "An option with a named argument.", "option1", 'o', { "FOOBAR :=", "This is the argument.", optargs_yes }, NULL },
		{ "Another option with a named argument.", "option2", 'O', { "BARFOO", "This is another argument.", optargs_maybe }, NULL },
		{ "A third option with a named argument.", "option3", '0', { "KEKEKE =>", "This a third argument.", optargs_maybe }, NULL },
		{ "A fourth option with a long name argument.", "option3", '0', { "This-is-an-option-argument-with-a-really-long-name", "This is the fourth argument.", optargs_maybe }, NULL },
		optargs_opt_eol
	};

	const struct optargs_arg args[] =
	{
		{ "MY_ARGUMENT", "Just some argument.", optargs_yes },
		optargs_arg_eol
	};

	optargs_print_help(av[0], about, opts, args);

	return EXIT_SUCCESS;
}
