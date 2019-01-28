//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2014. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_MOVE_DETAIL_WORKAROUND_HPP
#define HBOOST_MOVE_DETAIL_WORKAROUND_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if    !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
   #define HBOOST_MOVE_PERFECT_FORWARDING
#endif

#if defined(__has_feature)
   #define HBOOST_MOVE_HAS_FEATURE __has_feature
#else
   #define HBOOST_MOVE_HAS_FEATURE(x) 0
#endif

#if HBOOST_MOVE_HAS_FEATURE(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
   #define HBOOST_MOVE_ADDRESS_SANITIZER_ON
#endif

//Macros for documentation purposes. For code, expands to the argument
#define HBOOST_MOVE_IMPDEF(TYPE) TYPE
#define HBOOST_MOVE_SEEDOC(TYPE) TYPE
#define HBOOST_MOVE_DOC0PTR(TYPE) TYPE
#define HBOOST_MOVE_DOC1ST(TYPE1, TYPE2) TYPE2
#define HBOOST_MOVE_I ,
#define HBOOST_MOVE_DOCIGN(T1) T1

#if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ < 5) && !defined(__clang__)
   //Pre-standard rvalue binding rules
   #define HBOOST_MOVE_OLD_RVALUE_REF_BINDING_RULES
#elif defined(_MSC_VER) && (_MSC_VER == 1600)
   //Standard rvalue binding rules but with some bugs
   #define HBOOST_MOVE_MSVC_10_MEMBER_RVALUE_REF_BUG
   #define HBOOST_MOVE_MSVC_AUTO_MOVE_RETURN_BUG
#elif defined(_MSC_VER) && (_MSC_VER == 1700)
   #define HBOOST_MOVE_MSVC_AUTO_MOVE_RETURN_BUG
#endif

#define HBOOST_MOVE_DISABLE_FORCEINLINE

#if defined(HBOOST_MOVE_DISABLE_FORCEINLINE)
   #define HBOOST_MOVE_FORCEINLINE inline
#elif defined(HBOOST_MOVE_FORCEINLINE_IS_HBOOST_FORCELINE)
   #define HBOOST_MOVE_FORCEINLINE HBOOST_FORCEINLINE
#elif defined(HBOOST_MSVC) && defined(_DEBUG)
   //"__forceinline" and MSVC seems to have some bugs in debug mode
   #define HBOOST_MOVE_FORCEINLINE inline
#else
   #define HBOOST_MOVE_FORCEINLINE HBOOST_FORCEINLINE
#endif

#endif   //#ifndef HBOOST_MOVE_DETAIL_WORKAROUND_HPP
