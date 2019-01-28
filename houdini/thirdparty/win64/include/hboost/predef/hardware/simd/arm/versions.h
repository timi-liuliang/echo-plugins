/*
Copyright Charly Chevalier 2015
Copyright Joel Falcou 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_HARDWARE_SIMD_ARM_VERSIONS_H
#define HBOOST_PREDEF_HARDWARE_SIMD_ARM_VERSIONS_H

#include <hboost/predef/version_number.h>

/*`
 Those defines represent ARM SIMD extensions versions.

 [note You *MUST* compare them with the predef `HBOOST_HW_SIMD_ARM`.]
 */

// ---------------------------------

/*`
 [heading `HBOOST_HW_SIMD_ARM_NEON_VERSION`]

 The [@https://en.wikipedia.org/wiki/ARM_architecture#Advanced_SIMD_.28NEON.29 NEON]
 ARM extension version number.

 Version number is: *1.0.0*.
 */
#define HBOOST_HW_SIMD_ARM_NEON_VERSION HBOOST_VERSION_NUMBER(1, 0, 0)

#endif
