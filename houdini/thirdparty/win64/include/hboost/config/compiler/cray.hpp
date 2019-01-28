//  (C) Copyright John Maddock 2011.
//  (C) Copyright Cray, Inc. 2013
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Greenhills C compiler setup:

#define HBOOST_COMPILER "Cray C version " HBOOST_STRINGIZE(_RELEASE)

#if _RELEASE < 8
#  error "Boost is not configured for Cray compilers prior to version 8, please try the configure script."
#endif

//
// Check this is a recent EDG based compiler, otherwise we don't support it here:
//
#ifndef __EDG_VERSION__
#  error "Unsupported Cray compiler, please try running the configure script."
#endif

#include "hboost/config/compiler/common_edg.hpp"


//
//
#define HBOOST_NO_CXX11_STATIC_ASSERT
#define HBOOST_NO_CXX11_AUTO_DECLARATIONS
#define HBOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#define HBOOST_HAS_NRVO
#define HBOOST_NO_CXX11_VARIADIC_MACROS
#define HBOOST_NO_CXX11_VARIADIC_TEMPLATES
#define HBOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#define HBOOST_NO_CXX11_UNICODE_LITERALS
#define HBOOST_NO_TWO_PHASE_NAME_LOOKUP
#define HBOOST_HAS_NRVO
#define HBOOST_NO_CXX11_TEMPLATE_ALIASES
#define HBOOST_NO_CXX11_STATIC_ASSERT
#define HBOOST_NO_SFINAE_EXPR
#define HBOOST_NO_CXX11_SCOPED_ENUMS
#define HBOOST_NO_CXX11_RVALUE_REFERENCES
#define HBOOST_NO_CXX11_RANGE_BASED_FOR
#define HBOOST_NO_CXX11_RAW_LITERALS
#define HBOOST_NO_CXX11_NULLPTR
#define HBOOST_NO_CXX11_NOEXCEPT
#define HBOOST_NO_CXX11_LAMBDAS
#define HBOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#define HBOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#define HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#define HBOOST_NO_CXX11_DELETED_FUNCTIONS
#define HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#define HBOOST_NO_CXX11_DECLTYPE_N3276
#define HBOOST_NO_CXX11_DECLTYPE
#define HBOOST_NO_CXX11_CONSTEXPR
#define HBOOST_NO_CXX11_USER_DEFINED_LITERALS
#define HBOOST_NO_COMPLETE_VALUE_INITIALIZATION
#define HBOOST_NO_CXX11_CHAR32_T
#define HBOOST_NO_CXX11_CHAR16_T
#define HBOOST_NO_CXX11_REF_QUALIFIERS
#define HBOOST_NO_CXX11_FINAL


//#define HBOOST_BCB_PARTIAL_SPECIALIZATION_BUG
#define HBOOST_MATH_DISABLE_STD_FPCLASSIFY
//#define HBOOST_HAS_FPCLASSIFY

#define HBOOST_SP_USE_PTHREADS 
#define HBOOST_AC_USE_PTHREADS 

/* everything that follows is working around what are thought to be
 * compiler shortcomings.  Revist all of these regularly.
 */

//#define HBOOST_USE_ENUM_STATIC_ASSERT
//#define HBOOST_BUGGY_INTEGRAL_CONSTANT_EXPRESSIONS //(this may be implied by the previous #define

// These constants should be provided by the 
// compiler, at least when -hgnu is asserted on the command line.

#ifndef __ATOMIC_RELAXED
#define __ATOMIC_RELAXED 0
#define __ATOMIC_CONSUME 1
#define __ATOMIC_ACQUIRE 2
#define __ATOMIC_RELEASE 3
#define __ATOMIC_ACQ_REL 4
#define __ATOMIC_SEQ_CST 5
#endif



