//  boost thread_clock.cpp  -----------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//
#ifndef HBOOST_CHRONO_DETAIL_INLINED_WIN_THREAD_CLOCK_HPP
#define HBOOST_CHRONO_DETAIL_INLINED_WIN_THREAD_CLOCK_HPP

#include <hboost/chrono/config.hpp>
#include <hboost/chrono/thread_clock.hpp>
#include <cassert>

#include <hboost/detail/winapi/GetLastError.hpp>
#include <hboost/detail/winapi/GetCurrentThread.hpp>
#include <hboost/detail/winapi/GetThreadTimes.hpp>

namespace hboost
{
namespace chrono
{

#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
thread_clock::time_point thread_clock::now( system::error_code & ec )
{
    //  note that Windows uses 100 nanosecond ticks for FILETIME
    hboost::detail::winapi::FILETIME_ creation, exit, user_time, system_time;

    if ( hboost::detail::winapi::GetThreadTimes(
            hboost::detail::winapi::GetCurrentThread (), &creation, &exit,
            &system_time, &user_time ) )
    {
        duration user = duration(
                ((static_cast<duration::rep>(user_time.dwHighDateTime) << 32)
                        | user_time.dwLowDateTime) * 100 );

        duration system = duration(
                ((static_cast<duration::rep>(system_time.dwHighDateTime) << 32)
                        | system_time.dwLowDateTime) * 100 );

        if (!HBOOST_CHRONO_IS_THROWS(ec)) 
        {
            ec.clear();
        }
        return time_point(system+user);

    }
    else
    {
        if (HBOOST_CHRONO_IS_THROWS(ec)) 
        {
            hboost::throw_exception(
                    system::system_error( 
                            hboost::detail::winapi::GetLastError(), 
                            HBOOST_CHRONO_SYSTEM_CATEGORY, 
                            "chrono::thread_clock" ));
        } 
        else 
        {
            ec.assign( hboost::detail::winapi::GetLastError(), HBOOST_CHRONO_SYSTEM_CATEGORY );
            return thread_clock::time_point(duration(0));
        }
    }
}
#endif

thread_clock::time_point thread_clock::now() HBOOST_NOEXCEPT
{

    //  note that Windows uses 100 nanosecond ticks for FILETIME
    hboost::detail::winapi::FILETIME_ creation, exit, user_time, system_time;

    if ( hboost::detail::winapi::GetThreadTimes( 
            hboost::detail::winapi::GetCurrentThread (), &creation, &exit,
            &system_time, &user_time ) )
    {
        duration user   = duration(
                ((static_cast<duration::rep>(user_time.dwHighDateTime) << 32)
                        | user_time.dwLowDateTime) * 100 );

        duration system = duration(
                ((static_cast<duration::rep>(system_time.dwHighDateTime) << 32)
                        | system_time.dwLowDateTime) * 100 );

        return time_point(system+user);
    }
    else
    {
      HBOOST_ASSERT(0 && "Boost::Chrono - Internal Error");
      return time_point();
    }

}

} // namespace chrono
} // namespace hboost

#endif
