//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_ALLOCATOR_SCOPED_ALLOCATOR_FWD_HPP
#define HBOOST_CONTAINER_ALLOCATOR_SCOPED_ALLOCATOR_FWD_HPP

//! \file
//! This header file forward declares hboost::container::scoped_allocator_adaptor

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>
#include <hboost/container/detail/std_fwd.hpp>
#include <hboost/container/uses_allocator_fwd.hpp>

#if defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#include <hboost/move/detail/fwd_macros.hpp>
#endif

namespace hboost { namespace container {

#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   #if !defined(HBOOST_CONTAINER_UNIMPLEMENTED_PACK_EXPANSION_TO_FIXED_LIST)

      template <typename OuterAlloc, typename ...InnerAllocs>
      class scoped_allocator_adaptor;

   #else // #if !defined(HBOOST_CONTAINER_UNIMPLEMENTED_PACK_EXPANSION_TO_FIXED_LIST)

      template <typename ...InnerAllocs>
      class scoped_allocator_adaptor;

      template <typename OuterAlloc, typename ...InnerAllocs>
      class scoped_allocator_adaptor<OuterAlloc, InnerAllocs...>;

   #endif   // #if !defined(HBOOST_CONTAINER_UNIMPLEMENTED_PACK_EXPANSION_TO_FIXED_LIST)

#else    // #if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   template <typename OuterAlloc, HBOOST_MOVE_CLASSDFLT9>
   class scoped_allocator_adaptor;

#endif


#else    //HBOOST_CONTAINER_DOXYGEN_INVOKED

#endif   //#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED

}} // namespace hboost { namespace container {

#include <hboost/container/detail/config_end.hpp>

#endif //  HBOOST_CONTAINER_ALLOCATOR_SCOPED_ALLOCATOR_FWD_HPP
