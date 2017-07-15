/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016-2017 Hemmo Nieminen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
	[SAMPLE_OPT_K] =
	{
		.short_option = 'k',
		.long_option = "kkkk",
		.description = "x",
		.argument = (struct optargs_arg [])
		{
			{
				.name = "ARG",
				.description = "foo",
				.type = optargs_arg_any,
			},
			optargs_arg_eol
		}
	},
	[SAMPLE_OPT_L] =
	{
		.short_option = 'l',
		.long_option = "llll",
		.description = "x",
		.argument = (struct optargs_arg [])
		{
			{
				.name = "ARG",
				.description = "foo",
				.type = optargs_arg_any_opt,
			},
			optargs_arg_eol
		}
	},
	[SAMPLE_OPT_M] =
	{
		.short_option = 'm',
		.long_option = "mmmm",
		.description = "x",
		.argument = (struct optargs_arg [])
		{
			{
				.name = "ARG",
				.description = "foo",
				.type = optargs_arg_any,
			},
			optargs_arg_eol
		}
	},
	[SAMPLE_OPT_N] =
	{
		.short_option = 'n',
		.long_option = "nnnn",
		.description = "x",
		.argument = (struct optargs_arg [])
		{
			{
				.name = "ARG",
				.description = "foo",
				.type = optargs_arg_any_opt,
			},
			optargs_arg_eol
		}
	},
	[_SAMPLE_OPT_NONE] = optargs_opt_eol,
};

struct optargs_arg args_sample[] =
{
	{ .name = "arg1", .description = "Argument #1" },
	{ .name = "arg2", .description = "Argument #2" },
	{ .name = "GROUP", .type = optargs_arg_group },
	{ .name = "g1", .type = optargs_arg_group_member, .description = "G1 I1" },
	{ .name = "g2", .type = optargs_arg_group_member, .description = "G1 I2"  },
	{ .name = "g3", .type = optargs_arg_group_member, .description = "G1 I3"  },
	optargs_arg_div,
	{ .name = "arg3", .description = "Argument #4" },
	{ .name = "arg4", .description = "Argument #5", .type = optargs_arg_any_opt },
	{ .name = "GROUP2", .type = optargs_arg_group_opt },
	{ .name = "g4",.type = optargs_arg_group_member, .description = "G2 I1"  },
	{ .name = "g5",.type = optargs_arg_group_member, .description = "G2 I2"  },
	{ .name = "g6",.type = optargs_arg_group_member, .description = "G2 I3"  },
	optargs_arg_sink
};

struct optargs_arg * huge_subargs_sample = (struct optargs_arg [])
{
	{
		.type = optargs_arg_any,
		.name = "WHAEWA",
		.description = "Frisky first."
	},
	{
		.type = optargs_arg_group,
		.name = "CMD",
		.description = "Da command!"
	},
	{
		.type = optargs_arg_group_member,
		.name = "CMD0",
		.description = "Da command #0!",
	},
	{
		.type = optargs_arg_group_member,
		.name = "CMD1",
		.description = "Da command #1!",
		.subargument = (struct optargs_arg [])
		{
			{
				.type = optargs_arg_any,
				.name = "CMD1.ANY",
				.description = "Anything for CMD1!"
			},
			optargs_arg_eol
		}
	},
	{
		.type = optargs_arg_group_member,
		.name = "CMD2",
		.description = "Da command #2!",
		.subargument = (struct optargs_arg [])
		{
			{
				.type = optargs_arg_group,
				.name = "CMD2.GROUP",
				.description = "Group for CMD2."
			},
			{
				.type = optargs_arg_group_member,
				.name = "CMD2.GROUP.1",
				.description = "Group for CMD2 #1."
			},
			{
				.type = optargs_arg_group_member,
				.name = "CMD2.GROUP.2",
				.description = "Group for CMD2 #2."
			},
			{
				.type = optargs_arg_any,
				.name = "CMD2.ANY",
				.description = "Anything for CMD2 #2!"
			},
			{
				.type = optargs_arg_any,
				.name = "CMD2.ANY",
				.description = "Anything for CMD2 #3!"
			},
			optargs_arg_eol
		}
	},
	{
		.type = optargs_arg_group_member,
		.name = "CMD3",
		.description = "Da command #3!",
		.subargument = (struct optargs_arg [])
		{
			{
				.type = optargs_arg_group,
				.name = "CMD3.GROUP",
				.description = "Group for CMD3."
			},
			{
				.type = optargs_arg_group_member,
				.name = "CMD3.GROUP.1",
				.description = "Group for CMD3 #1.",
				.subargument = (struct optargs_arg [])
				{
					{
						.type = optargs_arg_any,
						.name = "CMD3.GROUP.1.ANY.1",
						.description = "Subargception #1."
					},
					{
						.type = optargs_arg_any_opt,
						.name = "CMD3.GROUP.1.ANY.2",
						.description = "Subargception #2."
					},
					optargs_arg_eol
				}
			},
			{
				.type = optargs_arg_group_member,
				.name = "CMD3.GROUP.2",
				.description = "Group for CMD3 #2.",
				.subargument = (struct optargs_arg [])
				{
					{
						.type = optargs_arg_group,
						.name = "CMD3.GROUP.2.GROUP",
						.description = "Subargception #1."
					},
					{
						.type = optargs_arg_group_member,
						.name = "CMD3.GROUP.2.GROUP.1",
						.description = "Subargception #1!!!",
						.subargument = (struct optargs_arg [])
						{
							{
								.type = optargs_arg_any,
								.name = "WTF",
								.description = "How can this even..."
							},
							optargs_arg_eol
						}
					},
					{
						.type = optargs_arg_group_member,
						.name = "CMD3.GROUP.2.GROUP.2",
						.description = "Subargception #2!!!"
					},
					optargs_arg_eol
				}
			},
			{
				.type = optargs_arg_group_member,
				.name = "CMD3.GROUP.3",
				.description = "Group for CMD3 #3."
			},
			optargs_arg_eol
		}
	},
	optargs_arg_eol
};

/************************************************************************/

static void
UT_optargs_parse_opts()
{
	struct argv_res_pair
	{
		int argc;
		char const * argv[20];
		int index;
		struct optargs_res result[_SAMPLE_OPT_NONE];
	}
	tests[] =
	{
		[0] = { 1, { "exe" }, 1,
			{ optargs_res_null }},
		[1] = { 2, { "exe", "foobar" }, 1,
			{ optargs_res_null }},

		/* Ignore options after argument. */
		[2] = { 3, { "exe", "foobar", "-b" }, 1,
			{ optargs_res_null }},
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
			{ optargs_res_null } },

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
			{ optargs_res_null }
		},

		/* Missing argument (for long option). */
		[23] = { 2, { "exe", "--kkkk" }, -EINVAL,
			{ optargs_res_null }
		},

		/* Redefine count to string */
		[24] = { 3, { "exe", "-l", "-l=foo" }, -EINVAL,
			{ optargs_res_null }
		},

		/* Redefine string to count */
		[25] = { 3, { "exe", "-l=foo", "-l" }, -EINVAL,
			{ optargs_res_null }
		},

		/* Redefine string to string */
		[26] = { 3, { "exe", "-l=bar", "-l=foo" }, -EINVAL,
			{ optargs_res_null }
		},

	};

	for (size_t i = 0 ; i < sizeof(tests)/sizeof(struct argv_res_pair) ; i++)
	{
		printf("Test #%zd\n", i);

		assert_int_equal(optargs_parse_opts(tests[i].argc, tests[i].argv, opts_sample), tests[i].index);

		if (tests[i].index >= 0)
			for (int j = 0 ; j < _SAMPLE_OPT_NONE ; j++)
			{
				assert_int_equal(tests[i].result[j].type, opts_sample[j].result.type);

				if (!optargs_res_type(&tests[i].result[j]))
					continue;

				if (optargs_res_type(&tests[i].result[j]) == optargs_string)
					assert_string_equal(tests[i].result[j].value.string, opts_sample[j].result.value.string);
				else
					assert_int_equal(tests[i].result[j].value.count, opts_sample[j].result.value.count);
			}
	}
}

