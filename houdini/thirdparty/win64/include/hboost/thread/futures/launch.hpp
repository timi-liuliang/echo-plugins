//  (C) Copyright 2008-10 Anthony Williams
//  (C) Copyright 2011-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_FUTURES_LAUNCH_HPP
#define HBOOST_THREAD_FUTURES_LAUNCH_HPP

#include <hboost/thread/detail/config.hpp>
#include <hboost/core/scoped_enum.hpp>

namespace hboost
{
  //enum class launch
  HBOOST_SCOPED_ENUM_DECLARE_BEGIN(launch)
  {
      none = 0,
      async = 1,
      deferred = 2,
#ifdef HBOOST_THREAD_PROVIDES_EXECUTORS
      executor = 4,
#endif
      inherit = 8,
      sync = 16,
      any = async | deferred
  }
  HBOOST_SCOPED_ENUM_DECLARE_END(launch)
}

#endif // header