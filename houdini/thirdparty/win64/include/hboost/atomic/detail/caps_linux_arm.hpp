/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2009, 2011 Helge Bahmann
 * Copyright (c) 2009 Phil Endecott
 * Copyright (c) 2013 Tim Blechmann
 * Linux-specific code by Phil Endecott
 * Copyright (c) 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/caps_linux_arm.hpp
 *
 * This header defines feature capabilities macros
 */

#ifndef HBOOST_ATOMIC_DETAIL_CAPS_LINUX_ARM_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_CAPS_LINUX_ARM_HPP_INCLUDED_

#include <hboost/atomic/detail/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#define HBOOST_ATOMIC_INT8_LOCK_FREE 2
#define HBOOST_ATOMIC_INT16_LOCK_FREE 2
#define HBOOST_ATOMIC_INT32_LOCK_FREE 2
#define HBOOST_ATOMIC_POINTER_LOCK_FREE 2

#define HBOOST_ATOMIC_THREAD_FENCE 2
#define HBOOST_ATOMIC_SIGNAL_FENCE 2

#endif // HBOOST_ATOMIC_DETAIL_CAPS_LINUX_ARM_HPP_INCLUDED_
