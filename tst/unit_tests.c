#include <errno.h>
#include <stdio.h>
#include "cmocka-wrapper.h"
#include "optargs.h"

/************************************************************************/

enum
{
	SAMPLE_OPT_A, SAMPLE_OPT_B, SAMPLE_OPT_C, SAMPLE_OPT_D, SAMPLE_OPT_E,
	SAMPLE_OPT_F, SAMPLE_OPT_G, SAMPLE_OPT_H, SAMPLE_OPT_I, SAMPLE_OPT_J,
	SAMPLE_OPT_K, SAMPLE_OPT_L, SAMPLE_OPT_M, SAMPLE_OPT_N, _SAMPLE_OPT_NONE
};

struct optargs_opt opts_sample[] =
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
	[SAMPLE_OPT_J] = { .long_option = "jjjj" },
	[SAMPLE_OPT_K] = { .short_option = 'k', .long_option = "kkkk", .description = "x",
		.argument = { .mandatory = optargs_yes }},
	[SAMPLE_OPT_L] = { .short_option = 'l', .long_option = "llll", .description = "x",
		.argument = { .mandatory = optargs_maybe }},
	[SAMPLE_OPT_M] = { .short_option = 'm', .long_option = "mmmm", .description = "x",
		.argument = { .mandatory = optargs_yes }},
	[SAMPLE_OPT_N] = { .short_option = 'n', .long_option = "nnnn", .description = "x",
		.argument = { .mandatory = optargs_maybe }},
	[_SAMPLE_OPT_NONE] = optargs_opt_eol,
};

struct optargs_arg args_sample[] =
{
	{ .name = "arg1", .description = "Argument #1" },
	{ .name = "arg2", .description = "Argument #2", .mandatory = optargs_yes },
	{ .name = "arg3", .description = "Argument #3", .mandatory = optargs_maybe },
	{ .name = "UUH" },
	{ .name = "arg4", .description = "Argument #4" },
	{ .name = "arg5", .description = "Argument #5", .mandatory = optargs_yes },
	{ .name = "arg6", .description = "Argument #6", .mandatory = optargs_maybe },
	optargs_arg_eol
};

/************************************************************************/

