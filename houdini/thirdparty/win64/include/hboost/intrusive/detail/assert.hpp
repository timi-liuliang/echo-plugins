/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2006-2013
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_ASSERT_HPP
#define HBOOST_INTRUSIVE_DETAIL_ASSERT_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#pragma once
#endif

#if !defined(HBOOST_INTRUSIVE_INVARIANT_ASSERT)
   #include <hboost/assert.hpp>
   #define HBOOST_INTRUSIVE_INVARIANT_ASSERT HBOOST_ASSERT
#elif defined(HBOOST_INTRUSIVE_INVARIANT_ASSERT_INCLUDE)
   #include HBOOST_INTRUSIVE_INVARIANT_ASSERT_INCLUDE
#endif

#if !defined(HBOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT)
   #include <hboost/assert.hpp>
   #define HBOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT HBOOST_ASSERT
#elif defined(HBOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT_INCLUDE)
   #include HBOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT_INCLUDE
#endif

#if !defined(HBOOST_INTRUSIVE_SAFE_HOOK_DESTRUCTOR_ASSERT)
   #include <hboost/assert.hpp>
   #define HBOOST_INTRUSIVE_SAFE_HOOK_DESTRUCTOR_ASSERT HBOOST_ASSERT
#elif defined(HBOOST_INTRUSIVE_SAFE_HOOK_DESTRUCTOR_ASSERT_INCLUDE)
   #include HBOOST_INTRUSIVE_SAFE_HOOK_DESTRUCTOR_ASSERT_INCLUDE
#endif

#endif //HBOOST_INTRUSIVE_DETAIL_ASSERT_HPP
