 //////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2011-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_INTERPROCESS_WINDOWS_NAMED_MUTEX_HPP
#define HBOOST_INTERPROCESS_WINDOWS_NAMED_MUTEX_HPP

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
#include <hboost/interprocess/permissions.hpp>
#include <hboost/interprocess/detail/interprocess_tester.hpp>
#include <hboost/interprocess/detail/posix_time_types_wrk.hpp>
#include <hboost/interprocess/sync/windows/sync_utils.hpp>
#include <hboost/interprocess/sync/windows/named_sync.hpp>
#include <hboost/interprocess/sync/windows/winapi_mutex_wrapper.hpp>
#include <hboost/interprocess/errors.hpp>
#include <hboost/interprocess/exceptions.hpp>
#include <limits>

namespace hboost {
namespace interprocess {
namespace ipcdetail {



class windows_named_mutex
{
   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

   //Non-copyable
   windows_named_mutex();
   windows_named_mutex(const windows_named_mutex &);
   windows_named_mutex &operator=(const windows_named_mutex &);
   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

   public:
   windows_named_mutex(create_only_t, const char *name, const permissions &perm = permissions());

   windows_named_mutex(open_or_create_t, const char *name, const permissions &perm = permissions());

   windows_named_mutex(open_only_t, const char *name);

   ~windows_named_mutex();

   void unlock();
   void lock();
   bool try_lock();
   bool timed_lock(const hboost::posix_time::ptime &abs_time);

   static bool remove(const char *name);

   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   private:
   friend class interprocess_tester;
   void dont_close_on_destruction();
   winapi_mutex_wrapper m_mtx_wrapper;
   windows_named_sync m_named_sync;

   class named_mut_callbacks : public windows_named_sync_interface
   {
      public:
      named_mut_callbacks(winapi_mutex_wrapper &mtx_wrapper)
         : m_mtx_wrapper(mtx_wrapper)
      {}

      virtual std::size_t get_data_size() const
      {  return 0u;   }

      virtual const void *buffer_with_init_data_to_file()
      {  return 0; }

      virtual const void *buffer_with_final_data_to_file()
      {  return 0; }

      virtual void *buffer_to_store_init_data_from_file()
      {  return 0; }

      virtual bool open(create_enum_t, const char *id_name)
      {
         std::string aux_str  = "Global\\bipc.mut.";
         aux_str += id_name;
         //
         permissions mut_perm;
         mut_perm.set_unrestricted();
         return m_mtx_wrapper.open_or_create(aux_str.c_str(), mut_perm);
      }

      virtual void close()
      {
         m_mtx_wrapper.close();
      }

      virtual ~named_mut_callbacks()
      {}

      private:
      winapi_mutex_wrapper&     m_mtx_wrapper;
   };
   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
};

inline windows_named_mutex::~windows_named_mutex()
{
   named_mut_callbacks callbacks(m_mtx_wrapper);
   m_named_sync.close(callbacks);
}

inline void windows_named_mutex::dont_close_on_destruction()
{}

inline windows_named_mutex::windows_named_mutex
   (create_only_t, const char *name, const permissions &perm)
   : m_mtx_wrapper()
{
   named_mut_callbacks callbacks(m_mtx_wrapper);
   m_named_sync.open_or_create(DoCreate, name, perm, callbacks);
}

inline windows_named_mutex::windows_named_mutex
   (open_or_create_t, const char *name, const permissions &perm)
   : m_mtx_wrapper()
{
   named_mut_callbacks callbacks(m_mtx_wrapper);
   m_named_sync.open_or_create(DoOpenOrCreate, name, perm, callbacks);
}

inline windows_named_mutex::windows_named_mutex(open_only_t, const char *name)
   : m_mtx_wrapper()
{
   named_mut_callbacks callbacks(m_mtx_wrapper);
   m_named_sync.open_or_create(DoOpen, name, permissions(), callbacks);
}

inline void windows_named_mutex::unlock()
{
   m_mtx_wrapper.unlock();
}

inline void windows_named_mutex::lock()
{
   m_mtx_wrapper.lock();
}

inline bool windows_named_mutex::try_lock()
{
   return m_mtx_wrapper.try_lock();
}

inline bool windows_named_mutex::timed_lock(const hboost::posix_time::ptime &abs_time)
{
   return m_mtx_wrapper.timed_lock(abs_time);
}

inline bool windows_named_mutex::remove(const char *name)
{
   return windows_named_sync::remove(name);
}

}  //namespace ipcdetail {
}  //namespace interprocess {
}  //namespace hboost {

#include <hboost/interprocess/detail/config_end.hpp>

#endif   //HBOOST_INTERPROCESS_WINDOWS_NAMED_MUTEX_HPP