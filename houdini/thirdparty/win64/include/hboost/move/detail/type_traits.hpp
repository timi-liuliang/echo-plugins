//////////////////////////////////////////////////////////////////////////////
// (C) Copyright John Maddock 2000.
// (C) Copyright Ion Gaztanaga 2005-2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
// The alignment and Type traits implementation comes from
// John Maddock's TypeTraits library.
//
// Some other tricks come from Howard Hinnant's papers and StackOverflow replies
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_MOVE_DETAIL_TYPE_TRAITS_HPP
#define HBOOST_MOVE_DETAIL_TYPE_TRAITS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/move/detail/config_begin.hpp>
#include <hboost/move/detail/workaround.hpp>

// move/detail
#include <hboost/move/detail/meta_utils.hpp>
// other
#include <hboost/assert.hpp>
#include <hboost/static_assert.hpp>
// std
#include <cstddef>

//Use of Boost.TypeTraits leads to long preprocessed source code due to
//MPL dependencies. We'll use intrinsics directly and make or own
//simplified version of TypeTraits.
//If someday Boost.TypeTraits dependencies are minimized, we should
//revisit this file redirecting code to Boost.TypeTraits traits.

//These traits don't care about volatile, reference or other checks
//made by Boost.TypeTraits because no volatile or reference types
//can be hold in Boost.Containers. This helps to avoid any Boost.TypeTraits
//dependency.

// Helper macros for builtin compiler support.
// If your compiler has builtin support for any of the following
// traits concepts, then redefine the appropriate macros to pick
// up on the compiler support:
//
// (these should largely ignore cv-qualifiers)
// HBOOST_MOVE_IS_POD(T) should evaluate to true if T is a POD type
// HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T) should evaluate to true if "T x;" has no effect
// HBOOST_MOVE_HAS_TRIVIAL_COPY(T) should evaluate to true if T(t) <==> memcpy
// HBOOST_MOVE_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T) should evaluate to true if T(hboost::move(t)) <==> memcpy
// HBOOST_MOVE_HAS_TRIVIAL_ASSIGN(T) should evaluate to true if t = u <==> memcpy
// HBOOST_MOVE_HAS_TRIVIAL_MOVE_ASSIGN(T) should evaluate to true if t = hboost::move(u) <==> memcpy
// HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) should evaluate to true if ~T() has no effect
// HBOOST_MOVE_HAS_NOTHROW_CONSTRUCTOR(T) should evaluate to true if "T x;" can not throw
// HBOOST_MOVE_HAS_NOTHROW_COPY(T) should evaluate to true if T(t) can not throw
// HBOOST_MOVE_HAS_NOTHROW_ASSIGN(T) should evaluate to true if t = u can not throw
// HBOOST_MOVE_IS_ENUM(T) should evaluate to true it t is a union type.
//
// The following can also be defined: when detected our implementation is greatly simplified.
//
// HBOOST_ALIGNMENT_OF(T) should evaluate to the alignment requirements of type T.

#if defined(__MSL_CPP__) && (__MSL_CPP__ >= 0x8000)
    // Metrowerks compiler is acquiring intrinsic type traits support
    // post version 8.  We hook into the published interface to pick up
    // user defined specializations as well as compiler intrinsics as
    // and when they become available:
#   include <msl_utility>
#   define HBOOST_MOVE_IS_UNION(T) HBOOST_STD_EXTENSION_NAMESPACE::is_union<T>::value
#   define HBOOST_MOVE_IS_POD(T) HBOOST_STD_EXTENSION_NAMESPACE::is_POD<T>::value
#   define HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T) HBOOST_STD_EXTENSION_NAMESPACE::has_trivial_default_ctor<T>::value
#   define HBOOST_MOVE_HAS_TRIVIAL_COPY(T) HBOOST_STD_EXTENSION_NAMESPACE::has_trivial_copy_ctor<T>::value
#   define HBOOST_MOVE_HAS_TRIVIAL_ASSIGN(T) HBOOST_STD_EXTENSION_NAMESPACE::has_trivial_assignment<T>::value
#   define HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) HBOOST_STD_EXTENSION_NAMESPACE::has_trivial_dtor<T>::value
#endif

#if (defined(HBOOST_MSVC) && defined(HBOOST_MSVC_FULL_VER) && (HBOOST_MSVC_FULL_VER >=140050215))\
         || (defined(HBOOST_INTEL) && defined(_MSC_VER) && (_MSC_VER >= 1500))
#   define HBOOST_MOVE_IS_UNION(T) __is_union(T)
#   define HBOOST_MOVE_IS_POD(T)                    (__is_pod(T) && __has_trivial_constructor(T))
#   define HBOOST_MOVE_IS_EMPTY(T)                  __is_empty(T)
#   define HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T)   __has_trivial_constructor(T)
#   define HBOOST_MOVE_HAS_TRIVIAL_COPY(T)          (__has_trivial_copy(T)|| ::hboost::move_detail::is_pod<T>::value)
#   define HBOOST_MOVE_HAS_TRIVIAL_ASSIGN(T)        (__has_trivial_assign(T) || ::hboost::move_detail::is_pod<T>::value)
#   define HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR(T)    (__has_trivial_destructor(T) || ::hboost::move_detail::is_pod<T>::value)
#   define HBOOST_MOVE_HAS_NOTHROW_CONSTRUCTOR(T)   (__has_nothrow_constructor(T) || ::hboost::move_detail::is_trivially_default_constructible<T>::value)
#   define HBOOST_MOVE_HAS_NOTHROW_COPY(T)          (__has_nothrow_copy(T) || ::hboost::move_detail::is_trivially_copy_constructible<T>::value)
#   define HBOOST_MOVE_HAS_NOTHROW_ASSIGN(T)        (__has_nothrow_assign(T) || ::hboost::move_detail::is_trivially_copy_assignable<T>::value)

