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

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optargs.h"

static int parse_arguments(int const ac, char const * const * const av, struct optargs_argument * args);

enum constants
{
	MAX_WIDTH = 79,
	LEFT_COLUMN_MAX_WIDTH = 38
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
argument_is_eol(struct optargs_argument const * const arg)
{
	return arg->type == _optargs_argument_eol || arg->type == _optargs_argument_sink;
}

static bool
option_is_valid(struct optargs_option const * const o)
{
	assert(o);

	if (o->value_type == optargs_flag)
		return true;
	else if (!o->argument)
		return true;
	else
		return !argument_is_eol(o->argument);
}

static bool
option_supports_argument(struct optargs_option const * o)
{
	assert(o);

	return o->value_type != optargs_flag && o->argument;
}

static int
opt_missing_arg_error(struct optargs_option const * const opt)
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
non_wanted_argument_error(struct optargs_option const * const opt)
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
redefine_option_argument_error(struct optargs_option const * const opt)
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
option_is_dividerider(struct optargs_option const * const opt)
{
	assert(opt);

	return !(opt->short_option || opt->long_option);
}

static bool
option_is_hidden(struct optargs_option const * const opt)
{
	assert(opt);

	return !option_is_dividerider(opt) && !opt->description;
}

static bool
argument_is_header(struct optargs_argument const * const arg)
{
	assert(arg);

	return arg->type == optargs_argument_group || arg->type == optargs_argument_group_opt;
}

static bool
argument_is_dividerider(struct optargs_argument const * const arg)
{
	assert(arg);

	return arg->type == _optargs_argument_divider;
}

static bool
argument_type_is_any(struct optargs_argument const * const arg)
{
	assert(arg);

	return arg->type == optargs_argument_any || arg->type == optargs_argument_any_opt;
}

static struct optargs_option *
locate_short_option(struct optargs_option * o, char const c)
{
	assert(o);

	for ( ; option_is_valid(o) ; o++)
		if (o->short_option == c)
			return o;

	return NULL;
}

static void
locate_long_option(struct optargs_option ** o, char const * l, int const ll)
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
mark_default_option(struct optargs_option * opt)
{
	assert(opt);

	if (opt->value_type == optargs_flag)
		opt->count++;
	else
	{
		opt->value_type = optargs_flag;
		opt->count = 1;
	}
}

static int
mark_no_argument_option(struct optargs_option * opt, bool const assign)
{
	assert(opt);

	if (assign)
		return non_wanted_argument_error(opt);

	mark_default_option(opt);

	return 0;
}

static int
mark_argument_option(struct optargs_option * opt, char const * const arg)
{
	if (opt->value_type)
		return redefine_option_argument_error(opt);

	if (!arg)
		return opt_missing_arg_error(opt);

	opt->value_type = optargs_argument;
	if (parse_arguments(1, &arg, opt->argument) < 0)
		return -1;

	return 1;
}

static int
mark_optional_argument_option(struct optargs_option * opt,
		char const * const arg, bool const assign)
{
	assert(opt);

	if (assign && arg)
		return mark_argument_option(opt, arg);

	if (opt->value_type)
		return redefine_option_argument_error(opt);

	mark_default_option(opt);

	return 0;
}

static int
mark_option(struct optargs_option * opt, char const * const arg, bool const force)
{
	assert(opt);

	if (!option_supports_argument(opt))
		return mark_no_argument_option(opt, force);

	switch (opt->argument->type)
	{
		case optargs_argument_any:
		case optargs_argument_group:
			return mark_argument_option(opt, arg);
		case _optargs_argument_eol:
		case _optargs_argument_sink:
		case _optargs_argument_divider:
		case optargs_argument_any_opt:
		case optargs_argument_group_opt:
			return mark_optional_argument_option(opt, arg, force);
		case optargs_argument_group_member:
			break;
	}
	abort();
}

static int
parse_short_option(char const * c, char const * n, struct optargs_option * const o)
{
	assert(c);
	assert(o);
	assert(starts_with_single_hyphen(c));

	struct optargs_option *t;
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
parse_long_option(char const * c, char const * n, struct optargs_option * o)
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

#if !defined(NDEBUG) || defined(UNIT_TESTING)

/**
 * Ensure all the required fields are filled for any given type of an
 * argument.
 */
static void
assert_argument_required_fields(struct optargs_argument const * const arg)
{
	switch (arg->type)
	{
		case optargs_argument_any:
		case optargs_argument_any_opt:
		case optargs_argument_group_member:
			assert(arg->name);
			assert(arg->description);
			break;
		case optargs_argument_group:
		case optargs_argument_group_opt:
			assert(arg->name);
			break;
		case _optargs_argument_divider:
			assert(!arg->description);
			break;
		default:
			break;
	}
}

/**
 * Ensure all the required fields are filled for any given type of an
 * option argument.
 */
static void
assert_option_argument_required_fields(struct optargs_argument const * const arg)
{
	switch (arg->type)
	{
		case optargs_argument_any:
		case optargs_argument_any_opt:
		case optargs_argument_group_member:
			assert(arg->name);
			break;
		case optargs_argument_group:
		case optargs_argument_group_opt:
			assert(arg->name);
			break;
		case _optargs_argument_divider:
			assert(!arg->description);
			break;
		default:
			break;
	}
}

/**
 * All optinal arguments should be at the and of the expected arguments' list
 */
static void
assert_optional_arguments_at_the_end(struct optargs_argument const * args)
{
	assert(args);

	for (bool f = false ; !argument_is_eol(args) ; args++)
	{
		if (args->type == _optargs_argument_divider
				|| args->type == optargs_argument_group_member)
			continue;

		assert(!f || args->type == optargs_argument_any_opt || args->type == optargs_argument_group_opt);

		if (args->type == optargs_argument_any_opt || args->type == optargs_argument_group_opt)
			f = true;
	}
}

/**
 * Only optargs_argument_group_member type arguments should use subarguments.
 */
static void
assert_subargument_sanity(struct optargs_argument const * args)
{
	assert(args);

	for ( ; !argument_is_eol(args) ; args++)
	{
		if (args->subargument)
		{
			assert(args->type == optargs_argument_group_member);
			assert_subargument_sanity(args->subargument);
		}
	}
}

static void
assert_argument_sanity(struct optargs_argument const * args)
{
	assert(args);

	assert_optional_arguments_at_the_end(args);
	assert_subargument_sanity(args);

	for ( ; !argument_is_eol(args) ; args++)
		assert_argument_required_fields(args);
}

static void
assert_option_sanity(struct optargs_option const * opts)
{
	assert(opts);

	for ( ; option_is_valid(opts) ; opts++)
	{
		if (option_supports_argument(opts))
		{
			struct optargs_argument const * arg = opts->argument;
			assert(arg);

			assert_option_argument_required_fields(arg);
			assert_optional_arguments_at_the_end(arg);

			for ( ; !argument_is_eol(arg) ; arg++ )
				assert(!arg->subargument);
			assert(arg->type != _optargs_argument_sink);
		}
	}
}

static void
assert_at_least_n_options(struct optargs_option const * opts, unsigned int i)
{
	assert(opts);

	unsigned int t;

	for ( t = 0 ;  option_is_valid(opts) ; opts++, t++);

	assert(t >= i);
}

#else

# define assert_at_least_n_options(...)
# define assert_optional_arguments_at_the_end(...)
# define assert_argument_sanity(...)
# define assert_option_sanity(...)

#endif

int
optargs_parse_options(int const ac, char const * const * const av, struct optargs_option * const opts)
{
	assert(ac > 0);
	assert(av);
	assert(opts);
	assert_option_sanity(opts);

	int i, j;
	char const *o, *n;

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

static int
argument_name_compare(const char * a, const char * b)
{
	assert(a);
	assert(b);

	int al = word_length(a),
	    bl = word_length(b);

	if (al > bl)
		return -1;
	else if (bl > al)
		return 1;
	else
		return strncmp(a, b, min(al, bl));
}

static int
invalid_argument_for_group(char const * a, char const * g)
{
	assert(a);
	assert(g);

	fprintf(stderr, "Invalid argument '%s' for '%s'.\n", a, g);

	return -EINVAL;
}

static int
missing_argument_error(char const * a)
{
	assert(a);

	fprintf(stderr, "Missing an argument for '%s'.\n", a);

	return -EINVAL;
}

static int
check_for_missing_mandatory_arguments(struct optargs_argument const * args)
{
	for ( ; !argument_is_eol(args) ; args++)
	{
		if (args->type == optargs_argument_group
				|| args->type == optargs_argument_any)
			return missing_argument_error(args->name);
	}

	return 0;
}

static int
excess_arguments_error(int g, int e)
{
	assert(g > 0);
	assert(e >= 0);

	fprintf(stderr, "Too many arguments; got %d and expected at most %d.\n", g, e);

	return -EINVAL;
}


static int
parse_arguments(int const ac, char const * const * const av, struct optargs_argument * args)
{
	assert(ac >= 0);
	assert(!ac || av);
	assert(args);

	struct optargs_argument * grp = NULL;
	int i, t;

	for (i = 0 ; !argument_is_eol(args) && i < ac ; args++)
	{
		if (args->type == _optargs_argument_divider)
		{}
		else if (grp)
		{
			if (args->type != optargs_argument_group_member)
			{
				if (grp->type == optargs_argument_group)
					return unmatching_mandatory_argument_error(av[i], grp->name);
				else
					return unmatching_optional_argument_error(av[i], grp->name);
			}

			if (!argument_name_compare(args->name, av[i]))
			{
				args->result.value = av[i];
				grp->result.match = args;
				grp = NULL;
				i++;
				if (args->subargument)
				{
					t = parse_arguments(ac - i, av + i, args->subargument);

					if (t < 0)
						return t;
					else
						i += t;
				}
			}
		}
		else if (args->type == optargs_argument_group_member)
		{}
		else if (args->type == optargs_argument_group
				|| args->type == optargs_argument_group_opt)
			grp = args;
		else
		{
			assert(args->type == optargs_argument_any || args->type == optargs_argument_any_opt);
			args->result.value = av[i];
			i++;
		}
	}

	if (grp)
	{
		assert(i < ac);
		return invalid_argument_for_group(av[i], grp->name);
	}

	if (i < ac && args->type != _optargs_argument_sink)
		return excess_arguments_error(ac, i);

	t = check_for_missing_mandatory_arguments(args);

	return t < 0 ? t : i;
}
int
optargs_parse_arguments(int const ac, char const * const * const av, struct optargs_argument * args)
{
	assert(ac >= 0);
	assert(!ac || av);
	assert(args);
	assert_argument_sanity(args);

	return parse_arguments(ac, av, args);
}

static void
close_optionality_brackets(int * l)
{
	assert(l);

	for ( ; *l ; --*l)
		printf("]");
}

static int print_subargument(struct optargs_argument const * args, int * l);

static int
print_group_names(struct optargs_argument const * args)
{
	assert(args);

	int rv, l;

	for ( rv = 0, l = 0 ; args->type == optargs_argument_group_member ; args++)
	{
		printf("%s %s", !rv ? "" : " |", args->name);
		rv += strlen(args->name) + (!rv ? 1 : 3);
		if (args->subargument)
			print_subargument(args->subargument, &l);
	}

	return rv;
}

static int
print_arg_name(struct optargs_argument const * const arg, int * l)
{
	assert(arg);

	int rv = 0;

	switch (arg->type)
	{
		case optargs_argument_group:
			if (!arg->name)
			{
				printf(" {");
				rv += print_group_names(arg + 1);
				printf(" }");
				rv += 4;
				break;
			}
			// fall through
		case optargs_argument_any:
			assert(arg->name);
			printf(" %.*s", word_length(arg->name), arg->name);
			rv = strlen(arg->name) + 1;
			break;
		case optargs_argument_group_opt:
			if (!arg->name)
			{
				printf(" [");
				rv += print_group_names(arg + 1);
				printf(" ]");
				rv += 4;
				break;
			}
			// fall through
		case optargs_argument_any_opt:
			++*l;
			assert(arg->name);
			printf(" [%.*s", word_length(arg->name), arg->name);
			rv = strlen(arg->name) + 3;
			break;
		case optargs_argument_group_member:
		case _optargs_argument_divider:
			break;
		case _optargs_argument_eol:
		case _optargs_argument_sink:
			abort();
	}

	return rv;
}

static void
print_arg_names(struct optargs_argument const * args)
{
	assert(args);

	int l = 0;

	for ( ; !argument_is_eol(args) ; args++)
		print_arg_name(args, &l);

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
get_option_argument_length(struct optargs_argument const * arg)
{
	assert(arg);

	int i = 0, j, e;

	if (arg->type == optargs_argument_any_opt || arg->type == optargs_argument_group_opt)
		e = 2;
	else
		e = 0;

	for ( ; !argument_is_eol(arg) ; arg++)
	{
		if (!arg->name)
			continue;

		j = strlen(arg->name);

		if (i < j)
			i = j;
	}

	return i + e + 1;
}

static int
get_opt_length(struct optargs_option const * const opt)
{
	assert(opt);

	int r = 0;

	if (opt->long_option)
	{
		r += strlen(opt->long_option) + 2;

		if (opt->short_option)
			r += 3;
	}
	else if (opt->short_option)
		r += 2;

	if (option_supports_argument(opt))
		r += get_option_argument_length(opt->argument);

	return r;
}


static int
find_longest_opt(struct optargs_option const * opts)
{
	assert(opts);

	int r, t;

	for (r = 0 ; option_is_valid(opts) ; opts++)
	{
		if (option_is_hidden(opts))
			continue;

		if (option_is_dividerider(opts))
			continue;

		t = get_opt_length(opts);

		if (t > LEFT_COLUMN_MAX_WIDTH)
			continue;

		r = max(r, t);
	}

	return r;
}

static int
get_arg_length(struct optargs_argument const * const arg)
{
	assert(arg);

	int l = arg->name ? strlen(arg->name) : 0;

	/* Headers will get a ':' character appended to them. */
	if (arg->type == optargs_argument_group
			|| arg->type == optargs_argument_group_opt)
		++l;

	return l;
}

static int
find_longest_arg(struct optargs_argument const * args)
{
	assert(args);

	int r;

	for (r = 0; !argument_is_eol(args) ; args++)
	{
		int t = get_arg_length(args);

		if (args->subargument)
			t += 1 + find_longest_arg(args->subargument);

		if (t > LEFT_COLUMN_MAX_WIDTH)
			continue;

		r = max(r, t);
	}

	return r;
}

static int
find_left_column_maximum(struct optargs_option const * const opts,
		struct optargs_argument const * const args)
{
	return min(
		LEFT_COLUMN_MAX_WIDTH,
		max(
			opts ? find_longest_opt(opts) : 0,
			args ? find_longest_arg(args) : 0
		) + 3
	);
}

static int
print_option_options(struct optargs_option const * const opt)
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
print_mandatory_option_argument(struct optargs_option const * const opt)
{
	assert(opt);
	assert(opt->argument);
	assert(opt->argument->name);

	int z = word_length(opt->argument->name);
	printf(" %.*s", (int)z, opt->argument->name);
	return z + 1;
}

static int
print_optional_option_argument(struct optargs_option const * const opt)
{
	assert(opt);
	assert(opt->argument);
	assert(opt->argument->name);

	int z = word_length(opt->argument->name);
	printf("[=%.*s]", z, opt->argument->name);
	return z + 3;
}

static int
print_option_argument(struct optargs_option const * const opt)
{
	assert(opt);

	if (!option_supports_argument(opt))
		return 0;

	switch (opt->argument->type)
	{
		case optargs_argument_any:
		case optargs_argument_group:
			return print_mandatory_option_argument(opt);
		case optargs_argument_any_opt:
		case optargs_argument_group_opt:
			return print_optional_option_argument(opt);
		case _optargs_argument_eol:
		case _optargs_argument_sink:
		case _optargs_argument_divider:
		case optargs_argument_group_member:
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
print_option_argument_desc(struct optargs_argument const * const arg, int left_column)
{
	assert(arg);
	assert(arg->name);
	assert(arg->description);

	int name_length = strlen(arg->name),
	    right_indent = 1;

	printf("    %s", arg->name);

	if (4 + name_length >= left_column)
		printf("\n%*s", left_column + right_indent, "");
	else
		printf("%*s", left_column - 4 - name_length + right_indent, "");

	print_wrapped(arg->description, MAX_WIDTH, left_column + right_indent);
}

static void
print_option_argument_header_item_desc(struct optargs_argument const * const arg, int left_column)
{
	assert(arg);
	assert(arg->name);
	assert(arg->description);

	int name_length = strlen(arg->name),
	    right_indent = 2;

	printf("      %s", arg->name);

	if (5 + name_length >= left_column)
		printf("\n%*s", left_column + right_indent, "");
	else
		printf("%*s", left_column - 5 - name_length + right_indent, "");

	print_wrapped(arg->description, MAX_WIDTH, left_column);
}

static void
print_option_argument_description(struct optargs_argument const * arg,
		int const left_column)
{
	assert(arg);

	for ( ; !argument_is_eol(arg) ; arg++)
	{
		if (arg->type == optargs_argument_group_member)
			print_option_argument_header_item_desc(arg, left_column);
		else if (arg->description)
			print_option_argument_desc(arg, left_column);
	}
}

static void
print_regular_option(struct optargs_option const * const opt,
		int const left_column)
{
	int len = print_option_options(opt);
	len += print_option_argument(opt);
	print_option_padding(left_column, len);
	print_wrapped(opt->description, MAX_WIDTH, left_column);

	if (option_supports_argument(opt))
		print_option_argument_description(opt->argument, left_column);
}


static void
print_option(struct optargs_option const * const opt, int const left_column)
{
	if (option_is_dividerider(opt))
		printf("\n");
	else if (!option_is_hidden(opt))
		print_regular_option(opt, left_column + 2);
}

static int
count_printable_options(struct optargs_option const * opts)
{
	assert(opts);

	int rv;

	for (rv = 0 ; option_is_valid(opts) ; opts++)
		if (!option_is_hidden(opts))
			rv++;

	return rv;
}

static void
print_all_options(struct optargs_option const * opts, int const indent)
{
	assert(opts);
	assert(indent > 0);

	if (count_printable_options(opts))
		printf("\nOPTIONS:\n");

	for ( ; option_is_valid(opts) ; ++opts)
		print_option(opts, indent);
}

static int
print_subargument(struct optargs_argument const * args, int * l)
{
	assert(args);
	assert(l);

	int rv = 0;

	for ( ; !argument_is_eol(args) ; args++)
	{
		rv += print_arg_name(args, l);
		if (args->subargument)
			rv += print_subargument(args->subargument, l);
	}

	close_optionality_brackets(l);

	return rv;
}

static void
print_normal_argument(struct optargs_argument const * const arg, int const left_column, int const indent)
{
	assert(arg);
	assert(left_column > 0);
	assert(indent >= 2);

	int length = indent + strlen(arg->name),
		tmp = 0;

	printf("%*s%s", indent, "", arg->name);

	if (arg->subargument)
		length += print_subargument(arg->subargument, &tmp);

	if (length >= left_column + indent)
		printf("\n%*s", left_column + indent, "");
	else
		printf("%*s", left_column - length + indent, "");

	print_wrapped(arg->description, MAX_WIDTH, left_column + indent);

	if (arg->type == optargs_argument_group_member && argument_type_is_any(arg + 1))
		printf("\n");
}

static void
print_argument_header(struct optargs_argument const * const arg, int const left_column, int const indent)
{
	assert(arg);
	assert(left_column > 0);
	assert(indent >= 0);

	int name_length = strlen(arg->name),
	    right_indent = indent ? indent : 1;

	if (!arg->description)
		printf("%*s%s:\n", indent, "", arg->name);
	else
	{
		printf("%*s%s:", indent, "", arg->name);

		if (indent + name_length + 1 >= left_column + right_indent)
			printf("\n%*s", left_column + right_indent, "");
		else
			printf("%*s", left_column - indent - name_length - 1 + right_indent, "");
		print_wrapped(arg->description, MAX_WIDTH, left_column + right_indent);
	}
}

static void
print_all_arguments(struct optargs_argument const * args, int const left_column, int const indent)
{
	assert(args);
	assert(left_column > 0);

	int extra_indent = 0;

	if (!indent && !argument_is_header(args))
	{
		printf("\nARGUMENTS:\n");
		extra_indent = 2;
	}

	for ( ; !argument_is_eol(args) ; args++)
	{
		if (argument_is_dividerider(args))
			printf("\n");

		if (!args->name)
			continue;

		if (argument_is_header(args))
		{
			if (indent)
				extra_indent = 1;
			else
			{
				extra_indent = 2;
				printf("\n");
			}
			print_argument_header(args, left_column, indent);
		}
		else if (args->description)
			print_normal_argument(args, left_column, indent +extra_indent );

		if (args->subargument)
			print_all_arguments(args->subargument, left_column, indent + extra_indent + 1);
	}
}

static void
print_usage(char const * const exe, struct optargs_option const * const opts,
		struct optargs_argument const * const args)
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
		struct optargs_option const * const opts,
		struct optargs_argument const * const args)
{
	assert(exe);

	int const left_column = (opts || args) ? find_left_column_maximum(opts, args) : 0;

	print_usage(exe, opts, args);

	if (opts)
		print_all_options(opts, left_column);

	if (args)
		print_all_arguments(args, left_column, 0);

	printf("\n");

	if (about)
		print_wrapped(about, MAX_WIDTH, 0);
}

char const *
optargs_option_string(struct optargs_option const * const opts, int const i)
{
	assert(opts);
	assert_at_least_n_options(opts, i);
	assert(opts[i].value_type != optargs_flag);

	if (optargs_option_type(opts, i) != optargs_argument)
	{
		assert(optargs_option_type(opts, i) == optargs_undef);
		return NULL;
	}

	return optargs_argument_value(opts[i].argument);
}

unsigned int
optargs_option_count(struct optargs_option const * const opts, int const i)
{
	assert(opts);
	assert_at_least_n_options(opts, i);
	assert(opts[i].value_type != optargs_argument);

	if (optargs_option_type(opts, i) == optargs_undef)
		return 0;

	return opts[i].count;
}

enum optargs_option_value_type
optargs_option_type(struct optargs_option const * const opts, int const i)
{
	assert(opts);
	assert_at_least_n_options(opts, i);

	return opts[i].value_type;
}

char const *
optargs_argument_value(struct optargs_argument const * a)
{
	assert(a);

	if (!a->result.defined)
		return NULL;

	if (argument_is_header(a))
		return a->result.match->result.value;
	return a->result.value;
}

int
optargs_argument_index(struct optargs_argument const * a, int i)
{
	assert(a);
	assert(i >= 0);

	int rv = optargs_argument_offset(&a[i]);

	if (rv < 0)
		return -1;
	else
		return i + rv;
}

int
optargs_argument_offset(struct optargs_argument const * a)
{
	assert(a);

	assert(argument_is_header(a));

	if (!a->result.defined)
		return -1;

	return ((long)a->result.match - (long)a)/sizeof(struct optargs_argument);
}
