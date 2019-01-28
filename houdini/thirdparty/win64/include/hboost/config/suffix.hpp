//  Boost config.hpp configuration header file  ------------------------------//
//  boostinspect:ndprecated_macros -- tell the inspect tool to ignore this file

//  Copyright (c) 2001-2003 John Maddock
//  Copyright (c) 2001 Darin Adler
//  Copyright (c) 2001 Peter Dimov
//  Copyright (c) 2002 Bill Kempf
//  Copyright (c) 2002 Jens Maurer
//  Copyright (c) 2002-2003 David Abrahams
//  Copyright (c) 2003 Gennaro Prota
//  Copyright (c) 2003 Eric Friedman
//  Copyright (c) 2010 Eric Jourdanneau, Joel Falcou
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for most recent version.

//  Boost config.hpp policy and rationale documentation has been moved to
//  http://www.boost.org/libs/config/
//
//  This file is intended to be stable, and relatively unchanging.
//  It should contain boilerplate code only - no compiler specific
//  code unless it is unavoidable - no changes unless unavoidable.

#ifndef HBOOST_CONFIG_SUFFIX_HPP
#define HBOOST_CONFIG_SUFFIX_HPP

#if defined(__GNUC__) && (__GNUC__ >= 4)
//
// Some GCC-4.x versions issue warnings even when __extension__ is used,
// so use this as a workaround:
//
#pragma GCC system_header
#endif

//
// ensure that visibility macros are always defined, thus symplifying use
//
#ifndef HBOOST_SYMBOL_EXPORT
# define HBOOST_SYMBOL_EXPORT
#endif
#ifndef HBOOST_SYMBOL_IMPORT
# define HBOOST_SYMBOL_IMPORT
#endif
#ifndef HBOOST_SYMBOL_VISIBLE
# define HBOOST_SYMBOL_VISIBLE
#endif

//
// look for long long by looking for the appropriate macros in <limits.h>.
// Note that we use limits.h rather than climits for maximal portability,
// remember that since these just declare a bunch of macros, there should be
// no namespace issues from this.
//
#if !defined(HBOOST_HAS_LONG_LONG) && !defined(HBOOST_NO_LONG_LONG)                                              \
   && !defined(HBOOST_MSVC) && !defined(__BORLANDC__)
# include <limits.h>
# if (defined(ULLONG_MAX) || defined(ULONG_LONG_MAX) || defined(ULONGLONG_MAX))
#   define HBOOST_HAS_LONG_LONG
# else
#   define HBOOST_NO_LONG_LONG
# endif
#endif

// GCC 3.x will clean up all of those nasty macro definitions that
// HBOOST_NO_CTYPE_FUNCTIONS is intended to help work around, so undefine
// it under GCC 3.x.
#if defined(__GNUC__) && (__GNUC__ >= 3) && defined(HBOOST_NO_CTYPE_FUNCTIONS)
#  undef HBOOST_NO_CTYPE_FUNCTIONS
#endif

//
// Assume any extensions are in namespace std:: unless stated otherwise:
//
#  ifndef HBOOST_STD_EXTENSION_NAMESPACE
#    define HBOOST_STD_EXTENSION_NAMESPACE std
#  endif

//
// If cv-qualified specializations are not allowed, then neither are cv-void ones:
//
#  if defined(HBOOST_NO_CV_SPECIALIZATIONS) \
      && !defined(HBOOST_NO_CV_VOID_SPECIALIZATIONS)
#     define HBOOST_NO_CV_VOID_SPECIALIZATIONS
#  endif

//
// If there is no numeric_limits template, then it can't have any compile time
// constants either!
//
#  if defined(HBOOST_NO_LIMITS) \
      && !defined(HBOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS)
#     define HBOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#     define HBOOST_NO_MS_INT64_NUMERIC_LIMITS
#     define HBOOST_NO_LONG_LONG_NUMERIC_LIMITS
#  endif

//
// if there is no long long then there is no specialisation
// for numeric_limits<long long> either:
//
#if !defined(HBOOST_HAS_LONG_LONG) && !defined(HBOOST_NO_LONG_LONG_NUMERIC_LIMITS)
#  define HBOOST_NO_LONG_LONG_NUMERIC_LIMITS
#endif

//
// if there is no __int64 then there is no specialisation
// for numeric_limits<__int64> either:
//
#if !defined(HBOOST_HAS_MS_INT64) && !defined(HBOOST_NO_MS_INT64_NUMERIC_LIMITS)
#  define HBOOST_NO_MS_INT64_NUMERIC_LIMITS
#endif

//
// if member templates are supported then so is the
// VC6 subset of member templates:
//
#  if !defined(HBOOST_NO_MEMBER_TEMPLATES) \
       && !defined(HBOOST_MSVC6_MEMBER_TEMPLATES)
#     define HBOOST_MSVC6_MEMBER_TEMPLATES
#  endif

//
// Without partial specialization, can't test for partial specialisation bugs:
//
#  if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(HBOOST_BCB_PARTIAL_SPECIALIZATION_BUG)
#     define HBOOST_BCB_PARTIAL_SPECIALIZATION_BUG
#  endif

//
// Without partial specialization, we can't have array-type partial specialisations:
//
#  if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(HBOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
#     define HBOOST_NO_ARRAY_TYPE_SPECIALIZATIONS
#  endif

//
// Without partial specialization, std::iterator_traits can't work:
//
#  if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(HBOOST_NO_STD_ITERATOR_TRAITS)
#     define HBOOST_NO_STD_ITERATOR_TRAITS
#  endif

//
// Without partial specialization, partial
// specialization with default args won't work either:
//
#  if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(HBOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
#     define HBOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#  endif

