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

#ifndef UNIT_TESTING
#  define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#  if NULLABILITY_SUPPORT == 1
#    pragma clang diagnostic ignored "-Wnullability-extension"
#    define NONNULL _Nonnull
#    define NULLABLE _Nullable
#  else
#    define NONNULL
#    define NULLABLE
#  endif
#else
#  define WARN_UNUSED_RESULT
#  define NONNULL
#  define NULLABLE
#endif
