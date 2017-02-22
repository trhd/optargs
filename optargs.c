/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optargs.h"

enum constants
{
	MAX_WIDTH = 79,
	LEFT_COLUMN_MAX_WIDTH = 40
};

static int
max(const int a, const int b)
{
	return a > b ? a : b;
}

static int
min(const int a, const int b)
{
	return a > b ? b : a;
}

static int
word_length(char const * s)
{
	assert(s);

	return strcspn(s, " =\t\v\n\r\0");
}

static bool
is_word_separator(char const c)
{
	return c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r' || c == '\0';
}

static bool
argument_is_valid(struct optargs_arg const * const arg)
{
	return !arg || (arg->type != _optargs_arg_eol && arg->type != _optargs_arg_sink);
}

static bool
option_is_valid(struct optargs_opt const * const o)
{
	assert(o);

	return argument_is_valid(o->argument);
}

static int
opt_missing_arg_error(struct optargs_opt const * const opt)
{
	assert(opt);

	char buf[5] = "'-";

	fprintf(stderr, "Mandatory argument for option %s%s%s%s%s missing.\n",
			opt->long_option ? "'--" : "", opt->long_option ? opt->long_option : "", opt->long_option ? "'" : "",
			opt->long_option && opt->short_option ? " / " : "",
			opt->short_option ? strncat(strncat(buf, &opt->short_option, 1), "'", 1) : "");

	return -EINVAL;
}

static int
non_wanted_argument_error(struct optargs_opt const * const opt)
{
	assert(opt);

	char buf[5] = "'-";

	fprintf(stderr, "The option %s%s%s%s%s doesn't take an argument.\n",
			opt->long_option ? "'--" : "", opt->long_option ? opt->long_option : "", opt->long_option ? "'" : "",
			opt->long_option && opt->short_option ? " / " : "",
			opt->short_option ? strncat(strncat(buf, &opt->short_option, 1), "'", 1) : "");

	return -EINVAL;
}

static int
redefine_option_argument_error(struct optargs_opt const * const opt)
{
	assert(opt);

	char buf[5] = "'-";

	fprintf(stderr, "Conflicting arguments given for the option %s%s%s%s%s.\n",
			opt->long_option ? "'--" : "", opt->long_option ? opt->long_option : "", opt->long_option ? "'" : "",
			opt->long_option && opt->short_option ? " / " : "",
			opt->short_option ? strncat(strncat(buf, &opt->short_option, 1), "'", 1) : "");

	return -EINVAL;
}

static int
unmatching_optional_argument_error(char const * const opt, char const * const hdr)
{
	assert(opt);
	assert(hdr);

	fprintf(stderr, "'%s' is not a valid value for optional argument '%s'.\n",
			opt, hdr);
	return -EINVAL;
}

static int
unmatching_mandatory_argument_error(char const * const opt, char const * const hdr)
{
	assert(opt);
	assert(hdr);

	fprintf(stderr, "'%s' is not a valid value for mandatory argument '%s'.\n",
			opt, hdr);
	return -EINVAL;
}

static bool
starts_with_single_hyphen(char const * const str)
{
	assert(str);

	return str[0] == '-';
}

static bool
starts_with_double_hyphen(char const * const opt)
{
	assert(opt);
	assert(starts_with_single_hyphen(opt));

	return opt[1] == '-';
}

static bool
is_empty_argument(char const * const str)
{
	assert(str);

	return *str == '\0';
}

static bool
is_double_hyphen(char const * const opt)
{
	assert(opt);

	return opt[0] == '-' && opt[1] == '-' && opt[2] == '\0';
}

static int
long_option_is_invalid(char const * o, int const l)
{
	assert(o);
	assert(l > 0);

	fprintf(stderr, "Unrecognized long option '%.*s'.\n", l, o);

	return -EINVAL;
}

static int
short_option_is_invalid(char const c)
{
	fprintf(stderr, "Unrecognized short option '%c'.\n", c);

	return -EINVAL;
}

static bool
option_is_divider(struct optargs_opt const * const opt)
{
	assert(opt);

	return !(opt->short_option || opt->long_option);
}