//
// Without member template support, we can't have template constructors
// in the standard library either:
//
#  if defined(HBOOST_NO_MEMBER_TEMPLATES) \
      && !defined(HBOOST_MSVC6_MEMBER_TEMPLATES) \
      && !defined(HBOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
#     define HBOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#  endif

//
// Without member template support, we can't have a conforming
// std::allocator template either:
//
#  if defined(HBOOST_NO_MEMBER_TEMPLATES) \
      && !defined(HBOOST_MSVC6_MEMBER_TEMPLATES) \
      && !defined(HBOOST_NO_STD_ALLOCATOR)
#     define HBOOST_NO_STD_ALLOCATOR
#  endif

//
// without ADL support then using declarations will break ADL as well:
//
#if defined(HBOOST_NO_ARGUMENT_DEPENDENT_LOOKUP) && !defined(HBOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL)
#  define HBOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#endif

//
// Without typeid support we have no dynamic RTTI either:
//
#if defined(HBOOST_NO_TYPEID) && !defined(HBOOST_NO_RTTI)
#  define HBOOST_NO_RTTI
#endif

//
// If we have a standard allocator, then we have a partial one as well:
//
#if !defined(HBOOST_NO_STD_ALLOCATOR)
#  define HBOOST_HAS_PARTIAL_STD_ALLOCATOR
#endif

//
// We can't have a working std::use_facet if there is no std::locale:
//
#  if defined(HBOOST_NO_STD_LOCALE) && !defined(HBOOST_NO_STD_USE_FACET)
#     define HBOOST_NO_STD_USE_FACET
#  endif

//
// We can't have a std::messages facet if there is no std::locale:
//
#  if defined(HBOOST_NO_STD_LOCALE) && !defined(HBOOST_NO_STD_MESSAGES)
#     define HBOOST_NO_STD_MESSAGES
#  endif

//
// We can't have a working std::wstreambuf if there is no std::locale:
//
#  if defined(HBOOST_NO_STD_LOCALE) && !defined(HBOOST_NO_STD_WSTREAMBUF)
#     define HBOOST_NO_STD_WSTREAMBUF
#  endif

//
// We can't have a <cwctype> if there is no <cwchar>:
//
#  if defined(HBOOST_NO_CWCHAR) && !defined(HBOOST_NO_CWCTYPE)
#     define HBOOST_NO_CWCTYPE
#  endif

//
// We can't have a swprintf if there is no <cwchar>:
//
#  if defined(HBOOST_NO_CWCHAR) && !defined(HBOOST_NO_SWPRINTF)
#     define HBOOST_NO_SWPRINTF
#  endif

//
// If Win32 support is turned off, then we must turn off
// threading support also, unless there is some other
// thread API enabled:
//
#if defined(HBOOST_DISABLE_WIN32) && defined(_WIN32) \
   && !defined(HBOOST_DISABLE_THREADS) && !defined(HBOOST_HAS_PTHREADS)
#  define HBOOST_DISABLE_THREADS
#endif

//
// Turn on threading support if the compiler thinks that it's in
// multithreaded mode.  We put this here because there are only a
// limited number of macros that identify this (if there's any missing
// from here then add to the appropriate compiler section):
//
#if (defined(__MT__) || defined(_MT) || defined(_REENTRANT) \
    || defined(_PTHREADS) || defined(__APPLE__) || defined(__DragonFly__)) \
    && !defined(HBOOST_HAS_THREADS)
#  define HBOOST_HAS_THREADS
#endif

//
// Turn threading support off if HBOOST_DISABLE_THREADS is defined:
//
#if defined(HBOOST_DISABLE_THREADS) && defined(HBOOST_HAS_THREADS)
#  undef HBOOST_HAS_THREADS
#endif

//
// Turn threading support off if we don't recognise the threading API:
//
#if defined(HBOOST_HAS_THREADS) && !defined(HBOOST_HAS_PTHREADS)\
      && !defined(HBOOST_HAS_WINTHREADS) && !defined(HBOOST_HAS_BETHREADS)\
      && !defined(HBOOST_HAS_MPTASKS)
#  undef HBOOST_HAS_THREADS
#endif

//
// Turn threading detail macros off if we don't (want to) use threading
//
#ifndef HBOOST_HAS_THREADS
#  undef HBOOST_HAS_PTHREADS
#  undef HBOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#  undef HBOOST_HAS_PTHREAD_YIELD
#  undef HBOOST_HAS_PTHREAD_DELAY_NP
#  undef HBOOST_HAS_WINTHREADS
#  undef HBOOST_HAS_BETHREADS
#  undef HBOOST_HAS_MPTASKS
#endif

//
// If the compiler claims to be C99 conformant, then it had better
// have a <stdint.h>:
//
#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#     define HBOOST_HAS_STDINT_H
#     ifndef HBOOST_HAS_LOG1P
#        define HBOOST_HAS_LOG1P
#     endif
#     ifndef HBOOST_HAS_EXPM1
#        define HBOOST_HAS_EXPM1
#     endif
#  endif

//
// Define HBOOST_NO_SLIST and HBOOST_NO_HASH if required.
// Note that this is for backwards compatibility only.
//
#  if !defined(HBOOST_HAS_SLIST) && !defined(HBOOST_NO_SLIST)
#     define HBOOST_NO_SLIST
#  endif

#  if !defined(HBOOST_HAS_HASH) && !defined(HBOOST_NO_HASH)
#     define HBOOST_NO_HASH
#  endif

//
// Set HBOOST_SLIST_HEADER if not set already:
//
#if defined(HBOOST_HAS_SLIST) && !defined(HBOOST_SLIST_HEADER)
#  define HBOOST_SLIST_HEADER <slist>
#endif

//
// Set HBOOST_HASH_SET_HEADER if not set already:
//
#if defined(HBOOST_HAS_HASH) && !defined(HBOOST_HASH_SET_HEADER)
#  define HBOOST_HASH_SET_HEADER <hash_set>
#endif

//
// Set HBOOST_HASH_MAP_HEADER if not set already:
//
#if defined(HBOOST_HAS_HASH) && !defined(HBOOST_HASH_MAP_HEADER)
#  define HBOOST_HASH_MAP_HEADER <hash_map>
#endif

