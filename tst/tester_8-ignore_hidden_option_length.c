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

#include <stdlib.h>
#include "optargs.h"

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_option opts[] =
	{
		{ .long_option = "opt", .description = "Blaablaa.", .short_option = 'b'},
		{ .long_option = "hidden-option-with-long-name", .short_option = 'h' },
		optargs_option_eol
	};

	optargs_print_help(av[0], NULL, opts, NULL);

	return EXIT_SUCCESS;
}