static bool
option_is_hidden(struct optargs_opt const * const opt)
{
	assert(opt);

	return !option_is_divider(opt) && !opt->description;
}

static bool
argument_is_header(struct optargs_arg const * const arg)
{
	assert(arg);

	return arg->type == optargs_arg_group || arg->type == optargs_arg_group_opt;
}

static bool
argument_is_mandatory(struct optargs_arg const * const arg)
{
	assert(arg);

	return arg->type == optargs_arg_any || arg->type == optargs_arg_group;
}

static bool
argument_is_divider(struct optargs_arg const * const arg)
{
	assert(arg);

	return arg->type == _optargs_arg_div;
}

static bool
argument_is_fixed(struct optargs_arg const * const arg)
{
	assert(arg);

	return arg->type == optargs_arg_any || arg->type == optargs_arg_any_opt;
}

static struct optargs_opt *
locate_short_option(struct optargs_opt * o, char const c)
{
	assert(o);

	for ( ; option_is_valid(o) ; o++)
		if (o->short_option == c)
			return o;

	return NULL;
}

static void
locate_long_option(struct optargs_opt ** o, char const * l, int const ll)
{
	assert(o);
	assert(*o);
	assert(l);
	assert(ll > 0);

	for ( ; option_is_valid(*o) ; (*o)++)
	{
		if (!(*o)->long_option)
			continue;

		if ((int)strlen((*o)->long_option) != ll)
			continue;

		if ((*o)->long_option && !strncmp((*o)->long_option, l, ll))
			break;
	}
}

static void
mark_default_option(struct optargs_opt * opt)
{
	assert(opt);

	opt->result.type = optargs_count;
	opt->result.value.count++;
}

static int
mark_no_argument_option(struct optargs_opt * opt, bool const assign)
{
	assert(opt);

	if (assign)
		return non_wanted_argument_error(opt);

	mark_default_option(opt);

	return 0;
}

static int
mark_argument_option(struct optargs_opt * opt, char const * const arg)
{
	if (optargs_res_type(&opt->result))
		return redefine_option_argument_error(opt);

	if (!arg)
		return opt_missing_arg_error(opt);

	opt->result.type = optargs_string;
	opt->result.value.string = arg;

	return 1;
}

static int
mark_optional_argument_option(struct optargs_opt * opt,
		char const * const arg, bool const assign)
{
	assert(opt);

	if (assign && arg)
		return mark_argument_option(opt, arg);

	if (optargs_res_type(&opt->result) == optargs_string)
		return redefine_option_argument_error(opt);

	mark_default_option(opt);

	return 0;
}

static int
mark_option(struct optargs_opt * opt, char const * const arg, bool const force)
{
	assert(opt);

	if (!opt->argument)
		return mark_no_argument_option(opt, force);

	switch (opt->argument->type)
	{
		case optargs_arg_any:
		case optargs_arg_group:
			return mark_argument_option(opt, arg);
		case _optargs_arg_eol:
		case _optargs_arg_sink:
		case _optargs_arg_div:
		case optargs_arg_any_opt:
		case optargs_arg_group_opt:
			return mark_optional_argument_option(opt, arg, force);
		case optargs_arg_group_member:
			break;
	}
	abort();
}

static int
parse_short_option(char const * c, char const * n, struct optargs_opt * const o)
{
	assert(c);
	assert(o);
	assert(starts_with_single_hyphen(c));

	struct optargs_opt *t;
	int r;

	if (*++c == '\0')
		return short_option_is_invalid(*c);

	for (r = 0 ; r == 0 && *c != '\0'; c++)
	{
		t = locate_short_option(o, *c);

		if (!t)
			r = short_option_is_invalid(*c);
		else if (c[1] == '=')
			r = mark_option(t, c + 2, true);
		else if (c[1] != '\0')
			r = mark_option(t, c + 1, false);
		else
		{
			r = mark_option(t, n, false);
			if (r >= 0)
				r++;
		}
	}

	return r;
}