//  HBOOST_HAS_ABI_HEADERS
//  This macro gets set if we have headers that fix the ABI,
//  and prevent ODR violations when linking to external libraries:
#if defined(HBOOST_ABI_PREFIX) && defined(HBOOST_ABI_SUFFIX) && !defined(HBOOST_HAS_ABI_HEADERS)
#  define HBOOST_HAS_ABI_HEADERS
#endif

#if defined(HBOOST_HAS_ABI_HEADERS) && defined(HBOOST_DISABLE_ABI_HEADERS)
#  undef HBOOST_HAS_ABI_HEADERS
#endif

//  HBOOST_NO_STDC_NAMESPACE workaround  --------------------------------------//
//  Because std::size_t usage is so common, even in boost headers which do not
//  otherwise use the C library, the <cstddef> workaround is included here so
//  that ugly workaround code need not appear in many other boost headers.
//  NOTE WELL: This is a workaround for non-conforming compilers; <cstddef>
//  must still be #included in the usual places so that <cstddef> inclusion
//  works as expected with standard conforming compilers.  The resulting
//  double inclusion of <cstddef> is harmless.

# if defined(HBOOST_NO_STDC_NAMESPACE) && defined(__cplusplus)
#   include <cstddef>
    namespace std { using ::ptrdiff_t; using ::size_t; }
# endif

//  Workaround for the unfortunate min/max macros defined by some platform headers

#define HBOOST_PREVENT_MACRO_SUBSTITUTION

#ifndef HBOOST_USING_STD_MIN
#  define HBOOST_USING_STD_MIN() using std::min
#endif

#ifndef HBOOST_USING_STD_MAX
#  define HBOOST_USING_STD_MAX() using std::max
#endif

//  HBOOST_NO_STD_MIN_MAX workaround  -----------------------------------------//

#  if defined(HBOOST_NO_STD_MIN_MAX) && defined(__cplusplus)

namespace std {
  template <class _Tp>
  inline const _Tp& min HBOOST_PREVENT_MACRO_SUBSTITUTION (const _Tp& __a, const _Tp& __b) {
    return __b < __a ? __b : __a;
  }
  template <class _Tp>
  inline const _Tp& max HBOOST_PREVENT_MACRO_SUBSTITUTION (const _Tp& __a, const _Tp& __b) {
    return  __a < __b ? __b : __a;
  }
}

#  endif

// HBOOST_STATIC_CONSTANT workaround --------------------------------------- //
// On compilers which don't allow in-class initialization of static integral
// constant members, we must use enums as a workaround if we want the constants
// to be available at compile-time. This macro gives us a convenient way to
// declare such constants.

#  ifdef HBOOST_NO_INCLASS_MEMBER_INITIALIZATION
#       define HBOOST_STATIC_CONSTANT(type, assignment) enum { assignment }
#  else
#     define HBOOST_STATIC_CONSTANT(type, assignment) static const type assignment
#  endif

// HBOOST_USE_FACET / HAS_FACET workaround ----------------------------------//
// When the standard library does not have a conforming std::use_facet there
// are various workarounds available, but they differ from library to library.
// The same problem occurs with has_facet.
// These macros provide a consistent way to access a locale's facets.
// Usage:
//    replace
//       std::use_facet<Type>(loc);
//    with
//       HBOOST_USE_FACET(Type, loc);
//    Note do not add a std:: prefix to the front of HBOOST_USE_FACET!
//  Use for HBOOST_HAS_FACET is analogous.

#if defined(HBOOST_NO_STD_USE_FACET)
#  ifdef HBOOST_HAS_TWO_ARG_USE_FACET
#     define HBOOST_USE_FACET(Type, loc) std::use_facet(loc, static_cast<Type*>(0))
#     define HBOOST_HAS_FACET(Type, loc) std::has_facet(loc, static_cast<Type*>(0))
#  elif defined(HBOOST_HAS_MACRO_USE_FACET)
#     define HBOOST_USE_FACET(Type, loc) std::_USE(loc, Type)
#     define HBOOST_HAS_FACET(Type, loc) std::_HAS(loc, Type)
#  elif defined(HBOOST_HAS_STLP_USE_FACET)
#     define HBOOST_USE_FACET(Type, loc) (*std::_Use_facet<Type >(loc))
#     define HBOOST_HAS_FACET(Type, loc) std::has_facet< Type >(loc)
#  endif
#else
#  define HBOOST_USE_FACET(Type, loc) std::use_facet< Type >(loc)
#  define HBOOST_HAS_FACET(Type, loc) std::has_facet< Type >(loc)
#endif

// HBOOST_NESTED_TEMPLATE workaround ------------------------------------------//
// Member templates are supported by some compilers even though they can't use
// the A::template member<U> syntax, as a workaround replace:
//
// typedef typename A::template rebind<U> binder;
//
// with:
//
// typedef typename A::HBOOST_NESTED_TEMPLATE rebind<U> binder;

#ifndef HBOOST_NO_MEMBER_TEMPLATE_KEYWORD
#  define HBOOST_NESTED_TEMPLATE template
#else
#  define HBOOST_NESTED_TEMPLATE
#endif

// HBOOST_UNREACHABLE_RETURN(x) workaround -------------------------------------//
// Normally evaluates to nothing, unless HBOOST_NO_UNREACHABLE_RETURN_DETECTION
// is defined, in which case it evaluates to return x; Use when you have a return
// statement that can never be reached.

#ifndef HBOOST_UNREACHABLE_RETURN
#  ifdef HBOOST_NO_UNREACHABLE_RETURN_DETECTION
#     define HBOOST_UNREACHABLE_RETURN(x) return x;
#  else
#     define HBOOST_UNREACHABLE_RETURN(x)
#  endif
#endif

