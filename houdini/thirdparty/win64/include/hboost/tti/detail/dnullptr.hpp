
//  (C) Copyright Edward Diener 2012
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_TTI_DETAIL_NULLPTR_HPP)
#define HBOOST_TTI_DETAIL_NULLPTR_HPP

#include <hboost/config.hpp>

#if defined(HBOOST_NO_CXX11_NULLPTR)

#define HBOOST_TTI_DETAIL_NULLPTR 0

#else // !HBOOST_NO_CXX11_NULLPTR

#define HBOOST_TTI_DETAIL_NULLPTR nullptr

#endif // HBOOST_NO_CXX11_NULLPTR

#endif // HBOOST_TTI_DETAIL_NULLPTR_HPP
