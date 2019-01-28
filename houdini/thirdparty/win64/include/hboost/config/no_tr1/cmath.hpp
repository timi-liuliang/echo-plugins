//  (C) Copyright John Maddock 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// The aim of this header is just to include <cmath> but to do
// so in a way that does not result in recursive inclusion of
// the Boost TR1 components if hboost/tr1/tr1/cmath is in the
// include search path.  We have to do this to avoid circular
// dependencies:
//

#ifndef HBOOST_CONFIG_CMATH
#  define HBOOST_CONFIG_CMATH

#  ifndef HBOOST_TR1_NO_RECURSION
#     define HBOOST_TR1_NO_RECURSION
#     define HBOOST_CONFIG_NO_CMATH_RECURSION
#  endif

#  include <cmath>

#  ifdef HBOOST_CONFIG_NO_CMATH_RECURSION
#     undef HBOOST_TR1_NO_RECURSION
#     undef HBOOST_CONFIG_NO_CMATH_RECURSION
#  endif

#endif