//  time.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba
//  Copyright (c) Microsoft Corporation 2014
//  Copyright 2015 Andrey Semashev

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_DETAIL_WINAPI_TIME_HPP
#define HBOOST_DETAIL_WINAPI_TIME_HPP

#include <hboost/detail/winapi/basic_types.hpp>
#include <hboost/predef/platform.h>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if !defined( HBOOST_USE_WINDOWS_H )
extern "C" {
struct _FILETIME;
struct _SYSTEMTIME;

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::VOID_ WINAPI
GetSystemTime(::_SYSTEMTIME* lpSystemTime);

#ifdef HBOOST_HAS_GETSYSTEMTIMEASFILETIME  // Windows CE does not define GetSystemTimeAsFileTime
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::VOID_ WINAPI
GetSystemTimeAsFileTime(::_FILETIME* lpSystemTimeAsFileTime);
#endif

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
SystemTimeToFileTime(
    const ::_SYSTEMTIME* lpSystemTime,
    ::_FILETIME* lpFileTime);

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
FileTimeToSystemTime(
    const ::_FILETIME* lpFileTime,
    ::_SYSTEMTIME* lpSystemTime);

#if HBOOST_PLAT_WINDOWS_DESKTOP
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
FileTimeToLocalFileTime(
    const ::_FILETIME* lpFileTime,
    ::_FILETIME* lpLocalFileTime);

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
LocalFileTimeToFileTime(
    const ::_FILETIME* lpLocalFileTime,
    ::_FILETIME* lpFileTime);

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::DWORD_ WINAPI
GetTickCount(HBOOST_DETAIL_WINAPI_VOID);
#endif

#if HBOOST_USE_WINAPI_VERSION >= HBOOST_WINAPI_VERSION_WIN6
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::ULONGLONG_ WINAPI
GetTickCount64(HBOOST_DETAIL_WINAPI_VOID);
#endif
}
#endif

namespace hboost {
namespace detail {
namespace winapi {

typedef struct HBOOST_DETAIL_WINAPI_MAY_ALIAS _FILETIME {
    DWORD_ dwLowDateTime;
    DWORD_ dwHighDateTime;
} FILETIME_, *PFILETIME_, *LPFILETIME_;

typedef struct HBOOST_DETAIL_WINAPI_MAY_ALIAS _SYSTEMTIME {
    WORD_ wYear;
    WORD_ wMonth;
    WORD_ wDayOfWeek;
    WORD_ wDay;
    WORD_ wHour;
    WORD_ wMinute;
    WORD_ wSecond;
    WORD_ wMilliseconds;
} SYSTEMTIME_, *PSYSTEMTIME_, *LPSYSTEMTIME_;

#if HBOOST_PLAT_WINDOWS_DESKTOP
using ::GetTickCount;
#endif
#if HBOOST_USE_WINAPI_VERSION >= HBOOST_WINAPI_VERSION_WIN6
using ::GetTickCount64;
#endif

HBOOST_FORCEINLINE VOID_ GetSystemTime(LPSYSTEMTIME_ lpSystemTime)
{
    ::GetSystemTime(reinterpret_cast< ::_SYSTEMTIME* >(lpSystemTime));
}

#if defined( HBOOST_HAS_GETSYSTEMTIMEASFILETIME )
HBOOST_FORCEINLINE VOID_ GetSystemTimeAsFileTime(LPFILETIME_ lpSystemTimeAsFileTime)
{
    ::GetSystemTimeAsFileTime(reinterpret_cast< ::_FILETIME* >(lpSystemTimeAsFileTime));
}
#else
// Windows CE does not define GetSystemTimeAsFileTime
HBOOST_FORCEINLINE VOID_ GetSystemTimeAsFileTime(FILETIME_* lpFileTime)
{
    hboost::detail::winapi::SYSTEMTIME_ st;
    hboost::detail::winapi::GetSystemTime(&st);
    hboost::detail::winapi::SystemTimeToFileTime(&st, lpFileTime);
}
#endif

HBOOST_FORCEINLINE BOOL_ SystemTimeToFileTime(const SYSTEMTIME_* lpSystemTime, FILETIME_* lpFileTime)
{
    return ::SystemTimeToFileTime(reinterpret_cast< const ::_SYSTEMTIME* >(lpSystemTime), reinterpret_cast< ::_FILETIME* >(lpFileTime));
}

HBOOST_FORCEINLINE BOOL_ FileTimeToSystemTime(const FILETIME_* lpFileTime, SYSTEMTIME_* lpSystemTime)
{
    return ::FileTimeToSystemTime(reinterpret_cast< const ::_FILETIME* >(lpFileTime), reinterpret_cast< ::_SYSTEMTIME* >(lpSystemTime));
}

#if HBOOST_PLAT_WINDOWS_DESKTOP
HBOOST_FORCEINLINE BOOL_ FileTimeToLocalFileTime(const FILETIME_* lpFileTime, FILETIME_* lpLocalFileTime)
{
    return ::FileTimeToLocalFileTime(reinterpret_cast< const ::_FILETIME* >(lpFileTime), reinterpret_cast< ::_FILETIME* >(lpLocalFileTime));
}

HBOOST_FORCEINLINE BOOL_ LocalFileTimeToFileTime(const FILETIME_* lpLocalFileTime, FILETIME_* lpFileTime)
{
    return ::LocalFileTimeToFileTime(reinterpret_cast< const ::_FILETIME* >(lpLocalFileTime), reinterpret_cast< ::_FILETIME* >(lpFileTime));
}
#endif

}
}
}

#endif // HBOOST_DETAIL_WINAPI_TIME_HPP