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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

int
main(int ac, char **av)
{
	struct optargs_option opts[] =
	{
		{ .long_option = "verbose", .short_option = 'v' },
		{ .long_option = "wanha", .short_option = 'w' },
		optargs_option_eol
	};
	unsigned t;

	if (optargs_parse_options(ac, (char const **)av, opts) < 0)
	{
		printf("WTF m8!?!?\n");
		return EXIT_FAILURE;
	}

	printf("Got: ");

	t = optargs_option_count(opts, 0);
	if (t)
		printf("v=%u ", t);

	t = optargs_option_count(opts, 1);
	if (t)
		printf("w=%u ", t);

	printf("\n");

	return EXIT_SUCCESS;
}
