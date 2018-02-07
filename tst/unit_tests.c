/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016-2018 Hemmo Nieminen
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
#include <string.h>
#include "cmocka-wrapper.h"
#include "optargs.h"

/************************************************************************/

static void
_UT_optargs_parse_options__helper(struct optargs_option const * parsed,
		struct optargs_option const * reference)
{
	assert_non_null(parsed);
	assert_non_null(reference);

	for ( ; !reference->argument || reference->value_type == optargs_flag || reference->argument->type != _optargs_argument_eol; reference++, parsed++)
	{
		if (!reference->argument)
			assert_true(!parsed->argument || parsed->value_type == optargs_undef);
		else
		{
			assert_true(parsed->argument);
			assert_true(reference->value_type == parsed->value_type);
			if (reference->value_type == optargs_flag)
				assert_int_equal(reference->count, parsed->count);
			else
				assert_string_equal(reference->argument->result.value, parsed->argument->result.value);
		}
	}
}

static void
UT_optargs_parse_options__no_options()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = { [_SAMPLE_OPT_NONE] = optargs_option_eol };
	assert_int_equal(optargs_parse_options(1, (char const * []){"exe"}, opts_sample), 1);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__argument_but_no_options()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = { [_SAMPLE_OPT_NONE] = optargs_option_eol };
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "foobar"}, opts_sample), 1);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__option_after_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_A] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "-a", "foobar", "-b"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__option_after_double_hyphen()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_A] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "-a", "--", "-b"}, opts_sample), 3);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__double_hyphen_before_arguments()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_A] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "-a", "--", "foobar"}, opts_sample), 3);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__option_middle_of_arguments()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = { [_SAMPLE_OPT_NONE] = optargs_option_eol };
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "barfoo", "-b", "foobar"}, opts_sample), 1);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__option_and_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_B] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-b", "foobar"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__just_option()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_B] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "-b"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__just_long_option()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_D] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "--dddd"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__long_option_and_arguments()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_D] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(5, (char const * []){"exe", "--dddd", "one", "two", "three"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__short_option_and_arguments()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_E] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(5, (char const * []){"exe", "-E", "one", "two", "three"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__same_long_and_short_option()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_F] = { .value_type = optargs_flag, .count = 2 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(6, (char const * []){"exe", "-F", "--ffff", "one", "two", "three"}, opts_sample), 3);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__different_long_and_short_option()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_E] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_F] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(6, (char const * []){"exe", "-E", "--ffff", "one", "two", "three"}, opts_sample), 3);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__two_different_shorts()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_E] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_C] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "-E", "-c", "one"}, opts_sample), 3);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__two_different_shorts_together()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_E] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_C] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-Ec", "one"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__hidden_long_option()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_J] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "--jjjj"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__hidden_short_option()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_J] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "-j"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__long_option_with_assigned_mandatory_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_K] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "foobar"}, optargs_argument_eol} },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "--kkkk=foobar", "some_arg"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__long_option_with_separate_mandatory_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_K] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "foobar"}, optargs_argument_eol} },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "--kkkk", "foobar", "some_arg"}, opts_sample), 3);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__short_option_with_assigned_mandatory_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_K] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "barf"}, optargs_argument_eol} },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-k=barf", "some_arg"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__short_option_with_appended_mandatory_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_K] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "barf"}, optargs_argument_eol} },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-kbarf", "some_arg"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__short_option_with_separated_mandatory_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_K] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "barf"}, optargs_argument_eol} },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "-k", "barf", "some_arg"}, opts_sample), 3);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__long_option_with_assigned_optional_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_L] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "lisa"}, optargs_argument_eol} },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "--llll=lisa", "some_arg"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__long_option_with_separated_optional_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_L] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "--llll", "lisa", "some_arg"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__short_option_with_assigned_optional_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_L] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "lisa"}, optargs_argument_eol} },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-l=lisa", "some_arg"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__short_option_with_appended_optional_argument()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-lfoobar", "some_arg"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__short_option_with_separated_optional_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_L] = { .value_type = optargs_flag, .count = 1 },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(4, (char const * []){"exe", "-l", "foobar", "some_arg"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__megamix()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_A] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_B] = { .value_type = optargs_flag, .count = 4 },
		[SAMPLE_OPT_C] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_D] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_E] = { .value_type = optargs_flag, .count = 2 },
		[SAMPLE_OPT_F] = { .value_type = optargs_flag, .count = 2 },
		[SAMPLE_OPT_G] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_H] = { .value_type = optargs_flag, .count = 2 },
		[SAMPLE_OPT_I] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_J] = { .value_type = optargs_flag, .count = 1 },
		[SAMPLE_OPT_K] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "foobar"}, optargs_argument_eol } },
		[SAMPLE_OPT_L] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = "bar foo"}, optargs_argument_eol } },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(18, (char const * []){ "exe", "-a", "-bbb", "-DEF", "-bc", "-EF", "--gggg", "--iiii", "--hhhh", "--jjjj", "-k=foobar", "--llll=bar foo", "--hhhh", "yo", "mama", "-a", "--bbbb", "-DEF" }, opts_sample), 13);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__short_option_missing_argument()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "-k"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__long_option_missing_argument()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "--kkkk"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__short_option_with_empty_mandatory_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_K] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = ""}, optargs_argument_eol } },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "-k="}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__short_option_with_empty_optional_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_L] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = ""}, optargs_argument_eol } },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "-l="}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__long_option_with_empty_mandatory_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_K] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = ""}, optargs_argument_eol } },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "--kkkk=", "ou"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__long_option_with_empty_optional_argument()
{
	#include "opts_sample.inc"
	struct optargs_option results[] = {
		[SAMPLE_OPT_L] = { .value_type = optargs_argument, .argument = (struct optargs_argument []){ {.result.value = ""}, optargs_argument_eol } },
		[_SAMPLE_OPT_NONE] = optargs_option_eol
	};
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "--llll=", "beibi"}, opts_sample), 2);
	_UT_optargs_parse_options__helper(opts_sample, results);
}

