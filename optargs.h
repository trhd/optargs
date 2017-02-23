/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2016-2017 Hemmo Nieminen
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

#include <stdbool.h>

/**
 * A macro for initializing an all zero optargs_res structure.
 */
#define optargs_res_null \
{\
	.type = 0,\
	.value = { 0 }\
}

/**
 * A convenience macro for generating an argument sink entry.
 *
 * A sink entry will consume all extra arguments allowing more arguments
 * to be given to a program that are actually expected by the
 * optargs_arg_parse() function. A sink entry will also end argument
 * parsing just like optargs_arg_eol does.
 */
#define optargs_arg_sink \
{\
	.type = _optargs_arg_sink\
}

/**
 * A convenience macro for initializing an argument entry that will
 * generate an horizontal space in argument descriptions.
 */
#define optargs_arg_div \
{\
	.type = _optargs_arg_div\
}

/**
 * A convenience macro for initializing an option entry that will
 * generate an horizontal space in option descriptions.
 */
#define optargs_opt_div \
{\
	.argument = (struct optargs_arg []) { optargs_arg_div, optargs_arg_eol }\
}

/**
 * A convenience macro for initializing an "end of list" argument entry.
 *
 * This can (and must if optargs_arg_sink is not used) be used to indicate
 * the end of an argument array.
 */
#define optargs_arg_eol \
{\
	.name = (void*)0,\
	.description = (void*)0,\
	.type = _optargs_arg_eol\
}

/**
 * A convenience macro for initializing an "end of list" option entry.
 *
 * This can (and must) be used to indicate the end of an option array.
 */
#define optargs_opt_eol \
{\
	.long_option = (void*)0,\
	.short_option = 0,\
	.description = (void*)0,\
	.argument = (struct optargs_arg []){optargs_arg_eol},\
	.result = optargs_res_null\
}

/**
 * Argument result structure.
 *
 * An instance of this structure will be used to store and relay
 * information about which options/arguments were found from the parsed
 * input.
 */
struct optargs_res
{
	/**
	 * The user supplied character pointer (argument/value) or the number
	 * of times an option was given without an user supplied
	 * argument/value.
	 */
	union
	{
		char const * string;
		unsigned long count;
	} value;

	/**
	 * The type of the result (i.e. which type from the value union should
	 * be used).
	 */
	enum optargs_res_type
	{
		optargs_undef,
		optargs_string,
		optargs_count
	} type : 8;
};

/**
 * Command line argument structure.
 *
 * Specifies the argument's name, description and the type of the
 * argument.
 */
struct optargs_arg
{
	/**
	 * The name of the argument (group).
	 */
	char const * const name;

	/**
	 * Description of the argument. Not necessarily needed for a group
	 * type argument (individual group members will require descriptions).
	 */
	char const * const description;

	/**
	 * Type of the argument.
	 *
	 * An argument of the type "any" accepts any value the user might give
	 * (one per variable). A group type argument expects the user provided
	 * value to match the name of an group member argument (that
	 * immediately follows the group argument). A divider argument can be
	 * used to create a horizontal space (divider) into the help text. An
	 * EOL type of argument implies the end of an argument structure list.
	 * A sink argument will in addition to terminating the list of
	 * arguments consume all the extra arguments given by the user, that
	 * would otherwise cause an error (too many arguments).
	 *
	 * A _opt suffix implies that the argument is optional and will not
	 * cause an error if missing.
	 */
	enum optargs_arg_type
	{
		optargs_arg_any,
		optargs_arg_any_opt,
		optargs_arg_group,
		optargs_arg_group_opt,
		optargs_arg_group_member,
		_optargs_arg_div,
		_optargs_arg_sink,
		_optargs_arg_eol
	} type : 8;

	/**
	 * The result of the argument after parsing.
	 */
	union
	{
		/**
		 * Non-zero if the argument was given.
		 */
		unsigned long defined;

		/**
		 * The input given by the user.
		 */
		char const * value;

		/**
		 * A pointer to the group member entry that was given by the user.
		 * This will be used only for group types of arguments.
		 */
		struct optargs_arg * match;
	} result;

