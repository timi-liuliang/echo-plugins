#ifndef HBOOST_THREAD_PTHREAD_THREAD_DATA_HPP
#define HBOOST_THREAD_PTHREAD_THREAD_DATA_HPP
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2008 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba

#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/thread_time.hpp>
#include <hboost/thread/win32/thread_primitives.hpp>
#include <hboost/thread/win32/thread_heap_alloc.hpp>

#include <hboost/predef/platform.h>

#include <hboost/intrusive_ptr.hpp>
#ifdef HBOOST_THREAD_USES_CHRONO
#include <hboost/chrono/system_clocks.hpp>
#endif

#include <map>
#include <vector>
#include <utility>

#include <hboost/config/abi_prefix.hpp>

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace hboost
{
  class condition_variable;
  class mutex;

  class thread_attributes {
  public:
      thread_attributes() HBOOST_NOEXCEPT {
        val_.stack_size = 0;
        //val_.lpThreadAttributes=0;
      }
      ~thread_attributes() {
      }
      // stack size
      void set_stack_size(std::size_t size) HBOOST_NOEXCEPT {
        val_.stack_size = size;
      }

      std::size_t get_stack_size() const HBOOST_NOEXCEPT {
          return val_.stack_size;
      }

      //void set_security(LPSECURITY_ATTRIBUTES lpThreadAttributes)
      //{
      //  val_.lpThreadAttributes=lpThreadAttributes;
      //}
      //LPSECURITY_ATTRIBUTES get_security()
      //{
      //  return val_.lpThreadAttributes;
      //}

      struct win_attrs {
        std::size_t stack_size;
        //LPSECURITY_ATTRIBUTES lpThreadAttributes;
      };
      typedef win_attrs native_handle_type;
      native_handle_type* native_handle() {return &val_;}
      const native_handle_type* native_handle() const {return &val_;}

  private:
      win_attrs val_;
  };

    namespace detail
    {
        struct shared_state_base;
        struct tss_cleanup_function;
        struct thread_exit_callback_node;
        struct tss_data_node
        {
            hboost::shared_ptr<hboost::detail::tss_cleanup_function> func;
            void* value;

            tss_data_node(hboost::shared_ptr<hboost::detail::tss_cleanup_function> func_,
                          void* value_):
                func(func_),value(value_)
            {}
        };

        struct thread_data_base;
        void intrusive_ptr_add_ref(thread_data_base * p);
        void intrusive_ptr_release(thread_data_base * p);

        struct HBOOST_THREAD_DECL thread_data_base
        {
            long count;

            // Win32 threading APIs are not available in store apps so
            // use abstraction on top of Windows::System::Threading.
#if HBOOST_PLAT_WINDOWS_RUNTIME
            detail::win32::scoped_winrt_thread thread_handle;
#else
            detail::win32::handle_manager thread_handle;
#endif

            hboost::detail::thread_exit_callback_node* thread_exit_callbacks;
            unsigned id;
            std::map<void const*,hboost::detail::tss_data_node> tss_data;
            typedef std::vector<std::pair<condition_variable*, mutex*>
            //, hidden_allocator<std::pair<condition_variable*, mutex*> >
            > notify_list_t;
            notify_list_t notify;

            typedef std::vector<shared_ptr<shared_state_base> > async_states_t;
            async_states_t async_states_;
//#if defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS
            // These data must be at the end so that the access to the other fields doesn't change
            // when HBOOST_THREAD_PROVIDES_INTERRUPTIONS is defined
            // Another option is to have them always
            detail::win32::handle_manager interruption_handle;
            bool interruption_enabled;
//#endif

            thread_data_base():
                count(0),
                thread_handle(),
                thread_exit_callbacks(0),
                id(0),
                tss_data(),
                notify(),
                async_states_()
//#if defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS
                , interruption_handle(create_anonymous_event(detail::win32::manual_reset_event,detail::win32::event_initially_reset))
                , interruption_enabled(true)
//#endif
            {}
            virtual ~thread_data_base();

            friend void intrusive_ptr_add_ref(thread_data_base * p)
            {
                HBOOST_INTERLOCKED_INCREMENT(&p->count);
            }

            friend void intrusive_ptr_release(thread_data_base * p)
            {
                if(!HBOOST_INTERLOCKED_DECREMENT(&p->count))
                {
                    detail::heap_delete(p);
                }
            }

#if defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS
            void interrupt()
            {
                HBOOST_VERIFY(detail::win32::SetEvent(interruption_handle)!=0);
            }
#endif
            typedef detail::win32::handle native_handle_type;

            virtual void run()=0;

            virtual void notify_all_at_thread_exit(condition_variable* cv, mutex* m)
            {
              notify.push_back(std::pair<condition_variable*, mutex*>(cv, m));
            }

            void make_ready_at_thread_exit(shared_ptr<shared_state_base> as)
            {
              async_states_.push_back(as);
            }

        };
        HBOOST_THREAD_DECL thread_data_base* get_current_thread_data();

        typedef hboost::intrusive_ptr<detail::thread_data_base> thread_data_ptr;

        struct HBOOST_SYMBOL_VISIBLE timeout
        {
            win32::ticks_type start;
            uintmax_t milliseconds;
            bool relative;
            hboost::system_time abs_time;

            static unsigned long const max_non_infinite_wait=0xfffffffe;

            timeout(uintmax_t milliseconds_):
                start(win32::GetTickCount64_()()),
                milliseconds(milliseconds_),
                relative(true)
            //,
            //    abs_time(hboost::get_system_time())
            {}

            timeout(hboost::system_time const& abs_time_):
                start(win32::GetTickCount64_()()),
                milliseconds(0),
                relative(false),
                abs_time(abs_time_)
            {}

            struct HBOOST_SYMBOL_VISIBLE remaining_time
            {
                bool more;
                unsigned long milliseconds;

                remaining_time(uintmax_t remaining):
                    more(remaining>max_non_infinite_wait),
                    milliseconds(more?max_non_infinite_wait:(unsigned long)remaining)
                {}
            };

            remaining_time remaining_milliseconds() const
            {
                if(is_sentinel())
                {
                    return remaining_time(win32::infinite);
                }
                else if(relative)
                {
                    win32::ticks_type const now=win32::GetTickCount64_()();
                    win32::ticks_type const elapsed=now-start;
                    return remaining_time((elapsed<milliseconds)?(milliseconds-elapsed):0);
                }
                else
                {
                    system_time const now=get_system_time();
                    if(abs_time<=now)
                    {
                        return remaining_time(0);
                    }
                    return remaining_time((abs_time-now).total_milliseconds()+1);
                }
            }

            bool is_sentinel() const
            {
                return milliseconds==~uintmax_t(0);
            }


            static timeout sentinel()
            {
                return timeout(sentinel_type());
            }
        private:
            struct sentinel_type
            {};

            explicit timeout(sentinel_type):
                start(0),milliseconds(~uintmax_t(0)),relative(true)
            {}
        };

        inline uintmax_t pin_to_zero(intmax_t value)
        {
            return (value<0)?0u:(uintmax_t)value;
        }
    }

    namespace this_thread
    {
        void HBOOST_THREAD_DECL yield() HBOOST_NOEXCEPT;

        bool HBOOST_THREAD_DECL interruptible_wait(detail::win32::handle handle_to_wait_for,detail::timeout target_time);
        inline void interruptible_wait(uintmax_t milliseconds)
        {
            interruptible_wait(detail::win32::invalid_handle_value,milliseconds);
        }
        inline HBOOST_SYMBOL_VISIBLE void interruptible_wait(system_time const& abs_time)
        {
            interruptible_wait(detail::win32::invalid_handle_value,abs_time);
        }
        template<typename TimeDuration>
        inline HBOOST_SYMBOL_VISIBLE void sleep(TimeDuration const& rel_time)
        {
            interruptible_wait(detail::pin_to_zero(rel_time.total_milliseconds()));
        }
        inline HBOOST_SYMBOL_VISIBLE void sleep(system_time const& abs_time)
        {
            interruptible_wait(abs_time);
        }
// #11322   sleep_for() nanoseconds overload will always return too early on windows
//#ifdef HBOOST_THREAD_USES_CHRONO
//        inline void HBOOST_SYMBOL_VISIBLE sleep_for(const chrono::nanoseconds& ns)
//        {
//          interruptible_wait(chrono::duration_cast<chrono::milliseconds>(ns).count());
//        }
//#endif
        namespace no_interruption_point
        {
          bool HBOOST_THREAD_DECL non_interruptible_wait(detail::win32::handle handle_to_wait_for,detail::timeout target_time);
          inline void non_interruptible_wait(uintmax_t milliseconds)
          {
            non_interruptible_wait(detail::win32::invalid_handle_value,milliseconds);
          }
          inline HBOOST_SYMBOL_VISIBLE void non_interruptible_wait(system_time const& abs_time)
          {
            non_interruptible_wait(detail::win32::invalid_handle_value,abs_time);
          }
          template<typename TimeDuration>
          inline HBOOST_SYMBOL_VISIBLE void sleep(TimeDuration const& rel_time)
          {
            non_interruptible_wait(detail::pin_to_zero(rel_time.total_milliseconds()));
          }
          inline HBOOST_SYMBOL_VISIBLE void sleep(system_time const& abs_time)
          {
            non_interruptible_wait(abs_time);
          }
// #11322   sleep_for() nanoseconds overload will always return too early on windows
//#ifdef HBOOST_THREAD_USES_CHRONO
//          inline void HBOOST_SYMBOL_VISIBLE sleep_for(const chrono::nanoseconds& ns)
//          {
//            non_interruptible_wait(chrono::duration_cast<chrono::milliseconds>(ns).count());
//          }
//#endif
        }
    }

}

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#include <hboost/config/abi_suffix.hpp>

#endif