static void
UT_optargs_parse_options__redefine_flag_to_argument()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-l", "-l=raptori"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__redefine_argument_to_flag()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-l=raptori", "-l"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__redefine_argument_to_argument()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(3, (char const * []){"exe", "-l=raptori", "-l=on-kovis"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__invalid_short_option()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "-Y"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__invalid_short_option_with_other_options()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(2, (char const * []){"exe", "-abYa"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__invalid_long_option()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(5, (char const * []){"exe", "--YYYY"}, opts_sample), -EINVAL);
}

static void
UT_optargs_parse_options__invalid_long_option_with_other_options()
{
	#include "opts_sample.inc"
	assert_int_equal(optargs_parse_options(5, (char const * []){"exe", "-ab", "--llll", "--YYYY", "-E"}, opts_sample), -EINVAL);
}

/************************************************************************/

static void
UT_optargs_parse_options__sanity_check()
{
	expect_assert_failure(optargs_parse_options(
				3,
				(char const * []) { "foo", "--test", "bar", "kek" },
				(struct optargs_option [])
				{
					{
						.long_option = "test",
						.description = "foo",
						.argument = (struct optargs_argument [])
						{
							{
								.name = "eka",
								.type = optargs_argument_group_opt
							}, {
								.name = "eka.1",
								.description = "foo",
								.type = optargs_argument_group_member
							}, {
								.name = "eka.2",
								.description = "foo",
								.type = optargs_argument_group_member
							}, {
								.name = "toka",
								.type = optargs_argument_group
							}, {
								.name = "toka.1",
								.description = "foo",
								.type = optargs_argument_group_member
							}, {
								.name = "toka.2",
								.description = "foo",
								.type = optargs_argument_group_member
							}, {
								.name = "kolmas",
								.type = optargs_argument_group_opt
							}, {
								.name = "kolmas.1",
								.description = "foo",
								.type = optargs_argument_group_member
							}, {
								.name = "kolmas.2",
								.description = "foo",
								.type = optargs_argument_group_member
							},
							optargs_argument_eol
						}
					},
					optargs_option_eol
				}
			)
		);

	expect_assert_failure(optargs_parse_options(
				3,
				(char const * []) { "foo", "--test", "bar", "kek" },
				(struct optargs_option [])
				{
					{
						.long_option = "test",
						.description = "foo",
						.argument = (struct optargs_argument [])
						{
							{
								.name = "eka",
								.type = optargs_argument_any
							}, {
								.name = "eka.1",
								.description = "foo",
								.type = optargs_argument_any_opt
							},
							optargs_argument_sink
						}
					},
					optargs_option_eol
				}
			)
		);
}

/************************************************************************/

static void
UT_optargs_parse_options__argument_sink()
{
	struct optargs_option opts[] =
	{
		{
			.long_option = "test",
			.description = "Test",
			.argument = (struct optargs_argument [])
			{
				{ .name = "GROUP", .type = optargs_argument_group },
				{ .name = "g1", .type = optargs_argument_group_member },
				{ .name = "g2", .type = optargs_argument_group_member },
				{ .name = "g3", .type = optargs_argument_group_member },
				optargs_argument_sink
			},
		},
		optargs_option_eol
	};

	expect_assert_failure(optargs_parse_options(1, (char const * []){"exe"}, opts));
	expect_assert_failure(optargs_parse_options(3, (char const * []){"exe", "--test", "g2"}, opts));
	expect_assert_failure(optargs_parse_options(4, (char const * []){"exe", "--test", "g2", "foo"}, opts));
}

/************************************************************************/

static void
UT_optargs_parse_options__NULL()
{
	#include "opts_sample.inc"
	char const * argv[] = { "exe", "--help", "arg1", "arg2", "arg3" };

	expect_assert_failure(optargs_parse_options(0, NULL, NULL));

	expect_assert_failure(optargs_parse_options(5, argv, NULL));
	expect_assert_failure(optargs_parse_options(5, NULL, opts_sample));
	expect_assert_failure(optargs_parse_options(0, argv, opts_sample));

	assert_int_equal(optargs_parse_options(5, argv, opts_sample), -EINVAL);
}

/************************************************************************/

#define four_arguments \
struct optargs_argument * args = (struct optargs_argument []) \
{ \
	{.type = optargs_argument_any, .name = "EKA", .description = "Desc EKA."}, \
	{.type = optargs_argument_group, .name = "TOKA", .description = "Desc TOKA."}, \
	{.type = optargs_argument_group_member, .name = "TOKA.1", .description = "Desc TOKA.1."}, \
	{.type = optargs_argument_group_member, .name = "TOKA.2", .description = "Desc TOKA.2."}, \
	optargs_argument_divider, \
	{.type = optargs_argument_group_member, .name = "TOKA.3", .description = "Desc TOKA.3."}, \
	{.type = optargs_argument_any_opt, .name = "KOLMAS", .description = "Desc KOLMAS."}, \
	{.type = optargs_argument_group_opt, .name = "NELJAS", .description = "Desc NELJAS."}, \
	{.type = optargs_argument_group_member, .name = "NELJAS.1", .description = "Desc NELJAS.1."}, \
	{.type = optargs_argument_group_member, .name = "NELJAS.2", .description = "Desc NELJAS.2."}, \
	{.type = optargs_argument_group_member, .name = "NELJAS.3", .description = "Desc NELJAS.3."}, \
	optargs_argument_sink \
};

static void
UT_optargs_parse_arguments__four_arguments__no_arguments()
{
	four_arguments
	assert_int_equal(optargs_parse_arguments(0, NULL, args), -EINVAL);
}

static void
UT_optargs_parse_arguments__four_arguments__missing_argument()
{
	four_arguments
	assert_int_equal(optargs_parse_arguments(1, (char const * const []){"a"}, args), -EINVAL);
}

static void
UT_optargs_parse_arguments__four_arguments__invalid_group_member_argument()
{
	four_arguments
	assert_int_equal(optargs_parse_arguments(2, (char const * const []){"a", "X"}, args), -EINVAL);
}

static void
UT_optargs_parse_arguments__four_arguments__invalid_argument__group_title_as_member()
{
	four_arguments
	assert_int_equal(optargs_parse_arguments(2, (char const * const []){"a", "TOKA"}, args), -EINVAL);
}


static void
UT_optargs_parse_arguments__four_arguments__required_arguments()
{
	four_arguments

	assert_int_equal(optargs_parse_arguments(2, (char const * const []){"a", "TOKA.1"}, args), 2);

	assert_string_equal("a", optargs_argument_value(&args[0]));
	assert_string_equal("TOKA.1", optargs_argument_value(&args[1]));
	assert_string_equal("TOKA.1", optargs_argument_value(&args[2]));
	assert_null(optargs_argument_value(&args[3]));
	assert_null(optargs_argument_value(&args[4]));
	assert_null(optargs_argument_value(&args[5]));
	assert_null(optargs_argument_value(&args[6]));
	assert_null(optargs_argument_value(&args[7]));
	assert_null(optargs_argument_value(&args[8]));
	assert_null(optargs_argument_value(&args[9]));
	assert_null(optargs_argument_value(&args[10]));
	assert_null(optargs_argument_value(&args[11]));
}

static void
UT_optargs_parse_arguments__four_arguments__required_arguments_and_optional()
{
	four_arguments

	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"A", "TOKA.2", "b"}, args), 3);

	assert_string_equal("A", optargs_argument_value(&args[0]));
	assert_string_equal("TOKA.2", optargs_argument_value(&args[1]));
	assert_null(optargs_argument_value(&args[2]));
	assert_string_equal("TOKA.2", optargs_argument_value(&args[3]));
	assert_null(optargs_argument_value(&args[4]));
	assert_null(optargs_argument_value(&args[5]));
	assert_string_equal("b", optargs_argument_value(&args[6]));
	assert_null(optargs_argument_value(&args[7]));
	assert_null(optargs_argument_value(&args[8]));
	assert_null(optargs_argument_value(&args[9]));
	assert_null(optargs_argument_value(&args[10]));
	assert_null(optargs_argument_value(&args[11]));
}

