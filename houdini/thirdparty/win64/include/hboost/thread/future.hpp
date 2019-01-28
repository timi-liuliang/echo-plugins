//  (C) Copyright 2008-10 Anthony Williams
//  (C) Copyright 2011-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_FUTURE_HPP
#define HBOOST_THREAD_FUTURE_HPP

#include <hboost/thread/detail/config.hpp>

// hboost::thread::future requires exception handling
// due to hboost::exception::exception_ptr dependency

//#define HBOOST_THREAD_CONTINUATION_SYNC
#define HBOOST_THREAD_FUTURE_BLOCKING

#ifndef HBOOST_NO_EXCEPTIONS

#include <hboost/thread/condition_variable.hpp>
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/detail/invoker.hpp>
#include <hboost/thread/detail/invoke.hpp>
#include <hboost/thread/detail/is_convertible.hpp>
#include <hboost/thread/exceptional_ptr.hpp>
#include <hboost/thread/futures/future_error.hpp>
#include <hboost/thread/futures/future_error_code.hpp>
#include <hboost/thread/futures/future_status.hpp>
#include <hboost/thread/futures/is_future_type.hpp>
#include <hboost/thread/futures/launch.hpp>
#include <hboost/thread/futures/wait_for_all.hpp>
#include <hboost/thread/futures/wait_for_any.hpp>
#include <hboost/thread/lock_algorithms.hpp>
#include <hboost/thread/lock_types.hpp>
#include <hboost/thread/mutex.hpp>
#include <hboost/thread/thread_only.hpp>
#include <hboost/thread/thread_time.hpp>
#include <hboost/thread/executor.hpp>
#include <hboost/thread/executors/generic_executor_ref.hpp>

#if defined HBOOST_THREAD_FUTURE_USES_OPTIONAL
#include <hboost/optional.hpp>
#else
#include <hboost/thread/csbl/memory/unique_ptr.hpp>
#endif

#include <hboost/assert.hpp>
#include <hboost/bind.hpp>
#ifdef HBOOST_THREAD_USES_CHRONO
#include <hboost/chrono/system_clocks.hpp>
#endif
#include <hboost/core/enable_if.hpp>
#include <hboost/core/ref.hpp>
#include <hboost/enable_shared_from_this.hpp>
#include <hboost/exception_ptr.hpp>
#include <hboost/function.hpp>
#include <hboost/next_prior.hpp>
#include <hboost/scoped_array.hpp>
#include <hboost/shared_ptr.hpp>
#include <hboost/smart_ptr/make_shared.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/type_traits/conditional.hpp>
#include <hboost/type_traits/decay.hpp>
#include <hboost/type_traits/is_copy_constructible.hpp>
#include <hboost/type_traits/is_fundamental.hpp>
#include <hboost/type_traits/is_void.hpp>
#include <hboost/utility/result_of.hpp>


#if defined HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
#include <hboost/thread/detail/memory.hpp>
#include <hboost/container/scoped_allocator.hpp>
#if ! defined  HBOOST_NO_CXX11_ALLOCATOR
#include <memory>
#endif
#endif

#if defined HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#include <hboost/thread/csbl/tuple.hpp>
#include <hboost/thread/csbl/vector.hpp>
#endif

#include <algorithm>
#include <list>
#include <vector>
#include <utility>

#if defined HBOOST_THREAD_PROVIDES_FUTURE
#define HBOOST_THREAD_FUTURE future
#else
#define HBOOST_THREAD_FUTURE unique_future
#endif

namespace hboost
{
  template <class T>
  shared_ptr<T> static_shared_from_this(T* that)
  {
    return static_pointer_cast<T>(that->shared_from_this());
  }
  template <class T>
  shared_ptr<T const> static_shared_from_this(T const* that)
  {
    return static_pointer_cast<T const>(that->shared_from_this());
  }

#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
#else
    namespace executors {
        class executor;
    }
#endif
    typedef shared_ptr<executor> executor_ptr_type;

    namespace detail
    {

        struct relocker
        {
            hboost::unique_lock<hboost::mutex>& lock_;

            relocker(hboost::unique_lock<hboost::mutex>& lk):
                lock_(lk)
            {
                lock_.unlock();
            }
            ~relocker()
            {
              if (! lock_.owns_lock()) {
                lock_.lock();
              }
            }
            void lock() {
              if (! lock_.owns_lock()) {
                lock_.lock();
              }
            }
        private:
            relocker& operator=(relocker const&);
        };

        struct shared_state_base : enable_shared_from_this<shared_state_base>
        {
            typedef std::list<hboost::condition_variable_any*> waiter_list;
            typedef waiter_list::iterator notify_when_ready_handle;
            // This type should be only included conditionally if interruptions are allowed, but is included to maintain the same layout.
            typedef shared_ptr<shared_state_base> continuation_ptr_type;
            typedef std::vector<continuation_ptr_type> continuations_type;

            hboost::exception_ptr exception;
            bool done;
            bool is_valid_;
            bool is_deferred_;
            bool is_constructed;
            launch policy_;
            mutable hboost::mutex mutex;
            hboost::condition_variable waiters;
            waiter_list external_waiters;
            hboost::function<void()> callback;
            // This declaration should be only included conditionally, but is included to maintain the same layout.
            continuations_type continuations;
            executor_ptr_type ex;

            // This declaration should be only included conditionally, but is included to maintain the same layout.
            virtual void launch_continuation()
            {
            }

            shared_state_base():
                done(false),
                is_valid_(true),
                is_deferred_(false),
                is_constructed(false),
                policy_(launch::none),
                continuations(),
                ex()
            {}

            shared_state_base(exceptional_ptr const& ex):
                exception(ex.ptr_),
                done(true),
                is_valid_(true),
                is_deferred_(false),
                is_constructed(false),
                policy_(launch::none),
                continuations(),
                ex()
            {}


            virtual ~shared_state_base()
            {
            }
            executor_ptr_type get_executor()
            {
              return ex;
            }

            void set_executor_policy(executor_ptr_type aex)
            {
              set_executor();
              ex = aex;
            }
            void set_executor_policy(executor_ptr_type aex, hboost::lock_guard<hboost::mutex>&)
            {
              set_executor();
              ex = aex;
            }
            void set_executor_policy(executor_ptr_type aex, hboost::unique_lock<hboost::mutex>&)
            {
              set_executor();
              ex = aex;
            }

            bool valid(hboost::unique_lock<hboost::mutex>&) { return is_valid_; }
            bool valid() {
              hboost::unique_lock<hboost::mutex> lk(this->mutex);
              return valid(lk);
            }
            void invalidate(hboost::unique_lock<hboost::mutex>&) { is_valid_ = false; }
            void invalidate() {
              hboost::unique_lock<hboost::mutex> lk(this->mutex);
              invalidate(lk);
            }
            void validate(hboost::unique_lock<hboost::mutex>&) { is_valid_ = true; }
            void validate() {
              hboost::unique_lock<hboost::mutex> lk(this->mutex);
              validate(lk);
            }

            void set_deferred()
            {
              is_deferred_ = true;
              policy_ = launch::deferred;
            }
            void set_async()
            {
              is_deferred_ = false;
              policy_ = launch::async;
            }
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
            void set_executor()
            {
              is_deferred_ = false;
              policy_ = launch::executor;
            }
#else
            void set_executor()
            {
            }
#endif
            notify_when_ready_handle notify_when_ready(hboost::condition_variable_any& cv)
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                do_callback(lock);
                return external_waiters.insert(external_waiters.end(),&cv);
            }

            void unnotify_when_ready(notify_when_ready_handle it)
            {
                hboost::lock_guard<hboost::mutex> lock(this->mutex);
                external_waiters.erase(it);
            }

#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
            void do_continuation(hboost::unique_lock<hboost::mutex>& lock)
            {
                if (! continuations.empty()) {
                  continuations_type the_continuations = continuations;
                  continuations.clear();
                  relocker rlk(lock);
                  for (continuations_type::iterator it = the_continuations.begin(); it != the_continuations.end(); ++it) {
                    (*it)->launch_continuation();
                  }
                }
            }
#else
            void do_continuation(hboost::unique_lock<hboost::mutex>&)
            {
            }
#endif
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
            virtual void set_continuation_ptr(continuation_ptr_type continuation, hboost::unique_lock<hboost::mutex>& lock)
            {
              continuations.push_back(continuation);
              if (done) {
                do_continuation(lock);
              }
            }
#endif
            void mark_finished_internal(hboost::unique_lock<hboost::mutex>& lock)
            {
                done=true;
                waiters.notify_all();
                for(waiter_list::const_iterator it=external_waiters.begin(),
                        end=external_waiters.end();it!=end;++it)
                {
                    (*it)->notify_all();
                }
                do_continuation(lock);
            }
            void make_ready()
            {
              hboost::unique_lock<hboost::mutex> lock(this->mutex);
              mark_finished_internal(lock);
            }

            void do_callback(hboost::unique_lock<hboost::mutex>& lock)
            {
                if(callback && !done)
                {
                    hboost::function<void()> local_callback=callback;
                    relocker relock(lock);
                    local_callback();
                }
            }

            virtual bool run_if_is_deferred()
            {
              hboost::unique_lock<hboost::mutex> lk(this->mutex);
              if (is_deferred_)
              {
                is_deferred_=false;
                execute(lk);
                return true;
              }
              else
                return false;
            }
            virtual bool run_if_is_deferred_or_ready()
            {
              hboost::unique_lock<hboost::mutex> lk(this->mutex);
              if (is_deferred_)
              {
                is_deferred_=false;
                execute(lk);

                return true;
              }
              else
                return done;
            }
            void wait_internal(hboost::unique_lock<hboost::mutex> &lk, bool rethrow=true)
            {
              do_callback(lk);
              if (is_deferred_)
              {
                is_deferred_=false;
                execute(lk);
              }
              while(!done)
              {
                  waiters.wait(lk);
              }
              if(rethrow && exception)
              {
                  hboost::rethrow_exception(exception);
              }
            }

            virtual void wait(hboost::unique_lock<hboost::mutex>& lock, bool rethrow=true)
            {
                wait_internal(lock, rethrow);
            }

            void wait(bool rethrow=true)
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                wait(lock, rethrow);
            }

#if defined HBOOST_THREAD_USES_DATETIME
            bool timed_wait_until(hboost::system_time const& target_time)
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                if (is_deferred_)
                    return false;

                do_callback(lock);
                while(!done)
                {
                    bool const success=waiters.timed_wait(lock,target_time);
                    if(!success && !done)
                    {
                        return false;
                    }
                }
                return true;
            }
#endif
#ifdef HBOOST_THREAD_USES_CHRONO

            template <class Clock, class Duration>
            future_status
            wait_until(const chrono::time_point<Clock, Duration>& abs_time)
            {
              hboost::unique_lock<hboost::mutex> lock(this->mutex);
              if (is_deferred_)
                  return future_status::deferred;
              do_callback(lock);
              while(!done)
              {
                  cv_status const st=waiters.wait_until(lock,abs_time);
                  if(st==cv_status::timeout && !done)
                  {
                    return future_status::timeout;
                  }
              }
              return future_status::ready;
            }
#endif
            void mark_exceptional_finish_internal(hboost::exception_ptr const& e, hboost::unique_lock<hboost::mutex>& lock)
            {
                exception=e;
                mark_finished_internal(lock);
            }

            void mark_exceptional_finish()
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                mark_exceptional_finish_internal(hboost::current_exception(), lock);
            }

            void set_exception_at_thread_exit(exception_ptr e)
            {
              unique_lock<hboost::mutex> lk(this->mutex);
              if (has_value(lk))
              {
                  throw_exception(promise_already_satisfied());
              }
              exception=e;
              this->is_constructed = true;
              detail::make_ready_at_thread_exit(shared_from_this());
            }

            bool has_value() const
            {
                hboost::lock_guard<hboost::mutex> lock(this->mutex);
                return done && ! exception;
            }

            bool has_value(unique_lock<hboost::mutex>& )  const
            {
                return done && ! exception;
            }

            bool has_exception()  const
            {
                hboost::lock_guard<hboost::mutex> lock(this->mutex);
                return done && exception;
            }

            launch launch_policy(hboost::unique_lock<hboost::mutex>&) const
            {
                return policy_;
            }

            future_state::state get_state(hboost::unique_lock<hboost::mutex>&) const
            {
                if(!done)
                {
                    return future_state::waiting;
                }
                else
                {
                    return future_state::ready;
                }
            }
            future_state::state get_state() const
            {
                hboost::lock_guard<hboost::mutex> guard(this->mutex);
                if(!done)
                {
                    return future_state::waiting;
                }
                else
                {
                    return future_state::ready;
                }
            }

            exception_ptr get_exception_ptr()
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                wait_internal(lock, false);
                return exception;
            }

            template<typename F,typename U>
            void set_wait_callback(F f,U* u)
            {
                hboost::lock_guard<hboost::mutex> lock(this->mutex);
                callback=hboost::bind(f,hboost::ref(*u));
            }

            virtual void execute(hboost::unique_lock<hboost::mutex>&) {}

        private:
            shared_state_base(shared_state_base const&);
            shared_state_base& operator=(shared_state_base const&);
        };

        // Used to create stand-alone futures
        template<typename T>
        struct shared_state:
            detail::shared_state_base
        {
#if defined HBOOST_THREAD_FUTURE_USES_OPTIONAL
              typedef hboost::optional<T> storage_type;
#else
              typedef hboost::csbl::unique_ptr<T> storage_type;
#endif
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
            typedef T const& source_reference_type;
            typedef HBOOST_THREAD_RV_REF(T) rvalue_source_type;
            typedef T move_dest_type;
#elif defined HBOOST_THREAD_USES_MOVE
            typedef typename conditional<hboost::is_fundamental<T>::value,T,T const&>::type source_reference_type;
            typedef HBOOST_THREAD_RV_REF(T) rvalue_source_type;
            typedef T move_dest_type;
#else
            typedef T& source_reference_type;
            typedef typename conditional<hboost::thread_detail::is_convertible<T&,HBOOST_THREAD_RV_REF(T) >::value, HBOOST_THREAD_RV_REF(T),T const&>::type rvalue_source_type;
            typedef typename conditional<hboost::thread_detail::is_convertible<T&,HBOOST_THREAD_RV_REF(T) >::value, HBOOST_THREAD_RV_REF(T),T>::type move_dest_type;
#endif

            typedef const T& shared_future_get_result_type;

            storage_type result;

            shared_state():
                result()
            {}
            shared_state(exceptional_ptr const& ex):
              detail::shared_state_base(ex), result()
            {}


            ~shared_state()
            {
            }

            void mark_finished_with_result_internal(source_reference_type result_, hboost::unique_lock<hboost::mutex>& lock)
            {
#if defined HBOOST_THREAD_FUTURE_USES_OPTIONAL
                result = result_;
#else
                result.reset(new T(result_));
#endif
                this->mark_finished_internal(lock);
            }

            void mark_finished_with_result_internal(rvalue_source_type result_, hboost::unique_lock<hboost::mutex>& lock)
            {
#if defined HBOOST_THREAD_FUTURE_USES_OPTIONAL
                result = hboost::move(result_);
#elif ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
                result.reset(new T(hboost::move(result_)));
#else
                result.reset(new T(static_cast<rvalue_source_type>(result_)));
#endif
                this->mark_finished_internal(lock);
            }


#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
            template <class ...Args>
            void mark_finished_with_result_internal(hboost::unique_lock<hboost::mutex>& lock, HBOOST_THREAD_FWD_REF(Args)... args)
            {
#if defined HBOOST_THREAD_FUTURE_USES_OPTIONAL
                result.emplace(hboost::forward<Args>(args)...);
#else
                result.reset(new T(hboost::forward<Args>(args)...));
#endif
                this->mark_finished_internal(lock);
            }
#endif

            void mark_finished_with_result(source_reference_type result_)
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                this->mark_finished_with_result_internal(result_, lock);
            }

            void mark_finished_with_result(rvalue_source_type result_)
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);

#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
                mark_finished_with_result_internal(hboost::move(result_), lock);
#else
                mark_finished_with_result_internal(static_cast<rvalue_source_type>(result_), lock);
