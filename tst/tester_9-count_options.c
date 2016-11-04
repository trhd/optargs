#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

int
main(int ac, char **av)
{
	struct optargs_opt opts[] =
	{
		{ .long_option = "verbose", .short_option = 'v' },
		{ .long_option = "wanha", .short_option = 'w' },
		optargs_opt_eol
	};
	unsigned t;

	if (optargs_parse(ac, (char const **)av, opts) < 0)
	{
		printf("WTF m8!?!?\n");
		return EXIT_FAILURE;
	}

	printf("Got: ");

	t = optargs_count_by_long(opts, "verbose");
	if (t)
		printf("v=%u ", t);

	t = optargs_count_by_long(opts, "wanha");
	if (t)
		printf("w=%u ", t);

	printf("\n");

	return EXIT_SUCCESS;
}
