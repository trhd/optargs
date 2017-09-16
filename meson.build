# optargs -- A command line option and argument management library.
# Copyright (C) 2016-2017 Hemmo Nieminen
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

project(
	'optargs',
	'c',
	license : 'GPLv3',
	version : '5.0.1',
	meson_version : '>=0.31.0',
	default_options : [
		'werror=true',
		'warning_level=3',
		'stdsplit=false',
		'c_std=c11'
	]
)

cflags = []
if get_option('buildtype') == 'release'
	cflags += [ '-DNDEBUG' ]
endif

lib = library(
	'optargs',
	files('optargs.c'),
	c_args : cflags,
	version : meson.project_version(),
	install : true
)

dep = declare_dependency(
	include_directories : include_directories('.'),
	link_with : lib,
	version : meson.project_version()
)

install_headers(
	'optargs.h',
	subdir : 'optargs'
)

subdir('tst')

import('pkgconfig').generate(
	libraries : lib,
	subdirs : 'optargs',
	version : meson.project_version(),
	name : 'liboptargs',
	filebase : 'optargs',
	description : 'Option and argument parsing library with automatically generated help texts.'
)
