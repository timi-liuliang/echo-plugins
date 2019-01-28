
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SEQUENCE_COMMON_HPP)
#define HBOOST_VMD_DETAIL_SEQUENCE_COMMON_HPP

#include <hboost/preprocessor/arithmetic/inc.hpp>
#include <hboost/preprocessor/array/push_back.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/comparison/less_equal.hpp>
#include <hboost/preprocessor/comparison/not_equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/control/while.hpp>
#include <hboost/preprocessor/list/append.hpp>
#include <hboost/preprocessor/logical/bitor.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/seq/push_back.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/push_back.hpp>
#include <hboost/preprocessor/tuple/replace.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/is_empty_list.hpp>
#include <hboost/vmd/detail/array.hpp>
#include <hboost/vmd/detail/equal_type.hpp>
#include <hboost/vmd/detail/identifier.hpp>
#include <hboost/vmd/detail/identifier_type.hpp>
#include <hboost/vmd/detail/list.hpp>
#include <hboost/vmd/detail/modifiers.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/seq.hpp>
#include <hboost/vmd/detail/tuple.hpp>

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT_ELEM 0
#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ELEM 1
#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM_ELEM 2
#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_OUTTYPE_ELEM 3
#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM_ELEM 4
#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES_ELEM 5
#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX_ELEM 6

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT_ELEM,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ELEM,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state) \
        HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM_ELEM,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_OUTTYPE(state) \
        HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_OUTTYPE_ELEM,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state) \
        HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM_ELEM,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_RETURN(d,from,number) \
    HBOOST_PP_EQUAL_D \
        ( \
        d, \
        HBOOST_PP_TUPLE_ELEM(0,from), \
        number \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_NO_RETURN(d,from) \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_RETURN(d,from,HBOOST_VMD_DETAIL_MODS_NO_RETURN) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_EXACT_RETURN(d,from) \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_RETURN(d,from,HBOOST_VMD_DETAIL_MODS_RETURN) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_GENERAL_RETURN(d,from) \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_RETURN(d,from,HBOOST_VMD_DETAIL_MODS_RETURN_TUPLE) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_ARRAY_RETURN(d,from) \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_RETURN(d,from,HBOOST_VMD_DETAIL_MODS_RETURN_ARRAY) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_LIST_RETURN(d,from) \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_RETURN(d,from,HBOOST_VMD_DETAIL_MODS_RETURN_LIST) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER(from) \
    HBOOST_PP_EQUAL \
        ( \
        HBOOST_PP_TUPLE_ELEM(1,from), \
        HBOOST_VMD_DETAIL_MODS_RETURN_AFTER \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER_D(d,from) \
    HBOOST_PP_EQUAL_D \
        ( \
        d, \
        HBOOST_PP_TUPLE_ELEM(1,from), \
        HBOOST_VMD_DETAIL_MODS_RETURN_AFTER \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES(state) \
        HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES_ELEM,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX(state) \
        HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX_ELEM,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_IS_EMPTY(state) \
    HBOOST_VMD_IS_EMPTY \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_FROM_EMPTY(state,data) \
    (data) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_TO_SEQ(state,data) \
    HBOOST_PP_SEQ_PUSH_BACK(HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state),data) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_TO_TUPLE(state,data) \
    HBOOST_PP_TUPLE_PUSH_BACK(HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state),data) \
/**/

// Array

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_HBOOST_VMD_TYPE_ARRAY(d,state,data) \
    HBOOST_PP_ARRAY_PUSH_BACK(HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state),data) \
/**/

// List

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_HBOOST_VMD_TYPE_LIST(d,state,data) \
    HBOOST_PP_LIST_APPEND_D(d,HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state),(data,HBOOST_PP_NIL)) \
/**/

// Seq

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_HBOOST_VMD_TYPE_SEQ(d,state,data) \
    HBOOST_PP_IIF \
         ( \
         HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_IS_EMPTY(state), \
         HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_FROM_EMPTY, \
         HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_TO_SEQ \
         ) \
     (state,data) \
/**/

