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

#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

int
main(int ac, char ** av)
{
	enum index
	{
		EKA,
		TOKA,
		TOKA_1,
		TOKA_2,
		DIV,
		TOKA_3,
		KOLMAS,
		NELJAS,
		NELJAS_1,
		NELJAS_2,
		NELJAS_3,
		_COUNT
	};
	struct optargs_opt opts[] =
	{
		{
			.long_option = "help",
			.description = "Help..."
		},
		optargs_opt_eol
	};
	struct optargs_arg args[] =
	{
		[EKA] = {
			.type = optargs_arg_any,
			.name = "EKA",
			.description = "Desc EKA."
		},
		[TOKA] = {
			.type = optargs_arg_group,
			.name = "TOKA",
			.description = "Desc TOKA."
		},
		[TOKA_1] = {
			.type = optargs_arg_group_member,
			.name = "TOKA.1",
			.description = "Desc TOKA.1."
		},
		[TOKA_2] = {
			.type = optargs_arg_group_member,
			.name = "TOKA.2",
			.description = "Desc TOKA.2."
		},
		optargs_arg_div,
		[TOKA_3] = {
			.type = optargs_arg_group_member,
			.name = "TOKA.3",
			.description = "Desc TOKA.3."
		},
		[KOLMAS] = {
			.type = optargs_arg_any_opt,
			.name = "KOLMAS",
			.description = "Desc KOLMAS."
		},
		[NELJAS] = {
			.type = optargs_arg_group_opt,
			.name = "NELJAS",
			.description = "Desc NELJAS."
		},
		[NELJAS_1] = {
			.type = optargs_arg_group_member,
			.name = "NELJAS.1",
			.description = "Desc NELJAS.1."
		},
		[NELJAS_2] = {
			.type = optargs_arg_group_member,
			.name = "NELJAS.2",
			.description = "Desc NELJAS.2."
		},
		[NELJAS_3] = {
			.type = optargs_arg_group_member,
			.name = "NELJAS.3",
			.description = "Desc NELJAS.3."
		},
		optargs_arg_sink
	};
	int t;

	if ((t = optargs_parse_opts(ac, (char const * const *)av, opts)) < 0)
		return EXIT_FAILURE;

	if (optargs_opt_res_by_long(opts, "help"))
	{
		optargs_print_help(av[0], "Yeah!", opts, args);
		return EXIT_SUCCESS;
	}

	if (optargs_parse_args(ac - t, (char const * const *)av + t, args) < 0)
		return EXIT_FAILURE;

	for (int i = 0 ; i < _COUNT ; i++)
		if (args[i].result.defined)
		{
			if (args[i].type == optargs_arg_group_member)
				printf("%s,", args[i].name);
			else
				printf("%s=%s,", args[i].name, optargs_arg_value(&args[i]));
		}
	printf("\n");

	return EXIT_SUCCESS;
}
