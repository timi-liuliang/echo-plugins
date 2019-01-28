//  get_process_times.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_DETAIL_WINAPI_GET_PROCESS_TIMES_HPP
#define HBOOST_DETAIL_WINAPI_GET_PROCESS_TIMES_HPP

#include <hboost/detail/winapi/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

// Windows CE does not define GetProcessTimes
#if !defined( UNDER_CE )

#include <hboost/detail/winapi/basic_types.hpp>
#include <hboost/detail/winapi/time.hpp>

#if !defined( HBOOST_USE_WINDOWS_H )
extern "C" {
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
GetProcessTimes(
    hboost::detail::winapi::HANDLE_ hProcess,
    ::_FILETIME* lpCreationTime,
    ::_FILETIME* lpExitTime,
    ::_FILETIME* lpKernelTime,
    ::_FILETIME* lpUserTime);
}
#endif

namespace hboost {
namespace detail {
namespace winapi {

HBOOST_FORCEINLINE BOOL_ GetProcessTimes(
    HANDLE_ hProcess,
    LPFILETIME_ lpCreationTime,
    LPFILETIME_ lpExitTime,
    LPFILETIME_ lpKernelTime,
    LPFILETIME_ lpUserTime)
{
    return ::GetProcessTimes(
        hProcess,
        reinterpret_cast< ::_FILETIME* >(lpCreationTime),
        reinterpret_cast< ::_FILETIME* >(lpExitTime),
        reinterpret_cast< ::_FILETIME* >(lpKernelTime),
        reinterpret_cast< ::_FILETIME* >(lpUserTime));
}

}
}
}

#endif // !defined( UNDER_CE )
#endif // HBOOST_DETAIL_WINAPI_GET_PROCESS_TIMES_HPP
