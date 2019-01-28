//  boost thread_clock.cpp  -----------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//
#ifndef HBOOST_CHRONO_DETAIL_INLINED_THREAD_CLOCK_HPP
#define HBOOST_CHRONO_DETAIL_INLINED_THREAD_CLOCK_HPP

#include <hboost/chrono/config.hpp>
#include <hboost/version.hpp>
#if defined(HBOOST_CHRONO_HAS_THREAD_CLOCK)
#include <hboost/chrono/thread_clock.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/system/system_error.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/chrono/detail/system.hpp>

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#if defined(HBOOST_CHRONO_WINDOWS_API)
#include <hboost/chrono/detail/inlined/win/thread_clock.hpp>

//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//
#elif defined(HBOOST_CHRONO_MAC_API)
#include <hboost/chrono/detail/inlined/mac/thread_clock.hpp>

//----------------------------------------------------------------------------//
//                                POSIX                                     //
//----------------------------------------------------------------------------//
#elif defined(HBOOST_CHRONO_POSIX_API)
#include <hboost/chrono/detail/inlined/posix/thread_clock.hpp>

#endif  // POSIX

#endif
#endif
