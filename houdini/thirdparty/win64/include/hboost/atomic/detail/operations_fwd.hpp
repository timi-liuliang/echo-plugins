/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/operations_fwd.hpp
 *
 * This header contains forward declaration of the \c operations template.
 */

#ifndef HBOOST_ATOMIC_DETAIL_OPERATIONS_FWD_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_OPERATIONS_FWD_HPP_INCLUDED_

#include <cstddef>
#include <hboost/atomic/detail/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace hboost {
namespace atomics {
namespace detail {

template< std::size_t Size, bool Signed >
struct operations;

} // namespace detail
} // namespace atomics
} // namespace hboost

#endif // HBOOST_ATOMIC_DETAIL_OPERATIONS_FWD_HPP_INCLUDED_
