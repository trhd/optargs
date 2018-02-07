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

static char const about[] = "skibidiwaynow!";

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_option opts[] =
	{
		{
			.description = "An option with a named argument.",
			.long_option = "option1",
			.short_option = 'o',
			.argument = (struct optargs_argument [])
			{

				{
					.name = "foobar :=",
					.description = "This is the argument.",
					.type = optargs_argument_any
				},
				optargs_argument_eol
			}
		}, {
			.description = "Another option with a named argument.",
			.long_option = "option2",
			.short_option = 'O',
			.argument = (struct optargs_argument [])
			{
				{
					.name = "barfoo",
					.description = "This is another argument.",
					.type = optargs_argument_any_opt
				},
				optargs_argument_eol
			}
		}, {
			.description = "A third option with a named argument.",
			.long_option = "option3",
			.short_option = '0',
			.argument = (struct optargs_argument [])
			{

				{
					.name = "kekeke =>",
					.description = "This a third argument.",
					.type = optargs_argument_any_opt
				},
				optargs_argument_eol
			}
		}, {
			.description = "A fourth option with a long name argument.",
			.long_option = "option3",
			.short_option = '0',
			.argument = (struct optargs_argument [])
			{
				{
					.name = "This-is-an-option-argument-with-a-really-long-name",
					.description = "This is the fourth argument.",
					.type = optargs_argument_any_opt
				},
				optargs_argument_eol
			}
		}, {
			.description = "Fifth option.",
			.long_option = "option5",
			.short_option = '5',
			.argument = (struct optargs_argument [])
			{
				{
					.name = "YOMAMMA",
					.description = "This is the fifth option argument with a really long description that will wrap to multiple lines.",
					.type = optargs_argument_any_opt
				},
				optargs_argument_eol
			}
		},
		optargs_option_eol
	};
	struct optargs_argument const args[] =
	{
		{
			.name = "MY_ARGUMENT",
			.description = "Just some argument.",
			.type = optargs_argument_any
		},
		optargs_argument_eol
	};

	optargs_print_help(av[0], about, opts, args);

	return EXIT_SUCCESS;
}
