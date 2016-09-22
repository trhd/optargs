# optargs -- A command line option and argument management library.
# Copyright (C) 2016 Hemmo Nieminen
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#### Definitions, executables, etc. ######################################
                                                                         #
VALGRIND_ARGS = [ '--leak-check=full', '-v', '--show-leak-kinds=all', '--error-exitcode=1' ]

CONF_DATA = configuration_data()

MATCHER = executable(
	'matcher',
	'matcher.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

README = executable(
	'readme',
	'readme.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_1_PATH = meson.current_build_dir() + '/tester_1'
CONF_DATA.set('TESTER_1_PATH', TESTER_1_PATH)
TESTER_1 = executable(
	'tester_1',
	'tester_1.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_2_PATH = meson.current_build_dir() + '/tester_2'
CONF_DATA.set('TESTER_2_PATH', TESTER_2_PATH)
TESTER_2 = executable('tester_2',
	'tester_2.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_3_PATH = meson.current_build_dir() + '/tester_3'
CONF_DATA.set('TESTER_3_PATH', TESTER_3_PATH)
TESTER_3 = executable(
	'tester_3',
	'tester_3.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_4_PATH = meson.current_build_dir() + '/tester_4'
CONF_DATA.set('TESTER_4_PATH', TESTER_4_PATH)
TESTER_4 = executable(
	'tester_4',
	'tester_4.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_5_PATH = meson.current_build_dir() + '/tester_5'
TESTER_5 = executable(
	'tester_5',
	'tester_5.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_6_PATH = meson.current_build_dir() + '/tester_6-common_prefix'
TESTER_6 = executable(
	'tester_6-common_prefix',
	'tester_6-common_prefix.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_7_PATH = meson.current_build_dir() + '/tester_7-first_arg'
TESTER_7 = executable(
	'tester_7-first_arg',
	'tester_7-first_arg.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_8_PATH = meson.current_build_dir() + '/tester_8-ignore_hidden_option_length'
CONF_DATA.set('TESTER_8_PATH', TESTER_8_PATH)
TESTER_8 = executable(
	'tester_8-ignore_hidden_option_length',
	'tester_8-ignore_hidden_option_length.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

TESTER_9_PATH = meson.current_build_dir() + '/tester_9-count_options'
TESTER_9 = executable(
	'tester_9-count_options',
	'tester_9-count_options.c',
	link_with : OPTARGS_LIB,
	include_directories : OPTARGS_HDR
)

configure_file(
	input : 'help_text_sample_1.txt.in',
	output : 'help_text_sample_1.txt',
	configuration : CONF_DATA
)

configure_file(
	input : 'help_text_sample_2.txt.in',
	output : 'help_text_sample_2.txt',
	configuration : CONF_DATA
)

configure_file(
	input : 'help_text_sample_3.txt.in',
	output : 'help_text_sample_3.txt',
	configuration : CONF_DATA
)

configure_file(
	input : 'help_text_sample_4.txt.in',
	output : 'help_text_sample_4.txt',
	configuration : CONF_DATA
)

configure_file(
	input : 'help_text_sample_8.txt.in',
	output : 'help_text_sample_8.txt',
	configuration : CONF_DATA
)

SHORT_OPT = '-@0@'
SHORT_OPT_EQ = '-@0@=@1@'
SHORT_OPT_APPEND = '-@0@@1@'

LONG_OPT = '--@0@@0@@0@@0@'
LONG_OPT_EQ = '--@0@@0@@0@@0@=@1@'
LONG_OPT_APPEND = '--@0@@0@@0@@0@@1@'

A_STRING = 'This"is#some=random\'string-with|funny`characters.'
                                                                         #
#### Give a single option. ###############################################
                                                                         #
test('Ensure short option -a works on its own.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '-a' ])
test('Ensure short option -b works on its own.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '-b' ])
test('Ensure short option -c works on its own.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '-c' ])
test('Ensure short option -d works on its own.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '-d' ])
                                                                         #
#### Give a single long option. ##########################################
                                                                         #
test('Ensure long option --aaaa works on its own.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '--aaaa' ])
test('Ensure long option --bbbb works on its own.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '--bbbb' ])
test('Ensure long option --cccc works on its own.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '--cccc' ])
test('Ensure long option --dddd works on its own.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '--dddd' ])
                                                                         #
# Specify a short option twice in a single argument . ####################
                                                                         #
test('Ensure short option -a can be given twice in one argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '-aa' ])
test('Ensure short option -b can be given twice in one argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '-bb' ])
test('Ensure short option -c can be given twice in one argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '-cc' ])
test('Ensure short option -d can be given twice in one argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '-dd' ])
                                                                         #
#### Specify an option twice. ############################################
                                                                         #
test('Ensure short option -a can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '-a', '-a' ])
test('Ensure short option -b can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '-b', '-b' ])
test('Ensure short option -c can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '-c', '-c' ])
test('Ensure short option -d can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '-d', '-d' ])

test('Ensure long option --aaaa can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '--aaaa', '--aaaa' ])
test('Ensure long option --bbbb can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '--bbbb', '--bbbb' ])
test('Ensure long option --cccc can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '--cccc', '--cccc' ])
test('Ensure long option --dddd can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '--dddd', '--dddd' ])
                                                                         #
#### Specify a single short option four times. ###########################
                                                                         #
test('Ensure short option -a can be given four times.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '-a', '-a', '-a', '-a' ])
test('Ensure short option -b can be given four times.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '-b', '-b', '-b', '-b' ])
test('Ensure short option -c can be given four times.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '-c', '-c', '-c', '-c' ])
test('Ensure short option -d can be given four times.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '-d', '-d', '-d', '-d' ])
                                                                         #
#### Specify a single long option four times. ############################
                                                                         #
test('Ensure long option --aaaa can be given four times.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '--aaaa', '--aaaa', '--aaaa', '--aaaa' ])
test('Ensure long option --bbbb can be given four times.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '--bbbb', '--bbbb', '--bbbb', '--bbbb' ])
test('Ensure long option --cccc can be given four times.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '--cccc', '--cccc', '--cccc', '--cccc' ])
test('Ensure long option --dddd can be given four times.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '--dddd', '--dddd', '--dddd', '--dddd' ])
                                                                         #
#### Specify single short option four times in a single argument. ########
                                                                         #
test('Ensure short option -a can be given four times in one argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '-aaaa' ])
test('Ensure short option -b can be given four times in one argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '-bbbb' ])
test('Ensure short option -c can be given four times in one argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '-cccc' ])
test('Ensure short option -d can be given four times in one argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '-dddd' ])
                                                                         #
#### Mix identical double and single short options. ######################
                                                                         #
test('Mix long option --aaaa and -a short option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '--aaaa', '-a', '--aaaa', '-a' ])
test('Mix long option --bbbb and -b short option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '--bbbb', '-b', '--bbbb', '-b' ])
test('Mix long option --cccc and -c short option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '--cccc', '-c', '--cccc', '-c' ])
test('Mix long option --dddd and -d short option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '--dddd', '-d', '--dddd', '-d' ])
                                                                         #
#### Mix different short options. ########################################
                                                                         #
test('Mix -a and -b short options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ab', TESTER_1_PATH, '-a', '-b' ])
test('Mix -a, -cccc and -b short options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abc', TESTER_1_PATH, '-a', '-cccc', '-b' ])

test('Mix -a, -b, -c and -d options #1.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '-a', '-b', '-c', '-d' ])
test('Mix -a, -b, -c and -d options #2.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '-b', '-c', '-d', '-a' ])
test('Mix -a, -b, -c and -d options #3.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '-b', '-a', '-d', '-c' ])
test('Mix -a, -b, -c and -d options #4.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '-bb', '-a', '-ddddd', '-c', '-a' ])
                                                                         #
#### Try to give an argument for a non-argument short option. ############
                                                                         #
test('Try -a short option with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a,foobar,barfoo', TESTER_1_PATH, '-a', 'foobar', 'barfoo' ])
test('Try -b short option with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b,foobar,barfoo', TESTER_1_PATH, '-b', 'foobar', 'barfoo' ])
test('Try -c short option with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c,foobar,barfoo', TESTER_1_PATH, '-c', 'foobar', 'barfoo' ])
test('Try -d short option with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d,foobar,barfoo', TESTER_1_PATH, '-d', 'foobar', 'barfoo' ])
                                                                         #
#### Try to give an argument for a non-argument long option. ############
                                                                         #
test('Try --aaaa long option with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a,foobar,barfoo', TESTER_1_PATH, '--aaaa', 'foobar', 'barfoo' ])
test('Try --bbbb long option with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b,foobar,barfoo', TESTER_1_PATH, '--bbbb', 'foobar', 'barfoo' ])
test('Try --cccc long option with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c,foobar,barfoo', TESTER_1_PATH, '--cccc', 'foobar', 'barfoo' ])
test('Try --dddd long option with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d,foobar,barfoo', TESTER_1_PATH, '--dddd', 'foobar', 'barfoo' ])
                                                                         #
#### Try to give an argument for a non-argument short option twice. ######
                                                                         #
test('Try -a short option twice with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a,foobar,-a,barfoo', TESTER_1_PATH, '-a', 'foobar', '-a', 'barfoo' ])
test('Try -b short option twice with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b,foobar,-b,barfoo', TESTER_1_PATH, '-b', 'foobar', '-b', 'barfoo' ])
test('Try -c short option twice with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c,foobar,-c,barfoo', TESTER_1_PATH, '-c', 'foobar', '-c', 'barfoo' ])
test('Try -d short option twice with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d,foobar,-d,barfoo', TESTER_1_PATH, '-d', 'foobar', '-d', 'barfoo' ])
                                                                         #
#### Try to give an argument for a non-argument long option twice. #######
                                                                         #
test('Try --aaaa long option twice with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a,foobar,--aaaa,barfoo', TESTER_1_PATH, '--aaaa', 'foobar', '--aaaa', 'barfoo' ])
test('Try --bbbb long option twice with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b,foobar,--bbbb,barfoo', TESTER_1_PATH, '--bbbb', 'foobar', '--bbbb', 'barfoo' ])
test('Try --cccc long option twice with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c,foobar,--cccc,barfoo', TESTER_1_PATH, '--cccc', 'foobar', '--cccc', 'barfoo' ])
test('Try --dddd long option twice with arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d,foobar,--dddd,barfoo', TESTER_1_PATH, '--dddd', 'foobar', '--dddd', 'barfoo' ])
                                                                         #
#### Ensure empty argument is ignored before options. ####################
                                                                         #
test('Empty argument before short -a option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '', '-a' ])
test('Empty argument before short -b option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '', '-b' ])
test('Empty argument before short -c option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '', '-c' ])
test('Empty argument before short -d option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '', '-d' ])

test('Empty argument before long --aaaa option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a', TESTER_1_PATH, '', '--aaaa' ])
test('Empty argument before long --bbbb option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b', TESTER_1_PATH, '', '--bbbb' ])
test('Empty argument before long --cccc option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: c', TESTER_1_PATH, '', '--cccc' ])
test('Empty argument before long --dddd option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: d', TESTER_1_PATH, '', '--dddd' ])
                                                                         #
#### Ensure empty argument is ignored between options. ###################
                                                                         #
test('Empty argument between short -b and -a options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ab', TESTER_1_PATH, '-b', '', '-a' ])
test('Empty argument between short -b and -c options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: bc', TESTER_1_PATH, '-b', '', '-c' ])
test('Empty argument between short -c and -a options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ac', TESTER_1_PATH, '-c', '', '-a' ])
test('Empty argument between short -a and -d options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ad', TESTER_1_PATH, '-a', '', '-d' ])

test('Empty argument between long --aaaa and --cccc options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ac', TESTER_1_PATH, '--aaaa', '', '--cccc' ])
test('Empty argument between long --bbbb and --aaaa options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ab', TESTER_1_PATH, '--bbbb', '', '--aaaa' ])
test('Empty argument between long --bbbb and --cccc options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: bc', TESTER_1_PATH, '--bbbb', '', '--cccc' ])
test('Empty argument between long --aaaa and --dddd options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ad', TESTER_1_PATH, '--aaaa', '', '--dddd' ])

test('Empty argument between long --aaaa and short -c options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ac', TESTER_1_PATH, '--aaaa', '', '-c' ])
test('Empty argument between short -b and long --aaaa options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ab', TESTER_1_PATH, '-b', '', '--aaaa' ])
test('Empty argument between long --bbbb and short -c options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: bc', TESTER_1_PATH, '--bbbb', '', '-c' ])
test('Empty argument between short -a and long --dddd options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ad', TESTER_1_PATH, '-a', '', '--dddd' ])
                                                                         #
#### Ensure empty argument is ignored between options and arguments. #####
                                                                         #
test('Empty argument between long --aaaa and argument foobar.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a,foobar', TESTER_1_PATH, '--aaaa', '', 'foobar' ])
test('Empty argument between short -b and argument foobar.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b,foobar', TESTER_1_PATH, '-b', '', 'foobar' ])
test('Empty argument between long --bbbb and argument foobar.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: b,foobar', TESTER_1_PATH, '--bbbb', '', 'foobar' ])
test('Empty argument between short -a and argument foobar.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: a,foobar', TESTER_1_PATH, '-a', '', 'foobar' ])
                                                                         #
#### Mix simple short an long options. ###################################
                                                                         #
test('Mix a, b, c, and d long and short options #1.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '--aaaa', '--bbbb', '--cccc', '--dddd' ])
test('Mix a, b, c, and d long and short options #2.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '--bbbb', '--cccc', '-da' ])
test('Mix a, b, c, and d long and short options #3.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '--bbbb', '-a', '--dddd', '--aaaa', '--cccc' ])
test('Mix a, b, c, and d long and short options #4.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '-b', '--aaaa', '-d', '-c', '--aaaa' ])
test('Mix a, b, c, and d long and short options #5.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcd', TESTER_1_PATH, '-abcd', '--aaaa', '--bbbb', '--cccc', '--dddd' ])
                                                                         #
#### Test mandatory argument handling. ###################################
                                                                         #
test('Try -i short option with mandatory argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar}', TESTER_1_PATH, '-i', 'foobar' ])
test('Try -i short option with mandatory argument in the same argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar}', TESTER_1_PATH, '-ifoobar' ])
test('Try -i short option with assigned mandatory argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar}', TESTER_1_PATH, '-i=foobar' ])
test('Try --iiii long option with mandatory argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar}', TESTER_1_PATH, '--iiii', 'foobar' ])
test('Try --iiii long option with assigned mandatory argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar}', TESTER_1_PATH, '--iiii=foobar' ])
                                                                         #
#### Test mandatory argument handling with extra argument. ###############
                                                                         #
test('Try -i short option with mandatory argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '-i', 'foobar', 'barfoo' ])
test('Try -i short option with mandatory argument in the same argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '-ifoobar', 'barfoo' ])
test('Try -i short option with assigned mandatory argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '-i=foobar', 'barfoo' ])
test('Try --iiii long option with mandatory argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '--iiii', 'foobar', 'barfoo' ])
test('Try --iiii long option with assigned mandatory argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '--iiii=foobar', 'barfoo' ])
                                                                         #
#### Test mandatory argument handling with extra options. ################
                                                                         #
test('Try -i short option with mandatory argument and -ab short options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abi{foobar}', TESTER_1_PATH, '-i', 'foobar', '-ab' ])
test('Try -i short option with mandatory argument in the same argument and -a and -b short options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abi{foobar}', TESTER_1_PATH, '-b', '-ifoobar', '-a' ])
test('Try -i short option with assigned mandatory argument and --cccc long option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: ci{foobar}', TESTER_1_PATH, '-i=foobar', '--cccc' ])
test('Try --iiii long option with mandatory argument and --bbbb and -a options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: aci{foobar}', TESTER_1_PATH, '--iiii', 'foobar', '-a', '--cccc' ])
test('Try --iiii long option with assigned mandatory argument and -abc options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abci{foobar}', TESTER_1_PATH, '-abc', '--iiii=foobar', '-abc' ])
                                                                         #
#### Test mandatory argument handling with extra options and arguments. ##
                                                                         #
test('Try -i short option with mandatory argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abci{foobar},barfoo,--bbbb', TESTER_1_PATH, '-abc', '-ifoobar', '-abc', 'barfoo', '--bbbb' ])
test('Try -i short option with mandatory argument in the same argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abci{foobar},barfoo,--bbbb', TESTER_1_PATH, '-abc', '-i', 'foobar', '-abc', 'barfoo', '--bbbb' ])
test('Try -i short option with assigned mandatory argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abci{foobar},barfoo,--bbbb', TESTER_1_PATH, '-abc', '-i=foobar', '-abc', 'barfoo', '--bbbb' ])
test('Try --iiii long option with mandatory argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abci{foobar},barfoo,--bbbb', TESTER_1_PATH, '-abc', '--iiii', 'foobar', '-abc', 'barfoo', '--bbbb' ])
test('Try --iiii long option with assigned mandatory argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abci{foobar},barfoo,--bbbb', TESTER_1_PATH, '-abc', '--iiii=foobar', '-abc', 'barfoo', '--bbbb' ])
                                                                         #
#### Test conflicting mandatory arguments. ###############################
                                                                         #
test('Try -i short option twice with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i', 'foobar', '-i', 'barfoo' ])
test('Try -i short option assigned twice with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i=foobar', '-i=barfoo' ])
test('Try -i short option appended twice with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-ifoobar', '-ibarfoo' ])
test('Try -i short option assigned and appended with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i=foobar', '-ibarfoo' ])
test('Try -i short option appended and assigned with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-ifoobar', '-i=barfoo' ])

test('Try --iiii long option twice with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiii', 'foobar', '--iiii', 'barfoo' ])
test('Try --iiii long option assigned twice with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiii=foobar', '--iiii=barfoo' ])

test('Try short -i option with long --iiii option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i', 'foobar', '--iiii', 'barfoo' ])
test('Try assigned short -i option with long --iiii option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i=foobar', '--iiii', 'barfoo' ])
test('Try short -i option with assigned long --iiii option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i', 'foobar', '--iiii=barfoo' ])

