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

#pragma once

#include "optargs_config.h"

/**
 * A convenience macro for generating an argument sink entry.
 *
 * A sink entry will consume all extra arguments allowing more arguments
 * to be given to a program that are actually expected by the
 * optargs_parse_arguments() function. A sink entry will also end argument
 * parsing just like optargs_argument_eol does.
 */
#define optargs_argument_sink \
{ \
	.type = _optargs_argument_sink \
}

/**
 * A convenience macro for initializing an argument entry that will
 * generate a vertical space/divider in help texts' argument descriptions.
 */
#define optargs_argument_divider \
{ \
	.type = _optargs_argument_divider \
}

/**
 * A convenience macro for initializing an option entry that will
 * generate a vertical space/divider in help texts' option descriptions.
 */
#define optargs_option_divider \
{ \
	.argument = (struct optargs_argument []) \
	{ \
		optargs_argument_divider, \
		optargs_argument_eol \
	} \
}

/**
 * A convenience macro for initializing an "end of list" argument entry.
 *
 * This can (and must if optargs_argument_sink is not used) be used to
 * indicate the end of an argument array.
 */
#define optargs_argument_eol \
{ \
	.name = (void*)0, \
	.description = (void*)0, \
	.type = _optargs_argument_eol \
}

/**
 * A convenience macro for initializing an "end of list" option entry.
 *
 * This can (and must) be used to indicate the end of an option array.
 */
#define optargs_option_eol \
{ \
	.long_option = (void*)0, \
	.short_option = 0, \
	.description = (void*)0, \
	.value_type = optargs_undef, \
	.argument = (struct optargs_argument []) { optargs_argument_eol } \
}

/**
 * Command line argument structure.
 *
 * An instance of this structure will be used to store and relay
 * information about which arguments were found from the parsed input.
 *
 * A list of these is used to define the names, descriptions and types of
 * the accepted arguments.
 */
struct optargs_argument
{
	/**
	 * The name of the argument. E.g. "command", "file" or "directory".
	 *
	 * An argument group (see types below) without a name will be
	 * "expanded" (all the members of the group will be listed
	 * individually (e.g. on the usage line)).
	 */
	char const * const NULLABLE name;

	/**
	 * Description of the argument.
	 *
	 * Not necessarily needed for a group type of an argument (individual
	 * group members will require descriptions nevertheless).
	 */
	char const * const NULLABLE description;

	/**
	 * Type of the argument.
	 *
	 * Affects what kind of a input is accepted as this argument.
	 */
	enum optargs_argument_type
	{
		/**
		 * Anything the user might supply is accepted as this
		 * (positional) argument, but something must be given.
		 */
		optargs_argument_any,

		/**
		 * Anything the user might (or might not) supply is accepted as
		 * this (positional) argument.
		 */
		optargs_argument_any_opt,

		/**
		 * Specifies an argument group (i.e. a common name for a group of
		 * arguments out of which a single one can be given).
		 *
		 * The argument structures following this in the supplied list
		 * will define the members of this argument group (as long as
		 * their type is optargs_argument_group_member (see below)).
		 *
		 * E.g. an argument group could be defined as "command" and the
		 * members of the group define the actual commands (e.g. "start"
		 * and "stop") that are accepted as an argument for this group.
		 */
		optargs_argument_group,

		/**
		 * As optargs_argument_group, but optional.
		 *
		 * It will not be an error to omit the argument from the user input.
		 */
		optargs_argument_group_opt,

		/**
		 * An argument group member.
		 *
		 * Defines a member of a preceeding optargs_argument_group or
		 * optargs_argument_group_opt.
		 *
		 * Must immediately follow a optargs_argument_group or
		 * optargs_argument_group_opt element.
		 *
		 * Can be given multiple times to define multiple arguments for
		 * the given group. An optargs_argument structure element of any
		 * other type will indicate the end of the
		 * optargs_argument_group's or optargs_argument_group_opt's
		 * members.
		 */
		optargs_argument_group_member,

		/**
		 * Defines a argument divider.
		 *
		 * See optargs_argument_divider convenience macro which is the preferred
		 * way of using this.
		 */
		_optargs_argument_divider,

