// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2012 Vicente J. Botet Escriba

#ifndef HBOOST_THREAD_SHARED_LOCK_GUARD_HPP
#define HBOOST_THREAD_SHARED_LOCK_GUARD_HPP
#include <hboost/thread/detail/config.hpp>
//#include <hboost/thread/locks.hpp>
#include <hboost/thread/lock_options.hpp>
#include <hboost/thread/detail/delete.hpp>

namespace hboost
{

    template<typename SharedMutex>
    class shared_lock_guard
    {
    private:
        SharedMutex& m;

    public:
        typedef SharedMutex mutex_type;
        HBOOST_THREAD_NO_COPYABLE(shared_lock_guard)
        explicit shared_lock_guard(SharedMutex& m_):
            m(m_)
        {
            m.lock_shared();
        }
        shared_lock_guard(SharedMutex& m_,adopt_lock_t):
            m(m_)
        {}
        ~shared_lock_guard()
        {
            m.unlock_shared();
        }
    };

#ifdef HBOOST_THREAD_NO_AUTO_DETECT_MUTEX_TYPES

    template<typename T>
    struct is_mutex_type<shared_lock_guard<T> >
    {
        HBOOST_STATIC_CONSTANT(bool, value = true);
    };


#endif


}

#endif // header