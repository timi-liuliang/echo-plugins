//  boost process_cpu_clocks.cpp  -----------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//
#ifndef HBOOST_CHRONO_DETAIL_INLINED_PROCESS_CPU_CLOCKS_HPP
#define HBOOST_CHRONO_DETAIL_INLINED_PROCESS_CPU_CLOCKS_HPP


#include <hboost/chrono/config.hpp>
#if defined(HBOOST_CHRONO_HAS_PROCESS_CLOCKS)

#include <hboost/version.hpp>
#include <hboost/chrono/process_cpu_clocks.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/system/system_error.hpp>

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#if defined(HBOOST_CHRONO_WINDOWS_API)
#include <hboost/chrono/detail/inlined/win/process_cpu_clocks.hpp>

//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//
#elif defined(HBOOST_CHRONO_MAC_API)
#include <hboost/chrono/detail/inlined/mac/process_cpu_clocks.hpp>

//----------------------------------------------------------------------------//
//                                POSIX                                     //
//----------------------------------------------------------------------------//
#elif defined(HBOOST_CHRONO_POSIX_API)
#include <hboost/chrono/detail/inlined/posix/process_cpu_clocks.hpp>

#endif  // POSIX

#endif

#endif
