#ifndef HBOOST_THREAD_WIN32_SHARED_MUTEX_HPP
#define HBOOST_THREAD_WIN32_SHARED_MUTEX_HPP

//  (C) Copyright 2006-8 Anthony Williams
//  (C) Copyright 2011-2012 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/assert.hpp>
#include <hboost/detail/interlocked.hpp>
#include <hboost/thread/win32/thread_primitives.hpp>
#include <hboost/static_assert.hpp>
#include <limits.h>
#include <hboost/thread/thread_time.hpp>
#ifdef HBOOST_THREAD_USES_CHRONO
#include <hboost/chrono/system_clocks.hpp>
#include <hboost/chrono/ceil.hpp>
#endif
#include <hboost/thread/detail/delete.hpp>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
    class shared_mutex
    {
    private:
        struct state_data
        {
            unsigned shared_count:11,
                shared_waiting:11,
                exclusive:1,
                upgrade:1,
                exclusive_waiting:7,
                exclusive_waiting_blocked:1;

            friend bool operator==(state_data const& lhs,state_data const& rhs)
            {
                return *reinterpret_cast<unsigned const*>(&lhs)==*reinterpret_cast<unsigned const*>(&rhs);
            }
        };


        template<typename T>
        T interlocked_compare_exchange(T* target,T new_value,T comparand)
        {
            HBOOST_STATIC_ASSERT(sizeof(T)==sizeof(long));
            long const res=HBOOST_INTERLOCKED_COMPARE_EXCHANGE(reinterpret_cast<long*>(target),
                                                              *reinterpret_cast<long*>(&new_value),
                                                              *reinterpret_cast<long*>(&comparand));
            return *reinterpret_cast<T const*>(&res);
        }

        enum
        {
            unlock_sem = 0,
            exclusive_sem = 1
        };

        state_data state;
        detail::win32::handle semaphores[2];
        detail::win32::handle upgrade_sem;

        void release_waiters(state_data old_state)
        {
            if(old_state.exclusive_waiting)
            {
                HBOOST_VERIFY(detail::win32::ReleaseSemaphore(semaphores[exclusive_sem],1,0)!=0);
            }

            if(old_state.shared_waiting || old_state.exclusive_waiting)
            {
                HBOOST_VERIFY(detail::win32::ReleaseSemaphore(semaphores[unlock_sem],old_state.shared_waiting + (old_state.exclusive_waiting?1:0),0)!=0);
            }
        }
        void release_shared_waiters(state_data old_state)
        {
            if(old_state.shared_waiting || old_state.exclusive_waiting)
            {
                HBOOST_VERIFY(detail::win32::ReleaseSemaphore(semaphores[unlock_sem],old_state.shared_waiting + (old_state.exclusive_waiting?1:0),0)!=0);
            }
        }

    public:
        HBOOST_THREAD_NO_COPYABLE(shared_mutex)
        shared_mutex()
        {
            semaphores[unlock_sem]=detail::win32::create_anonymous_semaphore(0,LONG_MAX);
            semaphores[exclusive_sem]=detail::win32::create_anonymous_semaphore_nothrow(0,LONG_MAX);
            if (!semaphores[exclusive_sem])
            {
              detail::win32::release_semaphore(semaphores[unlock_sem],LONG_MAX);
              hboost::throw_exception(thread_resource_error());
            }
            upgrade_sem=detail::win32::create_anonymous_semaphore_nothrow(0,LONG_MAX);
            if (!upgrade_sem)
            {
              detail::win32::release_semaphore(semaphores[unlock_sem],LONG_MAX);
              detail::win32::release_semaphore(semaphores[exclusive_sem],LONG_MAX);
              hboost::throw_exception(thread_resource_error());
            }
            state_data state_={0,0,0,0,0,0};
            state=state_;
        }

        ~shared_mutex()
        {
            detail::win32::CloseHandle(upgrade_sem);
            detail::win32::CloseHandle(semaphores[unlock_sem]);
            detail::win32::CloseHandle(semaphores[exclusive_sem]);
        }

        bool try_lock_shared()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                if(!new_state.exclusive && !new_state.exclusive_waiting_blocked)
                {
                    ++new_state.shared_count;
                    if(!new_state.shared_count)
                    {
                        return false;
                    }
                }

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    break;
                }
                old_state=current_state;
            }
            return !(old_state.exclusive| old_state.exclusive_waiting_blocked);
        }

        void lock_shared()
        {

#if defined HBOOST_THREAD_USES_DATETIME
            HBOOST_VERIFY(timed_lock_shared(::hboost::detail::get_system_time_sentinel()));
#else
            HBOOST_VERIFY(try_lock_shared_until(chrono::steady_clock::now()));
#endif
        }

