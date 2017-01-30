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

#if !defined(NDEBUG) || defined(UNIT_TESTING)
#undef assert
#define assert(e) mock_assert((long)(e), #e, __FILE__, __LINE__)
#endif

#define abort() do { mock_assert(false, "abort()", __FILE__, __LINE__) ; exit(1) ; } while (false)