static void
UT_optargs_parse()
{
	struct argv_res_pair
	{
		int argc;
		char const * argv[20];
		int index;
		struct optargs_result result[_SAMPLE_OPT_NONE];
	}
	tests[] =
	{
		[0] = { 1, { "exe" }, 1,
			{ optargs_result_nil }},
		[1] = { 2, { "exe", "foobar" }, 1,
			{ optargs_result_nil }},

		/* Ignore options after argument. */
		[2] = { 3, { "exe", "foobar", "-b" }, 1,
			{ optargs_result_nil }},
		[3] = { 3, { "exe", "-b", "foobar" }, 2,
			{
				[SAMPLE_OPT_B] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Long option. */
		[4] = { 5, { "exe", "--dddd", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_D] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Short option. */
		[5] = { 5, { "exe", "-E", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_E] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Short and long option (same). */
		[6] = { 6, { "exe", "--ffff", "-F", "arg1", "arg2", "arg3" }, 3,
			{
				[SAMPLE_OPT_F] = { .type = optargs_count, .value.count = 2 }
			}
		},

		/* Short and long option (different). */
		[7] = { 6, { "exe", "--ffff", "-a", "arg1", "arg2", "arg3" }, 3,
			{
				[SAMPLE_OPT_A] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_F] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Two different shorts. */
		[8] = { 6, { "exe", "-a", "-c", "arg1", "arg2", "arg3" }, 3,
			{
				[SAMPLE_OPT_A] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_C] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Combine two shorts. */
		[9] = { 5, { "exe", "-ac", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_A] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_C] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Hidden option. */
		[10] = { 5, { "exe", "--jjjj", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_J] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Long option with a mandatory argument (assigned). */
		[11] = { 5, { "exe", "--kkkk=foobar", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_K] = { .type = optargs_string, .value.string = "foobar" }
			}
		},

		/* Long option with a mandatory argument (separated). */
		[12] = { 6, { "exe", "--kkkk", "foobar", "arg1", "arg2", "arg3" }, 3,
			{
				[SAMPLE_OPT_K] = { .type = optargs_string, .value.string = "foobar" }
			}
		},

		/* Short option with a mandatory argument (assigned). */
		[13] = { 5, { "exe", "-k=foobar", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_K] = { .type = optargs_string, .value.string = "foobar" }
			}
		},

		/* Short option with a mandatory argument (appended). */
		[14] = { 5, { "exe", "-kfoobar", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_K] = { .type = optargs_string, .value.string = "foobar" }
			}
		},

		/* Short option with a mandatory argument (separated). */
		[15] = { 6, { "exe", "-k", "foobar", "arg1", "arg2", "arg3" }, 3,
			{
				[SAMPLE_OPT_K] = { .type = optargs_string, .value.string = "foobar" }
			}
		},

		/* Long option with an optional argument (assigned). */
		[16] = { 5, { "exe", "--llll=foobar", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_L] = { .type = optargs_string, .value.string = "foobar" }
			}
		},

		/* Long option with an optional argument (separated). */
		[17] = { 6, { "exe", "--llll", "foobar", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_L] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Short option with an optional argument (assigned). */
		[18] = { 5, { "exe", "-l=foobar", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_L] = { .type = optargs_string, .value.string = "foobar" }
			}
		},

		/* Short option with a mandatory argument (appended). */
		[19] = { 5, { "exe", "-lfoobar", "arg1", "arg2", "arg3" }, -EINVAL,
			{ optargs_result_nil } },

		/* Short option with a mandatory argument (separated). */
		[20] = { 6, { "exe", "-l", "foobar", "arg1", "arg2", "arg3" }, 2,
			{
				[SAMPLE_OPT_L] = { .type = optargs_count, .value.count = 1 }
			}
		},

		/* Megamix */
		[21] = { 18, { "exe", "-a", "-bbb", "-DEF", "-bc", "-EF", "--gggg", "--iiii", "--hhhh", "--jjjj", "-k=foobar", "--llll=bar foo", "--hhhh", "yo", "mama", "-a", "--bbbb", "-DEF" }, 13,
			{
				[SAMPLE_OPT_A] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_B] = { .type = optargs_count, .value.count = 4 },
				[SAMPLE_OPT_C] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_D] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_E] = { .type = optargs_count, .value.count = 2 },
				[SAMPLE_OPT_F] = { .type = optargs_count, .value.count = 2 },
				[SAMPLE_OPT_G] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_H] = { .type = optargs_count, .value.count = 2 },
				[SAMPLE_OPT_I] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_J] = { .type = optargs_count, .value.count = 1 },
				[SAMPLE_OPT_K] = { .type = optargs_string, .value.string = "foobar" },
				[SAMPLE_OPT_L] = { .type = optargs_string, .value.string = "bar foo" }
			}
		},

		/* Missing argument (for short option). */
		[22] = { 2, { "exe", "-k" }, -EINVAL,
			{ optargs_result_nil }
		},

		/* Missing argument (for long option). */
		[23] = { 2, { "exe", "--kkkk" }, -EINVAL,
			{ optargs_result_nil }
		},

		/* Redefine count to string */
		[24] = { 3, { "exe", "-l", "-l=foo" }, -EINVAL,
			{ optargs_result_nil }
		},

		/* Redefine string to count */
		[25] = { 3, { "exe", "-l=foo", "-l" }, -EINVAL,
			{ optargs_result_nil }
		},

		/* Redefine string to string */
		[26] = { 3, { "exe", "-l=bar", "-l=foo" }, -EINVAL,
			{ optargs_result_nil }
		},

	};

	for (size_t i = 0 ; i < sizeof(tests)/sizeof(struct argv_res_pair) ; i++)
	{
		printf("Test #%zd\n", i);

		assert_int_equal(optargs_parse(tests[i].argc, tests[i].argv, opts_sample), tests[i].index);

		if (tests[i].index >= 0)
			for (int j = 0 ; j < _SAMPLE_OPT_NONE ; j++)
			{
				assert_int_equal(tests[i].result[j].type, opts_sample[j].result.type);

				if (!optargs_result_type(&tests[i].result[j]))
					continue;

				if (optargs_result_type(&tests[i].result[j]) == optargs_string)
					assert_string_equal(tests[i].result[j].value.string, opts_sample[j].result.value.string);
				else
					assert_int_equal(tests[i].result[j].value.count, opts_sample[j].result.value.count);
			}
	}
}