test('Try long --iiii option with short -i option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiii', 'foobar', '-i', 'barfoo' ])
test('Try assigned long --iiii option with short -i option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiii=foobar', '-i', 'barfoo' ])
test('Try long --iiii option with assigned shot -i option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiii', 'foobar', '-i=barfoo' ])

test('Try --iiii long option twice with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiii', 'foobar', '--iiii', 'barfoo' ])
test('Try short -i option with long --iiii option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i', 'foobar', '--iiii', 'barfoo' ])
test('Try long --iiii option with short -i option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiii', 'foobar', '-i', 'barfoo' ])
                                                                         #
#### Test conflicting mandatory arguments with extra options. ############
                                                                         #
test('Try -i short option twice with conflicting arguments and extra options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i', 'foobar', '-c', '-i', 'barfoo', '-d' ])
test('Try --iiii long option twice with conflicting arguments and extra options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-abc', '--iiii', 'foobar', '--iiii', 'barfoo' ])
test('Try short -i option with long --iiii option with conflicting arguments and extra options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--bbbb', '-i', 'foobar', '-a', '--iiii', 'barfoo' ])
test('Try long --iiii option with short -i option with conflicting arguments and extra options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-b', '--iiii', 'foobar', '--cccc', '-i', 'barfoo' ])
                                                                         #