#   define HBOOST_MOVE_IS_ENUM(T) __is_enum(T)
#   if defined(_MSC_VER) && (_MSC_VER >= 1700)
#       define HBOOST_MOVE_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T)   (__has_trivial_move_constructor(T) || ::hboost::move_detail::is_pod<T>::value)
#       define HBOOST_MOVE_HAS_TRIVIAL_MOVE_ASSIGN(T)        (__has_trivial_move_assign(T) || ::hboost::move_detail::is_pod<T>::value)
#   endif
#endif

#if defined(HBOOST_CLANG) && defined(__has_feature)

#   if __has_feature(is_union)
#     define HBOOST_MOVE_IS_UNION(T) __is_union(T)
#   endif
#   if (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20080306 && __GLIBCXX__ != 20080519)) && __has_feature(is_pod)
#     define HBOOST_MOVE_IS_POD(T) __is_pod(T)
#   endif
#   if (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20080306 && __GLIBCXX__ != 20080519)) && __has_feature(is_empty)
#     define HBOOST_MOVE_IS_EMPTY(T) __is_empty(T)
#   endif
#   if __has_feature(has_trivial_constructor)
#     define HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#   endif
#   if __has_feature(has_trivial_copy)
#     //There are problems with deleted copy constructors detected as trivially copyable.
#     //http://stackoverflow.com/questions/12754886/has-trivial-copy-behaves-differently-in-clang-and-gcc-whos-right
#     define HBOOST_MOVE_HAS_TRIVIAL_COPY(T) (__has_trivial_copy(T) && ::hboost::move_detail::is_copy_constructible<T>::value)
#   endif
#   if __has_feature(has_trivial_assign)
#     define HBOOST_MOVE_HAS_TRIVIAL_ASSIGN(T) (__has_trivial_assign(T) )
#   endif
#   if __has_feature(has_trivial_destructor)
#     define HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#   endif
#   if __has_feature(has_nothrow_constructor)
#     define HBOOST_MOVE_HAS_NOTHROW_CONSTRUCTOR(T) __has_nothrow_constructor(T)
#   endif
#   if __has_feature(has_nothrow_copy)
#     define HBOOST_MOVE_HAS_NOTHROW_COPY(T) (__has_nothrow_copy(T))
#   endif
#   if __has_feature(is_nothrow_copy_assignable)
#     define HBOOST_MOVE_HAS_NOTHROW_ASSIGN(T) (__has_nothrow_assign(T))
#   endif
#   if __has_feature(is_enum)
#     define HBOOST_MOVE_IS_ENUM(T) __is_enum(T)
#   endif
#   if __has_feature(has_trivial_move_constructor)
#     define HBOOST_MOVE_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T) __has_trivial_move_constructor(T)
#   endif
#   if __has_feature(has_trivial_move_assign)
#     define HBOOST_MOVE_HAS_TRIVIAL_MOVE_ASSIGN(T) __has_trivial_move_assign(T)
#   endif
#   define HBOOST_MOVE_ALIGNMENT_OF(T) __alignof(T)
#endif

#if defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3) && !defined(__GCCXML__))) && !defined(HBOOST_CLANG)

#ifdef HBOOST_INTEL
#  define HBOOST_MOVE_INTEL_TT_OPTS || ::hboost::move_detail::is_pod<T>::value
#else
#  define HBOOST_MOVE_INTEL_TT_OPTS
#endif

#   define HBOOST_MOVE_IS_UNION(T) __is_union(T)
#   define HBOOST_MOVE_IS_POD(T) __is_pod(T)
#   define HBOOST_MOVE_IS_EMPTY(T) __is_empty(T)
#   define HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T) ((__has_trivial_constructor(T) HBOOST_MOVE_INTEL_TT_OPTS))
#   define HBOOST_MOVE_HAS_TRIVIAL_COPY(T) ((__has_trivial_copy(T) HBOOST_MOVE_INTEL_TT_OPTS))
#   define HBOOST_MOVE_HAS_TRIVIAL_ASSIGN(T) ((__has_trivial_assign(T) HBOOST_MOVE_INTEL_TT_OPTS) )
#   define HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) (__has_trivial_destructor(T) HBOOST_MOVE_INTEL_TT_OPTS)
#   define HBOOST_MOVE_HAS_NOTHROW_CONSTRUCTOR(T) (__has_nothrow_constructor(T) HBOOST_MOVE_INTEL_TT_OPTS)
#   define HBOOST_MOVE_HAS_NOTHROW_COPY(T) ((__has_nothrow_copy(T) HBOOST_MOVE_INTEL_TT_OPTS))
#   define HBOOST_MOVE_HAS_NOTHROW_ASSIGN(T) ((__has_nothrow_assign(T) HBOOST_MOVE_INTEL_TT_OPTS))