#endif
            }

            storage_type& get_storage(hboost::unique_lock<hboost::mutex>& lk)
            {
                wait_internal(lk);
                return result;
            }
            virtual move_dest_type get(hboost::unique_lock<hboost::mutex>& lk)
            {
                return hboost::move(*get_storage(lk));
            }
            move_dest_type get()
            {
                hboost::unique_lock<hboost::mutex> lk(this->mutex);
                return this->get(lk);
            }

            virtual shared_future_get_result_type get_sh(hboost::unique_lock<hboost::mutex>& lk)
            {
                return *get_storage(lk);
            }
            shared_future_get_result_type get_sh()
            {
                hboost::unique_lock<hboost::mutex> lk(this->mutex);
                return this->get_sh(lk);
            }

            void set_value_at_thread_exit(source_reference_type result_)
            {
              unique_lock<hboost::mutex> lk(this->mutex);
              if (this->has_value(lk))
              {
                  throw_exception(promise_already_satisfied());
              }
#if defined HBOOST_THREAD_FUTURE_USES_OPTIONAL
              result = result_;
#else
              result.reset(new T(result_));
#endif

              this->is_constructed = true;
              detail::make_ready_at_thread_exit(shared_from_this());
            }
            void set_value_at_thread_exit(rvalue_source_type result_)
            {
              unique_lock<hboost::mutex> lk(this->mutex);
              if (this->has_value(lk))
                  throw_exception(promise_already_satisfied());

#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
#if defined HBOOST_THREAD_FUTURE_USES_OPTIONAL
                result = hboost::move(result_);
#else
                result.reset(new T(hboost::move(result_)));
#endif
#else
#if defined HBOOST_THREAD_FUTURE_USES_OPTIONAL
                result = hboost::move(result_);
#else
                result.reset(new T(static_cast<rvalue_source_type>(result_)));
#endif
#endif
              this->is_constructed = true;
              detail::make_ready_at_thread_exit(shared_from_this());
            }

        private:
            shared_state(shared_state const&);
            shared_state& operator=(shared_state const&);
        };

        template<typename T>
        struct shared_state<T&>:
            detail::shared_state_base
        {
            typedef T* storage_type;
            typedef T& source_reference_type;
            typedef T& move_dest_type;
            typedef T& shared_future_get_result_type;

            T* result;

            shared_state():
                result(0)
            {}

            shared_state(exceptional_ptr const& ex):
              detail::shared_state_base(ex), result(0)
            {}

            ~shared_state()
            {
            }

            void mark_finished_with_result_internal(source_reference_type result_, hboost::unique_lock<hboost::mutex>& lock)
            {
                result= &result_;
                mark_finished_internal(lock);
            }

            void mark_finished_with_result(source_reference_type result_)
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                mark_finished_with_result_internal(result_, lock);
            }

            virtual T& get(hboost::unique_lock<hboost::mutex>& lock)
            {
                wait_internal(lock);
                return *result;
            }
            T& get()
            {
                hboost::unique_lock<hboost::mutex> lk(this->mutex);
                return get(lk);
            }

            virtual T& get_sh(hboost::unique_lock<hboost::mutex>& lock)
            {
                wait_internal(lock);
                return *result;
            }
            T& get_sh()
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                return get_sh(lock);
            }

            void set_value_at_thread_exit(T& result_)
            {
              unique_lock<hboost::mutex> lk(this->mutex);
              if (this->has_value(lk))
                  throw_exception(promise_already_satisfied());
              result= &result_;
              this->is_constructed = true;
              detail::make_ready_at_thread_exit(shared_from_this());
            }

        private:
            shared_state(shared_state const&);
            shared_state& operator=(shared_state const&);
        };

        template<>
        struct shared_state<void>:
            detail::shared_state_base
        {
            typedef void shared_future_get_result_type;
            typedef void move_dest_type;

            shared_state()
            {}

            shared_state(exceptional_ptr const& ex):
              detail::shared_state_base(ex)
            {}

            void mark_finished_with_result_internal(hboost::unique_lock<hboost::mutex>& lock)
            {
                mark_finished_internal(lock);
            }

            void mark_finished_with_result()
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                mark_finished_with_result_internal(lock);
            }

            virtual void get(hboost::unique_lock<hboost::mutex>& lock)
            {
                this->wait_internal(lock);
            }
            void get()
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                this->get(lock);
            }

            virtual void get_sh(hboost::unique_lock<hboost::mutex>& lock)
            {
                this->wait_internal(lock);
            }
            void get_sh()
            {
                hboost::unique_lock<hboost::mutex> lock(this->mutex);
                this->get_sh(lock);
            }

            void set_value_at_thread_exit()
            {
              unique_lock<hboost::mutex> lk(this->mutex);
              if (this->has_value(lk))
              {
                  throw_exception(promise_already_satisfied());
              }
              this->is_constructed = true;
              detail::make_ready_at_thread_exit(shared_from_this());
            }
        private:
            shared_state(shared_state const&);
            shared_state& operator=(shared_state const&);
        };

        /////////////////////////
        /// future_async_shared_state_base
        /////////////////////////
        template<typename Rp>
        struct future_async_shared_state_base: shared_state<Rp>
        {
          typedef shared_state<Rp> base_type;
        protected:
#ifdef HBOOST_THREAD_FUTURE_BLOCKING
          hboost::thread thr_;
          void join()
          {
              if (this_thread::get_id() == thr_.get_id())
              {
                  thr_.detach();
                  return;
              }
              if (thr_.joinable()) thr_.join();
          }
#endif
        public:
          future_async_shared_state_base()
          {
            this->set_async();
          }

          ~future_async_shared_state_base()
          {
#ifdef HBOOST_THREAD_FUTURE_BLOCKING
            join();
#endif
          }

          virtual void wait(hboost::unique_lock<hboost::mutex>& lk, bool rethrow)
          {
#ifdef HBOOST_THREAD_FUTURE_BLOCKING
              {
                relocker rlk(lk);
                join();
              }
#endif
              this->base_type::wait(lk, rethrow);
          }
        };

        /////////////////////////
        /// future_async_shared_state
        /////////////////////////
        template<typename Rp, typename Fp>
        struct future_async_shared_state: future_async_shared_state_base<Rp>
        {
          future_async_shared_state()
          {
          }

          void init(HBOOST_THREAD_FWD_REF(Fp) f)
          {
#ifdef HBOOST_THREAD_FUTURE_BLOCKING
            this->thr_ = thread(&future_async_shared_state::run, static_shared_from_this(this), hboost::forward<Fp>(f));
#else
            thread(&future_async_shared_state::run, static_shared_from_this(this), hboost::forward<Fp>(f)).detach();
#endif
          }

          static void run(shared_ptr<future_async_shared_state> that, HBOOST_THREAD_FWD_REF(Fp) f)
          {
            try
            {
              that->mark_finished_with_result(f());
            }
            catch(...)
            {
              that->mark_exceptional_finish();
            }
          }
        };

        template<typename Fp>
        struct future_async_shared_state<void, Fp>: public future_async_shared_state_base<void>
        {
          void init(HBOOST_THREAD_FWD_REF(Fp) f)
          {
#ifdef HBOOST_THREAD_FUTURE_BLOCKING
            this->thr_ = thread(&future_async_shared_state::run, static_shared_from_this(this), hboost::move(f));
#else
            thread(&future_async_shared_state::run, static_shared_from_this(this), hboost::move(f)).detach();
#endif
          }

          static void run(shared_ptr<future_async_shared_state> that, HBOOST_THREAD_FWD_REF(Fp) f)
          {
            try
            {
              f();
              that->mark_finished_with_result();
            }
            catch(...)
            {
              that->mark_exceptional_finish();
            }
          }
        };

        template<typename Rp, typename Fp>
        struct future_async_shared_state<Rp&, Fp>: future_async_shared_state_base<Rp&>
        {
          void init(HBOOST_THREAD_FWD_REF(Fp) f)
          {
#ifdef HBOOST_THREAD_FUTURE_BLOCKING
            this->thr_ = thread(&future_async_shared_state::run, static_shared_from_this(this), hboost::move(f));
#else
            thread(&future_async_shared_state::run, static_shared_from_this(this), hboost::move(f)).detach();
#endif
          }

          static void run(shared_ptr<future_async_shared_state> that, HBOOST_THREAD_FWD_REF(Fp) f)
          {
            try
            {
              that->mark_finished_with_result(f());
            }
            catch(...)
            {
              that->mark_exceptional_finish();
            }
          }
        };

        //////////////////////////
        /// future_deferred_shared_state
        //////////////////////////
        template<typename Rp, typename Fp>
        struct future_deferred_shared_state: shared_state<Rp>
        {
          typedef shared_state<Rp> base_type;
          Fp func_;

        public:
          explicit future_deferred_shared_state(HBOOST_THREAD_FWD_REF(Fp) f)
          : func_(hboost::move(f))
          {
            this->set_deferred();
          }

          virtual void execute(hboost::unique_lock<hboost::mutex>& lck) {
            try
            {
              Fp local_fuct=hboost::move(func_);
              relocker relock(lck);
              Rp res = local_fuct();
              relock.lock();
              this->mark_finished_with_result_internal(hboost::move(res), lck);
            }
            catch (...)
            {
              this->mark_exceptional_finish_internal(current_exception(), lck);
            }
          }
        };
        template<typename Rp, typename Fp>
        struct future_deferred_shared_state<Rp&,Fp>: shared_state<Rp&>
        {
          typedef shared_state<Rp&> base_type;
          Fp func_;

        public:
          explicit future_deferred_shared_state(HBOOST_THREAD_FWD_REF(Fp) f)
          : func_(hboost::move(f))
          {
            this->set_deferred();
          }

          virtual void execute(hboost::unique_lock<hboost::mutex>& lck) {
            try
            {
              this->mark_finished_with_result_internal(func_(), lck);
            }
            catch (...)
            {
              this->mark_exceptional_finish_internal(current_exception(), lck);
            }
          }
        };

        template<typename Fp>
        struct future_deferred_shared_state<void,Fp>: shared_state<void>
        {
          typedef shared_state<void> base_type;
          Fp func_;

        public:
          explicit future_deferred_shared_state(HBOOST_THREAD_FWD_REF(Fp) f)
          : func_(hboost::move(f))
          {
            this->set_deferred();
          }

          virtual void execute(hboost::unique_lock<hboost::mutex>& lck) {
            try
            {
              Fp local_fuct=hboost::move(func_);
              relocker relock(lck);
              local_fuct();
              relock.lock();
              this->mark_finished_with_result_internal(lck);
            }
            catch (...)
            {
              this->mark_exceptional_finish_internal(current_exception(), lck);
            }
          }
        };

        class future_waiter
        {
        public:
            typedef std::vector<int>::size_type count_type;
        private:
            struct registered_waiter;
            struct registered_waiter
            {
                hboost::shared_ptr<detail::shared_state_base> future_;
                detail::shared_state_base::notify_when_ready_handle handle;
                count_type index;

                registered_waiter(hboost::shared_ptr<detail::shared_state_base> const& a_future,
                                  detail::shared_state_base::notify_when_ready_handle handle_,
                                  count_type index_):
                    future_(a_future),handle(handle_),index(index_)
                {}
            };

            struct all_futures_lock
            {
#ifdef _MANAGED
                   typedef std::ptrdiff_t count_type_portable;
#else
                   typedef count_type count_type_portable;
#endif
                   count_type_portable count;
                   hboost::scoped_array<hboost::unique_lock<hboost::mutex> > locks;

                all_futures_lock(std::vector<registered_waiter>& futures):
                    count(futures.size()),locks(new hboost::unique_lock<hboost::mutex>[count])
                {
                    for(count_type_portable i=0;i<count;++i)
                    {
                        locks[i]=HBOOST_THREAD_MAKE_RV_REF(hboost::unique_lock<hboost::mutex>(futures[i].future_->mutex));
                    }
                }

                void lock()
                {
                    hboost::lock(locks.get(),locks.get()+count);
                }

                void unlock()
                {
                    for(count_type_portable i=0;i<count;++i)
                    {
                        locks[i].unlock();
                    }
                }
            };

            hboost::condition_variable_any cv;
            std::vector<registered_waiter> futures_;
            count_type future_count;

        public:
            future_waiter():
                future_count(0)
            {}

            template<typename F>
            void add(F& f)
            {
                if(f.future_)
                {
                  registered_waiter waiter(f.future_,f.future_->notify_when_ready(cv),future_count);
                  try {
                    futures_.push_back(waiter);
                  } catch(...) {
                    f.future_->unnotify_when_ready(waiter.handle);
                    throw;
                  }
                }
                ++future_count;
            }

#ifndef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
            template<typename F1, typename... Fs>
            void add(F1& f1, Fs&... fs)
            {
              add(f1); add(fs...);
            }
#endif

            count_type wait()
            {
                all_futures_lock lk(futures_);
                for(;;)
                {
                    for(count_type i=0;i<futures_.size();++i)
                    {
                        if(futures_[i].future_->done)
                        {
                            return futures_[i].index;
                        }
                    }
                    cv.wait(lk);
                }
            }

            ~future_waiter()
            {
                for(count_type i=0;i<futures_.size();++i)
                {
                    futures_[i].future_->unnotify_when_ready(futures_[i].handle);
                }
            }
        };

    }

    template <typename R>
    class HBOOST_THREAD_FUTURE;

    template <typename R>
    class shared_future;

    template<typename T>
    struct is_future_type<HBOOST_THREAD_FUTURE<T> > : true_type
    {
    };

    template<typename T>
    struct is_future_type<shared_future<T> > : true_type
    {
    };

//    template<typename Iterator>
//    typename hboost::disable_if<is_future_type<Iterator>,Iterator>::type wait_for_any(Iterator begin,Iterator end)
//    {
//        if(begin==end)
//            return end;
//
//        detail::future_waiter waiter;
//        for(Iterator current=begin;current!=end;++current)
//        {
//            waiter.add(*current);
//        }
//        return hboost::next(begin,waiter.wait());
//    }

#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
    template<typename F1,typename F2>
    typename hboost::enable_if<is_future_type<F1>,typename detail::future_waiter::count_type>::type wait_for_any(F1& f1,F2& f2)
    {
        detail::future_waiter waiter;
        waiter.add(f1);
        waiter.add(f2);
        return waiter.wait();
    }

    template<typename F1,typename F2,typename F3>
    typename detail::future_waiter::count_type wait_for_any(F1& f1,F2& f2,F3& f3)
    {
        detail::future_waiter waiter;
        waiter.add(f1);
        waiter.add(f2);
        waiter.add(f3);
        return waiter.wait();
    }

    template<typename F1,typename F2,typename F3,typename F4>
    typename detail::future_waiter::count_type wait_for_any(F1& f1,F2& f2,F3& f3,F4& f4)
    {
        detail::future_waiter waiter;
        waiter.add(f1);
        waiter.add(f2);
        waiter.add(f3);
        waiter.add(f4);
        return waiter.wait();
    }

    template<typename F1,typename F2,typename F3,typename F4,typename F5>
    typename detail::future_waiter::count_type wait_for_any(F1& f1,F2& f2,F3& f3,F4& f4,F5& f5)
    {
        detail::future_waiter waiter;
        waiter.add(f1);
        waiter.add(f2);
        waiter.add(f3);
        waiter.add(f4);
        waiter.add(f5);
        return waiter.wait();
    }
#else
    template<typename F1, typename... Fs>
    typename hboost::enable_if<is_future_type<F1>, typename detail::future_waiter::count_type>::type
    wait_for_any(F1& f1, Fs&... fs)
    {
      detail::future_waiter waiter;
      waiter.add(f1, fs...);
      return waiter.wait();
    }
#endif // !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

    template <typename R>
    class promise;

    template <typename R>
    class packaged_task;

    namespace detail
    {
      /// Common implementation for all the futures independently of the return type
      class base_future
      {
      public:
      };
      /// Common implementation for future and shared_future.
      template <typename R>
      class basic_future : public base_future
      {
      protected:
      public:

        typedef hboost::shared_ptr<detail::shared_state<R> > future_ptr;
        typedef typename detail::shared_state<R>::move_dest_type move_dest_type;

        static //HBOOST_CONSTEXPR
        future_ptr make_exceptional_future_ptr(exceptional_ptr const& ex) {
          return future_ptr(new detail::shared_state<R>(ex));
        }

        future_ptr future_;

        basic_future(future_ptr a_future):
          future_(a_future)
        {
        }

      public:
        typedef future_state::state state;

        HBOOST_THREAD_MOVABLE_ONLY(basic_future)
        basic_future(): future_() {}


        //HBOOST_CONSTEXPR
        basic_future(exceptional_ptr const& ex)
          : future_(make_exceptional_future_ptr(ex))
        {
        }

        ~basic_future() {
        }

        basic_future(HBOOST_THREAD_RV_REF(basic_future) other) HBOOST_NOEXCEPT:
        future_(HBOOST_THREAD_RV(other).future_)
        {
            HBOOST_THREAD_RV(other).future_.reset();
        }
        basic_future& operator=(HBOOST_THREAD_RV_REF(basic_future) other) HBOOST_NOEXCEPT
        {
            future_=HBOOST_THREAD_RV(other).future_;
            HBOOST_THREAD_RV(other).future_.reset();
            return *this;
        }
        void swap(basic_future& that) HBOOST_NOEXCEPT
        {
          future_.swap(that.future_);
        }
        // functions to check state, and wait for ready
        state get_state(hboost::unique_lock<hboost::mutex>& lk) const
        {
            if(!future_)
            {
                return future_state::uninitialized;
            }
            return future_->get_state(lk);
        }
        state get_state() const
        {
            if(!future_)
            {
                return future_state::uninitialized;
            }
            return future_->get_state();
        }

        bool is_ready() const
        {
            return get_state()==future_state::ready;
        }

        bool is_ready(hboost::unique_lock<hboost::mutex>& lk) const
        {
            return get_state(lk)==future_state::ready;
        }
        bool has_exception() const
        {
            return future_ && future_->has_exception();
        }

        bool has_value() const
        {
            return future_ && future_->has_value();
        }

        launch launch_policy(hboost::unique_lock<hboost::mutex>& lk) const
        {
            if ( future_ ) return future_->launch_policy(lk);
            else return launch(launch::none);
        }

        launch launch_policy() const
        {
          if ( future_ ) {
            hboost::unique_lock<hboost::mutex> lk(this->future_->mutex);
            return future_->launch_policy(lk);
          }
          else return launch(launch::none);
        }

        exception_ptr get_exception_ptr()
        {
            return future_
                ? future_->get_exception_ptr()
                : exception_ptr();
        }

        bool valid() const HBOOST_NOEXCEPT
        {
            return future_ != 0 && future_->valid();
        }

        void wait() const
        {
            if(!future_)
            {
                hboost::throw_exception(future_uninitialized());
            }
            future_->wait(false);
        }

        typedef detail::shared_state_base::notify_when_ready_handle notify_when_ready_handle;

        hboost::mutex& mutex() {
          if(!future_)
          {
              hboost::throw_exception(future_uninitialized());
          }
          return future_->mutex;
        };

        notify_when_ready_handle notify_when_ready(hboost::condition_variable_any& cv)
        {
          if(!future_)
          {
              hboost::throw_exception(future_uninitialized());
          }
          return future_->notify_when_ready(cv);
        }

        void unnotify_when_ready(notify_when_ready_handle h)
        {
          if(!future_)
          {
              hboost::throw_exception(future_uninitialized());
          }
          return future_->unnotify_when_ready(h);
        }

#if defined HBOOST_THREAD_USES_DATETIME
        template<typename Duration>
        bool timed_wait(Duration const& rel_time) const
        {
            return timed_wait_until(hboost::get_system_time()+rel_time);
        }

        bool timed_wait_until(hboost::system_time const& abs_time) const
        {
            if(!future_)
            {
                hboost::throw_exception(future_uninitialized());
            }
            return future_->timed_wait_until(abs_time);
        }
#endif
#ifdef HBOOST_THREAD_USES_CHRONO
        template <class Rep, class Period>
        future_status
        wait_for(const chrono::duration<Rep, Period>& rel_time) const
        {
          return wait_until(chrono::steady_clock::now() + rel_time);

        }
        template <class Clock, class Duration>
        future_status
        wait_until(const chrono::time_point<Clock, Duration>& abs_time) const
        {
          if(!future_)
          {
              hboost::throw_exception(future_uninitialized());
          }
          return future_->wait_until(abs_time);
        }
#endif

      };

    } // detail
    HBOOST_THREAD_DCL_MOVABLE_BEG(R) detail::basic_future<R> HBOOST_THREAD_DCL_MOVABLE_END

    namespace detail
    {
#if (!defined _MSC_VER || _MSC_VER >= 1400) // _MSC_VER == 1400 on MSVC 2005
        template <class Rp, class Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_future_async_shared_state(HBOOST_THREAD_FWD_REF(Fp) f);

        template <class Rp, class Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_future_deferred_shared_state(HBOOST_THREAD_FWD_REF(Fp) f);
#endif // #if (!defined _MSC_VER || _MSC_VER >= 1400)
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
        template<typename F, typename Rp, typename Fp>
        struct future_deferred_continuation_shared_state;
        template<typename F, typename Rp, typename Fp>
        struct future_async_continuation_shared_state;

        template <class F, class Rp, class Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_future_async_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template <class F, class Rp, class Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_future_sync_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template <class F, class Rp, class Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_future_deferred_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template<typename F, typename Rp, typename Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_shared_future_deferred_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

        template<typename F, typename Rp, typename Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_shared_future_async_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

        template<typename F, typename Rp, typename Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_shared_future_sync_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);


  #ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        template<typename Ex, typename F, typename Rp, typename Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_future_executor_continuation_shared_state(Ex& ex, hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template<typename Ex, typename F, typename Rp, typename Fp>
        HBOOST_THREAD_FUTURE<Rp>
        make_shared_future_executor_continuation_shared_state(Ex& ex, hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

        template <class Rp, class Fp, class Executor>
        HBOOST_THREAD_FUTURE<Rp>
        make_future_executor_shared_state(Executor& ex, HBOOST_THREAD_FWD_REF(Fp) f);
  #endif
#endif
#if defined HBOOST_THREAD_PROVIDES_FUTURE_UNWRAP
        template<typename F, typename Rp>
        struct future_unwrap_shared_state;
        template <class F, class Rp>
        inline HBOOST_THREAD_FUTURE<Rp>
        make_future_unwrap_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f);
#endif
    }
#if defined(HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY)
      template< typename InputIterator>
      typename hboost::disable_if<is_future_type<InputIterator>,
        HBOOST_THREAD_FUTURE<csbl::vector<typename InputIterator::value_type>  >
      >::type
      when_all(InputIterator first, InputIterator last);

      inline HBOOST_THREAD_FUTURE<csbl::tuple<> > when_all();

    #if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
      template< typename T0, typename ...T>
      HBOOST_THREAD_FUTURE<csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> >
      when_all(HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures);
    #endif

      template< typename InputIterator>
      typename hboost::disable_if<is_future_type<InputIterator>,
        HBOOST_THREAD_FUTURE<csbl::vector<typename InputIterator::value_type>  >
      >::type
      when_any(InputIterator first, InputIterator last);

      inline HBOOST_THREAD_FUTURE<csbl::tuple<> > when_any();

    #if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
      template< typename T0, typename ...T>
      HBOOST_THREAD_FUTURE<csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> >
      when_any(HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures);
    #endif
#endif // HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY


    template <typename R>
    class HBOOST_THREAD_FUTURE : public detail::basic_future<R>
    {
    private:
        typedef detail::basic_future<R> base_type;
        typedef typename base_type::future_ptr future_ptr;

        friend class shared_future<R>;
        friend class promise<R>;
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
        template <typename, typename, typename>
        friend struct detail::future_async_continuation_shared_state;
        template <typename, typename, typename>
        friend struct detail::future_deferred_continuation_shared_state;

        template <class F, class Rp, class Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_async_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template <class F, class Rp, class Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_sync_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template <class F, class Rp, class Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_deferred_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template<typename F, typename Rp, typename Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_shared_future_deferred_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

        template<typename F, typename Rp, typename Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_shared_future_async_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

        template<typename F, typename Rp, typename Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_shared_future_sync_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

  #ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        template<typename Ex, typename F, typename Rp, typename Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_executor_continuation_shared_state(Ex& ex, hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template<typename Ex, typename F, typename Rp, typename Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_shared_future_executor_continuation_shared_state(Ex& ex, hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

        template <class Rp, class Fp, class Executor>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_executor_shared_state(Executor& ex, HBOOST_THREAD_FWD_REF(Fp) f);
  #endif
#endif
#if defined HBOOST_THREAD_PROVIDES_FUTURE_UNWRAP
        template<typename F, typename Rp>
        friend struct detail::future_unwrap_shared_state;
        template <class F, class Rp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_unwrap_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f);