/************************************************************************/

static void
UT_optargs_parse__NULL()
{
	char const * argv[] = { "exe", "--help", "arg1", "arg2", "arg3" };

	expect_assert_failure(optargs_parse(0, NULL, NULL));

	expect_assert_failure(optargs_parse(5, argv, NULL));
	expect_assert_failure(optargs_parse(5, NULL, opts_sample));
	expect_assert_failure(optargs_parse(0, argv, opts_sample));

	assert_int_equal(optargs_parse(5, argv, opts_sample), -EINVAL);
}

/************************************************************************/

static void
UT_optargs_print_help()
{
	optargs_print_help("my_cmd", "about my_cmd", opts_sample, args_sample);
	optargs_print_help("my_cmd", "about my_cmd", NULL, args_sample);
	optargs_print_help("my_cmd", "about my_cmd", opts_sample, NULL);
	optargs_print_help("my_cmd", "about my_cmd", NULL, NULL);

	optargs_print_help("my_cmd", NULL, opts_sample, args_sample);
	optargs_print_help("my_cmd", NULL, NULL, args_sample);
	optargs_print_help("my_cmd", NULL, opts_sample, NULL);
	optargs_print_help("my_cmd", NULL, NULL, NULL);

	/* FIXME: Check that the produced help texts make sense. */
}

/************************************************************************/

static void
UT_optargs_print_help__NULL()
{
	expect_assert_failure(optargs_print_help(NULL, "about my_cmd", opts_sample, args_sample));
	expect_assert_failure(optargs_print_help(NULL, "about my_cmd", NULL, args_sample));
	expect_assert_failure(optargs_print_help(NULL, "about my_cmd", opts_sample, NULL));
	expect_assert_failure(optargs_print_help(NULL, "about my_cmd", NULL, NULL));

	expect_assert_failure(optargs_print_help(NULL, NULL, opts_sample, args_sample));
	expect_assert_failure(optargs_print_help(NULL, NULL, NULL, args_sample));
	expect_assert_failure(optargs_print_help(NULL, NULL, opts_sample, NULL));
	expect_assert_failure(optargs_print_help(NULL, NULL, NULL, NULL));
}

/************************************************************************/

static void
UT_optargs_result_by_long()
{
	char const * argv[] = { "foo", "-D", "--eeee", "-F", "bar" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 4);

	assert_ptr_equal(optargs_result_by_long(opts_sample, "dddd"), &opts_sample[SAMPLE_OPT_D].result);
	assert_ptr_equal(optargs_result_by_long(opts_sample, "eeee"), &opts_sample[SAMPLE_OPT_E].result);
	assert_ptr_equal(optargs_result_by_long(opts_sample, "ffff"), &opts_sample[SAMPLE_OPT_F].result);

	assert_null(optargs_result_by_long(opts_sample, "gggg"));
	assert_null(optargs_result_by_long(opts_sample, "hhhh"));
	assert_null(optargs_result_by_long(opts_sample, "iiii"));
	assert_null(optargs_result_by_long(opts_sample, "jjjj"));
	assert_null(optargs_result_by_long(opts_sample, "kkkk"));
	assert_null(optargs_result_by_long(opts_sample, "llll"));
}