		/**
		 * Defines a sink argument.
		 *
		 * See optargs_argument_sink convenience macro which is the preferred
		 * way of using this.
		 */
		_optargs_argument_sink,

		/**
		 * Defines an end of an argument list.
		 *
		 * See optargs_argument_eol convenience macro which is the preferred
		 * way of using this.
		 */
		_optargs_argument_eol
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
		 * by the user for this group of arguments.
		 *
		 * This will be used only for group types of arguments.
		 */
		struct optargs_argument * NULLABLE match;
	} result;

	/**
	 * A list of possible subarguments this argument might be expecting.
	 *
	 * The list must be terminated with an optargs_argument_eol of its own if
	 * defined. Must be left to NULL if no subarguments are expected.
	 */
	struct optargs_argument * NULLABLE subargument;
};

/**
 * Command line option structure.
 *
 * A list of these is used to define the names, descriptions and types of
 * the accepted options as well as to store the results of the option
 * parsing.
 */
struct optargs_option
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
	 * Should be left to 0 if no short form for this option is needed.
	 */
	char const short_option;

	/**
	 * Description of this option.
	 *
	 * Should be left to NULL if no description for this option is needed.
	 *
	 * An option without a description will not be shown in the help text
	 * and can be thought as an undocumented "secret option".
	 */
	char const * const NULLABLE description;

	/**
	 * The type of the result (i.e. which type from the below union should
	 * be used).
	 */
	enum optargs_option_value_type
	{
		/** This option has not been given/defined. */
		optargs_undef = 0,

		/** This option was given with an argument to it. */
		optargs_argument,

		/** This option was given one or more times without an argument. */
		optargs_flag
	} value_type : 8;

	union
	{
		/**
		 * A list of optargs_argument structures defining the argument for
		 * this option.
		 *
		 * Should be left to NULL if no argument is expected for this
		 * option.
		 *
		 * Supplying option arguments with subarguments is not supported.
		 *
		 * Supplying an optargs_argument_divider here (to an otherwise
		 * empty structure) will yield in a vertical empty space in help
		 * texts' option descriptions (see convenience macro
		 * optargs_option_divider above for this).
		 */
		struct optargs_argument * NULLABLE argument;

		/**
		 * The number of times this option was given by the user (without
		 * an option argument).
		 */
		unsigned long count;
	};
};

/**
 * Parse command line options from the given arguments.
 *
 * Arguments:
 *  argc:    Number of arguments in the argv array.
 *  argv:    An array of char * arguments to parse options from.
 *  options: An array of options to parse from the argv array. The array
 *           must be terminated with an optargs_option_eol element.
 *
 * Return value:
 *  The index number of the first non-option element (a "--" is not
 *  considered a non-option here) in the argv array or a negative number
 *  if an error was encountered. The return value equals to argc in case
 *  no non-option arguments were given.
 */
WARN_UNUSED_RESULT
int
optargs_parse_options(
		int argc,
		char const * NONNULL const * NONNULL const argv,
		struct optargs_option * NONNULL options
		);

/**
 * Parse command line arguments from the given set of char * arguments.
 *
 * Arguments:
 *  argc:      Number of arguments in the argv array.
 *  argv:      A list of char * arguments from where the arguments are to
 *             be parsed from.
 *  arguments: A list of optargs_argument structures defining the accepted
 *             arguments. The list should be terminated with an
 *             optargs_argument_eol element.
 *
 * Return value:
 *  The number of successfully parsed arguments (not counting what went
 *  into a sink) or a negative integer in case of an error.
 */
