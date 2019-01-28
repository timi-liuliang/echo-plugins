 /*
 * Copyright (c) 2002
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         mem_block_cache.hpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: memory block cache used by the non-recursive matcher.
  */

#ifndef HBOOST_REGEX_V4_MEM_BLOCK_CACHE_HPP
#define HBOOST_REGEX_V4_MEM_BLOCK_CACHE_HPP

#include <new>
#ifdef HBOOST_HAS_THREADS
#include <hboost/regex/pending/static_mutex.hpp>
#endif

#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_PREFIX
#endif

#ifndef HBOOST_NO_CXX11_HDR_ATOMIC
  #include <atomic>
  #if ATOMIC_POINTER_LOCK_FREE == 2
    #define HBOOST_REGEX_MEM_BLOCK_CACHE_LOCK_FREE
    #define HBOOST_REGEX_ATOMIC_POINTER std::atomic
  #endif
#endif

namespace hboost{
namespace HBOOST_REGEX_DETAIL_NS{

#ifdef HBOOST_REGEX_MEM_BLOCK_CACHE_LOCK_FREE /* lock free implementation */
struct mem_block_cache
{
  std::atomic<void*> cache[HBOOST_REGEX_MAX_CACHE_BLOCKS];

   ~mem_block_cache()
   {
     for (size_t i = 0;i < HBOOST_REGEX_MAX_CACHE_BLOCKS; ++i) {
       if (cache[i].load()) ::operator delete(cache[i].load());
     }
   }
   void* get()
   {
     for (size_t i = 0;i < HBOOST_REGEX_MAX_CACHE_BLOCKS; ++i) {
       void* p = cache[i].load();
       if (p != NULL) {
         if (cache[i].compare_exchange_strong(p, NULL)) return p;
       }
     }
     return ::operator new(HBOOST_REGEX_BLOCKSIZE);
   }
   void put(void* ptr)
   {
     for (size_t i = 0;i < HBOOST_REGEX_MAX_CACHE_BLOCKS; ++i) {
       void* p = cache[i].load();
       if (p == NULL) {
         if (cache[i].compare_exchange_strong(p, ptr)) return;
       }
     }
     ::operator delete(ptr);
   }
};


#else /* lock-based implementation */


struct mem_block_node
{
   mem_block_node* next;
};

struct mem_block_cache
{
   // this member has to be statically initialsed:
   mem_block_node* next;
   unsigned cached_blocks;
#ifdef HBOOST_HAS_THREADS
   hboost::static_mutex mut;
#endif

   ~mem_block_cache()
   {
      while(next)
      {
         mem_block_node* old = next;
         next = next->next;
         ::operator delete(old);
      }
   }
   void* get()
   {
#ifdef HBOOST_HAS_THREADS
      hboost::static_mutex::scoped_lock g(mut);
#endif
     if(next)
      {
         mem_block_node* result = next;
         next = next->next;
         --cached_blocks;
         return result;
      }
      return ::operator new(HBOOST_REGEX_BLOCKSIZE);
   }
   void put(void* p)
   {
#ifdef HBOOST_HAS_THREADS
      hboost::static_mutex::scoped_lock g(mut);
#endif
      if(cached_blocks >= HBOOST_REGEX_MAX_CACHE_BLOCKS)
      {
         ::operator delete(p);
      }
      else
      {
         mem_block_node* old = static_cast<mem_block_node*>(p);
         old->next = next;
         next = old;
         ++cached_blocks;
      }
   }
};
#endif

extern mem_block_cache block_cache;

}
} // namespace hboost

#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_SUFFIX
#endif

#endif