static int
parse_long_option(char const * c, char const * n, struct optargs_opt * o)
{
	assert(c);
	assert(o);

	int r, d;

	assert(c[0] == '-' && c[1] == '-');
	c += 2;
	d = word_length(c);

	locate_long_option(&o, c, d);

	if (!option_is_valid(o))
		r = long_option_is_invalid(c, d);
	else if (c[d] == '=')
		r = mark_option(o, &c[d + 1], true);
	else if ((r = mark_option(o, n, false)) >= 0)
		r++;

	return r;
}

static void
clear_opts(struct optargs_opt * o)
{
	assert(o);

	if (o)
		for ( ; option_is_valid(o) ; o++)
			memset(&o->result, 0, sizeof(o->result));
}

static void
clear_args(struct optargs_arg * a)
{
	assert(a);

	for ( ; argument_is_valid(a) ; a++)
		memset(&a->result, 0, sizeof(a->result));
}

#if !defined(NDEBUG) || defined(UNIT_TESTING)

static void
assert_argument_required_fields(struct optargs_arg const * const arg)
{
	switch (arg->type)
	{
		case optargs_arg_any:
		case optargs_arg_any_opt:
		case optargs_arg_group_member:
			assert(arg->name);
			assert(arg->description);
			break;
		case optargs_arg_group:
		case optargs_arg_group_opt:
			assert(arg->name);
			break;
		case _optargs_arg_div:
			assert(!arg->description);
			break;
		default:
			break;
	}
}

static void
assert_option_argument_required_fields(struct optargs_arg const * const arg)
{
	switch (arg->type)
	{
		case optargs_arg_any:
		case optargs_arg_any_opt:
		case optargs_arg_group_member:
			assert(arg->name);
			break;
		case optargs_arg_group:
		case optargs_arg_group_opt:
			assert(arg->name);
			break;
		case _optargs_arg_div:
			assert(!arg->description);
			break;
		default:
			break;
	}
}

static void
assert_single_set_of_optional_arguments(struct optargs_arg const * args)
{
	assert(args);

	for (int n = -1 ; argument_is_valid(args) ; args++)
	{
		if (args->type == optargs_arg_group_member
				|| args->type == _optargs_arg_div)
			continue;

		if (args->type == optargs_arg_any_opt
				|| args->type == optargs_arg_group_opt)
		{
			assert(n);

			if (n == -1)
				n = 1;
			else
				n++;
		}
		else if (n > 0)
			n--;
	}
}

static void
assert_argument_sanity(struct optargs_arg const * args)
{
	assert(args);

	assert_single_set_of_optional_arguments(args);

	for ( ; argument_is_valid(args) ; args++)
		assert_argument_required_fields(args);
}

static void
assert_option_sanity(struct optargs_opt const * opts)
{
	assert(opts);

	for ( ; option_is_valid(opts) ; opts++)
	{
		if (opts->argument)
		{
			struct optargs_arg const * arg = opts->argument;

			assert_option_argument_required_fields(arg);
			assert_single_set_of_optional_arguments(arg);

			for ( ; argument_is_valid(arg) ; arg++ );
			assert(arg->type != _optargs_arg_sink);
		}
	}
}

#else

# define assert_argument_sanity(...)
# define assert_option_sanity(...)

#endif

int
optargs_parse_opts(int const ac, char const * const * const av, struct optargs_opt * const opts)
{
	assert(ac > 0);
	assert(av);
	assert(opts);
	assert_option_sanity(opts);

	int i, j;
	char const *o, *n;

	clear_opts(opts);

	for (i = 1; i < ac; i += j)
	{
		o = av[i];
		assert(o);
		n = i < ac - 1 ? av[i + 1] : NULL;

		if (is_empty_argument(o))
		{
			j = 1;
			continue;
		}

		if (!starts_with_single_hyphen(o))
			return i;

		if (!starts_with_double_hyphen(o))
			j = parse_short_option(o, n, opts);
		else if (is_double_hyphen(o))
			return i + 1;
		else
			j = parse_long_option(o, n, opts);

		if (j < 0)
			return j;
	}

	return i;
}

