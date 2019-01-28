//  (C) Copyright John Maddock 2001 - 2003.
//  (C) Copyright Darin Adler 2001 - 2002.
//  (C) Copyright Peter Dimov 2001.
//  (C) Copyright Aleksey Gurtovoy 2002.
//  (C) Copyright David Abrahams 2002 - 2003.
//  (C) Copyright Beman Dawes 2002 - 2003.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.
//
//  Microsoft Visual C++ compiler setup:
//
//  We need to be careful with the checks in this file, as contrary
//  to popular belief there are versions with _MSC_VER with the final
//  digit non-zero (mainly the MIPS cross compiler).
//
//  So we either test _MSC_VER >= XXXX or else _MSC_VER < XXXX.
//  No other comparisons (==, >, or <=) are safe.
//

#define HBOOST_MSVC _MSC_VER

//
// Helper macro HBOOST_MSVC_FULL_VER for use in Boost code:
//
#if _MSC_FULL_VER > 100000000
#  define HBOOST_MSVC_FULL_VER _MSC_FULL_VER
#else
#  define HBOOST_MSVC_FULL_VER (_MSC_FULL_VER * 10)
#endif

// Attempt to suppress VC6 warnings about the length of decorated names (obsolete):
#pragma warning( disable : 4503 ) // warning: decorated name length exceeded

#define HBOOST_HAS_PRAGMA_ONCE

//
// versions check:
// we don't support Visual C++ prior to version 7.1:
#if _MSC_VER < 1310
#  error "Compiler not supported or configured - please reconfigure"
#endif

#if _MSC_FULL_VER < 180020827
#  define HBOOST_NO_FENV_H
#endif

#if _MSC_VER < 1400
// although a conforming signature for swprint exists in VC7.1
// it appears not to actually work:
#  define HBOOST_NO_SWPRINTF
// Our extern template tests also fail for this compiler:
#  define HBOOST_NO_CXX11_EXTERN_TEMPLATE
// Variadic macros do not exist for VC7.1 and lower
#  define HBOOST_NO_CXX11_VARIADIC_MACROS
#  define HBOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#endif

#if _MSC_VER < 1500  // 140X == VC++ 8.0
#  define HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
#endif

#if _MSC_VER < 1600  // 150X == VC++ 9.0
   // A bug in VC9:
#  define HBOOST_NO_ADL_BARRIER
#endif


#ifndef _NATIVE_WCHAR_T_DEFINED
#  define HBOOST_NO_INTRINSIC_WCHAR_T
#endif

//
// check for exception handling support:
#if !defined(_CPPUNWIND) && !defined(HBOOST_NO_EXCEPTIONS)
#  define HBOOST_NO_EXCEPTIONS
#endif

//
// __int64 support:
//
#define HBOOST_HAS_MS_INT64
#if defined(_MSC_EXTENSIONS) || (_MSC_VER >= 1400)
#   define HBOOST_HAS_LONG_LONG
#else
#   define HBOOST_NO_LONG_LONG
#endif
#if (_MSC_VER >= 1400) && !defined(_DEBUG)
#   define HBOOST_HAS_NRVO
#endif
#if _MSC_VER >= 1600  // 160X == VC++ 10.0
#  define HBOOST_HAS_PRAGMA_DETECT_MISMATCH
#endif
//
// disable Win32 API's if compiler extensions are
// turned off:
//
#if !defined(_MSC_EXTENSIONS) && !defined(HBOOST_DISABLE_WIN32)
#  define HBOOST_DISABLE_WIN32
#endif
#if !defined(_CPPRTTI) && !defined(HBOOST_NO_RTTI)
#  define HBOOST_NO_RTTI
#endif

//
// TR1 features:
//
#if (_MSC_VER >= 1700) && defined(_HAS_CXX17) && (_HAS_CXX17 > 0)
// # define HBOOST_HAS_TR1_HASH			// don't know if this is true yet.
// # define HBOOST_HAS_TR1_TYPE_TRAITS	// don't know if this is true yet.
# define HBOOST_HAS_TR1_UNORDERED_MAP
# define HBOOST_HAS_TR1_UNORDERED_SET
#endif

//
// C++0x features
//
//   See above for HBOOST_NO_LONG_LONG

// C++ features supported by VC++ 10 (aka 2010)
//
#if _MSC_VER < 1600
#  define HBOOST_NO_CXX11_AUTO_DECLARATIONS
#  define HBOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#  define HBOOST_NO_CXX11_LAMBDAS
#  define HBOOST_NO_CXX11_RVALUE_REFERENCES
#  define HBOOST_NO_CXX11_STATIC_ASSERT
#  define HBOOST_NO_CXX11_NULLPTR
#  define HBOOST_NO_CXX11_DECLTYPE
#endif // _MSC_VER < 1600

#if _MSC_VER >= 1600
#  define HBOOST_HAS_STDINT_H
#endif

