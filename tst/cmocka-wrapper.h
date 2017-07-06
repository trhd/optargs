/**
 * optargs -- A command line option and argument management library.
 * Copyright (C) 2017 Hemmo Nieminen
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

#ifndef UNIT_TESTING
#error Probably not what you were trying to do.
#endif

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <stdbool.h>

#ifndef NDEBUG
#undef assert
static inline void silence_scan_build(long p) { if (!p) exit(1) ; }
#define assert(e) (mock_assert((long)(e), #e, __FILE__, __LINE__), silence_scan_build((long)(e)))
#endif

#define abort() do { mock_assert(false, "abort()", __FILE__, __LINE__) ; exit(1) ; } while (false)
