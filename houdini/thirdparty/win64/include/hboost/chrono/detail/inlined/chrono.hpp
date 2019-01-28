//  chrono.cpp  --------------------------------------------------------------//

//  Copyright Beman Dawes 2008
//  Copyright Vicente J. Botet Escriba 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef HBOOST_CHRONO_DETAIL_INLINED_CHRONO_HPP
#define HBOOST_CHRONO_DETAIL_INLINED_CHRONO_HPP

#include <hboost/version.hpp>
#include <hboost/chrono/chrono.hpp>
#include <hboost/system/system_error.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/chrono/detail/system.hpp>

//----------------------------------------------------------------------------//
//                                                                            //
//                     Platform-specific Implementations                      //
//                                                                            //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#if defined(HBOOST_CHRONO_WINDOWS_API)
#include <hboost/chrono/detail/inlined/win/chrono.hpp>

//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//
#elif defined(HBOOST_CHRONO_MAC_API)
#include <hboost/chrono/detail/inlined/mac/chrono.hpp>

//----------------------------------------------------------------------------//
//                                POSIX                                     //
//----------------------------------------------------------------------------//
#elif defined(HBOOST_CHRONO_POSIX_API)
#include <hboost/chrono/detail/inlined/posix/chrono.hpp>

#endif  // POSIX

#endif
