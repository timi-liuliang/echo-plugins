//  boost thread_clock.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008
//  Copyright Vicente J. Botet Escriba 2009-2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//

#include <hboost/chrono/config.hpp>
#include <hboost/chrono/thread_clock.hpp>
#include <cassert>

#if !defined(__VXWORKS__)
# include <sys/times.h>
#endif
# include <pthread.h>
# include <unistd.h>

namespace hboost { namespace chrono {

    thread_clock::time_point thread_clock::now( ) HBOOST_NOEXCEPT
    {
      struct timespec ts;
#if defined CLOCK_THREAD_CPUTIME_ID
        // get the timespec associated to the thread clock
        if ( ::clock_gettime( CLOCK_THREAD_CPUTIME_ID, &ts ) )
#else
        // get the current thread
        pthread_t pth=pthread_self();
        // get the clock_id associated to the current thread
        clockid_t clock_id;
        pthread_getcpuclockid(pth, &clock_id);
        // get the timespec associated to the thread clock
        if ( ::clock_gettime( clock_id, &ts ) )
#endif
        {
          HBOOST_ASSERT(0 && "Boost::Chrono - Internal Error");
        }

        // transform to nanoseconds
        return time_point(duration(
            static_cast<thread_clock::rep>( ts.tv_sec ) * 1000000000 + ts.tv_nsec));

    }

#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
    thread_clock::time_point thread_clock::now( system::error_code & ec )
    {
      struct timespec ts;
#if defined CLOCK_THREAD_CPUTIME_ID
        // get the timespec associated to the thread clock
        if ( ::clock_gettime( CLOCK_THREAD_CPUTIME_ID, &ts ) )
#else
        // get the current thread
        pthread_t pth=pthread_self();
        // get the clock_id associated to the current thread
        clockid_t clock_id;
        pthread_getcpuclockid(pth, &clock_id);
        // get the timespec associated to the thread clock
        if ( ::clock_gettime( clock_id, &ts ) )
#endif
        {
            if (HBOOST_CHRONO_IS_THROWS(ec))
            {
                hboost::throw_exception(
                        system::system_error(
                                errno,
                                HBOOST_CHRONO_SYSTEM_CATEGORY,
                                "chrono::thread_clock" ));
            }
            else
            {
                ec.assign( errno, HBOOST_CHRONO_SYSTEM_CATEGORY );
                return time_point();
            }
        }
        if (!HBOOST_CHRONO_IS_THROWS(ec))
        {
            ec.clear();
        }
        // transform to nanoseconds
        return time_point(duration(
            static_cast<thread_clock::rep>( ts.tv_sec ) * 1000000000 + ts.tv_nsec));

    }
#endif
} }