/************************************************************************/

static void
UT_optargs_result_by_long__NULL()
{
	expect_assert_failure(optargs_result_by_long(NULL, "--foo"));
	expect_assert_failure(optargs_result_by_long(opts_sample, NULL));
}

/************************************************************************/

static void
UT_optargs_result_by_long__ENOENT()
{
	expect_assert_failure(optargs_result_by_long(opts_sample, "aaaa"));
	expect_assert_failure(optargs_result_by_long(opts_sample, "non-existing-option"));
}

/************************************************************************/

// struct optargs_result const * optargs_result_by_short(struct optargs_opt const * options, char short_option);
static void
UT_optargs_result_by_short()
{
	char const * argv[] = { "foo", "-D", "--eeee", "-F", "bar" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 4);

	assert_ptr_equal(optargs_result_by_short(opts_sample, 'D'), &opts_sample[SAMPLE_OPT_D].result);
	assert_ptr_equal(optargs_result_by_short(opts_sample, 'E'), &opts_sample[SAMPLE_OPT_E].result);
	assert_ptr_equal(optargs_result_by_short(opts_sample, 'F'), &opts_sample[SAMPLE_OPT_F].result);

	assert_null(optargs_result_by_short(opts_sample, 'a'));
	assert_null(optargs_result_by_short(opts_sample, 'b'));
	assert_null(optargs_result_by_short(opts_sample, 'c'));

	assert_null(optargs_result_by_short(opts_sample, 'k'));
	assert_null(optargs_result_by_short(opts_sample, 'l'));
}

/************************************************************************/

static void
UT_optargs_result_by_short__NULL()
{
	expect_assert_failure(optargs_result_by_short(NULL, 'a'));
	expect_assert_failure(optargs_result_by_short(opts_sample, '\0'));
}

/************************************************************************/

static void
UT_optargs_result_by_short__ENOENT()
{
	expect_assert_failure(optargs_result_by_short(opts_sample, 'x'));
	expect_assert_failure(optargs_result_by_short(opts_sample, '1'));
}

/************************************************************************/

static void
UT_optargs_result_by_index()
{
	char const * argv[] = { "foo", "-D", "--eeee", "-F", "bar" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 4);

	assert_ptr_equal(optargs_result_by_index(opts_sample, SAMPLE_OPT_D), &opts_sample[SAMPLE_OPT_D].result);
	assert_ptr_equal(optargs_result_by_index(opts_sample, SAMPLE_OPT_E), &opts_sample[SAMPLE_OPT_E].result);
	assert_ptr_equal(optargs_result_by_index(opts_sample, SAMPLE_OPT_F), &opts_sample[SAMPLE_OPT_F].result);

	assert_null(optargs_result_by_index(opts_sample, SAMPLE_OPT_A));
	assert_null(optargs_result_by_index(opts_sample, SAMPLE_OPT_B));
	assert_null(optargs_result_by_index(opts_sample, SAMPLE_OPT_C));

	assert_null(optargs_result_by_index(opts_sample, SAMPLE_OPT_G));
	assert_null(optargs_result_by_index(opts_sample, SAMPLE_OPT_H));
	assert_null(optargs_result_by_index(opts_sample, SAMPLE_OPT_I));

	assert_null(optargs_result_by_index(opts_sample, SAMPLE_OPT_K));
	assert_null(optargs_result_by_index(opts_sample, SAMPLE_OPT_L));
}

/************************************************************************/

static void
UT_optargs_result_by_index__NULL()
{
	expect_assert_failure(optargs_result_by_index(NULL, 0));
}

/************************************************************************/

#ifndef NDEBUG

