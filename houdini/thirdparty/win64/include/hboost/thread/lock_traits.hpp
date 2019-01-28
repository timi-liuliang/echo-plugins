// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2009-2012 Vicente J. Botet Escriba

#ifndef HBOOST_THREAD_LOCK_TRAITS_HPP
#define HBOOST_THREAD_LOCK_TRAITS_HPP

#include <hboost/thread/detail/config.hpp>
//#include <hboost/thread/detail/move.hpp>
//#include <hboost/thread/exceptions.hpp>
//
//#ifdef HBOOST_THREAD_USES_CHRONO
//#include <hboost/chrono/time_point.hpp>
//#include <hboost/chrono/duration.hpp>
//#endif

#include <hboost/type_traits/integral_constant.hpp>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{

/**
 * An strict lock is a lock ensuring the mutex is locked on the scope of the lock
 * There is no single way to define a strict lock as the strict_lock and
 * nesteed_strict_lock shows. So we need a metafunction that states if a
 * lock is a strict lock "sur parole".
 */

template <typename Lock>
struct is_strict_lock_sur_parolle : false_type {};


template <typename Lock>
struct is_strict_lock_sur_parole : is_strict_lock_sur_parolle<Lock> {};

template <typename Lock>
struct is_strict_lock : is_strict_lock_sur_parole<Lock> {};

}
#include <hboost/config/abi_suffix.hpp>

#endif
