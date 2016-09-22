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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "optargs.h"

#define DEFAULT_VALUE_ADDRESS_MASK (((unsigned char *)-1) - 0xffff)

enum constants
{
	MAX_WIDTH = 79,
	LEFT_COLUMN_MAX_WIDTH = 40
};

const char default_value[2];

static int
max(int a, int b)
{
	return a > b ? a : b;
}

static int
min(int a, int b)
{
	return a > b ? b : a;
}

static int
word_length(const char *s)
{
	assert(s);

	return strcspn(s, " =\t\v\n\r\0");
}

static bool
is_word_separator(char c)
{
	return c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r' || c == '\0';
}

static bool
argument_is_valid(const struct optargs_arg *o)
{
	assert(o);

	return o->mandatory != _optargs_eol;
}

static bool
option_is_valid(const struct optargs_opt *o)
{
	assert(o);

	return argument_is_valid(&o->argument);
}

static int
opt_missing_arg_error(const struct optargs_opt *opt)
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
non_wanted_argument_error(const struct optargs_opt *opt)
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
redefine_option_argument_error(const struct optargs_opt *opt)
{
	assert(opt);

	char buf[5] = "'-";

	fprintf(stderr, "Conflicting arguments given for the option %s%s%s%s%s.\n",
			opt->long_option ? "'--" : "", opt->long_option ? opt->long_option : "", opt->long_option ? "'" : "",
			opt->long_option && opt->short_option ? " / " : "",
			opt->short_option ? strncat(strncat(buf, &opt->short_option, 1), "'", 1) : "");

	return -EINVAL;
}


static bool
starts_with_single_hyphen(const char *str)
{
	assert(str);

	return str[0] == '-';
}

static bool
starts_with_double_hyphen(const char *opt)
{
	assert(opt);
	assert(starts_with_single_hyphen(opt));

	return opt[1] == '-';
}

static bool
is_empty_argument(const char *str)
{
	assert(str);

	return *str == '\0';
}

static bool
is_double_hyphen(const char *opt)
{
	assert(opt);

	return !strcmp(opt, "--");
}

static int
long_option_is_invalid(const char *o, int l)
{
	assert(o);
	assert(l > 0);

	fprintf(stderr, "Unrecognized long option '%.*s'.\n", (int)l, o);

	return -EINVAL;
}

static int
short_option_is_invalid(const char c)
{
	fprintf(stderr, "Unrecognized short option '%c'.\n", c);

	return -EINVAL;
}

static bool
option_is_divider(const struct optargs_opt *opt)
{
	assert(opt);

	return !(opt->short_option || opt->long_option);
}

static bool
option_is_hidden(const struct optargs_opt *opt)
{
	assert(opt);

	return !option_is_divider(opt) && !opt->description;
}

static bool
argument_is_header(const struct optargs_arg *arg)
{
	assert(arg);
	assert(arg->name);

	return !arg->description && arg->name;
}

static bool
argument_is_divider(const struct optargs_arg *arg)
{
	assert(arg);

	return !(arg->description || arg->name);
}

static struct optargs_opt *
locate_short_option(struct optargs_opt *o, const char c)
{
	assert(o);

	for ( ; option_is_valid(o) ; o++)
		if (o->short_option == c)
			return o;

	return NULL;
}

static void
locate_long_option(struct optargs_opt **o, const char *l, int ll)
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

static int
mark_default_option(struct optargs_opt *opt)
{
	assert(opt);

	opt->result = (const char *)
		(((size_t)DEFAULT_VALUE_ADDRESS_MASK) + 1 + optargs_is_default(opt->result));

	return 0;
}

static int
mark_no_argument_option(struct optargs_opt *opt, bool assign)
{
	assert(opt);

	if (assign)
		return non_wanted_argument_error(opt);

	return mark_default_option(opt);
}

static int
mark_argument_option(struct optargs_opt *opt, const char *arg)
{
	if (opt->result)
		return redefine_option_argument_error(opt);

	if (!arg)
		return opt_missing_arg_error(opt);

	opt->result = arg;

	return 1;
}

static int
mark_optional_argument_option(struct optargs_opt *opt, const char *arg, bool assign)
{
	assert(opt);

	if (assign && arg)
		return mark_argument_option(opt, arg);

	if (opt->result && !optargs_is_default(opt->result))
		return redefine_option_argument_error(opt);

	return mark_default_option(opt);
}

