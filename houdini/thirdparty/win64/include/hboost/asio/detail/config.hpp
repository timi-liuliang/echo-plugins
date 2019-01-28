//
// detail/config.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_CONFIG_HPP
#define HBOOST_ASIO_DETAIL_CONFIG_HPP

#if defined(HBOOST_ASIO_STANDALONE)
# define HBOOST_ASIO_DISABLE_HBOOST_ARRAY 1
# define HBOOST_ASIO_DISABLE_HBOOST_ASSERT 1
# define HBOOST_ASIO_DISABLE_HBOOST_BIND 1
# define HBOOST_ASIO_DISABLE_HBOOST_CHRONO 1
# define HBOOST_ASIO_DISABLE_HBOOST_DATE_TIME 1
# define HBOOST_ASIO_DISABLE_HBOOST_LIMITS 1
# define HBOOST_ASIO_DISABLE_HBOOST_REGEX 1
# define HBOOST_ASIO_DISABLE_HBOOST_STATIC_CONSTANT 1
# define HBOOST_ASIO_DISABLE_HBOOST_THROW_EXCEPTION 1
# define HBOOST_ASIO_DISABLE_HBOOST_WORKAROUND 1
#else // defined(HBOOST_ASIO_STANDALONE)
# include <hboost/config.hpp>
# include <hboost/version.hpp>
# define HBOOST_ASIO_HAS_HBOOST_CONFIG 1
#endif // defined(HBOOST_ASIO_STANDALONE)

// Default to a header-only implementation. The user must specifically request
// separate compilation by defining either HBOOST_ASIO_SEPARATE_COMPILATION or
// HBOOST_ASIO_DYN_LINK (as a DLL/shared library implies separate compilation).
#if !defined(HBOOST_ASIO_HEADER_ONLY)
# if !defined(HBOOST_ASIO_SEPARATE_COMPILATION)
#  if !defined(HBOOST_ASIO_DYN_LINK)
#   define HBOOST_ASIO_HEADER_ONLY 1
#  endif // !defined(HBOOST_ASIO_DYN_LINK)
# endif // !defined(HBOOST_ASIO_SEPARATE_COMPILATION)
#endif // !defined(HBOOST_ASIO_HEADER_ONLY)

#if defined(HBOOST_ASIO_HEADER_ONLY)
# define HBOOST_ASIO_DECL inline
#else // defined(HBOOST_ASIO_HEADER_ONLY)
# if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
// We need to import/export our code only if the user has specifically asked
// for it by defining HBOOST_ASIO_DYN_LINK.
#  if defined(HBOOST_ASIO_DYN_LINK)
// Export if this is our own source, otherwise import.
#   if defined(HBOOST_ASIO_SOURCE)
#    define HBOOST_ASIO_DECL __declspec(dllexport)
#   else // defined(HBOOST_ASIO_SOURCE)
#    define HBOOST_ASIO_DECL __declspec(dllimport)
#   endif // defined(HBOOST_ASIO_SOURCE)
#  endif // defined(HBOOST_ASIO_DYN_LINK)
# endif // defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

// If HBOOST_ASIO_DECL isn't defined yet define it now.
#if !defined(HBOOST_ASIO_DECL)
# define HBOOST_ASIO_DECL
#endif // !defined(HBOOST_ASIO_DECL)

// Microsoft Visual C++ detection.
#if !defined(HBOOST_ASIO_MSVC)
# if defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_MSVC)
#  define HBOOST_ASIO_MSVC HBOOST_MSVC
# elif defined(_MSC_VER) && !defined(__MWERKS__) && !defined(__EDG_VERSION__)
#  define HBOOST_ASIO_MSVC _MSC_VER
# endif // defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_MSVC)
#endif // defined(HBOOST_ASIO_MSVC)

// Clang / libc++ detection.
#if defined(__clang__)
# if (__cplusplus >= 201103)
#  if __has_include(<__config>)
#   include <__config>
#   if defined(_LIBCPP_VERSION)
#    define HBOOST_ASIO_HAS_CLANG_LIBCXX 1
#   endif // defined(_LIBCPP_VERSION)
#  endif // __has_include(<__config>)
# endif // (__cplusplus >= 201103)
#endif // defined(__clang__)

// Support move construction and assignment on compilers known to allow it.
#if !defined(HBOOST_ASIO_HAS_MOVE)
# if !defined(HBOOST_ASIO_DISABLE_MOVE)
#  if defined(__clang__)
#   if __has_feature(__cxx_rvalue_references__)
#    define HBOOST_ASIO_HAS_MOVE 1
#   endif // __has_feature(__cxx_rvalue_references__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_MOVE 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_MOVE 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_MOVE)
#endif // !defined(HBOOST_ASIO_HAS_MOVE)

// If HBOOST_ASIO_MOVE_CAST isn't defined, and move support is available, define
// HBOOST_ASIO_MOVE_ARG and HBOOST_ASIO_MOVE_CAST to take advantage of rvalue
// references and perfect forwarding.
#if defined(HBOOST_ASIO_HAS_MOVE) && !defined(HBOOST_ASIO_MOVE_CAST)
# define HBOOST_ASIO_MOVE_ARG(type) type&&
# define HBOOST_ASIO_MOVE_CAST(type) static_cast<type&&>
# define HBOOST_ASIO_MOVE_CAST2(type1, type2) static_cast<type1, type2&&>
#endif // defined(HBOOST_ASIO_HAS_MOVE) && !defined(HBOOST_ASIO_MOVE_CAST)

