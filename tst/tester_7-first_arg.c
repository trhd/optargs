#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

int
main(int ac, char **av)
{
	int i;

	struct optargs_opt opts[] =
	{
		{
			.description = "Description A.",
			.long_option = "aaaa",
			.short_option = 'a',
			.argument = { .mandatory = optargs_no},
		}, {
			.description = "Description B.",
			.long_option = "bbbb",
			.short_option = 'b',
			.argument = { .mandatory = optargs_no},
		}, {
			.description = "Description C.",
			.long_option = "cccc",
			.short_option = 'c',
			.argument = { .mandatory = optargs_no},
		}, {
			.description = "Description D.",
			.long_option = "dddd",
			.short_option = 'd',
			.argument = { .mandatory = optargs_yes},
		}, {
			.description = "Description E.",
			.long_option = "eeee",
			.short_option = 'e',
			.argument = { .mandatory = optargs_maybe},
		},
	};

	i = optargs_parse(ac, (const char **)av, opts);

	if (i < 0)
	{
		printf("ERROR: parsing options failed.\n");
		return EXIT_FAILURE;
	}

	printf("First argument: %s\n", i == 0 ? "--" :  av[i]);

	return EXIT_SUCCESS;
}