static void
count_mandatory_and_optinal_arguments(struct optargs_arg const * args, int * mand, int * opts)
{
	assert(args);
	assert(mand);
	assert(opts);

	for (*mand = 0, *opts = 0 ; argument_is_valid(args) ; args++)
	{
		switch (args->type)
		{
			case optargs_arg_group:
			case optargs_arg_any:
				++*mand;
				break;
			case optargs_arg_group_opt:
			case optargs_arg_any_opt:
				++*opts;
				break;
			case optargs_arg_group_member:
			case _optargs_arg_div:
				break;
			case _optargs_arg_eol:
			case _optargs_arg_sink:
				abort();
		}
	}

	if (args->type == _optargs_arg_sink)
		*opts = -1;
}

bool
optargs_parse_args(int const ac, char const * const * const av, struct optargs_arg * args)
{
	assert(ac >= 0);
	assert(!ac || av);
	assert(args);
	assert_argument_sanity(args);

	int m, o, i;
	struct optargs_arg * hdr = NULL;

	if (args)
		clear_args(args);
	count_mandatory_and_optinal_arguments(args, &m, &o);

	if (ac < m)
	{
		fprintf(stderr, "Missing mandatory arguments; got %d, expected %s%d.\n",
				ac, o > 0 ? "at least " : "", m);
		return -EINVAL;
	}

	if (o >= 0 && ac > m + o)
	{
		fprintf(stderr, "Got excess arguments; got %d, expected %s%d.\n",
				ac, o > 0 ? "at most " : "", m + o);
		return -EINVAL;
	}

	for (i = 0 ; argument_is_valid(args) && i < ac; args++)
	{
		if (args->type == _optargs_arg_div)
			continue;

		if (hdr)
		{
			if (args->type != optargs_arg_group_member)
			{
				if (hdr->type == optargs_arg_group)
					return unmatching_mandatory_argument_error(av[i], hdr->name);
				hdr = NULL;
			}
			else
			{
				if (!strcmp(args->name, av[i]))
				{
					args->result.value = av[i];
					hdr->result.match = args;
					hdr = NULL;
					i++;
				}
				continue;
			}
		}
		else if (args->type == optargs_arg_group_member)
			continue;

		switch (args->type)
		{
			case optargs_arg_group_opt:
				if (hdr)
					return unmatching_optional_argument_error(av[i], hdr->name);
				hdr = args;
				break;
			case optargs_arg_group:
				if (hdr)
					return unmatching_mandatory_argument_error(av[i], hdr->name);
				hdr = args;
				break;
			case optargs_arg_any_opt:
				if (!o)
					break;
				o--;
			case optargs_arg_any:
				args->result.value = av[i];
				i++;
				break;
			case optargs_arg_group_member:
			case _optargs_arg_div:
			case _optargs_arg_eol:
			case _optargs_arg_sink:
				abort();
		}
	}

	if (hdr)
	{
		if (hdr->type == optargs_arg_group)
			return unmatching_mandatory_argument_error(av[i], hdr->name);
		else
		{
			assert(hdr->type == optargs_arg_group_opt);

			if (i < ac)
			{
				assert(args->type == _optargs_arg_sink);
				return unmatching_optional_argument_error(av[i], hdr->name);
			}
		}
	}

	return 0;
}

static void
close_optionality_brackets(int * l)
{
	assert(l);

	for ( ; *l ; --*l)
		printf("]");
}

static void
print_arg_name(struct optargs_arg const * const arg, int * l)
{
	assert(arg);

	switch (arg->type)
	{
		case optargs_arg_group:
		case optargs_arg_any:
			assert(arg->name);
			printf(" %.*s", word_length(arg->name), arg->name);
			break;
		case optargs_arg_group_opt:
		case optargs_arg_any_opt:
			++*l;
			assert(arg->name);
			printf(" [%.*s", word_length(arg->name), arg->name);
			break;
		case optargs_arg_group_member:
		case _optargs_arg_div:
			break;
		case _optargs_arg_eol:
		case _optargs_arg_sink:
			abort();
	}
}

static void
print_arg_names(struct optargs_arg const * args)
{
	assert(args);

	int l = 0;

	for ( ; argument_is_valid(args) ; args++)
	{
		if (argument_is_mandatory(args))
			close_optionality_brackets(&l);
		print_arg_name(args, &l);
	}

	close_optionality_brackets(&l);
}

