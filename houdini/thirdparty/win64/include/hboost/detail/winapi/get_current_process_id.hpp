//  get_current_process_id.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_DETAIL_WINAPI_GET_CURRENT_PROCESS_ID_HPP
#define HBOOST_DETAIL_WINAPI_GET_CURRENT_PROCESS_ID_HPP

#include <hboost/detail/winapi/basic_types.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

// Windows CE define GetCurrentProcessId as an inline function in kfuncs.h
#if !defined( HBOOST_USE_WINDOWS_H ) && !defined( UNDER_CE )
extern "C" {
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::DWORD_ WINAPI GetCurrentProcessId(HBOOST_DETAIL_WINAPI_VOID);
}
#endif

namespace hboost {
namespace detail {
namespace winapi {
using ::GetCurrentProcessId;
}
}
}

#endif // HBOOST_DETAIL_WINAPI_GET_CURRENT_PROCESS_ID_HPP
