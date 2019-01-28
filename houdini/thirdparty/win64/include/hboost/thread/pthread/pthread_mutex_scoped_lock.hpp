#ifndef HBOOST_PTHREAD_MUTEX_SCOPED_LOCK_HPP
#define HBOOST_PTHREAD_MUTEX_SCOPED_LOCK_HPP
//  (C) Copyright 2007-8 Anthony Williams 
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <pthread.h>
#include <hboost/assert.hpp>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
    namespace pthread
    {
        class pthread_mutex_scoped_lock
        {
            pthread_mutex_t* m;
            bool locked;
        public:
            explicit pthread_mutex_scoped_lock(pthread_mutex_t* m_):
                m(m_),locked(true)
            {
                HBOOST_VERIFY(!pthread_mutex_lock(m));
            }
            void unlock()
            {
                HBOOST_VERIFY(!pthread_mutex_unlock(m));
                locked=false;
            }
            
            ~pthread_mutex_scoped_lock()
            {
                if(locked)
                {
                    unlock();
                }
            }
            
        };

        class pthread_mutex_scoped_unlock
        {
            pthread_mutex_t* m;
        public:
            explicit pthread_mutex_scoped_unlock(pthread_mutex_t* m_):
                m(m_)
            {
                HBOOST_VERIFY(!pthread_mutex_unlock(m));
            }
            ~pthread_mutex_scoped_unlock()
            {
                HBOOST_VERIFY(!pthread_mutex_lock(m));
            }
            
        };
    }
}

#include <hboost/config/abi_suffix.hpp>

#endif
