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

static char const about[] =
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
	char const * t;

	enum option_index
	{
		OPTION_A,
		OPTION_B,
		OPTION_C,
		OPTION_D,
		OPTION_E,
		OPTION_F,
		OPTION_G,
		OPTION_H,
		OPTION_HIDDEN,
		OPTION_DIV_1,
		OPTION_I1,
		OPTION_I2,
		OPTION_I3,
		OPTION_J,
		OPTION_K,
		OPTION_L,
		OPTION_M,
		OPTION_LONG_1,
		OPTION_HELP,
		OPTION_LONG_2,
		OPTION_LONG_3,
		OPTION_DIV_2,
		OPTION_CRAZY_1,
		OPTION_CRAZY_2,
		OPTION_CRAZY_3,
		OPTION_DIV_3,
		OPTION_CRAZY_4,
		OPTION_CRAZY_5,
		OPTION_CRAZY_6,
		OPTION_CRAZY_7,
		OPTION_CRAZY_8,
		OPTION_DIV_4,
		OPTION_CRAZY_9,
		OPTION_CRAZY_10,
		OPTION_CRAZY_11,
		OPTION_CRAZY_12,
		OPTION_CRAZY_13,
		_OPTION_EOL
	};

	struct optargs_option opts[] =
	{
#ifndef NO_OPTS
		[OPTION_A] = {
			.description = "Description A.",
			.long_option = "aaaa",
			.short_option = 'a',
		},
		[OPTION_B] = {
			.description = "Description B.",
			.long_option = "bbbb",
			.short_option = 'b',
		},
		[OPTION_C] = {
			.description = "Description C.",
			.long_option = "cccc",
			.short_option = 'c',
		},
		[OPTION_D] = {
			.description = "Description D.",
			.long_option = "dddd",
			.short_option = 'd',
		},
		[OPTION_E] = {
			.description = "Description E.",
			.long_option = "eeee",
			.short_option = 'e',
		},
		[OPTION_F] = {
			.description = "Description F.",
			.long_option = "ffff",
			.short_option = 'f',
		},
		[OPTION_G] = {
			.description = "Description G.",
			.long_option = "gggg",
			.short_option = 'g',
		},
		[OPTION_H] = {
			.description = "Description H.",
			.long_option = "hhhh",
			.short_option = 'h',
		},
		[OPTION_HIDDEN] = {
			.description = NULL,
			.long_option = "hidden-option",
		},
		[OPTION_DIV_1] = optargs_option_divider,
		[OPTION_I1] = {
			.description = "Description I.",
			.long_option = "iiii",
			.short_option = 'i',
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any}, optargs_argument_eol},
		},
		[OPTION_I2] = {
			.description = "Description I2.",
			.long_option = "iiii2",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any}, optargs_argument_eol},
		},
		[OPTION_I3] = {
			.description = "Description I3.",
			.long_option = NULL,
			.short_option = 'I',
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any}, optargs_argument_eol},
		},
		[OPTION_J] = {
			.description = "Description J.",
			.long_option = "jjjj",
			.short_option = 'j',
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		[OPTION_K] = {
			.description = "Description K.",
			.long_option = NULL,
			.short_option = 'k',
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		[OPTION_L] = {
			.description = "Description L.",
			.long_option = "llll",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		[OPTION_M] = {
			.description = "Description M.",
			.long_option = "mmmmmmmmmmmmmmmmmmmm",
			.short_option = 'm',
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		[OPTION_LONG_1] = {
			.description = "This is a super long option description that will have to be printed on several lines as it is too long to fit on only one line and we thus need to test that in this kinds of situations the descriptions are printed nicely to several lines with proper indentation.",
			.long_option = "nnnnnnnn",
			.short_option = 'n',
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
#endif
		[OPTION_HELP] = {
			.description = "Print help text.",
			.long_option = "help",
			.short_option = 'H',
		},
#ifndef NO_OPTS
		[OPTION_LONG_2] = {
			.description = "SuperLongWordThatShouldExceedTheLineWidthSpecificationThatWillTestThatTheProgramCanHandleSuperLongWordsProperly.",
			.long_option = "foobar",
			.short_option = 'F',
		},
		[OPTION_LONG_3] = {
			.description = "Description O with some extra words to make the description a bit longer. A few more words. Just about to be long enough.",
			.long_option = "oooo",
			.short_option = 'o',
		},
		[OPTION_DIV_2] = optargs_option_divider,
		[OPTION_CRAZY_1] =  {
			.description = "A crazy long option #1.",
			.long_option = "option-name-eq-LEFT_COLUMN_MAX_WIDTH",
		},
		[OPTION_CRAZY_2] = {
			.description = "A crazy long option #2.",
			.long_option = "option-name-just-less-than-in-above",
		},
		[OPTION_CRAZY_3] = {
			.description = "A crazy long option #3.",
			.long_option = "option-name-greater-than-LEFT_COLUMN_MAX_WIDTH",
		},
		[OPTION_DIV_3] = optargs_option_divider,
		[OPTION_CRAZY_4] = {
			.description = "A crazy long option #0.1.",
			.long_option = "option-lt-LEFT_COLUMN_MAX_WIDTH",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any}, optargs_argument_eol},
		},
		[OPTION_CRAZY_5] = {
			.description = "A crazy long option #0.2.",
			.long_option = "option-lt-LEFT_COLUMN_MAX_WIDTHx",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any}, optargs_argument_eol},
		},
		[OPTION_CRAZY_6] = {
			.description = "A crazy long option #1.",
			.long_option = "option-eq-LEFT_COLUMN_MAX_WIDTH-args",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any}, optargs_argument_eol},
		},
		[OPTION_CRAZY_7] = {
			.description = "A crazy long option #2.",
			.long_option = "option-args-just-less-than-in-above",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any}, optargs_argument_eol},
		},
		[OPTION_CRAZY_8] = {
			.description = "A crazy long option #3.",
			.long_option = "option-args-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any}, optargs_argument_eol},
		},
		[OPTION_DIV_4] = optargs_option_divider,
		[OPTION_CRAZY_9] = {
			.description = "A crazy long option #0.1.",
			.long_option = "opti-lt-LEFT_COLUMN_MAX_WIDTH",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		[OPTION_CRAZY_10] = {
			.description = "A crazy long option #0.2.",
			.long_option = "opti-lt-LEFT_COLUMN_MAX_WIDTHx",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		[OPTION_CRAZY_11] = {
			.description = "A crazy long option #1.",
			.long_option = "option-eq-LEFT_COLUMN_MAX_WIDTH-opti",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		[OPTION_CRAZY_12] = {
			.description = "A crazy long option #2.",
			.long_option = "option-opti-just-less-than-in-above",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
		[OPTION_CRAZY_13] = {
			.description = "A crazy long option #3.",
			.long_option = "option-opti-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.argument = (struct optargs_argument []){ {.name = "arg", .type = optargs_argument_any_opt}, optargs_argument_eol},
		},
#endif
		[_OPTION_EOL] = optargs_option_eol
	};
	struct optargs_argument args[] =
	{
#ifndef NO_argS
		{
			.name = "Name_1",
			.description = "Description 1."
		}, {
			.description = "This is a super long argument description because blaa blaa balabla blaa blaa. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum.",
			.name = "Name_3",
		}, {
			.type = _optargs_argument_divider
		}, {
			.name = "Name_2",
			.description = "Description 2.",
			.type = optargs_argument_any,
		}, {
			.name = "Name_4",
			.description = "fooboo",
		}, {
			.name = "commands",
			.type = optargs_argument_group,
		}, {
			.name = "command_1",
			.description = "Command 1 is the shiiit!",
			.type = optargs_argument_group_member,
		}, {
			.name = "command_2",
			.description = "Command 2 is the booomb!",
			.type = optargs_argument_group_member,
		}, {
			.name = "argument name eq LEFT_COLUMN_MAX_WIDTH",
			.description = "Some non-relevant description #2.",
			.type = optargs_argument_any,
		}, {
			.name = "argument name just less than in above",
			.description = "Some non-relevant description #3."
		}, {
			.name = "argument name greater than LEFT_COLUMN_MAX_WIDTH",
			.description = "Some non-relevant description #4."
		}, {
			.name = "curse_words",
			.type = optargs_argument_group_opt,
		}, {
			.name = "vittu",
			.description = "fuck / vag",
			.type = optargs_argument_group_member
		}, {
			.name = "perkele",
			.description = "god damn it / devil",
			.type = optargs_argument_group_member,
		}, {
			.name = "argument group name with maximum widt",
			.description = "tittidisd fksdlf klsdal kdsalkf sdlk flksdflksadkl fsdlk dslka dsakl dlksfakli",
			.type = optargs_argument_group_opt,
		}, {
			.name = "argument usb-argument",
			.description = "Some non-relevant description.",
			.type = optargs_argument_group_member,
		}, {
			.name = "argument group name with maximum width",
			.description = "tittidisd fksdlf klsdal kdsalkf sdlk flksdflksadkl fsdlk dslka dsakl dlksfakli",
			.type = optargs_argument_group_opt,
		}, {
			.name = "argument usb-argument",
			.description = "Some non-relevant description.",
			.type = optargs_argument_group_member,
		},
#endif
		optargs_argument_eol
	};

	if ((idx = optargs_parse_options(ac, (char const **)av, opts)) < 0)
		return EXIT_FAILURE;

	if (optargs_option_count(&opts[OPTION_HELP]))
	{
		optargs_print_help(av[0], about, opts, args);
		return EXIT_SUCCESS;
	}

	if (optargs_parse_arguments(ac - idx, (const char **)av + idx, args))
		return EXIT_FAILURE;

	printf("Got: ");

	if (optargs_option_count(&opts[OPTION_A]))
		printf("a");

	if (optargs_option_count(&opts[OPTION_B]))
		printf("b");

	if (optargs_option_count(&opts[OPTION_C]))
		printf("c");

	if (optargs_option_count(&opts[OPTION_D]))
		printf("d");

	if (optargs_option_count(&opts[OPTION_E]))
		printf("e");

	if (optargs_option_count(&opts[OPTION_F]))
		printf("f");

	if (optargs_option_count(&opts[OPTION_G]))
		printf("g");

	if (optargs_option_count(&opts[7]))
		printf("h");

	if (optargs_option_count(&opts[OPTION_HIDDEN]))
		printf("hidden!");

	if ((t = optargs_option_string(&opts[OPTION_I1])))
		printf("i{%s}", t);

	if (optargs_option_type(&opts[OPTION_J]) == optargs_flag)
	{
		t = "Oujea!";
		printf("j{%s}", t);
	}
	else if (optargs_option_type(&opts[OPTION_J]) == optargs_argument)
	{
		t = optargs_option_string(&opts[OPTION_J]);
		printf("j{%s}", t);
	}

	if (idx > 0)
		while (idx < ac)
			printf(",%s", av[idx++]);

	printf("\n");

	return EXIT_SUCCESS;
}