static void
UT_optargs_parse_arguments__four_arguments__invalid_fourth_argument()
{
	four_arguments
	assert_int_equal(optargs_parse_arguments(4, (char const * const []){"a", "TOKA.1", "b", "c"}, args), -EINVAL);
}

static void
UT_optargs_parse_arguments__four_arguments__all_four()
{
	four_arguments

	assert_int_equal(optargs_parse_arguments(4, (char const * const []){"1", "TOKA.3", "B", "NELJAS.1"}, args), 4);

	assert_string_equal("1", optargs_argument_value(&args[0]));
	assert_string_equal("TOKA.3", optargs_argument_value(&args[1]));
	assert_null(optargs_argument_value(&args[2]));
	assert_null(optargs_argument_value(&args[3]));
	assert_null(optargs_argument_value(&args[4]));
	assert_string_equal("TOKA.3", optargs_argument_value(&args[5]));
	assert_string_equal("B", optargs_argument_value(&args[6]));
	assert_string_equal("NELJAS.1", optargs_argument_value(&args[7]));
	assert_string_equal("NELJAS.1", optargs_argument_value(&args[8]));
	assert_null(optargs_argument_value(&args[9]));
	assert_null(optargs_argument_value(&args[10]));
	assert_null(optargs_argument_value(&args[11]));
	assert_int_equal(optargs_parse_arguments(7, (char const * const []){"1", "TOKA.3", "NELJAS.1", "NELJAS.1", "1", "2", "3"}, args), 4);
	assert_string_equal("1", optargs_argument_value(&args[0]));
	assert_string_equal("TOKA.3", optargs_argument_value(&args[1]));
	assert_null(optargs_argument_value(&args[2]));
	assert_null(optargs_argument_value(&args[3]));
	assert_null(optargs_argument_value(&args[4]));
	assert_string_equal("TOKA.3", optargs_argument_value(&args[5]));
	assert_string_equal("NELJAS.1", optargs_argument_value(&args[6]));
	assert_string_equal("NELJAS.1", optargs_argument_value(&args[7]));
	assert_string_equal("NELJAS.1", optargs_argument_value(&args[8]));
	assert_null(optargs_argument_value(&args[9]));
	assert_null(optargs_argument_value(&args[10]));
	assert_null(optargs_argument_value(&args[11]));
}

static void
UT_optargs_parse_arguments__four_arguments__double_hyphen_not_special()
{
	four_arguments
	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"--", "a", "TOKA.1"}, args), -EINVAL);
}

#undef four_arguments

/************************************************************************/

static void
UT_optargs_parse_arguments__subarguments__no_arguments()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(1, (char const * const []){"whaeva"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__invalid_argument()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"whaeva", "CMDX"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__no_subarguments()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(2, (char const * const []){"whaeva", "CMD0"}, huge_subargs_sample), 2);

	assert_string_equal(optargs_argument_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[1]), "CMD0");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[2]), "CMD0");
	assert_null(optargs_argument_value(&huge_subargs_sample[3]));
	assert_null(optargs_argument_value(&huge_subargs_sample[4]));
	assert_null(optargs_argument_value(&huge_subargs_sample[5]));
	assert_null(optargs_argument_value(&huge_subargs_sample[6]));
}

static void
UT_optargs_parse_arguments__subarguments__unwanted_subargument()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"whaeva", "CMD0", "foobar"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__mandatory_subargument()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"whaeva", "CMD1", "matafaka"}, huge_subargs_sample), 3);
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[1]), "CMD1");
	assert_null(optargs_argument_value(&huge_subargs_sample[2]));
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[3]), "CMD1");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[3].subargument[0]), "matafaka");
	assert_null(optargs_argument_value(&huge_subargs_sample[4]));
	assert_null(optargs_argument_value(&huge_subargs_sample[5]));
	assert_null(optargs_argument_value(&huge_subargs_sample[6]));
}

static void
UT_optargs_parse_arguments__subarguments__missing_mandatory_subargument_1()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(4, (char const * const []){"whaeva", "CMD2", "hei", "moi"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__missing_mandatory_subargument_2()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(5, (char const * const []){"whaeva", "CMD2", "hei", "moi", "terve"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__mandatory_any_subargument()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(5, (char const * const []){"whaeva", "CMD2", "CMD2.GROUP.2", "moi", "terve"}, huge_subargs_sample), 5);

	assert_string_equal(optargs_argument_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[1]), "CMD2");
	assert_null(optargs_argument_value(&huge_subargs_sample[2]));
	assert_null(optargs_argument_value(&huge_subargs_sample[3]));
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[4]), "CMD2");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[4].subargument[0]), "CMD2.GROUP.2");
	assert_null(optargs_argument_value(&huge_subargs_sample[4].subargument[1]));
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[4].subargument[2]), "CMD2.GROUP.2");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[4].subargument[3]), "moi");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[4].subargument[4]), "terve");
	assert_null(optargs_argument_value(&huge_subargs_sample[5]));
	assert_null(optargs_argument_value(&huge_subargs_sample[6]));
}