// HBOOST_DEDUCED_TYPENAME workaround ------------------------------------------//
//
// Some compilers don't support the use of `typename' for dependent
// types in deduced contexts, e.g.
//
//     template <class T> void f(T, typename T::type);
//                                  ^^^^^^^^
// Replace these declarations with:
//
//     template <class T> void f(T, HBOOST_DEDUCED_TYPENAME T::type);

#ifndef HBOOST_NO_DEDUCED_TYPENAME
#  define HBOOST_DEDUCED_TYPENAME typename
#else
#  define HBOOST_DEDUCED_TYPENAME
#endif

#ifndef HBOOST_NO_TYPENAME_WITH_CTOR
#  define HBOOST_CTOR_TYPENAME typename
#else
#  define HBOOST_CTOR_TYPENAME
#endif

// long long workaround ------------------------------------------//
// On gcc (and maybe other compilers?) long long is alway supported
// but it's use may generate either warnings (with -ansi), or errors
// (with -pedantic -ansi) unless it's use is prefixed by __extension__
//
#if defined(HBOOST_HAS_LONG_LONG) && defined(__cplusplus)
namespace hboost{
#  ifdef __GNUC__
   __extension__ typedef long long long_long_type;
   __extension__ typedef unsigned long long ulong_long_type;
#  else
   typedef long long long_long_type;
   typedef unsigned long long ulong_long_type;
#  endif
}
#endif
// same again for __int128:
#if defined(HBOOST_HAS_INT128) && defined(__cplusplus)
namespace hboost{
#  ifdef __GNUC__
   __extension__ typedef __int128 int128_type;
   __extension__ typedef unsigned __int128 uint128_type;
#  else
   typedef __int128 int128_type;
   typedef unsigned __int128 uint128_type;
#  endif
}
#endif
// same again for __float128:
#if defined(HBOOST_HAS_FLOAT128) && defined(__cplusplus)
namespace hboost {
#  ifdef __GNUC__
   __extension__ typedef __float128 float128_type;
#  else
   typedef __float128 float128_type;
#  endif
}
#endif

// HBOOST_[APPEND_]EXPLICIT_TEMPLATE_[NON_]TYPE macros --------------------------//

// These macros are obsolete. Port away and remove.

#  define HBOOST_EXPLICIT_TEMPLATE_TYPE(t)
#  define HBOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define HBOOST_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define HBOOST_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)

#  define HBOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(t)
#  define HBOOST_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define HBOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define HBOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)

// When HBOOST_NO_STD_TYPEINFO is defined, we can just import
// the global definition into std namespace:
#if defined(HBOOST_NO_STD_TYPEINFO) && defined(__cplusplus)
#include <typeinfo>
namespace std{ using ::type_info; }
#endif

// ---------------------------------------------------------------------------//

//
// Helper macro HBOOST_STRINGIZE:
// Converts the parameter X to a string after macro replacement
// on X has been performed.
//
#define HBOOST_STRINGIZE(X) HBOOST_DO_STRINGIZE(X)
#define HBOOST_DO_STRINGIZE(X) #X

//
// Helper macro HBOOST_JOIN:
// The following piece of macro magic joins the two
// arguments together, even when one of the arguments is
// itself a macro (see 16.3.1 in C++ standard).  The key
// is that macro expansion of macro arguments does not
// occur in HBOOST_DO_JOIN2 but does in HBOOST_DO_JOIN.
//
#define HBOOST_JOIN( X, Y ) HBOOST_DO_JOIN( X, Y )
#define HBOOST_DO_JOIN( X, Y ) HBOOST_DO_JOIN2(X,Y)
#define HBOOST_DO_JOIN2( X, Y ) X##Y

//
// Set some default values for compiler/library/platform names.
// These are for debugging config setup only:
//
#  ifndef HBOOST_COMPILER
#     define HBOOST_COMPILER "Unknown ISO C++ Compiler"
#  endif
#  ifndef HBOOST_STDLIB
#     define HBOOST_STDLIB "Unknown ISO standard library"
#  endif
#  ifndef HBOOST_PLATFORM
#     if defined(unix) || defined(__unix) || defined(_XOPEN_SOURCE) \
         || defined(_POSIX_SOURCE)
#        define HBOOST_PLATFORM "Generic Unix"
#     else
#        define HBOOST_PLATFORM "Unknown"
#     endif
#  endif

//
// Set some default values GPU support
//
#  ifndef HBOOST_GPU_ENABLED
#  define HBOOST_GPU_ENABLED
#  endif

// HBOOST_FORCEINLINE ---------------------------------------------//
// Macro to use in place of 'inline' to force a function to be inline
#if !defined(HBOOST_FORCEINLINE)
#  if defined(_MSC_VER)
#    define HBOOST_FORCEINLINE __forceinline
#  elif defined(__GNUC__) && __GNUC__ > 3
     // Clang also defines __GNUC__ (as 4)
#    define HBOOST_FORCEINLINE inline __attribute__ ((__always_inline__))
#  else
#    define HBOOST_FORCEINLINE inline
#  endif
#endif

// HBOOST_NOINLINE ---------------------------------------------//
// Macro to use in place of 'inline' to prevent a function to be inlined
#if !defined(HBOOST_NOINLINE)
#  if defined(_MSC_VER)
#    define HBOOST_NOINLINE __declspec(noinline)
#  elif defined(__GNUC__) && __GNUC__ > 3
     // Clang also defines __GNUC__ (as 4)
#    if defined(__CUDACC__)
       // nvcc doesn't always parse __noinline__, 
       // see: https://svn.boost.org/trac/hboost/ticket/9392
#      define HBOOST_NOINLINE __attribute__ ((noinline))
#    else
#      define HBOOST_NOINLINE __attribute__ ((__noinline__))
#    endif
#  else
#    define HBOOST_NOINLINE
#  endif
#endif

