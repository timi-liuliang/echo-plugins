//  (C) Copyright 2008-10 Anthony Williams
//  (C) Copyright 2011-2012,2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_FUTURES_FUTURE_ERROR_CODE_HPP
#define HBOOST_THREAD_FUTURES_FUTURE_ERROR_CODE_HPP

#include <hboost/thread/detail/config.hpp>
#include <hboost/core/scoped_enum.hpp>
#include <hboost/system/error_code.hpp>
#include <hboost/type_traits/integral_constant.hpp>

namespace hboost
{

  //enum class future_errc
  HBOOST_SCOPED_ENUM_DECLARE_BEGIN(future_errc)
  {
      broken_promise = 1,
      future_already_retrieved,
      promise_already_satisfied,
      no_state
  }
  HBOOST_SCOPED_ENUM_DECLARE_END(future_errc)

  namespace system
  {
    template <>
    struct HBOOST_SYMBOL_VISIBLE is_error_code_enum< ::hboost::future_errc> : public true_type {};

    #ifdef HBOOST_NO_CXX11_SCOPED_ENUMS
    template <>
    struct HBOOST_SYMBOL_VISIBLE is_error_code_enum< ::hboost::future_errc::enum_type> : public true_type { };
    #endif
  } // system

  HBOOST_THREAD_DECL
  const system::error_category& future_category() HBOOST_NOEXCEPT;

  namespace system
  {
    inline
    error_code
    make_error_code(future_errc e) HBOOST_NOEXCEPT
    {
        return error_code(underlying_cast<int>(e), hboost::future_category());
    }

    inline
    error_condition
    make_error_condition(future_errc e) HBOOST_NOEXCEPT
    {
        return error_condition(underlying_cast<int>(e), hboost::future_category());
    }
  } // system
} // boost

#endif // header