#   define HBOOST_MOVE_IS_ENUM(T) __is_enum(T)
#   if (!defined(unix) && !defined(__unix__)) || defined(__LP64__)
      // GCC sometimes lies about alignment requirements
      // of type double on 32-bit unix platforms, use the
      // old implementation instead in that case:
#     define HBOOST_MOVE_ALIGNMENT_OF(T) __alignof__(T)
#   endif
#endif

#if defined(__ghs__) && (__GHS_VERSION_NUMBER >= 600)

#   define HBOOST_MOVE_IS_UNION(T) __is_union(T)
#   define HBOOST_MOVE_IS_POD(T) __is_pod(T)
#   define HBOOST_MOVE_IS_EMPTY(T) __is_empty(T)
#   define HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#   define HBOOST_MOVE_HAS_TRIVIAL_COPY(T) (__has_trivial_copy(T))
#   define HBOOST_MOVE_HAS_TRIVIAL_ASSIGN(T) (__has_trivial_assign(T))
#   define HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#   define HBOOST_MOVE_HAS_NOTHROW_CONSTRUCTOR(T) __has_nothrow_constructor(T)
#   define HBOOST_MOVE_HAS_NOTHROW_COPY(T) (__has_nothrow_copy(T))
#   define HBOOST_MOVE_HAS_NOTHROW_ASSIGN(T) (__has_nothrow_assign(T))

#   define HBOOST_MOVE_IS_ENUM(T) __is_enum(T)
#   define HBOOST_MOVE_ALIGNMENT_OF(T) __alignof__(T)
#endif

# if defined(__CODEGEARC__)
#   define HBOOST_MOVE_IS_UNION(T) __is_union(T)
#   define HBOOST_MOVE_IS_POD(T) __is_pod(T)
#   define HBOOST_MOVE_IS_EMPTY(T) __is_empty(T)
#   define HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T) (__has_trivial_default_constructor(T))
#   define HBOOST_MOVE_HAS_TRIVIAL_COPY(T) (__has_trivial_copy_constructor(T))
#   define HBOOST_MOVE_HAS_TRIVIAL_ASSIGN(T) (__has_trivial_assign(T))
#   define HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) (__has_trivial_destructor(T))
#   define HBOOST_MOVE_HAS_NOTHROW_CONSTRUCTOR(T) (__has_nothrow_default_constructor(T))
#   define HBOOST_MOVE_HAS_NOTHROW_COPY(T) (__has_nothrow_copy_constructor(T))
#   define HBOOST_MOVE_HAS_NOTHROW_ASSIGN(T) (__has_nothrow_assign(T))

#   define HBOOST_MOVE_IS_ENUM(T) __is_enum(T)
#   define HBOOST_MOVE_ALIGNMENT_OF(T) alignof(T)

#endif

//Fallback definitions

#ifdef HBOOST_MOVE_IS_UNION
   #define HBOOST_MOVE_IS_UNION_IMPL(T) HBOOST_MOVE_IS_UNION(T)
#else
   #define HBOOST_MOVE_IS_UNION_IMPL(T) false
#endif

#ifdef HBOOST_MOVE_IS_POD
   //in some compilers the intrinsic is limited to class types so add scalar and void
   #define HBOOST_MOVE_IS_POD_IMPL(T) (::hboost::move_detail::is_scalar<T>::value ||\
                                      ::hboost::move_detail::is_void<T>::value   ||\
                                       HBOOST_MOVE_IS_POD(T))
#else
   #define HBOOST_MOVE_IS_POD_IMPL(T) \
      (::hboost::move_detail::is_scalar<T>::value || ::hboost::move_detail::is_void<T>::value)
#endif

#ifdef HBOOST_MOVE_IS_EMPTY
   #define HBOOST_MOVE_IS_EMPTY_IMPL(T) HBOOST_MOVE_IS_EMPTY(T)
#else
   #define HBOOST_MOVE_IS_EMPTY_IMPL(T)    ::hboost::move_detail::is_empty_nonintrinsic<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_TRIVIAL_COPY
   #define HBOOST_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)   HBOOST_MOVE_HAS_TRIVIAL_COPY(T)
#else
   #define HBOOST_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)   ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR
   #define HBOOST_MOVE_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T)  HBOOST_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T)
#else
   #define HBOOST_MOVE_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T)  ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_TRIVIAL_COPY
   #define HBOOST_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)   HBOOST_MOVE_HAS_TRIVIAL_COPY(T)
#else
   #define HBOOST_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)   ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_TRIVIAL_MOVE_CONSTRUCTOR
   #define HBOOST_MOVE_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T)   HBOOST_MOVE_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T)
#else
   #define HBOOST_MOVE_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T)   ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_TRIVIAL_ASSIGN
   #define HBOOST_MOVE_IS_TRIVIALLY_COPY_ASSIGNABLE(T) HBOOST_MOVE_HAS_TRIVIAL_ASSIGN(T)
#else
   #define HBOOST_MOVE_IS_TRIVIALLY_COPY_ASSIGNABLE(T) ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_TRIVIAL_MOVE_ASSIGN
   #define HBOOST_MOVE_IS_TRIVIALLY_MOVE_ASSIGNABLE(T)  HBOOST_MOVE_HAS_TRIVIAL_MOVE_ASSIGN(T)
