
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_LIST_HPP)
#define HBOOST_VMD_DETAIL_LIST_HPP

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/pop_back.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/empty_result.hpp>
#include <hboost/vmd/detail/identifier.hpp>
#include <hboost/vmd/detail/is_list.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/parens.hpp>

#define HBOOST_VMD_DETAIL_LIST_CHECK_FOR_LIST(tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST \
            ( \
            HBOOST_PP_TUPLE_ELEM \
                ( \
                0, \
                tuple \
                ) \
            ), \
        tuple, \
        (,) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_LIST_CHECK_FOR_LIST_D(d,tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST_D \
            ( \
            d, \
            HBOOST_PP_TUPLE_ELEM \
                ( \
                0, \
                tuple \
                ) \
            ), \
        tuple, \
        (,) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_LIST_CHECK_RETURN(tuple) \
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
        HBOOST_VMD_DETAIL_LIST_CHECK_FOR_LIST \
        ) \
    (tuple) \
/**/

#define HBOOST_VMD_DETAIL_LIST_CHECK_RETURN_D(d,tuple) \
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
        HBOOST_VMD_DETAIL_LIST_CHECK_FOR_LIST_D \
        ) \
    (d,tuple) \
/**/

#define HBOOST_VMD_DETAIL_LIST_EMPTY_LIST(list) \
    HBOOST_VMD_DETAIL_IDENTIFIER(list,HBOOST_PP_NIL,HBOOST_VMD_RETURN_AFTER,HBOOST_VMD_RETURN_INDEX) \
/**/

#define HBOOST_VMD_DETAIL_LIST_EMPTY_LIST_D(d,list) \
    HBOOST_VMD_DETAIL_IDENTIFIER_D(d,list,HBOOST_PP_NIL,HBOOST_VMD_RETURN_AFTER,HBOOST_VMD_RETURN_INDEX) \
/**/

#define HBOOST_VMD_DETAIL_LIST_TUPLE(param) \
    HBOOST_VMD_DETAIL_LIST_CHECK_RETURN \
        ( \
        HBOOST_VMD_DETAIL_PARENS(param,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_LIST_TUPLE_D(d,param) \
    HBOOST_VMD_DETAIL_LIST_CHECK_RETURN_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_PARENS_D(d,param,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_LIST_EMPTY_PROCESS(tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
            HBOOST_PP_TUPLE_ELEM(0,tuple) \
            ), \
        (,), \
        tuple \
        ) \
/**/

#define HBOOST_VMD_DETAIL_LIST_EMPTY(list) \
    HBOOST_VMD_DETAIL_LIST_EMPTY_PROCESS \
        ( \
        HBOOST_VMD_DETAIL_LIST_EMPTY_LIST(list) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_LIST_EMPTY_D(d,list) \
    HBOOST_VMD_DETAIL_LIST_EMPTY_PROCESS \
        ( \
        HBOOST_VMD_DETAIL_LIST_EMPTY_LIST_D(d,list) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_LIST_PROCESS(list) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_IS_BEGIN_PARENS(list), \
        HBOOST_VMD_DETAIL_LIST_TUPLE, \
        HBOOST_VMD_DETAIL_LIST_EMPTY \
        ) \
    (list) \
/**/

#define HBOOST_VMD_DETAIL_LIST_SPLIT(list) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(list), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_LIST_PROCESS \
        ) \
    (list) \
/**/

#define HBOOST_VMD_DETAIL_LIST_BEGIN(list) \
    HBOOST_PP_TUPLE_ELEM(0,HBOOST_VMD_DETAIL_LIST_SPLIT(list)) \
/**/

#define HBOOST_VMD_DETAIL_LIST_PROCESS_D(d,list) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_IS_BEGIN_PARENS(list), \
        HBOOST_VMD_DETAIL_LIST_TUPLE_D, \
        HBOOST_VMD_DETAIL_LIST_EMPTY_D \
        ) \
    (d,list) \
/**/

#define HBOOST_VMD_DETAIL_LIST_SPLIT_D(d,list) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(list), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_LIST_PROCESS_D \
        ) \
    (d,list) \
/**/

#define HBOOST_VMD_DETAIL_LIST_BEGIN_D(d,list) \
    HBOOST_PP_TUPLE_ELEM(0,HBOOST_VMD_DETAIL_LIST_SPLIT_D(d,list)) \
/**/

#define HBOOST_VMD_DETAIL_LIST_D(d,...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS_D(d,HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_LIST_SPLIT_D, \
        HBOOST_VMD_DETAIL_LIST_BEGIN_D \
        ) \
    (d,HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_LIST(...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_LIST_SPLIT, \
        HBOOST_VMD_DETAIL_LIST_BEGIN \
        ) \
    (HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#endif /* HBOOST_VMD_DETAIL_LIST_HPP */