#### Test optional argument handling. ####################################
                                                                         #
test('Try -j short option without optional argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!}', TESTER_1_PATH, '-j' ])
test('Try -j short option with optional argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar', TESTER_1_PATH, '-j', 'foobar' ])
test('Try -j short option with optional argument in the same argument.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-jfoobar' ])
test('Try -j short option with assigned optional argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{foobar}', TESTER_1_PATH, '-j=foobar' ])
test('Try --jjjj long option without optional argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!}', TESTER_1_PATH, '--jjjj' ])
test('Try --jjjj long option with optional argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar', TESTER_1_PATH, '--jjjj', 'foobar' ])
test('Try --jjjj long option with assigned optional argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{foobar}', TESTER_1_PATH, '--jjjj=foobar' ])
                                                                         #
#### Test optional argument handling with extra argument. ################
                                                                         #
test('Try -j short option with optional argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar,barfoo', TESTER_1_PATH, '-j', 'foobar', 'barfoo' ])
test('Try -j short option with optional argument in the same argument and extra argument.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-jfoobar', 'barfoo' ])
test('Try -j short option with assigned optional argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{foobar},barfoo', TESTER_1_PATH, '-j=foobar', 'barfoo' ])
test('Try --jjjj long option with optional argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar,barfoo', TESTER_1_PATH, '--jjjj', 'foobar', 'barfoo' ])
test('Try --jjjj long option with assigned optional argument and extra argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{foobar},barfoo', TESTER_1_PATH, '--jjjj=foobar', 'barfoo' ])
                                                                         #
