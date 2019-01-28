//  cv_status.hpp
//
// Copyright (C) 2011 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_CV_STATUS_HPP
#define HBOOST_THREAD_CV_STATUS_HPP

#include <hboost/core/scoped_enum.hpp>

namespace hboost
{

  // enum class cv_status;
  HBOOST_SCOPED_ENUM_DECLARE_BEGIN(cv_status)
  {
    no_timeout,
    timeout
  }
  HBOOST_SCOPED_ENUM_DECLARE_END(cv_status)
}

#endif // header
