
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SEQUENCE_TYPE_HPP)
#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_HPP

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/preprocessor/variadic/size.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/equal_type.hpp>
#include <hboost/vmd/detail/is_array_common.hpp>
#include <hboost/vmd/detail/is_list.hpp>
#include <hboost/vmd/detail/modifiers.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/sequence_elem.hpp>

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_ARRAY(dtuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX(HBOOST_PP_TUPLE_ELEM(1,dtuple)), \
        HBOOST_VMD_TYPE_ARRAY, \
        HBOOST_VMD_TYPE_TUPLE \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_ARRAY_D(d,dtuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX_D(d,HBOOST_PP_TUPLE_ELEM(1,dtuple)), \
        HBOOST_VMD_TYPE_ARRAY, \
        HBOOST_VMD_TYPE_TUPLE \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_LIST(dtuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST(HBOOST_PP_TUPLE_ELEM(1,dtuple)), \
        HBOOST_VMD_TYPE_LIST, \
        HBOOST_VMD_TYPE_TUPLE \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_LIST_D(d,dtuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST_D(d,HBOOST_PP_TUPLE_ELEM(1,dtuple)), \
        HBOOST_VMD_TYPE_LIST, \
        HBOOST_VMD_TYPE_TUPLE \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_BOTH(dtuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE \
                ( \
                HBOOST_VMD_TYPE_TUPLE, \
                HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_LIST(dtuple) \
                ), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_ARRAY, \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_LIST) \
            ) \
        (dtuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_BOTH_D(d,dtuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
                ( \
                d, \
                HBOOST_VMD_TYPE_TUPLE, \
                HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_LIST_D(d,dtuple) \
                ), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_ARRAY_D, \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_LIST) \
            ) \
        (d,dtuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_MODS(dtuple,rtype) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL(rtype,HBOOST_VMD_DETAIL_MODS_RETURN_ARRAY), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_ARRAY, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_PP_EQUAL(rtype,HBOOST_VMD_DETAIL_MODS_RETURN_LIST), \
                HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_LIST, \
                HBOOST_PP_IIF \
                    ( \
                    HBOOST_PP_EQUAL(rtype,HBOOST_VMD_DETAIL_MODS_RETURN_TUPLE), \
                    HBOOST_VMD_IDENTITY(HBOOST_PP_TUPLE_ELEM(0,dtuple)), \
                    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_BOTH \
                    ) \
                ) \
            ) \
        ) \
    (dtuple) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_MODS_D(d,dtuple,rtype) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL_D(d,rtype,HBOOST_VMD_DETAIL_MODS_RETURN_ARRAY), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_ARRAY_D, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_PP_EQUAL_D(d,rtype,HBOOST_VMD_DETAIL_MODS_RETURN_LIST), \
                HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_LIST_D, \
                HBOOST_PP_IIF \
                    ( \
                    HBOOST_PP_EQUAL_D(d,rtype,HBOOST_VMD_DETAIL_MODS_RETURN_TUPLE), \
                    HBOOST_VMD_IDENTITY(HBOOST_PP_TUPLE_ELEM(0,dtuple)), \
                    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_BOTH_D \
                    ) \
                ) \
            ) \
        ) \
    (d,dtuple) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_MORE(dtuple,...) \
    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_MODS \
        ( \
        dtuple, \
        HBOOST_VMD_DETAIL_MODS_RESULT_RETURN_TYPE \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_ALL,__VA_ARGS__) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_MORE_D(d,dtuple,...) \
    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_MODS_D \
        ( \
        d, \
        dtuple, \
        HBOOST_VMD_DETAIL_MODS_RESULT_RETURN_TYPE \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS_D(d,HBOOST_VMD_ALLOW_ALL,__VA_ARGS__) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_UNARY(dtuple,...) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE(HBOOST_VMD_TYPE_TUPLE,HBOOST_PP_TUPLE_ELEM(0,dtuple)), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_MORE, \
            HBOOST_VMD_IDENTITY(HBOOST_PP_TUPLE_ELEM(0,dtuple)) \
            ) \
        (dtuple,__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_UNARY_D(d,dtuple,...) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,HBOOST_VMD_TYPE_TUPLE,HBOOST_PP_TUPLE_ELEM(0,dtuple)), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_MORE_D, \
            HBOOST_VMD_IDENTITY(HBOOST_PP_TUPLE_ELEM(0,dtuple)) \
            ) \
        (d,dtuple,__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_SEQUENCE(tuple,...) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(1,tuple)), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_UNARY, \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_SEQUENCE) \
            ) \
        (HBOOST_PP_TUPLE_ELEM(0,tuple),__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_SEQUENCE_D(d,tuple,...) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(1,tuple)), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_UNARY_D, \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_SEQUENCE) \
            ) \
        (d,HBOOST_PP_TUPLE_ELEM(0,tuple),__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE(tuple,...) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(0,tuple)), \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_EMPTY), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_SEQUENCE \
            ) \
        (tuple,__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_D(d,tuple,...) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(0,tuple)), \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_EMPTY), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_SEQUENCE_D \
            ) \
        (d,tuple,__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE(...) \
    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM \
            ( \
            HBOOST_VMD_ALLOW_ALL, \
            0, \
            HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__), \
            HBOOST_VMD_RETURN_AFTER, \
            HBOOST_VMD_RETURN_TYPE_TUPLE \
            ), \
        __VA_ARGS__ \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TYPE_D(d,...) \
    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_TUPLE_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_D \
            ( \
            d, \
            HBOOST_VMD_ALLOW_ALL, \
            0, \
            HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__), \
            HBOOST_VMD_RETURN_AFTER, \
            HBOOST_VMD_RETURN_TYPE_TUPLE \
            ), \
        __VA_ARGS__ \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_SEQUENCE_TYPE_HPP */
