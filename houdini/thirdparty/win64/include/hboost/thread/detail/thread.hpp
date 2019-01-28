#ifndef HBOOST_THREAD_THREAD_COMMON_HPP
#define HBOOST_THREAD_THREAD_COMMON_HPP
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007-2010 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba

#include <hboost/thread/detail/config.hpp>
#include <hboost/predef/platform.h>

#include <hboost/thread/exceptions.hpp>
#ifndef HBOOST_NO_IOSTREAM
#include <ostream>
#endif
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/mutex.hpp>
#if defined HBOOST_THREAD_USES_DATETIME
#include <hboost/thread/xtime.hpp>
#endif
#include <hboost/thread/detail/thread_heap_alloc.hpp>
#include <hboost/thread/detail/make_tuple_indices.hpp>
#include <hboost/thread/detail/invoke.hpp>
#include <hboost/thread/detail/is_convertible.hpp>
#include <hboost/assert.hpp>
#include <list>
#include <algorithm>
#include <hboost/core/ref.hpp>
#include <hboost/cstdint.hpp>
#include <hboost/bind.hpp>
#include <stdlib.h>
#include <memory>
#include <hboost/core/enable_if.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/io/ios_state.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/decay.hpp>
#include <hboost/functional/hash.hpp>
#ifdef HBOOST_THREAD_USES_CHRONO
#include <hboost/chrono/system_clocks.hpp>
#include <hboost/chrono/ceil.hpp>
#endif

