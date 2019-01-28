 //////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_SHM_NAMED_SEMAPHORE_HPP
#define HBOOST_INTERPROCESS_SHM_NAMED_SEMAPHORE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/interprocess/detail/workaround.hpp>
#include <hboost/interprocess/creation_tags.hpp>
#include <hboost/interprocess/exceptions.hpp>
#include <hboost/interprocess/permissions.hpp>
#include <hboost/interprocess/detail/interprocess_tester.hpp>
#include <hboost/interprocess/detail/posix_time_types_wrk.hpp>
#include <hboost/interprocess/shared_memory_object.hpp>
#include <hboost/interprocess/detail/managed_open_or_create_impl.hpp>
#include <hboost/interprocess/sync/interprocess_semaphore.hpp>
#include <hboost/interprocess/sync/shm/named_creation_functor.hpp>

namespace hboost {
namespace interprocess {
namespace ipcdetail {

class shm_named_semaphore
{
   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

   //Non-copyable
   shm_named_semaphore();
   shm_named_semaphore(const shm_named_semaphore &);
   shm_named_semaphore &operator=(const shm_named_semaphore &);
   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

   public:
   shm_named_semaphore(create_only_t, const char *name, unsigned int initialCount, const permissions &perm = permissions());

   shm_named_semaphore(open_or_create_t, const char *name, unsigned int initialCount, const permissions &perm = permissions());

   shm_named_semaphore(open_only_t, const char *name);

   ~shm_named_semaphore();

   void post();
   void wait();
   bool try_wait();
   bool timed_wait(const hboost::posix_time::ptime &abs_time);

   static bool remove(const char *name);

   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   private:
   friend class interprocess_tester;
   void dont_close_on_destruction();

   interprocess_semaphore *semaphore() const
   {  return static_cast<interprocess_semaphore*>(m_shmem.get_user_address()); }

   typedef ipcdetail::managed_open_or_create_impl<shared_memory_object, 0, true, false> open_create_impl_t;
   open_create_impl_t m_shmem;
   typedef named_creation_functor<interprocess_semaphore, int> construct_func_t;
   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
};

inline shm_named_semaphore::~shm_named_semaphore()
{}

inline void shm_named_semaphore::dont_close_on_destruction()
{  interprocess_tester::dont_close_on_destruction(m_shmem);  }

inline shm_named_semaphore::shm_named_semaphore
   (create_only_t, const char *name, unsigned int initialCount, const permissions &perm)
   :  m_shmem  (create_only
               ,name
               ,sizeof(interprocess_semaphore) +
                  open_create_impl_t::ManagedOpenOrCreateUserOffset
               ,read_write
               ,0
               ,construct_func_t(DoCreate, initialCount)
               ,perm)
{}

inline shm_named_semaphore::shm_named_semaphore
   (open_or_create_t, const char *name, unsigned int initialCount, const permissions &perm)
   :  m_shmem  (open_or_create
               ,name
               ,sizeof(interprocess_semaphore) +
                  open_create_impl_t::ManagedOpenOrCreateUserOffset
               ,read_write
               ,0
               ,construct_func_t(DoOpenOrCreate, initialCount)
               ,perm)
{}

inline shm_named_semaphore::shm_named_semaphore
   (open_only_t, const char *name)
   :  m_shmem  (open_only
               ,name
               ,read_write
               ,0
               ,construct_func_t(DoOpen, 0))
{}

inline void shm_named_semaphore::post()
{  semaphore()->post();   }

inline void shm_named_semaphore::wait()
{  semaphore()->wait();   }

inline bool shm_named_semaphore::try_wait()
{  return semaphore()->try_wait();   }

inline bool shm_named_semaphore::timed_wait(const hboost::posix_time::ptime &abs_time)
{  return semaphore()->timed_wait(abs_time); }

inline bool shm_named_semaphore::remove(const char *name)
{  return shared_memory_object::remove(name); }

}  //namespace ipcdetail {
}  //namespace interprocess {
}  //namespace hboost {

#include <hboost/interprocess/detail/config_end.hpp>

#endif   //HBOOST_INTERPROCESS_SHM_NAMED_SEMAPHORE_HPP
