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

#pragma once

#include "optargs_config.h"
#include "optargs_compiler_attributes.h"
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
 * generate a vertical space/divider in argument descriptions.
 */
#define optargs_arg_div \
{\
	.type = _optargs_arg_div\
}

/**
 * A convenience macro for initializing an option entry that will
 * generate a vertical space/divider in option descriptions.
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
 * information about which options were found from the parsed input.
 */
struct optargs_res
{
	/**
	 * The value of this option (whether it was given by the user or not).
	 *
	 * Note that an option can not be given both with and without an
	 * argument during the same invocation (contradicting arguments).
	 */
	union
	{
		/**
		 * The user supplied character array given as an argument to this
		 * option.
		 */
		char const * NULLABLE string;

		/**
		 * The number of times this option was given by the user (without
		 * an option argument).
		 */
		unsigned long count;
	} value;

	/**
	 * The type of the result (i.e. which type from the value union should
	 * be used).
	 */
	enum optargs_res_type
	{
		/** This option was not given. */
		optargs_undef,

		/** This option was given with an argument to it. */
		optargs_string,

		/** This option was given one or more times without an argument. */
		optargs_count
	} type : 8;
};

/**
 * Command line argument structure.
 *
 * An instance of this structure will be used to store and relay
 * information about which arguments were found from the parsed input.
 *
 * A list of these is also used to define the names, descriptions and
 * types of the accepted arguments.
 */
struct optargs_arg
{
	/**
	 * The name of the argument (e.g. "COMMAND", "FILE" or "action").
	 */
	char const * const NULLABLE name;

	/**
	 * Description of the argument. Not necessarily needed for a group
	 * type of an argument (individual group members will require
	 * descriptions nevertheless).
	 */
	char const * const NULLABLE description;

	/**
	 * Type of the argument.
	 *
	 * Affects what kind of a input is accepted as this argument.
	 */
	enum optargs_arg_type
	{
		/**
		 * Anything the user might supply is accepted as this
		 * (positional) argument, but something must be given.
		 */
		optargs_arg_any,

		/**
		 * Anything the user might (or might not) supply is accepted as
		 * this (positional) argument.
		 */
		optargs_arg_any_opt,

		/**
		 * Specifies an argument group (name).
		 *
		 * The optargs_arg_structures following this in the supplied list
		 * will defined the members of this argument group.
		 *
		 * E.g. an argument group could be defined as "COMMAND" and the
		 * members of the group define the actual commands (names and
		 * descriptions) that are accepted as an argument for this group.
		 */
		optargs_arg_group,

		/**
		 * As optargs_arg_group, but optional.
		 *
		 * It will not be an error to omit the argument from the user
		 * input.
		 */
		optargs_arg_group_opt,

		/**
		 * An argument group member.
		 *
		 * Defines a member of optargs_arg_group or optargs_arg_group_opt.
		 * Must immediately follow a optargs_arg structure instance of
		 * type optargs_arg_group or optargs_arg_group_opt. Can be given
		 * multiple times. An optargs_arg structure instance of a type
		 * other than optargs_arg_group_member indicates the end of
		 * optargs_arg_group or optargs_arg_group_opt type of an argument.
		 */
		optargs_arg_group_member,

		/**
		 * Defines a argument divider.
		 *
		 * See optargs_arg_div convenience macro which is the preferred
		 * way of using this.
		 */
		_optargs_arg_div,

		/**
		 * Defines a sink argument.
		 *
		 * See optargs_arg_sink convenience macro which is the preferred
		 * way of using this.
		 */
		_optargs_arg_sink,

		/**
		 * Defines an end of an argument list.
		 *
		 * See optargs_arg_eol convenience macro which is the preferred
		 * way of using this.
		 */
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
		 *
		 * Valid for non-group type arguments (i.e. this is the "any"
		 * input).
		 */
		char const * NULLABLE value;

		/**
		 * A pointer to the argument group member instance that was given
		 * by the user.
		 *
		 * This will be used only for group types of arguments.
		 */
		struct optargs_arg * NULLABLE match;
	} result;

	/**
	 * A list of possible subarguments this argument might be expecting.
	 *
	 * The list must be terminated with an optargs_arg_eol of its own if
	 * defined. Can be left to NULL if no subarguments are expected.
	 */
	struct optargs_arg * NULLABLE subargument;
};

/**
 * Command line option structure.
 *
 * A list of these is used to define the names, descriptions and types of
 * the accepted options.
 *
 * The results of the option parsing will be stored in the _result_ field
 * after parsing.
 *
 * An option with only the _argument_ field defined will yield a vertical
 * divider into the option help text. See optargs_opt_div convenience
 * macro for more information about this.
 */
struct optargs_opt
{
	/**
	 * The long name of this option (e.g. "help" would become "--help"
	 * option).
	 *
	 * Should be left to NULL if no long form for this option is needed.
	 */
	char const * const NULLABLE long_option;

	/**
	 * The short name of this option (e.g. 'h' would become "-h" option).
	 *
	 * Should be left to NULL/0 if no short form for this option is needed.
	 */
	char const short_option;

