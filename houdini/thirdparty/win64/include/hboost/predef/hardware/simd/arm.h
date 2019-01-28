/*
Copyright Charly Chevalier 2015
Copyright Joel Falcou 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_HARDWARE_SIMD_ARM_H
#define HBOOST_PREDEF_HARDWARE_SIMD_ARM_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/hardware/simd/arm/versions.h>

/*`
 [heading `HBOOST_HW_SIMD_ARM`]

 The SIMD extension for ARM (*if detected*).
 Version number depends on the most recent detected extension.

 [table
     [[__predef_symbol__] [__predef_version__]]

     [[`__ARM_NEON__`] [__predef_detection__]]
     [[`__aarch64__`] [__predef_detection__]]
     [[`_M_ARM`] [__predef_detection__]]
     ]

 [table
     [[__predef_symbol__] [__predef_version__]]

     [[`__ARM_NEON__`] [HBOOST_HW_SIMD_ARM_NEON_VERSION]]
     [[`__aarch64__`] [HBOOST_HW_SIMD_ARM_NEON_VERSION]]
     [[`_M_ARM`] [HBOOST_HW_SIMD_ARM_NEON_VERSION]]
     ]

 */

#define HBOOST_HW_SIMD_ARM HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#undef HBOOST_HW_SIMD_ARM
#if !defined(HBOOST_HW_SIMD_ARM) && (defined(__ARM_NEON__) || defined(__aarch64__) || defined (_M_ARM))
#   define HBOOST_HW_SIMD_ARM HBOOST_HW_SIMD_ARM_NEON_VERSION
#endif

#if !defined(HBOOST_HW_SIMD_ARM)
#   define HBOOST_HW_SIMD_ARM HBOOST_VERSION_NUMBER_NOT_AVAILABLE
#else
#   define HBOOST_HW_SIMD_ARM_AVAILABLE
#endif

#define HBOOST_HW_SIMD_ARM_NAME "ARM SIMD"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_HW_SIMD_ARM, HBOOST_HW_SIMD_ARM_NAME)