// HBOOST_NORETURN ---------------------------------------------//
// Macro to use before a function declaration/definition to designate
// the function as not returning normally (i.e. with a return statement
// or by leaving the function scope, if the function return type is void).
#if !defined(HBOOST_NORETURN)
#  if defined(_MSC_VER)
#    define HBOOST_NORETURN __declspec(noreturn)
#  elif defined(__GNUC__)
#    define HBOOST_NORETURN __attribute__ ((__noreturn__))
#  else
#    define HBOOST_NO_NORETURN
#    define HBOOST_NORETURN
#  endif
#endif

// Branch prediction hints
// These macros are intended to wrap conditional expressions that yield true or false
//
//  if (HBOOST_LIKELY(var == 10))
//  {
//     // the most probable code here
//  }
//
#if !defined(HBOOST_LIKELY)
#  define HBOOST_LIKELY(x) x
#endif
#if !defined(HBOOST_UNLIKELY)
#  define HBOOST_UNLIKELY(x) x
#endif

// Type and data alignment specification
//
#if !defined(HBOOST_NO_CXX11_ALIGNAS)
#  define HBOOST_ALIGNMENT(x) alignas(x)
#elif defined(_MSC_VER)
#  define HBOOST_ALIGNMENT(x) __declspec(align(x))
#elif defined(__GNUC__)
#  define HBOOST_ALIGNMENT(x) __attribute__ ((__aligned__(x)))
#else
#  define HBOOST_NO_ALIGNMENT
#  define HBOOST_ALIGNMENT(x)
#endif

// Lack of non-public defaulted functions is implied by the lack of any defaulted functions
#if !defined(HBOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS) && defined(HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS)
#  define HBOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS
#endif

// Defaulted and deleted function declaration helpers
// These macros are intended to be inside a class definition.
// HBOOST_DEFAULTED_FUNCTION accepts the function declaration and its
// body, which will be used if the compiler doesn't support defaulted functions.
// HBOOST_DELETED_FUNCTION only accepts the function declaration. It
// will expand to a private function declaration, if the compiler doesn't support
// deleted functions. Because of this it is recommended to use HBOOST_DELETED_FUNCTION
// in the end of the class definition.
//
//  class my_class
//  {
//  public:
//      // Default-constructible
//      HBOOST_DEFAULTED_FUNCTION(my_class(), {})
//      // Copying prohibited
//      HBOOST_DELETED_FUNCTION(my_class(my_class const&))
//      HBOOST_DELETED_FUNCTION(my_class& operator= (my_class const&))
//  };
//
#if !(defined(HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS) || defined(HBOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS))
#   define HBOOST_DEFAULTED_FUNCTION(fun, body) fun = default;
#else
#   define HBOOST_DEFAULTED_FUNCTION(fun, body) fun body
#endif

#if !defined(HBOOST_NO_CXX11_DELETED_FUNCTIONS)
#   define HBOOST_DELETED_FUNCTION(fun) fun = delete;
#else
#   define HBOOST_DELETED_FUNCTION(fun) private: fun;
#endif

//
// Set HBOOST_NO_DECLTYPE_N3276 when HBOOST_NO_DECLTYPE is defined
//
#if defined(HBOOST_NO_CXX11_DECLTYPE) && !defined(HBOOST_NO_CXX11_DECLTYPE_N3276)
#define HBOOST_NO_CXX11_DECLTYPE_N3276 HBOOST_NO_CXX11_DECLTYPE
#endif

//  -------------------- Deprecated macros for 1.50 ---------------------------
//  These will go away in a future release

//  Use HBOOST_NO_CXX11_HDR_UNORDERED_SET or HBOOST_NO_CXX11_HDR_UNORDERED_MAP
//           instead of HBOOST_NO_STD_UNORDERED
#if defined(HBOOST_NO_CXX11_HDR_UNORDERED_MAP) || defined (HBOOST_NO_CXX11_HDR_UNORDERED_SET)
# ifndef HBOOST_NO_CXX11_STD_UNORDERED
#  define HBOOST_NO_CXX11_STD_UNORDERED
# endif
#endif

//  Use HBOOST_NO_CXX11_HDR_INITIALIZER_LIST instead of HBOOST_NO_INITIALIZER_LISTS
#if defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST) && !defined(HBOOST_NO_INITIALIZER_LISTS)
#  define HBOOST_NO_INITIALIZER_LISTS
#endif

//  Use HBOOST_NO_CXX11_HDR_ARRAY instead of HBOOST_NO_0X_HDR_ARRAY
#if defined(HBOOST_NO_CXX11_HDR_ARRAY) && !defined(HBOOST_NO_0X_HDR_ARRAY)
#  define HBOOST_NO_0X_HDR_ARRAY
#endif
//  Use HBOOST_NO_CXX11_HDR_CHRONO instead of HBOOST_NO_0X_HDR_CHRONO
#if defined(HBOOST_NO_CXX11_HDR_CHRONO) && !defined(HBOOST_NO_0X_HDR_CHRONO)
#  define HBOOST_NO_0X_HDR_CHRONO
#endif
//  Use HBOOST_NO_CXX11_HDR_CODECVT instead of HBOOST_NO_0X_HDR_CODECVT
#if defined(HBOOST_NO_CXX11_HDR_CODECVT) && !defined(HBOOST_NO_0X_HDR_CODECVT)
#  define HBOOST_NO_0X_HDR_CODECVT
#endif
//  Use HBOOST_NO_CXX11_HDR_CONDITION_VARIABLE instead of HBOOST_NO_0X_HDR_CONDITION_VARIABLE
#if defined(HBOOST_NO_CXX11_HDR_CONDITION_VARIABLE) && !defined(HBOOST_NO_0X_HDR_CONDITION_VARIABLE)
#  define HBOOST_NO_0X_HDR_CONDITION_VARIABLE
#endif
//  Use HBOOST_NO_CXX11_HDR_FORWARD_LIST instead of HBOOST_NO_0X_HDR_FORWARD_LIST
#if defined(HBOOST_NO_CXX11_HDR_FORWARD_LIST) && !defined(HBOOST_NO_0X_HDR_FORWARD_LIST)
#  define HBOOST_NO_0X_HDR_FORWARD_LIST
#endif
//  Use HBOOST_NO_CXX11_HDR_FUTURE instead of HBOOST_NO_0X_HDR_FUTURE
#if defined(HBOOST_NO_CXX11_HDR_FUTURE) && !defined(HBOOST_NO_0X_HDR_FUTURE)
#  define HBOOST_NO_0X_HDR_FUTURE
#endif