static int
mark_option(struct optargs_opt *opt, const char *arg, bool force)
{
	assert(opt);

	int rv = 0;

	switch (opt->argument.mandatory)
	{
		case optargs_no:
			rv = mark_no_argument_option(opt, force);
			break;
		case optargs_yes:
			rv = mark_argument_option(opt, arg);
			break;
		case optargs_maybe:
			rv = mark_optional_argument_option(opt, arg, force);
			break;
		default:
			abort();
	}

	return rv;
}

static int
parse_short_option(const char *c, const char *n, struct optargs_opt *o)
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
		r = 0;
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
parse_long_option(const char *c, const char *n, struct optargs_opt *o)
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

int
optargs_parse(int ac, const char **av, struct optargs_opt *opts)
{
	assert(ac > 0);
	assert(av);
	assert(opts);

	int i, j;
	const char *o, *n;

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

	return i == ac ? 0 : i;
}

static void
print_arg_name(const struct optargs_arg *arg)
{
	assert(arg);

	if (arg->name && arg->mandatory != optargs_no)
	{
		int l = word_length(arg->name);

		if (arg->mandatory == optargs_yes)
			printf(" %.*s", l, arg->name);
		else
		{
			assert(arg->mandatory == optargs_maybe);
			printf(" [%.*s]", l, arg->name);
		}
	}
}

static void
print_arg_names(const struct optargs_arg *args)
{
	assert(args);

	for ( ; argument_is_valid(args) ; args++)
		print_arg_name(args);
}

static int 
calc_line_length(const char *src, int cols)
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
print_wrapped(const char *src, int width, int indent)
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
get_long_opt_length(const struct optargs_opt *opt)
{
	assert(opt);

	int r;

	if (!opt->long_option)
		return 0;

	r = strlen(opt->long_option);

	if (opt->argument.name)
		r += strlen(opt->argument.name);
	else
		r += 3;

	return r;
}


static int
find_longest_opt(const struct optargs_opt *opts)
{
	assert(opts);

	int r, t;

	for (r = 0 ; option_is_valid(opts) ; opts++)
	{
		if (option_is_hidden(opts))
			continue;

		t = get_long_opt_length(opts);

		if (t > LEFT_COLUMN_MAX_WIDTH)
			continue;

		r = max(r, t);
	}

	return r + 12;
}

static int
get_arg_length(const struct optargs_arg *arg)
{
	assert(arg);

	return arg->name ? strlen(arg->name) : 0;
}

static int
find_longest_arg(const struct optargs_arg *args)
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
find_left_column_maximum(const struct optargs_opt *opts,
		const struct optargs_arg *args)
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
print_option_options(const struct optargs_opt *opt)
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
print_mandatory_option_argument(const struct optargs_opt *opt)
{
	assert(opt);

	int z;

	if (opt->argument.name)
	{
		z = word_length(opt->argument.name);
		printf(" %.*s", (int)z, opt->argument.name);
		z += 1;
	}
	else
	{
		printf(" ARG");
		z = 4;
	}

	return z;
}

static int
print_optional_option_argument(const struct optargs_opt *opt)
{
	assert(opt);

	int z;

	if (opt->argument.name)
	{
		z = word_length(opt->argument.name);
		printf(" [%.*s]", z, opt->argument.name);
		z += 3;
	}
	else
	{
		printf(" [ARG]");
		z = 6;
	}

	return z;
}

static int
print_option_argument(const struct optargs_opt *opt)
{
	assert(opt);

	if (opt->argument.mandatory == optargs_yes)
		return print_mandatory_option_argument(opt);
	else if (opt->argument.mandatory == optargs_maybe)
		return print_optional_option_argument(opt);
	else
		return 0;
}

static void
print_option_padding(int width, int pos)
{
	if (pos >= width)
		printf("\n%*s", width, "");
	else
		printf("%-*s", width - pos, "");
}

static void
print_argument(const struct optargs_arg *arg, int left_column)
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
	}
}

