// Copyright Alexander Nasonov & Paul A. Bristow 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_DETAIL_LCAST_PRECISION_HPP_INCLUDED
#define HBOOST_DETAIL_LCAST_PRECISION_HPP_INCLUDED

#include <climits>
#include <ios>
#include <limits>

#include <hboost/config.hpp>
#include <hboost/integer_traits.hpp>

#ifndef HBOOST_NO_IS_ABSTRACT
// Fix for SF:1358600 - lexical_cast & pure virtual functions & VC 8 STL
#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/is_abstract.hpp>
#endif

#if defined(HBOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS) || \
  (defined(HBOOST_MSVC) && (HBOOST_MSVC<1310))

#define HBOOST_LCAST_NO_COMPILE_TIME_PRECISION
#endif

#ifdef HBOOST_LCAST_NO_COMPILE_TIME_PRECISION
#include <hboost/assert.hpp>
#else
#include <hboost/static_assert.hpp>
#endif

namespace hboost { namespace detail {

class lcast_abstract_stub {};

#ifndef HBOOST_LCAST_NO_COMPILE_TIME_PRECISION
// Calculate an argument to pass to std::ios_base::precision from
// lexical_cast. See alternative implementation for broken standard
// libraries in lcast_get_precision below. Keep them in sync, please.
template<class T>
struct lcast_precision
{
#ifdef HBOOST_NO_IS_ABSTRACT
    typedef std::numeric_limits<T> limits; // No fix for SF:1358600.
#else
    typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::if_<
        hboost::is_abstract<T>
      , std::numeric_limits<lcast_abstract_stub>
      , std::numeric_limits<T>
      >::type limits;
#endif

    HBOOST_STATIC_CONSTANT(bool, use_default_precision =
            !limits::is_specialized || limits::is_exact
        );

    HBOOST_STATIC_CONSTANT(bool, is_specialized_bin =
            !use_default_precision &&
            limits::radix == 2 && limits::digits > 0
        );

    HBOOST_STATIC_CONSTANT(bool, is_specialized_dec =
            !use_default_precision &&
            limits::radix == 10 && limits::digits10 > 0
        );

    HBOOST_STATIC_CONSTANT(std::streamsize, streamsize_max =
            hboost::integer_traits<std::streamsize>::const_max
        );

    HBOOST_STATIC_CONSTANT(unsigned int, precision_dec = limits::digits10 + 1U);

    HBOOST_STATIC_ASSERT(!is_specialized_dec ||
            precision_dec <= streamsize_max + 0UL
        );

    HBOOST_STATIC_CONSTANT(unsigned long, precision_bin =
            2UL + limits::digits * 30103UL / 100000UL
        );

    HBOOST_STATIC_ASSERT(!is_specialized_bin ||
            (limits::digits + 0UL < ULONG_MAX / 30103UL &&
            precision_bin > limits::digits10 + 0UL &&
            precision_bin <= streamsize_max + 0UL)
        );

    HBOOST_STATIC_CONSTANT(std::streamsize, value =
            is_specialized_bin ? precision_bin
                               : is_specialized_dec ? precision_dec : 6
        );
};
#endif

template<class T>
inline std::streamsize lcast_get_precision(T* = 0)
{
#ifndef HBOOST_LCAST_NO_COMPILE_TIME_PRECISION
    return lcast_precision<T>::value;
#else // Follow lcast_precision algorithm at run-time:

#ifdef HBOOST_NO_IS_ABSTRACT
    typedef std::numeric_limits<T> limits; // No fix for SF:1358600.
#else
    typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::if_<
        hboost::is_abstract<T>
      , std::numeric_limits<lcast_abstract_stub>
      , std::numeric_limits<T>
      >::type limits;
#endif

    bool const use_default_precision =
        !limits::is_specialized || limits::is_exact;

    if(!use_default_precision)
    { // Includes all built-in floating-point types, float, double ...
      // and UDT types for which digits (significand bits) is defined (not zero)

        bool const is_specialized_bin =
            limits::radix == 2 && limits::digits > 0;
        bool const is_specialized_dec =
            limits::radix == 10 && limits::digits10 > 0;
        std::streamsize const streamsize_max =
            (hboost::integer_traits<std::streamsize>::max)();

        if(is_specialized_bin)
        { // Floating-point types with
          // limits::digits defined by the specialization.

            unsigned long const digits = limits::digits;
            unsigned long const precision = 2UL + digits * 30103UL / 100000UL;
            // unsigned long is selected because it is at least 32-bits
            // and thus ULONG_MAX / 30103UL is big enough for all types.
            HBOOST_ASSERT(
                    digits < ULONG_MAX / 30103UL &&
                    precision > limits::digits10 + 0UL &&
                    precision <= streamsize_max + 0UL
                );
            return precision;
        }
        else if(is_specialized_dec)
        {   // Decimal Floating-point type, most likely a User Defined Type
            // rather than a real floating-point hardware type.
            unsigned int const precision = limits::digits10 + 1U;
            HBOOST_ASSERT(precision <= streamsize_max + 0UL);
            return precision;
        }
    }

    // Integral type (for which precision has no effect)
    // or type T for which limits is NOT specialized,
    // so assume stream precision remains the default 6 decimal digits.
    // Warning: if your User-defined Floating-point type T is NOT specialized,
    // then you may lose accuracy by only using 6 decimal digits.
    // To avoid this, you need to specialize T with either
    // radix == 2 and digits == the number of significand bits,
    // OR
    // radix = 10 and digits10 == the number of decimal digits.

    return 6;
#endif
}

template<class T>
inline void lcast_set_precision(std::ios_base& stream, T*)
{
    stream.precision(lcast_get_precision<T>());
}

template<class Source, class Target>
inline void lcast_set_precision(std::ios_base& stream, Source*, Target*)
{
    std::streamsize const s = lcast_get_precision(static_cast<Source*>(0));
    std::streamsize const t = lcast_get_precision(static_cast<Target*>(0));
    stream.precision(s > t ? s : t);
}

}}

#endif //  HBOOST_DETAIL_LCAST_PRECISION_HPP_INCLUDED

