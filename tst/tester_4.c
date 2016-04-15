#include <stdio.h>
#include <stdlib.h>
#include "optargs.h"

static const char about[] = "skibidiwaynow!";

int
main(int ac __attribute__((unused)), char **av)
{
	struct optargs_opt opts[] =
	{
		{ "An option with a named argument.", "option1", 'o', { "FOOBAR :=", "This is the argument.", optargs_yes }, NULL },
		{ "Another option with a named argument.", "option2", 'O', { "BARFOO", "This is another argument.", optargs_maybe }, NULL },
		{ "A third option with a named argument.", "option3", '0', { "KEKEKE =>", "This a third argument.", optargs_maybe }, NULL },
		optargs_opt_eol
	};

	const struct optargs_arg args[] =
	{
		{ "MY_ARGUMENT", "Just some argument.", optargs_yes },
		optargs_arg_eol
	};

	optargs_print_help(av[0], about, opts, args);

	return EXIT_SUCCESS;
}
