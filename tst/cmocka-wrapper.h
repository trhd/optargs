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