#endif
#if defined(HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY)
      template< typename InputIterator>
      friend typename hboost::disable_if<is_future_type<InputIterator>,
        HBOOST_THREAD_FUTURE<csbl::vector<typename InputIterator::value_type>  >
      >::type
      when_all(InputIterator first, InputIterator last);

      //friend inline HBOOST_THREAD_FUTURE<csbl::tuple<> > when_all();

    #if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
      template< typename T0, typename ...T>
      friend HBOOST_THREAD_FUTURE<csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> >
      when_all(HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures);
    #endif

      template< typename InputIterator>
      friend typename hboost::disable_if<is_future_type<InputIterator>,
        HBOOST_THREAD_FUTURE<csbl::vector<typename InputIterator::value_type>  >
      >::type
      when_any(InputIterator first, InputIterator last);

      //friend inline HBOOST_THREAD_FUTURE<csbl::tuple<> > when_any();

    #if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
      template< typename T0, typename ...T>
      friend HBOOST_THREAD_FUTURE<csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> >
      when_any(HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures);
    #endif
#endif // HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
        template <class> friend class packaged_task; // todo check if this works in windows
#else
        friend class packaged_task<R>;
#endif
        friend class detail::future_waiter;

        template <class Rp, class Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_async_shared_state(HBOOST_THREAD_FWD_REF(Fp) f);

        template <class Rp, class Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_deferred_shared_state(HBOOST_THREAD_FWD_REF(Fp) f);

        typedef typename base_type::move_dest_type move_dest_type;

        HBOOST_THREAD_FUTURE(future_ptr a_future):
          base_type(a_future)
        {
        }

    public:
        HBOOST_THREAD_MOVABLE_ONLY(HBOOST_THREAD_FUTURE)
        typedef future_state::state state;
        typedef R value_type; // EXTENSION

        HBOOST_CONSTEXPR HBOOST_THREAD_FUTURE() {}
        //HBOOST_CONSTEXPR
        HBOOST_THREAD_FUTURE(exceptional_ptr const& ex):
            base_type(ex) {}

        ~HBOOST_THREAD_FUTURE() {
        }

        HBOOST_THREAD_FUTURE(HBOOST_THREAD_RV_REF(HBOOST_THREAD_FUTURE) other) HBOOST_NOEXCEPT:
        base_type(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))))
        {
        }
        inline HBOOST_THREAD_FUTURE(HBOOST_THREAD_RV_REF(HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R> >) other); // EXTENSION

        explicit HBOOST_THREAD_FUTURE(HBOOST_THREAD_RV_REF(shared_future<R>) other) :
        base_type(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))))
        {}

        HBOOST_THREAD_FUTURE& operator=(HBOOST_THREAD_RV_REF(HBOOST_THREAD_FUTURE) other) HBOOST_NOEXCEPT
        {
            this->base_type::operator=(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))));
            return *this;
        }

        shared_future<R> share()
        {
          return shared_future<R>(::hboost::move(*this));
        }

        void swap(HBOOST_THREAD_FUTURE& other)
        {
            static_cast<base_type*>(this)->swap(other);
        }

        // todo this function must be private and friendship provided to the internal users.
        void set_async()
        {
          this->future_->set_async();
        }
        // todo this function must be private and friendship provided to the internal users.
        void set_deferred()
        {
          this->future_->set_deferred();
        }
        bool run_if_is_deferred() {
          return this->future_->run_if_is_deferred();
        }
        bool run_if_is_deferred_or_ready() {
          return this->future_->run_if_is_deferred_or_ready();
        }
        // retrieving the value
        move_dest_type get()
        {
            if (this->future_ == 0)
            {
                hboost::throw_exception(future_uninitialized());
            }
            unique_lock<hboost::mutex> lk(this->future_->mutex);
            if (! this->future_->valid(lk))
            {
                hboost::throw_exception(future_uninitialized());
            }
#ifdef HBOOST_THREAD_PROVIDES_FUTURE_INVALID_AFTER_GET
            this->future_->invalidate(lk);
#endif
            return this->future_->get(lk);
        }

        template <typename R2>
        typename hboost::disable_if< is_void<R2>, move_dest_type>::type
        get_or(HBOOST_THREAD_RV_REF(R2) v)
        {

            if (this->future_ == 0)
            {
                hboost::throw_exception(future_uninitialized());
            }
            unique_lock<hboost::mutex> lk(this->future_->mutex);
            if (! this->future_->valid(lk))
            {
                hboost::throw_exception(future_uninitialized());
            }
            this->future_->wait(lk, false);
#ifdef HBOOST_THREAD_PROVIDES_FUTURE_INVALID_AFTER_GET
            this->future_->invalidate(lk);
#endif

            if (this->future_->has_value(lk)) {
              return this->future_->get(lk);
            }
            else {
              return hboost::move(v);
            }
        }

        template <typename R2>
        typename hboost::disable_if< is_void<R2>, move_dest_type>::type
        get_or(R2 const& v)  // EXTENSION
        {
            if (this->future_ == 0)
            {
                hboost::throw_exception(future_uninitialized());
            }
            unique_lock<hboost::mutex> lk(this->future_->mutex);
            if (! this->future_->valid(lk))
            {
                hboost::throw_exception(future_uninitialized());
            }
            this->future_->wait(lk, false);
#ifdef HBOOST_THREAD_PROVIDES_FUTURE_INVALID_AFTER_GET
            this->future_->invalidate(lk);
#endif
            if (this->future_->has_value(lk)) {
              return this->future_->get(lk);
            }
            else {
              return v;
            }
        }

#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
        template<typename F>
        inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE)>::type>
        then(HBOOST_THREAD_FWD_REF(F) func);  // EXTENSION
        template<typename F>
        inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE)>::type>
        then(launch policy, HBOOST_THREAD_FWD_REF(F) func);  // EXTENSION
  #ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        template<typename Ex, typename F>
        inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE)>::type>
        then(Ex& ex, HBOOST_THREAD_FWD_REF(F) func);  // EXTENSION
  #endif

        template <typename R2>
        inline typename hboost::disable_if< is_void<R2>, HBOOST_THREAD_FUTURE<R> >::type
        fallback_to(HBOOST_THREAD_RV_REF(R2) v);  // EXTENSION
        template <typename R2>
        inline typename hboost::disable_if< is_void<R2>, HBOOST_THREAD_FUTURE<R> >::type
        fallback_to(R2 const& v);  // EXTENSION

#endif

    };

    HBOOST_THREAD_DCL_MOVABLE_BEG(T) HBOOST_THREAD_FUTURE<T> HBOOST_THREAD_DCL_MOVABLE_END

        template <typename R2>
        class HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> > : public detail::basic_future<HBOOST_THREAD_FUTURE<R2> >
        {
          typedef HBOOST_THREAD_FUTURE<R2> R;

        private:
            typedef detail::basic_future<R> base_type;
            typedef typename base_type::future_ptr future_ptr;

            friend class shared_future<R>;
            friend class promise<R>;
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
            template <typename, typename, typename>
            friend struct detail::future_async_continuation_shared_state;
            template <typename, typename, typename>
            friend struct detail::future_deferred_continuation_shared_state;

            template <class F, class Rp, class Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_future_async_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

            template <class F, class Rp, class Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_future_sync_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

            template <class F, class Rp, class Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_future_deferred_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

            template<typename F, typename Rp, typename Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_shared_future_deferred_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

            template<typename F, typename Rp, typename Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_shared_future_async_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

            template<typename F, typename Rp, typename Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_shared_future_sync_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

      #ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
            template<typename Ex, typename F, typename Rp, typename Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_future_executor_continuation_shared_state(Ex& ex, hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

            template<typename Ex, typename F, typename Rp, typename Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_shared_future_executor_continuation_shared_state(Ex& ex, hboost::unique_lock<hboost::mutex> &lock, F f, HBOOST_THREAD_FWD_REF(Fp) c);

            template <class Rp, class Fp, class Executor>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_future_executor_shared_state(Executor& ex, HBOOST_THREAD_FWD_REF(Fp) f);
      #endif

#endif
#if defined HBOOST_THREAD_PROVIDES_FUTURE_UNWRAP
            template<typename F, typename Rp>
            friend struct detail::future_unwrap_shared_state;
        template <class F, class Rp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_unwrap_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f);
#endif
#if defined(HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY)
      template< typename InputIterator>
      friend typename hboost::disable_if<is_future_type<InputIterator>,
        HBOOST_THREAD_FUTURE<csbl::vector<typename InputIterator::value_type>  >
      >::type
      when_all(InputIterator first, InputIterator last);

      friend inline HBOOST_THREAD_FUTURE<csbl::tuple<> > when_all();

    #if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
      template< typename T0, typename ...T>
      friend HBOOST_THREAD_FUTURE<csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> >
      when_all(HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures);
    #endif

      template< typename InputIterator>
      friend typename hboost::disable_if<is_future_type<InputIterator>,
        HBOOST_THREAD_FUTURE<csbl::vector<typename InputIterator::value_type>  >
      >::type
      when_any(InputIterator first, InputIterator last);

      friend inline HBOOST_THREAD_FUTURE<csbl::tuple<> > when_any();

    #if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
      template< typename T0, typename ...T>
      friend HBOOST_THREAD_FUTURE<csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> >
      when_any(HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures);
    #endif
#endif // HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY

    #if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
            template <class> friend class packaged_task; // todo check if this works in windows
    #else
            friend class packaged_task<R>;
    #endif
            friend class detail::future_waiter;

            template <class Rp, class Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_future_async_shared_state(HBOOST_THREAD_FWD_REF(Fp) f);

            template <class Rp, class Fp>
            friend HBOOST_THREAD_FUTURE<Rp>
            detail::make_future_deferred_shared_state(HBOOST_THREAD_FWD_REF(Fp) f);

            typedef typename base_type::move_dest_type move_dest_type;

            HBOOST_THREAD_FUTURE(future_ptr a_future):
              base_type(a_future)
            {
            }
        public:

            HBOOST_THREAD_MOVABLE_ONLY(HBOOST_THREAD_FUTURE)
            typedef future_state::state state;
            typedef R value_type; // EXTENSION

            HBOOST_CONSTEXPR HBOOST_THREAD_FUTURE() {}
            //HBOOST_CONSTEXPR
            HBOOST_THREAD_FUTURE(exceptional_ptr const& ex):
                base_type(ex) {}

            ~HBOOST_THREAD_FUTURE() {
            }

            HBOOST_THREAD_FUTURE(HBOOST_THREAD_RV_REF(HBOOST_THREAD_FUTURE) other) HBOOST_NOEXCEPT:
            base_type(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))))
            {
            }

            HBOOST_THREAD_FUTURE& operator=(HBOOST_THREAD_RV_REF(HBOOST_THREAD_FUTURE) other) HBOOST_NOEXCEPT
            {
                this->base_type::operator=(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))));
                return *this;
            }

            shared_future<R> share()
            {
              return shared_future<R>(::hboost::move(*this));
            }

            void swap(HBOOST_THREAD_FUTURE& other)
            {
                static_cast<base_type*>(this)->swap(other);
            }

            // todo this function must be private and friendship provided to the internal users.
            void set_async()
            {
              this->future_->set_async();
            }
            // todo this function must be private and friendship provided to the internal users.
            void set_deferred()
            {
              this->future_->set_deferred();
            }
            bool run_if_is_deferred() {
              return this->future_->run_if_is_deferred();
            }
            bool run_if_is_deferred_or_ready() {
              return this->future_->run_if_is_deferred_or_ready();
            }
            // retrieving the value
            move_dest_type get()
            {
                if (this->future_ == 0)
                {
                    hboost::throw_exception(future_uninitialized());
                }
                unique_lock<hboost::mutex> lk(this->future_->mutex);
                if (! this->future_->valid(lk))
                {
                    hboost::throw_exception(future_uninitialized());
                }
    #ifdef HBOOST_THREAD_PROVIDES_FUTURE_INVALID_AFTER_GET
                this->future_->invalidate(lk);
    #endif
                return this->future_->get(lk);
            }
            move_dest_type get_or(HBOOST_THREAD_RV_REF(R) v) // EXTENSION
            {
                if (this->future_ == 0)
                {
                    hboost::throw_exception(future_uninitialized());
                }
                unique_lock<hboost::mutex> lk(this->future_->mutex);
                if (! this->future_->valid(lk))
                {
                    hboost::throw_exception(future_uninitialized());
                }
                this->future_->wait(lk, false);
    #ifdef HBOOST_THREAD_PROVIDES_FUTURE_INVALID_AFTER_GET
                this->future_->invalidate(lk);
    #endif
                if (this->future_->has_value(lk)) return this->future_->get(lk);
                else return hboost::move(v);
            }

            move_dest_type get_or(R const& v) // EXTENSION
            {
                if (this->future_ == 0)
                {
                    hboost::throw_exception(future_uninitialized());
                }
                unique_lock<hboost::mutex> lk(this->future_->mutex);
                if (! this->future_->valid(lk))
                {
                    hboost::throw_exception(future_uninitialized());
                }
                this->future_->wait(lk, false);
    #ifdef HBOOST_THREAD_PROVIDES_FUTURE_INVALID_AFTER_GET
                this->future_->invalidate(lk);
    #endif
                if (this->future_->has_value(lk)) return this->future_->get(lk);
                else return v;
            }


    #if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
            template<typename F>
            inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE)>::type>
            then(HBOOST_THREAD_FWD_REF(F) func); // EXTENSION
            template<typename F>
            inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE)>::type>
            then(launch policy, HBOOST_THREAD_FWD_REF(F) func); // EXTENSION
      #ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
            template<typename Ex, typename F>
            inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE)>::type>
            then(Ex &ex, HBOOST_THREAD_FWD_REF(F) func); // EXTENSION
      #endif
    #endif

    #if defined HBOOST_THREAD_PROVIDES_FUTURE_UNWRAP
            inline
            HBOOST_THREAD_FUTURE<R2>
            unwrap(); // EXTENSION
    #endif

  };

    template <typename R>
    class shared_future : public detail::basic_future<R>
    {
        typedef detail::basic_future<R> base_type;
        typedef typename base_type::future_ptr future_ptr;

        friend class detail::future_waiter;
        friend class promise<R>;

#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
        template <typename, typename, typename>
        friend struct detail::future_async_continuation_shared_state;
        template <typename, typename, typename>
        friend struct detail::future_deferred_continuation_shared_state;

        template <class F, class Rp, class Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_async_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template <class F, class Rp, class Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_sync_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);

        template <class F, class Rp, class Fp>
        friend HBOOST_THREAD_FUTURE<Rp>
        detail::make_future_deferred_continuation_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c);
#endif
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
        template <class> friend class packaged_task;// todo check if this works in windows
#else
        friend class packaged_task<R>;
#endif
        shared_future(future_ptr a_future):
          base_type(a_future)
        {}

    public:
        HBOOST_THREAD_COPYABLE_AND_MOVABLE(shared_future)
        typedef R value_type; // EXTENSION

        shared_future(shared_future const& other):
        base_type(other.future_)
        {}

        typedef future_state::state state;

        HBOOST_CONSTEXPR shared_future()
        {}
        //HBOOST_CONSTEXPR
        shared_future(exceptional_ptr const& ex):
            base_type(ex) {}
        ~shared_future()
        {}

        shared_future& operator=(HBOOST_THREAD_COPY_ASSIGN_REF(shared_future) other)
        {
            this->future_ = other.future_;
            return *this;
        }

        shared_future(HBOOST_THREAD_RV_REF(shared_future) other) HBOOST_NOEXCEPT :
        base_type(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))))
        {
        }
        shared_future(HBOOST_THREAD_RV_REF( HBOOST_THREAD_FUTURE<R> ) other) HBOOST_NOEXCEPT :
        base_type(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))))
        {
        }

        shared_future& operator=(HBOOST_THREAD_RV_REF(shared_future) other) HBOOST_NOEXCEPT
        {
            base_type::operator=(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))));
            return *this;
        }
        shared_future& operator=(HBOOST_THREAD_RV_REF( HBOOST_THREAD_FUTURE<R> ) other) HBOOST_NOEXCEPT
        {
            base_type::operator=(hboost::move(static_cast<base_type&>(HBOOST_THREAD_RV(other))));
            return *this;
        }

        void swap(shared_future& other) HBOOST_NOEXCEPT
        {
            static_cast<base_type*>(this)->swap(other);
        }
        bool run_if_is_deferred() {
          return this->future_->run_if_is_deferred();
        }
        bool run_if_is_deferred_or_ready() {
          return this->future_->run_if_is_deferred_or_ready();
        }
        // retrieving the value
        typename detail::shared_state<R>::shared_future_get_result_type get() const
        {
            if(!this->future_)
            {
                hboost::throw_exception(future_uninitialized());
            }
            return this->future_->get_sh();
        }

        template <typename R2>
        typename hboost::disable_if< is_void<R2>, typename detail::shared_state<R>::shared_future_get_result_type>::type
        get_or(HBOOST_THREAD_RV_REF(R2) v)  const // EXTENSION
        {
            if(!this->future_)
            {
                hboost::throw_exception(future_uninitialized());
            }
            this->future_->wait();
            if (this->future_->has_value()) return this->future_->get_sh();
            else return hboost::move(v);
        }

#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
        template<typename F>
        inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(shared_future)>::type>
        then(HBOOST_THREAD_FWD_REF(F) func) const; // EXTENSION
        template<typename F>
        inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(shared_future)>::type>
        then(launch policy, HBOOST_THREAD_FWD_REF(F) func) const; // EXTENSION
  #ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        template<typename Ex, typename F>
        inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(shared_future)>::type>
        then(Ex& ex, HBOOST_THREAD_FWD_REF(F) func) const; // EXTENSION
  #endif
