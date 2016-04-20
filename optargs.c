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

#define MAX_WIDTH 79
#define LEFT_COLUMN_MAX_WIDTH 40

const char optargs_default_result = 'x';

static size_t
max(int a, int b)
{
	return a > b ? a : b;
}

static size_t
min(int a, int b)
{
	return a > b ? b : a;
}

static size_t
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

	return o->mandatory;
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
long_option_is_invalid(const char *o, size_t l)
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
option_is_hidden(const struct optargs_opt *opt)
{
	assert(opt);

	return !opt->description;
}
static bool
option_is_divider(const struct optargs_opt *opt)
{
	assert(opt);
	assert(option_is_hidden(opt));

	return !(opt->short_option || opt->long_option);
}

static bool
argument_is_header(const struct optargs_arg *arg)
{
	assert(arg);
	assert(arg->name);

	return !arg->description;
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
locate_long_option(struct optargs_opt **o, const char *l, size_t ll)
{
	assert(o);
	assert(*o);
	assert(l);
	assert(ll > 0);

	for ( ; option_is_valid(*o) ; (*o)++)
	{
		if (!(*o)->long_option)
			continue;

		if (strlen((*o)->long_option) != ll)
			continue;

		if ((*o)->long_option && !strncmp((*o)->long_option, l, ll))
			break;
	}
}

static int
mark_option(struct optargs_opt *opt, const char *arg, bool force)
{
	assert(opt);

	int rv = 0;

	switch (opt->argument.mandatory)
	{
		case optargs_no:
			if (force)
				rv = non_wanted_argument_error(opt);
			else
				opt->result = &optargs_default_result;
			break;
		case optargs_yes:
			if (!arg)
				rv = opt_missing_arg_error(opt);
			else
			{
				opt->result = arg;
				rv = 1;
			}
			break;
		case optargs_maybe:
			if (force && arg)
			{
				opt->result = arg;
				rv = 1;
			}
			else
				opt->result = &optargs_default_result;
			break;
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

	size_t d;
	int r;

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

	int l;

	if (arg->name && arg->mandatory != optargs_no)
	{
		l = word_length(arg->name);

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

static size_t
calc_line_length(const char *src, size_t cols)
{
	assert(src);
	assert(cols > 0);

	size_t r, t;

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
print_wrapped(const char *src, size_t width, size_t indent)
{
	assert(src);
	assert(width > 0);
	assert(*src != '\0');

	size_t l;
	bool first = true;

	width -= indent;

	do {
		l = calc_line_length(src, width);
		printf("%*s%.*s\n", first ? 0 : (int)indent, "", (int)l, src);
		first = false;
		src += l + 1;
	} while (*(src - 1) != '\0');
}

static size_t
get_long_opt_length(const struct optargs_opt *opt)
{
	assert(opt);

	size_t r;

	if (!opt->long_option)
		return 0;

	r = strlen(opt->long_option);

	if (opt->argument.name)
		r += strlen(opt->argument.name);
	else
		r += 3;

	return r;
}


static size_t
find_longest_opt(const struct optargs_opt *opts)
{
	assert(opts);

	size_t r;

	for (r = 0 ; option_is_valid(opts) ; opts++)
		r = max(r, get_long_opt_length(opts));

	return r + 12;
}

static size_t
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
		r = max(r, get_arg_length(args));

	return r + 6;
}

static size_t
find_left_column_maximum(const struct optargs_opt *opts,
		const struct optargs_arg *args)
{
	size_t m = max(opts ? find_longest_opt(opts) : 0, args ? find_longest_arg(args) : 0);
	return min(m, LEFT_COLUMN_MAX_WIDTH);
}

static void
print_option_argument_description(const struct optargs_opt *opt, size_t left_column)
{
	assert(opt);

	if (opt->argument.description)
	{
		/** An option with description must have a name as well. */
		assert(opt->argument.name);

		/** Doesn't make any sense to describe arguments that can not be given. */
		assert(opt->argument.mandatory == optargs_yes
				|| opt->argument.mandatory == optargs_maybe);

		if (strlen(opt->argument.name) >= left_column)
			printf("  %-*s\n%*s", (int)left_column - 2, opt->argument.name, (int)left_column, "");
		else
			printf("  %-*s", (int)left_column - 2, opt->argument.name);

		print_wrapped(opt->argument.description, MAX_WIDTH, left_column);

	}
}

static void
print_option_argument_descriptions(const struct optargs_opt *opts, size_t width)
{

	assert(opts);
	assert(width > 0);

	/** No need to pass the options without argument descriptions. */
	for ( ; option_is_valid(opts) && !opts->argument.description; opts++);

	if (option_is_valid(opts))
	{
		printf("\nOPTION ARGUMENTS:\n");

		for ( ; option_is_valid(opts) ; opts++)
			print_option_argument_description(opts, width);
	}
}

static size_t
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

static size_t
print_mandatory_option_argument(const struct optargs_opt *opt)
{
	assert(opt);

	size_t z;

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

static size_t
print_optional_option_argument(const struct optargs_opt *opt)
{
	assert(opt);

	size_t z;

	if (opt->argument.name)
	{
		z = word_length(opt->argument.name);
		printf(" [%.*s]", (int)z, opt->argument.name);
		z += 3;
	}
	else
	{
		printf(" [ARG]");
		z = 6;
	}

	return z;
}

static size_t
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
print_option_padding(size_t width, size_t pos)
{

	if (pos >= (size_t)width)
		printf("\n%*s", (int)width, "");
	else
		printf("%-*s", (int)(width - pos), "");

}

static void
print_option(const struct optargs_opt *opt, size_t left_column)
{
	size_t len;

	if (option_is_hidden(opt))
	{
		if (option_is_divider(opt))
			printf("\n");
		return;
	}

	len = print_option_options(opt);
	len += print_option_argument(opt);
	print_option_padding(left_column, len);
	print_wrapped(opt->description, MAX_WIDTH, left_column);
}

static void
print_options(const struct optargs_opt *opts, int indent)
{
	assert(opts);
	assert(indent > 0);

	printf("\nOPTIONS:\n");

	for (; option_is_valid(opts); ++opts)
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
print_argument(const struct optargs_arg *arg, size_t left_column)
{
	assert(arg);

	if (!arg->description)
	{
		if (arg->name)
			printf("\n%s:\n", arg->name);
		else
			printf("\n");
	}
	else
	{
		assert(arg->name);

		if (strlen(arg->name) + 2 >= (size_t)left_column)
			printf("  %-*s\n%*s", (int)left_column - 2, arg->name, (int)left_column, "");
		else
			printf("  %-*s", (int)left_column - 2, arg->name);

		print_wrapped(arg->description, MAX_WIDTH, left_column);
	}
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
	{
		print_options(opts, indent);
		print_option_argument_descriptions(opts, indent);
	}

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

	int r;
	size_t l;

	for (r = 0; argument_is_valid(&args[r]); r++)
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
	int i;

	for (i = 0; option_is_valid(&opts[i]); ++i)
		if (opts[i].long_option == l)
			return opts[i].result;
	abort();
}

const char *
optargs_opt_by_short(const struct optargs_opt *opts, const char s)
{
	int i;

	for (i = 0; option_is_valid(&opts[i]); ++i)
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