#else
   #define HBOOST_MOVE_IS_TRIVIALLY_MOVE_ASSIGNABLE(T)  ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR
   #define HBOOST_MOVE_IS_TRIVIALLY_DESTRUCTIBLE(T)   HBOOST_MOVE_HAS_TRIVIAL_DESTRUCTOR(T)
#else
   #define HBOOST_MOVE_IS_TRIVIALLY_DESTRUCTIBLE(T)   ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_NOTHROW_CONSTRUCTOR
   #define HBOOST_MOVE_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T)  HBOOST_MOVE_HAS_NOTHROW_CONSTRUCTOR(T)
#else
   #define HBOOST_MOVE_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T)  ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_NOTHROW_COPY
   #define HBOOST_MOVE_IS_NOTHROW_COPY_CONSTRUCTIBLE(T)   HBOOST_MOVE_HAS_NOTHROW_COPY(T)
#else
   #define HBOOST_MOVE_IS_NOTHROW_COPY_CONSTRUCTIBLE(T)   ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_NOTHROW_MOVE
   #define HBOOST_MOVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)   HBOOST_MOVE_HAS_NOTHROW_MOVE(T)
#else
   #define HBOOST_MOVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)   ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_NOTHROW_ASSIGN
   #define HBOOST_MOVE_IS_NOTHROW_COPY_ASSIGNABLE(T) HBOOST_MOVE_HAS_NOTHROW_ASSIGN(T)
#else
   #define HBOOST_MOVE_IS_NOTHROW_COPY_ASSIGNABLE(T) ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_HAS_NOTHROW_MOVE_ASSIGN
   #define HBOOST_MOVE_IS_NOTHROW_MOVE_ASSIGNABLE(T) HBOOST_MOVE_HAS_NOTHROW_MOVE_ASSIGN(T)
#else
   #define HBOOST_MOVE_IS_NOTHROW_MOVE_ASSIGNABLE(T) ::hboost::move_detail::is_pod<T>::value
#endif

#ifdef HBOOST_MOVE_IS_ENUM
   #define HBOOST_MOVE_IS_ENUM_IMPL(T)   HBOOST_MOVE_IS_ENUM(T)
#else
   #define HBOOST_MOVE_IS_ENUM_IMPL(T)   ::hboost::move_detail::is_enum_nonintrinsic<T>::value
#endif

