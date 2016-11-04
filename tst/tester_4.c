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

static const char about[] = "skibidiwaynow!";

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_opt opts[] =
	{
		{
			.description = "An option with a named argument.",
			.long_option = "option1",
			.short_option = 'o',
			.argument =
			{
				.name = "FOOBAR :=",
				.description = "This is the argument.",
				.mandatory = optargs_yes
			}
		}, {
			.description = "Another option with a named argument.",
			.long_option = "option2",
			.short_option = 'O',
			.argument =
			{
				.name = "BARFOO",
				.description = "This is another argument.",
				.mandatory = optargs_maybe
			}
		}, {
			.description = "A third option with a named argument.",
			.long_option = "option3",
			.short_option = '0',
			.argument =
			{
				.name = "KEKEKE =>",
				.description = "This a third argument.",
				.mandatory = optargs_maybe
			}
		}, {
			.description = "A fourth option with a long name argument.",
				.long_option = "option3",
				.short_option = '0',
				.argument =
				{
					.name = "This-is-an-option-argument-with-a-really-long-name",
					.description = "This is the fourth argument.",
					.mandatory = optargs_maybe
				}
		}, {
			.description = "Fifth option.",
				.long_option = "option5",
				.short_option = '5',
				.argument =
				{
					.name = "YOMAMMA",
					.description = "This is the fifth option argument with a really long description that will wrap to multiple lines.",
					.mandatory = optargs_maybe
				}
		},
		optargs_opt_eol
	};

	const struct optargs_arg args[] =
	{
		{
			.name = "MY_ARGUMENT",
			.description = "Just some argument.",
			.mandatory = optargs_yes
		},
		optargs_arg_eol
	};

	optargs_print_help(av[0], about, opts, args);

	return EXIT_SUCCESS;
}
