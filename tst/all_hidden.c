/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016-2017 Hemmo Nieminen
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
	struct optargs_opt opts[] =
	{
		{
			.long_option = "hidden-option",
		},
		{
			.long_option = "second-hidden-option",
		},
		optargs_opt_eol
	};
	struct optargs_arg args[] =
	{
		{
			.type = optargs_arg_group_opt,
			.name = "ALTERNATIVE-COMMAND"
		},
		{
			.type = optargs_arg_group_member,
			.name = "help",
			.description = "Print help."
		},
		optargs_arg_eol
	};
	int t;

	if ((t = optargs_parse_opts(ac, (char const **)av, opts)) < 0
			|| optargs_parse_args(ac - t, (char const **)av + t, args) < 0)
		return -1;

	if (optargs_opt_res_by_long(opts, "hidden-option"))
		printf("hidden option invoked\n");

	if (optargs_opt_res_by_long(opts, "second-hidden-option"))
		printf("second hidden option invoked\n");

	if (optargs_arg_value(&args[0]))
		optargs_print_help(av[0], NULL, opts, args);

	return 0;
}