static void
UT_optargs_result_by_index__ENOENT()
{
	expect_assert_failure(optargs_result_by_index(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_string_by_long()
{
	char const * argv[] = { "", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 7);

	assert_string_equal(optargs_string_by_long(opts_sample, "llll"), "fez");
	assert_string_equal(optargs_string_by_long(opts_sample, "kkkk"), "bar");
	assert_null(optargs_string_by_long(opts_sample, "mmmm"));
	assert_null(optargs_string_by_long(opts_sample, "nnnn"));

	assert_null(optargs_string_by_long(opts_sample, "gggg"));
	assert_null(optargs_string_by_long(opts_sample, "hhhh"));
	assert_null(optargs_string_by_long(opts_sample, "iiii"));
	assert_null(optargs_string_by_long(opts_sample, "jjjj"));

	expect_assert_failure(optargs_string_by_long(opts_sample, "dddd"));
	expect_assert_failure(optargs_string_by_long(opts_sample, "eeee"));
	expect_assert_failure(optargs_string_by_long(opts_sample, "ffff"));
}

/************************************************************************/

static void
UT_optargs_string_by_long__NULL()
{
	expect_assert_failure(optargs_string_by_long(NULL, "--foo"));
	expect_assert_failure(optargs_string_by_long(opts_sample, NULL));
}

/************************************************************************/

static void
UT_optargs_string_by_short()
{
	char const * argv[] = { "", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 7);

	assert_string_equal(optargs_string_by_short(opts_sample, 'l'), "fez");
	assert_string_equal(optargs_string_by_short(opts_sample, 'k'), "bar");
	assert_null(optargs_string_by_short(opts_sample, 'm'));
	assert_null(optargs_string_by_short(opts_sample, 'n'));

	assert_null(optargs_string_by_short(opts_sample, 'a'));
	assert_null(optargs_string_by_short(opts_sample, 'b'));
	assert_null(optargs_string_by_short(opts_sample, 'c'));

	expect_assert_failure(optargs_string_by_short(opts_sample, 'D'));
	expect_assert_failure(optargs_string_by_short(opts_sample, 'E'));
	expect_assert_failure(optargs_string_by_short(opts_sample, 'F'));
}

/************************************************************************/

static void
UT_optargs_string_by_short__NULL()
{
	expect_assert_failure(optargs_string_by_short(NULL, 'a'));
	expect_assert_failure(optargs_string_by_short(opts_sample, '\0'));
}

/************************************************************************/

static void
UT_optargs_string_by_index()
{
	char const * argv[] = { "", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 7);

	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_A));
	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_B));
	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_C));
	expect_assert_failure(optargs_string_by_index(opts_sample,  SAMPLE_OPT_D));
	expect_assert_failure(optargs_string_by_index(opts_sample,  SAMPLE_OPT_E));
	expect_assert_failure(optargs_string_by_index(opts_sample,  SAMPLE_OPT_F));
	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_G));
	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_H));
	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_I));
	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_J));
	assert_string_equal(optargs_string_by_index(opts_sample,    SAMPLE_OPT_K), "bar");
	assert_string_equal(optargs_string_by_index(opts_sample,    SAMPLE_OPT_L), "fez");
	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_M));
	assert_null(optargs_string_by_index(opts_sample,            SAMPLE_OPT_N));
}

/************************************************************************/

static void
UT_optargs_string_by_index__NULL()
{
	expect_assert_failure(optargs_string_by_index(NULL, 0));
}

/************************************************************************/

#ifndef NDEBUG