#if defined HBOOST_THREAD_USES_DATETIME
        template<typename TimeDuration>
        bool timed_lock_shared(TimeDuration const & relative_time)
        {
            return timed_lock_shared(get_system_time()+relative_time);
        }
        bool timed_lock_shared(hboost::system_time const& wait_until)
        {
            for(;;)
            {
                state_data old_state=state;
                for(;;)
                {
                    state_data new_state=old_state;
                    if(new_state.exclusive || new_state.exclusive_waiting_blocked)
                    {
                        ++new_state.shared_waiting;
                        if(!new_state.shared_waiting)
                        {
                            hboost::throw_exception(hboost::lock_error());
                        }
                    }
                    else
                    {
                        ++new_state.shared_count;
                        if(!new_state.shared_count)
                        {
                            hboost::throw_exception(hboost::lock_error());
                        }
                    }

                    state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                    if(current_state==old_state)
                    {
                        break;
                    }
                    old_state=current_state;
                }

                if(!(old_state.exclusive| old_state.exclusive_waiting_blocked))
                {
                    return true;
                }

                unsigned long const res=detail::win32::WaitForSingleObjectEx(semaphores[unlock_sem],::hboost::detail::get_milliseconds_until(wait_until), 0);
                if(res==detail::win32::timeout)
                {
                    for(;;)
                    {
                        state_data new_state=old_state;
                        if(new_state.exclusive || new_state.exclusive_waiting_blocked)
                        {
                            if(new_state.shared_waiting)
                            {
                                --new_state.shared_waiting;
                            }
                        }
                        else
                        {
                            ++new_state.shared_count;
                            if(!new_state.shared_count)
                            {
                                return false;
                            }
                        }

                        state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                        if(current_state==old_state)
                        {
                            break;
                        }
                        old_state=current_state;
                    }

                    if(!(old_state.exclusive| old_state.exclusive_waiting_blocked))
                    {
                        return true;
                    }
                    return false;
                }

                HBOOST_ASSERT(res==0);
            }
        }
#endif

#ifdef HBOOST_THREAD_USES_CHRONO
        template <class Rep, class Period>
        bool try_lock_shared_for(const chrono::duration<Rep, Period>& rel_time)
        {
          return try_lock_shared_until(chrono::steady_clock::now() + rel_time);
        }
        template <class Clock, class Duration>
        bool try_lock_shared_until(const chrono::time_point<Clock, Duration>& t)
        {
          using namespace chrono;
          system_clock::time_point     s_now = system_clock::now();
          typename Clock::time_point  c_now = Clock::now();
          return try_lock_shared_until(s_now + ceil<system_clock::duration>(t - c_now));
        }
        template <class Duration>
        bool try_lock_shared_until(const chrono::time_point<chrono::system_clock, Duration>& t)
        {
          using namespace chrono;
          typedef time_point<chrono::system_clock, chrono::system_clock::duration> sys_tmpt;
          return try_lock_shared_until(sys_tmpt(chrono::ceil<chrono::system_clock::duration>(t.time_since_epoch())));
        }
        bool try_lock_shared_until(const chrono::time_point<chrono::system_clock, chrono::system_clock::duration>& tp)
        {
          for(;;)
          {
            state_data old_state=state;
            for(;;)
            {
              state_data new_state=old_state;
              if(new_state.exclusive || new_state.exclusive_waiting_blocked)
              {
                  ++new_state.shared_waiting;
                  if(!new_state.shared_waiting)
                  {
                      hboost::throw_exception(hboost::lock_error());
                  }
              }
              else
              {
                  ++new_state.shared_count;
                  if(!new_state.shared_count)
                  {
                      hboost::throw_exception(hboost::lock_error());
                  }
              }

              state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
              if(current_state==old_state)
              {
                  break;
              }
              old_state=current_state;
            }

            if(!(old_state.exclusive| old_state.exclusive_waiting_blocked))
            {
              return true;
            }

            chrono::system_clock::time_point n = chrono::system_clock::now();
            unsigned long res;
            if (tp>n) {
              chrono::milliseconds rel_time= chrono::ceil<chrono::milliseconds>(tp-n);
              res=detail::win32::WaitForSingleObjectEx(semaphores[unlock_sem],
                static_cast<unsigned long>(rel_time.count()), 0);
            } else {
              res=detail::win32::timeout;
            }
            if(res==detail::win32::timeout)
            {
              for(;;)
              {
                state_data new_state=old_state;
                if(new_state.exclusive || new_state.exclusive_waiting_blocked)
                {
                  if(new_state.shared_waiting)
                  {
                      --new_state.shared_waiting;
                  }
                }
                else
                {
                  ++new_state.shared_count;
                  if(!new_state.shared_count)
                  {
                      return false;
                  }
                }

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    break;
                }
                old_state=current_state;
              }

              if(!(old_state.exclusive| old_state.exclusive_waiting_blocked))
              {
                return true;
              }
              return false;
            }

            HBOOST_ASSERT(res==0);
          }
        }
