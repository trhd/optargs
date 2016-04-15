#include <stdlib.h>
#include "optargs.h"

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_opt opts[] =
	{
		{ "Blaablaa.", "blaa", 'b', { .mandatory = optargs_yes }, NULL},
		optargs_opt_eol
	};

	optargs_print_help(av[0], NULL, opts, NULL);

	return EXIT_SUCCESS;
}
