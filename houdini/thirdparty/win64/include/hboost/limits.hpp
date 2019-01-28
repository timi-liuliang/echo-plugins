
//  (C) Copyright John maddock 1999. 
//  (C) David Abrahams 2002.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// use this header as a workaround for missing <limits>

//  See http://www.boost.org/libs/compatibility/index.html for documentation.

#ifndef HBOOST_LIMITS
#define HBOOST_LIMITS

#include <hboost/config.hpp>

#ifdef HBOOST_NO_LIMITS
#  error "There is no std::numeric_limits suppport available."
#else
# include <limits>
#endif

#if (defined(HBOOST_HAS_LONG_LONG) && defined(HBOOST_NO_LONG_LONG_NUMERIC_LIMITS)) \
      || (defined(HBOOST_HAS_MS_INT64) && defined(HBOOST_NO_MS_INT64_NUMERIC_LIMITS))
// Add missing specializations for numeric_limits:
#ifdef HBOOST_HAS_MS_INT64
#  define HBOOST_LLT __int64
#  define HBOOST_ULLT unsigned __int64
#else
#  define HBOOST_LLT  ::hboost::long_long_type
#  define HBOOST_ULLT  ::hboost::ulong_long_type
#endif

#include <climits>  // for CHAR_BIT

namespace std
{
  template<>
  class numeric_limits<HBOOST_LLT> 
  {
   public:

      HBOOST_STATIC_CONSTANT(bool, is_specialized = true);
#ifdef HBOOST_HAS_MS_INT64
      static HBOOST_LLT min HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0x8000000000000000i64; }
      static HBOOST_LLT max HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0x7FFFFFFFFFFFFFFFi64; }
#elif defined(LLONG_MAX)
      static HBOOST_LLT min HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return LLONG_MIN; }
      static HBOOST_LLT max HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return LLONG_MAX; }
#elif defined(LONGLONG_MAX)
      static HBOOST_LLT min HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return LONGLONG_MIN; }
      static HBOOST_LLT max HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return LONGLONG_MAX; }
#else
      static HBOOST_LLT min HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return 1LL << (sizeof(HBOOST_LLT) * CHAR_BIT - 1); }
      static HBOOST_LLT max HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return ~(min)(); }
#endif
      HBOOST_STATIC_CONSTANT(int, digits = sizeof(HBOOST_LLT) * CHAR_BIT -1);
      HBOOST_STATIC_CONSTANT(int, digits10 = (CHAR_BIT * sizeof (HBOOST_LLT) - 1) * 301L / 1000);
      HBOOST_STATIC_CONSTANT(bool, is_signed = true);
      HBOOST_STATIC_CONSTANT(bool, is_integer = true);
      HBOOST_STATIC_CONSTANT(bool, is_exact = true);
      HBOOST_STATIC_CONSTANT(int, radix = 2);
      static HBOOST_LLT epsilon() throw() { return 0; };
      static HBOOST_LLT round_error() throw() { return 0; };

      HBOOST_STATIC_CONSTANT(int, min_exponent = 0);
      HBOOST_STATIC_CONSTANT(int, min_exponent10 = 0);
      HBOOST_STATIC_CONSTANT(int, max_exponent = 0);
      HBOOST_STATIC_CONSTANT(int, max_exponent10 = 0);

      HBOOST_STATIC_CONSTANT(bool, has_infinity = false);
      HBOOST_STATIC_CONSTANT(bool, has_quiet_NaN = false);
      HBOOST_STATIC_CONSTANT(bool, has_signaling_NaN = false);
      HBOOST_STATIC_CONSTANT(bool, has_denorm = false);
      HBOOST_STATIC_CONSTANT(bool, has_denorm_loss = false);
      static HBOOST_LLT infinity() throw() { return 0; };
      static HBOOST_LLT quiet_NaN() throw() { return 0; };
      static HBOOST_LLT signaling_NaN() throw() { return 0; };
      static HBOOST_LLT denorm_min() throw() { return 0; };

      HBOOST_STATIC_CONSTANT(bool, is_iec559 = false);
      HBOOST_STATIC_CONSTANT(bool, is_bounded = true);
      HBOOST_STATIC_CONSTANT(bool, is_modulo = true);

      HBOOST_STATIC_CONSTANT(bool, traps = false);
      HBOOST_STATIC_CONSTANT(bool, tinyness_before = false);
      HBOOST_STATIC_CONSTANT(float_round_style, round_style = round_toward_zero);
      
  };

  template<>
  class numeric_limits<HBOOST_ULLT> 
  {
   public:

      HBOOST_STATIC_CONSTANT(bool, is_specialized = true);
#ifdef HBOOST_HAS_MS_INT64
      static HBOOST_ULLT min HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0ui64; }
      static HBOOST_ULLT max HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0xFFFFFFFFFFFFFFFFui64; }