static int
calc_line_length(char const * const src, int const cols)
{
	assert(src);
	assert(cols > 0);

	int r, t;

	for (r = 0, t = 0 ; r < cols && src[r] != '\n' && src[r] != '\0' ; r++)
	{
		if (is_word_separator(src[r]))
			t = r;
	}

	if (!is_word_separator(src[r]))
	{
		if (t)
			r = t;
		else
			while (!is_word_separator(src[++r]));
	}

	return r;
}

static void
print_wrapped(char const * src, int width, int const indent)
{
	assert(src);
	assert(width > 0);
	assert(*src != '\0');

	bool first = true;

	width -= indent;

	do {
		int l = calc_line_length(src, width);
		printf("%*s%.*s\n", first ? 0 : indent, "", l, src);
		first = false;
		src += l + 1;
	} while (*(src - 1) != '\0');
}

static int
get_argument_name_length(struct optargs_arg const * arg)
{
	assert(arg);

	int i = 0, j;

	for ( ; argument_is_valid(arg) ; arg++)
	{
		if (!arg->name)
			continue;

		j = strlen(arg->name);
		if (i < j)
			i = j;
	}

	return i;
}

static int
get_long_opt_length(struct optargs_opt const * const opt)
{
	assert(opt);

	int r, t;

	if (!opt->long_option)
		return 0;

	r = strlen(opt->long_option);

	if (!opt->argument)
		r += 3;
	else
	{
		t = get_argument_name_length(opt->argument);
		r += t ? t : 3;
	}

	return r;
}


static int
find_longest_opt(struct optargs_opt const * opts)
{
	assert(opts);

	int r, t;

	for (r = 0 ; option_is_valid(opts) ; opts++)
	{
		if (option_is_hidden(opts))
			continue;

		if (option_is_divider(opts))
			continue;

		t = get_long_opt_length(opts);

		if (t > LEFT_COLUMN_MAX_WIDTH)
			continue;

		r = max(r, t);
	}

	return r + 12;
}

static int
get_arg_length(struct optargs_arg const * const arg)
{
	assert(arg);

	return arg->name ? strlen(arg->name) : 0;
}

static int
find_longest_arg(struct optargs_arg const * args)
{
	assert(args);

	int r;

	for (r = 0; argument_is_valid(args) ; args++)
	{
		int t = get_arg_length(args);

		if (t > LEFT_COLUMN_MAX_WIDTH)
			continue;

		r = max(r, t);
	}

	return r + 6;
}

static int
find_left_column_maximum(struct optargs_opt const * const opts,
		struct optargs_arg const * const args)
{
	return min(
		LEFT_COLUMN_MAX_WIDTH,
		max(
			opts ? find_longest_opt(opts) : 0,
			args ? find_longest_arg(args) : 0
		)
	);
}

static int
print_option_options(struct optargs_opt const * const opt)
{
	assert(opt);

	if (opt->short_option && opt->long_option)
	{
		printf("  -%c,--%s", opt->short_option, opt->long_option);
		return 7 + strlen(opt->long_option);
	}

	if (opt->short_option)
	{
		printf("  -%c", opt->short_option);
		return 4;
	}

	assert(opt->long_option);
	printf("  --%s", opt->long_option);
	return strlen(opt->long_option) + 4;
}

static int
print_mandatory_option_argument(struct optargs_opt const * const opt)
{
	assert(opt);
	assert(opt->argument);
	assert(opt->argument->name);

	int z = word_length(opt->argument->name);
	printf(" %.*s", (int)z, opt->argument->name);
	return z + 1;
}

static int
print_optional_option_argument(struct optargs_opt const * const opt)
{
	assert(opt);
	assert(opt->argument);
	assert(opt->argument->name);

	int z = word_length(opt->argument->name);
	printf("[=%.*s]", z, opt->argument->name);
	return z + 3;
}

static int
print_option_argument(struct optargs_opt const * const opt)
{
	assert(opt);

	if (!opt->argument)
		return 0;

	switch (opt->argument->type)
	{
		case optargs_arg_any:
		case optargs_arg_group:
			return print_mandatory_option_argument(opt);
		case optargs_arg_any_opt:
		case optargs_arg_group_opt:
			return print_optional_option_argument(opt);
		case _optargs_arg_eol:
		case _optargs_arg_sink:
		case _optargs_arg_div:
		case optargs_arg_group_member:
			return 0;
	}
	abort();
}

