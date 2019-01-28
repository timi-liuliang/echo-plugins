/*
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 * (C) Copyright 2013 Tim Blechmann
 * (C) Copyright 2013 Andrey Semashev
 */

#ifndef HBOOST_ATOMIC_DETAIL_PAUSE_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_PAUSE_HPP_INCLUDED_

#include <hboost/atomic/detail/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if defined(_MSC_VER) && (defined(_M_AMD64) || defined(_M_IX86))
extern "C" void _mm_pause(void);
#if defined(HBOOST_MSVC)
#pragma intrinsic(_mm_pause)
#endif
#endif

namespace hboost {
namespace atomics {
namespace detail {

HBOOST_FORCEINLINE void pause() HBOOST_NOEXCEPT
{
#if defined(_MSC_VER) && (defined(_M_AMD64) || defined(_M_IX86))
    _mm_pause();
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
    __asm__ __volatile__("pause;");
#endif
}

} // namespace detail
} // namespace atomics
} // namespace hboost

#endif // HBOOST_ATOMIC_DETAIL_PAUSE_HPP_INCLUDED_