#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
#include <tuple>
#endif
#include <hboost/config/abi_prefix.hpp>

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace hboost
{

    namespace detail
    {

#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)

      template<typename F, class ...ArgTypes>
      class thread_data:
          public detail::thread_data_base
      {
      public:
          HBOOST_THREAD_NO_COPYABLE(thread_data)
            thread_data(HBOOST_THREAD_RV_REF(F) f_, HBOOST_THREAD_RV_REF(ArgTypes)... args_):
              fp(hboost::forward<F>(f_), hboost::forward<ArgTypes>(args_)...)
            {}
          template <std::size_t ...Indices>
          void run2(tuple_indices<Indices...>)
          {

              detail::invoke(std::move(std::get<0>(fp)), std::move(std::get<Indices>(fp))...);
          }
          void run()
          {
              typedef typename make_tuple_indices<std::tuple_size<std::tuple<F, ArgTypes...> >::value, 1>::type index_type;

              run2(index_type());
          }

      private:
          std::tuple<typename decay<F>::type, typename decay<ArgTypes>::type...> fp;
      };
#else // defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)

        template<typename F>
        class thread_data:
            public detail::thread_data_base
        {
        public:
            HBOOST_THREAD_NO_COPYABLE(thread_data)
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
              thread_data(HBOOST_THREAD_RV_REF(F) f_):
                f(hboost::forward<F>(f_))
              {}
// This overloading must be removed if we want the packaged_task's tests to pass.
//            thread_data(F& f_):
//                f(f_)
//            {}
#else

            thread_data(HBOOST_THREAD_RV_REF(F) f_):
              f(f_)
            {}
            thread_data(F f_):
                f(f_)
            {}
#endif
            //thread_data() {}

            void run()
            {
                f();
            }

        private:
            F f;
        };

        template<typename F>
        class thread_data<hboost::reference_wrapper<F> >:
            public detail::thread_data_base
        {
        private:
            F& f;
        public:
            HBOOST_THREAD_NO_COPYABLE(thread_data)
            thread_data(hboost::reference_wrapper<F> f_):
                f(f_)
            {}
            void run()
            {
                f();
            }
        };

        template<typename F>
        class thread_data<const hboost::reference_wrapper<F> >:
            public detail::thread_data_base
        {
        private:
            F& f;
        public:
            HBOOST_THREAD_NO_COPYABLE(thread_data)
            thread_data(const hboost::reference_wrapper<F> f_):
                f(f_)
            {}
            void run()
            {
                f();
            }
        };
#endif
    }

    class HBOOST_THREAD_DECL thread
    {
    public:
      typedef thread_attributes attributes;

      HBOOST_THREAD_MOVABLE_ONLY(thread)
    private:

        struct dummy;

        void release_handle();

        detail::thread_data_ptr thread_info;

    private:
        bool start_thread_noexcept();
        bool start_thread_noexcept(const attributes& attr);
        void start_thread()
        {
          if (!start_thread_noexcept())
          {
            hboost::throw_exception(thread_resource_error());
          }
        }
        void start_thread(const attributes& attr)
        {
          if (!start_thread_noexcept(attr))
          {
            hboost::throw_exception(thread_resource_error());
          }
        }

        explicit thread(detail::thread_data_ptr data);

        detail::thread_data_ptr get_thread_info HBOOST_PREVENT_MACRO_SUBSTITUTION () const;

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        template<typename F, class ...ArgTypes>
        static inline detail::thread_data_ptr make_thread_info(HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_RV_REF(ArgTypes)... args)
        {
            return detail::thread_data_ptr(detail::heap_new<
                  detail::thread_data<typename hboost::remove_reference<F>::type, ArgTypes...>
                  >(
                    hboost::forward<F>(f), hboost::forward<ArgTypes>(args)...
                  )
                );
        }
#else
        template<typename F>
        static inline detail::thread_data_ptr make_thread_info(HBOOST_THREAD_RV_REF(F) f)
        {
            return detail::thread_data_ptr(detail::heap_new<detail::thread_data<typename hboost::remove_reference<F>::type> >(
                hboost::forward<F>(f)));
        }
#endif
        static inline detail::thread_data_ptr make_thread_info(void (*f)())
        {
            return detail::thread_data_ptr(detail::heap_new<detail::thread_data<void(*)()> >(
                hboost::forward<void(*)()>(f)));
        }
#else
        template<typename F>
        static inline detail::thread_data_ptr make_thread_info(F f
            , typename disable_if_c<
                //hboost::thread_detail::is_convertible<F&,HBOOST_THREAD_RV_REF(F)>::value ||
                is_same<typename decay<F>::type, thread>::value,
                dummy* >::type=0
                )
        {
            return detail::thread_data_ptr(detail::heap_new<detail::thread_data<F> >(f));
        }
        template<typename F>
        static inline detail::thread_data_ptr make_thread_info(HBOOST_THREAD_RV_REF(F) f)
        {
            return detail::thread_data_ptr(detail::heap_new<detail::thread_data<F> >(f));
        }

#endif
    public:
#if 0 // This should not be needed anymore. Use instead HBOOST_THREAD_MAKE_RV_REF.
#if HBOOST_WORKAROUND(__SUNPRO_CC, < 0x5100)
        thread(const volatile thread&);
#endif
#endif
        thread() HBOOST_NOEXCEPT;
        ~thread()
        {

    #if defined HBOOST_THREAD_PROVIDES_THREAD_DESTRUCTOR_CALLS_TERMINATE_IF_JOINABLE
          if (joinable()) {
            std::terminate();
          }
    #else
            detach();
    #endif
        }
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
        template <
          class F
        >
        explicit thread(HBOOST_THREAD_RV_REF(F) f
        //, typename disable_if<is_same<typename decay<F>::type, thread>, dummy* >::type=0
        ):
          thread_info(make_thread_info(thread_detail::decay_copy(hboost::forward<F>(f))))
        {
            start_thread();
        }
        template <
          class F
        >
        thread(attributes const& attrs, HBOOST_THREAD_RV_REF(F) f):
          thread_info(make_thread_info(thread_detail::decay_copy(hboost::forward<F>(f))))
        {
            start_thread(attrs);
        }

#else
#ifdef HBOOST_NO_SFINAE
        template <class F>
        explicit thread(F f):
            thread_info(make_thread_info(f))
        {
            start_thread();
        }
        template <class F>
        thread(attributes const& attrs, F f):
            thread_info(make_thread_info(f))
        {
            start_thread(attrs);
        }
#else
        template <class F>
        explicit thread(F f
        , typename disable_if_c<
        hboost::thread_detail::is_rv<F>::value // todo ass a thread_detail::is_rv
        //hboost::thread_detail::is_convertible<F&,HBOOST_THREAD_RV_REF(F)>::value
            //|| is_same<typename decay<F>::type, thread>::value
           , dummy* >::type=0
        ):
            thread_info(make_thread_info(f))
        {
            start_thread();
        }
        template <class F>
        thread(attributes const& attrs, F f
            , typename disable_if<hboost::thread_detail::is_rv<F>, dummy* >::type=0
            //, typename disable_if<hboost::thread_detail::is_convertible<F&,HBOOST_THREAD_RV_REF(F) >, dummy* >::type=0
        ):
            thread_info(make_thread_info(f))
        {
            start_thread(attrs);
        }
#endif
        template <class F>
        explicit thread(HBOOST_THREAD_RV_REF(F) f
        , typename disable_if<is_same<typename decay<F>::type, thread>, dummy* >::type=0
        ):
#ifdef HBOOST_THREAD_USES_MOVE
        thread_info(make_thread_info(hboost::move<F>(f))) // todo : Add forward
#else
        thread_info(make_thread_info(f)) // todo : Add forward
#endif
        {
            start_thread();
        }

        template <class F>
        thread(attributes const& attrs, HBOOST_THREAD_RV_REF(F) f):
#ifdef HBOOST_THREAD_USES_MOVE
            thread_info(make_thread_info(hboost::move<F>(f))) // todo : Add forward
#else
            thread_info(make_thread_info(f)) // todo : Add forward
#endif
        {
            start_thread(attrs);
        }
#endif
        thread(HBOOST_THREAD_RV_REF(thread) x) HBOOST_NOEXCEPT
        {
            thread_info=HBOOST_THREAD_RV(x).thread_info;
            HBOOST_THREAD_RV(x).thread_info.reset();
        }
#if 0 // This should not be needed anymore. Use instead HBOOST_THREAD_MAKE_RV_REF.
#if HBOOST_WORKAROUND(__SUNPRO_CC, < 0x5100)
        thread& operator=(thread x)
        {
            swap(x);
            return *this;
        }
#endif
#endif

        thread& operator=(HBOOST_THREAD_RV_REF(thread) other) HBOOST_NOEXCEPT
        {

#if defined HBOOST_THREAD_PROVIDES_THREAD_MOVE_ASSIGN_CALLS_TERMINATE_IF_JOINABLE
            if (joinable()) std::terminate();
#else
            detach();
#endif
            thread_info=HBOOST_THREAD_RV(other).thread_info;
            HBOOST_THREAD_RV(other).thread_info.reset();
            return *this;
        }

#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        template <class F, class Arg, class ...Args>
        thread(F&& f, Arg&& arg, Args&&... args) :
          thread_info(make_thread_info(
              thread_detail::decay_copy(hboost::forward<F>(f)),
              thread_detail::decay_copy(hboost::forward<Arg>(arg)),
              thread_detail::decay_copy(hboost::forward<Args>(args))...)
          )

        {
          start_thread();
        }
        template <class F, class Arg, class ...Args>
        thread(attributes const& attrs, F&& f, Arg&& arg, Args&&... args) :
          thread_info(make_thread_info(
              thread_detail::decay_copy(hboost::forward<F>(f)),
              thread_detail::decay_copy(hboost::forward<Arg>(arg)),
              thread_detail::decay_copy(hboost::forward<Args>(args))...)
          )

        {
          start_thread(attrs);
        }
#else
        template <class F,class A1>
        thread(F f,A1 a1,typename disable_if<hboost::thread_detail::is_convertible<F&,thread_attributes >, dummy* >::type=0):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1)))
        {
            start_thread();
        }
        template <class F,class A1,class A2>
        thread(F f,A1 a1,A2 a2):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1,a2)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3>
        thread(F f,A1 a1,A2 a2,A3 a3):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1,a2,a3)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1,a2,a3,a4)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1,a2,a3,a4,a5)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5,class A6>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1,a2,a3,a4,a5,a6)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1,a2,a3,a4,a5,a6,a7)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1,a2,a3,a4,a5,a6,a7,a8)))
        {
            start_thread();
        }

        template <class F,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9>
        thread(F f,A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9):
            thread_info(make_thread_info(hboost::bind(hboost::type<void>(),f,a1,a2,a3,a4,a5,a6,a7,a8,a9)))
        {
            start_thread();
        }
