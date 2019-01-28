//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_DETAIL_POOL_COMMON_ALLOC_HPP
#define HBOOST_CONTAINER_DETAIL_POOL_COMMON_ALLOC_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>
#include <hboost/container/throw_exception.hpp>

#include <hboost/intrusive/slist.hpp>
#include <hboost/container/detail/pool_common.hpp>
#include <hboost/container/detail/dlmalloc.hpp>
#include <cstddef>

namespace hboost{
namespace container{
namespace container_detail{

struct node_slist_helper
   : public hboost::container::container_detail::node_slist<void*>
{};

struct fake_segment_manager
{
   typedef void * void_pointer;
   static const std::size_t PayloadPerAllocation = HBOOST_CONTAINER_ALLOCATION_PAYLOAD;

   typedef hboost::container::container_detail::
      basic_multiallocation_chain<void*>              multiallocation_chain;
   static void deallocate(void_pointer p)
   { dlmalloc_free(p); }

   static void deallocate_many(multiallocation_chain &chain)
   {
      std::size_t size = chain.size();
      std::pair<void*, void*> ptrs = chain.extract_data();
      dlmalloc_memchain dlchain;
      HBOOST_CONTAINER_MEMCHAIN_INIT_FROM(&dlchain, ptrs.first, ptrs.second, size);
      dlmalloc_multidealloc(&dlchain);
   }

   typedef std::ptrdiff_t  difference_type;
   typedef std::size_t     size_type;

   static void *allocate_aligned(std::size_t nbytes, std::size_t alignment)
   {
      void *ret = dlmalloc_memalign(nbytes, alignment);
      if(!ret)
         hboost::container::throw_bad_alloc();
      return ret;
   }

   static void *allocate(std::size_t nbytes)
   {
      void *ret = dlmalloc_malloc(nbytes);
      if(!ret)
         hboost::container::throw_bad_alloc();
      return ret;
   }
};

}  //namespace hboost{
}  //namespace container{
}  //namespace container_detail{

namespace hboost {
namespace container {
namespace container_detail {

template<class T>
struct is_stateless_segment_manager;

template<>
struct is_stateless_segment_manager
   <hboost::container::container_detail::fake_segment_manager>
{
   static const bool value = true;
};

}  //namespace container_detail {
}  //namespace container {
}  //namespace hboost {

#include <hboost/container/detail/config_end.hpp>

#endif   //HBOOST_CONTAINER_DETAIL_POOL_COMMON_ALLOC_HPP