static void
UT_optargs_parse_arguments__subarguments__subargception_1()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(2, (char const * const []){"whaeva", "CMD3"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__subargception_2()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"whaeva", "CMD3", "XXX"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__subargception_3()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.1"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__subargception_4()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(4, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.1", "jeejee"}, huge_subargs_sample), 4);

	assert_string_equal(optargs_argument_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[1]), "CMD3");
	assert_null(optargs_argument_value(&huge_subargs_sample[2]));
	assert_null(optargs_argument_value(&huge_subargs_sample[3]));
	assert_null(optargs_argument_value(&huge_subargs_sample[4]));
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5]), "CMD3");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5].subargument[0]), "CMD3.GROUP.1");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5].subargument[1]), "CMD3.GROUP.1");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5].subargument[1].subargument[0]), "jeejee");
	assert_null(optargs_argument_value(&huge_subargs_sample[5].subargument[1].subargument[1]));
	assert_null(optargs_argument_value(&huge_subargs_sample[5].subargument[2]));
	assert_null(optargs_argument_value(&huge_subargs_sample[6]));
}

static void
UT_optargs_parse_arguments__subarguments__subargception_5()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(5, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.1", "jeejee", "oujee"}, huge_subargs_sample), 5);
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[0]), "whaeva");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[1]), "CMD3");
	assert_null(optargs_argument_value(&huge_subargs_sample[2]));
	assert_null(optargs_argument_value(&huge_subargs_sample[3]));
	assert_null(optargs_argument_value(&huge_subargs_sample[4]));
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5]), "CMD3");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5].subargument[0]), "CMD3.GROUP.1");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5].subargument[1]), "CMD3.GROUP.1");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5].subargument[1].subargument[0]), "jeejee");
	assert_string_equal(optargs_argument_value(&huge_subargs_sample[5].subargument[1].subargument[1]), "oujee");
	assert_null(optargs_argument_value(&huge_subargs_sample[5].subargument[2]));
	assert_null(optargs_argument_value(&huge_subargs_sample[6]));
}

static void
UT_optargs_parse_arguments__subarguments__subargception_6()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__subargception_7()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(4, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "XXX"}, huge_subargs_sample), -EINVAL);
}

static void
UT_optargs_parse_arguments__subarguments__subargception_8()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(4, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "CMD3.GROUP.2.GROUP.2"}, huge_subargs_sample), 4);
}

static void
UT_optargs_parse_arguments__subarguments__subargception_9()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(4, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "CMD3.GROUP.2.GROUP.1"}, huge_subargs_sample), -EINVAL);
}


static void
UT_optargs_parse_arguments__subarguments__subargception_10()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(5, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "CMD3.GROUP.2.GROUP.1", "wee!"}, huge_subargs_sample), 5);
}

static void
UT_optargs_parse_arguments__subarguments__subargception_11()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(6, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.2", "CMD3.GROUP.2.GROUP.1", "wee!", "too-much"}, huge_subargs_sample), -EINVAL);
}


static void
UT_optargs_parse_arguments__subarguments__subargception_12()
{
	#include "huge_subargs_sample.inc"
	assert_int_equal(optargs_parse_arguments(3, (char const * const []){"whaeva", "CMD3", "CMD3.GROUP.3"}, huge_subargs_sample), 3);
}

/************************************************************************/

static void
UT_optargs_parse_arguments__mandatory_fixed_argument()
{
	struct optargs_argument * args = (struct optargs_argument [])
	{
		{
			.name = "A",
			.type = optargs_argument_any,
			.description = "foobar"
		},
		optargs_argument_eol
	};
	assert_int_equal(optargs_parse_arguments(0, NULL, args), -EINVAL);

	assert_int_equal(optargs_parse_arguments(1, (char const * []){"argh!"}, args), 1);
	assert_string_equal(args[0].result.value, "argh!");


	args = (struct optargs_argument [])
	{
		{.name = "A", .type = optargs_argument_any, .description = "foobar"},
		{.name = "B", .type = optargs_argument_any, .description = "foobar"},
		optargs_argument_eol
	};

	assert_int_equal(optargs_parse_arguments(1, (char const * []){"argz"}, args), -EINVAL);

	assert_int_equal(optargs_parse_arguments(2, (char const * []){"argz1", "argz2"}, args), 2);
	assert_string_equal(args[0].result.value, "argz1");
	assert_string_equal(args[1].result.value, "argz2");
}

/************************************************************************/

#define mandatory_header_opts \
struct optargs_argument * args = (struct optargs_argument []) \
{ \
	{.name = "A", .type = optargs_argument_group}, \
	{.name = "a", .type = optargs_argument_group_member, .description = "foobar"}, \
	{.name = "b", .type = optargs_argument_group_member, .description = "foobar"}, \
	optargs_argument_eol \
};

static void
UT_optargs_parse_arguments__mandatory_header_argument__no_arguments()
{
	mandatory_header_opts
	assert_int_equal(optargs_parse_arguments(0, NULL, args), -EINVAL);
}

static void
UT_optargs_parse_arguments__mandatory_header_argument__a()
{
	mandatory_header_opts
	assert_int_equal(optargs_parse_arguments(1, (char const * []){"a"}, args), 1);
	assert_ptr_equal(args[0].result.match, &args[1]);
	assert_string_equal(args[1].result.value, "a");
	assert_null(args[2].result.value);
}

static void
UT_optargs_parse_arguments__mandatory_header_argument__b()
{
	mandatory_header_opts
	assert_int_equal(optargs_parse_arguments(1, (char const * []){"b"}, args), 1);
	assert_string_equal(args[0].result.match, &args[2]);
	assert_null(args[1].result.value);
	assert_string_equal(args[2].result.value, "b");
}

static void
UT_optargs_parse_arguments__mandatory_header_argument__c()
{
	mandatory_header_opts
	assert_int_equal(optargs_parse_arguments(1, (char const * []){"c"}, args), -EINVAL);
}

#undef mandatory_header_opts

/************************************************************************/

#define mandatory_header_opts \
struct optargs_argument * args = (struct optargs_argument []) \
{ \
	{.name = "A", .type = optargs_argument_group}, \
	{.name = "a", .type = optargs_argument_group_member, .description = "foobar"}, \
	{.name = "b", .type = optargs_argument_group_member, .description = "foobar"}, \
	{.name = "B", .type = optargs_argument_group}, \
	{.name = "c", .type = optargs_argument_group_member, .description = "foobar"}, \
	{.name = "d", .type = optargs_argument_group_member, .description = "foobar"}, \
	optargs_argument_eol \
};