#endif
        void swap(thread& x) HBOOST_NOEXCEPT
        {
            thread_info.swap(x.thread_info);
        }

        class id;
#ifdef HBOOST_THREAD_PLATFORM_PTHREAD
        inline id get_id()  const HBOOST_NOEXCEPT;
#else
        id get_id() const HBOOST_NOEXCEPT;
#endif


        bool joinable() const HBOOST_NOEXCEPT;
    private:
        bool join_noexcept();
    public:
        inline void join();

#ifdef HBOOST_THREAD_USES_CHRONO
#if defined(HBOOST_THREAD_PLATFORM_WIN32)
        template <class Rep, class Period>
        bool try_join_for(const chrono::duration<Rep, Period>& rel_time)
        {
          chrono::milliseconds rel_time2= chrono::ceil<chrono::milliseconds>(rel_time);
          return do_try_join_until(rel_time2.count());
        }
#else
        template <class Rep, class Period>
        bool try_join_for(const chrono::duration<Rep, Period>& rel_time)
        {
          return try_join_until(chrono::steady_clock::now() + rel_time);
        }
#endif
        template <class Clock, class Duration>
        bool try_join_until(const chrono::time_point<Clock, Duration>& t)
        {
          using namespace chrono;
          bool joined= false;
          do {
            system_clock::time_point     s_now = system_clock::now();
            typename Clock::duration   d = ceil<nanoseconds>(t-Clock::now());
            if (d <= Clock::duration::zero()) return false; // in case the Clock::time_point t is already reached
            joined = try_join_until(s_now + d);
          } while (! joined);
          return true;
        }
        template <class Duration>
        bool try_join_until(const chrono::time_point<chrono::system_clock, Duration>& t)
        {
          using namespace chrono;
          typedef time_point<system_clock, nanoseconds> nano_sys_tmpt;
          return try_join_until(nano_sys_tmpt(ceil<nanoseconds>(t.time_since_epoch())));
        }
