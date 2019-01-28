/*
Copyright Charly Chevalier 2015
Copyright Joel Falcou 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_HARDWARE_SIMD_X86_AMD_VERSIONS_H
#define HBOOST_PREDEF_HARDWARE_SIMD_X86_AMD_VERSIONS_H

#include <hboost/predef/version_number.h>

/*`
 Those defines represent x86 (AMD specific) SIMD extensions versions.

 [note You *MUST* compare them with the predef `HBOOST_HW_SIMD_X86_AMD`.]
 */


// ---------------------------------

/*`
 [heading `HBOOST_HW_SIMD_X86_SSE4A_VERSION`]

 [@https://en.wikipedia.org/wiki/SSE4##SSE4A SSE4A] x86 extension (AMD specific).

 Version number is: *4.0.0*.
 */
#define HBOOST_HW_SIMD_X86_AMD_SSE4A_VERSION HBOOST_VERSION_NUMBER(4, 0, 0)

/*`
 [heading `HBOOST_HW_SIMD_X86_FMA4_VERSION`]

 [@https://en.wikipedia.org/wiki/FMA_instruction_set#FMA4_instruction_set FMA4] x86 extension (AMD specific).

 Version number is: *5.1.0*.
 */
#define HBOOST_HW_SIMD_X86_AMD_FMA4_VERSION HBOOST_VERSION_NUMBER(5, 1, 0)

/*`
 [heading `HBOOST_HW_SIMD_X86_XOP_VERSION`]

 [@https://en.wikipedia.org/wiki/XOP_instruction_set XOP] x86 extension (AMD specific).

 Version number is: *5.1.1*.
 */
#define HBOOST_HW_SIMD_X86_AMD_XOP_VERSION HBOOST_VERSION_NUMBER(5, 1, 1)


#endif
