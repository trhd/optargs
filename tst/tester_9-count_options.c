/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016-2017 Hemmo Nieminen
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

int
main(int ac, char **av)
{
	struct optargs_opt opts[] =
	{
		{ .long_option = "verbose", .short_option = 'v' },
		{ .long_option = "wanha", .short_option = 'w' },
		optargs_opt_eol
	};
	unsigned t;

	if (optargs_parse_opts(ac, (char const **)av, opts) < 0)
	{
		printf("WTF m8!?!?\n");
		return EXIT_FAILURE;
	}

	printf("Got: ");

	t = optargs_opt_count_by_long(opts, "verbose");
	if (t)
		printf("v=%u ", t);

	t = optargs_opt_count_by_long(opts, "wanha");
	if (t)
		printf("w=%u ", t);

	printf("\n");

	return EXIT_SUCCESS;
}
