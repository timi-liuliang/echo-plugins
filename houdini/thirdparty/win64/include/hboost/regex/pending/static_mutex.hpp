/*
 *
 * Copyright (c) 2004
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
 
 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         static_mutex.hpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: Declares static_mutex lock type, there are three different
  *                implementations: POSIX pthreads, WIN32 threads, and portable,
  *                these are described in more detail below.
  */

#ifndef HBOOST_REGEX_STATIC_MUTEX_HPP
#define HBOOST_REGEX_STATIC_MUTEX_HPP

#include <hboost/config.hpp>
#include <hboost/regex/config.hpp> // dll import/export options.

#ifdef HBOOST_HAS_PTHREADS
#include <pthread.h>
#endif

#if defined(HBOOST_HAS_PTHREADS) && defined(PTHREAD_MUTEX_INITIALIZER)
//
// pthreads version:
// simple wrap around a pthread_mutex_t initialized with
// PTHREAD_MUTEX_INITIALIZER.
//
namespace hboost{

class static_mutex;

#define HBOOST_STATIC_MUTEX_INIT { PTHREAD_MUTEX_INITIALIZER, }

class HBOOST_REGEX_DECL scoped_static_mutex_lock
{
public:
   scoped_static_mutex_lock(static_mutex& mut, bool lk = true);
   ~scoped_static_mutex_lock();
   inline bool locked()const
   {
      return m_have_lock;
   }
   inline operator void const*()const
   {
      return locked() ? this : 0;
   }
   void lock();
   void unlock();
private:
   static_mutex& m_mutex;
   bool m_have_lock;
};

class static_mutex
{
public:
   typedef scoped_static_mutex_lock scoped_lock;
   pthread_mutex_t m_mutex;
};

} // namespace hboost
#elif defined(HBOOST_HAS_WINTHREADS)
//
// Win32 version:
// Use a 32-bit int as a lock, along with a test-and-set
// implementation using InterlockedCompareExchange.
//

#include <hboost/cstdint.hpp>

namespace hboost{

class HBOOST_REGEX_DECL scoped_static_mutex_lock;

class static_mutex
{
public:
   typedef scoped_static_mutex_lock scoped_lock;
   hboost::int32_t m_mutex;
};

#define HBOOST_STATIC_MUTEX_INIT { 0, }

class HBOOST_REGEX_DECL scoped_static_mutex_lock
{
public:
   scoped_static_mutex_lock(static_mutex& mut, bool lk = true);
   ~scoped_static_mutex_lock();
   operator void const*()const
   {
      return locked() ? this : 0;
   }
   bool locked()const
   {
      return m_have_lock;
   }
   void lock();
   void unlock();
private:
   static_mutex& m_mutex;
   bool m_have_lock;
   scoped_static_mutex_lock(const scoped_static_mutex_lock&);
   scoped_static_mutex_lock& operator=(const scoped_static_mutex_lock&);
};

} // namespace

#else
//
// Portable version of a static mutex based on Boost.Thread library:
// This has to use a single mutex shared by all instances of static_mutex
// because hboost::call_once doesn't alow us to pass instance information
// down to the initialisation proceedure.  In fact the initialisation routine
// may need to be called more than once - but only once per instance.
//
// Since this preprocessor path is almost never taken, we hide these header
// dependencies so that build tools don't find them.
//
#define HBOOST_REGEX_H1 <hboost/thread/once.hpp>
#define HBOOST_REGEX_H2 <hboost/thread/recursive_mutex.hpp>
#define HBOOST_REGEX_H3 <hboost/thread/lock_types.hpp>
#include HBOOST_REGEX_H1
#include HBOOST_REGEX_H2
#include HBOOST_REGEX_H3
#undef HBOOST_REGEX_H1
#undef HBOOST_REGEX_H2
#undef HBOOST_REGEX_H3

namespace hboost{

class HBOOST_REGEX_DECL scoped_static_mutex_lock;
extern "C" HBOOST_REGEX_DECL void hboost_regex_free_static_mutex();

class HBOOST_REGEX_DECL static_mutex
{
public:
   typedef scoped_static_mutex_lock scoped_lock;
   static void init();
   static hboost::recursive_mutex* m_pmutex;
   static hboost::once_flag m_once;
};

#define HBOOST_STATIC_MUTEX_INIT {  }

class HBOOST_REGEX_DECL scoped_static_mutex_lock
{
public:
   scoped_static_mutex_lock(static_mutex& mut, bool lk = true);
   ~scoped_static_mutex_lock();
   operator void const*()const;
   bool locked()const;
   void lock();
   void unlock();
private:
   hboost::unique_lock<hboost::recursive_mutex>* m_plock;
   bool m_have_lock;
};

inline scoped_static_mutex_lock::operator void const*()const
{
   return locked() ? this : 0;
}

inline bool scoped_static_mutex_lock::locked()const
{
   return m_have_lock;
}

} // namespace

#endif

#endif
