//  get_current_thread_id.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba
//  Copyright 2015 Andrey Semashev

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_DETAIL_WINAPI_GET_CURRENT_THREAD_ID_HPP
#define HBOOST_DETAIL_WINAPI_GET_CURRENT_THREAD_ID_HPP

#include <hboost/detail/winapi/basic_types.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

// Windows CE define GetCurrentThreadId as an inline function in kfuncs.h
#if !defined( HBOOST_USE_WINDOWS_H ) && !defined( UNDER_CE )
extern "C" {
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::DWORD_ WINAPI GetCurrentThreadId(HBOOST_DETAIL_WINAPI_VOID);
}
#endif

namespace hboost {
namespace detail {
namespace winapi {
using ::GetCurrentThreadId;
}
}
}

#endif // HBOOST_DETAIL_WINAPI_GET_CURRENT_THREAD_ID_HPP
