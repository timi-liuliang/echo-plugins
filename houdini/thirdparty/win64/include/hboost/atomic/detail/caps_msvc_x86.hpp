/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2009 Helge Bahmann
 * Copyright (c) 2013 Tim Blechmann
 * Copyright (c) 2012 - 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/caps_msvc_x86.hpp
 *
 * This header defines feature capabilities macros
 */

#ifndef HBOOST_ATOMIC_DETAIL_CAPS_MSVC_X86_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_CAPS_MSVC_X86_HPP_INCLUDED_

#include <hboost/atomic/detail/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if defined(_M_IX86) && _M_IX86 >= 500
#define HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG8B 1
#endif

#if _MSC_VER >= 1500 && defined(_M_AMD64) && !defined(HBOOST_ATOMIC_NO_CMPXCHG16B)
#define HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG16B 1
#endif

#define HBOOST_ATOMIC_INT8_LOCK_FREE 2
#define HBOOST_ATOMIC_INT16_LOCK_FREE 2
#define HBOOST_ATOMIC_INT32_LOCK_FREE 2

#if defined(_M_AMD64) || defined(HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG8B)
#define HBOOST_ATOMIC_INT64_LOCK_FREE 2
#endif

#if defined(HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG16B) && (defined(HBOOST_HAS_INT128) || !defined(HBOOST_NO_ALIGNMENT))
#define HBOOST_ATOMIC_INT128_LOCK_FREE 2
#endif

#define HBOOST_ATOMIC_POINTER_LOCK_FREE 2

#define HBOOST_ATOMIC_THREAD_FENCE 2
#define HBOOST_ATOMIC_SIGNAL_FENCE 2

#endif // HBOOST_ATOMIC_DETAIL_CAPS_MSVC_X86_HPP_INCLUDED_