// If HBOOST_ASIO_MOVE_CAST still isn't defined, default to a C++03-compatible
// implementation. Note that older g++ and MSVC versions don't like it when you
// pass a non-member function through a const reference, so for most compilers
// we'll play it safe and stick with the old approach of passing the handler by
// value.
#if !defined(HBOOST_ASIO_MOVE_CAST)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
#   define HBOOST_ASIO_MOVE_ARG(type) const type&
#  else // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
#   define HBOOST_ASIO_MOVE_ARG(type) type
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
# elif defined(HBOOST_ASIO_MSVC)
#  if (_MSC_VER >= 1400)
#   define HBOOST_ASIO_MOVE_ARG(type) const type&
#  else // (_MSC_VER >= 1400)
#   define HBOOST_ASIO_MOVE_ARG(type) type
#  endif // (_MSC_VER >= 1400)
# else
#  define HBOOST_ASIO_MOVE_ARG(type) type
# endif
# define HBOOST_ASIO_MOVE_CAST(type) static_cast<const type&>
# define HBOOST_ASIO_MOVE_CAST2(type1, type2) static_cast<const type1, type2&>
#endif // !defined(HBOOST_ASIO_MOVE_CAST)

// Support variadic templates on compilers known to allow it.
#if !defined(HBOOST_ASIO_HAS_VARIADIC_TEMPLATES)
# if !defined(HBOOST_ASIO_DISABLE_VARIADIC_TEMPLATES)
#  if defined(__clang__)
#   if __has_feature(__cxx_variadic_templates__)
#    define HBOOST_ASIO_HAS_VARIADIC_TEMPLATES 1
#   endif // __has_feature(__cxx_variadic_templates__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_VARIADIC_TEMPLATES 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(HBOOST_ASIO_DISABLE_VARIADIC_TEMPLATES)
#endif // !defined(HBOOST_ASIO_HAS_VARIADIC_TEMPLATES)

// Support constexpr on compilers known to allow it.
#if !defined(HBOOST_ASIO_HAS_CONSTEXPR)
# if !defined(HBOOST_ASIO_DISABLE_CONSTEXPR)
#  if defined(__clang__)
#   if __has_feature(__cxx_constexpr__)
#    define HBOOST_ASIO_HAS_CONSTEXPR 1
#   endif // __has_feature(__cxx_constexr__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_CONSTEXPR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(HBOOST_ASIO_DISABLE_CONSTEXPR)
#endif // !defined(HBOOST_ASIO_HAS_CONSTEXPR)
#if !defined(HBOOST_ASIO_CONSTEXPR)
# if defined(HBOOST_ASIO_HAS_CONSTEXPR)
#  define HBOOST_ASIO_CONSTEXPR constexpr
# else // defined(HBOOST_ASIO_HAS_CONSTEXPR)
#  define HBOOST_ASIO_CONSTEXPR
# endif // defined(HBOOST_ASIO_HAS_CONSTEXPR)
#endif // !defined(HBOOST_ASIO_CONSTEXPR)

// Standard library support for system errors.
# if !defined(HBOOST_ASIO_DISABLE_STD_SYSTEM_ERROR)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_SYSTEM_ERROR 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<system_error>)
#     define HBOOST_ASIO_HAS_STD_SYSTEM_ERROR 1
#    endif // __has_include(<system_error>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_SYSTEM_ERROR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_STD_SYSTEM_ERROR 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_SYSTEM_ERROR)

// Compliant C++11 compilers put noexcept specifiers on error_category members.
#if !defined(HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT)
# if (HBOOST_VERSION >= 105300)
#  define HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT HBOOST_NOEXCEPT
# elif defined(__clang__)
#  if __has_feature(__cxx_noexcept__)
#   define HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT noexcept(true)
#  endif // __has_feature(__cxx_noexcept__)
# elif defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#   if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT noexcept(true)
#   endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
# elif defined(HBOOST_ASIO_MSVC)
#  if (_MSC_VER >= 1900)
#   define HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT noexcept(true)
#  endif // (_MSC_VER >= 1900)
# endif // defined(HBOOST_ASIO_MSVC)
# if !defined(HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT)
#  define HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT
# endif // !defined(HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT)
#endif // !defined(HBOOST_ASIO_ERROR_CATEGORY_NOEXCEPT)

// Standard library support for arrays.
#if !defined(HBOOST_ASIO_HAS_STD_ARRAY)
# if !defined(HBOOST_ASIO_DISABLE_STD_ARRAY)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_ARRAY 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<array>)
#     define HBOOST_ASIO_HAS_STD_ARRAY 1
#    endif // __has_include(<array>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_ARRAY 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1600)
#    define HBOOST_ASIO_HAS_STD_ARRAY 1
#   endif // (_MSC_VER >= 1600)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_ARRAY)
#endif // !defined(HBOOST_ASIO_HAS_STD_ARRAY)