	struct optargs_arg * subargument;
};

/**
 * Command line option structure.
 *
 * Defines both the option's short and long form and the description of
 * the option that should be shown to the user. An argument field is
 * included to contain all the necessary information about the option's
 * possible argument. The result field will be filled during parsing.
 */
struct optargs_opt
{
	/**
	 * The long name of this option (e.g. "help" would become "--help").
	 */
	char const * const long_option;

	/**
	 * The short name of this option (e.g. 'h' would become "-h").
	 */
	char const short_option;

	/**
	 * Description of this option. An option without a description will
	 * not be shown in the help text (i.e. a "secret option").
	 *
	 * An option that has only the argument field defined will cause only
	 * an empty line (divider) in the help text.
	 */
	char const * const description;

	/**
	 * Will contain the result(s) of this option after parsing.
	 */
	struct optargs_res result;

	/**
	 * Defines a possible argument for this option.
	 */
	struct optargs_arg * argument;
};

/**
 * Parse the command line options from the given arguments.
 *
 * Arguments:
 *  argc: Number of arguments in the argv array.
 *  argv: An array of arguments to parse options from.
 *  options: A null terminated array of options to parse from the argv array.
 *
 * Return value:
 *  The index number of the first non-option element (a -- is considered
 *  as an option here) in the argv array or a negative number if an error
 *  was encountered. The return value equals to argc in case no non-option
 *  arguments were given.
 */
int
optargs_parse_opts(int argc, char const * const * argv, struct optargs_opt * options);

/**
 * Parse the optargs arguments from the given set of char * arguments.
 *
 * Arguments:
 *  argument_count: Number of argument character pointers in the arguments array.
 *  arguments: The char * arguments that are to ber parsed.
 *  supported_arguments: The arguments structures defining the accepted arguments.
 *
 * Return value:
 *  The number of successfully parsed argument (not counting what went
 *  into the sink) or a negative integer in case of an error.
 */
int
optargs_parse_args(int argument_count, char const * const * arguments,
		struct optargs_arg * supported_arguments);

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
void
optargs_print_help(char const * cmd, char const * about,
		struct optargs_opt const * options, struct optargs_arg const * arguments);

/**
 * Find the result struct for the given long option.
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
 *  A pointer to the result structure from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
struct optargs_res const *
optargs_opt_res_by_long(struct optargs_opt const * options, char const * long_option);

/**
 * Find the result struct for the given short option.
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
 *  A pointer to the result structure from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
struct optargs_res const *
optargs_opt_res_by_short(struct optargs_opt const * options, char short_option);

/**
 * Find the result struct for the option in the given index.
 *
 * Simply returns the (index + 1)th option's result (pointer) from the
 * given array. The given index number must match an element in the given
 * list of options.
 *
 * Arguments:
 *  options: Null terminated array of options from where to get the sought option.
 *  index:   The index of the sought option in the options array.
 *
 * Return value:
 *  A pointer to the result structure from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
struct optargs_res const *
optargs_opt_res_by_index(struct optargs_opt const * opts, int index);

/**
 * Find the result string for the given long option.
 *
 * The given long option must exist in the given list of options. The type
 * of the result struct must be optargs_string (or optargs_undef).
 *
 * Arguments:
 *  options:     Null terminated array of options from where to search
 *               for the given long options long_option.
 *  long_option: The name of the long option to search for from the
 *               given list of options.
 *
 * Return value:
 *  A pointer to the result string from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
char const *
optargs_opt_value_by_long(struct optargs_opt const * opts, char const * long_option);

/**
 * Find the result string for the given short option.
 *
 * The given short option must exist in the given list of options. The
 * type of the result struct must be optargs_string (or optargs_undef).
 *
 * Arguments:
 *  options:      Null terminated array of options from where to search
 *                for the given short option short_option.
 *  short_option: The name of the short option to search for from the
 *                given list of options.
 *
 * Return value:
 *  A pointer to the result string from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
char const *
optargs_opt_value_by_short(struct optargs_opt const * opts, char short_option);

/**
 * Find the result string for the option in the given index.
 *
 * Simply returns the (index + 1)th option's result (pointer) from the
 * given array. The given index number must match an element in the given
 * list of options. The type of the result struct must be optargs_string
 * (or optargs_undef).
 *
 * Arguments:
 *  options: Null terminated array of options from where to get the sought option.
 *  index:   The index of the sought option in the options array.
 *
 * Return value:
 *  A pointer to the result string from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
char const *
optargs_opt_value_by_index(struct optargs_opt const * opts, int index);

/**
 * Find the result count for the given long option.
 *
 * The given long option must exist in the given list of options. The type
 * of the result struct must be optargs_count (or optargs_undef).
 *
 * Arguments:
 *  options:     Null terminated array of options from where to search
 *               for the given long options long_option.
 *  long_option: The name of the long option to search for from the
 *               given list of options.
 *
 * Return value:
 *  A count of how many times the given option was given by the user.
 */
