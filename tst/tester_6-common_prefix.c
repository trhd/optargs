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

int
main(int ac, char **av)
{
	struct optargs_opt opts[] =
	{
		{
			"help",
			'h',
			"Print help.",
			optargs_arg_nil,
			optargs_result_nil
		}, {
			"help-full",
			'h',
			"Print full help.",
			optargs_arg_nil,
			optargs_result_nil
		},
		optargs_opt_eol
	};

	if (optargs_parse(ac, (char const **)av, opts) < 0)
	{
		fprintf(stderr, "Something went wron while parsing.\n");
		return EXIT_FAILURE;
	}

	if (optargs_result_by_long(opts, "help"))
		printf("got basic\n");

	if (optargs_result_by_long(opts, "help-full"))
		printf("got full\n");

	return EXIT_SUCCESS;
}