	/**
	 * Description of this option.
	 *
	 * Should be left to NULL if no description for this option is needed.
	 *
	 * An option without a description will not be shown in the help text.
	 * It could be seen as an undocumented "secret option".
	 */
	char const * const NULLABLE description;

	/**
	 * Will contain the result(s) of this option after options have been
	 * processed.
	 */
	struct optargs_res result;

	/**
	 * Defines a (single) possible argument for this option.
	 *
	 * Should be left to NULL if no argument is expected for this option.
	 */
	struct optargs_arg * NULLABLE argument;
};

/**
 * Parse the command line options from the given arguments.
 *
 * Arguments:
 *  argc:    Number of arguments in the argv array.
 *  argv:    An array of arguments to parse options from.
 *  options: An array of options to parse from the argv array. The array
 *           must be terminated with an optargs_opt_eol instance.
 *
 * Return value:
 *  The index number of the first non-option element (a "--" is not
 *  considered a non-option here) in the argv array or a negative number
 *  if an error was encountered. The return value equals to argc in case
 *  no non-option arguments were given.
 */
WARN_UNUSED_RESULT
int
optargs_parse_opts(
		int argc,
		char const * NONNULL const * NONNULL argv,
		struct optargs_opt * NONNULL options
		);

/**
 * Parse the optargs arguments from the given set of char * arguments.
 *
 * Arguments:
 *  argc:      Number of arguments (char *) in the argv array.
 *  argv:      The arguments (char *) from where the arguments are to be
 *             parsed from.
 *  arguments: The arguments structures defining the accepted arguments.
 *
 * Return value:
 *  The number of successfully parsed argument (not counting what went
 *  into a sink) or a negative integer in case of an error.
 */
WARN_UNUSED_RESULT
int
optargs_parse_args(
		int argument_count,
		char const * NONNULL const * NONNULL arguments,
		struct optargs_arg * NONNULL supported_arguments
		);

/**
 * Print a help text based on the given arguments.
 *
 * Print a help text to stdout that is formed from the given executable
 * name, description of the program and supported command line options and
 * arguments.
 *
 * Arguments:
 *  cmd:       The name/path of the executed file (i.e. name of the program).
 *  about:     A general description of the executed program or NULL if
 *             not relevant.
 *  options:   A null terminated array of supported options or NULL if
 *             none are supported.
 *  arguments: A null terminated array of supported arguments or NULL if
 *             none are supported.
 */
void
optargs_print_help(
		char const * NONNULL cmd,
		char const * NULLABLE about,
		struct optargs_opt const * NULLABLE options,
		struct optargs_arg const * NULLABLE arguments
		);

/**
 * Find the result struct for the given long option.
 *
 * The given long option must exist in the given list of options.
 *
 * Arguments:
 *  options:     An array of options from where to search for the given
 *               option by the field long_option. The array should have
 *               previously been processed by the optargs_opt_parse()
 *               function.
 *  long_option: The name of the long option to search for from the
 *               given list of options.
 *
 * Return value:
 *  A pointer to the result structure from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
WARN_UNUSED_RESULT
struct optargs_res const * NULLABLE
optargs_opt_res_by_long(
		struct optargs_opt const * NONNULL options,
		char const * NONNULL long_option
		);

/**
 * Find the result struct for the given short option.
 *
 * The given short option must exist in the given list of options.
 *
 * Arguments:
 *  options:      An array of options from where to search for the given
 *                option by the field short_option. The array should have
 *                previously been processed by the optargs_opt_parse()
 *                function.
 *  short_option: The name of the short option to search for from the
 *                given list of options.
 *
 * Return value:
 *  A pointer to the result structure from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
WARN_UNUSED_RESULT
struct optargs_res const * NULLABLE
optargs_opt_res_by_short(
		struct optargs_opt const * NONNULL options,
		char short_option);

/**
 * Find the result struct for the option in the given index.
 *
 * Simply returns the (index + 1)th option's result (pointer) from the
 * given array. The given index number must match an element in the given
 * list of options.
 *
 * Arguments:
 *  options: An array of options from where to fetch the desired option.
 *           The array should have previously been processed by the
 *           optargs_opt_parse() function.
 *  index:   The index of the sought option in the options array.
 *
 * Return value:
 *  A pointer to the result structure from the option structure or NULL if
 *  the specified option was not given by the user.
 */
WARN_UNUSED_RESULT
struct optargs_res const * NULLABLE
optargs_opt_res_by_index(
		struct optargs_opt const * NONNULL opts,
		int index
		);