#### Test optional argument handling with extra options. #################
                                                                         #
test('Try -j short option with assigned optional argument and -ab short options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abj{foobar}', TESTER_1_PATH, '-j=foobar', '-ab' ])
test('Try -j short option with default value and -a and -b short options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abj{Oujea!}', TESTER_1_PATH, '-b', '-j', '-a' ])
test('Try --jjjj long option with default value and -a and -b short options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abj{Oujea!}', TESTER_1_PATH, '-b', '--jjjj', '-a' ])
test('Try -j short option with assigned optional argument and --cccc long option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: cj{foobar}', TESTER_1_PATH, '-j=foobar', '--cccc' ])
test('Try --jjjj long option with assigned optional argument and --bbbb and -a options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: acj{foobar}', TESTER_1_PATH, '--jjjj=foobar', '-a', '--cccc' ])
test('Try --jjjj long option with assigned optional argument and -abc options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcj{foobar}', TESTER_1_PATH, '-abc', '--jjjj=foobar', '-abc' ])
                                                                         #
#### Test optional argument handling with extra options and arguments. ###
                                                                         #
test('Try -j short option with default value and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcj{Oujea!},barfoo,--bbbb', TESTER_1_PATH, '-abc', '-j', '-abc', 'barfoo', '--bbbb' ])
test('Try -j short option with optional argument in the same argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcj{Oujea!},foobar,-abc,barfoo,--bbbb', TESTER_1_PATH, '-abc', '-j', 'foobar', '-abc', 'barfoo', '--bbbb' ])
test('Try -j short option with assigned optional argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcj{foobar},barfoo,--bbbb', TESTER_1_PATH, '-abc', '-j=foobar', '-abc', 'barfoo', '--bbbb' ])
test('Try --jjjj long option with optional argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcj{Oujea!},foobar,-abc,barfoo,--bbbb', TESTER_1_PATH, '-abc', '--jjjj', 'foobar', '-abc', 'barfoo', '--bbbb' ])
test('Try --jjjj long option with assigned optional argument and extra options and arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abcj{foobar},barfoo,--bbbb', TESTER_1_PATH, '-abc', '--jjjj=foobar', '-abc', 'barfoo', '--bbbb' ])
                                                                         #
