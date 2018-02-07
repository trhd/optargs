enum
{
	SAMPLE_OPT_A, SAMPLE_OPT_B, SAMPLE_OPT_C, SAMPLE_OPT_D, SAMPLE_OPT_E,
	SAMPLE_OPT_F, SAMPLE_OPT_G, SAMPLE_OPT_H, SAMPLE_OPT_I, SAMPLE_OPT_J,
	SAMPLE_OPT_K, SAMPLE_OPT_L, SAMPLE_OPT_M, SAMPLE_OPT_N, _SAMPLE_OPT_NONE
};

struct optargs_option opts_sample[] =
{
	[SAMPLE_OPT_A] = { .short_option = 'a', .description = "x" },
	[SAMPLE_OPT_B] = { .short_option = 'b', .description = "x" },
	[SAMPLE_OPT_C] = { .short_option = 'c', .description = "x" },
	[SAMPLE_OPT_D] = { .long_option = "dddd", .short_option = 'D', .description = "x" },
	[SAMPLE_OPT_E] = { .long_option = "eeee", .short_option = 'E', .description = "x" },
	[SAMPLE_OPT_F] = { .long_option = "ffff", .short_option = 'F', .description = "x" },
	[SAMPLE_OPT_G] = { .long_option = "gggg", .description = "x" },
	[SAMPLE_OPT_H] = { .long_option = "hhhh", .description = "x" },
	[SAMPLE_OPT_I] = { .long_option = "iiii", .description = "x" },
	[SAMPLE_OPT_J] = { .long_option = "jjjj", .short_option = 'j' },
	[SAMPLE_OPT_K] =
	{
		.short_option = 'k',
		.long_option = "kkkk",
		.description = "x",
		.argument = (struct optargs_argument [])
		{
			{
				.name = "ARG",
				.description = "foo",
				.type = optargs_argument_any,
			},
			optargs_argument_eol
		}
	},
	[SAMPLE_OPT_L] =
	{
		.short_option = 'l',
		.long_option = "llll",
		.description = "x",
		.argument = (struct optargs_argument [])
		{
			{
				.name = "ARG",
				.description = "foo",
				.type = optargs_argument_any_opt,
			},
			optargs_argument_eol
		}
	},
	[SAMPLE_OPT_M] =
	{
		.short_option = 'm',
		.long_option = "mmmm",
		.description = "x",
		.argument = (struct optargs_argument [])
		{
			{
				.name = "ARG",
				.description = "foo",
				.type = optargs_argument_any,
			},
			optargs_argument_eol
		}
	},
	[SAMPLE_OPT_N] =
	{
		.short_option = 'n',
		.long_option = "nnnn",
		.description = "x",
		.argument = (struct optargs_argument [])
		{
			{
				.name = "ARG",
				.description = "foo",
				.type = optargs_argument_any_opt,
			},
			optargs_argument_eol
		}
	},
	[_SAMPLE_OPT_NONE] = optargs_option_eol,
};
