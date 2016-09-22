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

#pragma once

/**
 * A macro for initializing an all zero instance of struct optargs_arg.
 * Can be used e.g. to indicate the end of an argument list.
 */
#define optargs_arg_eol { (void*)0, (void*)0, 0 }

/**
 * A macro for initializing an all zero instance of struct optargs_opt.
 * Can be used e.g. to indicate the end of an option list.
 */
#define optargs_opt_eol { (void*)0, (void*)0, 0, optargs_arg_eol, (void*)0 }

/**
 * A multiple choice enumeration.
 *
 * Used e.g. to indicate whether an argument is mandatory,
 * optional or not allowed.
 */
enum optargs_multiple_choice
{
	optargs_no = 1,
	optargs_yes,
	optargs_maybe
};

/**
 * Command line argument structure.
 *
 * Specifies the argument's name, description and whether it is an
 * mandatory or an optional argument.
 */
struct optargs_arg
{
	/**
	 * If defined, this will be shown on the "usage line" of the help text
	 * (with the mandatoryness of this taken into account).
	 */
	const char *name;

	/**
	 * Description of this argument. If defined, this will be shown in the
	 * arguments section of the help text. This requires that the argument
	 * also has the name field defined. An argument with no description
	 * will become a subheader for a group of arguments (e.g. "COMMANDS").
	 */
	const char *description;

	/**
	 * Defines the mandatoriness of this argument. Affects how the
	 * argument's name (if defined) is show on the "usage line" of the
	 * help text.
	 *
	 * Mandatory field is always mandatory!
	 */
	enum optargs_multiple_choice mandatory;
};

/**
 * Command line option structure.
 *
 * Defines the option's both short and long form, whether it
 * requires or accepts an argument and the description of the
 * option that should be shown to the user. The struct also
 * includes a result pointer for storing the result of whether the
 * option was given or not and if the option was given an argument.
 */
struct optargs_opt
{
	/**
	 * Description of this option. An option without a description will
	 * not be shown in the help text (i.e. a "secret option").
	 *
	 * An option that has only the argument field defined will cause only
	 * an empty line (divider) in the help text.
	 */
	const char *description;

	/**
	 * The long name of this option (e.g. "help" would become "--help").
	 */
	const char *long_option;

	/**
	 * The short name of this option (e.g. 'h' would become "-h").
	 */
	const char short_option;

	/**
	 * Defines the possible argument for this option. This field must
	 * always be defined (along with its mandatory mandatory field). See
	 * struct optargs_arg's comments above for more information.
	 */
	const struct optargs_arg argument;

	/**
	 * Will contain the result of this option after parsing. If the option
	 * wasn't given by the user this will contain NULL. Othwerwise the
	 * value of this will be the address of the optargs' default option
	 * value or that of the user provided argument for this option.
	 */
	const char *result;
};

/**
 * Parse the command line options from the given arguments.
 *
 * Arguments:
 *  argc: Number of arguments in the argv array.
 *  argv: An array of arguments to parse.
 *  options: A null terminated array of options to parse from the argv array.
 *
 * Return value:
 *  The index number of the first non-option element in the argv array or
 *  a negative number if an error was encountered.
 */
int optargs_parse(int argc, const char **argv, struct optargs_opt *options);

/**
 * Print a help text based on the given arguments.
 *
 * Print a help text to stdout that is formed from the given executable
 * name, optional description of the program and supported command line
 * options and arguments.
 *
 * Arguments:
 *  cmd:       The name/path of the executed file.
 *  about:     Optional (can be null) general description of the executed program.
 *  options:   A null terminated array of supported options.
 *  arguments: A null terminated array of supported arguments.
 */
void optargs_print_help(const char *cmd, const char *about,
		const struct optargs_opt *options, const struct optargs_arg *arguments);

/**
 *
 * Determine if an option's value is the "default value" meaning that the
 * option was given without an argument.
 *
 * An option that was not given an argument by the user will have the
 * optargs' default value assigned to it. This function can be used to
 * determine if this is the case.
 *
 * This function will also tell if the option was given multiple times
 * (without an argument).
 *
 * Arguments:
 *  result:   The value of the option whose defaultness or number of times
 *            given is to be determined.
 *
 * Return value:
 *  The number of times the option was given without an argument or zero
 *  if the value of the option is not the default value.
 */
int optargs_is_default(const char *result);

/**
 * Find the matching option from a list by long option name.
 *
 * The given long option must exist in the given list of options.
 *
 * Arguments:
 *  options:     Null terminated array of options from where to search
 *               for the given long options long_option.
 *  long_option: The name of the long option to search for from the
 *               given list of options.
 *
 * Return value:
 *  The result pointer from the matching option structure.
 */
const char * optargs_opt_by_long(const struct optargs_opt *options, const char *long_option);

/**
 * Find the matching option from a list by short option name.
 *
 * The given short option must exist in the given list of options.
 *
 * Arguments:
 *  options:      Null terminated array of options from where to search
 *                for the given short option short_option.
 *  short_option: The name of the short option to search for from the
 *                given list of options.
 *
 * Return value:
 *    The result pointer from the matching option structure.
 */
const char * optargs_opt_by_short(const struct optargs_opt *options, const char short_option);

/**
 * Get the value of an option by its index number.
 *
 * Simply returns the (index + 1)th option's result pointer from the given
 * array. The given index number must match an element in the given list
 * of options.
 *
 * Arguments:
 *  options: Null terminated array of options from where to get the sought
 *           option.
 *  index:   The index of the sought option in the options array.
 *
 * Return value:
 *  The result pointer from the matching option structure.
 */
const char * optargs_opt_by_index(const struct optargs_opt *opts, int index);

/**
 * Get the index of the argument with a matching name.
 *
 * Can be used to e.g. check if "name" is a known argument.
 *
 * Arguments:
 *  arguments: Null terminated array of arguments from where to look
 *             for the matching argument.
 *  name:      The name of the sought argument.
 *
 * Return value:
 *  The index of the matching argument or a negative number if a matching
 *  argument was not found.
 */
int optargs_arg_index(const struct optargs_arg *arguments, const char *name);
