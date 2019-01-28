//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_WORKAROUND_HPP
#define HBOOST_INTRUSIVE_DETAIL_WORKAROUND_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#ifndef HBOOST_CONFIG_HPP
#include <hboost/config.hpp>
#endif

#if    !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
   #define HBOOST_INTRUSIVE_PERFECT_FORWARDING
#endif

//Macros for documentation purposes. For code, expands to the argument
#define HBOOST_INTRUSIVE_IMPDEF(TYPE) TYPE
#define HBOOST_INTRUSIVE_SEEDOC(TYPE) TYPE
#define HBOOST_INTRUSIVE_DOC1ST(TYPE1, TYPE2) TYPE2
#define HBOOST_INTRUSIVE_I ,
#define HBOOST_INTRUSIVE_DOCIGN(T1) T1

#define HBOOST_INTRUSIVE_DISABLE_FORCEINLINE

#if defined(HBOOST_INTRUSIVE_DISABLE_FORCEINLINE)
   #define HBOOST_INTRUSIVE_FORCEINLINE inline
#elif defined(HBOOST_INTRUSIVE_FORCEINLINE_IS_HBOOST_FORCELINE)
   #define HBOOST_INTRUSIVE_FORCEINLINE HBOOST_FORCEINLINE
#elif defined(HBOOST_MSVC) && defined(_DEBUG)
   //"__forceinline" and MSVC seems to have some bugs in debug mode
   #define HBOOST_INTRUSIVE_FORCEINLINE inline
#else
   #define HBOOST_INTRUSIVE_FORCEINLINE HBOOST_FORCEINLINE
#endif

#endif   //#ifndef HBOOST_INTRUSIVE_DETAIL_WORKAROUND_HPP