// Standard library support for shared_ptr and weak_ptr.
#if !defined(HBOOST_ASIO_HAS_STD_SHARED_PTR)
# if !defined(HBOOST_ASIO_DISABLE_STD_SHARED_PTR)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_SHARED_PTR 1
#   elif (__cplusplus >= 201103)
#    define HBOOST_ASIO_HAS_STD_SHARED_PTR 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_SHARED_PTR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1600)
#    define HBOOST_ASIO_HAS_STD_SHARED_PTR 1
#   endif // (_MSC_VER >= 1600)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_SHARED_PTR)
#endif // !defined(HBOOST_ASIO_HAS_STD_SHARED_PTR)

// Standard library support for atomic operations.
#if !defined(HBOOST_ASIO_HAS_STD_ATOMIC)
# if !defined(HBOOST_ASIO_DISABLE_STD_ATOMIC)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_ATOMIC 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<atomic>)
#     define HBOOST_ASIO_HAS_STD_ATOMIC 1
#    endif // __has_include(<atomic>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_ATOMIC 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_STD_ATOMIC 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_ATOMIC)
#endif // !defined(HBOOST_ASIO_HAS_STD_ATOMIC)

// Standard library support for chrono. Some standard libraries (such as the
// libstdc++ shipped with gcc 4.6) provide monotonic_clock as per early C++0x
// drafts, rather than the eventually standardised name of steady_clock.
#if !defined(HBOOST_ASIO_HAS_STD_CHRONO)
# if !defined(HBOOST_ASIO_DISABLE_STD_CHRONO)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_CHRONO 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<chrono>)
#     define HBOOST_ASIO_HAS_STD_CHRONO 1
#    endif // __has_include(<chrono>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_CHRONO 1
#     if ((__GNUC__ == 4) && (__GNUC_MINOR__ == 6))
#      define HBOOST_ASIO_HAS_STD_CHRONO_MONOTONIC_CLOCK 1
#     endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ == 6))
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_STD_CHRONO 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_CHRONO)
#endif // !defined(HBOOST_ASIO_HAS_STD_CHRONO)

// Boost support for chrono.
#if !defined(HBOOST_ASIO_HAS_HBOOST_CHRONO)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_CHRONO)
#  if (HBOOST_VERSION >= 104700)
#   define HBOOST_ASIO_HAS_HBOOST_CHRONO 1
#  endif // (HBOOST_VERSION >= 104700)
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_CHRONO)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_CHRONO)

// Boost support for the DateTime library.
#if !defined(HBOOST_ASIO_HAS_HBOOST_DATE_TIME)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_DATE_TIME)
#  define HBOOST_ASIO_HAS_HBOOST_DATE_TIME 1
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_DATE_TIME)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_DATE_TIME)

// Standard library support for addressof.
#if !defined(HBOOST_ASIO_HAS_STD_ADDRESSOF)
# if !defined(HBOOST_ASIO_DISABLE_STD_ADDRESSOF)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_ADDRESSOF 1
#   elif (__cplusplus >= 201103)
#    define HBOOST_ASIO_HAS_STD_ADDRESSOF 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_ADDRESSOF 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_STD_ADDRESSOF 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_ADDRESSOF)
#endif // !defined(HBOOST_ASIO_HAS_STD_ADDRESSOF)

// Standard library support for the function class.
#if !defined(HBOOST_ASIO_HAS_STD_FUNCTION)
# if !defined(HBOOST_ASIO_DISABLE_STD_FUNCTION)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_FUNCTION 1
#   elif (__cplusplus >= 201103)
#    define HBOOST_ASIO_HAS_STD_FUNCTION 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_FUNCTION 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_STD_FUNCTION 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_FUNCTION)
#endif // !defined(HBOOST_ASIO_HAS_STD_FUNCTION)

// Standard library support for type traits.
#if !defined(HBOOST_ASIO_HAS_STD_TYPE_TRAITS)
# if !defined(HBOOST_ASIO_DISABLE_STD_TYPE_TRAITS)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_TYPE_TRAITS 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<type_traits>)
#     define HBOOST_ASIO_HAS_STD_TYPE_TRAITS 1
#    endif // __has_include(<type_traits>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_TYPE_TRAITS 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_STD_TYPE_TRAITS 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_TYPE_TRAITS)
#endif // !defined(HBOOST_ASIO_HAS_STD_TYPE_TRAITS)

// Standard library support for the cstdint header.
#if !defined(HBOOST_ASIO_HAS_CSTDINT)
# if !defined(HBOOST_ASIO_DISABLE_CSTDINT)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_CSTDINT 1
#   elif (__cplusplus >= 201103)
#    define HBOOST_ASIO_HAS_CSTDINT 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_CSTDINT 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_CSTDINT 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_CSTDINT)
#endif // !defined(HBOOST_ASIO_HAS_CSTDINT)

// Standard library support for the thread class.
#if !defined(HBOOST_ASIO_HAS_STD_THREAD)
# if !defined(HBOOST_ASIO_DISABLE_STD_THREAD)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_THREAD 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<thread>)
#     define HBOOST_ASIO_HAS_STD_THREAD 1
#    endif // __has_include(<thread>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_THREAD 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_STD_THREAD 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_THREAD)
#endif // !defined(HBOOST_ASIO_HAS_STD_THREAD)