static void
UT_optargs_string_by_index__ENOENT()
{
	expect_assert_failure(optargs_string_by_index(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_count_by_long()
{
	char const * argv[] = { "", "-abcaba", "--gggg", "--jjjj", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 10);

	expect_assert_failure(optargs_count_by_long(opts_sample,    "aaaa"));
	expect_assert_failure(optargs_count_by_long(opts_sample,    "bbbb"));
	expect_assert_failure(optargs_count_by_long(opts_sample,    "cccc"));
	assert_int_equal(optargs_count_by_long(opts_sample,         "dddd"), 1);
	assert_int_equal(optargs_count_by_long(opts_sample,         "eeee"), 1);
	assert_int_equal(optargs_count_by_long(opts_sample,         "ffff"), 1);
	assert_int_equal(optargs_count_by_long(opts_sample,         "gggg"), 1);
	assert_int_equal(optargs_count_by_long(opts_sample,         "hhhh"), 0);
	assert_int_equal(optargs_count_by_long(opts_sample,         "iiii"), 0);
	assert_int_equal(optargs_count_by_long(opts_sample,         "jjjj"), 1);
	expect_assert_failure(optargs_count_by_long(opts_sample,    "kkkk"));
	expect_assert_failure(optargs_count_by_long(opts_sample,    "llll"));
	assert_int_equal(optargs_count_by_long(opts_sample,         "mmmm"), 0);
	assert_int_equal(optargs_count_by_long(opts_sample,         "nnnn"), 0);
}

/************************************************************************/

static void
UT_optargs_count_by_long__NULL()
{
	expect_assert_failure(optargs_count_by_long(NULL, "--foo"));
	expect_assert_failure(optargs_count_by_long(opts_sample, NULL));
}

/************************************************************************/

static void
UT_optargs_count_by_short()
{
	char const * argv[] = { "", "-abcaba", "--gggg", "--jjjj", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 10);

	assert_int_equal(optargs_count_by_short(opts_sample,        'a'), 3);
	assert_int_equal(optargs_count_by_short(opts_sample,        'b'), 2);
	assert_int_equal(optargs_count_by_short(opts_sample,        'c'), 1);
	assert_int_equal(optargs_count_by_short(opts_sample,        'D'), 1);
	assert_int_equal(optargs_count_by_short(opts_sample,        'E'), 1);
	assert_int_equal(optargs_count_by_short(opts_sample,        'F'), 1);

	expect_assert_failure(optargs_count_by_short(opts_sample,   'g'));
	expect_assert_failure(optargs_count_by_short(opts_sample,   'h'));
	expect_assert_failure(optargs_count_by_short(opts_sample,   'i'));
	expect_assert_failure(optargs_count_by_short(opts_sample,   'j'));

	expect_assert_failure(optargs_count_by_short(opts_sample,   'k'));
	expect_assert_failure(optargs_count_by_short(opts_sample,   'l'));

	assert_int_equal(optargs_count_by_short(opts_sample,        'm'), 0);
	assert_int_equal(optargs_count_by_short(opts_sample,        'n'), 0);
}

/************************************************************************/

static void
UT_optargs_count_by_short__NULL()
{
	expect_assert_failure(optargs_count_by_short(opts_sample, 0));
	expect_assert_failure(optargs_count_by_short(NULL, 'a'));
}

/************************************************************************/

static void
UT_optargs_count_by_index()
{
	char const * argv[] = { "", "-abcaba", "--gggg", "--jjjj", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse(argc, argv, opts_sample), 10);

	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_A), 3);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_B), 2);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_C), 1);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_D), 1);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_E), 1);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_F), 1);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_G), 1);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_H), 0);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_I), 0);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_J), 1);
	expect_assert_failure(optargs_count_by_index(opts_sample,   SAMPLE_OPT_K));
	expect_assert_failure(optargs_count_by_index(opts_sample,   SAMPLE_OPT_L));
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_M), 0);
	assert_int_equal(optargs_count_by_index(opts_sample,        SAMPLE_OPT_N), 0);
}

/************************************************************************/

static void
UT_optargs_count_by_index__NULL()
{
	expect_assert_failure(optargs_count_by_index(NULL, 0));
}

/************************************************************************/

#ifndef NDEBUG

