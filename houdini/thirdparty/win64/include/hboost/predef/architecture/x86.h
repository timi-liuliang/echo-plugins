/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <hboost/predef/architecture/x86/32.h>
#include <hboost/predef/architecture/x86/64.h>

#ifndef HBOOST_PREDEF_ARCHITECTURE_X86_H
#define HBOOST_PREDEF_ARCHITECTURE_X86_H

/*`
[heading `HBOOST_ARCH_X86`]

[@http://en.wikipedia.org/wiki/X86 Intel x86] architecture. This is
a category to indicate that either `HBOOST_ARCH_X86_32` or
`HBOOST_ARCH_X86_64` is detected.
 */

#define HBOOST_ARCH_X86 HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if HBOOST_ARCH_X86_32 || HBOOST_ARCH_X86_64
#   undef HBOOST_ARCH_X86
#   define HBOOST_ARCH_X86 HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_ARCH_X86
#   define HBOOST_ARCH_X86_AVAILABLE
#endif

#define HBOOST_ARCH_X86_NAME "Intel x86"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_ARCH_X86,HBOOST_ARCH_X86_NAME)