static void
UT_optargs_parse_arguments__mandatory_header_argument_2__no_arguments()
{
	mandatory_header_opts
	assert_int_equal(optargs_parse_arguments(1, (char const * []){"a"}, args), -EINVAL);
}

static void
UT_optargs_parse_arguments__mandatory_header_argument_2__ac()
{
	mandatory_header_opts
	assert_int_equal(optargs_parse_arguments(2, (char const * []){"a", "c"}, args), 2);
	assert_ptr_equal(args[0].result.match, &args[1]);
	assert_string_equal(args[1].result.value, "a");
	assert_null(args[2].result.value);
	assert_ptr_equal(args[3].result.match, &args[4]);
	assert_string_equal(args[4].result.value, "c");
	assert_null(args[5].result.value);
}


static void
UT_optargs_parse_arguments__mandatory_header_argument_2__ad()
{
	mandatory_header_opts
	assert_int_equal(optargs_parse_arguments(2, (char const * []){"a", "d"}, args), 2);
	assert_ptr_equal(args[0].result.match, &args[1]);
	assert_string_equal(args[1].result.value, "a");
	assert_null(args[2].result.value);
	assert_ptr_equal(args[3].result.match, &args[5]);
	assert_null(args[4].result.value);
	assert_string_equal(args[5].result.value, "d");
}

static void
UT_optargs_parse_arguments__mandatory_header_argument_2__ae()
{
	mandatory_header_opts
	assert_int_equal(optargs_parse_arguments(2, (char const * []){"a", "e"}, args), -EINVAL);
}

/************************************************************************/

static void
UT_optargs_parse_arguments__NULL()
{
	#include "args_sample.inc"
	expect_assert_failure(optargs_parse_arguments(5,
				NULL, args_sample));

	expect_assert_failure(optargs_parse_arguments(5,
				(char const * []){"exe", "foo", "bar", "keke", "zou"}, NULL));

	expect_assert_failure(optargs_parse_arguments(5,
				NULL, NULL));
}

/************************************************************************/

static void
UT_optargs_parse_arguments__sanity_check()
{
	expect_assert_failure(optargs_parse_arguments(
			2, (char const * []){"1", "2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_any_opt, .description = "foobar"},
				{.name = "B", .type = optargs_argument_any, .description = "foobar"},
				{.name = "C", .type = optargs_argument_any, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_arguments(
			2, (char const * []){"1", "2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_any, .description = "foobar"},
				{.name = "B", .type = optargs_argument_any_opt, .description = "foobar"},
				{.name = "C", .type = optargs_argument_any, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_arguments(
			2, (char const * []){"1", "2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_any, .description = "foobar"},
				{.name = "B", .type = optargs_argument_any, .description = "foobar"},
				{.name = "C", .type = optargs_argument_any_opt, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_arguments(
			2, (char const * []){"1", "2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_any, .description = "foobar"},
				{.name = "B", .type = optargs_argument_any_opt, .description = "foobar"},
				optargs_argument_divider,
				{.name = "C", .type = optargs_argument_any_opt, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_arguments(
			2, (char const * []){"1", "2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_any_opt, .description = "foobar"},
				{.name = "B", .type = optargs_argument_any_opt, .description = "foobar"},
				{.name = "C", .type = optargs_argument_any, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_arguments(
			2, (char const * []){"1", "2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_any_opt, .description = "foobar"},
				{.name = "B", .type = optargs_argument_any_opt, .description = "foobar"},
				{.name = "C", .type = optargs_argument_any_opt, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_arguments(
			2, (char const * []){"1", "2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_any_opt, .description = "foobar"},
				{.name = "B", .type = optargs_argument_any, .description = "foobar"},
				{.name = "C", .type = optargs_argument_any_opt, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_arguments(
			2, (char const * []){"b.1", "c.2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "a.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_divider,
				{.name = "B", .type = optargs_argument_group, .description = "foobar"},
				{.name = "b.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_divider,
				{.name = "C", .type = optargs_argument_group, .description = "foobar"},
				{.name = "c.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_arguments(
			2, (char const * []){"a.1", "c.2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_group, .description = "foobar"},
				{.name = "a.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_divider,
				{.name = "B", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "b.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_argument_group, .description = "foobar"},
				{.name = "c.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_arguments(
			2, (char const * []){"a.1", "b.2"},
			(struct optargs_argument []) {
				optargs_argument_divider,
				{.name = "A", .type = optargs_argument_group, .description = "foobar"},
				{.name = "a.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_argument_group, .description = "foobar"},
				{.name = "b.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "c.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_divider,
				optargs_argument_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_arguments(
			2, (char const * []){"a.1", "b.1"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_group, .description = "foobar"},
				{.name = "a.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "b.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_divider,
				{.name = "C", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "c.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_arguments(
			2, (char const * []){"a.1", "c.2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "a.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "b.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_argument_group, .description = "foobar"},
				{.name = "c.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	assert_int_equal(2,
		optargs_parse_arguments(
			2, (char const * []){"a.1", "b.2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "a.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "b.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "c.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);

	expect_assert_failure(optargs_parse_arguments(
			2, (char const * []){"a.1", "b.2"},
			(struct optargs_argument []) {
				{.name = "A", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "a.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "a.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "B", .type = optargs_argument_group, .description = "foobar"},
				{.name = "b.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "b.3", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "C", .type = optargs_argument_group_opt, .description = "foobar"},
				{.name = "c.1", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.2", .type = optargs_argument_group_member, .description = "foobar"},
				{.name = "c.3", .type = optargs_argument_group_member, .description = "foobar"},
				optargs_argument_eol
			}
		)
	);
}

/************************************************************************/

static void
UT_optargs_print_help()
{
	#include "opts_sample.inc"
	#include "args_sample.inc"
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
	#include "opts_sample.inc"
	#include "args_sample.inc"
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
UT_optargs_option_string()
{
	#include "opts_sample.inc"
	#include "args_sample.inc"
	char const * argv[] = { "", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_options(argc, argv, opts_sample), 7);
	assert_int_equal(optargs_parse_arguments(argc - 7, argv + 7, args_sample), 6);

	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_A));
	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_B));
	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_C));
	expect_assert_failure(optargs_option_string(opts_sample,  SAMPLE_OPT_D));
	expect_assert_failure(optargs_option_string(opts_sample,  SAMPLE_OPT_E));
	expect_assert_failure(optargs_option_string(opts_sample,  SAMPLE_OPT_F));
	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_G));
	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_H));
	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_I));
	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_J));
	assert_string_equal(optargs_option_string(opts_sample,    SAMPLE_OPT_K), "bar");
	assert_string_equal(optargs_option_string(opts_sample,    SAMPLE_OPT_L), "fez");
	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_M));
	assert_null(optargs_option_string(opts_sample,            SAMPLE_OPT_N));
}

/************************************************************************/

static void
UT_optargs_option_string__NULL()
{
	expect_assert_failure(optargs_option_string(NULL, 0));
	expect_assert_failure(optargs_option_string(NULL, 1));
}

/************************************************************************/

#if !defined(NDEBUG) || defined(UNIT_TESTING)

static void
UT_optargs_option_string__ENOENT()
{
	#include "opts_sample.inc"
	expect_assert_failure(optargs_option_string(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_option_count()
{
	#include "opts_sample.inc"
	#include "args_sample.inc"
	char const * argv[] = { "", "-abcaba", "--gggg", "--jjjj", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_options(argc, argv, opts_sample), 10);
	assert_int_equal(optargs_parse_arguments(argc - 10, argv + 10, args_sample), 6);

	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_A), 3);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_B), 2);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_C), 1);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_D), 1);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_E), 1);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_F), 1);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_G), 1);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_H), 0);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_I), 0);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_J), 1);
	expect_assert_failure(optargs_option_count(opts_sample,   SAMPLE_OPT_K));
	expect_assert_failure(optargs_option_count(opts_sample,   SAMPLE_OPT_L));
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_M), 0);
	assert_int_equal(optargs_option_count(opts_sample,        SAMPLE_OPT_N), 0);
}

/************************************************************************/

static void
UT_optargs_option_count__NULL()
{
	expect_assert_failure(optargs_option_count(NULL, 0));
	expect_assert_failure(optargs_option_count(NULL, 1));
}

/************************************************************************/

#if !defined(NDEBUG) || defined(UNIT_TESTING)

static void
UT_optargs_option_count__ENOENT()
{
	#include "opts_sample.inc"
	expect_assert_failure(optargs_option_count(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_option_type()
{
	#include "opts_sample.inc"
	#include "args_sample.inc"

	char const * argv[] = { "", "-abcaba", "--gggg", "--jjjj", "-D", "--eeee", "-k", "bar", "--llll=fez", "-F", "arg1", "arg2", "g2", "arg3", "arg4", "g5" };
	int const argc = sizeof(argv)/sizeof(char const *);

	assert_int_equal(optargs_parse_options(argc, argv, opts_sample), 10);
	assert_int_equal(optargs_parse_arguments(argc - 10, argv + 10, args_sample), 6);

	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_A), optargs_flag);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_B), optargs_flag);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_C), optargs_flag);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_D), optargs_flag);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_E), optargs_flag);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_F), optargs_flag);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_G), optargs_flag);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_H), optargs_undef);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_I), optargs_undef);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_J), optargs_flag);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_K), optargs_argument);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_L), optargs_argument);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_M), optargs_undef);
	assert_int_equal(optargs_option_type(opts_sample, SAMPLE_OPT_N), optargs_undef);
}

