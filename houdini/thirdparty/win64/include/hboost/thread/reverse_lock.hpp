// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2012 Vicente J. Botet Escriba

#ifndef HBOOST_THREAD_REVERSE_LOCK_HPP
#define HBOOST_THREAD_REVERSE_LOCK_HPP
#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/lockable_traits.hpp>
#include <hboost/thread/lock_options.hpp>
#include <hboost/thread/detail/delete.hpp>

namespace hboost
{

    template<typename Lock>
    class reverse_lock
    {
    public:
        typedef typename Lock::mutex_type mutex_type;
        HBOOST_THREAD_NO_COPYABLE(reverse_lock)

        explicit reverse_lock(Lock& m_)
        : m(m_), mtx(0)
        {
            if (m.owns_lock())
            {
              m.unlock();
            }
            mtx=m.release();
        }
        ~reverse_lock()
        {
          if (mtx) {
            mtx->lock();
            m = HBOOST_THREAD_MAKE_RV_REF(Lock(*mtx, adopt_lock));
          }
        }

    private:
      Lock& m;
      mutex_type* mtx;
    };


#ifdef HBOOST_THREAD_NO_AUTO_DETECT_MUTEX_TYPES
    template<typename T>
    struct is_mutex_type<reverse_lock<T> >
    {
        HBOOST_STATIC_CONSTANT(bool, value = true);
    };

#endif


}

#endif // header
