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

static char const about[] = "foobar";

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_option opts[] =
	{
		{ .description = "AAA", .short_option = 'a', .long_option = "aaa" },
		{ .short_option = 'x', .long_option = "secret" },
		{ .description = "BBB", .short_option = 'b', .long_option = "bbb" },
		optargs_option_divider,
		{ .description = "CCC", .short_option = 'c', .long_option = "ccc" },
		optargs_option_eol
	};
	struct optargs_argument args[] =
	{
		{ .name = "Name_0", .type = optargs_argument_any, .description = "Jottain 0." },
		{ .name = "Name_1", .type = optargs_argument_any, .description = "Jottain 1." },
		optargs_argument_eol
	};

	optargs_print_help(av[0], about, opts, args);

	return EXIT_SUCCESS;
}
