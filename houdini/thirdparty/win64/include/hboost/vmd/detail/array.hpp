
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_ARRAY_HPP)
#define HBOOST_VMD_DETAIL_ARRAY_HPP

#include <hboost/preprocessor/control/expr_iif.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/empty_result.hpp>
#include <hboost/vmd/detail/is_array_common.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/tuple.hpp>

#define HBOOST_VMD_DETAIL_ARRAY_CHECK_FOR_ARRAY(tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX \
                ( \
                HBOOST_PP_TUPLE_ELEM \
                    ( \
                    0, \
                    tuple \
                    ) \
                ), \
            HBOOST_VMD_IDENTITY(tuple), \
            HBOOST_VMD_DETAIL_EMPTY_RESULT \
            ) \
        (tuple)    \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_CHECK_FOR_ARRAY_D(d,tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX_D \
                ( \
                d, \
                HBOOST_PP_TUPLE_ELEM \
                    ( \
                    0, \
                    tuple \
                    ) \
                ), \
            HBOOST_VMD_IDENTITY(tuple), \
            HBOOST_VMD_DETAIL_EMPTY_RESULT \
            ) \
        (tuple)    \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_PROCESS(tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
            HBOOST_PP_TUPLE_ELEM \
                ( \
                0, \
                tuple \
                ) \
            ), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_ARRAY_CHECK_FOR_ARRAY \
        ) \
    (tuple) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_PROCESS_D(d,tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
            HBOOST_PP_TUPLE_ELEM \
                ( \
                0, \
                tuple \
                ) \
            ), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_ARRAY_CHECK_FOR_ARRAY_D \
        ) \
    (d,tuple) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_SPLIT(param) \
    HBOOST_VMD_DETAIL_ARRAY_PROCESS \
        ( \
        HBOOST_VMD_DETAIL_TUPLE(param,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_SPLIT_D(d,param) \
    HBOOST_VMD_DETAIL_ARRAY_PROCESS_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_TUPLE_D(d,param,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_BEGIN_CHECK_FOR_ARRAY(tuple) \
    HBOOST_PP_EXPR_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX(tuple), \
        tuple \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_BEGIN_CHECK_FOR_ARRAY_D(d,tuple) \
    HBOOST_PP_EXPR_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX_D(d,tuple), \
        tuple \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_BEGIN_TUPLE(tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(tuple), \
        HBOOST_VMD_EMPTY, \
        HBOOST_VMD_DETAIL_ARRAY_BEGIN_CHECK_FOR_ARRAY \
        ) \
    (tuple) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_BEGIN_TUPLE_D(d,tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(tuple), \
        HBOOST_VMD_EMPTY, \
        HBOOST_VMD_DETAIL_ARRAY_BEGIN_CHECK_FOR_ARRAY_D \
        ) \
    (d,tuple) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_BEGIN(param) \
    HBOOST_VMD_DETAIL_ARRAY_BEGIN_TUPLE \
        ( \
        HBOOST_VMD_DETAIL_TUPLE(param) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_BEGIN_D(d,param) \
    HBOOST_VMD_DETAIL_ARRAY_BEGIN_TUPLE_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_TUPLE_D(d,param) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY(...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_ARRAY_SPLIT, \
        HBOOST_VMD_DETAIL_ARRAY_BEGIN \
        ) \
    (HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_ARRAY_D(d,...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_ARRAY_SPLIT_D, \
        HBOOST_VMD_DETAIL_ARRAY_BEGIN_D \
        ) \
    (d,HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#endif /* HBOOST_VMD_DETAIL_ARRAY_HPP */