#endif
#if defined(HBOOST_THREAD_PLATFORM_WIN32)
    private:
        bool do_try_join_until_noexcept(uintmax_t milli, bool& res);
        inline bool do_try_join_until(uintmax_t milli);
    public:
        bool timed_join(const system_time& abs_time);
        //{
        //  return do_try_join_until(get_milliseconds_until(wait_until));
        //}

#ifdef HBOOST_THREAD_USES_CHRONO
        bool try_join_until(const chrono::time_point<chrono::system_clock, chrono::nanoseconds>& tp)
        {
          chrono::milliseconds rel_time= chrono::ceil<chrono::milliseconds>(tp-chrono::system_clock::now());
          return do_try_join_until(rel_time.count());
        }
#endif


#else
    private:
        bool do_try_join_until_noexcept(struct timespec const &timeout, bool& res);
        inline bool do_try_join_until(struct timespec const &timeout);
    public:
#if defined HBOOST_THREAD_USES_DATETIME
        bool timed_join(const system_time& abs_time)
        {
          struct timespec const ts=detail::to_timespec(abs_time);
          return do_try_join_until(ts);
        }
#endif
#ifdef HBOOST_THREAD_USES_CHRONO
        bool try_join_until(const chrono::time_point<chrono::system_clock, chrono::nanoseconds>& tp)
        {
          using namespace chrono;
          nanoseconds d = tp.time_since_epoch();
          timespec ts = hboost::detail::to_timespec(d);
          return do_try_join_until(ts);
        }
#endif

#endif
      public:

#if defined HBOOST_THREAD_USES_DATETIME
        template<typename TimeDuration>
        inline bool timed_join(TimeDuration const& rel_time)
        {
            return timed_join(get_system_time()+rel_time);
        }
#endif
        void detach();

        static unsigned hardware_concurrency() HBOOST_NOEXCEPT;
        static unsigned physical_concurrency() HBOOST_NOEXCEPT;

#define HBOOST_THREAD_DEFINES_THREAD_NATIVE_HANDLE
        typedef detail::thread_data_base::native_handle_type native_handle_type;
        native_handle_type native_handle();

#if defined HBOOST_THREAD_PROVIDES_THREAD_EQ
        // Use thread::id when comparisions are needed
        // backwards compatibility
        bool operator==(const thread& other) const;
        bool operator!=(const thread& other) const;
