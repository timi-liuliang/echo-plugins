
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_VARIADIC_POP_FRONT_HPP)
#define HBOOST_VMD_DETAIL_VARIADIC_POP_FRONT_HPP

#include <hboost/preprocessor/tuple/enum.hpp>
#include <hboost/preprocessor/tuple/pop_front.hpp>
#include <hboost/preprocessor/variadic/to_tuple.hpp>

#define HBOOST_VMD_DETAIL_VARIADIC_POP_FRONT(...) \
    HBOOST_PP_TUPLE_ENUM \
        ( \
        HBOOST_PP_TUPLE_POP_FRONT \
            ( \
            HBOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__) \
            ) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_VARIADIC_POP_FRONT_HPP */