//  Use HBOOST_NO_CXX11_HDR_INITIALIZER_LIST
//  instead of HBOOST_NO_0X_HDR_INITIALIZER_LIST or HBOOST_NO_INITIALIZER_LISTS
#ifdef HBOOST_NO_CXX11_HDR_INITIALIZER_LIST
# ifndef HBOOST_NO_0X_HDR_INITIALIZER_LIST
#  define HBOOST_NO_0X_HDR_INITIALIZER_LIST
# endif
# ifndef HBOOST_NO_INITIALIZER_LISTS
#  define HBOOST_NO_INITIALIZER_LISTS
# endif
#endif

//  Use HBOOST_NO_CXX11_HDR_MUTEX instead of HBOOST_NO_0X_HDR_MUTEX
#if defined(HBOOST_NO_CXX11_HDR_MUTEX) && !defined(HBOOST_NO_0X_HDR_MUTEX)
#  define HBOOST_NO_0X_HDR_MUTEX
#endif
//  Use HBOOST_NO_CXX11_HDR_RANDOM instead of HBOOST_NO_0X_HDR_RANDOM
#if defined(HBOOST_NO_CXX11_HDR_RANDOM) && !defined(HBOOST_NO_0X_HDR_RANDOM)
#  define HBOOST_NO_0X_HDR_RANDOM
#endif
//  Use HBOOST_NO_CXX11_HDR_RATIO instead of HBOOST_NO_0X_HDR_RATIO
#if defined(HBOOST_NO_CXX11_HDR_RATIO) && !defined(HBOOST_NO_0X_HDR_RATIO)
#  define HBOOST_NO_0X_HDR_RATIO
#endif
//  Use HBOOST_NO_CXX11_HDR_REGEX instead of HBOOST_NO_0X_HDR_REGEX
#if defined(HBOOST_NO_CXX11_HDR_REGEX) && !defined(HBOOST_NO_0X_HDR_REGEX)
#  define HBOOST_NO_0X_HDR_REGEX
#endif
//  Use HBOOST_NO_CXX11_HDR_SYSTEM_ERROR instead of HBOOST_NO_0X_HDR_SYSTEM_ERROR
#if defined(HBOOST_NO_CXX11_HDR_SYSTEM_ERROR) && !defined(HBOOST_NO_0X_HDR_SYSTEM_ERROR)
#  define HBOOST_NO_0X_HDR_SYSTEM_ERROR
#endif
//  Use HBOOST_NO_CXX11_HDR_THREAD instead of HBOOST_NO_0X_HDR_THREAD
#if defined(HBOOST_NO_CXX11_HDR_THREAD) && !defined(HBOOST_NO_0X_HDR_THREAD)
#  define HBOOST_NO_0X_HDR_THREAD
#endif
//  Use HBOOST_NO_CXX11_HDR_TUPLE instead of HBOOST_NO_0X_HDR_TUPLE
#if defined(HBOOST_NO_CXX11_HDR_TUPLE) && !defined(HBOOST_NO_0X_HDR_TUPLE)
#  define HBOOST_NO_0X_HDR_TUPLE
#endif
//  Use HBOOST_NO_CXX11_HDR_TYPE_TRAITS instead of HBOOST_NO_0X_HDR_TYPE_TRAITS
#if defined(HBOOST_NO_CXX11_HDR_TYPE_TRAITS) && !defined(HBOOST_NO_0X_HDR_TYPE_TRAITS)
#  define HBOOST_NO_0X_HDR_TYPE_TRAITS
#endif
//  Use HBOOST_NO_CXX11_HDR_TYPEINDEX instead of HBOOST_NO_0X_HDR_TYPEINDEX
#if defined(HBOOST_NO_CXX11_HDR_TYPEINDEX) && !defined(HBOOST_NO_0X_HDR_TYPEINDEX)
#  define HBOOST_NO_0X_HDR_TYPEINDEX
#endif
//  Use HBOOST_NO_CXX11_HDR_UNORDERED_MAP instead of HBOOST_NO_0X_HDR_UNORDERED_MAP
#if defined(HBOOST_NO_CXX11_HDR_UNORDERED_MAP) && !defined(HBOOST_NO_0X_HDR_UNORDERED_MAP)
#  define HBOOST_NO_0X_HDR_UNORDERED_MAP
#endif
//  Use HBOOST_NO_CXX11_HDR_UNORDERED_SET instead of HBOOST_NO_0X_HDR_UNORDERED_SET
#if defined(HBOOST_NO_CXX11_HDR_UNORDERED_SET) && !defined(HBOOST_NO_0X_HDR_UNORDERED_SET)
#  define HBOOST_NO_0X_HDR_UNORDERED_SET
#endif

//  ------------------ End of deprecated macros for 1.50 ---------------------------

//  -------------------- Deprecated macros for 1.51 ---------------------------
//  These will go away in a future release