/************************************************************************/

static void
UT_optargs_option_type__NULL()
{
	expect_assert_failure(optargs_option_type(NULL, 0));
	expect_assert_failure(optargs_option_type(NULL, 1));
}

/************************************************************************/

#if !defined(NDEBUG) || defined(UNIT_TESTING)

static void
UT_optargs_option_type__ENOENT()
{
	#include "opts_sample.inc"
	expect_assert_failure(optargs_option_type(opts_sample, 99999));
}

#endif

/************************************************************************/

static void
UT_optargs_argument_value()
{
	struct optargs_argument args[] =
	{
		{.name = "a", .type = optargs_argument_any, .description = "aaaa"},
		{.name = "b", .type = optargs_argument_any, .description = "bbbb"},
		{.name = "C", .type = optargs_argument_group},
		{.name = "c1", .type = optargs_argument_group_member, .description = "c1c1" },
		{.name = "c2", .type = optargs_argument_group_member, .description = "c2c2" },
		{.name = "c3", .type = optargs_argument_group_member, .description = "c3c3" },
		{.name = "d", .type = optargs_argument_any, .description = "dddd"},
		{.name = "D", .type = optargs_argument_group},
		{.name = "d1", .type = optargs_argument_group_member, .description = "d1d1" },
		{.name = "d2", .type = optargs_argument_group_member, .description = "d2d2" },
		{.name = "d3", .type = optargs_argument_group_member, .description = "d3d3" },
		{.name = "e", .type = optargs_argument_any_opt, .description = "eeee"},
		{.name = "f", .type = optargs_argument_any_opt, .description = "ffff"},
		optargs_argument_sink
	};

	assert_int_equal(optargs_parse_arguments(6, (char const * []){"foo", "bar", "c2", "keke", "d3", "har"}, args), 6);
	assert_string_equal(optargs_argument_value(&args[0]), "foo");
	assert_string_equal(optargs_argument_value(&args[1]), "bar");
	assert_string_equal(optargs_argument_value(&args[2]), "c2");
	assert_null(optargs_argument_value(&args[3]));
	assert_string_equal(optargs_argument_value(&args[4]), "c2");
	assert_null(optargs_argument_value(&args[5]));
	assert_string_equal(optargs_argument_value(&args[6]), "keke");
	assert_string_equal(optargs_argument_value(&args[7]), "d3");
	assert_null(optargs_argument_value(&args[8]));
	assert_null(optargs_argument_value(&args[9]));
	assert_string_equal(optargs_argument_value(&args[10]), "d3");
	assert_string_equal(optargs_argument_value(&args[11]), "har");
	assert_null(optargs_argument_value(&args[12]));
	assert_null(optargs_argument_value(&args[13]));
}

/************************************************************************/

static void
UT_optargs_argument_value__NULL()
{
	expect_assert_failure(optargs_argument_value(NULL));
}

/************************************************************************/

