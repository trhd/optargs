#include "optargs.h"

int
main(int ac, char ** av)
{
	struct optargs_opt opts[] =
	{
		{
			.long_option = "help",
			.description = "Print help."
		},
		optargs_opt_eol
	};

	if (optargs_parse_opts(ac, (char const **)av, opts) < 0)
		return -1;

	if (optargs_res_by_long(opts, "help"))
		optargs_print_help(av[0], 0, opts, 0);

	return 0;
}