// Standard library support for the mutex and condition variable classes.
#if !defined(HBOOST_ASIO_HAS_STD_MUTEX_AND_CONDVAR)
# if !defined(HBOOST_ASIO_DISABLE_STD_MUTEX_AND_CONDVAR)
#  if defined(__clang__)
#   if defined(HBOOST_ASIO_HAS_CLANG_LIBCXX)
#    define HBOOST_ASIO_HAS_STD_MUTEX_AND_CONDVAR 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<mutex>)
#     define HBOOST_ASIO_HAS_STD_MUTEX_AND_CONDVAR 1
#    endif // __has_include(<mutex>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define HBOOST_ASIO_HAS_STD_MUTEX_AND_CONDVAR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(HBOOST_ASIO_MSVC)
#   if (_MSC_VER >= 1700)
#    define HBOOST_ASIO_HAS_STD_MUTEX_AND_CONDVAR 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(HBOOST_ASIO_MSVC)
# endif // !defined(HBOOST_ASIO_DISABLE_STD_MUTEX_AND_CONDVAR)
#endif // !defined(HBOOST_ASIO_HAS_STD_MUTEX_AND_CONDVAR)

// Windows App target. Windows but with a limited API.
#if !defined(HBOOST_ASIO_WINDOWS_APP)
# if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0603)
#  include <winapifamily.h>
#  if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) \
   && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#   define HBOOST_ASIO_WINDOWS_APP 1
#  endif // WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
         // && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
# endif // defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0603)
#endif // !defined(HBOOST_ASIO_WINDOWS_APP)

// Legacy WinRT target. Windows App is preferred.
#if !defined(HBOOST_ASIO_WINDOWS_RUNTIME)
# if !defined(HBOOST_ASIO_WINDOWS_APP)
#  if defined(__cplusplus_winrt)
#   include <winapifamily.h>
#   if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) \
    && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#    define HBOOST_ASIO_WINDOWS_RUNTIME 1
#   endif // WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
          // && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#  endif // defined(__cplusplus_winrt)
# endif // !defined(HBOOST_ASIO_WINDOWS_APP)
#endif // !defined(HBOOST_ASIO_WINDOWS_RUNTIME)

// Windows target. Excludes WinRT but includes Windows App targets.
#if !defined(HBOOST_ASIO_WINDOWS)
# if !defined(HBOOST_ASIO_WINDOWS_RUNTIME)
#  if defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_WINDOWS)
#   define HBOOST_ASIO_WINDOWS 1
#  elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define HBOOST_ASIO_WINDOWS 1
#  elif defined(HBOOST_ASIO_WINDOWS_APP)
#   define HBOOST_ASIO_WINDOWS 1
#  endif // defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_WINDOWS)
# endif // !defined(HBOOST_ASIO_WINDOWS_RUNTIME)
#endif // !defined(HBOOST_ASIO_WINDOWS)

// Windows: target OS version.
#if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
# if !defined(_WIN32_WINNT) && !defined(_WIN32_WINDOWS)
#  if defined(_MSC_VER) || defined(__BORLANDC__)
#   pragma message( \
  "Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately. For example:\n"\
  "- add -D_WIN32_WINNT=0x0501 to the compiler command line; or\n"\
  "- add _WIN32_WINNT=0x0501 to your project's Preprocessor Definitions.\n"\
  "Assuming _WIN32_WINNT=0x0501 (i.e. Windows XP target).")
#  else // defined(_MSC_VER) || defined(__BORLANDC__)
#   warning Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately.
#   warning For example, add -D_WIN32_WINNT=0x0501 to the compiler command line.
#   warning Assuming _WIN32_WINNT=0x0501 (i.e. Windows XP target).
#  endif // defined(_MSC_VER) || defined(__BORLANDC__)
#  define _WIN32_WINNT 0x0501
# endif // !defined(_WIN32_WINNT) && !defined(_WIN32_WINDOWS)
# if defined(_MSC_VER)
#  if defined(_WIN32) && !defined(WIN32)
#   if !defined(_WINSOCK2API_)
#    define WIN32 // Needed for correct types in winsock2.h
#   else // !defined(_WINSOCK2API_)
#    error Please define the macro WIN32 in your compiler options
#   endif // !defined(_WINSOCK2API_)
#  endif // defined(_WIN32) && !defined(WIN32)
# endif // defined(_MSC_VER)
# if defined(__BORLANDC__)
#  if defined(__WIN32__) && !defined(WIN32)
#   if !defined(_WINSOCK2API_)
#    define WIN32 // Needed for correct types in winsock2.h
#   else // !defined(_WINSOCK2API_)
#    error Please define the macro WIN32 in your compiler options
#   endif // !defined(_WINSOCK2API_)
#  endif // defined(__WIN32__) && !defined(WIN32)
# endif // defined(__BORLANDC__)
# if defined(__CYGWIN__)
#  if !defined(__USE_W32_SOCKETS)
#   error You must add -D__USE_W32_SOCKETS to your compiler options.
#  endif // !defined(__USE_W32_SOCKETS)
# endif // defined(__CYGWIN__)
#endif // defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)

// Windows: minimise header inclusion.
#if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
# if !defined(HBOOST_ASIO_NO_WIN32_LEAN_AND_MEAN)
#  if !defined(WIN32_LEAN_AND_MEAN)
#   define WIN32_LEAN_AND_MEAN
#  endif // !defined(WIN32_LEAN_AND_MEAN)
# endif // !defined(HBOOST_ASIO_NO_WIN32_LEAN_AND_MEAN)
#endif // defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)