#endif

    };

    HBOOST_THREAD_DCL_MOVABLE_BEG(T) shared_future<T> HBOOST_THREAD_DCL_MOVABLE_END

    template <typename R>
    class promise
    {
        typedef hboost::shared_ptr<detail::shared_state<R> > future_ptr;

        typedef typename detail::shared_state<R>::source_reference_type source_reference_type;
        typedef typename detail::shared_state<R>::rvalue_source_type rvalue_source_type;
        typedef typename detail::shared_state<R>::move_dest_type move_dest_type;
        typedef typename detail::shared_state<R>::shared_future_get_result_type shared_future_get_result_type;

        future_ptr future_;
        bool future_obtained;

        void lazy_init()
        {
#if defined HBOOST_THREAD_PROVIDES_PROMISE_LAZY
#include <hboost/detail/atomic_undef_macros.hpp>
          if(!atomic_load(&future_))
            {
                future_ptr blank;
                atomic_compare_exchange(&future_,&blank,future_ptr(new detail::shared_state<R>));
            }
#include <hboost/detail/atomic_redef_macros.hpp>
#endif
        }

    public:
        HBOOST_THREAD_MOVABLE_ONLY(promise)
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
        template <class Allocator>
        promise(hboost::allocator_arg_t, Allocator a)
        {
          typedef typename Allocator::template rebind<detail::shared_state<R> >::other A2;
          A2 a2(a);
          typedef thread_detail::allocator_destructor<A2> D;

          future_ = future_ptr(::new(a2.allocate(1)) detail::shared_state<R>(), D(a2, 1) );
          future_obtained = false;
        }
#endif
        promise():
#if defined HBOOST_THREAD_PROVIDES_PROMISE_LAZY
            future_(),
#else
            future_(new detail::shared_state<R>()),
#endif
            future_obtained(false)
        {}

        ~promise()
        {
            if(future_)
            {
                hboost::unique_lock<hboost::mutex> lock(future_->mutex);

                if(!future_->done && !future_->is_constructed)
                {
                    future_->mark_exceptional_finish_internal(hboost::copy_exception(broken_promise()), lock);
                }
            }
        }

        // Assignment
        promise(HBOOST_THREAD_RV_REF(promise) rhs) HBOOST_NOEXCEPT :
            future_(HBOOST_THREAD_RV(rhs).future_),future_obtained(HBOOST_THREAD_RV(rhs).future_obtained)
        {
            HBOOST_THREAD_RV(rhs).future_.reset();
            HBOOST_THREAD_RV(rhs).future_obtained=false;
        }
        promise & operator=(HBOOST_THREAD_RV_REF(promise) rhs) HBOOST_NOEXCEPT
        {
            future_=HBOOST_THREAD_RV(rhs).future_;
            future_obtained=HBOOST_THREAD_RV(rhs).future_obtained;
            HBOOST_THREAD_RV(rhs).future_.reset();
            HBOOST_THREAD_RV(rhs).future_obtained=false;
            return *this;
        }

        void swap(promise& other)
        {
            future_.swap(other.future_);
            std::swap(future_obtained,other.future_obtained);
        }

#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        void set_executor(executor_ptr_type aex)
        {
          lazy_init();
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          hboost::lock_guard<hboost::mutex> lk(future_->mutex);
          future_->set_executor_policy(aex, lk);
        }
#endif
        // Result retrieval
        HBOOST_THREAD_FUTURE<R> get_future()
        {
            lazy_init();
            if (future_.get()==0)
            {
                hboost::throw_exception(promise_moved());
            }
            if (future_obtained)
            {
                hboost::throw_exception(future_already_retrieved());
            }
            future_obtained=true;
            return HBOOST_THREAD_FUTURE<R>(future_);
        }

#if defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
        template <class TR>
        typename hboost::enable_if_c<is_copy_constructible<TR>::value && is_same<R, TR>::value, void>::type set_value(TR const &  r)
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
            future_->mark_finished_with_result_internal(r, lock);
        }
#else
        void set_value(source_reference_type r)
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
            future_->mark_finished_with_result_internal(r, lock);
        }
#endif

        void set_value(rvalue_source_type r)
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
            future_->mark_finished_with_result_internal(hboost::move(r), lock);
#else
            future_->mark_finished_with_result_internal(static_cast<rvalue_source_type>(r), lock);
#endif
        }

#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
        template <class ...Args>
        void emplace(HBOOST_THREAD_FWD_REF(Args) ...args)
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
            future_->mark_finished_with_result_internal(lock, hboost::forward<Args>(args)...);
        }

#endif

        void set_exception(hboost::exception_ptr p)
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
            future_->mark_exceptional_finish_internal(p, lock);
        }
        template <typename E>
        void set_exception(E ex)
        {
          set_exception(hboost::copy_exception(ex));
        }
        // setting the result with deferred notification
#if defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
        template <class TR>
        typename hboost::enable_if_c<is_copy_constructible<TR>::value && is_same<R, TR>::value, void>::type set_value_at_thread_exit(TR const& r)
        {
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          future_->set_value_at_thread_exit(r);
        }
#else
        void set_value_at_thread_exit(source_reference_type r)
        {
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          future_->set_value_at_thread_exit(r);
        }
#endif
        void set_value_at_thread_exit(HBOOST_THREAD_RV_REF(R) r)
        {
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          future_->set_value_at_thread_exit(hboost::move(r));
        }
        void set_exception_at_thread_exit(exception_ptr e)
        {
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          future_->set_exception_at_thread_exit(e);
        }
        template <typename E>
        void set_exception_at_thread_exit(E ex)
        {
          set_exception_at_thread_exit(hboost::copy_exception(ex));
        }

        template<typename F>
        void set_wait_callback(F f)
        {
            lazy_init();
            future_->set_wait_callback(f,this);
        }

    };

    template <typename R>
    class promise<R&>
    {
        typedef hboost::shared_ptr<detail::shared_state<R&> > future_ptr;

        future_ptr future_;
        bool future_obtained;

        void lazy_init()
        {
#if defined HBOOST_THREAD_PROVIDES_PROMISE_LAZY
#include <hboost/detail/atomic_undef_macros.hpp>
            if(!atomic_load(&future_))
            {
                future_ptr blank;
                atomic_compare_exchange(&future_,&blank,future_ptr(new detail::shared_state<R&>));
            }
#include <hboost/detail/atomic_redef_macros.hpp>
#endif
        }

    public:
        HBOOST_THREAD_MOVABLE_ONLY(promise)
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
        template <class Allocator>
        promise(hboost::allocator_arg_t, Allocator a)
        {
          typedef typename Allocator::template rebind<detail::shared_state<R&> >::other A2;
          A2 a2(a);
          typedef thread_detail::allocator_destructor<A2> D;

          future_ = future_ptr(::new(a2.allocate(1)) detail::shared_state<R&>(), D(a2, 1) );
          future_obtained = false;
        }
#endif
        promise():
#if defined HBOOST_THREAD_PROVIDES_PROMISE_LAZY
            future_(),
#else
            future_(new detail::shared_state<R&>()),
#endif
            future_obtained(false)
        {}

        ~promise()
        {
            if(future_)
            {
                hboost::unique_lock<hboost::mutex> lock(future_->mutex);

                if(!future_->done && !future_->is_constructed)
                {
                    future_->mark_exceptional_finish_internal(hboost::copy_exception(broken_promise()), lock);
                }
            }
        }

        // Assignment
        promise(HBOOST_THREAD_RV_REF(promise) rhs) HBOOST_NOEXCEPT :
            future_(HBOOST_THREAD_RV(rhs).future_),future_obtained(HBOOST_THREAD_RV(rhs).future_obtained)
        {
            HBOOST_THREAD_RV(rhs).future_.reset();
            HBOOST_THREAD_RV(rhs).future_obtained=false;
        }
        promise & operator=(HBOOST_THREAD_RV_REF(promise) rhs) HBOOST_NOEXCEPT
        {
            future_=HBOOST_THREAD_RV(rhs).future_;
            future_obtained=HBOOST_THREAD_RV(rhs).future_obtained;
            HBOOST_THREAD_RV(rhs).future_.reset();
            HBOOST_THREAD_RV(rhs).future_obtained=false;
            return *this;
        }

        void swap(promise& other)
        {
            future_.swap(other.future_);
            std::swap(future_obtained,other.future_obtained);
        }

        // Result retrieval
        HBOOST_THREAD_FUTURE<R&> get_future()
        {
            lazy_init();
            if (future_.get()==0)
            {
                hboost::throw_exception(promise_moved());
            }
            if (future_obtained)
            {
                hboost::throw_exception(future_already_retrieved());
            }
            future_obtained=true;
            return HBOOST_THREAD_FUTURE<R&>(future_);
        }

        void set_value(R& r)
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
            future_->mark_finished_with_result_internal(r, lock);
        }

        void set_exception(hboost::exception_ptr p)
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
            future_->mark_exceptional_finish_internal(p, lock);
        }
        template <typename E>
        void set_exception(E ex)
        {
          set_exception(hboost::copy_exception(ex));
        }

        // setting the result with deferred notification
        void set_value_at_thread_exit(R& r)
        {
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          future_->set_value_at_thread_exit(r);
        }

        void set_exception_at_thread_exit(exception_ptr e)
        {
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          future_->set_exception_at_thread_exit(e);
        }
        template <typename E>
        void set_exception_at_thread_exit(E ex)
        {
          set_exception_at_thread_exit(hboost::copy_exception(ex));
        }

        template<typename F>
        void set_wait_callback(F f)
        {
            lazy_init();
            future_->set_wait_callback(f,this);
        }
    };

    template <>
    class promise<void>
    {
        typedef hboost::shared_ptr<detail::shared_state<void> > future_ptr;

        future_ptr future_;
        bool future_obtained;

        void lazy_init()
        {
#if defined HBOOST_THREAD_PROVIDES_PROMISE_LAZY
            if(!atomic_load(&future_))
            {
                future_ptr blank;
                atomic_compare_exchange(&future_,&blank,future_ptr(new detail::shared_state<void>));
            }
#endif
        }
    public:
        HBOOST_THREAD_MOVABLE_ONLY(promise)

#if defined HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
        template <class Allocator>
        promise(hboost::allocator_arg_t, Allocator a)
        {
          typedef typename Allocator::template rebind<detail::shared_state<void> >::other A2;
          A2 a2(a);
          typedef thread_detail::allocator_destructor<A2> D;

          future_ = future_ptr(::new(a2.allocate(1)) detail::shared_state<void>(), D(a2, 1) );
          future_obtained = false;
        }
#endif
        promise():
#if defined HBOOST_THREAD_PROVIDES_PROMISE_LAZY
            future_(),
#else
            future_(new detail::shared_state<void>),
#endif
            future_obtained(false)
        {}

        ~promise()
        {
            if(future_)
            {
                hboost::unique_lock<hboost::mutex> lock(future_->mutex);

                if(!future_->done && !future_->is_constructed)
                {
                    future_->mark_exceptional_finish_internal(hboost::copy_exception(broken_promise()), lock);
                }
            }
        }

        // Assignment
        promise(HBOOST_THREAD_RV_REF(promise) rhs) HBOOST_NOEXCEPT :
            future_(HBOOST_THREAD_RV(rhs).future_),future_obtained(HBOOST_THREAD_RV(rhs).future_obtained)
        {
          // we need to release the future as shared_ptr doesn't implements move semantics
            HBOOST_THREAD_RV(rhs).future_.reset();
            HBOOST_THREAD_RV(rhs).future_obtained=false;
        }

        promise & operator=(HBOOST_THREAD_RV_REF(promise) rhs) HBOOST_NOEXCEPT
        {
            future_=HBOOST_THREAD_RV(rhs).future_;
            future_obtained=HBOOST_THREAD_RV(rhs).future_obtained;
            HBOOST_THREAD_RV(rhs).future_.reset();
            HBOOST_THREAD_RV(rhs).future_obtained=false;
            return *this;
        }

        void swap(promise& other)
        {
            future_.swap(other.future_);
            std::swap(future_obtained,other.future_obtained);
        }

        // Result retrieval
        HBOOST_THREAD_FUTURE<void> get_future()
        {
            lazy_init();

            if (future_.get()==0)
            {
                hboost::throw_exception(promise_moved());
            }
            if(future_obtained)
            {
                hboost::throw_exception(future_already_retrieved());
            }
            future_obtained=true;
            //return HBOOST_THREAD_MAKE_RV_REF(HBOOST_THREAD_FUTURE<void>(future_));
            return HBOOST_THREAD_FUTURE<void>(future_);
        }

        void set_value()
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
            future_->mark_finished_with_result_internal(lock);
        }

        void set_exception(hboost::exception_ptr p)
        {
            lazy_init();
            hboost::unique_lock<hboost::mutex> lock(future_->mutex);
            if(future_->done)
            {
                hboost::throw_exception(promise_already_satisfied());
            }
            future_->mark_exceptional_finish_internal(p,lock);
        }
        template <typename E>
        void set_exception(E ex)
        {
          set_exception(hboost::copy_exception(ex));
        }

        // setting the result with deferred notification
        void set_value_at_thread_exit()
        {
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          future_->set_value_at_thread_exit();
        }

        void set_exception_at_thread_exit(exception_ptr e)
        {
          if (future_.get()==0)
          {
              hboost::throw_exception(promise_moved());
          }
          future_->set_exception_at_thread_exit(e);
        }
        template <typename E>
        void set_exception_at_thread_exit(E ex)
        {
          set_exception_at_thread_exit(hboost::copy_exception(ex));
        }

        template<typename F>
        void set_wait_callback(F f)
        {
            lazy_init();
            future_->set_wait_callback(f,this);
        }

    };
}
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
namespace hboost { namespace container {
    template <class R, class Alloc>
    struct uses_allocator< ::hboost::promise<R> , Alloc> : true_type
    {
    };
}}
#if ! defined  HBOOST_NO_CXX11_ALLOCATOR
namespace std {
    template <class R, class Alloc>
    struct uses_allocator< ::hboost::promise<R> , Alloc> : true_type
    {
    };
}
#endif
#endif

namespace hboost
{

    HBOOST_THREAD_DCL_MOVABLE_BEG(T) promise<T> HBOOST_THREAD_DCL_MOVABLE_END