static void
print_option_padding(int const width, int const pos)
{
	if (pos >= width)
		printf("\n%*s", width, "");
	else
		printf("%-*s", width - pos, "");
}

static void
print_argument(struct optargs_arg const * const arg, int const left_column)
{
	assert(arg);
	assert(arg->name || !arg->description);

	if (argument_is_divider(arg))
		printf("\n");
	else if (argument_is_header(arg))
		printf("\n%s:\n", arg->name);
	else
	{
		printf("  %-*s", left_column - 2, arg->name);

		if ((int)strlen(arg->name) + 2 >= left_column)
			printf("\n%*s", left_column, "");

		print_wrapped(arg->description, MAX_WIDTH, left_column);

		if (arg->type == optargs_arg_group_member && argument_is_fixed(arg + 1))
			printf("\n");
	}
}

static void
print_option_argument_desc(struct optargs_arg const * const arg, int left_column)
{
	assert(arg);
	assert(arg->name);
	assert(arg->description);

	if (!arg->description)
		printf("    %s:\n", arg->name);
	else
	{
		printf("    %s:%*s", arg->name, left_column - 4 - (int)strlen(arg->name), "");
		print_wrapped(arg->description, MAX_WIDTH, left_column + 1);
	}

}

static void
print_option_argument_header_item_desc(struct optargs_arg const * const arg, int left_column)
{
	assert(arg);
	assert(arg->name);

	if (!arg->description)
	{
		printf("   %s\n", arg->name);
		return;
	}

	printf("     %s%*s", arg->name, left_column - 5 - (int)strlen(arg->name) + 2, "");
	print_wrapped(arg->description, MAX_WIDTH, left_column + 2);
}

static void
print_option_argument_description(struct optargs_arg const * arg,
		int const left_column)
{
	assert(arg);

	for ( ; argument_is_valid(arg) ; arg++)
	{
		if (arg->type == optargs_arg_group_member)
			print_option_argument_header_item_desc(arg, left_column);
		else if (arg->description)
			print_option_argument_desc(arg, left_column);
	}
}

static void
print_regular_option(struct optargs_opt const * const opt,
		int const left_column)
{
	int len = print_option_options(opt);
	len += print_option_argument(opt);
	print_option_padding(left_column, len);
	print_wrapped(opt->description, MAX_WIDTH, left_column);

	if (opt->argument)
		print_option_argument_description(opt->argument, left_column);
}


static void
print_option(struct optargs_opt const * const opt, int const left_column)
{
	if (option_is_divider(opt))
		printf("\n");
	else if (!option_is_hidden(opt))
		print_regular_option(opt, left_column);
}

static int
count_printable_options(struct optargs_opt const * opts)
{
	assert(opts);

	int rv;

	for (rv = 0 ; option_is_valid(opts) ; opts++)
		if (!option_is_hidden(opts))
			rv++;

	return rv;
}

static void
print_options(struct optargs_opt const * opts, int const indent)
{
	assert(opts);
	assert(indent > 0);

	if (count_printable_options(opts))
		printf("\nOPTIONS:\n");

	for ( ; option_is_valid(opts) ; ++opts)
		print_option(opts, indent);
}

static void
print_arguments(struct optargs_arg const * args, int const indent)
{
	assert(args);
	assert(indent > 0);

	if (!argument_is_header(args))
		printf("\nARGUMENTS:\n");

	for ( ; argument_is_valid(args) ; args++)
		print_argument(args, indent);
}

static void
print_usage(char const * const exe, struct optargs_opt const * const opts,
		struct optargs_arg const * const args)
{
	assert(exe);

	printf("Usage: %s", exe);

	if (opts && count_printable_options(opts))
		printf(" [OPTIONS]");

	if (args)
		print_arg_names(args);

	printf("\n");
}

void
optargs_print_help(char const * const exe, char const * const about,
		struct optargs_opt const * const opts,
		struct optargs_arg const * const args)
{
	assert(exe);

	int const indent = (opts || args) ? find_left_column_maximum(opts, args) : 0;

