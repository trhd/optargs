project(
	'optargs',
	'c',
	license : 'GPLv2',
	version : '1.0.0',
	meson_version : '>=0.31.0',
	default_options : [ 'werror=true', 'warning_level=3', 'stdsplit=false' ]
)

OPTARGS_HDR = include_directories(meson.current_source_dir())

if get_option('buildtype') == 'release'
	release_flags = [ '-DNDEBUG' ]
else
	release_flags = []
endif

OPTARGS_LIB = shared_library(
	'optargs',
	'optargs.c',
	c_args : release_flags,
	version : meson.project_version(),
	install : true
)

if meson.is_subproject()
	OPTARGS_BIN = static_library(
		'optargs',
		'optargs.c',
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
