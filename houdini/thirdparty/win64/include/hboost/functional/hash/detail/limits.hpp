
// Copyright 2005-2009 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// On some platforms std::limits gives incorrect values for long double.
// This tries to work around them.

#if !defined(HBOOST_FUNCTIONAL_HASH_DETAIL_LIMITS_HEADER)
#define HBOOST_FUNCTIONAL_HASH_DETAIL_LIMITS_HEADER

#include <hboost/config.hpp>
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#pragma once
#endif

#include <hboost/limits.hpp>

// On OpenBSD, numeric_limits is not reliable for long doubles, but
// the macros defined in <float.h> are and support long double when STLport
// doesn't.

#if defined(__OpenBSD__) || defined(_STLP_NO_LONG_DOUBLE)
#include <float.h>
#endif

namespace hboost
{
    namespace hash_detail
    {
        template <class T>
        struct limits : std::numeric_limits<T> {};

#if defined(__OpenBSD__) || defined(_STLP_NO_LONG_DOUBLE)
        template <>
        struct limits<long double>
             : std::numeric_limits<long double>
        {
            static long double epsilon() {
                return LDBL_EPSILON;
            }

            static long double (max)() {
                return LDBL_MAX;
            }

            static long double (min)() {
                return LDBL_MIN;
            }

            HBOOST_STATIC_CONSTANT(int, digits = LDBL_MANT_DIG);
            HBOOST_STATIC_CONSTANT(int, max_exponent = LDBL_MAX_EXP);
            HBOOST_STATIC_CONSTANT(int, min_exponent = LDBL_MIN_EXP);
#if defined(_STLP_NO_LONG_DOUBLE)
            HBOOST_STATIC_CONSTANT(int, radix = FLT_RADIX);
#endif
        };
#endif // __OpenBSD__
    }
}

#endif