	print_usage(exe, opts, args);

	if (opts)
		print_options(opts, indent);

	if (args)
		print_arguments(args, indent);

	printf("\n");

	if (about)
		print_wrapped(about, MAX_WIDTH, 0);
}

struct optargs_res const *
optargs_opt_res_by_long(struct optargs_opt const * const opts, char const * const l)
{
	assert(opts);
	assert(l);

	for (int i = 0; option_is_valid(&opts[i]); ++i)
		if (opts[i].long_option == l)
			return optargs_res_type(&opts[i].result) ? &opts[i].result : NULL;
	abort();
}

struct optargs_res const *
optargs_opt_res_by_short(struct optargs_opt const * const opts, char const s)
{
	assert(opts);
	assert(s);

	for (int i = 0; option_is_valid(&opts[i]); ++i)
		if (opts[i].short_option == s)
			return optargs_res_type(&opts[i].result) ? &opts[i].result : NULL;
	abort();
}

struct optargs_res const *
optargs_opt_res_by_index(struct optargs_opt const * const opts, int const i)
{
	assert(opts);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	int j;

	for (j = 0; j <= i; j++)
		assert(option_is_valid(&opts[j]));
#endif
	return optargs_res_type(&opts[i].result) ? &opts[i].result : NULL;
}

static char const *
get_result_string(struct optargs_res const * const r)
{
	if (!r)
		return NULL;

	assert(r->type == optargs_string);

	return r->value.string;
}

static unsigned int
get_result_count(struct optargs_res const * const r)
{
	if (!r)
		return 0;

	assert(r->type == optargs_count);

	return r->value.count;
}

char const *
optargs_opt_value_by_short(struct optargs_opt const * const opts, char const s)
{
	assert(opts);
	assert(s);

	return get_result_string(optargs_opt_res_by_short(opts, s));
}

char const *
optargs_opt_value_by_long(struct optargs_opt const * const opts, char const * const l)
{
	assert(opts);
	assert(l);

	return get_result_string(optargs_opt_res_by_long(opts, l));
}

char const *
optargs_opt_value_by_index(struct optargs_opt const * const opts, int const i)
{
	assert(opts);

	return get_result_string(optargs_opt_res_by_index(opts, i));
}

unsigned int
optargs_opt_count_by_short(struct optargs_opt const * const opts, char const s)
{
	assert(opts);
	assert(s);

	return get_result_count(optargs_opt_res_by_short(opts, s));
}

unsigned int
optargs_opt_count_by_long(struct optargs_opt const * const opts, char const * const l)
{
	assert(opts);
	assert(l);

	return get_result_count(optargs_opt_res_by_long(opts, l));
}

unsigned int
optargs_opt_count_by_index(struct optargs_opt const * const opts, int const i)
{
	assert(opts);

	return get_result_count(optargs_opt_res_by_index(opts, i));
}

enum optargs_res_type
optargs_res_type(struct optargs_res const * const r)
{
	if (!r)
		return optargs_undef;

	return r->type;
}

int
optargs_parse_args_all(int ac, char const * const * av,
		struct optargs_arg ** argsv, int argsvc)
{
	assert(ac >= 0);
	assert(argsv);
	assert(argsvc);

	for (int i = 0 ; i < argsvc ; i++)
	{
		if (!optargs_parse_args(ac, av, argsv[i]))
			return i;
	}

	return -1;
}

char const *
optargs_arg_value(struct optargs_arg const * a)
{
	assert(a);

	if (!a->result.defined)
		return NULL;

	if (argument_is_header(a))
		return a->result.match->result.value;
	return a->result.value;
}

int
optargs_arg_value_index(struct optargs_arg const * a, int i)
{
	assert(a);
	assert(i >= 0);

	int rv = optargs_arg_value_offset(&a[i]);

	if (rv <= 0)
		return rv;

	assert(a->result.defined);
	return i + rv;
}

int
optargs_arg_value_offset(struct optargs_arg const * a)
{
	assert(a);

	if (!argument_is_header(a))
		return -1;

	if (!a->result.defined)
		return 0;

	return ((long)a->result.match - (long)a)/sizeof(struct optargs_arg);
}