static void
UT_optargs_argument_offset()
{
	struct optargs_argument args[] =
	{
		{.name = "a", .type = optargs_argument_any, .description = "aaaa"},
		{.name = "b", .type = optargs_argument_any, .description = "bbbb"},
		{.name = "C", .type = optargs_argument_group},
		{.name = "c1", .type = optargs_argument_group_member, .description = "c1c1" },
		{.name = "c2", .type = optargs_argument_group_member, .description = "c2c2" },
		{.name = "c3", .type = optargs_argument_group_member, .description = "c3c3" },
		{.name = "d", .type = optargs_argument_any, .description = "dd"},
		{.name = "D", .type = optargs_argument_group, .description = "dddd"},
		{.name = "d1", .type = optargs_argument_group_member, .description = "d1d1" },
		{.name = "d2", .type = optargs_argument_group_member, .description = "d2d2" },
		{.name = "d3", .type = optargs_argument_group_member, .description = "d3d3" },
		{.name = "e", .type = optargs_argument_any_opt, .description = "eeee"},
		{.name = "F", .type = optargs_argument_group_opt, .description = "ffff"},
		{.name = "f1", .type = optargs_argument_group_member, .description = "f1f1" },
		{.name = "f2", .type = optargs_argument_group_member, .description = "f2f2" },
		{.name = "f3", .type = optargs_argument_group_member, .description = "f3f3" },
		{.name = "G", .type = optargs_argument_group_opt, .description = "gggg"},
		{.name = "g1", .type = optargs_argument_group_member, .description = "g1g1" },
		{.name = "g2", .type = optargs_argument_group_member, .description = "g2g2" },
		{.name = "g3", .type = optargs_argument_group_member, .description = "g3g3" },
		optargs_argument_sink
	};

	assert_int_equal(7,
			optargs_parse_arguments(7,
				(char const * []){"foo", "bar", "c2", "keke", "d3", "har", "f1"},
				args));

	expect_assert_failure(optargs_argument_offset(&args[0]));   // foo
	expect_assert_failure(optargs_argument_offset(&args[1]));  // bar
	assert_int_equal(optargs_argument_offset(&args[2]),   2);  // C=c2
	expect_assert_failure(optargs_argument_offset(&args[3]));  // c1
	expect_assert_failure(optargs_argument_offset(&args[4]));  // c2*
	expect_assert_failure(optargs_argument_offset(&args[5]));  // c3
	expect_assert_failure(optargs_argument_offset(&args[6]));  // d
	assert_int_equal(optargs_argument_offset(&args[7]),   3);  // D=d3
	expect_assert_failure(optargs_argument_offset(&args[8]));  // d1
	expect_assert_failure(optargs_argument_offset(&args[9]));  // d2
	expect_assert_failure(optargs_argument_offset(&args[10])); // d3*
	expect_assert_failure(optargs_argument_offset(&args[11])); // e=har
	assert_int_equal(optargs_argument_offset(&args[12]),  1);  // F=f1
	expect_assert_failure(optargs_argument_offset(&args[13])); // f1*
	expect_assert_failure(optargs_argument_offset(&args[14])); // f2
	expect_assert_failure(optargs_argument_offset(&args[15])); // f3
	assert_int_equal(optargs_argument_offset(&args[16]),  -1);  // G=
	expect_assert_failure(optargs_argument_offset(&args[17])); // g1
	expect_assert_failure(optargs_argument_offset(&args[18])); // g2
	expect_assert_failure(optargs_argument_offset(&args[19])); // g3
	expect_assert_failure(optargs_argument_offset(&args[20])); // -
}

/************************************************************************/

static void
UT_optargs_argument_offset__NULL()
{
	expect_assert_failure(optargs_argument_offset(NULL));
}

/************************************************************************/

static void
UT_optargs_argument_index()
{
	struct optargs_argument args[] =
	{
		{.name = "a", .type = optargs_argument_any, .description = "aaaa"},
		{.name = "b", .type = optargs_argument_any, .description = "bbbb"},
		{.name = "C", .type = optargs_argument_group},
		{.name = "c1", .type = optargs_argument_group_member, .description = "c1c1" },
		{.name = "c2", .type = optargs_argument_group_member, .description = "c2c2" },
		{.name = "c3", .type = optargs_argument_group_member, .description = "c3c3" },
		{.name = "d", .type = optargs_argument_any, .description = "dd"},
		{.name = "D", .type = optargs_argument_group, .description = "dddd"},
		{.name = "d1", .type = optargs_argument_group_member, .description = "d1d1" },
		{.name = "d2", .type = optargs_argument_group_member, .description = "d2d2" },
		{.name = "d3", .type = optargs_argument_group_member, .description = "d3d3" },
		{.name = "e", .type = optargs_argument_any_opt, .description = "eeee"},
		{.name = "F", .type = optargs_argument_group_opt, .description = "ffff"},
		{.name = "f1", .type = optargs_argument_group_member, .description = "f1f1" },
		{.name = "f2", .type = optargs_argument_group_member, .description = "f2f2" },
		{.name = "f3", .type = optargs_argument_group_member, .description = "f3f3" },
		{.name = "G", .type = optargs_argument_group_opt, .description = "gggg"},
		{.name = "g1", .type = optargs_argument_group_member, .description = "g1g1" },
		{.name = "g2", .type = optargs_argument_group_member, .description = "g2g2" },
		{.name = "g3", .type = optargs_argument_group_member, .description = "g3g3" },
		optargs_argument_sink
	};

	assert_int_equal(7,
			optargs_parse_arguments(7,
				(char const * []){"foo", "bar", "c2", "keke", "d3", "har", "f1"},
				args));

	expect_assert_failure(optargs_argument_index(args, 0));  // a
	expect_assert_failure(optargs_argument_index(args, 1));  // b
	assert_int_equal(optargs_argument_index(args, 2),   4);  // C
	expect_assert_failure(optargs_argument_index(args, 3));  // c1
	expect_assert_failure(optargs_argument_index(args, 4));  // c2
	expect_assert_failure(optargs_argument_index(args, 5));  // c3
	expect_assert_failure(optargs_argument_index(args, 6));  // d
	assert_int_equal(optargs_argument_index(args, 7),  10);  // D
	expect_assert_failure(optargs_argument_index(args, 8));  // d1
	expect_assert_failure(optargs_argument_index(args, 9));  // d2
	expect_assert_failure(optargs_argument_index(args, 10)); // d3
	expect_assert_failure(optargs_argument_index(args, 11)); // e
	assert_int_equal(optargs_argument_index(args, 12), 13);  // F
	expect_assert_failure(optargs_argument_index(args, 13)); // f1
	expect_assert_failure(optargs_argument_index(args, 14)); // f2
	expect_assert_failure(optargs_argument_index(args, 15)); // f3
	assert_int_equal(optargs_argument_index(args, 16), -1);  // G
	expect_assert_failure(optargs_argument_index(args, 17)); // g1
	expect_assert_failure(optargs_argument_index(args, 18)); // g2
	expect_assert_failure(optargs_argument_index(args, 19)); // g3
	expect_assert_failure(optargs_argument_index(args, 20)); // sink
}

