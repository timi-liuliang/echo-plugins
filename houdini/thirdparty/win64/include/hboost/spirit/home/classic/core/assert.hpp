/*=============================================================================
    Copyright (c) 2001-2003 Joel de Guzman
    Copyright (c) 2002-2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_ASSERT_HPP)
#define HBOOST_SPIRIT_ASSERT_HPP

#include <hboost/config.hpp>
#include <hboost/throw_exception.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  HBOOST_SPIRIT_ASSERT is used throughout the framework.  It can be
//  overridden by the user. If HBOOST_SPIRIT_ASSERT_EXCEPTION is defined,
//  then that will be thrown, otherwise, HBOOST_SPIRIT_ASSERT simply turns
//  into a plain HBOOST_ASSERT()
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_SPIRIT_ASSERT)
#if defined(NDEBUG)
    #define HBOOST_SPIRIT_ASSERT(x)
#elif defined (HBOOST_SPIRIT_ASSERT_EXCEPTION)
    #define HBOOST_SPIRIT_ASSERT_AUX(f, l, x) HBOOST_SPIRIT_ASSERT_AUX2(f, l, x)
    #define HBOOST_SPIRIT_ASSERT_AUX2(f, l, x)                                   \
    do{ if (!(x)) hboost::throw_exception(                                       \
        HBOOST_SPIRIT_ASSERT_EXCEPTION(f "(" #l "): " #x)); } while(0)
    #define HBOOST_SPIRIT_ASSERT(x) HBOOST_SPIRIT_ASSERT_AUX(__FILE__, __LINE__, x)
#else
    #include <hboost/assert.hpp>
    #define HBOOST_SPIRIT_ASSERT(x) HBOOST_ASSERT(x)
#endif
#endif // !defined(HBOOST_SPIRIT_ASSERT)

#endif // HBOOST_SPIRIT_ASSERT_HPP
