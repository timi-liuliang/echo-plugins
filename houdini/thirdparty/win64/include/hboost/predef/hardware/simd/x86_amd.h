/*
Copyright Charly Chevalier 2015
Copyright Joel Falcou 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_HARDWARE_SIMD_X86_AMD_H
#define HBOOST_PREDEF_HARDWARE_SIMD_X86_AMD_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/hardware/simd/x86_amd/versions.h>

/*`
 [heading `HBOOST_HW_SIMD_X86_AMD`]

 The SIMD extension for x86 (AMD) (*if detected*).
 Version number depends on the most recent detected extension.

 [table
     [[__predef_symbol__] [__predef_version__]]

     [[`__SSE4A__`] [__predef_detection__]]

     [[`__FMA4__`] [__predef_detection__]]

     [[`__XOP__`] [__predef_detection__]]

     [[`HBOOST_HW_SIMD_X86`] [__predef_detection__]]
     ]

 [table
     [[__predef_symbol__] [__predef_version__]]

     [[`__SSE4A__`] [HBOOST_HW_SIMD_X86_SSE4A_VERSION]]

     [[`__FMA4__`] [HBOOST_HW_SIMD_X86_FMA4_VERSION]]

     [[`__XOP__`] [HBOOST_HW_SIMD_X86_XOP_VERSION]]

     [[`HBOOST_HW_SIMD_X86`] [HBOOST_HW_SIMD_X86]]
     ]

 [note This predef includes every other x86 SIMD extensions and also has other
 more specific extensions (FMA4, XOP, SSE4a). You should use this predef
 instead of `HBOOST_HW_SIMD_X86` to test if those specific extensions have
 been detected.]

 */

#define HBOOST_HW_SIMD_X86_AMD HBOOST_VERSION_NUMBER_NOT_AVAILABLE

// AMD CPUs also use x86 architecture. We first try to detect if any AMD
// specific extension are detected, if yes, then try to detect more recent x86
// common extensions.

#undef HBOOST_HW_SIMD_X86_AMD
#if !defined(HBOOST_HW_SIMD_X86_AMD) && defined(__XOP__)
#   define HBOOST_HW_SIMD_X86_AMD HBOOST_HW_SIMD_X86_AMD_XOP_VERSION
#endif
#if !defined(HBOOST_HW_SIMD_X86_AMD) && defined(__FMA4__)
#   define HBOOST_HW_SIMD_X86_AMD HBOOST_HW_SIMD_X86_AMD_FMA4_VERSION
#endif
#if !defined(HBOOST_HW_SIMD_X86_AMD) && defined(__SSE4A__)
#   define HBOOST_HW_SIMD_X86_AMD HBOOST_HW_SIMD_X86_AMD_SSE4A_VERSION
#endif

#if !defined(HBOOST_HW_SIMD_X86_AMD)
#   define HBOOST_HW_SIMD_X86_AMD HBOOST_VERSION_NUMBER_NOT_AVAILABLE
#else
    // At this point, we know that we have an AMD CPU, we do need to check for
    // other x86 extensions to determine the final version number.
#   include <hboost/predef/hardware/simd/x86.h>
#   if HBOOST_HW_SIMD_X86 > HBOOST_HW_SIMD_X86_AMD
#      undef HBOOST_HW_SIMD_X86_AMD
#      define HBOOST_HW_SIMD_X86_AMD HBOOST_HW_SIMD_X86
#   endif
#   define HBOOST_HW_SIMD_X86_AMD_AVAILABLE
#endif

#define HBOOST_HW_SIMD_X86_AMD_NAME "x86 (AMD) SIMD"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_HW_SIMD_X86_AMD, HBOOST_HW_SIMD_X86_AMD_NAME)