#endif
#if defined HBOOST_THREAD_USES_DATETIME
        static inline void yield() HBOOST_NOEXCEPT
        {
            this_thread::yield();
        }

        static inline void sleep(const system_time& xt)
        {
            this_thread::sleep(xt);
        }
#endif

#if defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS
        // extensions
        void interrupt();
        bool interruption_requested() const HBOOST_NOEXCEPT;
#endif
    };

    inline void swap(thread& lhs,thread& rhs) HBOOST_NOEXCEPT
    {
        return lhs.swap(rhs);
    }

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
    inline thread&& move(thread& t) HBOOST_NOEXCEPT
    {
        return static_cast<thread&&>(t);
    }
#endif

    HBOOST_THREAD_DCL_MOVABLE(thread)

    namespace this_thread
    {
#ifdef HBOOST_THREAD_PLATFORM_PTHREAD
        inline thread::id get_id() HBOOST_NOEXCEPT;
#else
        thread::id HBOOST_THREAD_DECL get_id() HBOOST_NOEXCEPT;
#endif

#if defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS
        void HBOOST_THREAD_DECL interruption_point();
        bool HBOOST_THREAD_DECL interruption_enabled() HBOOST_NOEXCEPT;
        bool HBOOST_THREAD_DECL interruption_requested() HBOOST_NOEXCEPT;
#endif

#if defined HBOOST_THREAD_USES_DATETIME
        inline HBOOST_SYMBOL_VISIBLE void sleep(xtime const& abs_time)
        {
            sleep(system_time(abs_time));
        }
#endif
    }

    class HBOOST_SYMBOL_VISIBLE thread::id
    {
    private:
        friend inline
        std::size_t
        hash_value(const thread::id &v)
        {
#if defined HBOOST_THREAD_PROVIDES_BASIC_THREAD_ID
          return hash_value(v.thread_data);
#else
          return hash_value(v.thread_data.get());
#endif
        }

#if defined HBOOST_THREAD_PROVIDES_BASIC_THREAD_ID
#if defined(HBOOST_THREAD_PLATFORM_WIN32)
        typedef unsigned int data;
#else
        typedef thread::native_handle_type data;
#endif
#else
        typedef detail::thread_data_ptr data;
#endif
        data thread_data;

        id(data thread_data_):
            thread_data(thread_data_)
        {}
        friend class thread;
        friend id HBOOST_THREAD_DECL this_thread::get_id() HBOOST_NOEXCEPT;
    public:
        id() HBOOST_NOEXCEPT:
#if defined HBOOST_THREAD_PROVIDES_BASIC_THREAD_ID
        thread_data(0)
#else
        thread_data()
#endif
        {}

        id(const id& other) HBOOST_NOEXCEPT :
            thread_data(other.thread_data)
        {}

        bool operator==(const id& y) const HBOOST_NOEXCEPT
        {
            return thread_data==y.thread_data;
        }

        bool operator!=(const id& y) const HBOOST_NOEXCEPT
        {
            return thread_data!=y.thread_data;
        }

        bool operator<(const id& y) const HBOOST_NOEXCEPT
        {
            return thread_data<y.thread_data;
        }

        bool operator>(const id& y) const HBOOST_NOEXCEPT
        {
            return y.thread_data<thread_data;
        }

        bool operator<=(const id& y) const HBOOST_NOEXCEPT
        {
            return !(y.thread_data<thread_data);
        }

        bool operator>=(const id& y) const HBOOST_NOEXCEPT
        {
            return !(thread_data<y.thread_data);
        }

#ifndef HBOOST_NO_IOSTREAM
#ifndef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template<class charT, class traits>
        friend HBOOST_SYMBOL_VISIBLE
  std::basic_ostream<charT, traits>&
        operator<<(std::basic_ostream<charT, traits>& os, const id& x)
        {
            if(x.thread_data)
            {
                io::ios_flags_saver  ifs( os );
                return os<< std::hex << x.thread_data;
            }
            else
            {
                return os<<"{Not-any-thread}";
            }
        }
#else
        template<class charT, class traits>
        HBOOST_SYMBOL_VISIBLE
  std::basic_ostream<charT, traits>&
        print(std::basic_ostream<charT, traits>& os) const
        {
            if(thread_data)
            {
              io::ios_flags_saver  ifs( os );
              return os<< std::hex << thread_data;
            }
            else
            {
                return os<<"{Not-any-thread}";
            }
        }

#endif
#endif
    };

