//  win/chrono.cpp  --------------------------------------------------------------//

//  Copyright Beman Dawes 2008
//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#ifndef HBOOST_CHRONO_DETAIL_INLINED_WIN_CHRONO_HPP
#define HBOOST_CHRONO_DETAIL_INLINED_WIN_CHRONO_HPP

#include <hboost/detail/winapi/time.hpp>
#include <hboost/detail/winapi/timers.hpp>
#include <hboost/detail/winapi/GetLastError.hpp>

namespace hboost
{
namespace chrono
{
namespace chrono_detail
{

  HBOOST_CHRONO_INLINE double get_nanosecs_per_tic() HBOOST_NOEXCEPT
  {
      hboost::detail::winapi::LARGE_INTEGER_ freq;
      if ( !hboost::detail::winapi::QueryPerformanceFrequency( &freq ) )
          return 0.0L;
      return double(1000000000.0L / freq.QuadPart);
  }

}

  steady_clock::time_point steady_clock::now() HBOOST_NOEXCEPT
  {
    double nanosecs_per_tic = chrono_detail::get_nanosecs_per_tic();

    hboost::detail::winapi::LARGE_INTEGER_ pcount;
    if ( nanosecs_per_tic <= 0.0L )
    {
      HBOOST_ASSERT(0 && "Boost::Chrono - get_nanosecs_per_tic Internal Error");
      return steady_clock::time_point();
    }
    unsigned times=0;
    while ( ! hboost::detail::winapi::QueryPerformanceCounter( &pcount ) )
    {
      if ( ++times > 3 )
      {
        HBOOST_ASSERT(0 && "Boost::Chrono - QueryPerformanceCounter Internal Error");
        return steady_clock::time_point();
      }
    }

    return steady_clock::time_point(steady_clock::duration(
      static_cast<steady_clock::rep>((nanosecs_per_tic) * pcount.QuadPart)));
  }


#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
  steady_clock::time_point steady_clock::now( system::error_code & ec )
  {
    double nanosecs_per_tic = chrono_detail::get_nanosecs_per_tic();

    hboost::detail::winapi::LARGE_INTEGER_ pcount;
    if ( (nanosecs_per_tic <= 0.0L)
            || (!hboost::detail::winapi::QueryPerformanceCounter( &pcount )) )
    {
        hboost::detail::winapi::DWORD_ cause =
            ((nanosecs_per_tic <= 0.0L)
                    ? ERROR_NOT_SUPPORTED
                    : hboost::detail::winapi::GetLastError());
        if (HBOOST_CHRONO_IS_THROWS(ec)) {
            hboost::throw_exception(
                    system::system_error(
                            cause,
                            HBOOST_CHRONO_SYSTEM_CATEGORY,
                            "chrono::steady_clock" ));
        }
        else
        {
            ec.assign( cause, HBOOST_CHRONO_SYSTEM_CATEGORY );
            return steady_clock::time_point(duration(0));
        }
    }

    if (!HBOOST_CHRONO_IS_THROWS(ec))
    {
        ec.clear();
    }
    return time_point(duration(
      static_cast<steady_clock::rep>(nanosecs_per_tic * pcount.QuadPart)));
  }
#endif

  HBOOST_CHRONO_INLINE
  system_clock::time_point system_clock::now() HBOOST_NOEXCEPT
  {
    hboost::detail::winapi::FILETIME_ ft;
    hboost::detail::winapi::GetSystemTimeAsFileTime( &ft );  // never fails
    return system_clock::time_point(
      system_clock::duration(
        ((static_cast<__int64>( ft.dwHighDateTime ) << 32) | ft.dwLowDateTime)
       - 116444736000000000LL
       //- (134775LL*864000000000LL)
      )
    );
  }

#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
  HBOOST_CHRONO_INLINE
  system_clock::time_point system_clock::now( system::error_code & ec )
  {
    hboost::detail::winapi::FILETIME_ ft;
    hboost::detail::winapi::GetSystemTimeAsFileTime( &ft );  // never fails
    if (!HBOOST_CHRONO_IS_THROWS(ec))
    {
        ec.clear();
    }
    return system_clock::time_point(
      system_clock::duration(
       ((static_cast<__int64>( ft.dwHighDateTime ) << 32) | ft.dwLowDateTime)
       - 116444736000000000LL
       //- (134775LL*864000000000LL)
       ));
  }
#endif

  HBOOST_CHRONO_INLINE
  std::time_t system_clock::to_time_t(const system_clock::time_point& t) HBOOST_NOEXCEPT
  {
      __int64 temp = t.time_since_epoch().count();
      temp /= 10000000;
      return static_cast<std::time_t>( temp );
  }

  HBOOST_CHRONO_INLINE
  system_clock::time_point system_clock::from_time_t(std::time_t t) HBOOST_NOEXCEPT
  {
      __int64 temp = t;
      temp *= 10000000;
      return time_point(duration(temp));
  }

}  // namespace chrono
}  // namespace hboost

#endif
