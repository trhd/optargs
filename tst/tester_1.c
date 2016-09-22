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

static const char about[] =
"1. This is a tester program for optargs. "
"2. This is a tester program for optargs. "
"3. This is a tester program for optargs."
"\n"
"4. This is a tester program for optargs. "
"5. This is a tester program for optargs. "
"6. This is a tester program for optargs. "
"7. This is a tester program for optargs. "
"8. This is a tester program for optargs. "
"9. This is a tester program for optargs. "
"10. This is a tester program for optargs. "
"11. This is a tester program for optargs. "
"12. This is a tester program for optargs."
"\n"
"\n"
"13. This is a tester program for optargs. "
"14. This is a tester program for optargs. "
"15. This is a tester program for optargs. "
"16. This is a tester program for optargs. "
"17. This is a tester program for optargs."
"\n"
"18. This is a tester program for optargs. "
"19. This is a tester program for optargs. "
"20. This is a tester program for optargs. "
"21. This is a tester program for optargs. "
"22. This is a tester program for optargs. "
"23. This is a tester program for optargs."
"\n"
"\n"
"24. This is a tester program for optargs. "
"25. This is a tester program for optargs. "
"26. This is a tester program for optargs."
"\n"
"27. This is a tester program for optargs. "
"28. This is a tester program for optargs. "
"29. This is a tester program for optargs. "
"30. This is a tester program for optargs."
;

int
main(int ac, char **av)
{
	int idx;
	const char *t;

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
		optargs_opt_nil,
		{
			.description = "Description I.",
			.long_option = "iiii",
			.short_option = 'i',
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "Description I2.",
			.long_option = "iiii2",
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "Description I3.",
			.long_option = NULL,
			.short_option = 'I',
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "Description J.",
			.long_option = "jjjj",
			.short_option = 'j',
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "Description K.",
			.long_option = NULL,
			.short_option = 'k',
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "Description L.",
			.long_option = "llll",
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "Description M.",
			.long_option = "mmmmmmmmmmmmmmmmmmmm",
			.short_option = 'm',
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "This is a super long option description that will have to be printed on several lines as it is too long to fit on only one line and we thus need to test that in this kinds of situations the descriptions are printed nicely to several lines with proper indentation.",
			.long_option = "nnnnnnnn",
			.short_option = 'n',
			.argument = { .mandatory = optargs_maybe },
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
		optargs_opt_nil,
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
		optargs_opt_nil,
		{
			.description = "A crazy long option #0.1.",
			.long_option = "option-lt-LEFT_COLUMN_MAX_WIDTH",
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #0.2.",
			.long_option = "option-lt-LEFT_COLUMN_MAX_WIDTHx",
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #1.",
			.long_option = "option-eq-LEFT_COLUMN_MAX_WIDTH-args",
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #2.",
			.long_option = "option-args-just-less-than-in-above",
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #3.",
			.long_option = "option-args-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.argument = { .mandatory = optargs_yes },
		}, {
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #0.1.",
			.long_option = "opti-lt-LEFT_COLUMN_MAX_WIDTH",
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "A crazy long option #0.2.",
			.long_option = "opti-lt-LEFT_COLUMN_MAX_WIDTHx",
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "A crazy long option #1.",
			.long_option = "option-eq-LEFT_COLUMN_MAX_WIDTH-opti",
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "A crazy long option #2.",
			.long_option = "option-opti-just-less-than-in-above",
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "A crazy long option #3.",
			.long_option = "option-opti-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.argument = { .mandatory = optargs_maybe },
		},
		optargs_opt_eol
	};
	struct optargs_arg args[] =
	{
		{ .name = "ARGUMENTS" },
		{ "Name_1", "Description 1.", optargs_no },
		{ "Name_3", "This is a super long argument description because blaa blaa balabla blaa blaa. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum.", optargs_maybe },
		optargs_arg_nil,
		{ "Name_2", "Description 2.", optargs_yes },
		{ "Name_4", "fooboo", optargs_yes },
		{ .name = "COMMANDS", .mandatory = optargs_yes },
		{ "command_1", "Command 1 is the shiiit!", optargs_maybe },
		{ "command_2", "Command 2 is the booomb!", optargs_yes },
		{ .name = "curse_words", .mandatory = optargs_yes },
		{ "vittu", "fuck / vag", optargs_no },
		{ "perkele", "god damn it / devil", optargs_maybe },
		{ "args with long name", NULL, optargs_yes },
		{ "argument usb-argument", "Some non-relevant description.", optargs_yes },
		{ "argument name eq LEFT_COLUMN_MAX_WIDTH", "Some non-relevant description #2.", optargs_yes },
		{ "argument name just less than in above", "Some non-relevant description #3.", optargs_no },
		{ "argument name greater than LEFT_COLUMN_MAX_WIDTH", "Some non-relevant description #4.", optargs_no },
		optargs_arg_eol
	};

	if ((idx = optargs_parse(ac, (const char **)av, opts)) < 0)
		return EXIT_FAILURE;

	if (optargs_opt_by_long(opts, "help"))
	{
		optargs_print_help(av[0], about, opts, args);
		return EXIT_SUCCESS;
	}

	printf("Got: ");

	if (optargs_opt_by_short(opts, 'a'))
		printf("a");

	if (optargs_opt_by_long(opts, "bbbb"))
		printf("b");

	if (optargs_opt_by_short(opts, 'c'))
		printf("c");

	if (optargs_opt_by_long(opts, "dddd"))
		printf("d");

	if (optargs_opt_by_short(opts, 'e'))
		printf("e");

	if (optargs_opt_by_long(opts, "ffff"))
		printf("f");

	if (optargs_opt_by_short(opts, 'g'))
		printf("g");

	if (optargs_opt_by_index(opts, 7))
		printf("h");

	if (optargs_opt_by_long(opts, "hidden-option"))
		printf("hidden!");

	if ((t = optargs_opt_by_short(opts, 'i')))
		printf("i{%s}", t);

	if ((t = optargs_opt_by_long(opts, "jjjj")))
	{
		if (optargs_is_default(t))
			t = "Oujea!";

		printf("j{%s}", t);
	}

	if (idx > 0)
		while (idx < ac)
			printf(",%s", av[idx++]);

	printf("\n");

	return EXIT_SUCCESS;
}