unsigned int
optargs_opt_count_by_long(struct optargs_opt const * opts, char const * long_option);

/**
 * Find the result count for the given short option.
 *
 * The given short option must exist in the given list of options. The
 * type of the result struct must be optargs_count (or optargs_undef).
 *
 * Arguments:
 *  options:      Null terminated array of options from where to search
 *                for the given short option short_option.
 *  short_option: The name of the short option to search for from the
 *                given list of options.
 *
 * Return value:
 *  A count of how many times the given option was given by the user.
 */
unsigned int
optargs_opt_count_by_short(struct optargs_opt const * opts, char short_option);

/**
 * Find the result count for the option in the given index.
 *
 * Simply returns the (index + 1)th option's result (pointer) from the
 * given array. The given index number must match an element in the given
 * list of options. The type of the result struct must be optargs_count
 * (or optargs_undef).
 *
 * Arguments:
 *  options: Null terminated array of options from where to get the sought option.
 *  index:   The index of the sought option in the options array.
 *
 * Return value:
 *  A count of how many times the given option was given by the user.
 */
unsigned int
optargs_opt_count_by_index(struct optargs_opt const * opts, int index);

/**
 * Check if the given result structure has been defined (it has a value).
 *
 * A result structure is defined if the option that "owns" the result was
 * encoutered during the parsing of the options.
 *
 * Arguments:
 *  result: A pointer to the result structure that is to be checked.
 *
 * Return value:
 *  The type of the result value (i.e. optargs_undef for an undefined
 *  result and the type of the result otherwise).
 */

enum optargs_res_type
optargs_res_type(struct optargs_res const * result);

/**
 * Get the value of an argument.
 *
 * The user provided value for an argument is returned or NULL if the
 * argument was not specified by the user. The argument argument should be
 * of type group or any.
 *
 * Arguments:
 *   argument: The argument from which to get the value from.
 *
 * Return value:
 *   The value of the argument or NULL if the argument was not defined by
 *   the user.
 */
char const *
optargs_arg_value(struct optargs_arg const * argument);

/**
 * Get the offset for a group argument's value.
 *
 * Get the distance (measured from the argument arrays index numbers) from
 * the group argument to the group member argument that was given by the
 * user.
 *
 * Arguments:
 *   argument: The argument from which to calculate the distance to the
 *             user supplied group member argument.
 *
 * Return value:
 *   The calculated distance (offset) or -1 if the argument was not
 *   given by the user.
 */
int
optargs_arg_value_offset(struct optargs_arg const * argument);

/**
 * Get the index for a group argument's value.
 *
 * Get the index of the group member argument that was provided by the
 * user as the value for the group type argument identified from the
 * argument array by the given base index.
 *
 * Arguments:
 *   arguments:  The argument array from which to look for the group and
 *               group member arguments.
 *   base_index: The index of the group argument for which the sought
 *               group member argument belongs to.
 *
 * Return value:
 *   The calculated distance (offset) or -1 if the argument was not
 *   given by the user.
 */
int
optargs_arg_value_index(struct optargs_arg const * arguments, int base_index);
