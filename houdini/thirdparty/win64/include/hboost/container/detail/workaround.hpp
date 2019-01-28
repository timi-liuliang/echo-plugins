//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_DETAIL_WORKAROUND_HPP
#define HBOOST_CONTAINER_DETAIL_WORKAROUND_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if    !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)\
    && !defined(HBOOST_INTERPROCESS_DISABLE_VARIADIC_TMPL)
   #define HBOOST_CONTAINER_PERFECT_FORWARDING
#endif

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && defined(__GXX_EXPERIMENTAL_CXX0X__)\
    && (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__ < 40700)
   #define HBOOST_CONTAINER_UNIMPLEMENTED_PACK_EXPANSION_TO_FIXED_LIST
#endif

#if !defined(HBOOST_FALLTHOUGH)
   #define HBOOST_CONTAINER_FALLTHOUGH
#else
   #define HBOOST_CONTAINER_FALLTHOUGH HBOOST_FALLTHOUGH;
#endif

//Macros for documentation purposes. For code, expands to the argument
#define HBOOST_CONTAINER_IMPDEF(TYPE) TYPE
#define HBOOST_CONTAINER_SEEDOC(TYPE) TYPE

//Macros for memset optimization. In most platforms
//memsetting pointers and floatings is safe and faster.
//
//If your platform does not offer these guarantees
//define these to value zero.
#ifndef HBOOST_CONTAINER_MEMZEROED_FLOATING_POINT_IS_NOT_ZERO
#define HBOOST_CONTAINER_MEMZEROED_FLOATING_POINT_IS_ZERO 1
#endif

#ifndef HBOOST_CONTAINER_MEMZEROED_POINTER_IS_NOT_NULL
#define HBOOST_CONTAINER_MEMZEROED_POINTER_IS_NULL
#endif

#define HBOOST_CONTAINER_DOC1ST(TYPE1, TYPE2) TYPE2
#define HBOOST_CONTAINER_I ,
#define HBOOST_CONTAINER_DOCIGN(T) T
#define HBOOST_CONTAINER_DOCONLY(T)

/*
   we need to import/export our code only if the user has specifically
   asked for it by defining either HBOOST_ALL_DYN_LINK if they want all boost
   libraries to be dynamically linked, or HBOOST_CONTAINER_DYN_LINK
   if they want just this one to be dynamically liked:
*/
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_CONTAINER_DYN_LINK)

   /* export if this is our own source, otherwise import: */
   #ifdef HBOOST_CONTAINER_SOURCE
   #  define HBOOST_CONTAINER_DECL HBOOST_SYMBOL_EXPORT
   #else
   #  define HBOOST_CONTAINER_DECL HBOOST_SYMBOL_IMPORT
   
   #endif  /* HBOOST_CONTAINER_SOURCE */
#else
   #define HBOOST_CONTAINER_DECL
#endif  /* DYN_LINK */

//#define HBOOST_CONTAINER_DISABLE_FORCEINLINE

#if defined(HBOOST_CONTAINER_DISABLE_FORCEINLINE)
   #define HBOOST_CONTAINER_FORCEINLINE inline
#elif defined(HBOOST_CONTAINER_FORCEINLINE_IS_HBOOST_FORCELINE)
   #define HBOOST_CONTAINER_FORCEINLINE HBOOST_FORCEINLINE
#elif defined(HBOOST_MSVC) && defined(_DEBUG)
   //"__forceinline" and MSVC seems to have some bugs in debug mode
   #define HBOOST_CONTAINER_FORCEINLINE inline
#else
   #define HBOOST_CONTAINER_FORCEINLINE HBOOST_FORCEINLINE
#endif

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_WORKAROUND_HPP
