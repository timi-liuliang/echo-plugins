//  ratio_fwd.hpp  ---------------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes
//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

/*

This code was derived by Beman Dawes from Howard Hinnant's time2_demo prototype.
Many thanks to Howard for making his code available under the Boost license.
The original code was modified to conform to Boost conventions and to section
20.4 Compile-time rational arithmetic [ratio], of the C++ committee working
paper N2798.
See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2798.pdf.

time2_demo contained this comment:

    Much thanks to Andrei Alexandrescu,
                   Walter Brown,
                   Peter Dimov,
                   Jeff Garland,
                   Terry Golubiewski,
                   Daniel Krugler,
                   Anthony Williams.
*/

// The way overflow is managed for ratio_less is taken from llvm/libcxx/include/ratio

#ifndef HBOOST_RATIO_RATIO_FWD_HPP
#define HBOOST_RATIO_RATIO_FWD_HPP

#include <hboost/ratio/config.hpp>

#if defined(__GNUC__) && (__GNUC__ >= 4)
#pragma GCC system_header
#endif

namespace hboost
{

//----------------------------------------------------------------------------//
//                                                                            //
//              20.6 Compile-time rational arithmetic [ratio]                 //
//                                                                            //
//----------------------------------------------------------------------------//

// ratio
template <hboost::intmax_t N, hboost::intmax_t D = 1> class ratio;

// ratio arithmetic
template <class R1, class R2> struct ratio_add;
template <class R1, class R2> struct ratio_subtract;
template <class R1, class R2> struct ratio_multiply;
template <class R1, class R2> struct ratio_divide;
#ifdef HBOOST_RATIO_EXTENSIONS
template <class R1, class R2> struct ratio_gcd;
template <class R1, class R2> struct ratio_lcm;
template <class R> struct ratio_negate;
template <class R> struct ratio_abs;
template <class R> struct ratio_sign;
template <class R, int P> struct ratio_power;
#endif

// ratio comparison
template <class R1, class R2> struct ratio_equal;
template <class R1, class R2> struct ratio_not_equal;
template <class R1, class R2> struct ratio_less;
template <class R1, class R2> struct ratio_less_equal;
template <class R1, class R2> struct ratio_greater;
template <class R1, class R2> struct ratio_greater_equal;

// convenience SI typedefs
typedef ratio<HBOOST_RATIO_INTMAX_C(1), HBOOST_RATIO_INTMAX_C(1000000000000000000)> atto;
typedef ratio<HBOOST_RATIO_INTMAX_C(1),    HBOOST_RATIO_INTMAX_C(1000000000000000)> femto;
typedef ratio<HBOOST_RATIO_INTMAX_C(1),       HBOOST_RATIO_INTMAX_C(1000000000000)> pico;
typedef ratio<HBOOST_RATIO_INTMAX_C(1),          HBOOST_RATIO_INTMAX_C(1000000000)> nano;
typedef ratio<HBOOST_RATIO_INTMAX_C(1),             HBOOST_RATIO_INTMAX_C(1000000)> micro;
typedef ratio<HBOOST_RATIO_INTMAX_C(1),                HBOOST_RATIO_INTMAX_C(1000)> milli;
typedef ratio<HBOOST_RATIO_INTMAX_C(1),                 HBOOST_RATIO_INTMAX_C(100)> centi;
typedef ratio<HBOOST_RATIO_INTMAX_C(1),                  HBOOST_RATIO_INTMAX_C(10)> deci;
typedef ratio<                 HBOOST_RATIO_INTMAX_C(10), HBOOST_RATIO_INTMAX_C(1)> deca;
typedef ratio<                HBOOST_RATIO_INTMAX_C(100), HBOOST_RATIO_INTMAX_C(1)> hecto;
typedef ratio<               HBOOST_RATIO_INTMAX_C(1000), HBOOST_RATIO_INTMAX_C(1)> kilo;
typedef ratio<            HBOOST_RATIO_INTMAX_C(1000000), HBOOST_RATIO_INTMAX_C(1)> mega;
typedef ratio<         HBOOST_RATIO_INTMAX_C(1000000000), HBOOST_RATIO_INTMAX_C(1)> giga;
typedef ratio<      HBOOST_RATIO_INTMAX_C(1000000000000), HBOOST_RATIO_INTMAX_C(1)> tera;
typedef ratio<   HBOOST_RATIO_INTMAX_C(1000000000000000), HBOOST_RATIO_INTMAX_C(1)> peta;
typedef ratio<HBOOST_RATIO_INTMAX_C(1000000000000000000), HBOOST_RATIO_INTMAX_C(1)> exa;

#ifdef HBOOST_RATIO_EXTENSIONS

#define HBOOST_RATIO_1024 HBOOST_RATIO_INTMAX_C(1024)

// convenience IEC typedefs
typedef ratio<                                                                                     HBOOST_RATIO_1024> kibi;
typedef ratio<                                                                    HBOOST_RATIO_1024*HBOOST_RATIO_1024> mebi;
typedef ratio<                                                   HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024> gibi;
typedef ratio<                                  HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024> tebi;
typedef ratio<                 HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024> pebi;
typedef ratio<HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024*HBOOST_RATIO_1024> exbi;

#endif
}  // namespace hboost


#endif  // HBOOST_RATIO_HPP
