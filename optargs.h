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
 * A macro for initializing an all zero optargs_result structure.
 */
#define optargs_result_nil \
{\
	.type = 0,\
	.value = { 0 }\
}

/**
 * A macro for initializing an all zero optargs_arg structure.
 */
#define optargs_arg_nil \
{\
	.name = (void*)0,\
	.description = (void*)0,\
	.mandatory = 0\
}

/**
 * A macro for initializing an all zero optargs_opt structure.
 *
 * This can effectively be used to create an empty line into the option
 * help text.
 */
#define optargs_opt_nil \
{\
	.long_option = (void*)0,\
	.short_option = 0,\
	.description = (void*)0,\
	.argument = optargs_arg_nil,\
	.result = optargs_result_nil\
}

/**
 * A macro for initializing an "end of list" instance of struct optargs_arg.
 *
 * This can (and should) be used e.g. to indicate the end of an argument
 * array.
 */
#define optargs_arg_eol \
{\
	.name = (void*)0,\
	.description = (void*)0,\
	.mandatory = _optargs_eol\
}

/**
 * A macro for initializing an "end of list" instance of struct optargs_opt.
 *
 * This can (and should) be used e.g. to indicate the end of an option
 * array.
 */
#define optargs_opt_eol \
{\
	.long_option = (void*)0,\
	.short_option = 0,\
	.description = (void*)0,\
	.argument = optargs_arg_eol,\
	.result = optargs_result_nil\
}

/**
 * Argument result structure.
 *
 * Will be used to store and relay information about which
 * options/arguments were found from the parsed arguments and options.
 */
struct optargs_result
{
	/**
	 * The user supplied character pointer (argument) or the number of
	 * times an option was given without an user supplied argument.
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
	enum optargs_result_type
	{
		optargs_undef,
		optargs_string,
		optargs_count
	} type : 8;
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
	char const * const name;

	/**
	 * Description of this argument. If defined, this will be shown in the
	 * arguments section of the help text. This requires that the argument
	 * also has the name field defined. An argument with no description
	 * will become a subheader for a group of arguments (e.g. "COMMANDS").
	 */
	char const * const description;

	/**
	 * A multiple choice enumeration for the mandatoryness of the argument.
	 *
	 * Determines wheter the argument should be given, can be given or
	 * must not be given. Also affects how the argument's name (if
	 * defined) is show on the "usage line" of the help text.
	 *
	 * Defaults to optargs_no (i.e. an argument will not be accepted).
	 */
	enum optargs_multiple_choice
	{
		optargs_no,
		optargs_yes,
		optargs_maybe,
		_optargs_eol
	} mandatory : 8;
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
	 * Defines the possible argument for this option. This field must
	 * always be defined (along with its mandatory mandatory field). See
	 * struct optargs_arg's comments above for more information.
	 */
	struct optargs_arg argument;

	/**
	 * Will contain the result(s) of this option after parsing.
	 */
	struct optargs_result result;
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
 *  a negative number if an error was encountered. The return value equals
 *  to argc in case no non-option arguments were given.
 */
int
optargs_parse(int argc, char const * const * argv, struct optargs_opt * options);

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
struct optargs_result const *
optargs_result_by_long(struct optargs_opt const * options, char const * long_option);

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
struct optargs_result const *
optargs_result_by_short(struct optargs_opt const * options, char short_option);

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
struct optargs_result const *
optargs_result_by_index(struct optargs_opt const * opts, int index);

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
optargs_string_by_long(struct optargs_opt const * opts, char const * long_option);

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
optargs_string_by_short(struct optargs_opt const * opts, char short_option);

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
optargs_string_by_index(struct optargs_opt const * opts, int index);

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
optargs_count_by_long(struct optargs_opt const * opts, char const * long_option);

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
optargs_count_by_short(struct optargs_opt const * opts, char short_option);

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
optargs_count_by_index(struct optargs_opt const * opts, int index);

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
int
optargs_arg_index(struct optargs_arg const * arguments, char const * name);

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
enum optargs_result_type
optargs_result_type(struct optargs_result const * result);