// Windows: suppress definition of "min" and "max" macros.
#if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
# if !defined(HBOOST_ASIO_NO_NOMINMAX)
#  if !defined(NOMINMAX)
#   define NOMINMAX 1
#  endif // !defined(NOMINMAX)
# endif // !defined(HBOOST_ASIO_NO_NOMINMAX)
#endif // defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)

// Windows: IO Completion Ports.
#if !defined(HBOOST_ASIO_HAS_IOCP)
# if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
#  if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)
#   if !defined(UNDER_CE) && !defined(HBOOST_ASIO_WINDOWS_APP)
#    if !defined(HBOOST_ASIO_DISABLE_IOCP)
#     define HBOOST_ASIO_HAS_IOCP 1
#    endif // !defined(HBOOST_ASIO_DISABLE_IOCP)
#   endif // !defined(UNDER_CE) && !defined(HBOOST_ASIO_WINDOWS_APP)
#  endif // defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)
# endif // defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
#endif // !defined(HBOOST_ASIO_HAS_IOCP)

// On POSIX (and POSIX-like) platforms we need to include unistd.h in order to
// get access to the various platform feature macros, e.g. to be able to test
// for threads support.
#if !defined(HBOOST_ASIO_HAS_UNISTD_H)
# if !defined(HBOOST_ASIO_HAS_HBOOST_CONFIG)
#  if defined(unix) \
   || defined(__unix) \
   || defined(_XOPEN_SOURCE) \
   || defined(_POSIX_SOURCE) \
   || (defined(__MACH__) && defined(__APPLE__)) \
   || defined(__FreeBSD__) \
   || defined(__NetBSD__) \
   || defined(__OpenBSD__) \
   || defined(__linux__)
#   define HBOOST_ASIO_HAS_UNISTD_H 1
#  endif
# endif // !defined(HBOOST_ASIO_HAS_HBOOST_CONFIG)
#endif // !defined(HBOOST_ASIO_HAS_UNISTD_H)
#if defined(HBOOST_ASIO_HAS_UNISTD_H)
# include <unistd.h>
#endif // defined(HBOOST_ASIO_HAS_UNISTD_H)

// Linux: epoll, eventfd and timerfd.
#if defined(__linux__)
# include <linux/version.h>
# if !defined(HBOOST_ASIO_HAS_EPOLL)
#  if !defined(HBOOST_ASIO_DISABLE_EPOLL)
#   if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,45)
#    define HBOOST_ASIO_HAS_EPOLL 1
#   endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,45)
#  endif // !defined(HBOOST_ASIO_DISABLE_EPOLL)
# endif // !defined(HBOOST_ASIO_HAS_EPOLL)
# if !defined(HBOOST_ASIO_HAS_EVENTFD)
#  if !defined(HBOOST_ASIO_DISABLE_EVENTFD)
#   if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#    define HBOOST_ASIO_HAS_EVENTFD 1
#   endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#  endif // !defined(HBOOST_ASIO_DISABLE_EVENTFD)
# endif // !defined(HBOOST_ASIO_HAS_EVENTFD)
# if !defined(HBOOST_ASIO_HAS_TIMERFD)
#  if defined(HBOOST_ASIO_HAS_EPOLL)
#   if (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 8)
#    define HBOOST_ASIO_HAS_TIMERFD 1
#   endif // (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 8)
#  endif // defined(HBOOST_ASIO_HAS_EPOLL)
# endif // !defined(HBOOST_ASIO_HAS_TIMERFD)
#endif // defined(__linux__)

// Mac OS X, FreeBSD, NetBSD, OpenBSD: kqueue.
#if (defined(__MACH__) && defined(__APPLE__)) \
  || defined(__FreeBSD__) \
  || defined(__NetBSD__) \
  || defined(__OpenBSD__)
# if !defined(HBOOST_ASIO_HAS_KQUEUE)
#  if !defined(HBOOST_ASIO_DISABLE_KQUEUE)
#   define HBOOST_ASIO_HAS_KQUEUE 1
#  endif // !defined(HBOOST_ASIO_DISABLE_KQUEUE)
# endif // !defined(HBOOST_ASIO_HAS_KQUEUE)
#endif // (defined(__MACH__) && defined(__APPLE__))
       //   || defined(__FreeBSD__)
       //   || defined(__NetBSD__)
       //   || defined(__OpenBSD__)

// Solaris: /dev/poll.
#if defined(__sun)
# if !defined(HBOOST_ASIO_HAS_DEV_POLL)
#  if !defined(HBOOST_ASIO_DISABLE_DEV_POLL)
#   define HBOOST_ASIO_HAS_DEV_POLL 1
#  endif // !defined(HBOOST_ASIO_DISABLE_DEV_POLL)
# endif // !defined(HBOOST_ASIO_HAS_DEV_POLL)
#endif // defined(__sun)

// Serial ports.
#if !defined(HBOOST_ASIO_HAS_SERIAL_PORT)
# if defined(HBOOST_ASIO_HAS_IOCP) \
  || !defined(HBOOST_ASIO_WINDOWS) \
  && !defined(HBOOST_ASIO_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)