// Tuple

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_HBOOST_VMD_TYPE_TUPLE(d,state,data) \
    HBOOST_PP_IIF \
         ( \
         HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_IS_EMPTY(state), \
         HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_FROM_EMPTY, \
         HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_TO_TUPLE \
         ) \
     (state,data) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_GET_NAME(state) \
    HBOOST_PP_CAT \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_, \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_OUTTYPE(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_PROCESS(d,name,state,data) \
    name(d,state,data) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_GET_DATA(d,state,tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_NO_RETURN \
                ( \
                d, \
                HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state) \
                ), \
            HBOOST_PP_TUPLE_ELEM, \
            HBOOST_VMD_IDENTITY(tuple) \
            ) \
        (1,tuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD(d,state,ttuple) \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_PROCESS \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_GET_NAME(state), \
        state, \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD_GET_DATA(d,state,ttuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_PROCESSING_ELEM_CHECK(d,state) \
    HBOOST_PP_EQUAL_D \
        ( \
        d, \
        HBOOST_PP_SEQ_SIZE(HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state)), \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_PROCESSING_ELEM(d,state) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state)), \
            HBOOST_VMD_IDENTITY(0), \
            HBOOST_VMD_DETAIL_SEQUENCE_PROCESSING_ELEM_CHECK \
            ) \
        (d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_GET_TYPE(state) \
        HBOOST_PP_TUPLE_ELEM \
            ( \
            0, \
            HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX(state),HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES(state)) \
            ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_STATE_GET_TYPE_REENTRANT(state) \
        HBOOST_PP_TUPLE_ELEM \
            ( \
            1, \
            HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX(state),HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES(state)) \
            ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_UNKNOWN(d,state) \
    ( \
    , \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD(d,state,(HBOOST_VMD_TYPE_UNKNOWN,HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state))), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_OUTTYPE(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES(state), \
    HBOOST_PP_INC(HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX(state)) \
    ) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_GET_FULL_TYPE_CHECK_ID(d,type,id) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,type,HBOOST_VMD_TYPE_IDENTIFIER), \
            HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_D, \
            HBOOST_VMD_IDENTITY(type) \
            ) \
        (d,id) \
        ) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_GET_FULL_TYPE(d,state,tuple) \
    HBOOST_VMD_DETAIL_SEQUENCE_GET_FULL_TYPE_CHECK_ID \
        ( \
        d, \
        HBOOST_PP_CAT \
            ( \
            HBOOST_VMD_TYPE_, \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_GET_TYPE(state) \
            ), \
        HBOOST_PP_TUPLE_ELEM(0,tuple) \
        ) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_FOUND_PROCESS(d,state,tuple) \
    ( \
    HBOOST_PP_TUPLE_ELEM(1,tuple), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD \
        ( \
        d, \
        state, \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_GET_FULL_TYPE(d,state,tuple), \
            HBOOST_PP_TUPLE_ELEM(0,tuple) \
            ) \
        ), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_OUTTYPE(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES(state), \
    HBOOST_PP_INC(HBOOST_PP_TUPLE_SIZE(HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES(state))) \
    ) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_FOUND_SEQ_SINGLE(d,tuple) \
    HBOOST_PP_EQUAL_D(d,HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(0,tuple)),1) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_FOUND_SEQ(d,state,tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
                ( \
                d, \
                HBOOST_VMD_DETAIL_SEQUENCE_GET_FULL_TYPE(d,state,tuple), \
                HBOOST_VMD_TYPE_SEQ \
                ), \
            HBOOST_VMD_DETAIL_SEQUENCE_TYPE_FOUND_SEQ_SINGLE, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,tuple) \
        ) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_FOUND(d,state,tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_TYPE_FOUND_SEQ(d,state,tuple), \
        HBOOST_VMD_DETAIL_SEQUENCE_INCREMENT_INDEX, \
        HBOOST_VMD_DETAIL_SEQUENCE_TYPE_FOUND_PROCESS \
        ) \
    (d,state,tuple) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_INCREMENT_INDEX(d,state,tuple) \
    HBOOST_PP_TUPLE_REPLACE_D(d,state,HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX_ELEM,HBOOST_PP_INC(HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX(state))) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_TEST_TYPE_TUPLE(d,state,tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
            HBOOST_PP_TUPLE_ELEM(0,tuple) \
            ), \
        HBOOST_VMD_DETAIL_SEQUENCE_INCREMENT_INDEX, \
        HBOOST_VMD_DETAIL_SEQUENCE_TYPE_FOUND \
        ) \
    (d,state,tuple) \
/**/