namespace hboost {
namespace move_detail {

//////////////////////////
//    is_reference
//////////////////////////
template<class T>
struct is_reference
{  static const bool value = false; };

template<class T>
struct is_reference<T&>
{  static const bool value = true; };

#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
template<class T>
struct is_reference<T&&>
{  static const bool value = true; };
#endif

//////////////////////////
//    is_pointer
//////////////////////////
template<class T>
struct is_pointer
{  static const bool value = false; };

template<class T>
struct is_pointer<T*>
{  static const bool value = true; };

//////////////////////////
//       is_const
//////////////////////////
template<class T>
struct is_const
{  static const bool value = false; };

template<class T>
struct is_const<const T>
{  static const bool value = true; };

//////////////////////////
//       unvoid_ref
//////////////////////////
template <typename T> struct unvoid_ref : add_lvalue_reference<T>{};
template <> struct unvoid_ref<void>                { typedef unvoid_ref & type; };
template <> struct unvoid_ref<const void>          { typedef unvoid_ref & type; };
template <> struct unvoid_ref<volatile void>       { typedef unvoid_ref & type; };
template <> struct unvoid_ref<const volatile void> { typedef unvoid_ref & type; };

template <typename T>
struct add_reference : add_lvalue_reference<T>
{};

//////////////////////////
//    add_const_reference
//////////////////////////
template <class T>
struct add_const_reference
{  typedef const T &type;   };

template <class T>
struct add_const_reference<T&>
{  typedef T& type;   };

//////////////////////////
//    add_const_if_c
//////////////////////////
template<class T, bool Add>
struct add_const_if_c
   : if_c<Add, typename add_const<T>::type, T>
{};

//////////////////////////
//    remove_const
//////////////////////////
template<class T>
struct remove_const
{  typedef T type;   };

template<class T>
struct remove_const< const T>
{  typedef T type;   };

//////////////////////////
//    remove_cv
//////////////////////////
template<typename T> struct remove_cv                    {  typedef T type;   };
template<typename T> struct remove_cv<const T>           {  typedef T type;   };
template<typename T> struct remove_cv<const volatile T>  {  typedef T type;   };
template<typename T> struct remove_cv<volatile T>        {  typedef T type;   };

//////////////////////////
//    make_unsigned
//////////////////////////
template <class T>
struct make_unsigned_impl                                         {  typedef T type;   };
template <> struct make_unsigned_impl<signed char>                {  typedef unsigned char  type; };
template <> struct make_unsigned_impl<signed short>               {  typedef unsigned short type; };
template <> struct make_unsigned_impl<signed int>                 {  typedef unsigned int   type; };
template <> struct make_unsigned_impl<signed long>                {  typedef unsigned long  type; };
#ifdef HBOOST_HAS_LONG_LONG
template <> struct make_unsigned_impl< ::hboost::long_long_type >  {  typedef ::hboost::ulong_long_type type; };
#endif

template <class T>
struct make_unsigned
   : make_unsigned_impl<typename remove_cv<T>::type>
{};

//////////////////////////
//    is_floating_point
//////////////////////////
template<class T> struct is_floating_point_cv               {  static const bool value = false; };
template<>        struct is_floating_point_cv<float>        {  static const bool value = true; };
template<>        struct is_floating_point_cv<double>       {  static const bool value = true; };
template<>        struct is_floating_point_cv<long double>  {  static const bool value = true; };

template<class T>
struct is_floating_point
   : is_floating_point_cv<typename remove_cv<T>::type>
{};

//////////////////////////
//    is_integral
//////////////////////////
template<class T> struct is_integral_cv                    {  static const bool value = false; };
template<> struct is_integral_cv<                     bool>{  static const bool value = true; };
template<> struct is_integral_cv<                     char>{  static const bool value = true; };
template<> struct is_integral_cv<            unsigned char>{  static const bool value = true; };
template<> struct is_integral_cv<              signed char>{  static const bool value = true; };
#ifndef HBOOST_NO_CXX11_CHAR16_T
template<> struct is_integral_cv<                 char16_t>{  static const bool value = true; };
#endif
#ifndef HBOOST_NO_CXX11_CHAR32_T
template<> struct is_integral_cv<                 char32_t>{  static const bool value = true; };
#endif
#ifndef HBOOST_NO_INTRINSIC_WCHAR_T
template<> struct is_integral_cv<                  wchar_t>{  static const bool value = true; };
#endif
template<> struct is_integral_cv<                    short>{  static const bool value = true; };
template<> struct is_integral_cv<           unsigned short>{  static const bool value = true; };
template<> struct is_integral_cv<                      int>{  static const bool value = true; };
template<> struct is_integral_cv<             unsigned int>{  static const bool value = true; };
template<> struct is_integral_cv<                     long>{  static const bool value = true; };
template<> struct is_integral_cv<            unsigned long>{  static const bool value = true; };
#ifdef HBOOST_HAS_LONG_LONG
template<> struct is_integral_cv< ::hboost:: long_long_type>{  static const bool value = true; };
template<> struct is_integral_cv< ::hboost::ulong_long_type>{  static const bool value = true; };
#endif

template<class T>
struct is_integral
   : public is_integral_cv<typename remove_cv<T>::type>
{};

//////////////////////////////////////
//          remove_all_extents
//////////////////////////////////////
template <class T>
struct remove_all_extents
{  typedef T type;};

template <class T>
struct remove_all_extents<T[]>
{  typedef typename remove_all_extents<T>::type type; };

template <class T, std::size_t N>
struct remove_all_extents<T[N]>
{  typedef typename remove_all_extents<T>::type type;};

//////////////////////////
//    is_scalar
//////////////////////////
template<class T>
struct is_scalar
{  static const bool value = is_integral<T>::value || is_floating_point<T>::value; };

//////////////////////////
//       is_void
//////////////////////////
template<class T>
struct is_void_cv
{  static const bool value = false; };

template<>
struct is_void_cv<void>
{  static const bool value = true; };

template<class T>
struct is_void
   : is_void_cv<typename remove_cv<T>::type>
{};

//////////////////////////////////////
//          is_array
//////////////////////////////////////
template<class T>
struct is_array
{  static const bool value = false; };

template<class T>
struct is_array<T[]>
{  static const bool value = true;  };

template<class T, std::size_t N>
struct is_array<T[N]>
{  static const bool value = true;  };

//////////////////////////////////////
//           is_member_pointer
//////////////////////////////////////
template <class T>         struct is_member_pointer_cv         {  static const bool value = false; };
template <class T, class U>struct is_member_pointer_cv<T U::*> {  static const bool value = true; };

template <class T>
struct is_member_pointer
    : is_member_pointer_cv<typename remove_cv<T>::type>
{};

//////////////////////////////////////
//          is_nullptr_t
//////////////////////////////////////
template <class T>
struct is_nullptr_t_cv
{  static const bool value = false; };

#if !defined(HBOOST_NO_CXX11_NULLPTR)
template <>
struct is_nullptr_t_cv
   #if !defined(HBOOST_NO_CXX11_DECLTYPE)
   <decltype(nullptr)>
   #else
   <std::nullptr_t>
   #endif
{  static const bool value = true; };
#endif

template <class T>
struct is_nullptr_t
   : is_nullptr_t_cv<typename remove_cv<T>::type>
{};

//////////////////////////////////////
//          is_function
//////////////////////////////////////
//Inspired by libc++, thanks to Howard Hinnant
//For a function to pointer an lvalue of function type T can be implicitly converted to a prvalue
//pointer to that function. This does not apply to non-static member functions because lvalues
//that refer to non-static member functions do not exist.
template <class T>
struct is_reference_convertible_to_pointer
{
   struct twochar { char dummy[2]; };
   template <class U> static char    test(U*);
   template <class U> static twochar test(...);
   static T& source();
   static const bool value = sizeof(char) == sizeof(test<T>(source()));
};
//Filter out:
// - class types that might have implicit conversions
// - void (to avoid forming a reference to void later)
// - references (e.g.: filtering reference to functions)
// - nullptr_t (convertible to pointer)
template < class T
         , bool Filter = is_class_or_union<T>::value  ||
                         is_void<T>::value            ||
                         is_reference<T>::value       ||
                         is_nullptr_t<T>::value       >
struct is_function_impl
{  static const bool value = is_reference_convertible_to_pointer<T>::value; };

template <class T>
struct is_function_impl<T, true>
{  static const bool value = false; };

template <class T>
struct is_function
   : is_function_impl<T>
{};

//////////////////////////////////////
//       is_union
//////////////////////////////////////
template<class T>
struct is_union_noextents_cv
{  static const bool value = HBOOST_MOVE_IS_UNION_IMPL(T); };

template<class T>
struct is_union
   : is_union_noextents_cv<typename remove_cv<typename remove_all_extents<T>::type>::type>
{};

//////////////////////////////////////
//             is_class
//////////////////////////////////////
template <class T>
struct is_class
{
   static const bool value = is_class_or_union<T>::value && ! is_union<T>::value;
};


//////////////////////////////////////
//             is_arithmetic
//////////////////////////////////////
template <class T>
struct is_arithmetic
{
   static const bool value = is_floating_point<T>::value ||
                             is_integral<T>::value;
};

//////////////////////////////////////
//    is_member_function_pointer
//////////////////////////////////////
template <class T>
struct is_member_function_pointer_cv
{
   static const bool value = false;
};

template <class T, class C>
struct is_member_function_pointer_cv<T C::*>
   : is_function<T>
{};

template <class T>
struct is_member_function_pointer
    : is_member_function_pointer_cv<typename remove_cv<T>::type>
{};

//////////////////////////////////////
//             is_enum
//////////////////////////////////////
#if !defined(HBOOST_MOVE_IS_ENUM)
//Based on (http://howardhinnant.github.io/TypeHiearchy.pdf)
template <class T>
struct is_enum_nonintrinsic
{
   static const bool value =  !is_arithmetic<T>::value     &&
                              !is_reference<T>::value      &&
                              !is_class_or_union<T>::value &&
                              !is_array<T>::value          &&
                              !is_void<T>::value           &&
                              !is_nullptr_t<T>::value      &&
                              !is_member_pointer<T>::value &&
                              !is_pointer<T>::value        &&
                              !is_function<T>::value;
};
#endif

template <class T>
struct is_enum
{  static const bool value = HBOOST_MOVE_IS_ENUM_IMPL(T);  };

//////////////////////////////////////
//       is_pod
//////////////////////////////////////
template<class T>
struct is_pod_noextents_cv  //for non-c++11 compilers, a safe fallback
{  static const bool value = HBOOST_MOVE_IS_POD_IMPL(T); };

template<class T>
struct is_pod
   : is_pod_noextents_cv<typename remove_cv<typename remove_all_extents<T>::type>::type>
{};

//////////////////////////////////////
//             is_empty
//////////////////////////////////////
#if !defined(HBOOST_MOVE_IS_EMPTY)

template <typename T>
struct empty_helper_t1 : public T
{
   empty_helper_t1();  // hh compiler bug workaround
   int i[256];
   private:

   empty_helper_t1(const empty_helper_t1&);
   empty_helper_t1& operator=(const empty_helper_t1&);
};

struct empty_helper_t2 { int i[256]; };

template <typename T, bool IsClass = is_class<T>::value >
struct is_empty_nonintrinsic
{
   static const bool value = false;
};

template <typename T>
struct is_empty_nonintrinsic<T, true>
{
   static const bool value = sizeof(empty_helper_t1<T>) == sizeof(empty_helper_t2);
};
#endif

template <class T>
struct is_empty
{  static const bool value = HBOOST_MOVE_IS_EMPTY_IMPL(T);  };


template<class T>
struct has_hboost_move_no_copy_constructor_or_assign_type
{
   template <class U>
   static yes_type test(typename U::hboost_move_no_copy_constructor_or_assign*);

   template <class U>
   static no_type test(...);

   static const bool value = sizeof(test<T>(0)) == sizeof(yes_type);
};

//////////////////////////////////////
//       is_copy_constructible
//////////////////////////////////////
#if !defined(HBOOST_NO_CXX11_DELETED_FUNCTIONS) && !defined(HBOOST_NO_CXX11_DECLTYPE) \
   && !defined(HBOOST_INTEL_CXX_VERSION) && \
      !(defined(HBOOST_MSVC) && _MSC_VER == 1800)
#define HBOOST_MOVE_TT_CXX11_IS_COPY_CONSTRUCTIBLE
#endif

template<class T>
struct is_copy_constructible
{
   // Intel compiler has problems with SFINAE for copy constructors and deleted functions:
   //
   // error: function *function_name* cannot be referenced -- it is a deleted function
   // static yes_type test(U&, decltype(U(hboost::declval<U&>()))* = 0);
   //                                                        ^ 
   // MSVC 12.0 (Visual 2013) has problems when the copy constructor has been deleted. See:
   // https://connect.microsoft.com/VisualStudio/feedback/details/800328/std-is-copy-constructible-is-broken
   #if defined(HBOOST_MOVE_TT_CXX11_IS_COPY_CONSTRUCTIBLE)
      template<class U> static typename add_reference<U>::type source();
      static no_type test(...);
      #ifdef HBOOST_NO_CXX11_DECLTYPE
         template <class U>
         static yes_type test(U&, bool_<sizeof(U(source<U>()))>* = 0);
      #else
         template <class U>
         static yes_type test(U&, decltype(U(source<U>()))* = 0);
      #endif
      static const bool value = sizeof(test(source<T>())) == sizeof(yes_type);
   #else
   static const bool value = !has_hboost_move_no_copy_constructor_or_assign_type<T>::value;
   #endif
};


//////////////////////////////////////
//       is_copy_assignable
//////////////////////////////////////
#if !defined(HBOOST_NO_CXX11_DELETED_FUNCTIONS) && !defined(HBOOST_NO_CXX11_DECLTYPE) \
   && !defined(HBOOST_INTEL_CXX_VERSION) && \
      !(defined(HBOOST_MSVC) && _MSC_VER == 1800)
#define HBOOST_MOVE_TT_CXX11_IS_COPY_ASSIGNABLE
#endif

template <class T>
struct is_copy_assignable
{
// Intel compiler has problems with SFINAE for copy constructors and deleted functions:
//
// error: function *function_name* cannot be referenced -- it is a deleted function
// static hboost::type_traits::yes_type test(T1&, decltype(T1(hboost::declval<T1&>()))* = 0);
//                                                        ^ 
//
// MSVC 12.0 (Visual 2013) has problems when the copy constructor has been deleted. See:
// https://connect.microsoft.com/VisualStudio/feedback/details/800328/std-is-copy-constructible-is-broken
#if defined(HBOOST_MOVE_TT_CXX11_IS_COPY_ASSIGNABLE)
   typedef char yes_type;
   struct no_type { char dummy[2]; };
   