/************************************************************************/

static void
UT_optargs_parse_opts__sanity_check()
{
	expect_assert_failure(optargs_parse_opts(
				3,
				(char const * []) { "foo", "--test", "bar", "kek" },
				(struct optargs_opt [])
				{
					{
						.long_option = "test",
						.description = "foo",
						.argument = (struct optargs_arg [])
						{
							{
								.name = "eka",
								.type = optargs_arg_group_opt
							}, {
								.name = "eka.1",
								.description = "foo",
								.type = optargs_arg_group_member
							}, {
								.name = "eka.2",
								.description = "foo",
								.type = optargs_arg_group_member
							}, {
								.name = "toka",
								.type = optargs_arg_group
							}, {
								.name = "toka.1",
								.description = "foo",
								.type = optargs_arg_group_member
							}, {
								.name = "toka.2",
								.description = "foo",
								.type = optargs_arg_group_member
							}, {
								.name = "kolmas",
								.type = optargs_arg_group_opt
							}, {
								.name = "kolmas.1",
								.description = "foo",
								.type = optargs_arg_group_member
							}, {
								.name = "kolmas.2",
								.description = "foo",
								.type = optargs_arg_group_member
							},
							optargs_arg_eol
						}
					},
					optargs_opt_eol
				}
			)
		);

	expect_assert_failure(optargs_parse_opts(
				3,
				(char const * []) { "foo", "--test", "bar", "kek" },
				(struct optargs_opt [])
				{
					{
						.long_option = "test",
						.description = "foo",
						.argument = (struct optargs_arg [])
						{
							{
								.name = "eka",
								.type = optargs_arg_any
							}, {
								.name = "eka.1",
								.description = "foo",
								.type = optargs_arg_any_opt
							},
							optargs_arg_sink
						}
					},
					optargs_opt_eol
				}
			)
		);
}

/************************************************************************/

static void
UT_optargs_parse_opts__argument_sink()
{
	struct optargs_opt opts[] =
	{
		{
			.long_option = "test",
			.description = "Test",
			.argument = (struct optargs_arg [])
			{
				{ .name = "GROUP", .type = optargs_arg_group },
				{ .name = "g1", .type = optargs_arg_group_member },
				{ .name = "g2", .type = optargs_arg_group_member },
				{ .name = "g3", .type = optargs_arg_group_member },
				optargs_arg_sink
			},
		},
		optargs_opt_eol
	};

	expect_assert_failure(optargs_parse_opts(1, (char const * []){"exe"}, opts));
	expect_assert_failure(optargs_parse_opts(3, (char const * []){"exe", "--test", "g2"}, opts));
	expect_assert_failure(optargs_parse_opts(4, (char const * []){"exe", "--test", "g2", "foo"}, opts));
}

/************************************************************************/

static void
UT_optargs_parse_opts__NULL()
{
	char const * argv[] = { "exe", "--help", "arg1", "arg2", "arg3" };

	expect_assert_failure(optargs_parse_opts(0, NULL, NULL));

	expect_assert_failure(optargs_parse_opts(5, argv, NULL));
	expect_assert_failure(optargs_parse_opts(5, NULL, opts_sample));
	expect_assert_failure(optargs_parse_opts(0, argv, opts_sample));

	assert_int_equal(optargs_parse_opts(5, argv, opts_sample), -EINVAL);
}

/************************************************************************/

