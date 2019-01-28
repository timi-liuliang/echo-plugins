//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_MATH_TOOLS_REAL_CAST_HPP
#define HBOOST_MATH_TOOLS_REAL_CAST_HPP

#include <hboost/math/tools/config.hpp>

#ifdef _MSC_VER
#pragma once
#endif

namespace hboost{ namespace math
{
  namespace tools
  {
    template <class To, class T>
    inline HBOOST_MATH_CONSTEXPR To real_cast(T t) HBOOST_NOEXCEPT_IF(HBOOST_MATH_IS_FLOAT(T) && HBOOST_MATH_IS_FLOAT(To))
    {
       return static_cast<To>(t);
    }
  } // namespace tools
} // namespace math
} // namespace hboost

#endif // HBOOST_MATH_TOOLS_REAL_CAST_HPP