#define    HBOOST_VMD_DETAIL_SEQUENCE_TEST_TYPE(d,call,state) \
    HBOOST_VMD_DETAIL_SEQUENCE_TEST_TYPE_TUPLE \
        ( \
        d, \
        state, \
        call(HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state),HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/
    
#define    HBOOST_VMD_DETAIL_SEQUENCE_TEST_TYPE_D(d,call,state) \
    HBOOST_VMD_DETAIL_SEQUENCE_TEST_TYPE_TUPLE \
        ( \
        d, \
        state, \
        call(d,HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state),HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/
    
#define HBOOST_VMD_DETAIL_SEQUENCE_GCLRT(state) \
    HBOOST_PP_CAT \
        ( \
        HBOOST_VMD_DETAIL_, \
        HBOOST_PP_CAT(HBOOST_VMD_DETAIL_SEQUENCE_STATE_GET_TYPE(state),_D) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_GCLPL(state) \
    HBOOST_PP_CAT \
        ( \
        HBOOST_VMD_DETAIL_, \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_GET_TYPE(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_TEST_GCL(state,rflag) \
    HBOOST_PP_IIF \
        ( \
        rflag, \
        HBOOST_VMD_DETAIL_SEQUENCE_GCLRT, \
        HBOOST_VMD_DETAIL_SEQUENCE_GCLPL \
        ) \
    (state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_TEST_RT_CALL(d,call,state,rflag) \
    HBOOST_PP_IIF \
        ( \
        rflag, \
        HBOOST_VMD_DETAIL_SEQUENCE_TEST_TYPE_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_TEST_TYPE \
        ) \
    (d,call,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_TEST_RT(d,state,rflag) \
    HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_TEST_RT_CALL \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_TEST_GCL(state,rflag), \
        state, \
        rflag \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_TEST(d,state) \
    HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_TEST_RT \
        ( \
        d, \
        state, \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_GET_TYPE_REENTRANT(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX(state), \
            HBOOST_PP_TUPLE_SIZE(HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES(state)) \
            ), \
        HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_UNKNOWN, \
        HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP_TEST \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_INNER_PRED(d,state) \
    HBOOST_PP_NOT_EQUAL_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_INDEX(state), \
        HBOOST_PP_INC(HBOOST_PP_TUPLE_SIZE(HBOOST_VMD_DETAIL_SEQUENCE_STATE_TYPES(state))) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES_ELEM_FROM(d,from) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_GENERAL_RETURN(d,from), \
        ((SEQ,1),(TUPLE,1)), \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_EXACT_RETURN(d,from), \
            ((SEQ,1),(LIST,1),(ARRAY,1),(TUPLE,1)), \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_ARRAY_RETURN(d,from), \
                ((SEQ,1),(ARRAY,1),(TUPLE,1)), \
                HBOOST_PP_IIF \
                    ( \
                    HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_LIST_RETURN(d,from), \
                    ((SEQ,1),(LIST,1),(TUPLE,1)), \
                    ((SEQ,1),(TUPLE,1)) \
                    ) \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES_ELEM(d,state) \
    HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES_ELEM_FROM \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES_ANY(d,state) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY \
                ( \
                HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state) \
                ), \
            HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES_ELEM, \
            HBOOST_VMD_IDENTITY(((SEQ,1),(TUPLE,1))) \
            ) \
        (d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_PROCESSING_ELEM(d,state), \
        HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES_ELEM, \
        HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES_ANY \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN(d,state) \
    HBOOST_PP_WHILE_ ## d \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_INNER_PRED, \
        HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP, \
        HBOOST_PP_TUPLE_PUSH_BACK \
            ( \
            HBOOST_PP_TUPLE_PUSH_BACK \
                ( \
                state, \
                HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN_TUPLE_TYPES(d,state) \
                ), \
            0 \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_ID_LOOP(d,state) \
    HBOOST_PP_WHILE_ ## d \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_INNER_PRED, \
        HBOOST_VMD_DETAIL_SEQUENCE_INNER_OP, \
        HBOOST_PP_TUPLE_PUSH_BACK(HBOOST_PP_TUPLE_PUSH_BACK(state,((IDENTIFIER,1))),0) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_ID_EL(d,state) \
    ( \
    HBOOST_PP_TUPLE_ELEM(1,HBOOST_VMD_DETAIL_LIST_D(d,HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state),HBOOST_VMD_RETURN_AFTER)), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT_ADD(d,state,(HBOOST_VMD_TYPE_LIST,HBOOST_PP_NIL)), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_OUTTYPE(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state) \
    ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_ID(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY_LIST_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_IDENTIFIER_D \
                ( \
                d, \
                HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state) \
                ) \
            ), \
        HBOOST_VMD_DETAIL_SEQUENCE_OP_ID_EL, \
        HBOOST_VMD_DETAIL_SEQUENCE_OP_ID_LOOP \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP_REDUCE_STATE(state) \
    ( \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_OUTTYPE(state), \
    HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state) \
    ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_OP(d,state) \
    HBOOST_VMD_DETAIL_SEQUENCE_OP_REDUCE_STATE \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_IS_BEGIN_PARENS \
                ( \
                HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state) \
                ), \
            HBOOST_VMD_DETAIL_SEQUENCE_OP_PAREN, \
            HBOOST_VMD_DETAIL_SEQUENCE_OP_ID \
            ) \
        (d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_PRED_CELEM_SZ(d,state) \
    HBOOST_PP_LESS_EQUAL_D \
        ( \
        d, \
        HBOOST_PP_SEQ_SIZE(HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state)), \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_PRED_CELEM(d,state) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITOR \
                ( \
                HBOOST_VMD_IS_EMPTY \
                    ( \
                    HBOOST_VMD_DETAIL_SEQUENCE_STATE_ELEM(state) \
                    ), \
                HBOOST_VMD_IS_EMPTY \
                    ( \
                    HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state) \
                    ) \
                ), \
            HBOOST_VMD_IDENTITY(1), \
            HBOOST_VMD_DETAIL_SEQUENCE_PRED_CELEM_SZ \
            ) \
        (d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_PRED(d,state) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY \
                ( \
                HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state) \
                ), \
            HBOOST_VMD_IDENTITY(0), \
            HBOOST_VMD_DETAIL_SEQUENCE_PRED_CELEM \
            ) \
        (d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_EMPTY_TYPE_CHECK(output) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE(output,HBOOST_VMD_TYPE_ARRAY), \
        (0,()), \
        HBOOST_PP_NIL \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_EMPTY_TYPE_CHECK_D(d,output) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,output,HBOOST_VMD_TYPE_ARRAY), \
        (0,()), \
        HBOOST_PP_NIL \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_EMPTY_TYPE(output) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITOR \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE(output,HBOOST_VMD_TYPE_SEQ), \
            HBOOST_VMD_DETAIL_EQUAL_TYPE(output,HBOOST_VMD_TYPE_TUPLE) \
            ), \
        HBOOST_VMD_EMPTY, \
        HBOOST_VMD_DETAIL_SEQUENCE_EMPTY_TYPE_CHECK \
        ) \
    (output) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_EMPTY_TYPE_D(d,output) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITOR \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,output,HBOOST_VMD_TYPE_SEQ), \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,output,HBOOST_VMD_TYPE_TUPLE) \
            ), \
        HBOOST_VMD_EMPTY, \
        HBOOST_VMD_DETAIL_SEQUENCE_EMPTY_TYPE_CHECK_D \
        ) \
    (d,output) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE_GET(state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state) \
            ), \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state), \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state) \
            ), \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state) \
        ) 
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE_GET_D(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_FROM(state) \
            ), \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state), \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_INPUT(state) \
            ), \
            HBOOST_VMD_DETAIL_SEQUENCE_STATE_RESULT(state) \
        ) 
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE(vseq,elem,output,from) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE_GET \
        ( \
        HBOOST_PP_WHILE \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_PRED, \
            HBOOST_VMD_DETAIL_SEQUENCE_OP, \
                ( \
                vseq, \
                HBOOST_VMD_DETAIL_SEQUENCE_EMPTY_TYPE \
                    ( \
                    output \
                    ), \
                elem, \
                output, \
                from \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE_D(d,vseq,elem,output,from) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE_GET_D \
        ( \
        d, \
        HBOOST_PP_WHILE_ ## d \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_PRED, \
            HBOOST_VMD_DETAIL_SEQUENCE_OP, \
                ( \
                vseq, \
                HBOOST_VMD_DETAIL_SEQUENCE_EMPTY_TYPE_D \
                    ( \
                    d, \
                    output \
                    ), \
                elem, \
                output, \
                from \
                ) \
            ) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_SEQUENCE_COMMON_HPP */