/**
 * Find the result string for the given long option.
 *
 * The given long option must exist in the given list of options. The type
 * of the result struct must be optargs_string (or optargs_undef).
 *
 * Arguments:
 *  options:     An array of options from where to search for the given
 *               option by the field long_option. The array should have
 *               previously been processed by the optargs_opt_parse()
 *               function.
 *  long_option: The name of the long option to search for from the
 *               given list of options.
 *
 * Return value:
 *  A pointer to the result string from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
WARN_UNUSED_RESULT
char const * NULLABLE
optargs_opt_value_by_long(
		struct optargs_opt const * NONNULL opts,
		char const * NONNULL long_option
		);

/**
 * Find the result string for the given short option.
 *
 * The given short option must exist in the given list of options. The
 * type of the result struct must be optargs_string (or optargs_undef).
 *
 * Arguments:
 *  options:      An array of options from where to search for the given
 *                option by the field short_option. The array should have
 *                previously been processed by the optargs_opt_parse()
 *                function.
 *  short_option: The name of the short option to search for from the
 *                given list of options.
 *
 * Return value:
 *  A pointer to the result string from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
WARN_UNUSED_RESULT
char const * NULLABLE
optargs_opt_value_by_short(
		struct optargs_opt const * NONNULL opts,
		char short_option);

/**
 * Find the result string for the option in the given index.
 *
 * Simply returns the (index + 1)th option's result (pointer) from the
 * given array. The given index number must match an element in the given
 * list of options. The type of the result struct must be optargs_string
 * (or optargs_undef).
 *
 * Arguments:
 *  options: An array of options from where to fetch the desired option.
 *           The array should have previously been processed by the
 *           optargs_opt_parse() function.
 *  index:   The index of the sought option in the options array.
 *
 * Return value:
 *  A pointer to the result string from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
WARN_UNUSED_RESULT
char const * NULLABLE
optargs_opt_value_by_index(
		struct optargs_opt const * NONNULL opts,
		int index
		);

/**
 * Find the result count for the given long option.
 *
 * The given long option must exist in the given list of options. The type
 * of the result struct must be optargs_count (or optargs_undef).
 *
 * Arguments:
 *  options:     An array of options from where to search for the given
 *               option by the field long_option. The array should have
 *               previously been processed by the optargs_opt_parse()
 *               function.
 *  long_option: The name of the long option to search for from the
 *               given list of options.
 *
 * Return value:
 *  A count of how many times the given option was given by the user.
 */
WARN_UNUSED_RESULT
unsigned int
optargs_opt_count_by_long(
		struct optargs_opt const * NONNULL opts,
		char const * NONNULL long_option
		);

/**
 * Find the result count for the given short option.
 *
 * The given short option must exist in the given list of options. The
 * type of the result struct must be optargs_count (or optargs_undef).
 *
 * Arguments:
 *  options:      An array of options from where to search for the given
 *                option by the field short_option. The array should have
 *                previously been processed by the optargs_opt_parse()
 *                function.
 *  short_option: The name of the short option to search for from the
 *                given list of options.
 *
 * Return value:
 *  A count of how many times the given option was given by the user.
 */
WARN_UNUSED_RESULT
unsigned int
optargs_opt_count_by_short(
		struct optargs_opt const * NONNULL opts,
		char short_option
		);

/**
 * Find the result count for the option in the given index.
 *
 * Simply returns the (index + 1)th option's result (pointer) from the
 * given array. The given index number must match an element in the given
 * list of options. The type of the result struct must be optargs_count
 * (or optargs_undef).
 *
 * Arguments:
 *  options: An array of options from where to fetch the desired option.
 *           The array should have previously been processed by the
 *           optargs_opt_parse() function.
 *  index:   The index of the sought option in the options array.
 *
 * Return value:
 *  A count of how many times the given option was given by the user.
 */
WARN_UNUSED_RESULT
unsigned int
optargs_opt_count_by_index(
		struct optargs_opt const * NONNULL opts,
		int index
		);

/**
 * Check the type of the given optargs_res structure.
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

WARN_UNUSED_RESULT
enum optargs_res_type
optargs_res_type(
		struct optargs_res const * NONNULL result
		);

/**
 * Get the value of an argument.
 *
 * The user provided value for an argument is returned or NULL if the
 * argument was not specified by the user.
 *
 * Arguments:
 *   argument: The argument from which to get the value from. Must be of
 *             type optargs_arg_group, optargs_arg_group_opt,
 *             optargs_arg_any or optargs_arg_any_opt.
 *
 * Return value:
 *   The value of the argument or NULL if the argument was not defined by
 *   the user.
 */
WARN_UNUSED_RESULT
char const * NULLABLE
optargs_arg_value(
		struct optargs_arg const * NONNULL argument
		);

/**
 * Get the index offset from a optargs_arg_group (or optargs_arg_group_opt)
 * to the optargs_arg_group_member array member that was given by the
 * user.
 *
 * Arguments:
 *   argument: The optargs_arg_group (or optargs_arg_group_opt) array
 *             member from which to calculate the distance to the user
 *             supplied optargs_arg_group_member instance. The array
 *             should have previously been processed by the
 *             optargs_opt_parse() function.
 *
 * Return value:
 *   The calculated distance (offset) or a negative value if the argument
 *   was not given by the user.
 */
WARN_UNUSED_RESULT
int
optargs_arg_value_offset(
		struct optargs_arg const * NONNULL argument
		);

/**
 * Get the index of the optargs_arg_group_member array member that was
 * provided by the user as the value for the specified optarg_arg_group
 * (or optargs_arg_group_opt).
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
WARN_UNUSED_RESULT
int
optargs_arg_value_index(
		struct optargs_arg const * NONNULL arguments,
		int base_index
		);
