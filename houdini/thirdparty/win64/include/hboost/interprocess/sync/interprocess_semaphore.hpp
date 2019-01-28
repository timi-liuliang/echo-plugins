//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_SEMAPHORE_HPP
#define HBOOST_INTERPROCESS_SEMAPHORE_HPP

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

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
#include <hboost/interprocess/detail/posix_time_types_wrk.hpp>

#if !defined(HBOOST_INTERPROCESS_FORCE_GENERIC_EMULATION) && \
   (defined(HBOOST_INTERPROCESS_POSIX_PROCESS_SHARED) && defined(HBOOST_INTERPROCESS_POSIX_UNNAMED_SEMAPHORES))
   #include <hboost/interprocess/sync/posix/semaphore.hpp>
   #define HBOOST_INTERPROCESS_USE_POSIX
//Experimental...
#elif !defined(HBOOST_INTERPROCESS_FORCE_GENERIC_EMULATION) && defined (HBOOST_INTERPROCESS_WINDOWS)
   #include <hboost/interprocess/sync/windows/semaphore.hpp>
   #define HBOOST_INTERPROCESS_USE_WINDOWS
#elif !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   #include <hboost/interprocess/sync/spin/semaphore.hpp>
   #define HBOOST_INTERPROCESS_USE_GENERIC_EMULATION
#endif

#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

//!\file
//!Describes a interprocess_semaphore class for inter-process synchronization

namespace hboost {
namespace interprocess {

//!Wraps a interprocess_semaphore that can be placed in shared memory and can be
//!shared between processes. Allows timed lock tries
class interprocess_semaphore
{
   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   //Non-copyable
   interprocess_semaphore(const interprocess_semaphore &);
   interprocess_semaphore &operator=(const interprocess_semaphore &);
   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
   public:
   //!Creates a interprocess_semaphore with the given initial count.
   //!interprocess_exception if there is an error.*/
   interprocess_semaphore(unsigned int initialCount);

   //!Destroys the interprocess_semaphore.
   //!Does not throw
   ~interprocess_semaphore();

   //!Increments the interprocess_semaphore count. If there are processes/threads blocked waiting
   //!for the interprocess_semaphore, then one of these processes will return successfully from
   //!its wait function. If there is an error an interprocess_exception exception is thrown.
   void post();

   //!Decrements the interprocess_semaphore. If the interprocess_semaphore value is not greater than zero,
   //!then the calling process/thread blocks until it can decrement the counter.
   //!If there is an error an interprocess_exception exception is thrown.
   void wait();

   //!Decrements the interprocess_semaphore if the interprocess_semaphore's value is greater than zero
   //!and returns true. If the value is not greater than zero returns false.
   //!If there is an error an interprocess_exception exception is thrown.
   bool try_wait();

   //!Decrements the interprocess_semaphore if the interprocess_semaphore's value is greater
   //!than zero and returns true. Otherwise, waits for the interprocess_semaphore
   //!to the posted or the timeout expires. If the timeout expires, the
   //!function returns false. If the interprocess_semaphore is posted the function
   //!returns true. If there is an error throws sem_exception
   bool timed_wait(const hboost::posix_time::ptime &abs_time);

   //!Returns the interprocess_semaphore count
//   int get_count() const;
   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   private:
   #if defined(HBOOST_INTERPROCESS_USE_GENERIC_EMULATION)
      #undef HBOOST_INTERPROCESS_USE_GENERIC_EMULATION
      ipcdetail::spin_semaphore m_sem;
   #elif defined(HBOOST_INTERPROCESS_USE_WINDOWS)
      #undef HBOOST_INTERPROCESS_USE_WINDOWS
      ipcdetail::windows_semaphore m_sem;
   #else
      #undef HBOOST_INTERPROCESS_USE_POSIX
      ipcdetail::posix_semaphore m_sem;
   #endif   //#if defined(HBOOST_INTERPROCESS_USE_GENERIC_EMULATION)
   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
};

}  //namespace interprocess {
}  //namespace hboost {

namespace hboost {
namespace interprocess {

inline interprocess_semaphore::interprocess_semaphore(unsigned int initialCount)
   : m_sem(initialCount)
{}

inline interprocess_semaphore::~interprocess_semaphore(){}

inline void interprocess_semaphore::wait()
{
   #ifdef HBOOST_INTERPROCESS_ENABLE_TIMEOUT_WHEN_LOCKING
      hboost::posix_time::ptime wait_time
         = microsec_clock::universal_time()
         + hboost::posix_time::milliseconds(HBOOST_INTERPROCESS_TIMEOUT_WHEN_LOCKING_DURATION_MS);
      if (!m_sem.timed_wait(wait_time))
      {
         throw interprocess_exception(timeout_when_waiting_error, "Interprocess semaphore timeout when waiting. Possible deadlock: owner died without posting?");
      }
   #else
      m_sem.wait();
   #endif
}
inline bool interprocess_semaphore::try_wait()
{ return m_sem.try_wait(); }

inline bool interprocess_semaphore::timed_wait(const hboost::posix_time::ptime &abs_time)
{ return m_sem.timed_wait(abs_time); }

inline void interprocess_semaphore::post()
{ m_sem.post(); }

}  //namespace interprocess {
}  //namespace hboost {

#include <hboost/interprocess/detail/config_end.hpp>

#endif   //HBOOST_INTERPROCESS_SEMAPHORE_HPP
