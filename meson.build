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

project(
	'optargs',
	'c',
	license : 'GPLv2',
	version : '3.0.1',
	meson_version : '>=0.31.0',
	default_options : [
		'werror=true',
		'warning_level=3',
		'stdsplit=false',
		'c_std=c11'
	]
)

OPTARGS_HDR = include_directories(meson.current_source_dir())
OPTARGS_SRC = meson.current_source_dir() + '/optargs.c'

if get_option('buildtype') == 'release'
	release_flags = [ '-DNDEBUG' ]
else
	release_flags = []
endif

OPTARGS_LIB = shared_library(
	'optargs',
	OPTARGS_SRC,
	c_args : release_flags,
	version : meson.project_version(),
	install : true
)

if meson.is_subproject()
	OPTARGS_BIN = static_library(
		'optargs',
		OPTARGS_SRC,
		c_args : release_flags
	)
endif

install_headers(
	'optargs.h',
	subdir : 'optargs'
)

subdir('tst')

import('pkgconfig').generate(
	libraries : OPTARGS_LIB,
	subdirs : 'optargs',
	version : meson.project_version(),
	name : 'liboptargs',
	filebase : 'optargs',
	description : 'Option and argument parsing library with automatically generated help texts.'
)
