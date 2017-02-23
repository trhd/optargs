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

#include <stdlib.h>
#include "optargs.h"

int
main(int ac, char ** av)
{
	struct optargs_opt opts[] =
	{
		{
			.long_option = "help",
			.short_option = 'h',
			.description = "Print help."
		},
		optargs_opt_eol
	};
	struct optargs_arg args[] =
	{
		{
			.type = optargs_arg_group,
			.name = "G1",
			.description = "Description of G1."
		},
		{
			.type = optargs_arg_group_member,
			.name = "g1m1",
			.description = "Description of g1m1.",
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group,
					//.name = "G1M1G1",
					//.description = "Description of G1M1G1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g1m1g1m1",
					.description = "Description of g1m1g1m1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g1m1g1m2",
					.description = "Description of g1m1g1m2."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g1m1g1m3",
					.description = "Description of g1m1g1m3."
				},
				optargs_arg_eol
			}
		},
		{
			.type = optargs_arg_group_member,
			.name = "g1m2",
			.description = "Description of g1m2.",
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group,
					.name = "G1M2G1",
					.description = "Description of G1M1G1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g1m2g1m1",
					.description = "Description of g1m2g1m1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g1m2g1m2",
					.description = "Description of g1m2g1m2."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g1m2g1m3",
					.description = "Description of g1m2g1m3."
				},
				optargs_arg_eol
			}
		},
		{
			.type = optargs_arg_group,
		},
		{
			.type = optargs_arg_group_member,
			.name = "g2m1",
			.description = "Description of g2m1.",
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group,
					.name = "G2M1G1",
					.description = "Description of G2M1G1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g2m1g1m1",
					.description = "Description of g2m1g1m1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g2m1g1m2",
					.description = "Description of g2m1g1m2."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g2m1g1m3",
					.description = "Description of g2m1g1m3."
				},
				optargs_arg_eol
			}
		},
		{
			.type = optargs_arg_group_member,
			.name = "g2m2",
			.description = "Description of g2m2.",
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group_opt,
					.name = "G2M2G1",
					.description = "Description of G2M1G1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g2m2g1m1",
					.description = "Description of g2m2g1m1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g2m2g1m2",
					.description = "Description of g2m2g1m2."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g2m2g1m3",
					.description = "Description of g2m2g1m3."
				},
				optargs_arg_eol
			}
		},
		{
			.type = optargs_arg_group_member,
			.name = "g2m3",
			.description = "Description of g2m3.",
		},
		{
			.type = optargs_arg_group,
			.name = "G3",
			.description = "Description of G3."
		},
		{
			.type = optargs_arg_group_member,
			.name = "g3m1",
			.description = "Description of g3m1.",
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group,
					.name = "G3M1G1",
					.description = "Description of G3M1G1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g3m1g1m1",
					.description = "Description of g3m1g1m1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g3m1g1m2",
					.description = "Description of g3m1g1m2."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g3m1g1m3",
					.description = "Description of g3m1g1m3."
				},
				optargs_arg_eol
			}
		},
		{
			.type = optargs_arg_group_member,
			.name = "g3m2",
			.description = "Description of g3m2.",
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group,
					.name = "G3M2G1",
					.description = "Description of G3M1G1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g3m2g1m1",
					.description = "Description of g3m2g1m1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g3m2g1m2",
					.description = "Description of g3m2g1m2."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g3m2g1m3",
					.description = "Description of g3m2g1m3."
				},
				optargs_arg_eol
			}
		},
		{
			.type = optargs_arg_group_opt,
		},
		{
			.type = optargs_arg_group_member,
			.name = "g4m1",
			.description = "Description of g4m1.",
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group,
					.name = "G4M1G1",
					.description = "Description of G4M1G1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g4m1g1m1",
					.description = "Description of g4m1g1m1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g4m1g1m2",
					.description = "Description of g4m1g1m2."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g4m1g1m3",
					.description = "Description of g4m1g1m3."
				},
				optargs_arg_eol
			}
		},
		{
			.type = optargs_arg_group_member,
			.name = "g4m2",
			.description = "Description of g4m2.",
			.subargument = (struct optargs_arg [])
			{
				{
					.type = optargs_arg_group,
				},
				{
					.type = optargs_arg_group_member,
					.name = "g4m2g1m1",
					.description = "Description of g4m2g1m1."
				},
				{
					.type = optargs_arg_group_member,
					.name = "g4m2g1m2",
					.description = "Description of g4m2g1m2.",
					.subargument = (struct optargs_arg [])
					{
						{
							.type = optargs_arg_any_opt,
							.name = "wtf",
							.description = "How is this even.."
						},
						optargs_arg_eol
					}
				},
				{
					.type = optargs_arg_group_member,
					.name = "g4m2g1m3",
					.description = "Description of g4m2g1m3."
				},
				optargs_arg_eol
			}
		},
		optargs_arg_eol
	};
	int t;

	if ((t = optargs_parse_opts(ac, (char const **)av, opts)) < 0)
		return EXIT_FAILURE;

	if (optargs_opt_res_by_long(opts, "help"))
	{
		optargs_print_help(av[0], "About this program...", opts, args);
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