   template <class U>   static typename add_reference<U>::type source();
   template <class U>   static decltype(source<U&>() = source<const U&>(), yes_type() ) test(int);
   template <class>     static no_type test(...);

   static const bool value = sizeof(test<T>(0)) == sizeof(yes_type);
#else
   static const bool value = !has_hboost_move_no_copy_constructor_or_assign_type<T>::value;
#endif
};

//////////////////////////////////////
//       is_trivially_destructible
//////////////////////////////////////
template<class T>
struct is_trivially_destructible
{  static const bool value = HBOOST_MOVE_IS_TRIVIALLY_DESTRUCTIBLE(T); };

//////////////////////////////////////
//       is_trivially_default_constructible
//////////////////////////////////////
template<class T>
struct is_trivially_default_constructible
{  static const bool value = HBOOST_MOVE_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T); };

//////////////////////////////////////
//       is_trivially_copy_constructible
//////////////////////////////////////
template<class T>
struct is_trivially_copy_constructible
{
   //In several compilers HBOOST_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE return true even with
   //deleted copy constructors so make sure the type is copy constructible.
   static const bool value = ::hboost::move_detail::is_pod<T>::value ||
                             ( ::hboost::move_detail::is_copy_constructible<T>::value &&
                               HBOOST_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T) );
};

//////////////////////////////////////
//       is_trivially_move_constructible
//////////////////////////////////////
template<class T>
struct is_trivially_move_constructible
{  static const bool value = HBOOST_MOVE_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T); };

//////////////////////////////////////
//       is_trivially_copy_assignable
//////////////////////////////////////
template<class T>
struct is_trivially_copy_assignable
{
   //In several compilers HBOOST_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE return true even with
   //deleted copy constructors so make sure the type is copy constructible.
   static const bool value = ::hboost::move_detail::is_pod<T>::value ||
                             ( ::hboost::move_detail::is_copy_assignable<T>::value &&
                               HBOOST_MOVE_IS_TRIVIALLY_COPY_ASSIGNABLE(T) );
};                             

//////////////////////////////////////
//       is_trivially_move_assignable
//////////////////////////////////////
template<class T>
struct is_trivially_move_assignable
{  static const bool value = HBOOST_MOVE_IS_TRIVIALLY_MOVE_ASSIGNABLE(T);  };

//////////////////////////////////////
//       is_nothrow_default_constructible
//////////////////////////////////////
template<class T>
struct is_nothrow_default_constructible
   : is_pod<T>
{  static const bool value = HBOOST_MOVE_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T);  };

//////////////////////////////////////
//    is_nothrow_copy_constructible
//////////////////////////////////////
template<class T>
struct is_nothrow_copy_constructible
{  static const bool value = HBOOST_MOVE_IS_NOTHROW_COPY_CONSTRUCTIBLE(T);  };

//////////////////////////////////////
//    is_nothrow_move_constructible
//////////////////////////////////////
template<class T>
struct is_nothrow_move_constructible
{  static const bool value = HBOOST_MOVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T);  };

//////////////////////////////////////
//       is_nothrow_copy_assignable
//////////////////////////////////////
template<class T>
struct is_nothrow_copy_assignable
{  static const bool value = HBOOST_MOVE_IS_NOTHROW_COPY_ASSIGNABLE(T);  };

//////////////////////////////////////
//    is_nothrow_move_assignable
//////////////////////////////////////
template<class T>
struct is_nothrow_move_assignable
{  static const bool value = HBOOST_MOVE_IS_NOTHROW_MOVE_ASSIGNABLE(T);  };

//////////////////////////////////////
//    is_nothrow_swappable
//////////////////////////////////////
template<class T>
struct is_nothrow_swappable
{
   static const bool value = is_empty<T>::value || is_pod<T>::value;
};

//////////////////////////////////////
//       alignment_of
//////////////////////////////////////
template <typename T>
struct alignment_of_hack
{
   T t1;
   char c;
   T t2;
   alignment_of_hack();
};

template <unsigned A, unsigned S>
struct alignment_logic
{  static const std::size_t value = A < S ? A : S; };

