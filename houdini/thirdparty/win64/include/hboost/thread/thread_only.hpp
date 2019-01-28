#ifndef HBOOST_THREAD_THREAD_ONLY_HPP
#define HBOOST_THREAD_THREAD_ONLY_HPP

//  thread.hpp
//
//  (C) Copyright 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/thread/detail/platform.hpp>

#if defined(HBOOST_THREAD_PLATFORM_WIN32)
#include <hboost/thread/win32/thread_data.hpp>
#elif defined(HBOOST_THREAD_PLATFORM_PTHREAD)
#include <hboost/thread/pthread/thread_data.hpp>
#else
#error "Boost threads unavailable on this platform"
#endif

#include <hboost/thread/detail/thread.hpp>
#if defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS
#include <hboost/thread/detail/thread_interruption.hpp>
#endif
#include <hboost/thread/v2/thread.hpp>


#endif