static void
UT_optargs_parse_args()
{
	struct optargs_arg * args = (struct optargs_arg [])
	{
		{.type = optargs_arg_any, .name = "EKA", .description = "Desc EKA."},
		{.type = optargs_arg_group, .name = "TOKA", .description = "Desc TOKA."},
		{.type = optargs_arg_group_member, .name = "TOKA.1", .description = "Desc TOKA.1."},
		{.type = optargs_arg_group_member, .name = "TOKA.2", .description = "Desc TOKA.2."},
		optargs_arg_div,
		{.type = optargs_arg_group_member, .name = "TOKA.3", .description = "Desc TOKA.3."},
		{.type = optargs_arg_any_opt, .name = "KOLMAS", .description = "Desc KOLMAS."},
		{.type = optargs_arg_group_opt, .name = "NELJAS", .description = "Desc NELJAS."},
		{.type = optargs_arg_group_member, .name = "NELJAS.1", .description = "Desc NELJAS.1."},
		{.type = optargs_arg_group_member, .name = "NELJAS.2", .description = "Desc NELJAS.2."},
		{.type = optargs_arg_group_member, .name = "NELJAS.3", .description = "Desc NELJAS.3."},
		optargs_arg_sink
	};

	assert_int_equal(optargs_parse_args(0, NULL, args), -EINVAL);
	assert_int_equal(optargs_parse_args(1, (char const * const []){"a"}, args), -EINVAL);
	assert_int_equal(optargs_parse_args(2, (char const * const []){"a", "X"}, args), -EINVAL);
	assert_int_equal(optargs_parse_args(2, (char const * const []){"a", "TOKA"}, args), -EINVAL);

	assert_int_equal(optargs_parse_args(2, (char const * const []){"a", "TOKA.1"}, args), 2);
	assert_string_equal("a", optargs_arg_value(&args[0]));
	assert_string_equal("TOKA.1", optargs_arg_value(&args[1]));
	assert_string_equal("TOKA.1", optargs_arg_value(&args[2]));
	assert_null(optargs_arg_value(&args[3]));
	assert_null(optargs_arg_value(&args[4]));
	assert_null(optargs_arg_value(&args[5]));
	assert_null(optargs_arg_value(&args[6]));
	assert_null(optargs_arg_value(&args[7]));
	assert_null(optargs_arg_value(&args[8]));
	assert_null(optargs_arg_value(&args[9]));
	assert_null(optargs_arg_value(&args[10]));
	assert_null(optargs_arg_value(&args[11]));

	assert_int_equal(optargs_parse_args(3, (char const * const []){"A", "TOKA.2", "b"}, args), 3);
	assert_string_equal("A", optargs_arg_value(&args[0]));
	assert_string_equal("TOKA.2", optargs_arg_value(&args[1]));
	assert_null(optargs_arg_value(&args[2]));
	assert_string_equal("TOKA.2", optargs_arg_value(&args[3]));
	assert_null(optargs_arg_value(&args[4]));
	assert_null(optargs_arg_value(&args[5]));
	assert_string_equal("b", optargs_arg_value(&args[6]));
	assert_null(optargs_arg_value(&args[7]));
	assert_null(optargs_arg_value(&args[8]));
	assert_null(optargs_arg_value(&args[9]));
	assert_null(optargs_arg_value(&args[10]));
	assert_null(optargs_arg_value(&args[11]));

	assert_int_equal(optargs_parse_args(4, (char const * const []){"a", "TOKA.1", "b", "c"}, args), -EINVAL);

	assert_int_equal(optargs_parse_args(4, (char const * const []){"1", "TOKA.3", "B", "NELJAS.1"}, args), 4);
	assert_string_equal("1", optargs_arg_value(&args[0]));
	assert_string_equal("TOKA.3", optargs_arg_value(&args[1]));
	assert_null(optargs_arg_value(&args[2]));
	assert_null(optargs_arg_value(&args[3]));
	assert_null(optargs_arg_value(&args[4]));
	assert_string_equal("TOKA.3", optargs_arg_value(&args[5]));
	assert_string_equal("B", optargs_arg_value(&args[6]));
	assert_string_equal("NELJAS.1", optargs_arg_value(&args[7]));
	assert_string_equal("NELJAS.1", optargs_arg_value(&args[8]));
	assert_null(optargs_arg_value(&args[9]));
	assert_null(optargs_arg_value(&args[10]));
	assert_null(optargs_arg_value(&args[11]));

	assert_int_equal(optargs_parse_args(7, (char const * const []){"1", "TOKA.3", "NELJAS.1", "NELJAS.1", "1", "2", "3"}, args), 4);
	assert_string_equal("1", optargs_arg_value(&args[0]));
	assert_string_equal("TOKA.3", optargs_arg_value(&args[1]));
	assert_null(optargs_arg_value(&args[2]));
	assert_null(optargs_arg_value(&args[3]));
	assert_null(optargs_arg_value(&args[4]));
	assert_string_equal("TOKA.3", optargs_arg_value(&args[5]));
	assert_string_equal("NELJAS.1", optargs_arg_value(&args[6]));
	assert_string_equal("NELJAS.1", optargs_arg_value(&args[7]));
	assert_string_equal("NELJAS.1", optargs_arg_value(&args[8]));
	assert_null(optargs_arg_value(&args[9]));
	assert_null(optargs_arg_value(&args[10]));
	assert_null(optargs_arg_value(&args[11]));

	assert_int_equal(optargs_parse_args(3, (char const * const []){"--", "a", "TOKA.1"}, args), -EINVAL);
}

/************************************************************************/