#endif

        void unlock_shared()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                bool const last_reader=!--new_state.shared_count;

                if(last_reader)
                {
                    if(new_state.upgrade)
                    {
                        new_state.upgrade=false;
                        new_state.exclusive=true;
                    }
                    else
                    {
                        if(new_state.exclusive_waiting)
                        {
                            --new_state.exclusive_waiting;
                            new_state.exclusive_waiting_blocked=false;
                        }
                        new_state.shared_waiting=0;
                    }
                }

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    if(last_reader)
                    {
                        if(old_state.upgrade)
                        {
                            HBOOST_VERIFY(detail::win32::ReleaseSemaphore(upgrade_sem,1,0)!=0);
                        }
                        else
                        {
                            release_waiters(old_state);
                        }
                    }
                    break;
                }
                old_state=current_state;
            }
        }

        void lock()
        {

#if defined HBOOST_THREAD_USES_DATETIME
            HBOOST_VERIFY(timed_lock(::hboost::detail::get_system_time_sentinel()));
#else
            HBOOST_VERIFY(try_lock_until(chrono::steady_clock::now()));
#endif
        }

#if defined HBOOST_THREAD_USES_DATETIME
        template<typename TimeDuration>
        bool timed_lock(TimeDuration const & relative_time)
        {
            return timed_lock(get_system_time()+relative_time);
        }
#endif

        bool try_lock()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                if(new_state.shared_count || new_state.exclusive)
                {
                    return false;
                }
                else
                {
                    new_state.exclusive=true;
                }

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    break;
                }
                old_state=current_state;
            }
            return true;
        }


#if defined HBOOST_THREAD_USES_DATETIME
        bool timed_lock(hboost::system_time const& wait_until)
        {
            for(;;)
            {
                state_data old_state=state;

                for(;;)
                {
                    state_data new_state=old_state;
                    if(new_state.shared_count || new_state.exclusive)
                    {
                        ++new_state.exclusive_waiting;
                        if(!new_state.exclusive_waiting)
                        {
                            hboost::throw_exception(hboost::lock_error());
                        }

                        new_state.exclusive_waiting_blocked=true;
                    }
                    else
                    {
                        new_state.exclusive=true;
                    }

                    state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                    if(current_state==old_state)
                    {
                        break;
                    }
                    old_state=current_state;
                }

                if(!old_state.shared_count && !old_state.exclusive)
                {
                    return true;
                }
                #ifndef UNDER_CE
                const bool wait_all = true;
                #else
                const bool wait_all = false;
                #endif
                unsigned long const wait_res=detail::win32::WaitForMultipleObjectsEx(2,semaphores,wait_all,::hboost::detail::get_milliseconds_until(wait_until), 0);
                if(wait_res==detail::win32::timeout)
                {
                    for(;;)
                    {
                        bool must_notify = false;
                        state_data new_state=old_state;
                        if(new_state.shared_count || new_state.exclusive)
                        {
                            if(new_state.exclusive_waiting)
                            {
                                if(!--new_state.exclusive_waiting)
                                {
                                    new_state.exclusive_waiting_blocked=false;
                                    must_notify = true;
                                }
                            }
                        }
                        else
                        {
                            new_state.exclusive=true;
                        }

                        state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                        if (must_notify)
                        {
                          HBOOST_VERIFY(detail::win32::ReleaseSemaphore(semaphores[unlock_sem],1,0)!=0);
                        }

                        if(current_state==old_state)
                        {
                            break;
                        }
                        old_state=current_state;
                    }
                    if(!old_state.shared_count && !old_state.exclusive)
                    {
                        return true;
                    }
                    return false;
                }
                HBOOST_ASSERT(wait_res<2);
            }
        }
