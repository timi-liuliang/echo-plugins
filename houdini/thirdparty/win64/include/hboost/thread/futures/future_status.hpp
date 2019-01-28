//  (C) Copyright 2008-10 Anthony Williams
//  (C) Copyright 2011-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_FUTURES_FUTURE_STATUS_HPP
#define HBOOST_THREAD_FUTURES_FUTURE_STATUS_HPP

#include <hboost/thread/detail/config.hpp>
#include <hboost/core/scoped_enum.hpp>

namespace hboost
{
  //enum class future_status
  HBOOST_SCOPED_ENUM_DECLARE_BEGIN(future_status)
  {
      ready,
      timeout,
      deferred
  }
  HBOOST_SCOPED_ENUM_DECLARE_END(future_status)
  namespace future_state
  {
      enum state { uninitialized, waiting, ready, moved, deferred };
  }
}

#endif // header