#  if !defined(__SYMBIAN32__)
#   if !defined(HBOOST_ASIO_DISABLE_SERIAL_PORT)
#    define HBOOST_ASIO_HAS_SERIAL_PORT 1
#   endif // !defined(HBOOST_ASIO_DISABLE_SERIAL_PORT)
#  endif // !defined(__SYMBIAN32__)
# endif // defined(HBOOST_ASIO_HAS_IOCP)
        //   || !defined(HBOOST_ASIO_WINDOWS)
        //   && !defined(HBOOST_ASIO_WINDOWS_RUNTIME)
        //   && !defined(__CYGWIN__)
#endif // !defined(HBOOST_ASIO_HAS_SERIAL_PORT)

// Windows: stream handles.
#if !defined(HBOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE)
# if !defined(HBOOST_ASIO_DISABLE_WINDOWS_STREAM_HANDLE)
#  if defined(HBOOST_ASIO_HAS_IOCP)
#   define HBOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE 1
#  endif // defined(HBOOST_ASIO_HAS_IOCP)
# endif // !defined(HBOOST_ASIO_DISABLE_WINDOWS_STREAM_HANDLE)
#endif // !defined(HBOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE)

// Windows: random access handles.
#if !defined(HBOOST_ASIO_HAS_WINDOWS_RANDOM_ACCESS_HANDLE)
# if !defined(HBOOST_ASIO_DISABLE_WINDOWS_RANDOM_ACCESS_HANDLE)
#  if defined(HBOOST_ASIO_HAS_IOCP)
#   define HBOOST_ASIO_HAS_WINDOWS_RANDOM_ACCESS_HANDLE 1
#  endif // defined(HBOOST_ASIO_HAS_IOCP)
# endif // !defined(HBOOST_ASIO_DISABLE_WINDOWS_RANDOM_ACCESS_HANDLE)
#endif // !defined(HBOOST_ASIO_HAS_WINDOWS_RANDOM_ACCESS_HANDLE)

// Windows: object handles.
#if !defined(HBOOST_ASIO_HAS_WINDOWS_OBJECT_HANDLE)
# if !defined(HBOOST_ASIO_DISABLE_WINDOWS_OBJECT_HANDLE)
#  if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
#   if !defined(UNDER_CE) && !defined(HBOOST_ASIO_WINDOWS_APP)
#    define HBOOST_ASIO_HAS_WINDOWS_OBJECT_HANDLE 1
#   endif // !defined(UNDER_CE) && !defined(HBOOST_ASIO_WINDOWS_APP)
#  endif // defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
# endif // !defined(HBOOST_ASIO_DISABLE_WINDOWS_OBJECT_HANDLE)
#endif // !defined(HBOOST_ASIO_HAS_WINDOWS_OBJECT_HANDLE)

// Windows: OVERLAPPED wrapper.
#if !defined(HBOOST_ASIO_HAS_WINDOWS_OVERLAPPED_PTR)
# if !defined(HBOOST_ASIO_DISABLE_WINDOWS_OVERLAPPED_PTR)
#  if defined(HBOOST_ASIO_HAS_IOCP)
#   define HBOOST_ASIO_HAS_WINDOWS_OVERLAPPED_PTR 1
#  endif // defined(HBOOST_ASIO_HAS_IOCP)
# endif // !defined(HBOOST_ASIO_DISABLE_WINDOWS_OVERLAPPED_PTR)
#endif // !defined(HBOOST_ASIO_HAS_WINDOWS_OVERLAPPED_PTR)

// POSIX: stream-oriented file descriptors.
#if !defined(HBOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
# if !defined(HBOOST_ASIO_DISABLE_POSIX_STREAM_DESCRIPTOR)
#  if !defined(HBOOST_ASIO_WINDOWS) \
  && !defined(HBOOST_ASIO_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)
#   define HBOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR 1
#  endif // !defined(HBOOST_ASIO_WINDOWS)
         //   && !defined(HBOOST_ASIO_WINDOWS_RUNTIME)
         //   && !defined(__CYGWIN__)
# endif // !defined(HBOOST_ASIO_DISABLE_POSIX_STREAM_DESCRIPTOR)
#endif // !defined(HBOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)

// UNIX domain sockets.
#if !defined(HBOOST_ASIO_HAS_LOCAL_SOCKETS)
# if !defined(HBOOST_ASIO_DISABLE_LOCAL_SOCKETS)
#  if !defined(HBOOST_ASIO_WINDOWS) \
  && !defined(HBOOST_ASIO_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)
#   define HBOOST_ASIO_HAS_LOCAL_SOCKETS 1
#  endif // !defined(HBOOST_ASIO_WINDOWS)
         //   && !defined(HBOOST_ASIO_WINDOWS_RUNTIME)
         //   && !defined(__CYGWIN__)
# endif // !defined(HBOOST_ASIO_DISABLE_LOCAL_SOCKETS)
#endif // !defined(HBOOST_ASIO_HAS_LOCAL_SOCKETS)

// Can use sigaction() instead of signal().
#if !defined(HBOOST_ASIO_HAS_SIGACTION)
# if !defined(HBOOST_ASIO_DISABLE_SIGACTION)
#  if !defined(HBOOST_ASIO_WINDOWS) \
  && !defined(HBOOST_ASIO_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)
