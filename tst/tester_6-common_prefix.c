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
	enum option
	{
		OPTION_HELP,
		OPTION_HELP_FULL,
		_OPTION_EOL
	};
	struct optargs_option opts[] =
	{
		[OPTION_HELP] = {
			"help",
			'h',
			"Print help.",
			optargs_undef,
			{NULL}
		},
		[OPTION_HELP_FULL] = {
			"help-full",
			'h',
			"Print full help.",
			optargs_undef,
			{NULL}
		},
		[_OPTION_EOL] = optargs_option_eol
	};

	if (optargs_parse_options(ac, (char const **)av, opts) < 0)
	{
		fprintf(stderr, "Something went wron while parsing.\n");
		return EXIT_FAILURE;
	}

	if (optargs_option_count(&opts[OPTION_HELP]))
		printf("got basic\n");

	if (optargs_option_count(&opts[OPTION_HELP_FULL]))
		printf("got full\n");

	return EXIT_SUCCESS;
}
