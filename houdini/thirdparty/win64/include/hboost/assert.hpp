//
//  hboost/assert.hpp - HBOOST_ASSERT(expr)
//                     HBOOST_ASSERT_MSG(expr, msg)
//                     HBOOST_VERIFY(expr)
//                     HBOOST_VERIFY_MSG(expr, msg)
//                     HBOOST_ASSERT_IS_VOID
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2007, 2014 Peter Dimov
//  Copyright (c) Beman Dawes 2011
//  Copyright (c) 2015 Ion Gaztanaga
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  Note: There are no include guards. This is intentional.
//
//  See http://www.boost.org/libs/assert/assert.html for documentation.
//

//
// Stop inspect complaining about use of 'assert':
//
// boostinspect:naassert_macro
//

//
// HBOOST_ASSERT, HBOOST_ASSERT_MSG, HBOOST_ASSERT_IS_VOID
//

#undef HBOOST_ASSERT
#undef HBOOST_ASSERT_MSG
#undef HBOOST_ASSERT_IS_VOID

#if defined(HBOOST_DISABLE_ASSERTS) || ( defined(HBOOST_ENABLE_ASSERT_DEBUG_HANDLER) && defined(NDEBUG) )

# define HBOOST_ASSERT(expr) ((void)0)
# define HBOOST_ASSERT_MSG(expr, msg) ((void)0)
# define HBOOST_ASSERT_IS_VOID

#elif defined(HBOOST_ENABLE_ASSERT_HANDLER) || ( defined(HBOOST_ENABLE_ASSERT_DEBUG_HANDLER) && !defined(NDEBUG) )

#include <hboost/config.hpp> // for HBOOST_LIKELY
#include <hboost/current_function.hpp>

namespace hboost
{
    void assertion_failed(char const * expr, char const * function, char const * file, long line); // user defined
    void assertion_failed_msg(char const * expr, char const * msg, char const * function, char const * file, long line); // user defined
} // namespace hboost

#define HBOOST_ASSERT(expr) (HBOOST_LIKELY(!!(expr))? ((void)0): ::hboost::assertion_failed(#expr, HBOOST_CURRENT_FUNCTION, __FILE__, __LINE__))
#define HBOOST_ASSERT_MSG(expr, msg) (HBOOST_LIKELY(!!(expr))? ((void)0): ::hboost::assertion_failed_msg(#expr, msg, HBOOST_CURRENT_FUNCTION, __FILE__, __LINE__))

#else

# include <assert.h> // .h to support old libraries w/o <cassert> - effect is the same

# define HBOOST_ASSERT(expr) assert(expr)
# define HBOOST_ASSERT_MSG(expr, msg) assert((expr)&&(msg))
#if defined(NDEBUG)
# define HBOOST_ASSERT_IS_VOID
#endif

#endif

//
// HBOOST_VERIFY, HBOOST_VERIFY_MSG
//

#undef HBOOST_VERIFY
#undef HBOOST_VERIFY_MSG

#if defined(HBOOST_DISABLE_ASSERTS) || ( !defined(HBOOST_ENABLE_ASSERT_HANDLER) && defined(NDEBUG) )

# define HBOOST_VERIFY(expr) ((void)(expr))
# define HBOOST_VERIFY_MSG(expr, msg) ((void)(expr))

#else

# define HBOOST_VERIFY(expr) HBOOST_ASSERT(expr)
# define HBOOST_VERIFY_MSG(expr, msg) HBOOST_ASSERT_MSG(expr,msg)

#endif
