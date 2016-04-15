#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

static const char about[] = "foobar";

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_opt opts[] =
	{
		{ .description = "AAA", .short_option = 'a', .long_option = "aaa", .argument = { .mandatory = optargs_no }},
		{ .short_option = 'x', .long_option = "secret", .argument = { .mandatory = optargs_no }},
		{ .description = "BBB", .short_option = 'b', .long_option = "bbb", .argument = { .mandatory = optargs_no }},
		{ .argument = { .mandatory = optargs_no }},
		{ .description = "CCC", .short_option = 'c', .long_option = "ccc", .argument = { .mandatory = optargs_no }},
		optargs_opt_eol
	};
	struct optargs_arg args[] =
	{
		{ "Name_0", NULL, optargs_yes },
		{ "Name_1", NULL, optargs_yes },
		optargs_arg_eol
	};

	optargs_print_help(av[0], about, opts, args);

	return EXIT_SUCCESS;
}