    namespace detail
    {
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
      template<typename R>
      struct task_base_shared_state;
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
      template<typename R, typename ...ArgTypes>
      struct task_base_shared_state<R(ArgTypes...)>:
#else
      template<typename R>
      struct task_base_shared_state<R()>:
#endif
#else
      template<typename R>
      struct task_base_shared_state:
#endif
            detail::shared_state<R>
        {
            bool started;

            task_base_shared_state():
                started(false)
            {}

            void reset()
            {
              // todo The packaged_task::reset must be as if an assignemnt froma new packaged_task with the same function
              // the reset function is an optimization that avoids reallocating a new task.
              started=false;
              this->validate();
            }
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            virtual void do_run(HBOOST_THREAD_RV_REF(ArgTypes) ... args)=0;
            void run(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
#else
            virtual void do_run()=0;
            void run()
#endif
            {
                {
                    hboost::lock_guard<hboost::mutex> lk(this->mutex);
                    if(started)
                    {
                        hboost::throw_exception(task_already_started());
                    }
                    started=true;
                }
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
                do_run(hboost::move(args)...);
#else
                do_run();
#endif
            }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            virtual void do_apply(HBOOST_THREAD_RV_REF(ArgTypes) ... args)=0;
            void apply(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
#else
            virtual void do_apply()=0;
            void apply()
#endif
            {
                {
                    hboost::lock_guard<hboost::mutex> lk(this->mutex);
                    if(started)
                    {
                        hboost::throw_exception(task_already_started());
                    }
                    started=true;
                }
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
                do_apply(hboost::move(args)...);
#else
                do_apply();
#endif
            }

            void owner_destroyed()
            {
                hboost::unique_lock<hboost::mutex> lk(this->mutex);
                if(!started)
                {
                    started=true;
                    this->mark_exceptional_finish_internal(hboost::copy_exception(hboost::broken_promise()), lk);
                }
            }
        };

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
        template<typename F, typename R>
        struct task_shared_state;
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        template<typename F, typename R, typename ...ArgTypes>
        struct task_shared_state<F, R(ArgTypes...)>:
          task_base_shared_state<R(ArgTypes...)>
#else
        template<typename F, typename R>
        struct task_shared_state<F, R()>:
          task_base_shared_state<R()>
#endif
#else
        template<typename F, typename R>
        struct task_shared_state:
            task_base_shared_state<R>
#endif
        {
        private:
          task_shared_state(task_shared_state&);
        public:
            F f;
            task_shared_state(F const& f_):
                f(f_)
            {}
            task_shared_state(HBOOST_THREAD_RV_REF(F) f_):
              f(hboost::move(f_))
            {}

            F callable()
            {
              return hboost::move(f);
            }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            void do_apply(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
            {
                try
                {
                    this->set_value_at_thread_exit(f(hboost::move(args)...));
                }
#else
            void do_apply()
            {
                try
                {
                    this->set_value_at_thread_exit(f());
                }
#endif
                catch(...)
                {
                    this->set_exception_at_thread_exit(current_exception());
                }
            }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            void do_run(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
            {
                try
                {
                    this->mark_finished_with_result(f(hboost::move(args)...));
                }
#else
            void do_run()
            {
                try
                {
#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
                  R res((f()));
                  this->mark_finished_with_result(hboost::move(res));
#else
                  this->mark_finished_with_result(f());
#endif
                  }
#endif
                catch(...)
                {
                    this->mark_exceptional_finish();
                }
            }
        };

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        template<typename F, typename R, typename ...ArgTypes>
        struct task_shared_state<F, R&(ArgTypes...)>:
          task_base_shared_state<R&(ArgTypes...)>
#else
        template<typename F, typename R>
        struct task_shared_state<F, R&()>:
          task_base_shared_state<R&()>
#endif
#else
        template<typename F, typename R>
        struct task_shared_state<F,R&>:
            task_base_shared_state<R&>
#endif
        {
        private:
          task_shared_state(task_shared_state&);
        public:
            F f;
            task_shared_state(F const& f_):
                f(f_)
            {}
            task_shared_state(HBOOST_THREAD_RV_REF(F) f_):
                f(hboost::move(f_))
            {}

            F callable()
            {
              return f;
            }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            void do_apply(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
            {
                try
                {
                    this->set_value_at_thread_exit(f(hboost::move(args)...));
                }
#else
            void do_apply()
            {
                try
                {
                    this->set_value_at_thread_exit(f());
                }
#endif
                catch(...)
                {
                    this->set_exception_at_thread_exit(current_exception());
                }
            }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            void do_run(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
            {
                try
                {
                    this->mark_finished_with_result(f(hboost::move(args)...));
                }
#else
            void do_run()
            {
                try
                {
                  R& res((f()));
                  this->mark_finished_with_result(res);
                }
#endif
                catch(...)
                {
                    this->mark_exceptional_finish();
                }
            }
        };

#if defined(HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR)

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        template<typename R, typename ...ArgTypes>
        struct task_shared_state<R (*)(ArgTypes...), R(ArgTypes...)>:
          task_base_shared_state<R(ArgTypes...)>
#else
        template<typename R>
        struct task_shared_state<R (*)(), R()>:
          task_base_shared_state<R()>
#endif
#else
        template<typename R>
        struct task_shared_state<R (*)(), R> :
           task_base_shared_state<R>
#endif
            {
            private:
              task_shared_state(task_shared_state&);
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
              typedef R (*CallableType)(ArgTypes ... );
#else
              typedef R (*CallableType)();
#endif
            public:
                CallableType f;
                task_shared_state(CallableType f_):
                    f(f_)
                {}

                CallableType callable()
                {
                  return f;
                }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
                void do_apply(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
                {
                    try
                    {
                        this->set_value_at_thread_exit(f(hboost::move(args)...));
                    }
#else
                void do_apply()
                {
                    try
                    {
                        R r((f()));
                        this->set_value_at_thread_exit(hboost::move(r));
                    }
#endif
                    catch(...)
                    {
                        this->set_exception_at_thread_exit(current_exception());
                    }
                }


#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
                void do_run(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
                {
                    try
                    {
                        this->mark_finished_with_result(f(hboost::move(args)...));
                    }
#else
                void do_run()
                {
                    try
                    {
                        R res((f()));
                        this->mark_finished_with_result(hboost::move(res));
                    }
#endif
                    catch(...)
                    {
                        this->mark_exceptional_finish();
                    }
                }
            };
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        template<typename R, typename ...ArgTypes>
        struct task_shared_state<R& (*)(ArgTypes...), R&(ArgTypes...)>:
          task_base_shared_state<R&(ArgTypes...)>
#else
        template<typename R>
        struct task_shared_state<R& (*)(), R&()>:
          task_base_shared_state<R&()>
#endif
#else
        template<typename R>
        struct task_shared_state<R& (*)(), R&> :
           task_base_shared_state<R&>
#endif
            {
            private:
              task_shared_state(task_shared_state&);
            public:
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
                typedef R& (*CallableType)(HBOOST_THREAD_RV_REF(ArgTypes) ... );
#else
                typedef R& (*CallableType)();
#endif
                CallableType f;
                task_shared_state(CallableType f_):
                    f(f_)
                {}

                CallableType callable()
                {
                  return hboost::move(f);
                }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
                void do_apply(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
                {
                    try
                    {
                        this->set_value_at_thread_exit(f(hboost::move(args)...));
                    }
#else
                void do_apply()
                {
                    try
                    {
                      this->set_value_at_thread_exit(f());
                    }
#endif
                    catch(...)
                    {
                        this->set_exception_at_thread_exit(current_exception());
                    }
                }


#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
                void do_run(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
                {
                    try
                    {
                        this->mark_finished_with_result(f(hboost::move(args)...));
                    }
#else
                void do_run()
                {
                    try
                    {
                        this->mark_finished_with_result(f());
                    }
#endif
                    catch(...)
                    {
                        this->mark_exceptional_finish();
                    }
                }
            };
#endif
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        template<typename F, typename ...ArgTypes>
        struct task_shared_state<F, void(ArgTypes...)>:
          task_base_shared_state<void(ArgTypes...)>
#else
        template<typename F>
        struct task_shared_state<F, void()>:
          task_base_shared_state<void()>
#endif
#else
        template<typename F>
        struct task_shared_state<F,void>:
          task_base_shared_state<void>
#endif
        {
        private:
          task_shared_state(task_shared_state&);
        public:
            typedef F CallableType;
            F f;
            task_shared_state(F const& f_):
                f(f_)
            {}
            task_shared_state(HBOOST_THREAD_RV_REF(F) f_):
                f(hboost::move(f_))
            {}
            F callable()
            {
              return hboost::move(f);
            }
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            void do_apply(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
            {
              try
              {
                f(hboost::move(args)...);
#else
            void do_apply()
            {
                try
                {
                    f();
#endif
                  this->set_value_at_thread_exit();
                }
                catch(...)
                {
                    this->set_exception_at_thread_exit(current_exception());
                }
            }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            void do_run(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
            {
                try
                {
                    f(hboost::move(args)...);
#else
            void do_run()
            {
                try
                {
                    f();
#endif
                    this->mark_finished_with_result();
                }
                catch(...)
                {
                    this->mark_exceptional_finish();
                }
            }
        };

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        template<typename ...ArgTypes>
        struct task_shared_state<void (*)(ArgTypes...), void(ArgTypes...)>:
        task_base_shared_state<void(ArgTypes...)>
#else
        template<>
        struct task_shared_state<void (*)(), void()>:
        task_base_shared_state<void()>
#endif
#else
        template<>
        struct task_shared_state<void (*)(),void>:
          task_base_shared_state<void>
#endif
        {
        private:
          task_shared_state(task_shared_state&);
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            typedef void (*CallableType)(ArgTypes...);
#else
            typedef void (*CallableType)();
#endif
        public:
            CallableType f;
            task_shared_state(CallableType f_):
                f(f_)
            {}
            CallableType callable()
            {
              return f;
            }
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            void do_apply(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
            {
                try
                {
                    f(hboost::move(args)...);
#else
            void do_apply()
            {
                try
                {
                    f();
#endif
                    this->set_value_at_thread_exit();
                }
                catch(...)
                {
                    this->set_exception_at_thread_exit(current_exception());
                }
            }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            void do_run(HBOOST_THREAD_RV_REF(ArgTypes) ... args)
            {
                try
                {
                    f(hboost::move(args)...);
#else
            void do_run()
            {
                try
                {
                  f();
#endif
                  this->mark_finished_with_result();
                }
                catch(...)
                {
                    this->mark_exceptional_finish();
                }
            }
        };
    }

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
    template<typename R, typename ...ArgTypes>
    class packaged_task<R(ArgTypes...)>
    {
      typedef hboost::shared_ptr<detail::task_base_shared_state<R(ArgTypes...)> > task_ptr;
      hboost::shared_ptr<detail::task_base_shared_state<R(ArgTypes...)> > task;
  #else
    template<typename R>
    class packaged_task<R()>
    {
      typedef hboost::shared_ptr<detail::task_base_shared_state<R()> > task_ptr;
      hboost::shared_ptr<detail::task_base_shared_state<R()> > task;
  #endif
#else
    template<typename R>
    class packaged_task
    {
      typedef hboost::shared_ptr<detail::task_base_shared_state<R> > task_ptr;
      hboost::shared_ptr<detail::task_base_shared_state<R> > task;
#endif
        bool future_obtained;
        struct dummy;

    public:
        typedef R result_type;
        HBOOST_THREAD_MOVABLE_ONLY(packaged_task)

        packaged_task():
            future_obtained(false)
        {}

        // construction and destruction
#if defined(HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR)

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        explicit packaged_task(R(*f)(), HBOOST_THREAD_FWD_REF(ArgTypes)... args)
        {
            typedef R(*FR)(HBOOST_THREAD_FWD_REF(ArgTypes)...);
            typedef detail::task_shared_state<FR,R(ArgTypes...)> task_shared_state_type;
            task= task_ptr(new task_shared_state_type(f, hboost::move(args)...));
            future_obtained=false;
        }
  #else
        explicit packaged_task(R(*f)())
        {
            typedef R(*FR)();
            typedef detail::task_shared_state<FR,R()> task_shared_state_type;
            task= task_ptr(new task_shared_state_type(f));
            future_obtained=false;
        }
  #endif
#else
        explicit packaged_task(R(*f)())
        {
              typedef R(*FR)();
            typedef detail::task_shared_state<FR,R> task_shared_state_type;
            task= task_ptr(new task_shared_state_type(f));
            future_obtained=false;
        }
#endif
#endif
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
        template <class F>
        explicit packaged_task(HBOOST_THREAD_FWD_REF(F) f
            , typename hboost::disable_if<is_same<typename decay<F>::type, packaged_task>, dummy* >::type=0
            )
        {
          typedef typename decay<F>::type FR;
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            typedef detail::task_shared_state<FR,R(ArgTypes...)> task_shared_state_type;
  #else
            typedef detail::task_shared_state<FR,R()> task_shared_state_type;
  #endif
#else
            typedef detail::task_shared_state<FR,R> task_shared_state_type;
#endif
            task = task_ptr(new task_shared_state_type(hboost::forward<F>(f)));
            future_obtained = false;

        }

#else
        template <class F>
        explicit packaged_task(F const& f
            , typename hboost::disable_if<is_same<typename decay<F>::type, packaged_task>, dummy* >::type=0
            )
        {
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            typedef detail::task_shared_state<F,R(ArgTypes...)> task_shared_state_type;
  #else
            typedef detail::task_shared_state<F,R()> task_shared_state_type;
  #endif
#else
            typedef detail::task_shared_state<F,R> task_shared_state_type;
#endif
            task = task_ptr(new task_shared_state_type(f));
            future_obtained=false;
        }
        template <class F>
        explicit packaged_task(HBOOST_THREAD_RV_REF(F) f)
        {
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
            typedef detail::task_shared_state<F,R(ArgTypes...)> task_shared_state_type;
            task = task_ptr(new task_shared_state_type(hboost::move(f)));
#else
            typedef detail::task_shared_state<F,R()> task_shared_state_type;
            task = task_ptr(new task_shared_state_type(hboost::move(f)));
#endif
#else
            typedef detail::task_shared_state<F,R> task_shared_state_type;
            task = task_ptr(new task_shared_state_type(hboost::move(f)));
#endif
            future_obtained=false;

        }
#endif

#if defined HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
#if defined(HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR)
        template <class Allocator>
        packaged_task(hboost::allocator_arg_t, Allocator a, R(*f)())
        {
          typedef R(*FR)();
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
          typedef detail::task_shared_state<FR,R(ArgTypes...)> task_shared_state_type;
  #else
          typedef detail::task_shared_state<FR,R()> task_shared_state_type;
  #endif
#else
          typedef detail::task_shared_state<FR,R> task_shared_state_type;
#endif
          typedef typename Allocator::template rebind<task_shared_state_type>::other A2;
          A2 a2(a);
          typedef thread_detail::allocator_destructor<A2> D;

          task = task_ptr(::new(a2.allocate(1)) task_shared_state_type(f), D(a2, 1) );
          future_obtained = false;
        }
#endif // HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR

#if ! defined HBOOST_NO_CXX11_RVALUE_REFERENCES
        template <class F, class Allocator>
        packaged_task(hboost::allocator_arg_t, Allocator a, HBOOST_THREAD_FWD_REF(F) f)
        {
          typedef typename decay<F>::type FR;

#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
          typedef detail::task_shared_state<FR,R(ArgTypes...)> task_shared_state_type;
  #else
          typedef detail::task_shared_state<FR,R()> task_shared_state_type;
  #endif
#else
          typedef detail::task_shared_state<FR,R> task_shared_state_type;
#endif
          typedef typename Allocator::template rebind<task_shared_state_type>::other A2;
          A2 a2(a);
          typedef thread_detail::allocator_destructor<A2> D;

          task = task_ptr(::new(a2.allocate(1)) task_shared_state_type(hboost::forward<F>(f)), D(a2, 1) );
          future_obtained = false;
        }
#else // ! defined HBOOST_NO_CXX11_RVALUE_REFERENCES
        template <class F, class Allocator>
        packaged_task(hboost::allocator_arg_t, Allocator a, const F& f)
        {
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
          typedef detail::task_shared_state<F,R(ArgTypes...)> task_shared_state_type;
  #else
          typedef detail::task_shared_state<F,R()> task_shared_state_type;
  #endif
#else
          typedef detail::task_shared_state<F,R> task_shared_state_type;
#endif
          typedef typename Allocator::template rebind<task_shared_state_type>::other A2;
          A2 a2(a);
          typedef thread_detail::allocator_destructor<A2> D;

          task = task_ptr(::new(a2.allocate(1)) task_shared_state_type(f), D(a2, 1) );
          future_obtained = false;
        }
        template <class F, class Allocator>
        packaged_task(hboost::allocator_arg_t, Allocator a, HBOOST_THREAD_RV_REF(F) f)
        {
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
          typedef detail::task_shared_state<F,R(ArgTypes...)> task_shared_state_type;
  #else
          typedef detail::task_shared_state<F,R()> task_shared_state_type;
  #endif
#else
          typedef detail::task_shared_state<F,R> task_shared_state_type;
#endif
          typedef typename Allocator::template rebind<task_shared_state_type>::other A2;
          A2 a2(a);
          typedef thread_detail::allocator_destructor<A2> D;

          task = task_ptr(::new(a2.allocate(1)) task_shared_state_type(hboost::move(f)), D(a2, 1) );
          future_obtained = false;
        }

#endif //HBOOST_NO_CXX11_RVALUE_REFERENCES
#endif // HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS

        ~packaged_task() {
            if(task) {
                task->owner_destroyed();
            }
        }

        // assignment
        packaged_task(HBOOST_THREAD_RV_REF(packaged_task) other) HBOOST_NOEXCEPT
        : future_obtained(HBOOST_THREAD_RV(other).future_obtained) {
            task.swap(HBOOST_THREAD_RV(other).task);
            HBOOST_THREAD_RV(other).future_obtained=false;
        }
        packaged_task& operator=(HBOOST_THREAD_RV_REF(packaged_task) other) HBOOST_NOEXCEPT {

#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
            packaged_task temp(hboost::move(other));
#else
            packaged_task temp(static_cast<HBOOST_THREAD_RV_REF(packaged_task)>(other));
#endif
            swap(temp);
            return *this;
        }

#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        void set_executor(executor_ptr_type aex)
        {
          if (!valid())
              hboost::throw_exception(task_moved());
          hboost::lock_guard<hboost::mutex> lk(task->mutex);
          task->set_executor_policy(aex, lk);
        }
#endif
        void reset() {
            if (!valid())
              hboost::throw_exception(future_error(system::make_error_code(future_errc::no_state)));

            // As if *this = packaged_task(task->callable());

            task->reset();
            future_obtained=false;
        }

        void swap(packaged_task& other) HBOOST_NOEXCEPT {
            task.swap(other.task);
            std::swap(future_obtained,other.future_obtained);
        }
        bool valid() const HBOOST_NOEXCEPT {
          return task.get()!=0;
        }

        // result retrieval
        HBOOST_THREAD_FUTURE<R> get_future() {
            if(!task) {
                hboost::throw_exception(task_moved());
            } else if(!future_obtained) {
                future_obtained=true;
                return HBOOST_THREAD_FUTURE<R>(task);
            } else {
                hboost::throw_exception(future_already_retrieved());
            }
        }

        // execution
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK && defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
        void operator()(ArgTypes... args) {
            if(!task) {
                hboost::throw_exception(task_moved());
            }
            task->run(hboost::move(args)...);
        }
        void make_ready_at_thread_exit(ArgTypes... args) {
          if(!task) {
              hboost::throw_exception(task_moved());
          }
          if (task->has_value()) {
                hboost::throw_exception(promise_already_satisfied());
          }
          task->apply(hboost::move(args)...);
        }
#else
        void operator()() {
            if(!task) {
                hboost::throw_exception(task_moved());
            }
            task->run();
        }
        void make_ready_at_thread_exit() {
          if(!task) {
              hboost::throw_exception(task_moved());
          }
          if (task->has_value()) hboost::throw_exception(promise_already_satisfied());
          task->apply();
        }
#endif
        template<typename F>
        void set_wait_callback(F f) {
            task->set_wait_callback(f,this);
        }
    };
}
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
namespace hboost { namespace container {
    template <class R, class Alloc>
    struct uses_allocator< ::hboost::packaged_task<R> , Alloc> : true_type
    {};
}}
#if ! defined  HBOOST_NO_CXX11_ALLOCATOR
namespace std {
    template <class R, class Alloc>
    struct uses_allocator< ::hboost::packaged_task<R> , Alloc> : true_type
    {};
}
#endif
#endif

namespace hboost
{
  HBOOST_THREAD_DCL_MOVABLE_BEG(T) packaged_task<T> HBOOST_THREAD_DCL_MOVABLE_END

namespace detail
{
  ////////////////////////////////
  // make_future_deferred_shared_state
  ////////////////////////////////
  template <class Rp, class Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_future_deferred_shared_state(HBOOST_THREAD_FWD_REF(Fp) f) {
    shared_ptr<future_deferred_shared_state<Rp, Fp> >
        h(new future_deferred_shared_state<Rp, Fp>(hboost::forward<Fp>(f)));
    return HBOOST_THREAD_FUTURE<Rp>(h);
  }

  ////////////////////////////////
  // make_future_async_shared_state
  ////////////////////////////////
  template <class Rp, class Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_future_async_shared_state(HBOOST_THREAD_FWD_REF(Fp) f) {
    shared_ptr<future_async_shared_state<Rp, Fp> >
        h(new future_async_shared_state<Rp, Fp>());
    h->init(hboost::forward<Fp>(f));
    return HBOOST_THREAD_FUTURE<Rp>(h);
  }
}

    ////////////////////////////////
    // template <class F, class... ArgTypes>
    // future<R> async(launch policy, F&&, ArgTypes&&...);
    ////////////////////////////////

#if defined HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR

#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
  template <class R, class... ArgTypes>
  HBOOST_THREAD_FUTURE<R>
  async(launch policy, R(*f)(HBOOST_THREAD_FWD_REF(ArgTypes)...), HBOOST_THREAD_FWD_REF(ArgTypes)... args) {
    typedef R(*F)(HBOOST_THREAD_FWD_REF(ArgTypes)...);
    typedef detail::invoker<typename decay<F>::type, typename decay<ArgTypes>::type...> BF;
    typedef typename BF::result_type Rp;

    if (underlying_cast<int>(policy) & int(launch::async)) {
      return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_async_shared_state<Rp>(
              BF(
                  f
                  , thread_detail::decay_copy(hboost::forward<ArgTypes>(args))...
              )
          ));
    } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
      return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_deferred_shared_state<Rp>(
              BF(
                  f
                  , thread_detail::decay_copy(hboost::forward<ArgTypes>(args))...
              )
          ));
    } else {
      std::terminate();
      //HBOOST_THREAD_FUTURE<R> ret;
      //return ::hboost::move(ret);
    }
  }

#else // defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)

  template <class R>
  HBOOST_THREAD_FUTURE<R>
  async(launch policy, R(*f)()) {
  #if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
    typedef packaged_task<R()> packaged_task_type;
  #else
    typedef packaged_task<R> packaged_task_type;
  #endif

    if (underlying_cast<int>(policy) & int(launch::async)) {
      packaged_task_type pt( f );
      HBOOST_THREAD_FUTURE<R> ret = HBOOST_THREAD_MAKE_RV_REF(pt.get_future());
      ret.set_async();
      hboost::thread( hboost::move(pt) ).detach();
      return ::hboost::move(ret);
    } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
      std::terminate();
      //HBOOST_THREAD_FUTURE<R> ret;
      //return ::hboost::move(ret);
    } else {
      std::terminate();
      //HBOOST_THREAD_FUTURE<R> ret;
      //return ::hboost::move(ret);
    }
  }
#endif
#endif // defined(HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR)

#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)

  template <class F, class ...ArgTypes>
  HBOOST_THREAD_FUTURE<typename hboost::result_of<typename decay<F>::type(
      typename decay<ArgTypes>::type...
  )>::type>
  async(launch policy, HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(ArgTypes)... args) {
    typedef detail::invoker<typename decay<F>::type, typename decay<ArgTypes>::type...> BF;
    typedef typename BF::result_type Rp;

    if (underlying_cast<int>(policy) & int(launch::async)) {
      return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_async_shared_state<Rp>(
              BF(
                  thread_detail::decay_copy(hboost::forward<F>(f))
                , thread_detail::decay_copy(hboost::forward<ArgTypes>(args))...
              )
          ));
    } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
      return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_deferred_shared_state<Rp>(
              BF(
                  thread_detail::decay_copy(hboost::forward<F>(f))
                , thread_detail::decay_copy(hboost::forward<ArgTypes>(args))...
              )
          ));
    } else {
      std::terminate();
      //HBOOST_THREAD_FUTURE<R> ret;
      //return ::hboost::move(ret);
    }
  }

#else // defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)

  template <class F>
  HBOOST_THREAD_FUTURE<typename hboost::result_of<typename decay<F>::type()>::type>
  async(launch policy, HBOOST_THREAD_FWD_REF(F) f) {
    typedef typename hboost::result_of<typename decay<F>::type()>::type R;
#if defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
    typedef packaged_task<R()> packaged_task_type;
#else // defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
    typedef packaged_task<R> packaged_task_type;
#endif // defined HBOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK

    if (underlying_cast<int>(policy) & int(launch::async)) {
      packaged_task_type pt( hboost::forward<F>(f) );
      HBOOST_THREAD_FUTURE<R> ret = pt.get_future();
      ret.set_async();
      hboost::thread( hboost::move(pt) ).detach();
      return ::hboost::move(ret);
    } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
      std::terminate();
      //HBOOST_THREAD_FUTURE<R> ret;
      //return ::hboost::move(ret);
      //          return hboost::detail::make_future_deferred_shared_state<Rp>(
      //              BF(
      //                  thread_detail::decay_copy(hboost::forward<F>(f))
      //              )
      //          );
    } else {
      std::terminate();
      //HBOOST_THREAD_FUTURE<R> ret;
      //return ::hboost::move(ret);
    }
  }
#endif // defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)

#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
namespace detail {

    /////////////////////////
    /// shared_state_nullary_task
    /////////////////////////
    template<typename Rp, typename Fp>
    struct shared_state_nullary_task
    {

      typedef shared_ptr<shared_state_base > storage_type;
      storage_type that;
      Fp f_;
    public:

      shared_state_nullary_task(storage_type st, HBOOST_THREAD_FWD_REF(Fp) f)
      : that(st), f_(hboost::move(f))
      {};

#if ! defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(shared_state_nullary_task)
      shared_state_nullary_task(shared_state_nullary_task const& x) //HBOOST_NOEXCEPT
      : that(x.that), f_(x.f_)
      {}
      shared_state_nullary_task& operator=(HBOOST_THREAD_COPY_ASSIGN_REF(shared_state_nullary_task) x) //HBOOST_NOEXCEPT
      {
        if (this != &x) {
          that=x.that;
          f_=x.f_;
        }
        return *this;
      }
      // move
      shared_state_nullary_task(HBOOST_THREAD_RV_REF(shared_state_nullary_task) x) //HBOOST_NOEXCEPT
      : that(x.that), f_(hboost::move(x.f_))
      {
        x.that.reset();
      }
      shared_state_nullary_task& operator=(HBOOST_THREAD_RV_REF(shared_state_nullary_task) x) //HBOOST_NOEXCEPT
      {
        if (this != &x) {
          that=x.that;
          f_=hboost::move(x.f_);
          x.that.reset();
        }
        return *this;
      }
#endif
      void operator()() {
        shared_ptr<shared_state<Rp> > that_ = static_pointer_cast<shared_state<Rp> >(that);
        try {
          that_->mark_finished_with_result(f_());
        } catch(...) {
          that_->mark_exceptional_finish();
        }
      }
      ~shared_state_nullary_task()
      {
      }
    };

    template<typename Fp>
    struct shared_state_nullary_task<void, Fp>
    {
      typedef shared_ptr<shared_state_base > storage_type;
      storage_type that;
      Fp f_;
    public:
      shared_state_nullary_task(storage_type st, HBOOST_THREAD_FWD_REF(Fp) f)
      : that(st), f_(hboost::move(f))
      {};

#if ! defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(shared_state_nullary_task)
      shared_state_nullary_task(shared_state_nullary_task const& x) //HBOOST_NOEXCEPT
      : that(x.that), f_(x.f_)
      {}
      shared_state_nullary_task& operator=(HBOOST_THREAD_COPY_ASSIGN_REF(shared_state_nullary_task) x) //HBOOST_NOEXCEPT
      {
        if (this != &x) {
          that=x.that;
          f_=x.f_;
        }
        return *this;
      }
      // move
      shared_state_nullary_task(HBOOST_THREAD_RV_REF(shared_state_nullary_task) x) HBOOST_NOEXCEPT
      : that(x.that), f_(hboost::move(x.f_))
      {
        x.that.reset();
      }
      shared_state_nullary_task& operator=(HBOOST_THREAD_RV_REF(shared_state_nullary_task) x) HBOOST_NOEXCEPT {
        if (this != &x) {
          that=x.that;
          f_=hboost::move(x.f_);
          x.that.reset();
        }
        return *this;
      }
#endif
      void operator()() {
        shared_ptr<shared_state<void> > that_ = static_pointer_cast<shared_state<void> >(that);
        try {
          f_();
          that_->mark_finished_with_result();
        } catch(...) {
          that_->mark_exceptional_finish();
        }
      }
    };

}
    HBOOST_THREAD_DCL_MOVABLE_BEG2(R,F) detail::shared_state_nullary_task<R,F> HBOOST_THREAD_DCL_MOVABLE_END
namespace detail {

    /////////////////////////
    /// future_executor_shared_state_base
    /////////////////////////
    template<typename Rp>
    struct future_executor_shared_state: shared_state<Rp>
    {
      typedef shared_state<Rp> base_type;
    protected:
    public:
      future_executor_shared_state() {
      }

      template <class Fp, class Executor>
      void init(Executor& ex, HBOOST_THREAD_FWD_REF(Fp) f)
      {
        typedef typename decay<Fp>::type Cont;
        this->set_executor_policy(executor_ptr_type(new executor_ref<Executor>(ex)));
        shared_state_nullary_task<Rp,Cont> t(this->shared_from_this(), hboost::forward<Fp>(f));
        ex.submit(hboost::move(t));
      }

      ~future_executor_shared_state() {}
    };

    ////////////////////////////////
    // make_future_executor_shared_state
    ////////////////////////////////
    template <class Rp, class Fp, class Executor>
    HBOOST_THREAD_FUTURE<Rp>
    make_future_executor_shared_state(Executor& ex, HBOOST_THREAD_FWD_REF(Fp) f) {
      shared_ptr<future_executor_shared_state<Rp> >
          h(new future_executor_shared_state<Rp>());
      h->init(ex, hboost::forward<Fp>(f));
      return HBOOST_THREAD_FUTURE<Rp>(h);
    }

} // detail

    ////////////////////////////////
    // template <class Executor, class F, class... ArgTypes>
    // future<R> async(Executor& ex, F&&, ArgTypes&&...);
    ////////////////////////////////

//#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#if defined(HBOOST_THREAD_PROVIDES_INVOKE) && ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && ! defined(HBOOST_NO_CXX11_HDR_TUPLE)

#if defined HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR

  template <class Executor, class R, class... ArgTypes>
  HBOOST_THREAD_FUTURE<R>
  async(Executor& ex, R(*f)(HBOOST_THREAD_FWD_REF(ArgTypes)...), HBOOST_THREAD_FWD_REF(ArgTypes)... args) {
    typedef R(*F)(HBOOST_THREAD_FWD_REF(ArgTypes)...);
    typedef detail::invoker<typename decay<F>::type, typename decay<ArgTypes>::type...> BF;
    typedef typename BF::result_type Rp;

    return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_executor_shared_state<Rp>(ex,
        BF(
            f
            , thread_detail::decay_copy(hboost::forward<ArgTypes>(args))...
        )
    ));
  }
#endif // defined HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR

  template <class Executor, class F, class ...ArgTypes>
  HBOOST_THREAD_FUTURE<typename hboost::result_of<typename decay<F>::type(
      typename decay<ArgTypes>::type...
  )>::type>
  async(Executor& ex, HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(ArgTypes)... args) {
    typedef detail::invoker<typename decay<F>::type, typename decay<ArgTypes>::type...> BF;
    typedef typename BF::result_type Rp;

    return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_executor_shared_state<Rp>(ex,
        BF(
            thread_detail::decay_copy(hboost::forward<F>(f))
            , thread_detail::decay_copy(hboost::forward<ArgTypes>(args))...
        )
    ));
  }

#else // ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#if defined HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR

  template <class Executor, class R>
  HBOOST_THREAD_FUTURE<R>
  async(Executor& ex, R(*f)()) {
    typedef R(*F)();
    typedef detail::invoker<F> BF;
    typedef typename BF::result_type Rp;

    return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_executor_shared_state<Rp>(ex,
        BF(
            f
        )
    ));
  }

  template <class Executor, class R, class A1>
  HBOOST_THREAD_FUTURE<R>
  async(Executor& ex, R(*f)(HBOOST_THREAD_FWD_REF(A1)), HBOOST_THREAD_FWD_REF(A1) a1) {
    typedef R(*F)(HBOOST_THREAD_FWD_REF(A1));
    typedef detail::invoker<F, typename decay<A1>::type> BF;
    typedef typename BF::result_type Rp;

    return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_executor_shared_state<Rp>(ex,
        BF(
            f
            , thread_detail::decay_copy(hboost::forward<A1>(a1))
        )
    ));
  }
#endif // defined HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR

  template <class Executor, class F>
  HBOOST_THREAD_FUTURE<typename hboost::result_of<typename decay<F>::type()>::type>
  async(Executor& ex, HBOOST_THREAD_FWD_REF(F) f)  {
    typedef detail::invoker<typename decay<F>::type> BF;
    typedef typename BF::result_type Rp;

    return hboost::detail::make_future_executor_shared_state<Rp>(ex,
        BF(
            thread_detail::decay_copy(hboost::forward<F>(f))
        )
    );
  }

  template <class Executor, class F, class A1>
  HBOOST_THREAD_FUTURE<typename hboost::result_of<typename decay<F>::type(
      typename decay<A1>::type
  )>::type>
  async(Executor& ex, HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(A1) a1) {
    typedef detail::invoker<typename decay<F>::type, typename decay<A1>::type> BF;
    typedef typename BF::result_type Rp;

    return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_executor_shared_state<Rp>(ex,
        BF(
            thread_detail::decay_copy(hboost::forward<F>(f))
          , thread_detail::decay_copy(hboost::forward<A1>(a1))
        )
    ));
  }

  template <class Executor, class F, class A1, class A2>
  HBOOST_THREAD_FUTURE<typename hboost::result_of<typename decay<F>::type(
      typename decay<A1>::type, typename decay<A2>::type
  )>::type>
  async(Executor& ex, HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(A1) a1, HBOOST_THREAD_FWD_REF(A2) a2) {
    typedef detail::invoker<typename decay<F>::type, typename decay<A1>::type, typename decay<A2>::type> BF;
    typedef typename BF::result_type Rp;

    return HBOOST_THREAD_MAKE_RV_REF(hboost::detail::make_future_executor_shared_state<Rp>(ex,
        BF(
            thread_detail::decay_copy(hboost::forward<F>(f))
          , thread_detail::decay_copy(hboost::forward<A1>(a1))
          , thread_detail::decay_copy(hboost::forward<A2>(a2))
        )
    ));
  }

#endif //! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#endif

  ////////////////////////////////
  // template <class F, class... ArgTypes>
  // future<R> async(F&&, ArgTypes&&...);
  ////////////////////////////////

#if defined HBOOST_THREAD_RVALUE_REFERENCES_DONT_MATCH_FUNTION_PTR
  #if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
  template <class R, class... ArgTypes>
  HBOOST_THREAD_FUTURE<R>
  async(R(*f)(HBOOST_THREAD_FWD_REF(ArgTypes)...), HBOOST_THREAD_FWD_REF(ArgTypes)... args) {
    return HBOOST_THREAD_MAKE_RV_REF(async(launch(launch::any), f, hboost::forward<ArgTypes>(args)...));
  }
  #else
  template <class R>
  HBOOST_THREAD_FUTURE<R>
  async(R(*f)()) {
    return HBOOST_THREAD_MAKE_RV_REF(async(launch(launch::any), f));
  }
  #endif
#endif

#if defined(HBOOST_THREAD_PROVIDES_VARIADIC_THREAD)
  template <class F, class ...ArgTypes>
  HBOOST_THREAD_FUTURE<typename hboost::result_of<typename decay<F>::type(
      typename decay<ArgTypes>::type...
  )>::type>
  async(HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(ArgTypes)... args) {
      return HBOOST_THREAD_MAKE_RV_REF(async(launch(launch::any), hboost::forward<F>(f), hboost::forward<ArgTypes>(args)...));
  }
#else
  template <class F>
  HBOOST_THREAD_FUTURE<typename hboost::result_of<F()>::type>
  async(HBOOST_THREAD_FWD_REF(F) f) {
      return HBOOST_THREAD_MAKE_RV_REF(async(launch(launch::any), hboost::forward<F>(f)));
  }
#endif

  ////////////////////////////////
  // make_future deprecated
  ////////////////////////////////
  template <typename T>
  HBOOST_THREAD_FUTURE<typename decay<T>::type> make_future(HBOOST_THREAD_FWD_REF(T) value) {
    typedef typename decay<T>::type future_value_type;
    promise<future_value_type> p;
    p.set_value(hboost::forward<future_value_type>(value));
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future());
  }

#if defined HBOOST_THREAD_USES_MOVE
  inline HBOOST_THREAD_FUTURE<void> make_future() {
    promise<void> p;
    p.set_value();
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future());
  }
#endif

  ////////////////////////////////
  // make_ready_future
  ////////////////////////////////
  namespace detail {
    template <class T>
    struct deduced_type_impl
    {
        typedef T type;
    };

    template <class T>
    struct deduced_type_impl<reference_wrapper<T> const>
    {
        typedef T& type;
    };
    template <class T>
    struct deduced_type_impl<reference_wrapper<T> >
    {
        typedef T& type;
    };
#if __cplusplus > 201103L
    template <class T>
    struct deduced_type_impl<std::reference_wrapper<T> >
    {
        typedef T& type;
    };
#endif
    template <class T>
    struct deduced_type
    {
        typedef typename detail::deduced_type_impl<typename decay<T>::type>::type type;
    };

  }


#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template <int = 0, int..., class T>
#else
  template <class T>
#endif
  HBOOST_THREAD_FUTURE<typename detail::deduced_type<T>::type> make_ready_future(HBOOST_THREAD_FWD_REF(T) value) {
    typedef typename detail::deduced_type<T>::type future_value_type;
    promise<future_value_type> p;
    p.set_value(hboost::forward<T>(value));
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future());
  }

  // explicit overloads
  template <class T>
  HBOOST_THREAD_FUTURE<T> make_ready_future(typename remove_reference<T>::type & x)
  {
    promise<T> p;
    p.set_value(x);
    return p.get_future();
  }

  template <class T>
  HBOOST_THREAD_FUTURE<T> make_ready_future(HBOOST_THREAD_FWD_REF(typename remove_reference<T>::type) x)
  {
    promise<T> p;
    p.set_value(forward<typename remove_reference<T>::type>(x));
    return p.get_future();
  }

  // variadic overload
#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template <class T, class ...Args>
  HBOOST_THREAD_FUTURE<T> make_ready_future(Args&&... args)
  {
    promise<T> p;
    p.emplace(forward<Args>(args)...);
    return p.get_future();

  }
#endif

  template <typename T, typename T1>
  HBOOST_THREAD_FUTURE<T> make_ready_no_decay_future(T1 value) {
    typedef T future_value_type;
    promise<future_value_type> p;
    p.set_value(value);
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future());
  }

#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined HBOOST_THREAD_USES_MOVE
  inline HBOOST_THREAD_FUTURE<void> make_ready_future() {
    promise<void> p;
    p.set_value();
    return p.get_future();
  }
#endif


  template <typename T>
  HBOOST_THREAD_FUTURE<T> make_exceptional_future(exception_ptr ex) {
    promise<T> p;
    p.set_exception(ex);
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future());
  }

  template <typename T, typename E>
  HBOOST_THREAD_FUTURE<T> make_exceptional_future(E ex) {
    promise<T> p;
    p.set_exception(hboost::copy_exception(ex));
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future());
  }

  template <typename T>
  HBOOST_THREAD_FUTURE<T> make_exceptional_future() {
    promise<T> p;
    p.set_exception(hboost::current_exception());
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future());
  }
  template <typename T>
  HBOOST_THREAD_FUTURE<T> make_ready_future(exception_ptr ex)  {
    return make_exceptional_future<T>(ex);
  }

#if 0
  template<typename CLOSURE>
  make_future(CLOSURE closure) -> HBOOST_THREAD_FUTURE<decltype(closure())> {
      typedef decltype(closure()) T;
      promise<T> p;
      try {
        p.set_value(closure());
      } catch(...) {
        p.set_exception(std::current_exception());
      }
      return HBOOST_THREAD_MAKE_RV_REF(p.get_future());
  }
#endif

  ////////////////////////////////
  // make_shared_future deprecated
  ////////////////////////////////
  template <typename T>
  shared_future<typename decay<T>::type> make_shared_future(HBOOST_THREAD_FWD_REF(T) value) {
    typedef typename decay<T>::type future_type;
    promise<future_type> p;
    p.set_value(hboost::forward<T>(value));
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future().share());
  }

  inline shared_future<void> make_shared_future()  {
    promise<void> p;
    return HBOOST_THREAD_MAKE_RV_REF(p.get_future().share());
  }

  ////////////////////////////////
  // detail::future_async_continuation_shared_state
  ////////////////////////////////
#if defined HBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

namespace detail
{
  //////////////////////
  // detail::continuation_shared_state
  //////////////////////
  template<typename F, typename Rp, typename Fp, class ShSt=shared_state<Rp> >
  struct continuation_shared_state: ShSt
  {
    F parent;
    Fp continuation;

  public:
    continuation_shared_state(HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c)
    : parent(hboost::move(f)),
      continuation(hboost::move(c))
    {
    }

    void init(hboost::unique_lock<hboost::mutex> &lock)
    {
      parent.future_->set_continuation_ptr(this->shared_from_this(), lock);
    }

    void call() {
      try {
        this->mark_finished_with_result(this->continuation(hboost::move(this->parent)));
      } catch(...) {
        this->mark_exceptional_finish();
      }
      // make sure parent is really cleared to prevent memory "leaks"
      this->parent = F();
    }

    void call(hboost::unique_lock<hboost::mutex>& lck) {
      try {
        relocker relock(lck);

        // neither continuation nor parent are protected by the lock - call() must only
        // be called once, and no one else must modify it.
        Rp res = this->continuation(hboost::move(this->parent));

        // make sure parent is really cleared to prevent memory "leaks"
        this->parent = F();

        relock.lock();

        this->mark_finished_with_result_internal(hboost::move(res), lck);
      } catch (...) {
        this->mark_exceptional_finish_internal(current_exception(), lck);

        // make sure parent is really cleared to prevent memory "leaks"
        relocker relock(lck);
        this->parent = F();
      }
    }

    static void run(shared_ptr<hboost::detail::shared_state_base> that_)
    {
      continuation_shared_state* that = static_cast<continuation_shared_state*>(that_.get());
      that->call();
    }

    ~continuation_shared_state() {}
  };

  template<typename F, typename Fp, class ShSt>
  struct continuation_shared_state<F, void, Fp, ShSt>: ShSt
  {
    F parent;
    Fp continuation;

  public:
    continuation_shared_state(HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c)
    : parent(hboost::move(f)),
      continuation(hboost::move(c))
    {
    }

    void init(hboost::unique_lock<hboost::mutex> &lock)
    {
      parent.future_->set_continuation_ptr(this->shared_from_this(), lock);
    }

    void call()
    {
      try {
        this->continuation(hboost::move(this->parent));
        this->mark_finished_with_result();
      } catch(...) {
        this->mark_exceptional_finish();
      }
      // make sure parent is really cleared to prevent memory "leaks"
      this->parent = F();
    }

    void call(hboost::unique_lock<hboost::mutex>& lck) {
      try {
        {
          relocker relock(lck);
          // neither continuation nor parent are protected by the lock - call() must only
          // be called once, and no one else must modify it.
          this->continuation(hboost::move(this->parent));

          // make sure parent is really cleared to prevent memory "leaks"
          this->parent = F();
        }
        this->mark_finished_with_result_internal(lck);
      } catch (...) {
        this->mark_exceptional_finish_internal(current_exception(), lck);

        // make sure parent is really cleared to prevent memory "leaks"
        relocker relock(lck);
        this->parent = F();
      }
    }

    static void run(shared_ptr<hboost::detail::shared_state_base> that_)
    {
      continuation_shared_state* that = static_cast<continuation_shared_state*>(that_.get());
      that->call();
    }

    ~continuation_shared_state() {}
  };
  /////////////////////////
  /// future_async_continuation_shared_state
  /////////////////////////

  template<typename F, typename Rp, typename Fp>
  struct future_async_continuation_shared_state: continuation_shared_state<F,Rp,Fp,future_async_shared_state_base<Rp> >
  {
    typedef continuation_shared_state<F,Rp,Fp,future_async_shared_state_base<Rp> > base_type;
  public:
    future_async_continuation_shared_state(HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c)
    : base_type(hboost::move(f), hboost::forward<Fp>(c))
    {    }

    void launch_continuation() {
#if defined HBOOST_THREAD_FUTURE_BLOCKING
      hboost::lock_guard<hboost::mutex> lk(this->mutex);
      this->thr_ = thread(&future_async_continuation_shared_state::run, static_shared_from_this(this));
#else
      thread(&base_type::run, static_shared_from_this(this)).detach();
#endif
    }
  };

  /////////////////////////
  /// future_sync_continuation_shared_state
  /////////////////////////

  template<typename F, typename Rp, typename Fp>
  struct future_sync_continuation_shared_state: continuation_shared_state<F,Rp,Fp,shared_state<Rp> >
  {
    typedef continuation_shared_state<F,Rp,Fp,shared_state<Rp> > base_type;
  public:
    future_sync_continuation_shared_state(HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c)
    : base_type(hboost::move(f), hboost::forward<Fp>(c))
    {    }

    void launch_continuation() {
      this->call();
    }
  };


  /////////////////////////
  /// future_executor_continuation_shared_state
  /////////////////////////
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS

  template <typename FutureExecutorContinuationSharedState>
  struct run_it {
    shared_ptr<FutureExecutorContinuationSharedState> that_;

#if ! defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(run_it)
      run_it(run_it const& x) //HBOOST_NOEXCEPT
      : that_(x.that_)
      {}
      run_it& operator=(HBOOST_THREAD_COPY_ASSIGN_REF(run_it) x) //HBOOST_NOEXCEPT
      {
        if (this != &x) {
          that_=x.that_;
        }
        return *this;
      }
      // move
      run_it(HBOOST_THREAD_RV_REF(run_it) x) HBOOST_NOEXCEPT
      : that_(x.that_)
      {
        x.that_.reset();
      }
      run_it& operator=(HBOOST_THREAD_RV_REF(run_it) x) HBOOST_NOEXCEPT {
        if (this != &x) {
          that_=x.that;
          x.that_.reset();
        }
        return *this;
      }
#endif
    run_it(shared_ptr<FutureExecutorContinuationSharedState> that) : that_ (that) {}

    void operator()()
    {
      that_->run(that_);
    }
  };

}
  HBOOST_THREAD_DCL_MOVABLE_BEG(F) detail::run_it<F> HBOOST_THREAD_DCL_MOVABLE_END

namespace detail {

  template<typename F, typename Rp, typename Fp>
  struct future_executor_continuation_shared_state: continuation_shared_state<F,Rp,Fp>
  {
    typedef continuation_shared_state<F,Rp,Fp> base_type;

  public:
    future_executor_continuation_shared_state(HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c)
    : base_type(hboost::move(f), hboost::forward<Fp>(c))
    {
    }

    template <class Ex>
    void init(hboost::unique_lock<hboost::mutex> &lk, Ex& ex)
    {
      this->set_executor_policy(executor_ptr_type(new executor_ref<Ex>(ex)), lk);
      this->base_type::init(lk);
    }

    void launch_continuation() {
      run_it<base_type> fct(static_shared_from_this(this));
      this->get_executor()->submit(hboost::move(fct));
    }

    ~future_executor_continuation_shared_state() {}
  };
#endif

  /////////////////////////
  /// shared_future_async_continuation_shared_state
  /////////////////////////

  template<typename F, typename Rp, typename Fp>
  struct shared_future_async_continuation_shared_state: continuation_shared_state<F,Rp,Fp,future_async_shared_state_base<Rp> >
  {
    typedef continuation_shared_state<F,Rp,Fp,future_async_shared_state_base<Rp> > base_type;

  public:
    shared_future_async_continuation_shared_state(F f, HBOOST_THREAD_FWD_REF(Fp) c)
    : base_type(hboost::move(f), hboost::forward<Fp>(c))
    {
    }

    void launch_continuation() {
#if defined HBOOST_THREAD_FUTURE_BLOCKING
      hboost::lock_guard<hboost::mutex> lk(this->mutex);
      this->thr_ = thread(&base_type::run, static_shared_from_this(this));
#else
      thread(&base_type::run, static_shared_from_this(this)).detach();
#endif
    }
  };

  /////////////////////////
  /// shared_future_async_continuation_shared_state
  /////////////////////////

  template<typename F, typename Rp, typename Fp>
  struct shared_future_sync_continuation_shared_state: continuation_shared_state<F,Rp,Fp,shared_state<Rp> >
  {
    typedef continuation_shared_state<F,Rp,Fp,shared_state<Rp> > base_type;

  public:
    shared_future_sync_continuation_shared_state(F f, HBOOST_THREAD_FWD_REF(Fp) c)
    : base_type(hboost::move(f), hboost::forward<Fp>(c))
    {
    }

    void launch_continuation() {
      this->call();
    }
  };


  /////////////////////////
  /// shared_future_executor_continuation_shared_state
  /////////////////////////
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS

  template<typename F, typename Rp, typename Fp>
  struct shared_future_executor_continuation_shared_state: continuation_shared_state<F,Rp,Fp>
  {
    typedef continuation_shared_state<F,Rp,Fp> base_type;

  public:

    shared_future_executor_continuation_shared_state(F f, HBOOST_THREAD_FWD_REF(Fp) c)
    : base_type(hboost::move(f), hboost::forward<Fp>(c))
    {
    }

    template <class Ex>
    void init(hboost::unique_lock<hboost::mutex> &lk, Ex& ex)
    {
      this->set_executor_policy(executor_ptr_type(new executor_ref<Ex>(ex)), lk);
      this->base_type::init(lk);
    }

    void launch_continuation() {
      run_it<base_type> fct(static_shared_from_this(this));
      this->get_executor()->submit(hboost::move(fct));
    }

    ~shared_future_executor_continuation_shared_state() {}
  };

#endif
  //////////////////////////
  /// future_deferred_continuation_shared_state
  //////////////////////////
  template<typename F, typename Rp, typename Fp>
  struct future_deferred_continuation_shared_state: continuation_shared_state<F,Rp,Fp>
  {
    typedef continuation_shared_state<F,Rp,Fp> base_type;
  public:
    future_deferred_continuation_shared_state(HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c)
    : base_type(hboost::move(f), hboost::forward<Fp>(c))
    {
      this->set_deferred();
    }

    virtual void execute(hboost::unique_lock<hboost::mutex>& lk) {
      this->parent.wait();
      this->call(lk);
    }

    virtual void launch_continuation() {    }
  };

  //////////////////////////
  /// shared_future_deferred_continuation_shared_state
  //////////////////////////
  template<typename F, typename Rp, typename Fp>
  struct shared_future_deferred_continuation_shared_state: continuation_shared_state<F,Rp,Fp>
  {
    typedef continuation_shared_state<F,Rp,Fp> base_type;

  public:
    shared_future_deferred_continuation_shared_state(F f, HBOOST_THREAD_FWD_REF(Fp) c)
    : base_type(hboost::move(f), hboost::forward<Fp>(c))
    {
      this->set_deferred();
    }

    virtual void execute(hboost::unique_lock<hboost::mutex>& lk) {
      this->parent.wait();
      this->call(lk);
    }

    virtual void launch_continuation() {    }
  };

  ////////////////////////////////
  // make_future_deferred_continuation_shared_state
  ////////////////////////////////
  template<typename F, typename Rp, typename Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_future_deferred_continuation_shared_state(
      hboost::unique_lock<hboost::mutex> &lock,
      HBOOST_THREAD_RV_REF(F) f, HBOOST_THREAD_FWD_REF(Fp) c) {
    typedef typename decay<Fp>::type Cont;
    shared_ptr<future_deferred_continuation_shared_state<F, Rp, Cont> >
        h(new future_deferred_continuation_shared_state<F, Rp, Cont>(hboost::move(f), hboost::forward<Fp>(c)));
    h->init(lock);
    return HBOOST_THREAD_FUTURE<Rp>(h);
  }

  ////////////////////////////////
  // make_future_async_continuation_shared_state
  ////////////////////////////////
  template<typename F, typename Rp, typename Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_future_async_continuation_shared_state(
      hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f,
      HBOOST_THREAD_FWD_REF(Fp) c) {
    typedef typename decay<Fp>::type Cont;
    shared_ptr<future_async_continuation_shared_state<F,Rp, Cont> >
        h(new future_async_continuation_shared_state<F,Rp, Cont>(hboost::move(f), hboost::forward<Fp>(c)));
    h->init(lock);

    return HBOOST_THREAD_FUTURE<Rp>(h);
  }
  ////////////////////////////////
  // make_future_sync_continuation_shared_state
  ////////////////////////////////
  template<typename F, typename Rp, typename Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_future_sync_continuation_shared_state(
      hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f,
      HBOOST_THREAD_FWD_REF(Fp) c) {
    typedef typename decay<Fp>::type Cont;
    shared_ptr<future_sync_continuation_shared_state<F,Rp, Cont> >
        h(new future_sync_continuation_shared_state<F,Rp, Cont>(hboost::move(f), hboost::forward<Fp>(c)));
    h->init(lock);

    return HBOOST_THREAD_FUTURE<Rp>(h);
  }

  ////////////////////////////////
  // make_future_executor_continuation_shared_state
  ////////////////////////////////
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS

  template<typename Ex, typename F, typename Rp, typename Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_future_executor_continuation_shared_state(Ex& ex,
      hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f,
      HBOOST_THREAD_FWD_REF(Fp) c) {
    typedef typename decay<Fp>::type Cont;
    shared_ptr<future_executor_continuation_shared_state<F,Rp, Cont> >
        h(new future_executor_continuation_shared_state<F,Rp, Cont>(hboost::move(f), hboost::forward<Fp>(c)));
    h->init(lock, ex);

    return HBOOST_THREAD_FUTURE<Rp>(h);
  }
#endif

  ////////////////////////////////
  // make_shared_future_deferred_continuation_shared_state
  ////////////////////////////////
  template<typename F, typename Rp, typename Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_shared_future_deferred_continuation_shared_state(
      hboost::unique_lock<hboost::mutex> &lock,
      F f, HBOOST_THREAD_FWD_REF(Fp) c) {
    typedef typename decay<Fp>::type Cont;
    shared_ptr<shared_future_deferred_continuation_shared_state<F, Rp, Cont> >
        h(new shared_future_deferred_continuation_shared_state<F, Rp, Cont>(f, hboost::forward<Fp>(c)));
    h->init(lock);

    return HBOOST_THREAD_FUTURE<Rp>(h);
  }
  ////////////////////////////////
  // make_shared_future_async_continuation_shared_state
  ////////////////////////////////
  template<typename F, typename Rp, typename Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_shared_future_async_continuation_shared_state(
      hboost::unique_lock<hboost::mutex> &lock, F f,
      HBOOST_THREAD_FWD_REF(Fp) c) {
    typedef typename decay<Fp>::type Cont;
    shared_ptr<shared_future_async_continuation_shared_state<F,Rp, Cont> >
        h(new shared_future_async_continuation_shared_state<F,Rp, Cont>(f, hboost::forward<Fp>(c)));
    h->init(lock);

    return HBOOST_THREAD_FUTURE<Rp>(h);
  }
  ////////////////////////////////
  // make_shared_future_sync_continuation_shared_state
  ////////////////////////////////
  template<typename F, typename Rp, typename Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_shared_future_sync_continuation_shared_state(
      hboost::unique_lock<hboost::mutex> &lock, F f,
      HBOOST_THREAD_FWD_REF(Fp) c) {
    typedef typename decay<Fp>::type Cont;
    shared_ptr<shared_future_sync_continuation_shared_state<F,Rp, Cont> >
        h(new shared_future_sync_continuation_shared_state<F,Rp, Cont>(f, hboost::forward<Fp>(c)));
    h->init(lock);

    return HBOOST_THREAD_FUTURE<Rp>(h);
  }
  ////////////////////////////////
  // make_shared_future_executor_continuation_shared_state
  ////////////////////////////////
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
  template<typename Ex, typename F, typename Rp, typename Fp>
  HBOOST_THREAD_FUTURE<Rp>
  make_shared_future_executor_continuation_shared_state(Ex& ex,
      hboost::unique_lock<hboost::mutex> &lock, F f,
      HBOOST_THREAD_FWD_REF(Fp) c) {
    typedef typename decay<Fp>::type Cont;
    shared_ptr<shared_future_executor_continuation_shared_state<F, Rp, Cont> >
        h(new shared_future_executor_continuation_shared_state<F, Rp, Cont>(f, hboost::forward<Fp>(c)));
    h->init(lock, ex);

    return HBOOST_THREAD_FUTURE<Rp>(h);
  }
#endif
}

  ////////////////////////////////
  // template<typename F>
  // auto future<R>::then(launch policy, F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R>
  template <typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type>
  HBOOST_THREAD_FUTURE<R>::then(launch policy, HBOOST_THREAD_FWD_REF(F) func) {
    typedef typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    // keep state alive as we move ourself but hold the lock
    shared_ptr<detail::shared_state_base> sentinel(this->future_);
    hboost::unique_lock<hboost::mutex> lock(sentinel->mutex);

    if (underlying_cast<int>(policy) & int(launch::async)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_deferred_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
    } else if (underlying_cast<int>(policy) & int(launch::executor)) {
      assert(this->future_->get_executor());
      typedef executor Ex;
      Ex& ex = *(this->future_->get_executor());
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_executor_continuation_shared_state<Ex, HBOOST_THREAD_FUTURE<R>, future_type>(ex,
                    lock, hboost::move(*this), hboost::forward<F>(func)
                )));
#endif
    } else if (underlying_cast<int>(policy) & int(launch::inherit)) {

        launch policy = this->launch_policy(lock);
        if (underlying_cast<int>(policy) & int(launch::async)) {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                      lock, hboost::move(*this), hboost::forward<F>(func)
                  )));
        } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_deferred_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                      lock, hboost::move(*this), hboost::forward<F>(func)
                  )));
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        } else if (underlying_cast<int>(policy) & int(launch::executor)) {
          assert(this->future_->get_executor());
          typedef executor Ex;
          Ex& ex = *(this->future_->get_executor());
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_executor_continuation_shared_state<Ex, HBOOST_THREAD_FUTURE<R>, future_type>(ex,
                        lock, hboost::move(*this), hboost::forward<F>(func)
                    )));
#endif
        } else {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                      lock, hboost::move(*this), hboost::forward<F>(func)
                  )));
        }
    } else {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    }
  }
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
  ////////////////////////////////
  // template<typename Ex, typename F>
  // auto future<future<R2> >::then(Ex&, F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R>
  template <typename Ex, typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type>
  HBOOST_THREAD_FUTURE<R>::then(Ex& ex, HBOOST_THREAD_FWD_REF(F) func) {
    typedef typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    // keep state alive as we move ourself but hold the lock
    shared_ptr<detail::shared_state_base> sentinel(this->future_);
    hboost::unique_lock<hboost::mutex> lock(sentinel->mutex);

    return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_executor_continuation_shared_state<Ex, HBOOST_THREAD_FUTURE<R>, future_type>(ex,
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
  }
#endif
  ////////////////////////////////
  // template<typename F>
  // auto future<future<R2> >::then(F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R>
  template <typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type>
  HBOOST_THREAD_FUTURE<R>::then(HBOOST_THREAD_FWD_REF(F) func)  {

#ifndef HBOOST_THREAD_CONTINUATION_SYNC
    return this->then(this->launch_policy(), hboost::forward<F>(func));
#else
    typedef typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    // keep state alive as we move ourself but hold the lock
    shared_ptr<detail::shared_state_base> sentinel(this->future_);
    hboost::unique_lock<hboost::mutex> lock(sentinel->mutex);

    launch policy = this->launch_policy(lock);
    if (underlying_cast<int>(policy) & int(launch::deferred)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_deferred_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    } else {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    }
#endif

  }

  ////////////////////////////////
  // template<typename F>
  // auto future<future<R2> >::then(launch, F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R2>
  template <typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> >)>::type>
  HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> >::then(launch policy, HBOOST_THREAD_FWD_REF(F) func) {
    typedef HBOOST_THREAD_FUTURE<R2> R;
    typedef typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    // keep state alive as we move ourself but hold the lock
    shared_ptr<detail::shared_state_base> sentinel(this->future_);
    hboost::unique_lock<hboost::mutex> lock(sentinel->mutex);

    if (underlying_cast<int>(policy) & int(launch::async)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_deferred_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    } else if (underlying_cast<int>(policy) & int(launch::sync)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_sync_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
    } else if (underlying_cast<int>(policy) & int(launch::executor)) {
      assert(this->future_->get_executor());
      typedef executor Ex;
      Ex& ex = *(this->future_->get_executor());
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_executor_continuation_shared_state<Ex, HBOOST_THREAD_FUTURE<R>, future_type>(ex,
                    lock, hboost::move(*this), hboost::forward<F>(func)
                )));