static void
UT_optargs_parse_args__subarguments()
{
	assert_int_equal(optargs_parse_args(1, (char const * const []){"whaeva"}, huge_subargs_sample), -EINVAL);
	assert_int_equal(optargs_parse_args(3, (char const * const []){"whaeva", "CMDX"}, huge_subargs_sample), -EINVAL);

	// No subargument.
	assert_int_equal(optargs_parse_args(2, (char const * const []){"whaeva", "CMD0"}, huge_subargs_sample), 2);
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[1]), "CMD0");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[2]), "CMD0");
	assert_null(optargs_arg_value(&huge_subargs_sample[3]));
	assert_null(optargs_arg_value(&huge_subargs_sample[4]));
	assert_null(optargs_arg_value(&huge_subargs_sample[5]));
	assert_null(optargs_arg_value(&huge_subargs_sample[6]));

	// Non-expected subargument.
	assert_int_equal(optargs_parse_args(3, (char const * const []){"whaeva", "CMD0", "foobar"}, huge_subargs_sample), -EINVAL);

	// Mandatory any subargument.
	assert_int_equal(optargs_parse_args(3, (char const * const []){"whaeva", "CMD1", "matafaka"}, huge_subargs_sample), 3);
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[1]), "CMD1");
	assert_null(optargs_arg_value(&huge_subargs_sample[2]));
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[3]), "CMD1");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[3].subargument[0]), "matafaka");
	assert_null(optargs_arg_value(&huge_subargs_sample[4]));
	assert_null(optargs_arg_value(&huge_subargs_sample[5]));
	assert_null(optargs_arg_value(&huge_subargs_sample[6]));

	assert_int_equal(optargs_parse_args(4, (char const * const []){"whaeva", "CMD2", "hei", "moi"}, huge_subargs_sample), -EINVAL);
	assert_int_equal(optargs_parse_args(5, (char const * const []){"whaeva", "CMD2", "hei", "moi", "terve"}, huge_subargs_sample), -EINVAL);

	// Mandatory any subarguments.
	assert_int_equal(optargs_parse_args(5, (char const * const []){"whaeva", "CMD2", "CMD2.GROUP.2", "moi", "terve"}, huge_subargs_sample), 5);
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[1]), "CMD2");
	assert_null(optargs_arg_value(&huge_subargs_sample[2]));
	assert_null(optargs_arg_value(&huge_subargs_sample[3]));
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[4]), "CMD2");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[4].subargument[0]), "CMD2.GROUP.2");
	assert_null(optargs_arg_value(&huge_subargs_sample[4].subargument[1]));
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[4].subargument[2]), "CMD2.GROUP.2");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[4].subargument[3]), "moi");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[4].subargument[4]), "terve");
	assert_null(optargs_arg_value(&huge_subargs_sample[5]));
	assert_null(optargs_arg_value(&huge_subargs_sample[6]));

	// Subargception...
	assert_int_equal(optargs_parse_args(2, (char const * const []){"whaeva", "CMD3"}, huge_subargs_sample), -EINVAL);
	assert_int_equal(optargs_parse_args(3, (char const * const []){"whaeva", "CMD3", "XXX"}, huge_subargs_sample), -EINVAL);
	assert_int_equal(optargs_parse_args(3, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.1"}, huge_subargs_sample), -EINVAL);

	assert_int_equal(optargs_parse_args(4, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.1", "jeejee"}, huge_subargs_sample), 4);
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[1]), "CMD3");
	assert_null(optargs_arg_value(&huge_subargs_sample[2]));
	assert_null(optargs_arg_value(&huge_subargs_sample[3]));
	assert_null(optargs_arg_value(&huge_subargs_sample[4]));
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5]), "CMD3");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5].subargument[0]), "CMD3.GROUP.1");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5].subargument[1]), "CMD3.GROUP.1");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5].subargument[1].subargument[0]), "jeejee");
	assert_null(optargs_arg_value(&huge_subargs_sample[5].subargument[1].subargument[1]));
	assert_null(optargs_arg_value(&huge_subargs_sample[5].subargument[2]));
	assert_null(optargs_arg_value(&huge_subargs_sample[6]));

	assert_int_equal(optargs_parse_args(5, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.1", "jeejee", "oujee"}, huge_subargs_sample), 5);
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[1]), "CMD3");
	assert_null(optargs_arg_value(&huge_subargs_sample[2]));
	assert_null(optargs_arg_value(&huge_subargs_sample[3]));
	assert_null(optargs_arg_value(&huge_subargs_sample[4]));
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5]), "CMD3");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5].subargument[0]), "CMD3.GROUP.1");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5].subargument[1]), "CMD3.GROUP.1");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5].subargument[1].subargument[0]), "jeejee");
	assert_string_equal(optargs_arg_value(&huge_subargs_sample[5].subargument[1].subargument[1]), "oujee");
	assert_null(optargs_arg_value(&huge_subargs_sample[5].subargument[2]));
	assert_null(optargs_arg_value(&huge_subargs_sample[6]));

	assert_int_equal(optargs_parse_args(3, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2"}, huge_subargs_sample), -EINVAL);
	assert_int_equal(optargs_parse_args(4, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "XXX"}, huge_subargs_sample), -EINVAL);

	assert_int_equal(optargs_parse_args(4, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "CMD3.GROUP.2.GROUP.2"}, huge_subargs_sample), 4);

	assert_int_equal(optargs_parse_args(4, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "CMD3.GROUP.2.GROUP.1"}, huge_subargs_sample), -EINVAL);

	assert_int_equal(optargs_parse_args(5, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "CMD3.GROUP.2.GROUP.1", "wee!"}, huge_subargs_sample), 5);
	assert_int_equal(optargs_parse_args(6, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "CMD3.GROUP.2.GROUP.1", "wee!", "too-much"}, huge_subargs_sample), -EINVAL);

	assert_int_equal(optargs_parse_args(3, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.3"}, huge_subargs_sample), 3);
}

/************************************************************************/

static void
UT_optargs_parse_args__mandatory_fixed_argument()
{
	struct optargs_arg * args = (struct optargs_arg [])
	{
		{
			.name = "A",
			.type = optargs_arg_any,
			.description = "foobar"
		},
		optargs_arg_eol
	};
	assert_int_equal(optargs_parse_args(0, NULL, args), -EINVAL);

	assert_int_equal(optargs_parse_args(1, (char const * []){"argh!"}, args), 1);
	assert_string_equal(args[0].result.value, "argh!");


	args = (struct optargs_arg [])
	{
		{.name = "A", .type = optargs_arg_any, .description = "foobar"},
		{.name = "B", .type = optargs_arg_any, .description = "foobar"},
		optargs_arg_eol
	};

	assert_int_equal(optargs_parse_args(1, (char const * []){"argz"}, args), -EINVAL);

	assert_int_equal(optargs_parse_args(2, (char const * []){"argz1", "argz2"}, args), 2);
	assert_string_equal(args[0].result.value, "argz1");
	assert_string_equal(args[1].result.value, "argz2");
}

/************************************************************************/

static void
UT_optargs_parse_args__mandatory_header_argument()
{
	struct optargs_arg * args = (struct optargs_arg [])
	{
		{.name = "A", .type = optargs_arg_group},
		{.name = "a", .type = optargs_arg_group_member, .description = "foobar"},
		{.name = "b", .type = optargs_arg_group_member, .description = "foobar"},
		optargs_arg_eol
	};
	assert_int_equal(optargs_parse_args(0, NULL, args), -EINVAL);

	assert_int_equal(optargs_parse_args(1, (char const * []){"a"}, args), 1);
	assert_ptr_equal(args[0].result.match, &args[1]);
	assert_string_equal(args[1].result.value, "a");
	assert_null(args[2].result.value);

	assert_int_equal(optargs_parse_args(1, (char const * []){"b"}, args), 1);
	assert_string_equal(args[0].result.match, &args[2]);
	assert_null(args[1].result.value);
	assert_string_equal(args[2].result.value, "b");

	assert_int_equal(optargs_parse_args(1, (char const * []){"c"}, args), -EINVAL);


	args = (struct optargs_arg [])
	{
		{.name = "A", .type = optargs_arg_group},
		{.name = "a", .type = optargs_arg_group_member, .description = "foobar"},
		{.name = "b", .type = optargs_arg_group_member, .description = "foobar"},
		{.name = "B", .type = optargs_arg_group},
		{.name = "c", .type = optargs_arg_group_member, .description = "foobar"},
		{.name = "d", .type = optargs_arg_group_member, .description = "foobar"},
		optargs_arg_eol
	};

	assert_int_equal(optargs_parse_args(1, (char const * []){"a"}, args), -EINVAL);

	assert_int_equal(optargs_parse_args(2, (char const * []){"a", "c"}, args), 2);
	assert_ptr_equal(args[0].result.match, &args[1]);
	assert_string_equal(args[1].result.value, "a");
	assert_null(args[2].result.value);
	assert_ptr_equal(args[3].result.match, &args[4]);
	assert_string_equal(args[4].result.value, "c");
	assert_null(args[5].result.value);

	assert_int_equal(optargs_parse_args(2, (char const * []){"a", "d"}, args), 2);
	assert_ptr_equal(args[0].result.match, &args[1]);
	assert_string_equal(args[1].result.value, "a");
	assert_null(args[2].result.value);
	assert_ptr_equal(args[3].result.match, &args[5]);
	assert_null(args[4].result.value);
	assert_string_equal(args[5].result.value, "d");

	assert_int_equal(optargs_parse_args(2, (char const * []){"a", "e"}, args), -EINVAL);
}

/************************************************************************/

static void
UT_optargs_parse_args__NULL()
{
	expect_assert_failure(optargs_parse_args(5,
				NULL, args_sample));

	expect_assert_failure(optargs_parse_args(5,
				(char const * []){"exe", "foo", "bar", "keke", "zou"}, NULL));

	expect_assert_failure(optargs_parse_args(5,
				NULL, NULL));
}

/************************************************************************/

static void
UT_optargs_parse_args__sanity_check()
{
	expect_assert_failure(optargs_parse_args(
			2, (char const * []){"1", "2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_any_opt, .description = "foobar"},
				{.name = "B", .type = optargs_arg_any, .description = "foobar"},
				{.name = "C", .type = optargs_arg_any, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_args(
			2, (char const * []){"1", "2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_any, .description = "foobar"},
				{.name = "B", .type = optargs_arg_any_opt, .description = "foobar"},
				{.name = "C", .type = optargs_arg_any, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_args(
			2, (char const * []){"1", "2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_any, .description = "foobar"},
				{.name = "B", .type = optargs_arg_any, .description = "foobar"},
				{.name = "C", .type = optargs_arg_any_opt, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_args(
			2, (char const * []){"1", "2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_any, .description = "foobar"},
				{.name = "B", .type = optargs_arg_any_opt, .description = "foobar"},
				optargs_arg_div,
				{.name = "C", .type = optargs_arg_any_opt, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_args(
			2, (char const * []){"1", "2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_any_opt, .description = "foobar"},
				{.name = "B", .type = optargs_arg_any_opt, .description = "foobar"},
				{.name = "C", .type = optargs_arg_any, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_args(
			2, (char const * []){"1", "2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_any_opt, .description = "foobar"},
				{.name = "B", .type = optargs_arg_any_opt, .description = "foobar"},
				{.name = "C", .type = optargs_arg_any_opt, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_args(
			2, (char const * []){"1", "2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_any_opt, .description = "foobar"},
				{.name = "B", .type = optargs_arg_any, .description = "foobar"},
				{.name = "C", .type = optargs_arg_any_opt, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_args(
			2, (char const * []){"b.1", "c.2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "a.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_div,
				{.name = "B", .type = optargs_arg_group, .description = "foobar"},
				{.name = "b.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_div,
				{.name = "C", .type = optargs_arg_group, .description = "foobar"},
				{.name = "c.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_args(
			2, (char const * []){"a.1", "c.2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_group, .description = "foobar"},
				{.name = "a.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_div,
				{.name = "B", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "b.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_arg_group, .description = "foobar"},
				{.name = "c.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_args(
			2, (char const * []){"a.1", "b.2"},
			(struct optargs_arg []) {
				optargs_arg_div,
				{.name = "A", .type = optargs_arg_group, .description = "foobar"},
				{.name = "a.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_arg_group, .description = "foobar"},
				{.name = "b.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "c.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_div,
				optargs_arg_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_args(
			2, (char const * []){"a.1", "b.1"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_group, .description = "foobar"},
				{.name = "a.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "b.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_div,
				{.name = "C", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "c.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_args(
			2, (char const * []){"a.1", "c.2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "a.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "b.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_arg_group, .description = "foobar"},
				{.name = "c.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_args(
			2, (char const * []){"a.1", "b.2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "a.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "b.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "c.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_args(
			2, (char const * []){"a.1", "b.2"},
			(struct optargs_arg []) {
				{.name = "A", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "a.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_arg_group, .description = "foobar"},
				{.name = "b.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_arg_group_opt, .description = "foobar"},
				{.name = "c.1", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_arg_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_arg_group_member, .description = "foobar"},
				optargs_arg_eol
			}
		)
	);
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
UT_optargs_opt_res_by_long()
{
	char const * argv[] = { "foo", "-D", "--eeee", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5"};
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 4);
	assert_int_equal(optargs_parse_args(argc - 4, argv + 4, args_sample), 6);

	assert_ptr_equal(optargs_opt_res_by_long(opts_sample, "dddd"), &opts_sample[SAMPLE_OPT_D].result);
	assert_ptr_equal(optargs_opt_res_by_long(opts_sample, "eeee"), &opts_sample[SAMPLE_OPT_E].result);
	assert_ptr_equal(optargs_opt_res_by_long(opts_sample, "ffff"), &opts_sample[SAMPLE_OPT_F].result);

	assert_null(optargs_opt_res_by_long(opts_sample, "gggg"));
	assert_null(optargs_opt_res_by_long(opts_sample, "hhhh"));
	assert_null(optargs_opt_res_by_long(opts_sample, "iiii"));
	assert_null(optargs_opt_res_by_long(opts_sample, "jjjj"));
	assert_null(optargs_opt_res_by_long(opts_sample, "kkkk"));
	assert_null(optargs_opt_res_by_long(opts_sample, "llll"));
}

/************************************************************************/

static void
UT_optargs_opt_res_by_long__NULL()
{
	expect_assert_failure(optargs_opt_res_by_long(NULL, "--foo"));
	expect_assert_failure(optargs_opt_res_by_long(opts_sample, NULL));
}

/************************************************************************/

static void
UT_optargs_opt_res_by_long__ENOENT()
{
	expect_assert_failure(optargs_opt_res_by_long(opts_sample, "aaaa"));
	expect_assert_failure(optargs_opt_res_by_long(opts_sample, "non-existing-option"));
}

/************************************************************************/

// struct optargs_res const * optargs_opt_res_by_short(struct optargs_opt const * options, char short_option);
static void
UT_optargs_opt_res_by_short()
{
	char const * argv[] = { "foo", "-D", "--eeee", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5"};
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 4);
	assert_int_equal(optargs_parse_args(argc - 4, argv + 4, args_sample), 6);

	assert_ptr_equal(optargs_opt_res_by_short(opts_sample, 'D'), &opts_sample[SAMPLE_OPT_D].result);
	assert_ptr_equal(optargs_opt_res_by_short(opts_sample, 'E'), &opts_sample[SAMPLE_OPT_E].result);
	assert_ptr_equal(optargs_opt_res_by_short(opts_sample, 'F'), &opts_sample[SAMPLE_OPT_F].result);

	assert_null(optargs_opt_res_by_short(opts_sample, 'a'));
	assert_null(optargs_opt_res_by_short(opts_sample, 'b'));
	assert_null(optargs_opt_res_by_short(opts_sample, 'c'));

	assert_null(optargs_opt_res_by_short(opts_sample, 'k'));
	assert_null(optargs_opt_res_by_short(opts_sample, 'l'));
}

/************************************************************************/

static void
UT_optargs_opt_res_by_short__NULL()
{
	expect_assert_failure(optargs_opt_res_by_short(NULL, 'a'));
	expect_assert_failure(optargs_opt_res_by_short(opts_sample, '\0'));
}

/************************************************************************/

static void
UT_optargs_opt_res_by_short__ENOENT()
{
	expect_assert_failure(optargs_opt_res_by_short(opts_sample, 'x'));
	expect_assert_failure(optargs_opt_res_by_short(opts_sample, '1'));
}

/************************************************************************/

static void
UT_optargs_opt_res_by_index()
{
	char const * argv[] = { "foo", "-D", "--eeee", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5"};
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 4);
	assert_int_equal(optargs_parse_args(argc - 4, argv + 4, args_sample), 6);

	assert_ptr_equal(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_D), &opts_sample[SAMPLE_OPT_D].result);
	assert_ptr_equal(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_E), &opts_sample[SAMPLE_OPT_E].result);
	assert_ptr_equal(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_F), &opts_sample[SAMPLE_OPT_F].result);

	assert_null(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_A));
	assert_null(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_B));
	assert_null(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_C));

	assert_null(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_G));
	assert_null(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_H));
	assert_null(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_I));

	assert_null(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_K));
	assert_null(optargs_opt_res_by_index(opts_sample, SAMPLE_OPT_L));
}

/************************************************************************/

static void
UT_optargs_opt_res_by_index__NULL()
{
	expect_assert_failure(optargs_opt_res_by_index(NULL, 0));
}

/************************************************************************/

#if !defined(NDEBUG) || defined(UNIT_TESTING)

static void
UT_optargs_opt_res_by_index__ENOENT()
{
	expect_assert_failure(optargs_opt_res_by_index(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_opt_value_by_long()
{
	char const * argv[] = { "", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 7);
	assert_int_equal(optargs_parse_args(argc - 7, argv + 7, args_sample), 6);

	assert_string_equal(optargs_opt_value_by_long(opts_sample, "llll"), "fez");
	assert_string_equal(optargs_opt_value_by_long(opts_sample, "kkkk"), "bar");
	assert_null(optargs_opt_value_by_long(opts_sample, "mmmm"));
	assert_null(optargs_opt_value_by_long(opts_sample, "nnnn"));

	assert_null(optargs_opt_value_by_long(opts_sample, "gggg"));
	assert_null(optargs_opt_value_by_long(opts_sample, "hhhh"));
	assert_null(optargs_opt_value_by_long(opts_sample, "iiii"));
	assert_null(optargs_opt_value_by_long(opts_sample, "jjjj"));

	expect_assert_failure(optargs_opt_value_by_long(opts_sample, "dddd"));
	expect_assert_failure(optargs_opt_value_by_long(opts_sample, "eeee"));
	expect_assert_failure(optargs_opt_value_by_long(opts_sample, "ffff"));
}

/************************************************************************/

static void
UT_optargs_opt_value_by_long__NULL()
{
	expect_assert_failure(optargs_opt_value_by_long(NULL, "--foo"));
	expect_assert_failure(optargs_opt_value_by_long(opts_sample, NULL));
}

/************************************************************************/

static void
UT_optargs_opt_value_by_short()
{
	char const * argv[] = { "", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 7);
	assert_int_equal(optargs_parse_args(argc - 7, argv + 7, args_sample), 6);

	assert_string_equal(optargs_opt_value_by_short(opts_sample, 'l'), "fez");
	assert_string_equal(optargs_opt_value_by_short(opts_sample, 'k'), "bar");
	assert_null(optargs_opt_value_by_short(opts_sample, 'm'));
	assert_null(optargs_opt_value_by_short(opts_sample, 'n'));

	assert_null(optargs_opt_value_by_short(opts_sample, 'a'));
	assert_null(optargs_opt_value_by_short(opts_sample, 'b'));
	assert_null(optargs_opt_value_by_short(opts_sample, 'c'));

	expect_assert_failure(optargs_opt_value_by_short(opts_sample, 'D'));
	expect_assert_failure(optargs_opt_value_by_short(opts_sample, 'E'));
	expect_assert_failure(optargs_opt_value_by_short(opts_sample, 'F'));
}

/************************************************************************/

static void
UT_optargs_opt_value_by_short__NULL()
{
	expect_assert_failure(optargs_opt_value_by_short(NULL, 'a'));
	expect_assert_failure(optargs_opt_value_by_short(opts_sample, '\0'));
}

/************************************************************************/

static void
UT_optargs_opt_value_by_index()
{
	char const * argv[] = { "", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 7);
	assert_int_equal(optargs_parse_args(argc - 7, argv + 7, args_sample), 6);

	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_A));
	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_B));
	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_C));
	expect_assert_failure(optargs_opt_value_by_index(opts_sample,  SAMPLE_OPT_D));
	expect_assert_failure(optargs_opt_value_by_index(opts_sample,  SAMPLE_OPT_E));
	expect_assert_failure(optargs_opt_value_by_index(opts_sample,  SAMPLE_OPT_F));
	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_G));
	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_H));
	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_I));
	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_J));
	assert_string_equal(optargs_opt_value_by_index(opts_sample,    SAMPLE_OPT_K), "bar");
	assert_string_equal(optargs_opt_value_by_index(opts_sample,    SAMPLE_OPT_L), "fez");
	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_M));
	assert_null(optargs_opt_value_by_index(opts_sample,            SAMPLE_OPT_N));
}

/************************************************************************/

static void
UT_optargs_opt_value_by_index__NULL()
{
	expect_assert_failure(optargs_opt_value_by_index(NULL, 0));
}

/************************************************************************/

#if !defined(NDEBUG) || defined(UNIT_TESTING)

static void
UT_optargs_opt_value_by_index__ENOENT()
{
	expect_assert_failure(optargs_opt_value_by_index(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_opt_count_by_long()
{
	char const * argv[] = { "", "-abcaba", "--gggg", "--jjjj", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 10);
	assert_int_equal(optargs_parse_args(argc - 10, argv + 10, args_sample), 6);

	expect_assert_failure(optargs_opt_count_by_long(opts_sample,    "aaaa"));
	expect_assert_failure(optargs_opt_count_by_long(opts_sample,    "bbbb"));
	expect_assert_failure(optargs_opt_count_by_long(opts_sample,    "cccc"));
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "dddd"), 1);
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "eeee"), 1);
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "ffff"), 1);
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "gggg"), 1);
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "hhhh"), 0);
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "iiii"), 0);
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "jjjj"), 1);
	expect_assert_failure(optargs_opt_count_by_long(opts_sample,    "kkkk"));
	expect_assert_failure(optargs_opt_count_by_long(opts_sample,    "llll"));
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "mmmm"), 0);
	assert_int_equal(optargs_opt_count_by_long(opts_sample,         "nnnn"), 0);
}

/************************************************************************/

static void
UT_optargs_opt_count_by_long__NULL()
{
	expect_assert_failure(optargs_opt_count_by_long(NULL, "--foo"));
	expect_assert_failure(optargs_opt_count_by_long(opts_sample, NULL));
}

/************************************************************************/

static void
UT_optargs_opt_count_by_short()
{
	char const * argv[] = { "", "-abcaba", "--gggg", "--jjjj", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 10);
	assert_int_equal(optargs_parse_args(argc - 10, argv + 10, args_sample), 6);

	assert_int_equal(optargs_opt_count_by_short(opts_sample,        'a'), 3);
	assert_int_equal(optargs_opt_count_by_short(opts_sample,        'b'), 2);
	assert_int_equal(optargs_opt_count_by_short(opts_sample,        'c'), 1);
	assert_int_equal(optargs_opt_count_by_short(opts_sample,        'D'), 1);
	assert_int_equal(optargs_opt_count_by_short(opts_sample,        'E'), 1);
	assert_int_equal(optargs_opt_count_by_short(opts_sample,        'F'), 1);

	expect_assert_failure(optargs_opt_count_by_short(opts_sample,   'g'));
	expect_assert_failure(optargs_opt_count_by_short(opts_sample,   'h'));
	expect_assert_failure(optargs_opt_count_by_short(opts_sample,   'i'));
	expect_assert_failure(optargs_opt_count_by_short(opts_sample,   'j'));

	expect_assert_failure(optargs_opt_count_by_short(opts_sample,   'k'));
	expect_assert_failure(optargs_opt_count_by_short(opts_sample,   'l'));

	assert_int_equal(optargs_opt_count_by_short(opts_sample,        'm'), 0);
	assert_int_equal(optargs_opt_count_by_short(opts_sample,        'n'), 0);
}

/************************************************************************/

static void
UT_optargs_opt_count_by_short__NULL()
{
	expect_assert_failure(optargs_opt_count_by_short(opts_sample, 0));
	expect_assert_failure(optargs_opt_count_by_short(NULL, 'a'));
}

/************************************************************************/

static void
UT_optargs_opt_count_by_index()
{
	char const * argv[] = { "", "-abcaba", "--gggg", "--jjjj", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_opts(argc, argv, opts_sample), 10);
	assert_int_equal(optargs_parse_args(argc - 10, argv + 10, args_sample), 6);

	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_A), 3);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_B), 2);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_C), 1);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_D), 1);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_E), 1);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_F), 1);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_G), 1);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_H), 0);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_I), 0);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_J), 1);
	expect_assert_failure(optargs_opt_count_by_index(opts_sample,   SAMPLE_OPT_K));
	expect_assert_failure(optargs_opt_count_by_index(opts_sample,   SAMPLE_OPT_L));
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_M), 0);
	assert_int_equal(optargs_opt_count_by_index(opts_sample,        SAMPLE_OPT_N), 0);
}