//  Use     HBOOST_NO_CXX11_AUTO_DECLARATIONS instead of   HBOOST_NO_AUTO_DECLARATIONS
#if defined(HBOOST_NO_CXX11_AUTO_DECLARATIONS) && !defined(HBOOST_NO_AUTO_DECLARATIONS)
#  define HBOOST_NO_AUTO_DECLARATIONS
#endif
//  Use     HBOOST_NO_CXX11_AUTO_MULTIDECLARATIONS instead of   HBOOST_NO_AUTO_MULTIDECLARATIONS
#if defined(HBOOST_NO_CXX11_AUTO_MULTIDECLARATIONS) && !defined(HBOOST_NO_AUTO_MULTIDECLARATIONS)
#  define HBOOST_NO_AUTO_MULTIDECLARATIONS
#endif
//  Use     HBOOST_NO_CXX11_CHAR16_T instead of   HBOOST_NO_CHAR16_T
#if defined(HBOOST_NO_CXX11_CHAR16_T) && !defined(HBOOST_NO_CHAR16_T)
#  define HBOOST_NO_CHAR16_T
#endif
//  Use     HBOOST_NO_CXX11_CHAR32_T instead of   HBOOST_NO_CHAR32_T
#if defined(HBOOST_NO_CXX11_CHAR32_T) && !defined(HBOOST_NO_CHAR32_T)
#  define HBOOST_NO_CHAR32_T
#endif
//  Use     HBOOST_NO_CXX11_TEMPLATE_ALIASES instead of   HBOOST_NO_TEMPLATE_ALIASES
#if defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES) && !defined(HBOOST_NO_TEMPLATE_ALIASES)
#  define HBOOST_NO_TEMPLATE_ALIASES
#endif
//  Use     HBOOST_NO_CXX11_CONSTEXPR instead of   HBOOST_NO_CONSTEXPR
#if defined(HBOOST_NO_CXX11_CONSTEXPR) && !defined(HBOOST_NO_CONSTEXPR)
#  define HBOOST_NO_CONSTEXPR
#endif
//  Use     HBOOST_NO_CXX11_DECLTYPE_N3276 instead of   HBOOST_NO_DECLTYPE_N3276
#if defined(HBOOST_NO_CXX11_DECLTYPE_N3276) && !defined(HBOOST_NO_DECLTYPE_N3276)
#  define HBOOST_NO_DECLTYPE_N3276
#endif
//  Use     HBOOST_NO_CXX11_DECLTYPE instead of   HBOOST_NO_DECLTYPE
#if defined(HBOOST_NO_CXX11_DECLTYPE) && !defined(HBOOST_NO_DECLTYPE)
#  define HBOOST_NO_DECLTYPE
#endif
//  Use     HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS instead of   HBOOST_NO_DEFAULTED_FUNCTIONS
#if defined(HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS) && !defined(HBOOST_NO_DEFAULTED_FUNCTIONS)
#  define HBOOST_NO_DEFAULTED_FUNCTIONS
#endif
//  Use     HBOOST_NO_CXX11_DELETED_FUNCTIONS instead of   HBOOST_NO_DELETED_FUNCTIONS
#if defined(HBOOST_NO_CXX11_DELETED_FUNCTIONS) && !defined(HBOOST_NO_DELETED_FUNCTIONS)
#  define HBOOST_NO_DELETED_FUNCTIONS
#endif
//  Use     HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS instead of   HBOOST_NO_EXPLICIT_CONVERSION_OPERATORS
#if defined(HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS) && !defined(HBOOST_NO_EXPLICIT_CONVERSION_OPERATORS)
#  define HBOOST_NO_EXPLICIT_CONVERSION_OPERATORS
#endif
//  Use     HBOOST_NO_CXX11_EXTERN_TEMPLATE instead of   HBOOST_NO_EXTERN_TEMPLATE
#if defined(HBOOST_NO_CXX11_EXTERN_TEMPLATE) && !defined(HBOOST_NO_EXTERN_TEMPLATE)
#  define HBOOST_NO_EXTERN_TEMPLATE
#endif
//  Use     HBOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS instead of   HBOOST_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#if defined(HBOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS) && !defined(HBOOST_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)
#  define HBOOST_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#endif
//  Use     HBOOST_NO_CXX11_LAMBDAS instead of   HBOOST_NO_LAMBDAS
#if defined(HBOOST_NO_CXX11_LAMBDAS) && !defined(HBOOST_NO_LAMBDAS)
#  define HBOOST_NO_LAMBDAS
#endif
//  Use     HBOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS instead of   HBOOST_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS
#if defined(HBOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS) && !defined(HBOOST_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS)
#  define HBOOST_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS
#endif
//  Use     HBOOST_NO_CXX11_NOEXCEPT instead of   HBOOST_NO_NOEXCEPT
#if defined(HBOOST_NO_CXX11_NOEXCEPT) && !defined(HBOOST_NO_NOEXCEPT)
#  define HBOOST_NO_NOEXCEPT
#endif
//  Use     HBOOST_NO_CXX11_NULLPTR instead of   HBOOST_NO_NULLPTR
#if defined(HBOOST_NO_CXX11_NULLPTR) && !defined(HBOOST_NO_NULLPTR)
#  define HBOOST_NO_NULLPTR
#endif
//  Use     HBOOST_NO_CXX11_RAW_LITERALS instead of   HBOOST_NO_RAW_LITERALS
#if defined(HBOOST_NO_CXX11_RAW_LITERALS) && !defined(HBOOST_NO_RAW_LITERALS)
#  define HBOOST_NO_RAW_LITERALS
#endif
//  Use     HBOOST_NO_CXX11_RVALUE_REFERENCES instead of   HBOOST_NO_RVALUE_REFERENCES
#if defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(HBOOST_NO_RVALUE_REFERENCES)
#  define HBOOST_NO_RVALUE_REFERENCES
#endif
//  Use     HBOOST_NO_CXX11_SCOPED_ENUMS instead of   HBOOST_NO_SCOPED_ENUMS
#if defined(HBOOST_NO_CXX11_SCOPED_ENUMS) && !defined(HBOOST_NO_SCOPED_ENUMS)
#  define HBOOST_NO_SCOPED_ENUMS
#endif
//  Use     HBOOST_NO_CXX11_STATIC_ASSERT instead of   HBOOST_NO_STATIC_ASSERT
#if defined(HBOOST_NO_CXX11_STATIC_ASSERT) && !defined(HBOOST_NO_STATIC_ASSERT)
#  define HBOOST_NO_STATIC_ASSERT
#endif
//  Use     HBOOST_NO_CXX11_STD_UNORDERED instead of   HBOOST_NO_STD_UNORDERED
#if defined(HBOOST_NO_CXX11_STD_UNORDERED) && !defined(HBOOST_NO_STD_UNORDERED)
#  define HBOOST_NO_STD_UNORDERED
#endif
//  Use     HBOOST_NO_CXX11_UNICODE_LITERALS instead of   HBOOST_NO_UNICODE_LITERALS
#if defined(HBOOST_NO_CXX11_UNICODE_LITERALS) && !defined(HBOOST_NO_UNICODE_LITERALS)
#  define HBOOST_NO_UNICODE_LITERALS
#endif
//  Use     HBOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX instead of   HBOOST_NO_UNIFIED_INITIALIZATION_SYNTAX
#if defined(HBOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX) && !defined(HBOOST_NO_UNIFIED_INITIALIZATION_SYNTAX)
#  define HBOOST_NO_UNIFIED_INITIALIZATION_SYNTAX
#endif
//  Use     HBOOST_NO_CXX11_VARIADIC_TEMPLATES instead of   HBOOST_NO_VARIADIC_TEMPLATES
#if defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_VARIADIC_TEMPLATES)
#  define HBOOST_NO_VARIADIC_TEMPLATES
#endif
//  Use     HBOOST_NO_CXX11_VARIADIC_MACROS instead of   HBOOST_NO_VARIADIC_MACROS
#if defined(HBOOST_NO_CXX11_VARIADIC_MACROS) && !defined(HBOOST_NO_VARIADIC_MACROS)
#  define HBOOST_NO_VARIADIC_MACROS
#endif
//  Use     HBOOST_NO_CXX11_NUMERIC_LIMITS instead of   HBOOST_NO_NUMERIC_LIMITS_LOWEST
#if defined(HBOOST_NO_CXX11_NUMERIC_LIMITS) && !defined(HBOOST_NO_NUMERIC_LIMITS_LOWEST)
#  define HBOOST_NO_NUMERIC_LIMITS_LOWEST
#endif
//  ------------------ End of deprecated macros for 1.51 ---------------------------



