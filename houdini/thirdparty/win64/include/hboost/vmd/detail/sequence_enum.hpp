
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SEQUENCE_ENUM_HPP)
#define HBOOST_VMD_DETAIL_SEQUENCE_ENUM_HPP

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/enum.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/sequence_to_tuple.hpp>

#define HBOOST_VMD_DETAIL_SEQUENCE_ENUM_PROCESS(tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(tuple), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_TUPLE_ENUM \
        ) \
    (tuple) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ENUM(...) \
    HBOOST_VMD_DETAIL_SEQUENCE_ENUM_PROCESS \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_TO_TUPLE(__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ENUM_D(d,...) \
    HBOOST_VMD_DETAIL_SEQUENCE_ENUM_PROCESS \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_TO_TUPLE_D(d,__VA_ARGS__) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_SEQUENCE_ENUM_HPP */
