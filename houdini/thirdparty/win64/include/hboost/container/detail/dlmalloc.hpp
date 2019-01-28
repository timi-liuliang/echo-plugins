//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_CONTAINER_DETAIL_ALLOC_LIB_HPP
#define HBOOST_CONTAINER_DETAIL_ALLOC_LIB_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>
#include <hboost/container/detail/auto_link.hpp>

#include <hboost/container/detail/alloc_lib.h>

namespace hboost{
namespace container{

typedef hboost_cont_command_ret_t dlmalloc_command_ret_t;
typedef hboost_cont_memchain dlmalloc_memchain;
typedef hboost_cont_memchain_it dlmalloc_memchain_it;
typedef hboost_cont_malloc_stats_t dlmalloc_malloc_stats_t;

HBOOST_CONTAINER_DECL size_t dlmalloc_size(const void *p);

HBOOST_CONTAINER_DECL void* dlmalloc_malloc(size_t bytes);

HBOOST_CONTAINER_DECL void  dlmalloc_free(void* mem);

HBOOST_CONTAINER_DECL void* dlmalloc_memalign(size_t bytes, size_t alignment);

HBOOST_CONTAINER_DECL int dlmalloc_multialloc_nodes
   (size_t n_elements, size_t elem_size, size_t contiguous_elements, hboost_cont_memchain *pchain);

HBOOST_CONTAINER_DECL int dlmalloc_multialloc_arrays
   (size_t n_elements, const size_t *sizes, size_t sizeof_element, size_t contiguous_elements, hboost_cont_memchain *pchain);

HBOOST_CONTAINER_DECL void dlmalloc_multidealloc(hboost_cont_memchain *pchain);

HBOOST_CONTAINER_DECL size_t dlmalloc_footprint();

HBOOST_CONTAINER_DECL size_t dlmalloc_allocated_memory();

HBOOST_CONTAINER_DECL size_t dlmalloc_chunksize(const void *p);

HBOOST_CONTAINER_DECL int dlmalloc_all_deallocated();

HBOOST_CONTAINER_DECL hboost_cont_malloc_stats_t dlmalloc_malloc_stats();

HBOOST_CONTAINER_DECL size_t dlmalloc_in_use_memory();

HBOOST_CONTAINER_DECL int dlmalloc_trim(size_t pad);

HBOOST_CONTAINER_DECL int dlmalloc_mallopt(int parameter_number, int parameter_value);

HBOOST_CONTAINER_DECL int dlmalloc_grow(void* oldmem, size_t minbytes, size_t maxbytes, size_t *received);

HBOOST_CONTAINER_DECL int dlmalloc_shrink(void* oldmem, size_t minbytes, size_t maxbytes, size_t *received, int do_commit);

HBOOST_CONTAINER_DECL void* dlmalloc_alloc(size_t minbytes, size_t preferred_bytes, size_t *received_bytes);

HBOOST_CONTAINER_DECL int dlmalloc_malloc_check();

HBOOST_CONTAINER_DECL hboost_cont_command_ret_t dlmalloc_allocation_command
   ( allocation_type command
   , size_t sizeof_object
   , size_t limit_objects
   , size_t preferred_objects
   , size_t *received_objects
   , void *reuse_ptr
   );

HBOOST_CONTAINER_DECL int dlmalloc_mallopt(int param_number, int value);

HBOOST_CONTAINER_DECL void *dlmalloc_sync_create();

HBOOST_CONTAINER_DECL void dlmalloc_sync_destroy(void *sync);

HBOOST_CONTAINER_DECL bool dlmalloc_sync_lock(void *sync);

HBOOST_CONTAINER_DECL void dlmalloc_sync_unlock(void *sync);

HBOOST_CONTAINER_DECL bool dlmalloc_global_sync_lock();

HBOOST_CONTAINER_DECL void dlmalloc_global_sync_unlock();

}  //namespace container{
}  //namespace hboost{

#include <hboost/container/detail/config_end.hpp>

#endif   //HBOOST_CONTAINER_DETAIL_ALLOC_LIB_HPP