#endif
#ifdef HBOOST_THREAD_USES_CHRONO
        template <class Rep, class Period>
        bool try_lock_for(const chrono::duration<Rep, Period>& rel_time)
        {
          return try_lock_until(chrono::steady_clock::now() + rel_time);
        }
        template <class Clock, class Duration>
        bool try_lock_until(const chrono::time_point<Clock, Duration>& t)
        {
          using namespace chrono;
          system_clock::time_point     s_now = system_clock::now();
          typename Clock::time_point  c_now = Clock::now();
          return try_lock_until(s_now + ceil<system_clock::duration>(t - c_now));
        }
        template <class Duration>
        bool try_lock_until(const chrono::time_point<chrono::system_clock, Duration>& t)
        {
          using namespace chrono;
          typedef time_point<chrono::system_clock, chrono::system_clock::duration> sys_tmpt;
          return try_lock_until(sys_tmpt(chrono::ceil<chrono::system_clock::duration>(t.time_since_epoch())));
        }
        bool try_lock_until(const chrono::time_point<chrono::system_clock, chrono::system_clock::duration>& tp)
        {
          for(;;)
          {
            state_data old_state=state;

            for(;;)
            {
              state_data new_state=old_state;
              if(new_state.shared_count || new_state.exclusive)
              {
                ++new_state.exclusive_waiting;
                if(!new_state.exclusive_waiting)
                {
                    hboost::throw_exception(hboost::lock_error());
                }

                new_state.exclusive_waiting_blocked=true;
              }
              else
              {
                new_state.exclusive=true;
              }

              state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
              if(current_state==old_state)
              {
                break;
              }
              old_state=current_state;
            }

            if(!old_state.shared_count && !old_state.exclusive)
            {
                return true;
            }
            #ifndef UNDER_CE
            const bool wait_all = true;
            #else
            const bool wait_all = false;
            #endif

            chrono::system_clock::time_point n = chrono::system_clock::now();
            unsigned long wait_res;
            if (tp>n) {
              chrono::milliseconds rel_time= chrono::ceil<chrono::milliseconds>(tp-chrono::system_clock::now());
              wait_res=detail::win32::WaitForMultipleObjectsEx(2,semaphores,wait_all,
                  static_cast<unsigned long>(rel_time.count()), 0);
            } else {
              wait_res=detail::win32::timeout;
            }
            if(wait_res==detail::win32::timeout)
            {
              for(;;)
              {
                bool must_notify = false;
                state_data new_state=old_state;
                if(new_state.shared_count || new_state.exclusive)
                {
                  if(new_state.exclusive_waiting)
                  {
                    if(!--new_state.exclusive_waiting)
                    {
                      new_state.exclusive_waiting_blocked=false;
                      must_notify = true;
                    }
                  }
                }
                else
                {
                  new_state.exclusive=true;
                }

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if (must_notify)
                {
                  HBOOST_VERIFY(detail::win32::ReleaseSemaphore(semaphores[unlock_sem],1,0)!=0);
                }
                if(current_state==old_state)
                {
                  break;
                }
                old_state=current_state;
              }
              if(!old_state.shared_count && !old_state.exclusive)
              {
                return true;
              }
              return false;
            }
            HBOOST_ASSERT(wait_res<2);
          }
        }