#### Test conflicting optional arguments. ################################
                                                                         #
test('Try -j short option twice with conflicting arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar,-j,barfoo', TESTER_1_PATH, '-j', 'foobar', '-j', 'barfoo' ])
test('Try -j short option assigned twice with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-j=foobar', '-j=barfoo' ])

test('Try --jjjj long option twice with conflicting arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar,--jjjj,barfoo', TESTER_1_PATH, '--jjjj', 'foobar', '--jjjj', 'barfoo' ])
test('Try --jjjj long option assigned twice with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--jjjj=foobar', '--jjjj=barfoo' ])

test('Try assigned options short -j and long --jjjj with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-j=foobar', '--jjjj=barfoo' ])
test('Try assigned options long --jjjj and short -j with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--jjjj=foobar', '-j=barfoo' ])
                                                                         #
#### Test conflicting optional arguments with extra options. #############
                                                                         #
test('Try assigned -j short option twice with conflicting arguments and extra options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-j=foobar', '-c', '-j=barfoo', '-d' ])
test('Try assigned --jjjj long option twice with conflicting arguments and extra options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-abc', '--jjjj=foobar', '--jjjj=barfoo' ])
test('Try assigned short -j option with assigned long --jjjj option with conflicting arguments and extra options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--bbbb', '-j=foobar', '-a', '--jjjj=barfoo' ])
test('Try assigned long --jjjj option with assigned short -j option with conflicting arguments and extra options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-b', '--jjjj=foobar', '--cccc', '-j=barfoo' ])
                                                                         #
#### Test conflicting optional arguments and default values. #############
                                                                         #
test('Try -j short option with conflicting default value.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-j=foobar', '-j' ])
test('Try -j short option with conflicting default value (reverse).',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-j', '-j=foobar'])
test('Try --jjjj long option with conflicting default value #1.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--jjjj=foobar', '-j' ])
test('Try --jjjj long option with conflicting default value #2.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--jjjj=foobar', '--jjjj' ])
test('Try assigned long --jjjj option with assigned short -j option with conflicting arguments.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--jjjj=foobar', '-j=barfoo' ])
test('Try --jjjj long option with default value with conflicting argument.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--jjjj', '-j=foobar'])
test('Try --jjjj long option with default value with conflicting argument (reverse).',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-j=foobar', '--jjjj'])
test('Try --jjjj long option with conflicting default value (reverse) #2.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--jjjj', '--jjjj=foobar'])
                                                                         #
#### Ensure separate argument is handled properly. #######################
                                                                         #
test('Give foobar as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '-i', 'foobar', 'barfoo' ])
test('Give foobar as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar,barfoo', TESTER_1_PATH, '-j', 'foobar', 'barfoo' ])
test('Give foobar as an optional argument for short option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '--iiii', 'foobar', 'barfoo' ])
test('Give foobar as an optional argument for short option --jjjj.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar,barfoo', TESTER_1_PATH, '--jjjj', 'foobar', 'barfoo' ])
                                                                         #
#### Ensure appended arguments are handled correctly. ####################
                                                                         #
test('Appending an argument for the long option --iiii should fail.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiiifoobar'])
test('Appending an (mandatory) argument for the short option -i should succeed.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar}', TESTER_1_PATH, '-ifoobar' ])
test('Appending an (optional) argument for the short option -j should fail.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-jfoobar' ])
test('Append an argument for the long option --jjjj.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--jjjjfoobar'])
                                                                         #
#### Ensure assigning a value works. #####################################
                                                                         #
test('Assign "foobar" as mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '-i=foobar', 'barfoo' ])
test('Assign "foobar" as optional argument for short option -j.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{foobar},barfoo', TESTER_1_PATH, '-j=foobar', 'barfoo' ])
test('Assign "foobar" as mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH, '--iiii=foobar', 'barfoo' ])
test('Assign "foobar" as optional argument for long option --jjjj.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{foobar},barfoo', TESTER_1_PATH, '--jjjj=foobar', 'barfoo' ])
                                                                         #
