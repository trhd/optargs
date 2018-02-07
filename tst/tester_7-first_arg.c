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

int
main(int ac, char **av)
{
	int i;

	struct optargs_option opts[] =
	{
		{
			.description = "Description A.",
			.long_option = "aaaa",
			.short_option = 'a',
		}, {
			.description = "Description B.",
			.long_option = "bbbb",
			.short_option = 'b',
		}, {
			.description = "Description C.",
			.long_option = "cccc",
			.short_option = 'c',
		}, {
			.description = "Description D.",
			.long_option = "dddd",
			.short_option = 'd',
			.argument = (struct optargs_argument []){{.name = "d-arg", .type = optargs_argument_any}, optargs_argument_eol},
		}, {
			.description = "Description E.",
			.long_option = "eeee",
			.short_option = 'e',
			.argument = (struct optargs_argument []){{.name = "e-arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		optargs_option_eol
	};

	i = optargs_parse_options(ac, (char const **)av, opts);

	if (i < 0)
	{
		printf("ERROR: parsing options failed.\n");
		return EXIT_FAILURE;
	}

	printf("First argument: %s\n", i == ac ? "--" :  av[i]);

	return EXIT_SUCCESS;
}
