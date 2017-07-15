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
#include <stdlib.h>
#include "optargs.h"

int
main(int ac, char **av)
{
	int idx;
	char const * t;

	struct optargs_opt opts[] =
	{
		{
			.description = "Description A.",
			.long_option = "aaaa",
			.short_option = 'a',
		}, {
			.description = "Description B.",
			.long_option = "bbbb",
			.short_option = 'b',
		}, {
			.description = "Description C.",
			.long_option = "cccc",
			.short_option = 'c',
		}, {
			.description = "Description D.",
			.long_option = "dddd",
			.short_option = 'd',
		}, {
			.description = "Description E.",
			.long_option = "eeee",
			.short_option = 'e',
		}, {
			.description = "Description F.",
			.long_option = "ffff",
			.short_option = 'f',
		}, {
			.description = "Description G.",
			.long_option = "gggg",
			.short_option = 'g',
		}, {
			.description = "Description H.",
			.long_option = "hhhh",
			.short_option = 'h',
		}, {
			.description = NULL,
			.long_option = "hidden-option",
		},
		optargs_opt_div,
		{
			.description = "Description I.",
			.long_option = "iiii",
			.short_option = 'i',
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any}, optargs_arg_eol},
		}, {
			.description = "Description I2.",
			.long_option = "iiii2",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any}, optargs_arg_eol},
		}, {
			.description = "Description I3.",
			.long_option = NULL,
			.short_option = 'I',
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any}, optargs_arg_eol},
		}, {
			.description = "Description J.",
			.long_option = "jjjj",
			.short_option = 'j',
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "Description K.",
			.long_option = NULL,
			.short_option = 'k',
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "Description L.",
			.long_option = "llll",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "Description M.",
			.long_option = "mmmmmmmmmmmmmmmmmmmm",
			.short_option = 'm',
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "This is a super long option description that will have to be printed on several lines as it is too long to fit on only one line and we thus need to test that in this kinds of situations the descriptions are printed nicely to several lines with proper indentation.",
			.long_option = "nnnnnnnn",
			.short_option = 'n',
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "Print help text.",
			.long_option = "help",
			.short_option = 'H',
		}, {
			.description = "SuperLongWordThatShouldExceedTheLineWidthSpecificationThatWillTestThatTheProgramCanHandleSuperLongWordsProperly.",
			.long_option = "foobar",
			.short_option = 'F',
		}, {
			.description = "Description O with some extra words to make the description a bit longer. A few more words. Just about to be long enough.",
			.long_option = "oooo",
			.short_option = 'o',
		},
		optargs_opt_div,
		{
			.description = "A crazy long option #1.",
			.long_option = "option-name-eq-LEFT_COLUMN_MAX_WIDTH",
		}, {
			.description = "A crazy long option #2.",
			.long_option = "option-name-just-less-than-in-above",
		}, {
			.description = "A crazy long option #3.",
			.long_option = "option-name-greater-than-LEFT_COLUMN_MAX_WIDTH",
		},
		optargs_opt_div,
		{
			.description = "A crazy long option #0.1.",
			.long_option = "option-lt-LEFT_COLUMN_MAX_WIDTH",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any}, optargs_arg_eol},
		}, {
			.description = "A crazy long option #0.2.",
			.long_option = "option-lt-LEFT_COLUMN_MAX_WIDTHx",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any}, optargs_arg_eol},
		}, {
			.description = "A crazy long option #1.",
			.long_option = "option-eq-LEFT_COLUMN_MAX_WIDTH-args",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any}, optargs_arg_eol},
		}, {
			.description = "A crazy long option #2.",
			.long_option = "option-args-just-less-than-in-above",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any}, optargs_arg_eol},
		}, {
			.description = "A crazy long option #3.",
			.long_option = "option-args-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any}, optargs_arg_eol},
		},
		optargs_opt_div,
		{
			.description = "A crazy long option #0.1.",
			.long_option = "opti-lt-LEFT_COLUMN_MAX_WIDTH",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "A crazy long option #0.2.",
			.long_option = "opti-lt-LEFT_COLUMN_MAX_WIDTHx",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "A crazy long option #1.",
			.long_option = "option-eq-LEFT_COLUMN_MAX_WIDTH-opti",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "A crazy long option #2.",
			.long_option = "option-opti-just-less-than-in-above",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		}, {
			.description = "A crazy long option #3.",
			.long_option = "option-opti-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.argument = (struct optargs_arg []){ {.name = "ARG", .type = optargs_arg_any_opt}, optargs_arg_eol},
		},
		optargs_opt_eol
	};
	struct optargs_res const * r;

	if ((idx = optargs_parse_opts(ac, (char const **)av, opts)) < 0)
		return EXIT_FAILURE;

	if (optargs_opt_count_by_long(opts, "help"))
	{
		optargs_print_help(av[0], "foobar", opts, NULL);
		return EXIT_SUCCESS;
	}

	printf("Got: ");

	if (optargs_opt_count_by_short(opts, 'a'))
		printf("a");

	if (optargs_opt_count_by_long(opts, "bbbb"))
		printf("b");

	if (optargs_opt_count_by_short(opts, 'c'))
		printf("c");

	if (optargs_opt_count_by_long(opts, "dddd"))
		printf("d");

	if (optargs_opt_count_by_short(opts, 'e'))
		printf("e");

	if (optargs_opt_count_by_long(opts, "ffff"))
		printf("f");

	if (optargs_opt_count_by_short(opts, 'g'))
		printf("g");

	if (optargs_opt_count_by_index(opts, 7))
		printf("h");

	if (optargs_opt_count_by_long(opts, "hidden-option"))
		printf("hidden!");

	if ((t = optargs_opt_value_by_short(opts, 'i')))
		printf("i{%s}", t);

	if ((r = optargs_opt_res_by_long(opts, "jjjj")))
	{
		if (r->type == optargs_count)
			t = "Oujea!";
		else
			t = r->value.string;

		printf("j{%s}", t);
	}

	if (idx > 0)
		while (idx < ac)
			printf(",%s", av[idx++]);

	printf("\n");

	return EXIT_SUCCESS;
}