/************************************************************************/

static void
UT_optargs_argument_index__NULL()
{
	#include "args_sample.inc"
	args_sample[2].result.match = &args_sample[3];
	expect_assert_failure(optargs_argument_index(NULL, -1));
	expect_assert_failure(optargs_argument_index(NULL, 0));
	expect_assert_failure(optargs_argument_index(args_sample, -1));
}

/************************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_optargs_parse_options__no_options),
		cmocka_unit_test(UT_optargs_parse_options__argument_but_no_options),
		cmocka_unit_test(UT_optargs_parse_options__option_after_argument),
		cmocka_unit_test(UT_optargs_parse_options__option_after_double_hyphen),
		cmocka_unit_test(UT_optargs_parse_options__double_hyphen_before_arguments),
		cmocka_unit_test(UT_optargs_parse_options__option_middle_of_arguments),
		cmocka_unit_test(UT_optargs_parse_options__option_and_argument),
		cmocka_unit_test(UT_optargs_parse_options__just_option),
		cmocka_unit_test(UT_optargs_parse_options__just_long_option),
		cmocka_unit_test(UT_optargs_parse_options__long_option_and_arguments),
		cmocka_unit_test(UT_optargs_parse_options__short_option_and_arguments),
		cmocka_unit_test(UT_optargs_parse_options__same_long_and_short_option),
		cmocka_unit_test(UT_optargs_parse_options__different_long_and_short_option),
		cmocka_unit_test(UT_optargs_parse_options__two_different_shorts),
		cmocka_unit_test(UT_optargs_parse_options__two_different_shorts_together),
		cmocka_unit_test(UT_optargs_parse_options__hidden_long_option),
		cmocka_unit_test(UT_optargs_parse_options__hidden_short_option),
		cmocka_unit_test(UT_optargs_parse_options__long_option_with_assigned_mandatory_argument),
		cmocka_unit_test(UT_optargs_parse_options__long_option_with_separate_mandatory_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_assigned_mandatory_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_appended_mandatory_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_separated_mandatory_argument),
		cmocka_unit_test(UT_optargs_parse_options__long_option_with_assigned_optional_argument),
		cmocka_unit_test(UT_optargs_parse_options__long_option_with_separated_optional_argument),
		cmocka_unit_test(UT_optargs_parse_options__long_option_with_assigned_optional_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_assigned_optional_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_appended_optional_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_separated_mandatory_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_separated_optional_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_missing_argument),
		cmocka_unit_test(UT_optargs_parse_options__long_option_missing_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_empty_mandatory_argument),
		cmocka_unit_test(UT_optargs_parse_options__short_option_with_empty_optional_argument),
		cmocka_unit_test(UT_optargs_parse_options__long_option_with_empty_mandatory_argument),
		cmocka_unit_test(UT_optargs_parse_options__long_option_with_empty_optional_argument),
		cmocka_unit_test(UT_optargs_parse_options__megamix),
		cmocka_unit_test(UT_optargs_parse_options__redefine_flag_to_argument),
		cmocka_unit_test(UT_optargs_parse_options__redefine_argument_to_flag),
		cmocka_unit_test(UT_optargs_parse_options__redefine_argument_to_argument),
		cmocka_unit_test(UT_optargs_parse_options__invalid_short_option),
		cmocka_unit_test(UT_optargs_parse_options__invalid_short_option_with_other_options),
		cmocka_unit_test(UT_optargs_parse_options__invalid_long_option),
		cmocka_unit_test(UT_optargs_parse_options__invalid_long_option_with_other_options),
		cmocka_unit_test(UT_optargs_parse_options__sanity_check),
		cmocka_unit_test(UT_optargs_parse_options__argument_sink),
		cmocka_unit_test(UT_optargs_parse_options__NULL),

		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__required_arguments),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__invalid_group_member_argument),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__required_arguments_and_optional),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__invalid_argument__group_title_as_member),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__required_arguments_and_optional),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__invalid_fourth_argument),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__missing_argument),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__no_arguments),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__double_hyphen_not_special),
		cmocka_unit_test(UT_optargs_parse_arguments__four_arguments__all_four),

		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_fixed_argument),
		cmocka_unit_test(UT_optargs_parse_arguments__NULL),
		cmocka_unit_test(UT_optargs_parse_arguments__sanity_check),

		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_header_argument__no_arguments),
		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_header_argument__a),
		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_header_argument__b),
		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_header_argument__c),

		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_header_argument_2__no_arguments),
		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_header_argument_2__ac),
		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_header_argument_2__ad),
		cmocka_unit_test(UT_optargs_parse_arguments__mandatory_header_argument_2__ae),

		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__no_arguments),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__invalid_argument),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__mandatory_subargument),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__mandatory_any_subargument),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__no_subarguments),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__unwanted_subargument),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__missing_mandatory_subargument_1),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__missing_mandatory_subargument_2),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_1),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_2),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_3),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_4),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_5),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_6),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_7),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_8),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_9),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_10),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_11),
		cmocka_unit_test(UT_optargs_parse_arguments__subarguments__subargception_12),

		cmocka_unit_test(UT_optargs_print_help),
		cmocka_unit_test(UT_optargs_print_help__NULL),

		cmocka_unit_test(UT_optargs_option_string),
		cmocka_unit_test(UT_optargs_option_string__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_option_string__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_option_count),
		cmocka_unit_test(UT_optargs_option_count__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_option_count__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_option_type),
		cmocka_unit_test(UT_optargs_option_type__NULL),
#if !defined(NDEBUG) || defined(UNIT_TESTING)
		cmocka_unit_test(UT_optargs_option_type__ENOENT),
#endif

		cmocka_unit_test(UT_optargs_argument_value),
		cmocka_unit_test(UT_optargs_argument_value__NULL),

		cmocka_unit_test(UT_optargs_argument_offset),
		cmocka_unit_test(UT_optargs_argument_offset__NULL),

		cmocka_unit_test(UT_optargs_argument_index),
		cmocka_unit_test(UT_optargs_argument_index__NULL)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/************************************************************************/
