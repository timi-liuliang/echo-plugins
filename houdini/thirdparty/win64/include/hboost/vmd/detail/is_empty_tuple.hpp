
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_HPP)
#define HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_HPP

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/is_tuple.hpp>

#define HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_CEM(tuple) \
    HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(0,tuple)) \
/**/

#define HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_SIZE(tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL(HBOOST_PP_TUPLE_SIZE(tuple),1), \
            HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_CEM, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (tuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_SIZE_D(d,tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL_D(d,HBOOST_PP_TUPLE_SIZE(tuple),1), \
            HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_CEM, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (tuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE(tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_TUPLE(tuple), \
            HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_SIZE, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (tuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_D(d,tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_TUPLE(tuple), \
            HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_SIZE_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,tuple) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_EMPTY_TUPLE_HPP */