#elif defined(ULLONG_MAX) && defined(ULLONG_MIN)
      static HBOOST_ULLT min HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return ULLONG_MIN; }
      static HBOOST_ULLT max HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return ULLONG_MAX; }
#elif defined(ULONGLONG_MAX) && defined(ULONGLONG_MIN)
      static HBOOST_ULLT min HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return ULONGLONG_MIN; }
      static HBOOST_ULLT max HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return ULONGLONG_MAX; }
#else
      static HBOOST_ULLT min HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return 0uLL; }
      static HBOOST_ULLT max HBOOST_PREVENT_MACRO_SUBSTITUTION (){ return ~0uLL; }
#endif
      HBOOST_STATIC_CONSTANT(int, digits = sizeof(HBOOST_LLT) * CHAR_BIT);
      HBOOST_STATIC_CONSTANT(int, digits10 = (CHAR_BIT * sizeof (HBOOST_LLT)) * 301L / 1000);
      HBOOST_STATIC_CONSTANT(bool, is_signed = false);
      HBOOST_STATIC_CONSTANT(bool, is_integer = true);
      HBOOST_STATIC_CONSTANT(bool, is_exact = true);
      HBOOST_STATIC_CONSTANT(int, radix = 2);
      static HBOOST_ULLT epsilon() throw() { return 0; };
      static HBOOST_ULLT round_error() throw() { return 0; };

      HBOOST_STATIC_CONSTANT(int, min_exponent = 0);
      HBOOST_STATIC_CONSTANT(int, min_exponent10 = 0);
      HBOOST_STATIC_CONSTANT(int, max_exponent = 0);
      HBOOST_STATIC_CONSTANT(int, max_exponent10 = 0);

      HBOOST_STATIC_CONSTANT(bool, has_infinity = false);
      HBOOST_STATIC_CONSTANT(bool, has_quiet_NaN = false);
      HBOOST_STATIC_CONSTANT(bool, has_signaling_NaN = false);
      HBOOST_STATIC_CONSTANT(bool, has_denorm = false);
      HBOOST_STATIC_CONSTANT(bool, has_denorm_loss = false);
      static HBOOST_ULLT infinity() throw() { return 0; };
      static HBOOST_ULLT quiet_NaN() throw() { return 0; };
      static HBOOST_ULLT signaling_NaN() throw() { return 0; };
      static HBOOST_ULLT denorm_min() throw() { return 0; };

      HBOOST_STATIC_CONSTANT(bool, is_iec559 = false);
      HBOOST_STATIC_CONSTANT(bool, is_bounded = true);
      HBOOST_STATIC_CONSTANT(bool, is_modulo = true);

      HBOOST_STATIC_CONSTANT(bool, traps = false);
      HBOOST_STATIC_CONSTANT(bool, tinyness_before = false);
      HBOOST_STATIC_CONSTANT(float_round_style, round_style = round_toward_zero);
      
  };
}
#endif 

#endif

