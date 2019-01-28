/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/operations_lockfree.hpp
 *
 * This header defines lockfree atomic operations.
 */

#ifndef HBOOST_ATOMIC_DETAIL_OPERATIONS_LOCKFREE_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_OPERATIONS_LOCKFREE_HPP_INCLUDED_

#include <hboost/atomic/detail/config.hpp>
#include <hboost/atomic/detail/platform.hpp>

#if !defined(HBOOST_ATOMIC_EMULATED)
#include HBOOST_ATOMIC_DETAIL_HEADER(hboost/atomic/detail/ops_)
#else
#include <hboost/atomic/detail/operations_fwd.hpp>
#endif

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#endif // HBOOST_ATOMIC_DETAIL_OPERATIONS_LOCKFREE_HPP_INCLUDED_
