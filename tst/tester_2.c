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

static char const about[] = "foobar";

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_opt opts[] =
	{
		{ .description = "AAA", .short_option = 'a', .long_option = "aaa" },
		{ .short_option = 'x', .long_option = "secret" },
		{ .description = "BBB", .short_option = 'b', .long_option = "bbb" },
		optargs_opt_div,
		{ .description = "CCC", .short_option = 'c', .long_option = "ccc" },
		optargs_opt_eol
	};
	struct optargs_arg args[] =
	{
		{ .name = "Name_0", .type = optargs_arg_any, .description = "Jottain 0." },
		{ .name = "Name_1", .type = optargs_arg_any, .description = "Jottain 1." },
		optargs_arg_eol
	};

	optargs_print_help(av[0], about, opts, args);

	return EXIT_SUCCESS;
}
