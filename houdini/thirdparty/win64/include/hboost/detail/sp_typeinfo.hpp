#ifndef HBOOST_DETAIL_SP_TYPEINFO_HPP_INCLUDED
#define HBOOST_DETAIL_SP_TYPEINFO_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//  detail/sp_typeinfo.hpp
//
//  Deprecated, please use hboost/core/typeinfo.hpp
//
//  Copyright 2007 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/core/typeinfo.hpp>

namespace hboost
{

namespace detail
{

typedef hboost::core::typeinfo sp_typeinfo;

} // namespace detail

} // namespace hboost

#define HBOOST_SP_TYPEID(T) HBOOST_CORE_TYPEID(T)

#endif  // #ifndef HBOOST_DETAIL_SP_TYPEINFO_HPP_INCLUDED
