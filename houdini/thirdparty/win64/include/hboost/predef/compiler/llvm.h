/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_LLVM_H
#define HBOOST_PREDEF_COMPILER_LLVM_H

/* Other compilers that emulate this one need to be detected first. */

#include <hboost/predef/compiler/clang.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_LLVM`]

[@http://en.wikipedia.org/wiki/LLVM LLVM] compiler.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__llvm__`] [__predef_detection__]]
    ]
 */

#define HBOOST_COMP_LLVM HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__llvm__)
#   define HBOOST_COMP_LLVM_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#ifdef HBOOST_COMP_LLVM_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_LLVM_EMULATED HBOOST_COMP_LLVM_DETECTION
#   else
#       undef HBOOST_COMP_LLVM
#       define HBOOST_COMP_LLVM HBOOST_COMP_LLVM_DETECTION
#   endif
#   define HBOOST_COMP_LLVM_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_LLVM_NAME "LLVM"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_LLVM,HBOOST_COMP_LLVM_NAME)

#ifdef HBOOST_COMP_LLVM_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_LLVM_EMULATED,HBOOST_COMP_LLVM_NAME)
#endif