#   define HBOOST_ASIO_HAS_SIGACTION 1
#  endif // !defined(HBOOST_ASIO_WINDOWS)
         //   && !defined(HBOOST_ASIO_WINDOWS_RUNTIME)
         //   && !defined(__CYGWIN__)
# endif // !defined(HBOOST_ASIO_DISABLE_SIGACTION)
#endif // !defined(HBOOST_ASIO_HAS_SIGACTION)

// Can use signal().
#if !defined(HBOOST_ASIO_HAS_SIGNAL)
# if !defined(HBOOST_ASIO_DISABLE_SIGNAL)
#  if !defined(UNDER_CE)
#   define HBOOST_ASIO_HAS_SIGNAL 1
#  endif // !defined(UNDER_CE)
# endif // !defined(HBOOST_ASIO_DISABLE_SIGNAL)
#endif // !defined(HBOOST_ASIO_HAS_SIGNAL)

// Can use getaddrinfo() and getnameinfo().
#if !defined(HBOOST_ASIO_HAS_GETADDRINFO)
# if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
#  if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0501)
#   define HBOOST_ASIO_HAS_GETADDRINFO 1
#  elif defined(UNDER_CE)
#   define HBOOST_ASIO_HAS_GETADDRINFO 1
#  endif // defined(UNDER_CE)
# elif !(defined(__MACH__) && defined(__APPLE__))
#  define HBOOST_ASIO_HAS_GETADDRINFO 1
# endif // !(defined(__MACH__) && defined(__APPLE__))
#endif // !defined(HBOOST_ASIO_HAS_GETADDRINFO)

// Whether standard iostreams are disabled.
#if !defined(HBOOST_ASIO_NO_IOSTREAM)
# if defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_NO_IOSTREAM)
#  define HBOOST_ASIO_NO_IOSTREAM 1
# endif // !defined(HBOOST_NO_IOSTREAM)
#endif // !defined(HBOOST_ASIO_NO_IOSTREAM)

// Whether exception handling is disabled.
#if !defined(HBOOST_ASIO_NO_EXCEPTIONS)
# if defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_NO_EXCEPTIONS)
#  define HBOOST_ASIO_NO_EXCEPTIONS 1
# endif // !defined(HBOOST_NO_EXCEPTIONS)
#endif // !defined(HBOOST_ASIO_NO_EXCEPTIONS)

// Whether the typeid operator is supported.
#if !defined(HBOOST_ASIO_NO_TYPEID)
# if defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_NO_TYPEID)
#  define HBOOST_ASIO_NO_TYPEID 1
# endif // !defined(HBOOST_NO_TYPEID)
#endif // !defined(HBOOST_ASIO_NO_TYPEID)

// Threads.
#if !defined(HBOOST_ASIO_HAS_THREADS)
# if !defined(HBOOST_ASIO_DISABLE_THREADS)
#  if defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_HAS_THREADS)
#   define HBOOST_ASIO_HAS_THREADS 1
#  elif defined(_MSC_VER) && defined(_MT)
#   define HBOOST_ASIO_HAS_THREADS 1
#  elif defined(__BORLANDC__) && defined(__MT__)
#   define HBOOST_ASIO_HAS_THREADS 1
#  elif defined(_POSIX_THREADS)
#   define HBOOST_ASIO_HAS_THREADS 1
#  endif // defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_HAS_THREADS)
# endif // !defined(HBOOST_ASIO_DISABLE_THREADS)
#endif // !defined(HBOOST_ASIO_HAS_THREADS)

// POSIX threads.
#if !defined(HBOOST_ASIO_HAS_PTHREADS)
# if defined(HBOOST_ASIO_HAS_THREADS)
#  if defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_HAS_PTHREADS)
#   define HBOOST_ASIO_HAS_PTHREADS 1
#  elif defined(_POSIX_THREADS)
#   define HBOOST_ASIO_HAS_PTHREADS 1
#  endif // defined(HBOOST_ASIO_HAS_HBOOST_CONFIG) && defined(HBOOST_HAS_PTHREADS)
# endif // defined(HBOOST_ASIO_HAS_THREADS)
#endif // !defined(HBOOST_ASIO_HAS_PTHREADS)

// Helper to prevent macro expansion.
#define HBOOST_ASIO_PREVENT_MACRO_SUBSTITUTION

// Helper to define in-class constants.
#if !defined(HBOOST_ASIO_STATIC_CONSTANT)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_STATIC_CONSTANT)
#  define HBOOST_ASIO_STATIC_CONSTANT(type, assignment) \
    HBOOST_STATIC_CONSTANT(type, assignment)
# else // !defined(HBOOST_ASIO_DISABLE_HBOOST_STATIC_CONSTANT)
#  define HBOOST_ASIO_STATIC_CONSTANT(type, assignment) \
    static const type assignment
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_STATIC_CONSTANT)
#endif // !defined(HBOOST_ASIO_STATIC_CONSTANT)

// Boost array library.
#if !defined(HBOOST_ASIO_HAS_HBOOST_ARRAY)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_ARRAY)
#  define HBOOST_ASIO_HAS_HBOOST_ARRAY 1
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_ARRAY)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_ARRAY)

// Boost assert macro.
#if !defined(HBOOST_ASIO_HAS_HBOOST_ASSERT)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_ASSERT)
#  define HBOOST_ASIO_HAS_HBOOST_ASSERT 1
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_ASSERT)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_ASSERT)