WARN_UNUSED_RESULT
int
optargs_parse_arguments(
		int argument_count,
		char const * NONNULL const * NONNULL const arguments,
		struct optargs_argument * NONNULL supported_arguments
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
 *  options:   A optargs_option_eol terminated array of supported options
 *             or NULL if none are supported.
 *  arguments: A optargs_argument_eol or optargs_argument_sink terminated
 *             array of supported arguments or NULL if none are supported.
 */
void
optargs_print_help(
		char const * NONNULL cmd,
		char const * NULLABLE about,
		struct optargs_option const * NULLABLE options,
		struct optargs_argument const * NULLABLE arguments
		);

/**
 * Find the result string for the option in the given index.
 *
 * Simply returns the (index + 1)th option's result (pointer) from the
 * given array. The given index number must match an element in the given
 * list of options.
 *
 * The type of the relevant option stucture must be optargs_string (or
 * optargs_undef).
 *
 * Arguments:
 *  option: A pointer to an option from where to fetch the desired string.
 *          The option should have previously been processed by the
 *          optargs_parse_options() function.
 *
 * Return value:
 *  A pointer to the result string from the matching option structure
 *  or NULL if the specified option was not given by the user.
 */
WARN_UNUSED_RESULT
char const * NULLABLE
optargs_option_string(
		struct optargs_option const * NONNULL option
		);

/**
 * Find the type for the option in the given index.
 *
 * Simply returns the (index + 1)th option's type from the given array.
 * The given index number must match an element in the given list of
 * options.
 *
 * Arguments:
 *  option: A pointer to an option from where to fetch its type.
 *          The option should have previously been processed by the
 *          optargs_parse_options() function.
 *
 * Return value:
 *  The type of the option's value.
 */
WARN_UNUSED_RESULT
enum optargs_option_value_type
optargs_option_type(
		struct optargs_option const * NONNULL opts
		);

/**
 * Find the count for the option in the given index.
 *
 * Simply returns the (index + 1)th option's count number (how many time
 * the option was given/defined) from the given array. The given index
 * number must match an element in the given list of options. The type of
 * the result struct must be optargs_flag (or optargs_undef).
 *
 * Arguments:
 *  option: A pointer to an option from where to fetch its type.
 *          The option should have previously been processed by the
 *          optargs_parse_options() function.
 *
 * Return value:
 *  The number of times the given option was given by the user (without an
 *  argument).
 */
WARN_UNUSED_RESULT
unsigned int
optargs_option_count(
		struct optargs_option const * NONNULL opts
		);

/**
 * Get the value of an argument.
 *
 * The user provided value for an argument is returned or NULL if the
 * argument was not specified by the user.
 *
 * Arguments:
 *   argument: The argument from which to get the value from. Must be of
 *             type optargs_argument_group, optargs_argument_group_opt,
 *             optargs_argument_any or optargs_argument_any_opt.
 *
 * Return value:
 *   The value of the argument or NULL if the argument was not defined by
 *   the user.
 */
WARN_UNUSED_RESULT
char const * NULLABLE
optargs_argument_value(
		struct optargs_argument const * NONNULL argument
		);

/**
 * Get the index offset from an optargs_argument_group (or
 * optargs_argument_group_opt) to the optargs_argument_group_member
 * element that was given by the user (the user provided matching input).
 *
 * I.e. if an argument group "command" contains the members "start" and
 * "stop" this function will return 1 or 2 for the  "command" group
 * element depending whether the user gave "start" or "stop" as an
 * argument for it.
 *
 * Arguments:
 *   argument: The optargs_argument_group (or optargs_argument_group_opt) element
 *             from which to calculate the distance to the user
 *             supplied optargs_argument_group_member element. The array
 *             should have previously been processed by the
 *             optargs_parse_arguments() function.
 *
 * Return value:
 *   The calculated distance (offset) or a negative value if the argument
 *   was not given by the user.
 */
WARN_UNUSED_RESULT
int
optargs_argument_offset(
		struct optargs_argument const * NONNULL argument
		);

/**
 * Get the (full) index of the optargs_argument_group_member array member
 * that was provided by the user as the value for the specified
 * optarg_arg_group (or optargs_argument_group_opt).
 *
 * Arguments:
 *   arguments:  The argument array from which to look for the group and
 *               group member arguments.
 *   base_index: The index of the group argument for which the sought
 *               group member argument belongs to.
 *
 * Return value:
 *   The index of the found optargs_argument_group_member element in the
 *   arguments array or NULL if the specified argument group element had
 *   no value associated to it.
 */
WARN_UNUSED_RESULT
int
optargs_argument_index(
		struct optargs_argument const * NONNULL arguments,
		int base_index
		);

// vim:noet:ts=4:sw=4:
