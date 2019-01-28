// (C) Copyright 2013 Ruslan Baratov
// Copyright (C) 2014 Vicente J. Botet Escriba
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See www.boost.org/libs/thread for documentation.

#ifndef HBOOST_THREAD_WITH_LOCK_GUARD_HPP
#define HBOOST_THREAD_WITH_LOCK_GUARD_HPP

#include <hboost/thread/lock_guard.hpp>
#include <hboost/utility/result_of.hpp>
//#include <hboost/thread/detail/invoke.hpp>

namespace hboost {

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && \
    !defined(HBOOST_NO_CXX11_DECLTYPE) && \
    !defined(HBOOST_NO_CXX11_TRAILING_RESULT_TYPES)

/**
 * Utility to run functions in scope protected by mutex.
 *
 * Examples:
 *
 *     int func(int, int&);
 *     hboost::mutex m;
 *     int a;
 *     int result = hboost::with_lock_guard(m, func, 1, hboost::ref(a));
 *
 *     // using hboost::bind
 *     int result = hboost::with_lock_guard(
 *         m, hboost::bind(func, 2, hboost::ref(a))
 *     );
 *
 *     // using lambda
 *     int a;
 *     int result = hboost::with_lock_guard(
 *         m,
 *         [&a](int x) {
 *           a = 3;
 *           return x + 4;
 *         },
 *         5
 *     );
 */
template <class Lockable, class Function, class... Args>
typename hboost::result_of<Function(Args...)>::type with_lock_guard(
    Lockable& m,
    HBOOST_FWD_REF(Function) func,
    HBOOST_FWD_REF(Args)... args
) //-> decltype(func(hboost::forward<Args>(args)...))
{
  hboost::lock_guard<Lockable> lock(m);
  return func(hboost::forward<Args>(args)...);
}

#else

// Workaround versions for compilers without c++11 variadic templates support.
// (function arguments limit: 4)
// (for lambda support define HBOOST_RESULT_OF_USE_DECLTYPE may be needed)

template <class Lockable, class Func>
typename hboost::result_of<Func()>::type with_lock_guard(
    Lockable& m,
    HBOOST_FWD_REF(Func) func
) {
  hboost::lock_guard<Lockable> lock(m);
  return func();
}

template <class Lockable, class Func, class Arg>
typename hboost::result_of<Func(Arg)>::type with_lock_guard(
    Lockable& m,
    HBOOST_FWD_REF(Func) func,
    HBOOST_FWD_REF(Arg) arg
) {
  hboost::lock_guard<Lockable> lock(m);
  return func(
      hboost::forward<Arg>(arg)
  );
}

template <class Lockable, class Func, class Arg1, class Arg2>
typename hboost::result_of<Func(Arg1, Arg2)>::type with_lock_guard(
    Lockable& m,
    HBOOST_FWD_REF(Func) func,
    HBOOST_FWD_REF(Arg1) arg1,
    HBOOST_FWD_REF(Arg2) arg2
) {
  hboost::lock_guard<Lockable> lock(m);
  return func(
      hboost::forward<Arg1>(arg1),
      hboost::forward<Arg2>(arg2)
  );
}

template <class Lockable, class Func, class Arg1, class Arg2, class Arg3>
typename hboost::result_of<Func(Arg1, Arg2, Arg3)>::type with_lock_guard(
    Lockable& m,
    HBOOST_FWD_REF(Func) func,
    HBOOST_FWD_REF(Arg1) arg1,
    HBOOST_FWD_REF(Arg2) arg2,
    HBOOST_FWD_REF(Arg3) arg3
) {
  hboost::lock_guard<Lockable> lock(m);
  return func(
      hboost::forward<Arg1>(arg1),
      hboost::forward<Arg2>(arg2),
      hboost::forward<Arg3>(arg3)
  );
}

template <
    class Lockable, class Func, class Arg1, class Arg2, class Arg3, class Arg4
>
typename hboost::result_of<Func(Arg1, Arg2, Arg3, Arg4)>::type with_lock_guard(
    Lockable& m,
    HBOOST_FWD_REF(Func) func,
    HBOOST_FWD_REF(Arg1) arg1,
    HBOOST_FWD_REF(Arg2) arg2,
    HBOOST_FWD_REF(Arg3) arg3,
    HBOOST_FWD_REF(Arg4) arg4
) {
  hboost::lock_guard<Lockable> lock(m);
  return func(
      hboost::forward<Arg1>(arg1),
      hboost::forward<Arg2>(arg2),
      hboost::forward<Arg3>(arg3),
      hboost::forward<Arg4>(arg4)
  );
}

// overloads for function pointer
// (if argument is not function pointer, static assert will trigger)
template <class Lockable, class Func>
typename hboost::result_of<
    typename hboost::add_pointer<Func>::type()
>::type with_lock_guard(
    Lockable& m,
    Func* func
) {
  HBOOST_STATIC_ASSERT(hboost::is_function<Func>::value);

  hboost::lock_guard<Lockable> lock(m);
  return func();
}

template <class Lockable, class Func, class Arg>
typename hboost::result_of<
    typename hboost::add_pointer<Func>::type(Arg)
>::type with_lock_guard(
    Lockable& m,
    Func* func,
    HBOOST_FWD_REF(Arg) arg
) {
  HBOOST_STATIC_ASSERT(hboost::is_function<Func>::value);

  hboost::lock_guard<Lockable> lock(m);
  return func(
      hboost::forward<Arg>(arg)
  );
}

template <class Lockable, class Func, class Arg1, class Arg2>
typename hboost::result_of<
    typename hboost::add_pointer<Func>::type(Arg1, Arg2)
>::type with_lock_guard(
    Lockable& m,
    Func* func,
    HBOOST_FWD_REF(Arg1) arg1,
    HBOOST_FWD_REF(Arg2) arg2
) {
  HBOOST_STATIC_ASSERT(hboost::is_function<Func>::value);

  hboost::lock_guard<Lockable> lock(m);
  return func(
      hboost::forward<Arg1>(arg1),
      hboost::forward<Arg2>(arg2)
  );
}

template <class Lockable, class Func, class Arg1, class Arg2, class Arg3>
typename hboost::result_of<
    typename hboost::add_pointer<Func>::type(Arg1, Arg2, Arg3)
>::type with_lock_guard(
    Lockable& m,
    Func* func,
    HBOOST_FWD_REF(Arg1) arg1,
    HBOOST_FWD_REF(Arg2) arg2,
    HBOOST_FWD_REF(Arg3) arg3
) {
  HBOOST_STATIC_ASSERT(hboost::is_function<Func>::value);

  hboost::lock_guard<Lockable> lock(m);
  return func(
      hboost::forward<Arg1>(arg1),
      hboost::forward<Arg2>(arg2),
      hboost::forward<Arg3>(arg3)
  );
}

template <
    class Lockable, class Func, class Arg1, class Arg2, class Arg3, class Arg4
>
typename hboost::result_of<
    typename hboost::add_pointer<Func>::type(Arg1, Arg2, Arg3, Arg4)
>::type with_lock_guard(
    Lockable& m,
    Func* func,
    HBOOST_FWD_REF(Arg1) arg1,
    HBOOST_FWD_REF(Arg2) arg2,
    HBOOST_FWD_REF(Arg3) arg3,
    HBOOST_FWD_REF(Arg4) arg4
) {
  HBOOST_STATIC_ASSERT(hboost::is_function<Func>::value);

  hboost::lock_guard<Lockable> lock(m);
  return func(
      hboost::forward<Arg1>(arg1),
      hboost::forward<Arg2>(arg2),
      hboost::forward<Arg3>(arg3),
      hboost::forward<Arg4>(arg4)
  );
}

#endif

} // namespace hboost

#endif // HBOOST_THREAD_WITH_LOCK_GUARD_HPP

