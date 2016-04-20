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
foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	foreach opt : [ 'a', 'b', 'c', 'd' ]
		my_args = [ opt_style.format(opt) ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@'.format(opt), TESTER_1_PATH ] + my_args)
	endforeach
endforeach
                                                                         #
# Specify a short option twice in a single argument . ####################
                                                                         #
foreach opt : [ 'a', 'b', 'c', 'd' ]
	my_args = [ SHORT_OPT.format(opt + opt) ]
	test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: @0@'.format(opt), TESTER_1_PATH ] + my_args)
endforeach
                                                                         #
#### Specify an option twice. ############################################
                                                                         #
foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	foreach opt : [ 'a', 'b', 'c', 'd' ]
		my_args = [ opt_style.format(opt), opt_style.format(opt) ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@'.format(opt), TESTER_1_PATH ] + my_args)
	endforeach
endforeach
                                                                         #
#### Specify single short option multiple times in a single argument. ####
                                                                         #
foreach opt : [ 'a', 'b', 'c', 'd' ]
	my_args = [ SHORT_OPT.format(opt + opt + opt + opt) ]
	test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: @0@'.format(opt), TESTER_1_PATH ] + my_args)
endforeach
                                                                         #
#### Specify a single option multiple times. #############################
                                                                         #
foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	foreach opt : [ 'a', 'b', 'c', 'd' ]
		my_args = [ opt_style.format(opt), opt_style.format(opt),
			opt_style.format(opt), opt_style.format(opt) ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@'.format(opt), TESTER_1_PATH ] + my_args)
	endforeach
endforeach
                                                                         #
#### Mix identical double and single short options. ######################
                                                                         #
foreach opt : [ 'a', 'b', 'c', 'd' ]
	my_args = [ SHORT_OPT.format(opt + opt), SHORT_OPT.format(opt),
		SHORT_OPT.format(opt + opt) ]
	test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: @0@'.format(opt), TESTER_1_PATH ] + my_args)
endforeach
                                                                         #
#### Mix different short options. ########################################
                                                                         #
my_args = [ '-b', '-c' ]
test(
	'@0@'.format(my_args),
	MATCHER,
	valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: bc', TESTER_1_PATH ] + my_args
)

my_args = [ '-b', '-cccccc', '-a' ]
test(
	'@0@'.format(my_args),
	MATCHER,
	valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: abc', TESTER_1_PATH ] + my_args
)

foreach my_args : [
			[ '-a', '-b', '-c', '-d' ],
			[ '-b', '-c', '-d', '-a' ],
			[ '-b', '-a', '-d', '-c' ],
			[ '-bb', '-a', '-ddddddd', '-c', '-a' ],
		]
	test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: abcd', TESTER_1_PATH ] + my_args)
endforeach
                                                                         #
#### Mix simple opts with arguments. #####################################
                                                                         #
foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	foreach opt : [ 'a', 'b', 'c', 'd' ]

		my_args = [ opt_style.format(opt), 'foobar' ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@,foobar'.format(opt), TESTER_1_PATH ] + my_args)

		my_args = [ opt_style.format(opt), 'foobar', 'barfoo' ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@,foobar,barfoo'.format(opt), TESTER_1_PATH ] + my_args)

		my_args = [ opt_style.format(opt), 'foobar', '-x', 'barfoo' ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
				args : [ 'Got: @0@,foobar,-x,barfoo'.format(opt), TESTER_1_PATH ] + my_args)

		my_args = [ opt_style.format(opt), 'foobar', opt_style.format(opt), 'barfoo' ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
				args : [ 'Got: @0@,foobar,@1@,barfoo'.format(opt, opt_style.format(opt)), TESTER_1_PATH ] + my_args)
	endforeach
endforeach
                                                                         #
#### Ensure empty argument is ignored. ###################################
                                                                         #
foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	foreach opt : [ 'a', 'b', 'c', 'd' ]
		my_args = [ '', opt_style.format(opt) ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@'.format(opt), TESTER_1_PATH ] + my_args)

		my_args = [ opt_style.format(opt), '', opt_style.format(opt) ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@'.format(opt), TESTER_1_PATH ] + my_args)

		my_args = [ opt_style.format(opt), '', opt_style.format(opt), 'foobar' ]
		test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@,foobar'.format(opt), TESTER_1_PATH ] + my_args)
	endforeach

	my_args = [ opt_style.format('a'), '', opt_style.format('b'), '', 'foobar' ]
	test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: ab,foobar'.format(opt), TESTER_1_PATH ] + my_args)
endforeach
                                                                         #
#### Mix simple short an long options. ###################################
                                                                         #
foreach my_args : [
		[ '-a', '--bbbb', '-c', '--dddd' ],
		[ '--aaaa', '-b', '--cccc', '-d' ],
		[ '--aaaa', '--bbbb', '-c', '-d' ],
		[ '-a', '-b', '--cccc', '--dddd' ],
		[ '-aaaaa', '-b', '--cccc', '--dddd', '--cccc' ],
		[ '-aaaaa', '-b', '-b', '-b', '-b', '--cccc', '--dddd', '--cccc' ]]
	test('@0@'.format(my_args), MATCHER, valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: abcd', TESTER_1_PATH ] + my_args)
endforeach
                                                                         #
#### Test mandatory argument handling. ###################################
                                                                         #
foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	my_args = [ opt_style.format('i'), 'foobar' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{foobar}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('i'), 'foobar', 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('i'), 'foobar', opt_style.format('i'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{barfoo}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('i'), 'foobar', opt_style.format('b'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: bi{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('a'), opt_style.format('i'), 'foobar', opt_style.format('b'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: abi{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('a'), opt_style.format('i'), 'foobar', opt_style.format('i'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: ai{barfoo}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('i'), A_STRING ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{@0@}'.format(A_STRING), TESTER_1_PATH ] + my_args
	)
endforeach

foreach opt_style : [ SHORT_OPT_EQ, LONG_OPT_EQ, SHORT_OPT_APPEND ]
	my_args = [ opt_style.format('i', 'foobar') ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{foobar}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('i', 'foobar'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('i', 'foobar'), opt_style.format('i', 'barfoo') ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{barfoo}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('i', 'foobar'), SHORT_OPT.format('b'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: bi{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ SHORT_OPT.format('a'), opt_style.format('i', 'foobar'), LONG_OPT.format('b'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: abi{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ LONG_OPT.format('a'), opt_style.format('i', 'foobar'), opt_style.format('i', 'barfoo') ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: ai{barfoo}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('i', A_STRING) ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{@0@}'.format(A_STRING), TESTER_1_PATH ] + my_args
	)
endforeach

my_args = [ LONG_OPT_APPEND.format('i', 'foobar') ]
test(
	'!! @0@'.format(my_args),
	TESTER_1,
	valgrind_args : VALGRIND_ARGS,
	should_fail : true,
	args : my_args
)

my_args = [ LONG_OPT_EQ.format('i', 'foobar'), SHORT_OPT_APPEND.format('i', 'barfoo') ]
test(
	'@0@'.format(my_args),
	MATCHER,
	valgrind_args : VALGRIND_ARGS,
	args : [ 'Got: i{barfoo}', TESTER_1_PATH ] + my_args
)
                                                                         #
#### Test optional argument handling. ####################################
                                                                         #

foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	my_args = [ opt_style.format('j'), 'foobar' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{Oujea!},foobar', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('j'), 'foobar', 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{Oujea!},foobar,barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('j'), 'foobar', opt_style.format('j'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{Oujea!},foobar,@0@,barfoo'.format(opt_style.format('j')), TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('a'), opt_style.format('j'), 'foobar', opt_style.format('b'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: aj{Oujea!},foobar,@0@,barfoo'.format(opt_style.format('b')), TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('j'), A_STRING ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{Oujea!},@0@'.format(A_STRING), TESTER_1_PATH ] + my_args
	)
endforeach

foreach my_args : [
			[ SHORT_OPT_APPEND.format('j', 'foobar') ],
			[ SHORT_OPT_APPEND.format('j', 'foobar'), 'barfoo' ],
			[ SHORT_OPT_APPEND.format('j', 'foobar'), SHORT_OPT_APPEND.format('j', 'barfoo') ],
			[ SHORT_OPT.format('a'), SHORT_OPT_APPEND.format('j', 'foobar'), SHORT_OPT.format('b'), 'barfoo' ],
			[ SHORT_OPT_APPEND.format('j', A_STRING) ]
		]
	test(
		'!! @0@'.format(my_args),
		TESTER_1,
		valgrind_args : VALGRIND_ARGS,
		should_fail : true,
		args : my_args
	)
endforeach

foreach opt_style : [ SHORT_OPT_EQ, LONG_OPT_EQ ]

	my_args = [ SHORT_OPT.format('j') ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{Oujea!}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('j', 'foobar') ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{foobar}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('j', 'foobar'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('j', 'foobar'), opt_style.format('j', 'barfoo') ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{barfoo}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('j', 'foobar'), SHORT_OPT.format('b'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: bj{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ SHORT_OPT.format('a'), opt_style.format('j', 'foobar'), LONG_OPT.format('b'), 'barfoo' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: abj{foobar},barfoo', TESTER_1_PATH ] + my_args
	)

	my_args = [ LONG_OPT.format('a'), opt_style.format('j', 'foobar'), opt_style.format('j', 'barfoo') ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: aj{barfoo}', TESTER_1_PATH ] + my_args
	)

	my_args = [ opt_style.format('j', A_STRING) ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: j{@0@}'.format(A_STRING), TESTER_1_PATH ] + my_args
	)
endforeach

my_args = [ LONG_OPT_APPEND.format('j', 'foobar') ]
test(
	'!! @0@'.format(my_args),
	TESTER_1,
	valgrind_args : VALGRIND_ARGS,
	should_fail : true,
	args : my_args
)
                                                                         #
#### Ensure an empty argument can be given. ##############################
                                                                         #
foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	my_args = [ opt_style.format('i'), '' ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{}', TESTER_1_PATH ] + my_args
	)
endforeach

foreach opt_style : [ SHORT_OPT_EQ, LONG_OPT_EQ ]
	foreach opt : [ 'i', 'j' ]
		my_args = [ opt_style.format(opt, '') ]
		test(
			'@0@'.format(my_args),
			MATCHER,
			valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: @0@{}'.format(opt), TESTER_1_PATH ] + my_args
		)
	endforeach
endforeach
                                                                         #
#### Test some weird arguments. ##########################################
                                                                         #
foreach arg : [ '--foobar', '-f' ]
	foreach opt_style : [ SHORT_OPT, LONG_OPT ]
		my_args = [ opt_style.format('i'), arg ]
		test(
			'@0@'.format(my_args),
			MATCHER,
			valgrind_args : VALGRIND_ARGS,
			args : [ 'Got: i{@0@}'.format(arg), TESTER_1_PATH ] + my_args
		)
	endforeach

	foreach opt_style : [ SHORT_OPT_EQ, LONG_OPT_EQ ]
		foreach opt : [ 'i', 'j' ]
			my_args = [ opt_style.format(opt, arg) ]
			test(
				'@0@'.format(my_args),
				MATCHER,
				valgrind_args : VALGRIND_ARGS,
				args : [ 'Got: @0@{@1@}'.format(opt, arg), TESTER_1_PATH ] + my_args
			)
		endforeach
	endforeach

	my_args = [ SHORT_OPT_APPEND.format('i', arg) ]
	test(
		'@0@'.format(my_args),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: @0@{@1@}'.format('i', arg), TESTER_1_PATH ] + my_args
	)
endforeach
                                                                         #
#### Test invalid options. ###############################################
                                                                         #
foreach my_args : [
			[ '-x' ],
			[ '--xxxx' ],
			[ '-a', '-x' ],
			[ '--aaaa', '-x' ],
			[ '-a', '--xxxx' ],
			[ '--aaaa', '--xxxx' ],
			[ '-x', '-a' ],
			[ '-x', '--aaaa' ],
			[ '--xxxx', '-a' ],
			[ '--xxxx', '--aaaa' ],
			[ '-b', '-x', '--aaaa' ],
			[ '-i', '-x', '-x', '--aaaa' ],
			[ '-b', '--xxxx', '--aaaa' ],
			[ '-i', '--xxxx', '--xxxx', '--aaaa' ],
			[ '-c', '-j=sd', '-x', '-a' ],
			[ '-c', '-j=sd', '--xxxx', '-a' ],
			[ '-' ],
			[ '-', '--' ],
			[ '-c', '-', '-a' ],
			[ '--cccc', '-', '-a' ],
			[ '--aaaa=foobar' ],
			[ '-b=foobar' ],
			[ '-a', '-c=foobar' ],
			[ '-aaaa', '--dddd=foobar', '-c' ],
			[ '-a-b' ],
			[ '-b-d' ],
			[ '-j-c' ],
			[ '---aaaa' ],
		]
	test(
		'!! @0@'.format(my_args),
		TESTER_1,
		valgrind_args : VALGRIND_ARGS,
		should_fail : true,
		args : my_args
	)
endforeach
                                                                         #
#### Test missing arguments. #############################################
                                                                         #
foreach opt_style : [ SHORT_OPT, LONG_OPT ]
	test(
		'!! @0@'.format([ opt_style.format('i') ]),
		TESTER_1,
		valgrind_args : VALGRIND_ARGS,
		should_fail : true,
		args : [ opt_style.format('i') ]
	)
endforeach

foreach opt_style : [ SHORT_OPT_EQ, LONG_OPT_EQ ]
	test(
		'@0@'.format([ opt_style.format('i') ]),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'Got: i{}', TESTER_1_PATH, opt_style.format('i', '') ]
	)
endforeach
                                                                         #
#### Test double hyphen. #################################################
                                                                         #
foreach my_args : [
			[ 'ab', [ '-a', '-b' ]],
			[ 'a,-b', [ '-a', '--', '-b' ]],
			[ 'a,--bbbb', [ '--aaaa', '--', '--bbbb' ]],
			[ 'ac,--bbbb,-d', [ '--aaaa', '-c', '--', '--bbbb', '-d' ]],
			[ 'i{foobar},--jjjj,barfoo', [ '--iiii', 'foobar', '--', '--jjjj', 'barfoo' ]],
			[ ',A,--iiii,foobar,--,--jjjj,barfoo', [ 'A', '--iiii', 'foobar', '--', '--jjjj', 'barfoo' ]],
		]
	test(
		'@0@'.format(my_args.get(1)),
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
		'@0@ => @1@'.format(my_args.get(1), my_args.get(0)),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ 'First argument: @0@'.format(my_args.get(0)), TESTER_7_PATH ] + my_args.get(1)
	)
endforeach
                                                                         #
#### Test "hidden option. ################################################
                                                                         #
foreach my_args : [
			[ 'Got: hidden!', [ '--hidden-option' ]],
			[ 'Got: ahidden!', [ '--hidden-option', '-a' ]],
			[ 'Got: abhidden!', [ '-a', '--hidden-option', '-b' ]],
			[ 'Got: abhidden!', [ '-a', '--hidden-option', '-b', '--hidden-option' ]],
		]
	test(
		'@0@'.format(my_args.get(1)),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ my_args.get(0), TESTER_1_PATH ] + my_args.get(1)
	)
endforeach
                                                                         #
#### Test options with common prefix. ####################################
                                                                         #
test(
	'tester_6: --help',
	MATCHER,
	valgrind_args : VALGRIND_ARGS,
	args : [ 'got basic', TESTER_6_PATH, '--help' ],
)

test(
	'tester_6: --help-full',
	MATCHER,
	valgrind_args : VALGRIND_ARGS,
	args : [ 'got full', TESTER_6_PATH, '--help-full' ],
)

test(
	'!! tester_6: --help',
	MATCHER,
	valgrind_args : VALGRIND_ARGS,
	args : [ '--fail-match', 'got full', TESTER_6_PATH, '--help' ],
)

test(
	'!! tester_6: --help-full',
	MATCHER,
	valgrind_args : VALGRIND_ARGS,
	args : [ '--fail-match', 'got basic', TESTER_6_PATH, '--help-full' ],
)
                                                                         #
#### Test (non option) argument handling. ################################
                                                                         #
foreach my_args : [
	[ '', []],
	[ '', [ 'arg1aa' ]],
	[ 'arg1a,', [ 'arg1a' ]],
	[ 'arg1b,', [ 'arg1aa', 'arg1b' ]],
	[ 'arg1a,arg1b,', [ 'arg1a', 'arg1b' ]],
	[ 'arg1a,arg1b,arg1c,', [ 'arg1a', 'arg1b', 'arg1c']],
	[ 'arg1a,arg1b,arg1c,', [ 'arg1a', 'foobar', 'arg1b', 'ARGS2A', 'arg1c', 'ARGS2B' ]],
	[ 'arg2a,arg1b,arg3c,', [ 'arg2a', 'foobar', 'arg1b', 'ARGS1A', 'arg3c', 'ARGS1B' ]],
	[ 'arg2a,arg3c,', [ 'arg2a', 'foobar', 'arg1bb', 'ARGS1', 'arg3c', 'ARGS1' ]],
	[ 'arg2a,arg1a,arg3c,arg1a,', [ 'arg2a', 'foobar', 'arg1a', 'ARGS2', 'arg3c', 'ARGS1', 'arg1a' ]],
	[ 'arg4a foo,', [ 'arg4a' ]],
	[ 'arg2a,arg4b bar,arg3c,arg4c keke,arg1c,', [ 'arg2a', 'foobar', 'arg4b', 'arg3c', 'arg4c', 'ARGS1', '--kurwa', 'arg1c' ]],
	[ 'arg2a,arg3c,arg4c keke,arg1c,', [ 'arg2a', 'foobar', 'arg4bb', 'arg3c', 'arg4c', 'ARGS1', '--kurwa', 'arg1c' ]],
]
	test(
		'@0@'.format(my_args.get(1)),
		MATCHER,
		valgrind_args : VALGRIND_ARGS,
		args : [ '--', my_args.get(0), TESTER_3_PATH ] + my_args.get(1)
	)
endforeach
                                                                         #
#### Ensure generated help texts don't change unintentionally. ###########
                                                                         #
foreach t : [
			[ '1', TESTER_1_PATH ],
			[ '2', TESTER_2_PATH ],
			[ '3', TESTER_3_PATH ],
			[ '4', TESTER_4_PATH ],
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
test('NULL about string', TESTER_5, valgrind_args : VALGRIND_ARGS)
                                                                         #
##########################################################################
