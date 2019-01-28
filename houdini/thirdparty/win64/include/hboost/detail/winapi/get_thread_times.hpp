//  get_thread_times.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba
//  Copyright 2015 Andrey Semashev

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_DETAIL_WINAPI_GET_THREAD_TIMES_HPP
#define HBOOST_DETAIL_WINAPI_GET_THREAD_TIMES_HPP

#include <hboost/detail/winapi/basic_types.hpp>
#include <hboost/detail/winapi/time.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if !defined( HBOOST_USE_WINDOWS_H )
extern "C" {
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
GetThreadTimes(
    hboost::detail::winapi::HANDLE_ hThread,
    ::_FILETIME* lpCreationTime,
    ::_FILETIME* lpExitTime,
    ::_FILETIME* lpKernelTime,
    ::_FILETIME* lpUserTime);
}
#endif

namespace hboost {
namespace detail {
namespace winapi {

HBOOST_FORCEINLINE BOOL_ GetThreadTimes(
    HANDLE_ hThread,
    LPFILETIME_ lpCreationTime,
    LPFILETIME_ lpExitTime,
    LPFILETIME_ lpKernelTime,
    LPFILETIME_ lpUserTime)
{
    return ::GetThreadTimes(
        hThread,
        reinterpret_cast< ::_FILETIME* >(lpCreationTime),
        reinterpret_cast< ::_FILETIME* >(lpExitTime),
        reinterpret_cast< ::_FILETIME* >(lpKernelTime),
        reinterpret_cast< ::_FILETIME* >(lpUserTime));
}

}
}
}

#endif // HBOOST_DETAIL_WINAPI_GET_THREAD_TIMES_HPP