static void
print_option_argument_description(const struct optargs_arg *arg, int left_column)
{
	char buf[MAX_WIDTH];
	int l = left_column + strlen(arg->name) + 1, o, d = strlen(arg->description);

	printf("%*s%s:%s", left_column, "", arg->name, l < MAX_WIDTH ? " " : "");

	if (l >= MAX_WIDTH)
	{
		printf("\n%*s", left_column, "");
		print_wrapped(arg->description, MAX_WIDTH, left_column);
	}
	else if (l + d <= MAX_WIDTH)
		printf("%s\n", arg->description);
	else
	{
		o = MAX_WIDTH - l;
		strncpy(buf, arg->description, o);

		for (--o ; o >= 0 ; o--)
			if (is_word_separator(buf[o]))
				break;

		if (o > 0)
		{
			buf[o] = '\0';
			print_wrapped(buf, MAX_WIDTH, l);
			++o;
		}

		printf("%*s", left_column, "");
		print_wrapped(arg->description + o, MAX_WIDTH, left_column);
	}
}

static void
print_regular_option(const struct optargs_opt *opt, int left_column)
{
	int len = print_option_options(opt);
	len += print_option_argument(opt);
	print_option_padding(left_column, len);
	print_wrapped(opt->description, MAX_WIDTH, left_column);

	if (opt->argument.description)
		print_option_argument_description(&opt->argument, left_column);
}


static void
print_option(const struct optargs_opt *opt, int left_column)
{
	if (option_is_divider(opt))
		printf("\n");
	else if (!option_is_hidden(opt))
		print_regular_option(opt, left_column);
}

static void
print_options(const struct optargs_opt *opts, int indent)
{
	assert(opts);
	assert(indent > 0);

	printf("\nOPTIONS:\n");

	for ( ; option_is_valid(opts) ; ++opts)
		print_option(opts, indent);
}

static bool
all_args_without_description(const struct optargs_arg *args)
{
	assert(args);

	for ( ; argument_is_valid(args) ; args++)
		if (args->description)
			return false;

	return true;
}

static void
print_arguments(const struct optargs_arg *args, int indent)
{
	assert(args);
	assert(indent > 0);

	if (all_args_without_description(args))
		return;

	if (args->description && args->description[0] != '\0')
		printf("\nARGUMENTS:\n");

	for ( ; argument_is_valid(args) ; args++)
		print_argument(args, indent);
}

static void
print_usage(const char *exe, const struct optargs_opt *opts, const struct optargs_arg *args)
{
	assert(exe);

	printf("Usage: %s", exe);

	if (opts)
		printf(" [OPTIONS]");

	if (args)
		print_arg_names(args);

	printf("\n");
}

void
optargs_print_help(const char *exe, const char *about,
		const struct optargs_opt *opts, const struct optargs_arg *args)
{
	assert(exe);

	const int indent = (opts || args) ? find_left_column_maximum(opts, args) : 0;

	print_usage(exe, opts, args);

	if (opts)
		print_options(opts, indent);

	if (args)
		print_arguments(args, indent);

	printf("\n");

	if (about)
		print_wrapped(about, MAX_WIDTH, 0);
}

int
optargs_arg_index(const struct optargs_arg *args, const char *name)
{
	assert(args);
	assert(name);

	for (int r = 0, l ; argument_is_valid(&args[r]) ; r++)
	{
		if (argument_is_header(&args[r]))
			continue;

		if ((l = word_length(args[r].name)) != word_length(name))
			continue;

		if (!strncmp(args[r].name, name, l))
			return r;
	}

	return -EINVAL;
}

const char *
optargs_opt_by_long(const struct optargs_opt *opts, const char *l)
{
	for (int i = 0; option_is_valid(&opts[i]); ++i)
		if (opts[i].long_option == l)
			return opts[i].result;
	abort();
}

const char *
optargs_opt_by_short(const struct optargs_opt *opts, const char s)
{
	for (int i = 0; option_is_valid(&opts[i]); ++i)
		if (opts[i].short_option == s)
			return opts[i].result;
	abort();
}

const char *
optargs_opt_by_index(const struct optargs_opt *opts, int i)
{
#ifndef NDEBUG
	int j;

	for (j = 0; j <= i; j++)
		assert(option_is_valid(&opts[j]));
#endif
	return opts[i].result;
}

int
optargs_is_default(const char *v)
{
	if (((size_t)DEFAULT_VALUE_ADDRESS_MASK & (size_t)v) == (size_t)DEFAULT_VALUE_ADDRESS_MASK)
		return (size_t)v & ~(size_t)DEFAULT_VALUE_ADDRESS_MASK;
	else
		return 0;
}