#endif

        void unlock()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                new_state.exclusive=false;
                if(new_state.exclusive_waiting)
                {
                    --new_state.exclusive_waiting;
                    new_state.exclusive_waiting_blocked=false;
                }
                new_state.shared_waiting=0;

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    break;
                }
                old_state=current_state;
            }
            release_waiters(old_state);
        }

        void lock_upgrade()
        {
            for(;;)
            {
                state_data old_state=state;
                for(;;)
                {
                    state_data new_state=old_state;
                    if(new_state.exclusive || new_state.exclusive_waiting_blocked || new_state.upgrade)
                    {
                        ++new_state.shared_waiting;
                        if(!new_state.shared_waiting)
                        {
                            hboost::throw_exception(hboost::lock_error());
                        }
                    }
                    else
                    {
                        ++new_state.shared_count;
                        if(!new_state.shared_count)
                        {
                            hboost::throw_exception(hboost::lock_error());
                        }
                        new_state.upgrade=true;
                    }

                    state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                    if(current_state==old_state)
                    {
                        break;
                    }
                    old_state=current_state;
                }

                if(!(old_state.exclusive|| old_state.exclusive_waiting_blocked|| old_state.upgrade))
                {
                    return;
                }

                HBOOST_VERIFY(!detail::win32::WaitForSingleObjectEx(semaphores[unlock_sem],detail::win32::infinite, 0));
            }
        }

        bool try_lock_upgrade()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                if(new_state.exclusive || new_state.exclusive_waiting_blocked || new_state.upgrade)
                {
                    return false;
                }
                else
                {
                    ++new_state.shared_count;
                    if(!new_state.shared_count)
                    {
                        return false;
                    }
                    new_state.upgrade=true;
                }

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    break;
                }
                old_state=current_state;
            }
            return true;
        }

        void unlock_upgrade()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                new_state.upgrade=false;
                bool const last_reader=!--new_state.shared_count;

                new_state.shared_waiting=0;
                if(last_reader)
                {
                    if(new_state.exclusive_waiting)
                    {
                        --new_state.exclusive_waiting;
                        new_state.exclusive_waiting_blocked=false;
                    }
                }

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    if(last_reader)
                    {
                        release_waiters(old_state);
                    }
                    else {
                        release_shared_waiters(old_state);
                    }
                    // #7720
                    //else {
                    //    release_waiters(old_state);
                    //}
                    break;
                }
                old_state=current_state;
            }
        }

        void unlock_upgrade_and_lock()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                bool const last_reader=!--new_state.shared_count;

                if(last_reader)
                {
                    new_state.upgrade=false;
                    new_state.exclusive=true;
                }

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    if(!last_reader)
                    {
                        HBOOST_VERIFY(!detail::win32::WaitForSingleObjectEx(upgrade_sem,detail::win32::infinite, 0));
                    }
                    break;
                }
                old_state=current_state;
            }
        }

        void unlock_and_lock_upgrade()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                new_state.exclusive=false;
                new_state.upgrade=true;
                ++new_state.shared_count;
                if(new_state.exclusive_waiting)
                {
                    --new_state.exclusive_waiting;
                    new_state.exclusive_waiting_blocked=false;
                }
                new_state.shared_waiting=0;

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    break;
                }
                old_state=current_state;
            }
            release_waiters(old_state);
        }
//        bool try_unlock_upgrade_and_lock()
//        {
//          return false;
//        }
//#ifdef HBOOST_THREAD_USES_CHRONO
//        template <class Rep, class Period>
//        bool
//        try_unlock_upgrade_and_lock_for(
//                                const chrono::duration<Rep, Period>& rel_time)
//        {
//          return try_unlock_upgrade_and_lock_until(
//                                 chrono::steady_clock::now() + rel_time);
//        }
//        template <class Clock, class Duration>
//        bool
//        try_unlock_upgrade_and_lock_until(
//                          const chrono::time_point<Clock, Duration>& abs_time)
//        {
//          return false;
//        }
//#endif

        void unlock_and_lock_shared()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                new_state.exclusive=false;
                ++new_state.shared_count;
                if(new_state.exclusive_waiting)
                {
                    --new_state.exclusive_waiting;
                    new_state.exclusive_waiting_blocked=false;
                }
                new_state.shared_waiting=0;

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    break;
                }
                old_state=current_state;
            }
            release_waiters(old_state);
        }
        void unlock_upgrade_and_lock_shared()
        {
            state_data old_state=state;
            for(;;)
            {
                state_data new_state=old_state;
                new_state.upgrade=false;
                if(new_state.exclusive_waiting)
                {
                    --new_state.exclusive_waiting;
                    new_state.exclusive_waiting_blocked=false;
                }
                new_state.shared_waiting=0;

                state_data const current_state=interlocked_compare_exchange(&state,new_state,old_state);
                if(current_state==old_state)
                {
                    break;
                }
                old_state=current_state;
            }
            release_waiters(old_state);
        }

    };
    typedef shared_mutex upgrade_mutex;

}

#include <hboost/config/abi_suffix.hpp>

#endif