#endif
    } else if (underlying_cast<int>(policy) & int(launch::inherit)) {
        launch policy = this->launch_policy(lock);

        if (underlying_cast<int>(policy) & int(launch::async)) {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                      lock, hboost::move(*this), hboost::forward<F>(func)
                  )));
        } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_deferred_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                      lock, hboost::move(*this), hboost::forward<F>(func)
                  )));
        } else if (underlying_cast<int>(policy) & int(launch::sync)) {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_sync_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                      lock, hboost::move(*this), hboost::forward<F>(func)
                  )));
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        } else if (underlying_cast<int>(policy) & int(launch::executor)) {
          assert(this->future_->get_executor());
          typedef executor Ex;
          Ex& ex = *(this->future_->get_executor());
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_executor_continuation_shared_state<Ex, HBOOST_THREAD_FUTURE<R>, future_type>(ex,
                        lock, hboost::move(*this), hboost::forward<F>(func)
                    )));
#endif
        } else {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                      lock, hboost::move(*this), hboost::forward<F>(func)
                  )));
        }
    } else {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_async_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    }
  }

#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
  ////////////////////////////////
  // template<typename Ex, typename F>
  // auto future<future<R2> >::then(Ex&, F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R2>
  template <typename Ex, typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> >)>::type>
  HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> >::then(Ex& ex, HBOOST_THREAD_FWD_REF(F) func) {
    typedef HBOOST_THREAD_FUTURE<R2> R;
    typedef typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    // keep state alive as we move ourself but hold the lock
    shared_ptr<detail::shared_state_base> sentinel(this->future_);
    hboost::unique_lock<hboost::mutex> lock(sentinel->mutex);

    return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_executor_continuation_shared_state<Ex, HBOOST_THREAD_FUTURE<R>, future_type>(ex,
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
  }
#endif

  ////////////////////////////////
  // template<typename F>
  // auto future<future<R2> >::then(F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R2>
  template <typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> >)>::type>
  HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> >::then(HBOOST_THREAD_FWD_REF(F) func)  {

#ifndef HBOOST_THREAD_CONTINUATION_SYNC
    return this->then(this->launch_policy(), hboost::forward<F>(func));
#else
    typedef HBOOST_THREAD_FUTURE<R2> R;
    typedef typename hboost::result_of<F(HBOOST_THREAD_FUTURE<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    // keep state alive as we move ourself but hold the lock
    shared_ptr<detail::shared_state_base> sentinel(this->future_);
    hboost::unique_lock<hboost::mutex> lock(sentinel->mutex);

    launch policy = this->launch_policy(lock);

    if  (underlying_cast<int>(policy) & int(launch::deferred)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_deferred_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    } else {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_future_sync_continuation_shared_state<HBOOST_THREAD_FUTURE<R>, future_type>(
                  lock, hboost::move(*this), hboost::forward<F>(func)
              )));
    }
