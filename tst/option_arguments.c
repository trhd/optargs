/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2018 Hemmo Nieminen
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
	int idx;
	enum option
	{
		OPTION_A,
		OPTION_C,
		OPTION_HELP,
		OPTION_B,
		_OPTION_EOL
	};
	struct optargs_option opts[] =
	{
		[OPTION_HELP] = {
			.description = "Halp!",
			.long_option = "help",
			.short_option = 'h'
		},
		[OPTION_A] = {
			.description = "desc a.",
			.long_option = "aaaa",
			.short_option = 'a'
		},
		[OPTION_B] = {
			.description = "desc b.",
			.long_option = "bbbb",
			.short_option = 'b',
			.argument = (struct optargs_argument [])
			{
				{
					.name = "barg",
					.description = "desc barg",
					.type = optargs_argument_group
				},
				{
					.name = "b1",
					.description = "desc b1",
					.type = optargs_argument_group_member
				},
				{
					.name = "b2",
					.description = "desc b2",
					.type = optargs_argument_group_member
				},
				optargs_argument_eol
			}
		},
		[OPTION_C] = {
			.description = "desc c.",
			.long_option = "cccc",
			.short_option = 'c',
			.argument = (struct optargs_argument [])
			{
				{
					.name = "carg",
					.description = "desc carg",
					.type = optargs_argument_group_opt
				},
				{
					.name = "c1",
					.description = "desc c1",
					.type = optargs_argument_group_member
				},
				{
					.name = "c2",
					.description = "desc c2",
					.type = optargs_argument_group_member
				},
				optargs_argument_eol
			}
		},
		[_OPTION_EOL] = optargs_option_eol
	};
	struct optargs_argument args[] =
	{
		{
			.name = "something",
			.type = optargs_argument_any,
			.description = "Something something. Something something."
		},
		optargs_argument_eol
	};

	if ((idx = optargs_parse_options(ac, (char const **)av, opts)) < 0)
		return EXIT_FAILURE;

	if (optargs_option_count(opts, OPTION_HELP))
	{
		optargs_print_help(av[0], "Töttöröö!", opts, args);
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
