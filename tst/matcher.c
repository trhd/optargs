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

#define _POSIX_C_SOURCE 200112L
#include <alloca.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "optargs.h"

static void __attribute__((noreturn))
error(const char *msg)
{
	fprintf(stderr, "ERROR: %s\n", msg);
	exit(EXIT_FAILURE);
}

static void
compare_outputs(const char *a, const char *b, size_t l, bool fail)
{
	if (strncmp(a, b, l) && !fail)
	{
		fflush(NULL);
		fprintf(stderr, "-%.*s-\n+%.*s+\n", (int)l, a, (int)l, b);
		error("Outputs differ.");
	}

	if (!fail)
		printf("=%s", a);
}

static int
min(int a, int b)
{
	return a < b ? a : b;
}

int
main(int ac, char **av)
{
	pid_t chld;
	int pp[2], idx;

	struct optargs_opt opts[] =
	{
		{
			.description = "Pattern matching shall fail.",
			.long_option = "fail-match",
			.short_option = 'f',
			.argument = { .mandatory = optargs_no },
		},
		{
			.description = "Pattern is a file name.",
			.long_option = "file",
			.short_option = 'F',
			.argument = { .mandatory = optargs_no },
		},
		{
			.description = "The argument shall match the executable's exit code.",
			.long_option = "exit-code",
			.short_option = 'e',
			.argument = { .mandatory = optargs_yes}
		}
	};

	if ((idx = optargs_parse(ac, (const char **)av, opts)) < 0)
	{
		printf("\n");
		optargs_print_help(av[0], "", opts, NULL);
		return EXIT_FAILURE;
	}

	if (pipe(pp))
		error("Failed to create a pipe.\n");

	if ((chld = fork()) == -1)
		error("Failed to fork.");
	else if (!chld)
	{
		if (close(pp[0]))
			error("Failed to close pipe's reading end.");

		if (dup2(pp[1], 2) == -1)
			error("Failed to dup() stderr.");

		if (dup2(pp[1], 1) == -1)
			error("Failed to dup() stdout.");

		if (execv(av[idx + 1], av + idx + 1))
			error("Failed to execv.");

		error("This should never be seen.");
	}
	else
	{
		enum { buf_size = 1024 };
		char buf1[buf_size], buf2[buf_size];
		FILE *fp = fdopen(pp[0], "r"), *ff = NULL;
		int i;

		if (!fp)
			error("fdopen() failed.");

		if (close(pp[1]))
			error("Failed to close pipe's writing end.");

		if (!optargs_opt_by_short(opts, 'F'))
		{
			if (!fgets(buf1, buf_size, fp))
				error("Failed to read program's output.");

			i = strlen(buf1);

			if (buf1[i-1] == '\n')
				buf1[i-1] = '\0';

			compare_outputs(buf1, av[idx],
					min(strlen(av[idx + 1]) + 1, strlen(buf1) + 1),
					optargs_opt_by_short(opts, 'f'));

		}
		else
		{
			ff = fopen(av[idx], "r");

			if (!ff)
				error("fdopen() failed");

			while (fgets(buf1, buf_size, fp) && fgets(buf2, buf_size, ff))
				compare_outputs(buf1, buf2, buf_size, optargs_opt_by_short(opts, 'f'));
		}


		if (close(pp[0]))
			error("Failed to close pipe's reading end.");

		if (waitpid(chld, &i, 0) != chld)
			error("waitpid() returned unexpected value.");

		if (!WIFEXITED(i))
			error("Expected child to return a status.");

		if (WEXITSTATUS(i) != (optargs_opt_by_short(opts, 'e') ? atoi(optargs_opt_by_short(opts, 'e')) : 0))
		{
			printf("Child returned: %d, expected %s.\n",
					WEXITSTATUS(i),
					optargs_opt_by_short(opts, 'e') ? optargs_opt_by_short(opts, 'e') : "0");
			error("Child returned incorrect exit code.");
		}
	}

	return EXIT_SUCCESS;
}
