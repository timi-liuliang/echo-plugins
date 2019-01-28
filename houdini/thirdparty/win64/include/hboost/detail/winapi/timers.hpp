//  timers.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba
//  Copyright 2015 Andrey Semashev

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_DETAIL_WINAPI_TIMERS_HPP
#define HBOOST_DETAIL_WINAPI_TIMERS_HPP

#include <hboost/detail/winapi/basic_types.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if !defined( HBOOST_USE_WINDOWS_H )
extern "C" {
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
QueryPerformanceCounter(::_LARGE_INTEGER* lpPerformanceCount);

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
QueryPerformanceFrequency(::_LARGE_INTEGER* lpFrequency);
}
#endif


namespace hboost {
namespace detail {
namespace winapi {

HBOOST_FORCEINLINE BOOL_ QueryPerformanceCounter(LARGE_INTEGER_* lpPerformanceCount)
{
    return ::QueryPerformanceCounter(reinterpret_cast< ::_LARGE_INTEGER* >(lpPerformanceCount));
}

HBOOST_FORCEINLINE BOOL_ QueryPerformanceFrequency(LARGE_INTEGER_* lpFrequency)
{
    return ::QueryPerformanceFrequency(reinterpret_cast< ::_LARGE_INTEGER* >(lpFrequency));
}

}
}
}

#endif // HBOOST_DETAIL_WINAPI_TIMERS_HPP