#### Ensure assigning empty argument results in an empty value. ##########
                                                                         #
test('Assign empty argument as mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{},foobar', TESTER_1_PATH, '-i=', 'foobar' ])
test('Assign empty argument as optional argument for short option -j.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{},foobar', TESTER_1_PATH, '-j=', 'foobar' ])
test('Assign empty argument as mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{},foobar', TESTER_1_PATH, '--iiii=', 'foobar' ])
test('Assign empty argument as optional argument for long option --jjjj.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{},foobar', TESTER_1_PATH, '--jjjj=', 'foobar' ])
                                                                         #
#### Ensure an empty argument can be given (as mandatory argument).#######
                                                                         #
test('Give an empty argument as mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{},foobar', TESTER_1_PATH, '-i', '', 'foobar' ])
test('Give an empty argument as mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{},foobar', TESTER_1_PATH, '--iiii', '', 'foobar' ])
                                                                         #
#### Ensure an empty argument is ignored (as optional argument).##########
                                                                         #
test('Give an empty argument as optional argument for short option -j.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar', TESTER_1_PATH, '-j', '', 'foobar' ])
test('Give an empty argument as optional argument for long option --jjjj.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{Oujea!},foobar', TESTER_1_PATH, '--jjjj', '', 'foobar' ])
                                                                         #
#### Use an option as an argument. #######################################
                                                                         #
test('Give --foobar as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--foobar},barfoo', TESTER_1_PATH, '-i', '--foobar', 'barfoo' ])
test('Give --foobar as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--foobar},barfoo', TESTER_1_PATH, '--iiii', '--foobar', 'barfoo' ])

test('Give -F as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-F},barfoo', TESTER_1_PATH, '-i', '-F', 'barfoo' ])
test('Give -F as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-F},barfoo', TESTER_1_PATH, '--iiii', '-F', 'barfoo' ])

test('Assign -F as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-F},barfoo', TESTER_1_PATH, '-i=-F', 'barfoo' ])
test('Assign -F as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-F},barfoo', TESTER_1_PATH, '--iiii=-F', 'barfoo' ])

test('Append --foobar as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--foobar},barfoo', TESTER_1_PATH, '-i--foobar', 'barfoo' ])
test('Append -F as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-F},barfoo', TESTER_1_PATH, '-i-F', 'barfoo' ])

test('Assign -F as an optional argument for short option -j.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{-F},barfoo', TESTER_1_PATH, '-j=-F', 'barfoo' ])
test('Assign -F as an optional argument for long option --jjjj.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{-F},barfoo', TESTER_1_PATH, '--jjjj=-F', 'barfoo' ])
                                                                         #
#### Use an non-existing option as an argument. ##########################
                                                                         #
test('Give --xxxx as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--xxxx},barfoo', TESTER_1_PATH, '-i', '--xxxx', 'barfoo' ])
test('Give --xxxx as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--xxxx},barfoo', TESTER_1_PATH, '--iiii', '--xxxx', 'barfoo' ])

test('Give -x as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-x},barfoo', TESTER_1_PATH, '-i', '-x', 'barfoo' ])
test('Give -x as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-x},barfoo', TESTER_1_PATH, '--iiii', '-x', 'barfoo' ])

test('Assign -x as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-x},barfoo', TESTER_1_PATH, '-i=-x', 'barfoo' ])
test('Assign -x as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-x},barfoo', TESTER_1_PATH, '--iiii=-x', 'barfoo' ])

test('Append --xxxx as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--xxxx},barfoo', TESTER_1_PATH, '-i--xxxx', 'barfoo' ])
test('Append -x as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-x},barfoo', TESTER_1_PATH, '-i-x', 'barfoo' ])

test('Assign -x as an optional argument for short option -j.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{-x},barfoo', TESTER_1_PATH, '-j=-x', 'barfoo' ])
test('Assign -x as an optional argument for long option --jjjj.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{-x},barfoo', TESTER_1_PATH, '--jjjj=-x', 'barfoo' ])
                                                                         #
#### Use dash / double dash as an argument. ##############################
                                                                         #
test('Give -- as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--},barfoo', TESTER_1_PATH, '-i', '--', 'barfoo' ])
test('Give -- as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--},barfoo', TESTER_1_PATH, '--iiii', '--', 'barfoo' ])

test('Give - as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-},barfoo', TESTER_1_PATH, '-i', '-', 'barfoo' ])
test('Give - as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-},barfoo', TESTER_1_PATH, '--iiii', '-', 'barfoo' ])

test('Assign - as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-},barfoo', TESTER_1_PATH, '-i=-', 'barfoo' ])
test('Assign - as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-},barfoo', TESTER_1_PATH, '--iiii=-', 'barfoo' ])

test('Append -- as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--},barfoo', TESTER_1_PATH, '-i--', 'barfoo' ])
test('Append - as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-},barfoo', TESTER_1_PATH, '-i-', 'barfoo' ])

test('Assign - as an optional argument for short option -j.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{-},barfoo', TESTER_1_PATH, '-j=-', 'barfoo' ])
test('Assign - as an optional argument for long option --jjjj.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{-},barfoo', TESTER_1_PATH, '--jjjj=-', 'barfoo' ])
                                                                         #
#### Use A_STRING as an argument. ########################################
                                                                         #
