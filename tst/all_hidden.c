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
#include "optargs.h"

int
main(int ac, char ** av)
{
	struct optargs_option opts[] =
	{
		{
			.long_option = "hidden-option",
		},
		{
			.long_option = "second-hidden-option",
		},
		optargs_option_eol
	};
	struct optargs_argument args[] =
	{
		{
			.type = optargs_argument_group_opt,
			.name = "ALTERNATIVE-COMMAND"
		},
		{
			.type = optargs_argument_group_member,
			.name = "help",
			.description = "Print help."
		},
		optargs_argument_eol
	};
	int t;

	if ((t = optargs_parse_options(ac, (char const **)av, opts)) < 0
			|| optargs_parse_arguments(ac - t, (char const **)av + t, args) < 0)
		return -1;

	if (optargs_option_type(&opts[0]))
		printf("hidden option invoked\n");

	if (optargs_option_count(&opts[1]))
		printf("second hidden option invoked\n");

	if (optargs_argument_value(&args[0]))
		optargs_print_help(av[0], NULL, opts, args);

	return 0;
}
