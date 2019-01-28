//  (C) Copyright 2008-10 Anthony Williams
//  (C) Copyright 2011-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_FUTURES_IS_FUTURE_TYPE_HPP
#define HBOOST_THREAD_FUTURES_IS_FUTURE_TYPE_HPP

#include <hboost/type_traits/integral_constant.hpp>

namespace hboost
{
    template<typename T>
    struct is_future_type : false_type
    {
    };
}

#endif // header
