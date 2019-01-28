//  boost process_timer.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008
//  Copyright 2009-2010 Vicente J. Botet Escriba
//  Copyright (c) Microsoft Corporation 2014

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//
#ifndef HBOOST_CHRONO_DETAIL_INLINED_WIN_PROCESS_CLOCK_HPP
#define HBOOST_CHRONO_DETAIL_INLINED_WIN_PROCESS_CLOCK_HPP

#include <hboost/chrono/config.hpp>
//#include <hboost/chrono/system_clocks.hpp>
#include <hboost/chrono/process_cpu_clocks.hpp>
#include <cassert>
#include <time.h>

#include <hboost/detail/winapi/GetLastError.hpp>
#include <hboost/detail/winapi/GetCurrentProcess.hpp>
#if HBOOST_PLAT_WINDOWS_DESKTOP
#include <hboost/detail/winapi/GetProcessTimes.hpp>
#endif

namespace hboost
{
namespace chrono
{

process_real_cpu_clock::time_point process_real_cpu_clock::now() HBOOST_NOEXCEPT
{
    clock_t c = ::clock();
    if ( c == clock_t(-1) ) // error
    {
      HBOOST_ASSERT(0 && "Boost::Chrono - Internal Error");
    }
    typedef ratio_divide<giga, ratio<CLOCKS_PER_SEC> >::type R;
    return time_point(
      duration(static_cast<rep>(c)*R::num/R::den)
    );
}

#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
process_real_cpu_clock::time_point process_real_cpu_clock::now(
        system::error_code & ec)
{
    clock_t c = ::clock();
    if ( c == clock_t(-1) ) // error
    {
            hboost::throw_exception(
                    system::system_error(
                            errno,
                            HBOOST_CHRONO_SYSTEM_CATEGORY,
                            "chrono::process_real_cpu_clock" ));
    }
    if (!HBOOST_CHRONO_IS_THROWS(ec))
    {
      ec.clear();
    }
    typedef ratio_divide<giga, ratio<CLOCKS_PER_SEC> >::type R;
    return time_point(
      duration(static_cast<rep>(c)*R::num/R::den)
    );
}
#endif

#if HBOOST_PLAT_WINDOWS_DESKTOP
process_user_cpu_clock::time_point process_user_cpu_clock::now() HBOOST_NOEXCEPT
{

    //  note that Windows uses 100 nanosecond ticks for FILETIME
    hboost::detail::winapi::FILETIME_ creation, exit, user_time, system_time;

    if ( hboost::detail::winapi::GetProcessTimes(
            hboost::detail::winapi::GetCurrentProcess(), &creation, &exit,
            &system_time, &user_time ) )
    {
        return time_point(duration(
                ((static_cast<process_user_cpu_clock::rep>(user_time.dwHighDateTime) << 32)
                  | user_time.dwLowDateTime) * 100
                ));
    }
    else
    {
        HBOOST_ASSERT(0 && "Boost::Chrono - Internal Error");
        return time_point();
    }

}

#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
process_user_cpu_clock::time_point process_user_cpu_clock::now(
        system::error_code & ec)
{

    //  note that Windows uses 100 nanosecond ticks for FILETIME
    hboost::detail::winapi::FILETIME_ creation, exit, user_time, system_time;

    if ( hboost::detail::winapi::GetProcessTimes(
            hboost::detail::winapi::GetCurrentProcess(), &creation, &exit,
            &system_time, &user_time ) )
    {
        if (!HBOOST_CHRONO_IS_THROWS(ec))
        {
            ec.clear();
        }
        return time_point(duration(
                ((static_cast<process_user_cpu_clock::rep>(user_time.dwHighDateTime) << 32)
                  | user_time.dwLowDateTime) * 100
                ));
    }
    else
    {
        hboost::detail::winapi::DWORD_ cause = hboost::detail::winapi::GetLastError();
        if (HBOOST_CHRONO_IS_THROWS(ec))
        {
            hboost::throw_exception(
                    system::system_error(
                            cause,
                            HBOOST_CHRONO_SYSTEM_CATEGORY,
                            "chrono::process_user_cpu_clock" ));
        }
        else
        {
            ec.assign( cause, HBOOST_CHRONO_SYSTEM_CATEGORY );
            return time_point();
        }
    }

}
#endif

process_system_cpu_clock::time_point process_system_cpu_clock::now() HBOOST_NOEXCEPT
{

    //  note that Windows uses 100 nanosecond ticks for FILETIME
    hboost::detail::winapi::FILETIME_ creation, exit, user_time, system_time;

    if ( hboost::detail::winapi::GetProcessTimes(
            hboost::detail::winapi::GetCurrentProcess(), &creation, &exit,
            &system_time, &user_time ) )
    {
        return time_point(duration(
                ((static_cast<process_system_cpu_clock::rep>(system_time.dwHighDateTime) << 32)
                                    | system_time.dwLowDateTime) * 100
                ));
    }
    else
    {
      HBOOST_ASSERT(0 && "Boost::Chrono - Internal Error");
      return time_point();
    }

}

#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
process_system_cpu_clock::time_point process_system_cpu_clock::now(
        system::error_code & ec)
{

    //  note that Windows uses 100 nanosecond ticks for FILETIME
    hboost::detail::winapi::FILETIME_ creation, exit, user_time, system_time;

    if ( hboost::detail::winapi::GetProcessTimes(
            hboost::detail::winapi::GetCurrentProcess(), &creation, &exit,
            &system_time, &user_time ) )
    {
        if (!HBOOST_CHRONO_IS_THROWS(ec))
        {
            ec.clear();
        }
        return time_point(duration(
                ((static_cast<process_system_cpu_clock::rep>(system_time.dwHighDateTime) << 32)
                                    | system_time.dwLowDateTime) * 100
                ));
    }
    else
    {
        hboost::detail::winapi::DWORD_ cause = hboost::detail::winapi::GetLastError();
        if (HBOOST_CHRONO_IS_THROWS(ec))
        {
            hboost::throw_exception(
                    system::system_error(
                            cause,
                            HBOOST_CHRONO_SYSTEM_CATEGORY,
                            "chrono::process_system_cpu_clock" ));
        }
        else
        {
            ec.assign( cause, HBOOST_CHRONO_SYSTEM_CATEGORY );
            return time_point();
        }
    }

}
#endif

process_cpu_clock::time_point process_cpu_clock::now()  HBOOST_NOEXCEPT
{

    //  note that Windows uses 100 nanosecond ticks for FILETIME
    hboost::detail::winapi::FILETIME_ creation, exit, user_time, system_time;

    if ( hboost::detail::winapi::GetProcessTimes(
            hboost::detail::winapi::GetCurrentProcess(), &creation, &exit,
            &system_time, &user_time ) )
    {
        time_point::rep r(process_real_cpu_clock::now().time_since_epoch().count()
                            ,
                ((static_cast<process_user_cpu_clock::rep>(user_time.dwHighDateTime) << 32)
                        | user_time.dwLowDateTime
                ) * 100,
                ((static_cast<process_system_cpu_clock::rep>(system_time.dwHighDateTime) << 32)
                        | system_time.dwLowDateTime
                ) * 100
        );
        return time_point(duration(r));
    }
    else
    {
      HBOOST_ASSERT(0 && "Boost::Chrono - Internal Error");
      return time_point();
    }

}

#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
process_cpu_clock::time_point process_cpu_clock::now(
        system::error_code & ec )
{

    //  note that Windows uses 100 nanosecond ticks for FILETIME
    hboost::detail::winapi::FILETIME_ creation, exit, user_time, system_time;

    if ( hboost::detail::winapi::GetProcessTimes(
            hboost::detail::winapi::GetCurrentProcess(), &creation, &exit,
            &system_time, &user_time ) )
    {
        if (!HBOOST_CHRONO_IS_THROWS(ec))
        {
            ec.clear();
        }
        time_point::rep r(process_real_cpu_clock::now().time_since_epoch().count()
                            ,
                ((static_cast<process_user_cpu_clock::rep>(user_time.dwHighDateTime) << 32)
                        | user_time.dwLowDateTime
                ) * 100,
                ((static_cast<process_system_cpu_clock::rep>(system_time.dwHighDateTime) << 32)
                        | system_time.dwLowDateTime
                ) * 100
        );
        return time_point(duration(r));
    }
    else
    {
        hboost::detail::winapi::DWORD_ cause = hboost::detail::winapi::GetLastError();
        if (HBOOST_CHRONO_IS_THROWS(ec))
        {
            hboost::throw_exception(
                    system::system_error(
                            cause,
                            HBOOST_CHRONO_SYSTEM_CATEGORY,
                            "chrono::process_cpu_clock" ));
        }
        else
        {
            ec.assign( cause, HBOOST_CHRONO_SYSTEM_CATEGORY );
            return time_point();
        }
    }

}
#endif
#endif
} // namespace chrono
} // namespace hboost

#endif