template< typename T >
struct alignment_of_impl
#if defined(HBOOST_MSVC) && (HBOOST_MSVC >= 1400)
    // With MSVC both the native __alignof operator
    // and our own logic gets things wrong from time to time :-(
    // Using a combination of the two seems to make the most of a bad job:
   : alignment_logic< sizeof(alignment_of_hack<T>) - 2*sizeof(T), __alignof(T)>
{};
#elif !defined(HBOOST_MOVE_ALIGNMENT_OF)
   : alignment_logic< sizeof(alignment_of_hack<T>) - 2*sizeof(T), sizeof(T)>
{};
#else
{  static const std::size_t value = HBOOST_MOVE_ALIGNMENT_OF(T);  };
#endif

template< typename T >
struct alignment_of
   : alignment_of_impl<T>
{};

class alignment_dummy;
typedef void (*function_ptr)();
typedef int (alignment_dummy::*member_ptr);
typedef int (alignment_dummy::*member_function_ptr)();
struct alignment_struct
{  long double dummy[4];  };

/////////////////////////////
//    max_align_t
/////////////////////////////
//This is not standard, but should work with all compilers
union max_align
{
   char        char_;
   short       short_;
   int         int_;
   long        long_;
   #ifdef HBOOST_HAS_LONG_LONG
   ::hboost::long_long_type   long_long_;
   #endif
   float       float_;
   double      double_;
   void *      void_ptr_;
   long double long_double_[4];
   alignment_dummy *unknown_class_ptr_;
   function_ptr function_ptr_;
   member_function_ptr member_function_ptr_;
   alignment_struct alignment_struct_;
};

typedef union max_align max_align_t;

/////////////////////////////
//    aligned_storage
/////////////////////////////

#if !defined(HBOOST_NO_ALIGNMENT)

template<std::size_t Len, std::size_t Align>
struct aligned_storage_impl;

#define HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(A)\
template<std::size_t Len>\
struct HBOOST_ALIGNMENT(A) aligned_storage_impl<Len, A>\
{\
   char dummy[Len];\
   typedef aligned_storage_impl<Len, A> type;\
};\
//

//Up to 4K alignment (typical page size)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x1)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x2)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x4)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x8)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x10)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x20)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x40)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x80)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x100)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x200)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x400)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x800)
HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT(0x1000)

#undef HBOOST_MOVE_ALIGNED_STORAGE_WITH_HBOOST_ALIGNMENT

#else //HBOOST_NO_ALIGNMENT

template<class T, std::size_t Len>
union aligned_union
{	
   T aligner;
   char dummy[Len];
};

template<std::size_t Len, std::size_t Align, class T, bool Ok>
struct aligned_next;

template<std::size_t Len, std::size_t Align, class T>
struct aligned_next<Len, Align, T, true>
{
   HBOOST_STATIC_ASSERT((alignment_of<T>::value == Align));
   typedef aligned_union<T, Len> type;
};

//End of search defaults to max_align_t
template<std::size_t Len, std::size_t Align>
struct aligned_next<Len, Align, max_align_t, false>
{	typedef aligned_union<max_align_t, Len> type;   };

//Now define a search list through types
#define HBOOST_MOVE_ALIGNED_NEXT_STEP(TYPE, NEXT_TYPE)\
   template<std::size_t Len, std::size_t Align>\
   struct aligned_next<Len, Align, TYPE, false>\
      : aligned_next<Len, Align, NEXT_TYPE, Align == alignment_of<NEXT_TYPE>::value>\
   {};\
   //
   HBOOST_MOVE_ALIGNED_NEXT_STEP(long double, max_align_t)
   HBOOST_MOVE_ALIGNED_NEXT_STEP(double, long double)
   #ifdef HBOOST_HAS_LONG_LONG
      HBOOST_MOVE_ALIGNED_NEXT_STEP(::hboost::long_long_type, double)
      HBOOST_MOVE_ALIGNED_NEXT_STEP(long, ::hboost::long_long_type)
   #else
      HBOOST_MOVE_ALIGNED_NEXT_STEP(long, double)
   #endif
   HBOOST_MOVE_ALIGNED_NEXT_STEP(int, long)
   HBOOST_MOVE_ALIGNED_NEXT_STEP(short, int)
   HBOOST_MOVE_ALIGNED_NEXT_STEP(char, short)
#undef HBOOST_MOVE_ALIGNED_NEXT_STEP

template<std::size_t Len, std::size_t Align>
struct aligned_storage_impl
   : aligned_next<Len, Align, char, Align == alignment_of<char>::value>
{};

#endif

template<std::size_t Len, std::size_t Align = alignment_of<max_align_t>::value>
struct aligned_storage
{
   //Sanity checks for input parameters
   HBOOST_STATIC_ASSERT(Align > 0);

   //Sanity checks for output type
   typedef typename aligned_storage_impl<Len ? Len : 1, Align>::type type;
   static const std::size_t value = alignment_of<type>::value;
   HBOOST_STATIC_ASSERT(value >= Align);
   HBOOST_STATIC_ASSERT((value % Align) == 0);

   //Just in case someone instantiates aligned_storage
   //instead of aligned_storage::type (typical error).
   private:
   aligned_storage();
};

}  //namespace move_detail {
}  //namespace hboost {

#include <hboost/move/detail/config_end.hpp>

#endif   //#ifndef HBOOST_MOVE_DETAIL_TYPE_TRAITS_HPP