test('Give A_STRING as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--},barfoo', TESTER_1_PATH, '-i', '--', 'barfoo' ])
test('Give A_STRING as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--},barfoo', TESTER_1_PATH, '--iiii', '--', 'barfoo' ])

test('Assign - as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-},barfoo', TESTER_1_PATH, '-i=-', 'barfoo' ])
test('Assign - as a mandatory argument for long option --iiii.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-},barfoo', TESTER_1_PATH, '--iiii=-', 'barfoo' ])

test('Append -- as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{--},barfoo', TESTER_1_PATH, '-i--', 'barfoo' ])
test('Append - as a mandatory argument for short option -i.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{-},barfoo', TESTER_1_PATH, '-i-', 'barfoo' ])

test('Assign - as an optional argument for short option -j.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{-},barfoo', TESTER_1_PATH, '-j=-', 'barfoo' ])
test('Assign - as an optional argument for long option --jjjj.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: j{-},barfoo', TESTER_1_PATH, '--jjjj=-', 'barfoo' ])
                                                                         #
#### Test invalid options. ###############################################
                                                                         #
test('Ensure assigning an argument to a regular long option --aaaa fails.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--aaaa=foobar'])
test('Ensure assigning an argument to a regular short option -a fails.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-a=foobar'])
test('Ensure extra dashes in long option causes an error.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '---aaaa'])
test('Ensure non-existing short option -x causes an error.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-x'])
test('Ensure non-existing long option --xxxx causes an error.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--xxxx'])
test('Ensure non-existing short option -x causes an error when mixed with other options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-a', '-bc', '-x', '-d'])
test('Ensure non-existing short option -x causes an error when mixed with other options #2.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-abxcd'])
test('Ensure non-existing long option --xxxx causes an error when mixed with other options.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-ab', '--cccc', '--xxxx', '-d'])
test('Ensure a single dash option causes an error #1.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-'])
test('Ensure a single dash option causes an error #2.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-', '--'])
test('Ensure a single dash option causes an error #3.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-ab', '-', '--cccc'])
test('Ensure a single dash option causes an error #4.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--aaaa', '-', '-cd'])
test('Ensure appending a dash to a short option causes an error.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-a-'])
test('Ensure appending two short options together causes an error.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-a-b'])
test('Ensure mixing a dash into a combined option argument causes an error.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-ab-cd'])
                                                                         #
#### Test missing arguments. #############################################
                                                                         #
test('Ensure missing mandatory argument from short option -i causes an error.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '-i' ])
test('Ensure missing mandatory argument from long option --iiii causes an error.',
	TESTER_1, valgrind_args : VALGRIND_ARGS, should_fail : true,
	args : [ '--iiii' ])
                                                                         #
#### Test double hyphen. #################################################
                                                                         #
foreach my_args : [
			[ 'a,-b', [ '-a', '--', '-b' ]],
			[ 'ab,-cd', [ '-ab', '--', '-cd' ]],
			[ 'a,--bbbb', [ '--aaaa', '--', '--bbbb' ]],
			[ 'ac,--bbbb,-d', [ '--aaaa', '-c', '--', '--bbbb', '-d' ]],
			[ 'i{foobar},--jjjj=barfoo', [ '--iiii', 'foobar', '--', '--jjjj=barfoo' ]],
			[ 'abc,A,-d,--,-ab', [ '-a', '-bc', 'A', '-d', '--', '-ab' ]],
			[ ',A,--iiii,foobar,--,--jjjj,barfoo', [ 'A', '--iiii', 'foobar', '--', '--jjjj', 'barfoo' ]],
		]
	test(
		'Ensure double dash stops option processing: @0@'.format(my_args.get(1)),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: @0@'.format(my_args.get(0)), TESTER_1_PATH ] + my_args.get(1)
	)
endforeach
                                                                         #
#### Identify first non-option argument. #################################
                                                                         #
foreach my_args : [
			[ 'foobar', [ 'foobar' ]],
			[ 'foobar', [ '-a', 'foobar' ]],
			[ 'foobar', [ '-a', '-b', 'foobar' ]],
			[ 'foobar', [ '-a', '-b', '-c', 'foobar' ]],
			[ '-c', [ '-a', '-b', '--', '-c', 'foobar' ]],
			[ '--', [ '-a', '-b', '-c', '-d', 'foobar' ]],
			[ 'foobar', [ '-a', '-b', '-c', '-d', 'keke', 'foobar' ]],
			[ 'foobar', [ '-a', '-d', 'keke', '-b', '-c', 'foobar' ]],
			[ '--', [ '-a', '-b', '-c', '-e=foobar' ]],
			[ '--', [ '-a', '-b', '-c', '-e' ]],
			[ '-c', [ '-a', '-b', '--', '-c', '-e' ]],
			[ '--', [ ]],
			[ '---aaaa', [ '--bbbb', '--', '---aaaa', '-c' ]],
		]
	test(
		'First non-option argument in @0@ should be @1@'.format(my_args.get(1), my_args.get(0)),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'First argument: @0@'.format(my_args.get(0)), TESTER_7_PATH ] + my_args.get(1)
	)
endforeach
                                                                         #
#### Test "hidden option. ################################################
                                                                         #
test('Ensure hidden option --hidden-option is recognized.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: hidden!', TESTER_1_PATH, '--hidden-option' ])
test('Ensure hidden option --hidden-option is recognized even with other options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abchidden!', TESTER_1_PATH, '-ac', '--hidden-option', '--bbbb' ])
test('Ensure hidden option --hidden-option can be given twice.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abchidden!', TESTER_1_PATH, '-ac', '--hidden-option', '--bbbb', '--hidden-option' ])
                                                                         #
