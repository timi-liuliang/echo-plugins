/*
Copyright Charly Chevalier 2015
Copyright Joel Falcou 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_HARDWARE_SIMD_PPC_H
#define HBOOST_PREDEF_HARDWARE_SIMD_PPC_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/hardware/simd/ppc/versions.h>

/*`
 [heading `HBOOST_HW_SIMD_PPC`]

 The SIMD extension for PowerPC (*if detected*).
 Version number depends on the most recent detected extension.

 [table
     [[__predef_symbol__] [__predef_version__]]

     [[`__VECTOR4DOUBLE__`] [__predef_detection__]]

     [[`__ALTIVEC__`] [__predef_detection__]]
     [[`__VEC__`] [__predef_detection__]]

     [[`__VSX__`] [__predef_detection__]]
     ]

 [table
     [[__predef_symbol__] [__predef_version__]]

     [[`__VECTOR4DOUBLE__`] [HBOOST_HW_SIMD_PPC_QPX_VERSION]]

     [[`__ALTIVEC__`] [HBOOST_HW_SIMD_PPC_VMX_VERSION]]
     [[`__VEC__`] [HBOOST_HW_SIMD_PPC_VMX_VERSION]]

     [[`__VSX__`] [HBOOST_HW_SIMD_PPC_VSX_VERSION]]
     ]

 */

#define HBOOST_HW_SIMD_PPC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#undef HBOOST_HW_SIMD_PPC
#if !defined(HBOOST_HW_SIMD_PPC) && defined(__VECTOR4DOUBLE__)
#   define HBOOST_HW_SIMD_PPC HBOOST_HW_SIMD_PPC_QPX_VERSION
#endif
#if !defined(HBOOST_HW_SIMD_PPC) && defined(__VSX__)
#   define HBOOST_HW_SIMD_PPC HBOOST_HW_SIMD_PPC_VSX_VERSION
#endif
#if !defined(HBOOST_HW_SIMD_PPC) && (defined(__ALTIVEC__) || defined(__VEC__))
#   define HBOOST_HW_SIMD_PPC HBOOST_HW_SIMD_PPC_VMX_VERSION
#endif

#if !defined(HBOOST_HW_SIMD_PPC)
#   define HBOOST_HW_SIMD_PPC HBOOST_VERSION_NUMBER_NOT_AVAILABLE
#else
#   define HBOOST_HW_SIMD_PPC_AVAILABLE
#endif

#define HBOOST_HW_SIMD_PPC_NAME "PPC SIMD"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_HW_SIMD_PPC, HBOOST_HW_SIMD_PPC_NAME)