static void
UT_optargs_count_by_index__ENOENT()
{
	expect_assert_failure(optargs_count_by_index(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_arg_index()
{
	struct optargs_arg args[] =
	{
		{ .name = "zero", .description = "Argument #0" },
		{ .name = "one", .description = "Argument #1" },
		{ .name = "two", .description = "Argument #2", .mandatory = optargs_yes },
		{ .name = "three", .description = "Argument #3", .mandatory = optargs_maybe },
		{ .name = "HEADER" },
		{ .name = "four", .description = "Argument #4" },
		{ .name = "five", .description = "Argument #5", .mandatory = optargs_yes },
		{ .name = "six", .description = "Argument #6", .mandatory = optargs_maybe },
		optargs_arg_eol
	};

	assert_int_equal(optargs_arg_index(args, "zero"), 0);
	assert_int_equal(optargs_arg_index(args, "one"), 1);
	assert_int_equal(optargs_arg_index(args, "two"), 2);
	assert_int_equal(optargs_arg_index(args, "three"), 3);
	assert_int_equal(optargs_arg_index(args, "HEADER"), -EINVAL);
	assert_int_equal(optargs_arg_index(args, "four"), 5);
	assert_int_equal(optargs_arg_index(args, "five"), 6);
	assert_int_equal(optargs_arg_index(args, "six"), 7);

	assert_int_equal(optargs_arg_index(args, "seven"), -EINVAL);
	assert_int_equal(optargs_arg_index(args, "minus-one"), -EINVAL);
	assert_int_equal(optargs_arg_index(args, "xxx"), -EINVAL);
	assert_int_equal(optargs_arg_index(args, "-a"), -EINVAL);
	assert_int_equal(optargs_arg_index(args, "--aaaa"), -EINVAL);
}

/************************************************************************/

static void
UT_optargs_arg_index__NULL()
{
	expect_assert_failure(optargs_arg_index(args_sample, NULL));
	expect_assert_failure(optargs_arg_index(NULL, "foobar"));
}

/************************************************************************/

static void
UT_optargs_result_type()
{
	struct optargs_result *res = test_malloc(sizeof(struct optargs_result));

	assert_int_equal(optargs_result_type(NULL), optargs_undef);

	res->type = optargs_undef;
	assert_int_equal(optargs_result_type(res), optargs_undef);
	res->type = optargs_count;
	assert_int_equal(optargs_result_type(res), optargs_count);
	res->type = optargs_string;
	assert_int_equal(optargs_result_type(res), optargs_string);

	test_free(res);
}

/************************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_optargs_parse),
		cmocka_unit_test(UT_optargs_parse__NULL),

		cmocka_unit_test(UT_optargs_print_help),
		cmocka_unit_test(UT_optargs_print_help__NULL),

		cmocka_unit_test(UT_optargs_result_by_long),
		cmocka_unit_test(UT_optargs_result_by_long__NULL),
		cmocka_unit_test(UT_optargs_result_by_long__ENOENT),

		cmocka_unit_test(UT_optargs_result_by_short),
		cmocka_unit_test(UT_optargs_result_by_short__NULL),
		cmocka_unit_test(UT_optargs_result_by_short__ENOENT),

		cmocka_unit_test(UT_optargs_result_by_index),
		cmocka_unit_test(UT_optargs_result_by_index__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_result_by_index__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_string_by_long),
		cmocka_unit_test(UT_optargs_string_by_long__NULL),

		cmocka_unit_test(UT_optargs_string_by_short),
		cmocka_unit_test(UT_optargs_string_by_short__NULL),

		cmocka_unit_test(UT_optargs_string_by_index),
		cmocka_unit_test(UT_optargs_string_by_index__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_string_by_index__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_count_by_long),
		cmocka_unit_test(UT_optargs_count_by_long__NULL),

		cmocka_unit_test(UT_optargs_count_by_short),
		cmocka_unit_test(UT_optargs_count_by_short__NULL),

		cmocka_unit_test(UT_optargs_count_by_index),
		cmocka_unit_test(UT_optargs_count_by_index__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_count_by_index__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_arg_index),
		cmocka_unit_test(UT_optargs_arg_index__NULL),

		cmocka_unit_test(UT_optargs_result_type)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/************************************************************************/
