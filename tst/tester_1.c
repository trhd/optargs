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
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description B.",
			.long_option = "bbbb",
			.short_option = 'b',
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description C.",
			.long_option = "cccc",
			.short_option = 'c',
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description D.",
			.long_option = "dddd",
			.short_option = 'd',
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description E.",
			.long_option = "eeee",
			.short_option = 'e',
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description F.",
			.long_option = "ffff",
			.short_option = 'f',
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description G.",
			.long_option = "gggg",
			.short_option = 'g',
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description H.",
			.long_option = "hhhh",
			.short_option = 'h',
			.argument = { .mandatory = optargs_no },
		}, {
			.description = NULL,
			.long_option = "hidden-option",
			.short_option = 0,
			.argument = { .mandatory = optargs_no },
		}, {
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description I.",
			.long_option = "iiii",
			.short_option = 'i',
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "Description I2.",
			.long_option = "iiii2",
			.short_option = 0,
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
			.short_option = 0,
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
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "SuperLongWordThatShouldExceedTheLineWidthSpecificationThatWillTestThatTheProgramCanHandleSuperLongWordsProperly.",
			.long_option = "foobar",
			.short_option = 'F',
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "Description O with some extra words to make the description a bit longer. A few more words. Just about to be long enough.",
			.long_option = "oooo",
			.short_option = 'o',
			.argument = { .mandatory = optargs_no },
		}, {
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "A crazy long option #1.",
			.long_option = "option-name-eq-LEFT_COLUMN_MAX_WIDTH",
			.short_option = 0,
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "A crazy long option #2.",
			.long_option = "option-name-just-less-than-in-above",
			.short_option = 0,
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "A crazy long option #3.",
			.long_option = "option-name-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.short_option = 0,
			.argument = { .mandatory = optargs_no },
		}, {
			.argument = { .mandatory = optargs_no },
		}, {
			.description = "A crazy long option #0.1.",
			.long_option = "option-lt-LEFT_COLUMN_MAX_WIDTH",
			.short_option = 0,
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #0.2.",
			.long_option = "option-lt-LEFT_COLUMN_MAX_WIDTHx",
			.short_option = 0,
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #1.",
			.long_option = "option-eq-LEFT_COLUMN_MAX_WIDTH-args",
			.short_option = 0,
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #2.",
			.long_option = "option-args-just-less-than-in-above",
			.short_option = 0,
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #3.",
			.long_option = "option-args-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.short_option = 0,
			.argument = { .mandatory = optargs_yes },
		}, {
			.argument = { .mandatory = optargs_yes },
		}, {
			.description = "A crazy long option #0.1.",
			.long_option = "opti-lt-LEFT_COLUMN_MAX_WIDTH",
			.short_option = 0,
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "A crazy long option #0.2.",
			.long_option = "opti-lt-LEFT_COLUMN_MAX_WIDTHx",
			.short_option = 0,
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "A crazy long option #1.",
			.long_option = "option-eq-LEFT_COLUMN_MAX_WIDTH-opti",
			.short_option = 0,
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "A crazy long option #2.",
			.long_option = "option-opti-just-less-than-in-above",
			.short_option = 0,
			.argument = { .mandatory = optargs_maybe },
		}, {
			.description = "A crazy long option #3.",
			.long_option = "option-opti-greater-than-LEFT_COLUMN_MAX_WIDTH",
			.short_option = 0,
			.argument = { .mandatory = optargs_maybe },
		},
		optargs_opt_eol
	};
	struct optargs_arg args[] =
	{
		{ "ARGUMENTS", NULL, optargs_no },
		{ "Name_1", "Description 1.", optargs_no },
		{ "Name_3", "This is a super long argument description because blaa blaa balabla blaa blaa. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum. Loorem lapsum lipsum lopsum.", optargs_maybe },
		{ .mandatory = optargs_no },
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
		if (t == &optargs_default_result)
			t = "Oujea!";

		printf("j{%s}", t);
	}

	if (idx > 0)
		while (idx < ac)
			printf(",%s", av[idx++]);

	printf("\n");

	return EXIT_SUCCESS;
}