// C++11 features supported by VC++ 11 (aka 2012)
//
#if _MSC_VER < 1700
#  define HBOOST_NO_CXX11_FINAL
#  define HBOOST_NO_CXX11_RANGE_BASED_FOR
#  define HBOOST_NO_CXX11_SCOPED_ENUMS
#endif // _MSC_VER < 1700

// C++11 features supported by VC++ 12 (aka 2013).
//
#if _MSC_FULL_VER < 180020827
#  define HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#  define HBOOST_NO_CXX11_DELETED_FUNCTIONS
#  define HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#  define HBOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  define HBOOST_NO_CXX11_RAW_LITERALS
#  define HBOOST_NO_CXX11_TEMPLATE_ALIASES
#  define HBOOST_NO_CXX11_TRAILING_RESULT_TYPES
#  define HBOOST_NO_CXX11_VARIADIC_TEMPLATES
#  define HBOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#  define HBOOST_NO_CXX11_DECLTYPE_N3276
#endif

#if _MSC_FULL_VER >= 180020827
#define HBOOST_HAS_EXPM1
#define HBOOST_HAS_LOG1P
#endif

// C++11 features supported by VC++ 14 (aka 2015)
//
#if (_MSC_FULL_VER < 190023026)
#  define HBOOST_NO_CXX11_NOEXCEPT
#  define HBOOST_NO_CXX11_REF_QUALIFIERS
#  define HBOOST_NO_CXX11_USER_DEFINED_LITERALS
#  define HBOOST_NO_CXX11_ALIGNAS
#  define HBOOST_NO_CXX11_INLINE_NAMESPACES
#  define HBOOST_NO_CXX11_CHAR16_T
#  define HBOOST_NO_CXX11_CHAR32_T
#  define HBOOST_NO_CXX11_UNICODE_LITERALS
#  define HBOOST_NO_CXX14_DECLTYPE_AUTO
#  define HBOOST_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
#  define HBOOST_NO_CXX14_RETURN_TYPE_DEDUCTION
#  define HBOOST_NO_CXX14_BINARY_LITERALS
#  define HBOOST_NO_CXX14_GENERIC_LAMBDAS
#  define HBOOST_NO_CXX14_DIGIT_SEPARATORS
#  define HBOOST_NO_CXX11_THREAD_LOCAL
#endif
// C++11 features supported by VC++ 14 update 3 (aka 2015)
//
#if (_MSC_FULL_VER < 190024210)
#  define HBOOST_NO_CXX14_VARIABLE_TEMPLATES
#  define HBOOST_NO_SFINAE_EXPR
#  define HBOOST_NO_CXX11_CONSTEXPR
#endif

// C++14 features supported by VC++ 14.1 (Visual Studio 2017)
//
#if (_MSC_VER < 1910)
#  define HBOOST_NO_CXX14_AGGREGATE_NSDMI
#endif

// C++17 features supported by VC++ 14.1 (Visual Studio 2017) Update 3
//
#if (_MSC_VER < 1911) || (_MSVC_LANG < 201703)
#  define HBOOST_NO_CXX17_STRUCTURED_BINDINGS
#endif

// MSVC including version 14 has not yet completely
// implemented value-initialization, as is reported:
// "VC++ does not value-initialize members of derived classes without
// user-declared constructor", reported in 2009 by Sylvester Hesp:
// https://connect.microsoft.com/VisualStudio/feedback/details/484295
// "Presence of copy constructor breaks member class initialization",
// reported in 2009 by Alex Vakulenko:
// https://connect.microsoft.com/VisualStudio/feedback/details/499606
// "Value-initialization in new-expression", reported in 2005 by
// Pavel Kuznetsov (MetaCommunications Engineering):
// https://connect.microsoft.com/VisualStudio/feedback/details/100744
// Reported again by John Maddock in 2015 for VC14:
// https://connect.microsoft.com/VisualStudio/feedback/details/1582233/c-subobjects-still-not-value-initialized-correctly
// See also: http://www.boost.org/libs/utility/value_init.htm#compiler_issues
// (Niels Dekker, LKEB, May 2010)
// Still present in VC15.5, Dec 2017.
#define HBOOST_NO_COMPLETE_VALUE_INITIALIZATION
//
// C++ 11:
//
// This is supported with /permissive- for 15.5 onwards, unfortunately we appear to have no way to tell
// if this is in effect or not, in any case nothing in Boost is currently using this, so we'll just go
// on defining it for now:
//
#define HBOOST_NO_TWO_PHASE_NAME_LOOKUP

#if (_MSC_VER < 1912) || (_MSVC_LANG < 201402)
// Supported from msvc-15.5 onwards:
#define HBOOST_NO_CXX11_SFINAE_EXPR
#endif
// C++ 14:
// Still gives internal compiler error for msvc-15.5:
#  define HBOOST_NO_CXX14_CONSTEXPR
// C++ 17:
#if (_MSC_VER < 1912) || (_MSVC_LANG < 201703)
#define HBOOST_NO_CXX17_INLINE_VARIABLES
#define HBOOST_NO_CXX17_FOLD_EXPRESSIONS
#endif

