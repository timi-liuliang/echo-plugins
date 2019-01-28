
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_DATA_EQUAL_COMMON_HPP)
#define HBOOST_VMD_DETAIL_DATA_EQUAL_COMMON_HPP

#include <hboost/preprocessor/arithmetic/inc.hpp>
#include <hboost/preprocessor/array/elem.hpp>
#include <hboost/preprocessor/array/size.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/list/at.hpp>
#include <hboost/preprocessor/list/size.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/logical/bitor.hpp>
#include <hboost/preprocessor/logical/compl.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/seq/elem.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/pop_front.hpp>
#include <hboost/preprocessor/tuple/push_back.hpp>
#include <hboost/preprocessor/tuple/replace.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/get_type.hpp>
#include <hboost/vmd/detail/equal_type.hpp>

#define HBOOST_VMD_DETAIL_DATA_EQUAL_IS_BOTH_COMPOSITE(vseq1,vseq2) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_PP_IS_BEGIN_PARENS(vseq1), \
        HBOOST_PP_IS_BEGIN_PARENS(vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM_ARRAY(d,index,data) \
    HBOOST_PP_ARRAY_ELEM(index,data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM_LIST(d,index,data) \
    HBOOST_PP_LIST_AT_D(d,data,index) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM_SEQ(d,index,data) \
    HBOOST_PP_SEQ_ELEM(index,data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM_TUPLE(d,index,data) \
    HBOOST_PP_TUPLE_ELEM(index,data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_ARRAY(data) \
    HBOOST_PP_ARRAY_SIZE(data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_LIST(data) \
    HBOOST_PP_LIST_SIZE(data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_SEQ(data) \
    HBOOST_PP_SEQ_SIZE(data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_TUPLE(data) \
    HBOOST_PP_TUPLE_SIZE(data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_ARRAY_D(d,data) \
    HBOOST_PP_ARRAY_SIZE(data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_LIST_D(d,data) \
    HBOOST_PP_LIST_SIZE_D(d,data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_SEQ_D(d,data) \
    HBOOST_PP_SEQ_SIZE(data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_TUPLE_D(d,data) \
    HBOOST_PP_TUPLE_SIZE(data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM(d,index,data,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_ARRAY), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM_ARRAY, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_LIST), \
            HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM_LIST, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_SEQ), \
                HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM_SEQ, \
                HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM_TUPLE \
                ) \
            ) \
        ) \
    (d,index,data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE(data,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_ARRAY), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_ARRAY, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_LIST), \
            HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_LIST, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_SEQ), \
                HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_SEQ, \
                HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_TUPLE \
                ) \
            ) \
        ) \
    (data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_D(d,data,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_ARRAY), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_ARRAY_D, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_LIST), \
            HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_LIST_D, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_SEQ), \
                HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_SEQ_D, \
                HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_TUPLE_D \
                ) \
            ) \
        ) \
    (d,data) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_RESULT(state) \
    HBOOST_PP_TUPLE_ELEM(0,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_FIRST(state) \
    HBOOST_PP_TUPLE_ELEM(1,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_SECOND(state) \
    HBOOST_PP_TUPLE_ELEM(2,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_SIZE(state) \
    HBOOST_PP_TUPLE_ELEM(3,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_TYPE(state) \
    HBOOST_PP_TUPLE_ELEM(4,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_INDEX(state) \
    HBOOST_PP_TUPLE_ELEM(5,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP(state) \
    HBOOST_PP_TUPLE_ELEM(6,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PROCESSING(d,state) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_PP_EQUAL_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_INDEX(state), \
            HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_SIZE(state) \
            ), \
        HBOOST_PP_COMPL(HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_EMPTY(state)) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_EMPTY(state) \
    HBOOST_VMD_IS_EMPTY(HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP(state)) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_INDEX(state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_EMPTY(state), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_TUPLE_ELEM \
        ) \
    (0,HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP(state)) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_POP_NE_EMPTY(d,state) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        state, \
        6, \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_POP_NE_REMOVE(d,state) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        state, \
        6, \
        HBOOST_PP_TUPLE_POP_FRONT(HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP(state)) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_POP(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL_D(d,HBOOST_PP_TUPLE_SIZE(HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP(state)),1), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_POP_NE_EMPTY, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_POP_NE_REMOVE \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PUSH_CREATE(d,state) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_OP_SUCCESS \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            6, \
            (HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_INDEX(state)) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PUSH_ADD(d,state) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_OP_SUCCESS \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            6, \
            HBOOST_PP_TUPLE_PUSH_BACK \
                ( \
                HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP(state), \
                HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_INDEX(state) \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PUSH(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_EMPTY(state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PUSH_CREATE, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PUSH_ADD \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_INDEX(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PROCESSING(d,state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_INDEX, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_INDEX \
        ) \
    (state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_FIRST_ELEMENT(d,state) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM \
        ( \
        d, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_INDEX(d,state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_FIRST(state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_TYPE(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_SECOND_ELEMENT(d,state) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_GET_ELEM \
        ( \
        d, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_INDEX(d,state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_SECOND(state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_TYPE(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_PRED(d,state) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_PP_EQUAL_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_RESULT(state), \
            1 \
            ), \
        HBOOST_PP_BITOR \
            ( \
            HBOOST_PP_NOT_EQUAL_D \
                ( \
                d, \
                HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_INDEX(state), \
                HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_SIZE(state) \
                ), \
            HBOOST_PP_COMPL \
                ( \
                HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_EMPTY(state) \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_OP_SUCCESS_NCP(d,state) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        state, \
        5, \
        HBOOST_PP_INC(HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_INDEX(state)) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_OP_SUCCESS(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PROCESSING(d,state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_POP, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_OP_SUCCESS_NCP \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_OP_FAILURE(d,state) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        state, \
        0, \
        0 \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_OP_RESULT(d,state,result) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL_D(d,result,0), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_OP_FAILURE, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL_D(d,result,1), \
            HBOOST_VMD_DETAIL_DATA_EQUAL_OP_SUCCESS, \
            HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PUSH \
            ) \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_TYPE(emf,ems,vtype) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE \
            ( \
            HBOOST_VMD_GET_TYPE(emf), \
            vtype \
            ), \
        HBOOST_VMD_DETAIL_EQUAL_TYPE \
            ( \
            HBOOST_VMD_GET_TYPE(ems), \
            vtype \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_TYPE_D(d,emf,ems,vtype) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
            ( \
            d, \
            HBOOST_VMD_GET_TYPE(emf), \
            vtype \
            ), \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
            ( \
            d, \
            HBOOST_VMD_GET_TYPE(ems), \
            vtype \
            ) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_DATA_EQUAL_COMMON_HPP */
