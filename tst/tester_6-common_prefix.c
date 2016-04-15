#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

int
main(int ac, char **av)
{
	struct optargs_opt opts[] =
	{
		{
			"Print help.",
			"help",
			'h',
			{ .mandatory = optargs_no },
			NULL,
		}, {
			"Print full help.",
			"help-full",
			'h',
			{ .mandatory = optargs_no },
			NULL,
		},
		optargs_opt_eol
	};

	if (optargs_parse(ac, (const char **)av, opts) < 0)
	{
		fprintf(stderr, "Something went wron while parsing.\n");
		return EXIT_FAILURE;
	}

	if (optargs_opt_by_long(opts, "help"))
		printf("got basic\n");

	if (optargs_opt_by_long(opts, "help-full"))
		printf("got full\n");

	return EXIT_SUCCESS;
}