//
// Things that don't work in clr mode:
//
#ifdef _M_CEE
#ifndef HBOOST_NO_CXX11_THREAD_LOCAL
#  define HBOOST_NO_CXX11_THREAD_LOCAL
#endif
#ifndef HBOOST_NO_SFINAE_EXPR
#  define HBOOST_NO_SFINAE_EXPR
#endif
#ifndef HBOOST_NO_CXX11_REF_QUALIFIERS
#  define HBOOST_NO_CXX11_REF_QUALIFIERS
#endif
#endif
#ifdef _M_CEE_PURE
#ifndef HBOOST_NO_CXX11_CONSTEXPR
#  define HBOOST_NO_CXX11_CONSTEXPR
#endif
#endif

//
// prefix and suffix headers:
//
#ifndef HBOOST_ABI_PREFIX
#  define HBOOST_ABI_PREFIX "hboost/config/abi/msvc_prefix.hpp"
#endif
#ifndef HBOOST_ABI_SUFFIX
#  define HBOOST_ABI_SUFFIX "hboost/config/abi/msvc_suffix.hpp"
#endif

#ifndef HBOOST_COMPILER
// TODO:
// these things are mostly bogus. 1200 means version 12.0 of the compiler. The
// artificial versions assigned to them only refer to the versions of some IDE
// these compilers have been shipped with, and even that is not all of it. Some
// were shipped with freely downloadable SDKs, others as crosscompilers in eVC.
// IOW, you can't use these 'versions' in any sensible way. Sorry.
# if defined(UNDER_CE)
#   if _MSC_VER < 1400
      // Note: I'm not aware of any CE compiler with version 13xx
#      if defined(HBOOST_ASSERT_CONFIG)
#         error "Unknown EVC++ compiler version - please run the configure tests and report the results"
#      else
#         pragma message("Unknown EVC++ compiler version - please run the configure tests and report the results")
#      endif
#   elif _MSC_VER < 1500
#     define HBOOST_COMPILER_VERSION evc8
#   elif _MSC_VER < 1600
#     define HBOOST_COMPILER_VERSION evc9
#   elif _MSC_VER < 1700
#     define HBOOST_COMPILER_VERSION evc10
#   elif _MSC_VER < 1800 
#     define HBOOST_COMPILER_VERSION evc11 
#   elif _MSC_VER < 1900 
#     define HBOOST_COMPILER_VERSION evc12
#   elif _MSC_VER < 2000  
#     define HBOOST_COMPILER_VERSION evc14
#   else
#      if defined(HBOOST_ASSERT_CONFIG)
#         error "Unknown EVC++ compiler version - please run the configure tests and report the results"
#      else
#         pragma message("Unknown EVC++ compiler version - please run the configure tests and report the results")
#      endif
#   endif
# else
#   if _MSC_VER < 1200
      // Note: Versions up to 7.0 aren't supported.
#     define HBOOST_COMPILER_VERSION 5.0
#   elif _MSC_VER < 1300
#     define HBOOST_COMPILER_VERSION 6.0
#   elif _MSC_VER < 1310
#     define HBOOST_COMPILER_VERSION 7.0
#   elif _MSC_VER < 1400
#     define HBOOST_COMPILER_VERSION 7.1
#   elif _MSC_VER < 1500
#     define HBOOST_COMPILER_VERSION 8.0
#   elif _MSC_VER < 1600
#     define HBOOST_COMPILER_VERSION 9.0
#   elif _MSC_VER < 1700
#     define HBOOST_COMPILER_VERSION 10.0
#   elif _MSC_VER < 1800 
#     define HBOOST_COMPILER_VERSION 11.0
#   elif _MSC_VER < 1900
#     define HBOOST_COMPILER_VERSION 12.0
#   elif _MSC_VER < 1910
#     define HBOOST_COMPILER_VERSION 14.0
#   elif _MSC_VER < 1920
#     define HBOOST_COMPILER_VERSION 14.1
#   else
#     define HBOOST_COMPILER_VERSION _MSC_VER
#   endif
# endif

#  define HBOOST_COMPILER "Microsoft Visual C++ version " HBOOST_STRINGIZE(HBOOST_COMPILER_VERSION)
#endif

//
// accept any 19.1x version for VS2017:
#if (_MSC_VER > 1920)
#  if defined(HBOOST_ASSERT_CONFIG)
#     error "Boost.Config is older than your current compiler version."
#  else
#     pragma message("Info: Boost.Config is older than your compiler version - probably nothing bad will happen - but you may wish to look for an updated Boost version.")
#  endif
#endif
