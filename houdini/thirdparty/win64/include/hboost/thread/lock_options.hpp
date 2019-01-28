// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba

#ifndef HBOOST_THREAD_LOCK_OPTIONS_HPP
#define HBOOST_THREAD_LOCK_OPTIONS_HPP

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
  struct defer_lock_t
  {
  };
  struct try_to_lock_t
  {
  };
  struct adopt_lock_t
  {
  };

  HBOOST_CONSTEXPR_OR_CONST defer_lock_t defer_lock = {};
  HBOOST_CONSTEXPR_OR_CONST try_to_lock_t try_to_lock = {};
  HBOOST_CONSTEXPR_OR_CONST adopt_lock_t adopt_lock = {};

}
#include <hboost/config/abi_suffix.hpp>

#endif