#ifdef HBOOST_THREAD_PLATFORM_PTHREAD
    thread::id thread::get_id() const HBOOST_NOEXCEPT
    {
    #if defined HBOOST_THREAD_PROVIDES_BASIC_THREAD_ID
        return const_cast<thread*>(this)->native_handle();
    #else
        detail::thread_data_ptr const local_thread_info=(get_thread_info)();
        return (local_thread_info? id(local_thread_info) : id());
    #endif
    }

    namespace this_thread
    {
        inline thread::id get_id() HBOOST_NOEXCEPT
        {
        #if defined HBOOST_THREAD_PROVIDES_BASIC_THREAD_ID
             return pthread_self();
        #else
            hboost::detail::thread_data_base* const thread_info=get_or_make_current_thread_data();
            return (thread_info?thread::id(thread_info->shared_from_this()):thread::id());
        #endif
        }
    }
#endif
    void thread::join() {
        if (this_thread::get_id() == get_id())
          hboost::throw_exception(thread_resource_error(static_cast<int>(system::errc::resource_deadlock_would_occur), "boost thread: trying joining itself"));

        HBOOST_THREAD_VERIFY_PRECONDITION( join_noexcept(),
            thread_resource_error(static_cast<int>(system::errc::invalid_argument), "boost thread: thread not joinable")
        );
    }

#ifdef HBOOST_THREAD_PLATFORM_PTHREAD
    bool thread::do_try_join_until(struct timespec const &timeout)
#else
    bool thread::do_try_join_until(uintmax_t timeout)
#endif
    {
        if (this_thread::get_id() == get_id())
          hboost::throw_exception(thread_resource_error(static_cast<int>(system::errc::resource_deadlock_would_occur), "boost thread: trying joining itself"));
        bool res;
        if (do_try_join_until_noexcept(timeout, res))
        {
          return res;
        }
        else
        {
          HBOOST_THREAD_THROW_ELSE_RETURN(
            (thread_resource_error(static_cast<int>(system::errc::invalid_argument), "boost thread: thread not joinable")),
            false
          );
        }
    }

#if !defined(HBOOST_NO_IOSTREAM) && defined(HBOOST_NO_MEMBER_TEMPLATE_FRIENDS)
    template<class charT, class traits>
    HBOOST_SYMBOL_VISIBLE
    std::basic_ostream<charT, traits>&
    operator<<(std::basic_ostream<charT, traits>& os, const thread::id& x)
    {
        return x.print(os);
    }
#endif

#if defined HBOOST_THREAD_PROVIDES_THREAD_EQ
    inline bool thread::operator==(const thread& other) const
    {
        return get_id()==other.get_id();
    }

    inline bool thread::operator!=(const thread& other) const
    {
        return get_id()!=other.get_id();
    }
#endif

    namespace detail
    {
        struct thread_exit_function_base
        {
            virtual ~thread_exit_function_base()
            {}
            virtual void operator()()=0;
        };

        template<typename F>
        struct thread_exit_function:
            thread_exit_function_base
        {
            F f;

            thread_exit_function(F f_):
                f(f_)
            {}

            void operator()()
            {
                f();
            }
        };

        void HBOOST_THREAD_DECL add_thread_exit_function(thread_exit_function_base*);
        struct shared_state_base;
#if defined(HBOOST_THREAD_PLATFORM_WIN32)
        inline void make_ready_at_thread_exit(shared_ptr<shared_state_base> as)
        {
          detail::thread_data_base* const current_thread_data(detail::get_current_thread_data());
          if(current_thread_data)
          {
            current_thread_data->make_ready_at_thread_exit(as);
          }
        }
#else
        void HBOOST_THREAD_DECL make_ready_at_thread_exit(shared_ptr<shared_state_base> as);
#endif
    }

    namespace this_thread
    {
        template<typename F>
        void hboostat_thread_exit(F f)
        {
            detail::thread_exit_function_base* const thread_exit_func=detail::heap_new<detail::thread_exit_function<F> >(f);
            detail::add_thread_exit_function(thread_exit_func);
        }
    }
}

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#include <hboost/config/abi_suffix.hpp>

#endif