//
// Helper macros HBOOST_NOEXCEPT, HBOOST_NOEXCEPT_IF, HBOOST_NOEXCEPT_EXPR
// These aid the transition to C++11 while still supporting C++03 compilers
//
#ifdef HBOOST_NO_CXX11_NOEXCEPT
#  define HBOOST_NOEXCEPT
#  define HBOOST_NOEXCEPT_OR_NOTHROW throw()
#  define HBOOST_NOEXCEPT_IF(Predicate)
#  define HBOOST_NOEXCEPT_EXPR(Expression) false
#else
#  define HBOOST_NOEXCEPT noexcept
#  define HBOOST_NOEXCEPT_OR_NOTHROW noexcept
#  define HBOOST_NOEXCEPT_IF(Predicate) noexcept((Predicate))
#  define HBOOST_NOEXCEPT_EXPR(Expression) noexcept((Expression))
#endif
//
// Helper macro HBOOST_FALLTHROUGH
// Fallback definition of HBOOST_FALLTHROUGH macro used to mark intended
// fall-through between case labels in a switch statement. We use a definition
// that requires a semicolon after it to avoid at least one type of misuse even
// on unsupported compilers.
//
#ifndef HBOOST_FALLTHROUGH
#  define HBOOST_FALLTHROUGH ((void)0)
#endif

//
// constexpr workarounds
//
#if defined(HBOOST_NO_CXX11_CONSTEXPR)
#define HBOOST_CONSTEXPR
#define HBOOST_CONSTEXPR_OR_CONST const
#else
#define HBOOST_CONSTEXPR constexpr
#define HBOOST_CONSTEXPR_OR_CONST constexpr
#endif
#if defined(HBOOST_NO_CXX14_CONSTEXPR)
#define HBOOST_CXX14_CONSTEXPR
#else
#define HBOOST_CXX14_CONSTEXPR constexpr
#endif

//
// Unused variable/typedef workarounds:
//
#ifndef HBOOST_ATTRIBUTE_UNUSED
#  define HBOOST_ATTRIBUTE_UNUSED
#endif

#define HBOOST_STATIC_CONSTEXPR  static HBOOST_CONSTEXPR_OR_CONST

//
// Set HBOOST_HAS_STATIC_ASSERT when HBOOST_NO_CXX11_STATIC_ASSERT is not defined
//
#if !defined(HBOOST_NO_CXX11_STATIC_ASSERT) && !defined(HBOOST_HAS_STATIC_ASSERT)
#  define HBOOST_HAS_STATIC_ASSERT
#endif

//
// Set HBOOST_HAS_RVALUE_REFS when HBOOST_NO_CXX11_RVALUE_REFERENCES is not defined
//
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(HBOOST_HAS_RVALUE_REFS)
#define HBOOST_HAS_RVALUE_REFS
#endif

//
// Set HBOOST_HAS_VARIADIC_TMPL when HBOOST_NO_CXX11_VARIADIC_TEMPLATES is not defined
//
#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_HAS_VARIADIC_TMPL)
#define HBOOST_HAS_VARIADIC_TMPL
#endif
//
// Set HBOOST_NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS when
// HBOOST_NO_CXX11_VARIADIC_TEMPLATES is set:
//
#if defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS)
#  define HBOOST_NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS
#endif

//
// Finish off with checks for macros that are depricated / no longer supported,
// if any of these are set then it's very likely that much of Boost will no
// longer work.  So stop with a #error for now, but give the user a chance
// to continue at their own risk if they really want to:
//
#if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(HBOOST_CONFIG_ALLOW_DEPRECATED)
#  error "You are using a compiler which lacks features which are now a minimum requirement in order to use Boost, define HBOOST_CONFIG_ALLOW_DEPRECATED if you want to continue at your own risk!!!"
#endif

#endif