/************************************************************************/

static void
UT_optargs_opt_count_by_index__NULL()
{
	expect_assert_failure(optargs_opt_count_by_index(NULL, 0));
}

/************************************************************************/

#if !defined(NDEBUG) || defined(UNIT_TESTING)

static void
UT_optargs_opt_count_by_index__ENOENT()
{
	expect_assert_failure(optargs_opt_count_by_index(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_arg_value()
{
	struct optargs_arg args[] =
	{
		{.name = "a", .type = optargs_arg_any, .description = "aaaa"},
		{.name = "b", .type = optargs_arg_any, .description = "bbbb"},
		{.name = "C", .type = optargs_arg_group},
		{.name = "c1", .type = optargs_arg_group_member, .description = "c1c1" },
		{.name = "c2", .type = optargs_arg_group_member, .description = "c2c2" },
		{.name = "c3", .type = optargs_arg_group_member, .description = "c3c3" },
		{.name = "d", .type = optargs_arg_any, .description = "dddd"},
		{.name = "D", .type = optargs_arg_group},
		{.name = "d1", .type = optargs_arg_group_member, .description = "d1d1" },
		{.name = "d2", .type = optargs_arg_group_member, .description = "d2d2" },
		{.name = "d3", .type = optargs_arg_group_member, .description = "d3d3" },
		{.name = "e", .type = optargs_arg_any_opt, .description = "eeee"},
		{.name = "f", .type = optargs_arg_any_opt, .description = "ffff"},
		optargs_arg_sink
	};

	assert_int_equal(optargs_parse_args(6, (char const * []){"foo", "bar", "c2", "keke", "d3", "har"}, args), 6);
	assert_string_equal(optargs_arg_value(&args[0]), "foo");
	assert_string_equal(optargs_arg_value(&args[1]), "bar");
	assert_string_equal(optargs_arg_value(&args[2]), "c2");
	assert_null(optargs_arg_value(&args[3]));
	assert_string_equal(optargs_arg_value(&args[4]), "c2");
	assert_null(optargs_arg_value(&args[5]));
	assert_string_equal(optargs_arg_value(&args[6]), "keke");
	assert_string_equal(optargs_arg_value(&args[7]), "d3");
	assert_null(optargs_arg_value(&args[8]));
	assert_null(optargs_arg_value(&args[9]));
	assert_string_equal(optargs_arg_value(&args[10]), "d3");
	assert_string_equal(optargs_arg_value(&args[11]), "har");
	assert_null(optargs_arg_value(&args[12]));
	assert_null(optargs_arg_value(&args[13]));
}

/************************************************************************/

static void
UT_optargs_arg_value__NULL()
{
	expect_assert_failure(optargs_arg_value(NULL));
}

/************************************************************************/

static void
UT_optargs_arg_value_offset()
{
	struct optargs_arg args[] =
	{
		{.name = "a", .type = optargs_arg_any, .description = "aaaa"},
		{.name = "b", .type = optargs_arg_any, .description = "bbbb"},
		{.name = "C", .type = optargs_arg_group},
		{.name = "c1", .type = optargs_arg_group_member, .description = "c1c1" },
		{.name = "c2", .type = optargs_arg_group_member, .description = "c2c2" },
		{.name = "c3", .type = optargs_arg_group_member, .description = "c3c3" },
		{.name = "d", .type = optargs_arg_any, .description = "dd"},
		{.name = "D", .type = optargs_arg_group, .description = "dddd"},
		{.name = "d1", .type = optargs_arg_group_member, .description = "d1d1" },
		{.name = "d2", .type = optargs_arg_group_member, .description = "d2d2" },
		{.name = "d3", .type = optargs_arg_group_member, .description = "d3d3" },
		{.name = "e", .type = optargs_arg_any_opt, .description = "eeee"},
		{.name = "F", .type = optargs_arg_group_opt, .description = "ffff"},
		{.name = "f1", .type = optargs_arg_group_member, .description = "f1f1" },
		{.name = "f2", .type = optargs_arg_group_member, .description = "f2f2" },
		{.name = "f3", .type = optargs_arg_group_member, .description = "f3f3" },
		{.name = "G", .type = optargs_arg_group_opt, .description = "gggg"},
		{.name = "g1", .type = optargs_arg_group_member, .description = "g1g1" },
		{.name = "g2", .type = optargs_arg_group_member, .description = "g2g2" },
		{.name = "g3", .type = optargs_arg_group_member, .description = "g3g3" },
		optargs_arg_sink
	};

	assert_int_equal(7,
			optargs_parse_args(7,
				(char const * []){"foo", "bar", "c2", "keke", "d3", "har", "f1"},
				args));

	expect_assert_failure(optargs_arg_value_offset(&args[0]));   // foo
	expect_assert_failure(optargs_arg_value_offset(&args[1]));  // bar
	assert_int_equal(optargs_arg_value_offset(&args[2]),   2);  // C=c2
	expect_assert_failure(optargs_arg_value_offset(&args[3]));  // c1
	expect_assert_failure(optargs_arg_value_offset(&args[4]));  // c2*
	expect_assert_failure(optargs_arg_value_offset(&args[5]));  // c3
	expect_assert_failure(optargs_arg_value_offset(&args[6]));  // d
	assert_int_equal(optargs_arg_value_offset(&args[7]),   3);  // D=d3
	expect_assert_failure(optargs_arg_value_offset(&args[8]));  // d1
	expect_assert_failure(optargs_arg_value_offset(&args[9]));  // d2
	expect_assert_failure(optargs_arg_value_offset(&args[10])); // d3*
	expect_assert_failure(optargs_arg_value_offset(&args[11])); // e=har
	assert_int_equal(optargs_arg_value_offset(&args[12]),  1);  // F=f1
	expect_assert_failure(optargs_arg_value_offset(&args[13])); // f1*
	expect_assert_failure(optargs_arg_value_offset(&args[14])); // f2
	expect_assert_failure(optargs_arg_value_offset(&args[15])); // f3
	assert_int_equal(optargs_arg_value_offset(&args[16]),  -1);  // G=
	expect_assert_failure(optargs_arg_value_offset(&args[17])); // g1
	expect_assert_failure(optargs_arg_value_offset(&args[18])); // g2
	expect_assert_failure(optargs_arg_value_offset(&args[19])); // g3
	expect_assert_failure(optargs_arg_value_offset(&args[20])); // -
}

/************************************************************************/

static void
UT_optargs_arg_value_offset__NULL()
{
	expect_assert_failure(optargs_arg_value_offset(NULL));
}

/************************************************************************/

static void
UT_optargs_arg_value_index()
{
	struct optargs_arg args[] =
	{
		{.name = "a", .type = optargs_arg_any, .description = "aaaa"},
		{.name = "b", .type = optargs_arg_any, .description = "bbbb"},
		{.name = "C", .type = optargs_arg_group},
		{.name = "c1", .type = optargs_arg_group_member, .description = "c1c1" },
		{.name = "c2", .type = optargs_arg_group_member, .description = "c2c2" },
		{.name = "c3", .type = optargs_arg_group_member, .description = "c3c3" },
		{.name = "d", .type = optargs_arg_any, .description = "dd"},
		{.name = "D", .type = optargs_arg_group, .description = "dddd"},
		{.name = "d1", .type = optargs_arg_group_member, .description = "d1d1" },
		{.name = "d2", .type = optargs_arg_group_member, .description = "d2d2" },
		{.name = "d3", .type = optargs_arg_group_member, .description = "d3d3" },
		{.name = "e", .type = optargs_arg_any_opt, .description = "eeee"},
		{.name = "F", .type = optargs_arg_group_opt, .description = "ffff"},
		{.name = "f1", .type = optargs_arg_group_member, .description = "f1f1" },
		{.name = "f2", .type = optargs_arg_group_member, .description = "f2f2" },
		{.name = "f3", .type = optargs_arg_group_member, .description = "f3f3" },
		{.name = "G", .type = optargs_arg_group_opt, .description = "gggg"},
		{.name = "g1", .type = optargs_arg_group_member, .description = "g1g1" },
		{.name = "g2", .type = optargs_arg_group_member, .description = "g2g2" },
		{.name = "g3", .type = optargs_arg_group_member, .description = "g3g3" },
		optargs_arg_sink
	};

	assert_int_equal(7,
			optargs_parse_args(7,
				(char const * []){"foo", "bar", "c2", "keke", "d3", "har", "f1"},
				args));

	expect_assert_failure(optargs_arg_value_index(args, 0));  // a
	expect_assert_failure(optargs_arg_value_index(args, 1));  // b
	assert_int_equal(optargs_arg_value_index(args, 2),   4);  // C
	expect_assert_failure(optargs_arg_value_index(args, 3));  // c1
	expect_assert_failure(optargs_arg_value_index(args, 4));  // c2
	expect_assert_failure(optargs_arg_value_index(args, 5));  // c3
	expect_assert_failure(optargs_arg_value_index(args, 6));  // d
	assert_int_equal(optargs_arg_value_index(args, 7),  10);  // D
	expect_assert_failure(optargs_arg_value_index(args, 8));  // d1
	expect_assert_failure(optargs_arg_value_index(args, 9));  // d2
	expect_assert_failure(optargs_arg_value_index(args, 10)); // d3
	expect_assert_failure(optargs_arg_value_index(args, 11)); // e
	assert_int_equal(optargs_arg_value_index(args, 12), 13);  // F
	expect_assert_failure(optargs_arg_value_index(args, 13)); // f1
	expect_assert_failure(optargs_arg_value_index(args, 14)); // f2
	expect_assert_failure(optargs_arg_value_index(args, 15)); // f3
	assert_int_equal(optargs_arg_value_index(args, 16), -1);  // G
	expect_assert_failure(optargs_arg_value_index(args, 17)); // g1
	expect_assert_failure(optargs_arg_value_index(args, 18)); // g2
	expect_assert_failure(optargs_arg_value_index(args, 19)); // g3
	expect_assert_failure(optargs_arg_value_index(args, 20)); // sink
}

/************************************************************************/

static void
UT_optargs_arg_value_index__NULL()
{
	args_sample[2].result.match = &args_sample[3];
	expect_assert_failure(optargs_arg_value_index(NULL, -1));
	expect_assert_failure(optargs_arg_value_index(NULL, 0));
	expect_assert_failure(optargs_arg_value_index(args_sample, -1));
}

/************************************************************************/

static void
UT_optargs_res_type()
{
	struct optargs_res *res = test_malloc(sizeof(struct optargs_res));

	assert_int_equal(optargs_res_type(NULL), optargs_undef);

	res->type = optargs_undef;
	assert_int_equal(optargs_res_type(res), optargs_undef);
	res->type = optargs_count;
	assert_int_equal(optargs_res_type(res), optargs_count);
	res->type = optargs_string;
	assert_int_equal(optargs_res_type(res), optargs_string);

	test_free(res);
}

/************************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_optargs_parse_opts),
		cmocka_unit_test(UT_optargs_parse_opts__sanity_check),
		cmocka_unit_test(UT_optargs_parse_opts__argument_sink),
		cmocka_unit_test(UT_optargs_parse_opts__NULL),

		cmocka_unit_test(UT_optargs_parse_args),
		cmocka_unit_test(UT_optargs_parse_args__subarguments),
		cmocka_unit_test(UT_optargs_parse_args__mandatory_fixed_argument),
		cmocka_unit_test(UT_optargs_parse_args__mandatory_header_argument),
		cmocka_unit_test(UT_optargs_parse_args__NULL),
		cmocka_unit_test(UT_optargs_parse_args__sanity_check),

		cmocka_unit_test(UT_optargs_print_help),
		cmocka_unit_test(UT_optargs_print_help__NULL),

		cmocka_unit_test(UT_optargs_opt_res_by_long),
		cmocka_unit_test(UT_optargs_opt_res_by_long__NULL),
		cmocka_unit_test(UT_optargs_opt_res_by_long__ENOENT),

		cmocka_unit_test(UT_optargs_opt_res_by_short),
		cmocka_unit_test(UT_optargs_opt_res_by_short__NULL),
		cmocka_unit_test(UT_optargs_opt_res_by_short__ENOENT),

		cmocka_unit_test(UT_optargs_opt_res_by_index),
		cmocka_unit_test(UT_optargs_opt_res_by_index__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_opt_res_by_index__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_opt_value_by_long),
		cmocka_unit_test(UT_optargs_opt_value_by_long__NULL),

		cmocka_unit_test(UT_optargs_opt_value_by_short),
		cmocka_unit_test(UT_optargs_opt_value_by_short__NULL),

		cmocka_unit_test(UT_optargs_opt_value_by_index),
		cmocka_unit_test(UT_optargs_opt_value_by_index__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_opt_value_by_index__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_opt_count_by_long),
		cmocka_unit_test(UT_optargs_opt_count_by_long__NULL),

		cmocka_unit_test(UT_optargs_opt_count_by_short),
		cmocka_unit_test(UT_optargs_opt_count_by_short__NULL),

		cmocka_unit_test(UT_optargs_opt_count_by_index),
		cmocka_unit_test(UT_optargs_opt_count_by_index__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_opt_count_by_index__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_arg_value),
		cmocka_unit_test(UT_optargs_arg_value__NULL),

		cmocka_unit_test(UT_optargs_arg_value_offset),
		cmocka_unit_test(UT_optargs_arg_value_offset__NULL),

		cmocka_unit_test(UT_optargs_arg_value_index),
		cmocka_unit_test(UT_optargs_arg_value_index__NULL),

		cmocka_unit_test(UT_optargs_res_type)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/************************************************************************/
