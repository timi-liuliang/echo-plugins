//  config.hpp  ---------------------------------------------------------------//

//  Copyright 2012 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_RATIO_CONFIG_HPP
#define HBOOST_RATIO_CONFIG_HPP

#include <hboost/config.hpp>
#include <hboost/cstdint.hpp>


#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 5) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#  if ! defined HBOOST_NO_CXX11_U16STRING
#    define HBOOST_NO_CXX11_U16STRING
#  endif
#  if ! defined HBOOST_NO_CXX11_U32STRING
#    define HBOOST_NO_CXX11_U32STRING
#  endif
#endif


#if !defined HBOOST_RATIO_VERSION
#define HBOOST_RATIO_VERSION 1
#else
#if HBOOST_RATIO_VERSION!=1  && HBOOST_RATIO_VERSION!=2
#error "HBOOST_RATIO_VERSION must be 1 or 2"
#endif
#endif

#if HBOOST_RATIO_VERSION==1
#if ! defined HBOOST_RATIO_DONT_PROVIDE_DEPRECATED_FEATURES_SINCE_V2_0_0
#define HBOOST_RATIO_PROVIDES_DEPRECATED_FEATURES_SINCE_V2_0_0
#endif
#endif

#if HBOOST_RATIO_VERSION==2
#if ! defined HBOOST_RATIO_PROVIDES_DEPRECATED_FEATURES_SINCE_V2_0_0
#define HBOOST_RATIO_DONT_PROVIDE_DEPRECATED_FEATURES_SINCE_V2_0_0
#endif
#endif

#ifdef INTMAX_C
#define HBOOST_RATIO_INTMAX_C(a) INTMAX_C(a)
#else
#define HBOOST_RATIO_INTMAX_C(a) a##LL
#endif

#ifdef UINTMAX_C
#define HBOOST_RATIO_UINTMAX_C(a) UINTMAX_C(a)
#else
#define HBOOST_RATIO_UINTMAX_C(a) a##ULL
#endif

#define HBOOST_RATIO_INTMAX_T_MAX (0x7FFFFFFFFFFFFFFELL)


#ifndef HBOOST_NO_CXX11_STATIC_ASSERT
#define HBOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) static_assert(CND,MSG)
#elif defined(HBOOST_RATIO_USES_STATIC_ASSERT)
#include <hboost/static_assert.hpp>
#define HBOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) HBOOST_STATIC_ASSERT(CND)
#elif defined(HBOOST_RATIO_USES_MPL_ASSERT)
#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/bool.hpp>
#define HBOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES)                                 \
    HBOOST_MPL_ASSERT_MSG(hboost::mpl::bool_< (CND) >::type::value, MSG, TYPES)
#else
//~ #elif defined(HBOOST_RATIO_USES_ARRAY_ASSERT)
#define HBOOST_RATIO_CONCAT(A,B) A##B
#define HBOOST_RATIO_NAME(A,B) HBOOST_RATIO_CONCAT(A,B)
#define HBOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) static char HBOOST_RATIO_NAME(__hboost_ratio_test_,__LINE__)[(CND)?1:-1]
//~ #define HBOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES)
#endif

#if !defined(HBOOST_NO_CXX11_STATIC_ASSERT) || !defined(HBOOST_RATIO_USES_MPL_ASSERT)
#define HBOOST_RATIO_OVERFLOW_IN_ADD "overflow in ratio add"
#define HBOOST_RATIO_OVERFLOW_IN_SUB "overflow in ratio sub"
#define HBOOST_RATIO_OVERFLOW_IN_MUL "overflow in ratio mul"
#define HBOOST_RATIO_OVERFLOW_IN_DIV "overflow in ratio div"
#define HBOOST_RATIO_NUMERATOR_IS_OUT_OF_RANGE "ratio numerator is out of range"
#define HBOOST_RATIO_DIVIDE_BY_0 "ratio divide by 0"
#define HBOOST_RATIO_DENOMINATOR_IS_OUT_OF_RANGE "ratio denominator is out of range"
#endif


//#define HBOOST_RATIO_EXTENSIONS

#endif  // header