#### Test options with common prefix. ####################################
                                                                         #
test('Ensure a long option is recognized even if it matches the prefix of another option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'got basic', TESTER_6_PATH, '--help' ])
test('Ensure a long option is recognized even if it matches the prefix of another option (inverse).',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ '--fail-match', 'got full', TESTER_6_PATH, '--help' ])
test('Ensure a long option is recognized when its prefix matches another option.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'got full', TESTER_6_PATH, '--help-full' ])
test('Ensure a long option is recognized when its prefix matches another option (inverse).',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ '--fail-match', 'got basic', TESTER_6_PATH, '--help-full' ])
                                                                         #
#### Test (non option) argument handling. ################################
                                                                         #
test('Ensure TESTER_3 works with no arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ '--', '', TESTER_3_PATH ])
test('Ensure TESTER_3 doesn\'t recognize invalid argument arg1aa.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ '--', '', TESTER_3_PATH,
		'arg1aa' ])
test('Ensure TESTER_3 recognizes argument arg1a.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg1a,', TESTER_3_PATH,
		'arg1a' ])
test('Ensure TESTER_3 recognizes arg1b from arguments arg1aa and arg1b.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg1b,', TESTER_3_PATH,
		'arg1aa', 'arg1b' ])
test('Ensure TESTER_3 recognizes the arguments arg1a and arg1b.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg1a,arg1b,', TESTER_3_PATH,
		'arg1a', 'arg1b' ])
test('Ensure TESTER_3 recognizes the arguments arg1b, arg1c and arg1a.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg1b,arg1c,arg1a,', TESTER_3_PATH,
		'arg1b', 'arg1c', 'arg1a' ])
test('Ensure TESTER_3 recognizes the arguments arg1b, arg1c and arg1a from a bigger set of arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg1b,arg1c,arg1a,', TESTER_3_PATH,
		'arg1b', 'foobar', 'arg1c', 'ARGS2A', 'arg1a', 'ARGS2B' ])
test('Ensure TESTER_3 recognizes the arguments arg2a, arg1b and arg3c from a bigger set of arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg2a,arg1b,arg3c,', TESTER_3_PATH,
		'arg2a', 'foobar', 'arg1b', 'ARGS1A', 'arg3c', 'ARGS1B' ])
test('Ensure TESTER_3 recognizes the arguments arg2a and arg3c from a bigger set of arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg2a,arg3c,', TESTER_3_PATH,
		'arg2a', 'foobar', 'arg1bb', 'ARGS1', 'arg3c', 'ARGS1' ])
test('Ensure TESTER_3 recognizes the arguments arg2a, arg1a, arg3c and arg1a from a bigger set of arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg2a,arg1a,arg3c,arg1a,', TESTER_3_PATH,
		'arg2a', 'foobar', 'arg1a', 'ARGS2', 'arg3c', 'ARGS1', 'arg1a' ])
test('Ensure TESTER_3 recognizes the arguments arg4a from a bigger set of arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg4a foo,', TESTER_3_PATH,
		'arg4a' ])
test('Ensure TESTER_3 recognizes a bunch of arguments correctly #1.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg2a,arg4b bar,arg3c,arg4c keke,arg1c,', TESTER_3_PATH,
		'arg2a', 'foobar', 'arg4b', 'arg3c', 'arg4c', 'ARGS1', '--kurwa', 'arg1c' ])
test('Ensure TESTER_3 recognizes a bunch of arguments correctly #2.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'arg2a,arg3c,arg4c keke,arg1c,', TESTER_3_PATH,
		'arg2a', 'foobar', 'arg4bb', 'arg3c', 'arg4c', 'ARGS1', '--kurwa', 'arg1c' ])
                                                                         #
#### Ensure generated help texts don't change unintentionally. ###########
                                                                         #
foreach t : [
			[ '1', TESTER_1_PATH ],
			[ '2', TESTER_2_PATH ],
			[ '3', TESTER_3_PATH ],
			[ '4', TESTER_4_PATH ],
			[ '8', TESTER_8_PATH ],
		]
	test(
		'Ensure help text integrity for TESTER_@0@.'.format(t.get(0)),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ '--file', meson.current_build_dir() + '/help_text_sample_@0@.txt'.format(t.get(0)),
				t.get(1), '--help' ]
	)
endforeach
                                                                         #
#### Ensure NULL about string can be used. ###############################
                                                                         #
test('Ensure NULL about string can be used.', TESTER_5, valgrind_args : VALGRIND_ARGS)
                                                                         #
#### Ensure the reame progjems "works". ##################################
                                                                         #
test('Try to execute the readme program.', README, valgrind_args : VALGRIND_ARGS)
                                                                         #
#### Count given options. ################################################
                                                                         #
test('Count three short verbose options from a single argument.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: v=3 ', TESTER_9_PATH, '-vvv' ])
test('Count two short verbose options from a individual arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: v=2 ', TESTER_9_PATH, '-v', '-v' ])
test('Count two long verbose options from a individual arguments.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: v=2 ', TESTER_9_PATH, '--verbose', '--verbose' ])
test('Count five mixed verbose options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: v=5 ', TESTER_9_PATH, '--verbose', '--verbose', '-v', '-vv' ])
test('Count mixed verbose and wanha options.',
	MATCHER, valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: v=4 w=5 ', TESTER_9_PATH, '--verbose', '--wanha', '--verbose', '-v', '-www', '-vw' ])
                                                                         #
##########################################################################