#endif
  }

  ////////////////////////////////
  // template<typename F>
  // auto shared_future<R>::then(launch policy, F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R>
  template <typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(shared_future<R>)>::type>
  shared_future<R>::then(launch policy, HBOOST_THREAD_FWD_REF(F) func)  const
  {
    typedef typename hboost::result_of<F(shared_future<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    hboost::unique_lock<hboost::mutex> lock(this->future_->mutex);
    if (underlying_cast<int>(policy) & int(launch::async)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_async_continuation_shared_state<shared_future<R>, future_type>(
                  lock, *this, hboost::forward<F>(func)
              )));
    } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_deferred_continuation_shared_state<shared_future<R>, future_type>(
                  lock, *this, hboost::forward<F>(func)
              )));
    } else if (underlying_cast<int>(policy) & int(launch::sync)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_sync_continuation_shared_state<shared_future<R>, future_type>(
                  lock, *this, hboost::forward<F>(func)
              )));
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
    } else if (underlying_cast<int>(policy) & int(launch::executor)) {
      typedef executor Ex;
      Ex& ex = *(this->future_->get_executor());
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_executor_continuation_shared_state<Ex, shared_future<R>, future_type>(ex,
                    lock, *this, hboost::forward<F>(func)
                )));
#endif
    } else if (underlying_cast<int>(policy) & int(launch::inherit)) {

        launch policy = this->launch_policy(lock);
        if (underlying_cast<int>(policy) & int(launch::async)) {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_async_continuation_shared_state<shared_future<R>, future_type>(
                      lock, *this, hboost::forward<F>(func)
                  )));
        } else if (underlying_cast<int>(policy) & int(launch::deferred)) {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_deferred_continuation_shared_state<shared_future<R>, future_type>(
                      lock, *this, hboost::forward<F>(func)
                  )));
        } else if (underlying_cast<int>(policy) & int(launch::sync)) {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_sync_continuation_shared_state<shared_future<R>, future_type>(
                      lock, *this, hboost::forward<F>(func)
                  )));
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
        } else if (underlying_cast<int>(policy) & int(launch::executor)) {
          typedef executor Ex;
          Ex& ex = *(this->future_->get_executor());
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_executor_continuation_shared_state<Ex, shared_future<R>, future_type>(ex,
                        lock, *this, hboost::forward<F>(func)
                    )));
#endif
        } else {
          return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_async_continuation_shared_state<shared_future<R>, future_type>(
                      lock, *this, hboost::forward<F>(func)
                  )));
        }

    } else {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_async_continuation_shared_state<shared_future<R>, future_type>(
                  lock, *this, hboost::forward<F>(func)
              )));
    }
  }
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
  ////////////////////////////////
  // template<typename Ex, typename F>
  // auto shared_future<R>::then(Ex&, F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R>
  template <typename Ex, typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(shared_future<R>)>::type>
  shared_future<R>::then(Ex& ex, HBOOST_THREAD_FWD_REF(F) func)  const
  {
    typedef typename hboost::result_of<F(shared_future<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    hboost::unique_lock<hboost::mutex> lock(this->future_->mutex);
    return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_executor_continuation_shared_state<Ex, shared_future<R>, future_type>(ex,
                  lock, *this, hboost::forward<F>(func)
              )));
  }