// Boost limits header.
#if !defined(HBOOST_ASIO_HAS_HBOOST_LIMITS)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_LIMITS)
#  define HBOOST_ASIO_HAS_HBOOST_LIMITS 1
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_LIMITS)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_LIMITS)

// Boost throw_exception function.
#if !defined(HBOOST_ASIO_HAS_HBOOST_THROW_EXCEPTION)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_THROW_EXCEPTION)
#  define HBOOST_ASIO_HAS_HBOOST_THROW_EXCEPTION 1
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_THROW_EXCEPTION)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_THROW_EXCEPTION)

// Boost regex library.
#if !defined(HBOOST_ASIO_HAS_HBOOST_REGEX)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_REGEX)
#  define HBOOST_ASIO_HAS_HBOOST_REGEX 1
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_REGEX)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_REGEX)

// Boost bind function.
#if !defined(HBOOST_ASIO_HAS_HBOOST_BIND)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_BIND)
#  define HBOOST_ASIO_HAS_HBOOST_BIND 1
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_BIND)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_BIND)

// Boost's HBOOST_WORKAROUND macro.
#if !defined(HBOOST_ASIO_HAS_HBOOST_WORKAROUND)
# if !defined(HBOOST_ASIO_DISABLE_HBOOST_WORKAROUND)
#  define HBOOST_ASIO_HAS_HBOOST_WORKAROUND 1
# endif // !defined(HBOOST_ASIO_DISABLE_HBOOST_WORKAROUND)
#endif // !defined(HBOOST_ASIO_HAS_HBOOST_WORKAROUND)

// Microsoft Visual C++'s secure C runtime library.
#if !defined(HBOOST_ASIO_HAS_SECURE_RTL)
# if !defined(HBOOST_ASIO_DISABLE_SECURE_RTL)
#  if defined(HBOOST_ASIO_MSVC) \
    && (HBOOST_ASIO_MSVC >= 1400) \
    && !defined(UNDER_CE)
#   define HBOOST_ASIO_HAS_SECURE_RTL 1
#  endif // defined(HBOOST_ASIO_MSVC)
         // && (HBOOST_ASIO_MSVC >= 1400)
         // && !defined(UNDER_CE)
# endif // !defined(HBOOST_ASIO_DISABLE_SECURE_RTL)
#endif // !defined(HBOOST_ASIO_HAS_SECURE_RTL)

// Handler hooking. Disabled for ancient Borland C++ and gcc compilers.
#if !defined(HBOOST_ASIO_HAS_HANDLER_HOOKS)
# if !defined(HBOOST_ASIO_DISABLE_HANDLER_HOOKS)
#  if defined(__GNUC__)
#   if (__GNUC__ >= 3)
#    define HBOOST_ASIO_HAS_HANDLER_HOOKS 1
#   endif // (__GNUC__ >= 3)
#  elif !defined(__BORLANDC__)
#   define HBOOST_ASIO_HAS_HANDLER_HOOKS 1
#  endif // !defined(__BORLANDC__)
# endif // !defined(HBOOST_ASIO_DISABLE_HANDLER_HOOKS)
#endif // !defined(HBOOST_ASIO_HAS_HANDLER_HOOKS)

// Support for the __thread keyword extension.
#if !defined(HBOOST_ASIO_DISABLE_THREAD_KEYWORD_EXTENSION)
# if defined(__linux__)
#  if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#   if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 3)
#    if !defined(__INTEL_COMPILER) && !defined(__ICL)
#     define HBOOST_ASIO_HAS_THREAD_KEYWORD_EXTENSION 1
#     define HBOOST_ASIO_THREAD_KEYWORD __thread
#    elif defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1100)
#     define HBOOST_ASIO_HAS_THREAD_KEYWORD_EXTENSION 1
#    endif // defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1100)
#   endif // ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 3)
#  endif // defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
# endif // defined(__linux__)
# if defined(HBOOST_ASIO_MSVC) && defined(HBOOST_ASIO_WINDOWS_RUNTIME)
#  if (_MSC_VER >= 1700)
#   define HBOOST_ASIO_HAS_THREAD_KEYWORD_EXTENSION 1
#   define HBOOST_ASIO_THREAD_KEYWORD __declspec(thread)
#  endif // (_MSC_VER >= 1700)
# endif // defined(HBOOST_ASIO_MSVC) && defined(HBOOST_ASIO_WINDOWS_RUNTIME)
#endif // !defined(HBOOST_ASIO_DISABLE_THREAD_KEYWORD_EXTENSION)
#if !defined(HBOOST_ASIO_THREAD_KEYWORD)
# define HBOOST_ASIO_THREAD_KEYWORD __thread
#endif // !defined(HBOOST_ASIO_THREAD_KEYWORD)

// Support for POSIX ssize_t typedef.
#if !defined(HBOOST_ASIO_DISABLE_SSIZE_T)
# if defined(__linux__) \
   || (defined(__MACH__) && defined(__APPLE__))
#  define HBOOST_ASIO_HAS_SSIZE_T 1
# endif // defined(__linux__)
        //   || (defined(__MACH__) && defined(__APPLE__))
#endif // !defined(HBOOST_ASIO_DISABLE_SSIZE_T)

#endif // HBOOST_ASIO_DETAIL_CONFIG_HPP