#endif

  ////////////////////////////////
  // template<typename F>
  // auto shared_future<R>::then(F&& func) -> HBOOST_THREAD_FUTURE<decltype(func(*this))>;
  ////////////////////////////////
  template <typename R>
  template <typename F>
  inline HBOOST_THREAD_FUTURE<typename hboost::result_of<F(shared_future<R>)>::type>
  shared_future<R>::then(HBOOST_THREAD_FWD_REF(F) func)  const {
#ifndef HBOOST_THREAD_CONTINUATION_SYNC
    return this->then(this->launch_policy(), hboost::forward<F>(func));
#else
    typedef typename hboost::result_of<F(shared_future<R>)>::type future_type;
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    hboost::unique_lock<hboost::mutex> lock(this->future_->mutex);
    launch policy = this->launch_policy(lock);
    if (underlying_cast<int>(policy) & int(launch::deferred)) {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_deferred_continuation_shared_state<shared_future<R>, future_type>(
                  lock, *this, hboost::forward<F>(func)
              )));
    } else {
      return HBOOST_THREAD_MAKE_RV_REF((hboost::detail::make_shared_future_sync_continuation_shared_state<shared_future<R>, future_type>(
                  lock, *this, hboost::forward<F>(func)
              )));
    }
#endif
  }

namespace detail
{
  template <typename T>
  struct mfallbacker_to
  {
    T value_;
    typedef T result_type;
    mfallbacker_to(HBOOST_THREAD_RV_REF(T) v)
    : value_(hboost::move(v))
    {}

    T operator()(HBOOST_THREAD_FUTURE<T> fut) {
      return fut.get_or(hboost::move(value_));
    }
  };
  template <typename T>
  struct cfallbacker_to
  {
    T value_;
    typedef T result_type;
    cfallbacker_to(T const& v)
    : value_(v)
    {}

    T operator()(HBOOST_THREAD_FUTURE<T> fut) const {
      return fut.get_or(value_);

    }
  };
}
  ////////////////////////////////
  // future<R> future<R>::fallback_to(R&& v);
  ////////////////////////////////

  template <typename R>
  template <typename R2>
  inline typename hboost::disable_if< is_void<R2>, HBOOST_THREAD_FUTURE<R> >::type
  HBOOST_THREAD_FUTURE<R>::fallback_to(HBOOST_THREAD_RV_REF(R2) v) {
    return then(detail::mfallbacker_to<R>(hboost::move(v)));
  }

  template <typename R>
  template <typename R2>
  inline typename hboost::disable_if< is_void<R2>, HBOOST_THREAD_FUTURE<R> >::type
  HBOOST_THREAD_FUTURE<R>::fallback_to(R2 const& v) {
    return then(detail::cfallbacker_to<R>(v));
  }

#endif

#if defined HBOOST_THREAD_PROVIDES_FUTURE_UNWRAP
namespace detail
{
  /////////////////////////
  /// future_unwrap_shared_state
  /////////////////////////

  template<typename F, typename Rp>
  struct future_unwrap_shared_state: shared_state<Rp>
  {
    F wrapped;
    typename F::value_type unwrapped;
  public:
    explicit future_unwrap_shared_state(HBOOST_THREAD_RV_REF(F) f)
    : wrapped(hboost::move(f)) {
    }

    void launch_continuation()
    {
      hboost::unique_lock<hboost::mutex> lk(this->mutex);
      // assert(wrapped.is_ready());
      if (! unwrapped.valid() )
      {
        if (wrapped.has_exception()) {
          this->mark_exceptional_finish_internal(wrapped.get_exception_ptr(), lk);
        } else {
          unwrapped = wrapped.get();
          if (unwrapped.valid())
          {
            lk.unlock();
            hboost::unique_lock<hboost::mutex> lk2(unwrapped.future_->mutex);
            unwrapped.future_->set_continuation_ptr(this->shared_from_this(), lk2);
          } else {
            this->mark_exceptional_finish_internal(hboost::copy_exception(future_uninitialized()), lk);
          }
        }
      } else {
        // assert(unwrapped.is_ready());
        if (unwrapped.has_exception()) {
          this->mark_exceptional_finish_internal(unwrapped.get_exception_ptr(), lk);
        } else {
          this->mark_finished_with_result_internal(unwrapped.get(), lk);
        }
      }
    }
  };

  template<typename F>
  struct future_unwrap_shared_state<F,void>: shared_state<void>
  {
    F wrapped;
    typename F::value_type unwrapped;
  public:
    explicit future_unwrap_shared_state(HBOOST_THREAD_RV_REF(F) f)
    : wrapped(hboost::move(f)) {
    }

    void launch_continuation()
    {
      hboost::unique_lock<hboost::mutex> lk(this->mutex);
      // assert(wrapped.is_ready());
      if (! unwrapped.valid() )
      {
        if (wrapped.has_exception()) {
          this->mark_exceptional_finish_internal(wrapped.get_exception_ptr(), lk);
        } else {
          unwrapped = wrapped.get();
          if (unwrapped.valid())
          {
            lk.unlock();
            hboost::unique_lock<hboost::mutex> lk2(unwrapped.future_->mutex);
            unwrapped.future_->set_continuation_ptr(this->shared_from_this(), lk2);
          } else {
            this->mark_exceptional_finish_internal(hboost::copy_exception(future_uninitialized()), lk);
          }
        }
      } else {
        // assert(unwrapped.is_ready());
        if (unwrapped.has_exception()) {
          this->mark_exceptional_finish_internal(unwrapped.get_exception_ptr(), lk);
        } else {
          this->mark_finished_with_result_internal(lk);
        }
      }
    }
  };

  template <class F, class Rp>
  HBOOST_THREAD_FUTURE<Rp>
  make_future_unwrap_shared_state(hboost::unique_lock<hboost::mutex> &lock, HBOOST_THREAD_RV_REF(F) f) {
    shared_ptr<future_unwrap_shared_state<F, Rp> >
        h(new future_unwrap_shared_state<F, Rp>(hboost::move(f)));
    h->wrapped.future_->set_continuation_ptr(h, lock);

    return HBOOST_THREAD_FUTURE<Rp>(h);
  }
}

  template <typename R>
  inline HBOOST_THREAD_FUTURE<R>::HBOOST_THREAD_FUTURE(HBOOST_THREAD_RV_REF(HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R> >) other)
  : base_type(other.unwrap()) {}

  template <typename R2>
  HBOOST_THREAD_FUTURE<R2>
  HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> >::unwrap()
  {
    HBOOST_THREAD_ASSERT_PRECONDITION(this->future_!=0, future_uninitialized());

    // keep state alive as we move ourself but hold the lock
    shared_ptr<detail::shared_state_base> sentinel(this->future_);
    hboost::unique_lock<hboost::mutex> lock(sentinel->mutex);

    return hboost::detail::make_future_unwrap_shared_state<HBOOST_THREAD_FUTURE<HBOOST_THREAD_FUTURE<R2> >, R2>(lock, hboost::move(*this));
  }
#endif

#if defined HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
namespace detail
{
  struct input_iterator_tag {};
  struct vector_tag {};
  struct values_tag {};
  template <typename T>
  struct alias_t { typedef T type; };

  HBOOST_CONSTEXPR_OR_CONST input_iterator_tag input_iterator_tag_value = {};
  HBOOST_CONSTEXPR_OR_CONST vector_tag vector_tag_value = {};
  HBOOST_CONSTEXPR_OR_CONST values_tag values_tag_value = {};
  ////////////////////////////////
  // detail::future_async_when_all_shared_state
  ////////////////////////////////
  template<typename F>
  struct future_when_all_vector_shared_state: future_async_shared_state_base<csbl::vector<F> >
  {
    typedef csbl::vector<F> vector_type;
    typedef typename F::value_type value_type;
    vector_type vec_;

    static void run(shared_ptr<hboost::detail::shared_state_base> that_) {
      future_when_all_vector_shared_state* that = static_cast<future_when_all_vector_shared_state*>(that_.get());
      try {
        hboost::wait_for_all(that->vec_.begin(), that->vec_.end());
        that->mark_finished_with_result(hboost::move(that->vec_));
      } catch(...) {
        that->mark_exceptional_finish();
      }
    }
    bool run_deferred() {

      bool res = false;
      for (typename csbl::vector<F>::iterator it = vec_.begin(); it != vec_.end(); ++it) {
        if (! it->run_if_is_deferred())
        {
          res = true;
        }
      }
      return res;
    }
    void init() {
      if (! run_deferred())
      {
        future_when_all_vector_shared_state::run(this->shared_from_this());
        return;
      }
#ifdef HBOOST_THREAD_FUTURE_BLOCKING
      this->thr_ = thread(&future_when_all_vector_shared_state::run, this->shared_from_this());
#else
      thread(&future_when_all_vector_shared_state::run, this->shared_from_this()).detach();
#endif
    }

  public:
    template< typename InputIterator>
    future_when_all_vector_shared_state(input_iterator_tag, InputIterator first, InputIterator last)
    : vec_(std::make_move_iterator(first), std::make_move_iterator(last))
    {
    }

    future_when_all_vector_shared_state(vector_tag, HBOOST_THREAD_RV_REF(csbl::vector<F>) v)
    : vec_(hboost::move(v))
    {
    }

#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template< typename T0, typename ...T>
    future_when_all_vector_shared_state(values_tag, HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures) {
      vec_.push_back(hboost::forward<T0>(f));
      typename alias_t<char[]>::type{
          ( //first part of magic unpacker
          vec_.push_back(hboost::forward<T>(futures)),'0'
          )..., '0'
      }; //second part of magic unpacker
    }
#endif

    ~future_when_all_vector_shared_state() {}
  };

  ////////////////////////////////
  // detail::future_async_when_any_shared_state
  ////////////////////////////////
  template<typename F>
  struct future_when_any_vector_shared_state: future_async_shared_state_base<csbl::vector<F> >
  {
    typedef csbl::vector<F> vector_type;
    typedef typename F::value_type value_type;
    vector_type vec_;

    static void run(shared_ptr<hboost::detail::shared_state_base> that_)
    {
      future_when_any_vector_shared_state* that = static_cast<future_when_any_vector_shared_state*>(that_.get());
      try {
        hboost::wait_for_any(that->vec_.begin(), that->vec_.end());
        that->mark_finished_with_result(hboost::move(that->vec_));
      } catch(...) {
        that->mark_exceptional_finish();
      }
    }
    bool run_deferred() {

      for (typename csbl::vector<F>::iterator it = vec_.begin(); it != vec_.end(); ++it) {
        if (it->run_if_is_deferred_or_ready())
        {
          return true;
        }
      }
      return false;
    }
    void init() {
      if (run_deferred())
      {
        future_when_any_vector_shared_state::run(this->shared_from_this());
        return;
      }

#ifdef HBOOST_THREAD_FUTURE_BLOCKING
      this->thr_ = thread(&future_when_any_vector_shared_state::run, this->shared_from_this());
#else
      thread(&future_when_any_vector_shared_state::run, this->shared_from_this()).detach();
#endif
    }

  public:
    template< typename InputIterator>
    future_when_any_vector_shared_state(input_iterator_tag, InputIterator first, InputIterator last)
    : vec_(std::make_move_iterator(first), std::make_move_iterator(last))
    {
    }

    future_when_any_vector_shared_state(vector_tag, HBOOST_THREAD_RV_REF(csbl::vector<F>) v)
    : vec_(hboost::move(v))
    {
    }

#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template< typename T0, typename ...T>
    future_when_any_vector_shared_state(values_tag,
        HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures
    ) {
      vec_.push_back(hboost::forward<T0>(f));
      typename alias_t<char[]>::type{
          ( //first part of magic unpacker
          vec_.push_back(hboost::forward<T>(futures))
          ,'0'
          )...,
          '0'
      }; //second part of magic unpacker
    }
#endif

    ~future_when_any_vector_shared_state() {}
  };

#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
  struct wait_for_all_fctr {
    template <class ...T>
    void operator()(T&&... v) {
      hboost::wait_for_all(hboost::forward<T>(v)...);
    }
  };

  struct wait_for_any_fctr {
    template <class ...T>
    void operator()(T&&... v) {
      hboost::wait_for_any(hboost::forward<T>(v)...);
    }
  };


  template <class Tuple, std::size_t i=csbl::tuple_size<Tuple>::value>
  struct accumulate_run_if_is_deferred {
    bool operator ()(Tuple& t)
    {
      return (! csbl::get<i-1>(t).run_if_is_deferred()) || accumulate_run_if_is_deferred<Tuple,i-1>()(t);
    }
  };
  template <class Tuple>
  struct accumulate_run_if_is_deferred<Tuple, 0> {
    bool operator ()(Tuple& )
    {
      return false;
    }
  };


  template< typename Tuple, typename T0, typename ...T>
  struct future_when_all_tuple_shared_state: future_async_shared_state_base<Tuple>
  {
    Tuple tup_;
    typedef typename make_tuple_indices<1+sizeof...(T)>::type Index;

    static void run(shared_ptr<hboost::detail::shared_state_base> that_) {
      future_when_all_tuple_shared_state* that = static_cast<future_when_all_tuple_shared_state*>(that_.get());
      try {
        // TODO make use of apply(that->tup_, hboost::detail::wait_for_all_fctor());
        that->wait_for_all(Index());

        that->mark_finished_with_result(hboost::move(that->tup_));
      } catch(...) {
        that->mark_exceptional_finish();
      }
    }

    template <size_t ...Indices>
    void wait_for_all(tuple_indices<Indices...>) {
#if defined HBOOST_THREAD_PROVIDES_INVOKE
      return invoke<void>(wait_for_all_fctr(), csbl::get<Indices>(tup_)...);
#else
      return wait_for_all_fctr()(csbl::get<Indices>(tup_)...);
#endif
    }

    bool run_deferred() {

      return accumulate_run_if_is_deferred<Tuple>()(tup_);
    }
    void init() {
      if (! run_deferred())
      {
        future_when_all_tuple_shared_state::run(this->shared_from_this());
        return;
      }
#ifdef HBOOST_THREAD_FUTURE_BLOCKING
      this->thr_ = thread(&future_when_all_tuple_shared_state::run, this->shared_from_this());
#else
      thread(&future_when_all_tuple_shared_state::run, this->shared_from_this()).detach();
#endif

    }
  public:
    template< typename F, typename ...Fs>
    future_when_all_tuple_shared_state(values_tag, HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(Fs) ... futures) :
      tup_(hboost::csbl::make_tuple(hboost::forward<F>(f), hboost::forward<Fs>(futures)...))
    {
    }

    ~future_when_all_tuple_shared_state() {}

  };


  template <class Tuple, std::size_t i=csbl::tuple_size<Tuple>::value>
  struct apply_any_run_if_is_deferred_or_ready {
    bool operator ()(Tuple& t)
    {
      if (csbl::get<i-1>(t).run_if_is_deferred_or_ready()) return true;
      return apply_any_run_if_is_deferred_or_ready<Tuple,i-1>()(t);
    }
  };
  template <class Tuple>
  struct apply_any_run_if_is_deferred_or_ready<Tuple, 0> {
    bool operator ()(Tuple& )
    {
      return false;
    }
  };

  template< typename Tuple, typename T0, typename ...T >
  struct future_when_any_tuple_shared_state: future_async_shared_state_base<Tuple>
  {
    Tuple tup_;
    typedef typename make_tuple_indices<1+sizeof...(T)>::type Index;

    static void run(shared_ptr<hboost::detail::shared_state_base> that_)
    {
      future_when_any_tuple_shared_state* that = static_cast<future_when_any_tuple_shared_state*>(that_.get());
      try {
        // TODO make use of apply(that->tup_, wait_for_any_fctr);
        that->wait_for_any(Index());

        that->mark_finished_with_result(hboost::move(that->tup_));
      } catch(...) {
        that->mark_exceptional_finish();
      }
    }
    template <size_t ...Indices>
    void wait_for_any(tuple_indices<Indices...>) {
#if defined HBOOST_THREAD_PROVIDES_INVOKE
      return invoke<void>(wait_for_any_fctr(), csbl::get<Indices>(tup_)...);
#else
      return wait_for_any_fctr()(csbl::get<Indices>(tup_)...);
#endif
    }
    bool run_deferred() {
      return apply_any_run_if_is_deferred_or_ready<Tuple>()(tup_);
    }
    void init() {
      if (run_deferred())
      {
        future_when_any_tuple_shared_state::run(this->shared_from_this());
        return;
      }

#ifdef HBOOST_THREAD_FUTURE_BLOCKING
      this->thr_ = thread(&future_when_any_tuple_shared_state::run, this->shared_from_this());
#else
      thread(&future_when_any_tuple_shared_state::run, this->shared_from_this()).detach();
#endif
    }

  public:
    template< typename F, typename ...Fs>
    future_when_any_tuple_shared_state(values_tag,
        HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(Fs) ... futures
    ) :
      tup_(hboost::csbl::make_tuple(hboost::forward<F>(f), hboost::forward<Fs>(futures)...))
    {
    }

    ~future_when_any_tuple_shared_state() {}
  };
#endif

}

  template< typename InputIterator>
  typename hboost::disable_if<is_future_type<InputIterator>,
    HBOOST_THREAD_FUTURE<csbl::vector<typename InputIterator::value_type>  >
  >::type
  when_all(InputIterator first, InputIterator last) {
    typedef  typename InputIterator::value_type value_type;
    typedef  csbl::vector<value_type> container_type;
    typedef  detail::future_when_all_vector_shared_state<value_type> factory_type;

    if (first==last) return make_ready_future(container_type());
    shared_ptr<factory_type >
        h(new factory_type(detail::input_iterator_tag_value, first,last));
    h->init();
    return HBOOST_THREAD_FUTURE<container_type>(h);
  }

  inline HBOOST_THREAD_FUTURE<csbl::tuple<> > when_all() {
    return make_ready_future(csbl::tuple<>());
  }

#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template< typename T0, typename ...T>
  HBOOST_THREAD_FUTURE<csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> >
  when_all(HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures) {
    typedef csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> container_type;
    typedef detail::future_when_all_tuple_shared_state<container_type, typename decay<T0>::type, typename decay<T>::type...> factory_type;

    shared_ptr<factory_type>
        h(new factory_type(detail::values_tag_value, hboost::forward<T0>(f), hboost::forward<T>(futures)...));
    h->init();
    return HBOOST_THREAD_FUTURE<container_type>(h);
  }
#endif

  template< typename InputIterator>
  typename hboost::disable_if<is_future_type<InputIterator>,
    HBOOST_THREAD_FUTURE<csbl::vector<typename InputIterator::value_type>  >
  >::type
  when_any(InputIterator first, InputIterator last) {
    typedef  typename InputIterator::value_type value_type;
    typedef  csbl::vector<value_type> container_type;
    typedef  detail::future_when_any_vector_shared_state<value_type> factory_type;

    if (first==last) return make_ready_future(container_type());
    shared_ptr<factory_type >
        h(new factory_type(detail::input_iterator_tag_value, first,last));
    h->init();
    return HBOOST_THREAD_FUTURE<container_type>(h);
  }

  inline HBOOST_THREAD_FUTURE<csbl::tuple<> > when_any() {
    return make_ready_future(csbl::tuple<>());
  }

#if ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template< typename T0, typename ...T>
  HBOOST_THREAD_FUTURE<csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> >
  when_any(HBOOST_THREAD_FWD_REF(T0) f, HBOOST_THREAD_FWD_REF(T) ... futures) {
    typedef csbl::tuple<typename decay<T0>::type, typename decay<T>::type...> container_type;
    typedef detail::future_when_any_tuple_shared_state<container_type, typename decay<T0>::type, typename decay<T>::type...> factory_type;

    shared_ptr<factory_type>
        h(new factory_type(detail::values_tag_value, hboost::forward<T0>(f), hboost::forward<T>(futures)...));
    h->init();
    return HBOOST_THREAD_FUTURE<container_type>(h);
  }
#endif
#endif // HBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
}

#endif // HBOOST_NO_EXCEPTION
#endif // header
