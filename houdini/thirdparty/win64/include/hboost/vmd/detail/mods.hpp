
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_MODS_HPP)
#define HBOOST_VMD_DETAIL_MODS_HPP

#include <hboost/preprocessor/arithmetic/inc.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/comparison/greater.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/control/while.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/pop_front.hpp>
#include <hboost/preprocessor/tuple/push_back.hpp>
#include <hboost/preprocessor/tuple/replace.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/preprocessor/variadic/to_tuple.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/is_type.hpp>
#include <hboost/vmd/detail/modifiers.hpp>

#define HBOOST_VMD_DETAIL_MODS_NO_RETURN 0
#define HBOOST_VMD_DETAIL_MODS_RETURN 1
#define HBOOST_VMD_DETAIL_MODS_RETURN_TUPLE 2
#define HBOOST_VMD_DETAIL_MODS_RETURN_ARRAY 3
#define HBOOST_VMD_DETAIL_MODS_RETURN_LIST 4
#define HBOOST_VMD_DETAIL_MODS_NO_AFTER 0
#define HBOOST_VMD_DETAIL_MODS_RETURN_AFTER 1
#define HBOOST_VMD_DETAIL_MODS_NO_INDEX 0
#define HBOOST_VMD_DETAIL_MODS_RETURN_INDEX 1
#define HBOOST_VMD_DETAIL_MODS_NO_ONLY_AFTER 0
#define HBOOST_VMD_DETAIL_MODS_RETURN_ONLY_AFTER 1

#define HBOOST_VMD_DETAIL_MODS_TUPLE_RETURN 0
#define HBOOST_VMD_DETAIL_MODS_TUPLE_AFTER 1
#define HBOOST_VMD_DETAIL_MODS_TUPLE_INDEX 2
#define HBOOST_VMD_DETAIL_MODS_TUPLE_OTHER 3
#define HBOOST_VMD_DETAIL_MODS_TUPLE_ONLY_AFTER 4
#define HBOOST_VMD_DETAIL_MODS_TUPLE_TYPE 5

#define HBOOST_VMD_DETAIL_MODS_DATA_INPUT 0
#define HBOOST_VMD_DETAIL_MODS_DATA_INDEX 1
#define HBOOST_VMD_DETAIL_MODS_DATA_SIZE 2
#define HBOOST_VMD_DETAIL_MODS_DATA_RESULT 3
#define HBOOST_VMD_DETAIL_MODS_DATA_ALLOW 4

#define HBOOST_VMD_DETAIL_MODS_STATE_INPUT(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_DATA_INPUT,state) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_INDEX(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_DATA_INDEX,state) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_SIZE(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_DATA_SIZE,state) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_DATA_RESULT,state) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_ALLOW(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_DATA_ALLOW,state) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_ALL(state) \
    HBOOST_VMD_DETAIL_IS_ALLOW_ALL(HBOOST_VMD_DETAIL_MODS_STATE_ALLOW(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_RETURN(state) \
    HBOOST_VMD_DETAIL_IS_ALLOW_RETURN(HBOOST_VMD_DETAIL_MODS_STATE_ALLOW(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_AFTER(state) \
    HBOOST_VMD_DETAIL_IS_ALLOW_AFTER(HBOOST_VMD_DETAIL_MODS_STATE_ALLOW(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_INDEX(state) \
    HBOOST_VMD_DETAIL_IS_ALLOW_INDEX(HBOOST_VMD_DETAIL_MODS_STATE_ALLOW(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_CURRENT(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_STATE_INDEX(state),HBOOST_VMD_DETAIL_MODS_STATE_INPUT(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_TYPE(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_RETURN,HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_AFTER(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_AFTER,HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_ONLY_AFTER(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_ONLY_AFTER,HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_STATE_TINDEX(state) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_INDEX,HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_RESULT_RETURN_TYPE(result) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_RETURN,result) \
/**/

#define HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(result) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_AFTER,result) \
/**/

#define HBOOST_VMD_DETAIL_MODS_IS_RESULT_ONLY_AFTER(result) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_ONLY_AFTER,result) \
/**/

#define HBOOST_VMD_DETAIL_MODS_IS_RESULT_INDEX(result) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_INDEX,result) \
/**/

#define HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(result) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_OTHER,result) \
/**/

#define HBOOST_VMD_DETAIL_MODS_RESULT_TYPE(result) \
    HBOOST_PP_TUPLE_ELEM(HBOOST_VMD_DETAIL_MODS_TUPLE_TYPE,result) \
/**/

#define HBOOST_VMD_DETAIL_MODS_PRED(d,state) \
    HBOOST_PP_GREATER_D(d,HBOOST_VMD_DETAIL_MODS_STATE_SIZE(state),HBOOST_VMD_DETAIL_MODS_STATE_INDEX(state))
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_RETURN_TYPE(d,state,number) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            HBOOST_VMD_DETAIL_MODS_DATA_RESULT, \
            HBOOST_PP_TUPLE_REPLACE_D \
                ( \
                d, \
                HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
                HBOOST_VMD_DETAIL_MODS_TUPLE_RETURN, \
                number \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ONLY_AFTER(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            HBOOST_VMD_DETAIL_MODS_DATA_RESULT, \
            HBOOST_PP_TUPLE_REPLACE_D \
                ( \
                d, \
                HBOOST_PP_TUPLE_REPLACE_D \
                    ( \
                    d, \
                    HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
                    HBOOST_VMD_DETAIL_MODS_TUPLE_ONLY_AFTER, \
                    1 \
                    ), \
                HBOOST_VMD_DETAIL_MODS_TUPLE_AFTER, \
                1 \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_INDEX(d,state,number) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            HBOOST_VMD_DETAIL_MODS_DATA_RESULT, \
            HBOOST_PP_TUPLE_REPLACE_D \
                ( \
                d, \
                HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
                HBOOST_VMD_DETAIL_MODS_TUPLE_INDEX, \
                number \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_GTT(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_RETURN_TYPE(d,state,HBOOST_VMD_DETAIL_MODS_RETURN_TUPLE) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ET(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_RETURN_TYPE(d,state,HBOOST_VMD_DETAIL_MODS_RETURN) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_SA(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_RETURN_TYPE(d,state,HBOOST_VMD_DETAIL_MODS_RETURN_ARRAY) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_SL(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_RETURN_TYPE(d,state,HBOOST_VMD_DETAIL_MODS_RETURN_LIST) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NT(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_RETURN_TYPE(d,state,HBOOST_VMD_DETAIL_MODS_NO_RETURN) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_AFT(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            HBOOST_VMD_DETAIL_MODS_DATA_RESULT, \
            HBOOST_PP_TUPLE_REPLACE_D \
                ( \
                d, \
                HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
                HBOOST_VMD_DETAIL_MODS_TUPLE_AFTER, \
                HBOOST_VMD_DETAIL_MODS_RETURN_AFTER \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NOAFT(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            HBOOST_VMD_DETAIL_MODS_DATA_RESULT, \
            HBOOST_PP_TUPLE_REPLACE_D \
                ( \
                d, \
                HBOOST_PP_TUPLE_REPLACE_D \
                    ( \
                    d, \
                    HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
                    HBOOST_VMD_DETAIL_MODS_TUPLE_ONLY_AFTER, \
                    0 \
                    ), \
                HBOOST_VMD_DETAIL_MODS_TUPLE_AFTER, \
                HBOOST_VMD_DETAIL_MODS_NO_AFTER \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_IND(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_INDEX(d,state,HBOOST_VMD_DETAIL_MODS_RETURN_INDEX) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NO_IND(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_INDEX(d,state,HBOOST_VMD_DETAIL_MODS_NO_INDEX) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_CTUPLE_REPLACE(d,state,id) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
        HBOOST_VMD_DETAIL_MODS_TUPLE_OTHER, \
        HBOOST_PP_VARIADIC_TO_TUPLE(id) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_CTUPLE_ADD(d,state,id) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
        HBOOST_VMD_DETAIL_MODS_TUPLE_OTHER, \
        HBOOST_PP_TUPLE_PUSH_BACK \
            ( \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state)), \
            id \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_CTUPLE(d,state,id) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state)) \
            ), \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_CTUPLE_REPLACE, \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_CTUPLE_ADD \
        ) \
    (d,state,id) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_TYPE_RETURN(d,state,id) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
        HBOOST_VMD_DETAIL_MODS_TUPLE_RETURN, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,id,HBOOST_VMD_TYPE_ARRAY), \
            HBOOST_VMD_DETAIL_MODS_RETURN_ARRAY, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,id,HBOOST_VMD_TYPE_LIST), \
                HBOOST_VMD_DETAIL_MODS_RETURN_LIST, \
                HBOOST_VMD_DETAIL_MODS_RETURN_TUPLE \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_TYPE(d,state,id) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_TYPE_RETURN(d,state,id), \
        HBOOST_VMD_DETAIL_MODS_TUPLE_TYPE, \
        id \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            HBOOST_VMD_DETAIL_MODS_DATA_RESULT, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_PP_BITAND \
                    ( \
                    HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_ALL(state), \
                    HBOOST_VMD_IS_TYPE_D(d,id) \
                    ), \
                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_TYPE, \
                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN_CTUPLE \
                ) \
            (d,state,id) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE(d,state) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        state, \
        HBOOST_VMD_DETAIL_MODS_DATA_INDEX, \
        HBOOST_PP_INC(HBOOST_VMD_DETAIL_MODS_STATE_INDEX(state)) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_ALL(d,state,id) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_RETURN_TYPE_TUPLE(id), \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_GTT, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_RETURN_TYPE(id), \
            HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ET, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_IS_RETURN_TYPE_ARRAY(id), \
                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_SA, \
                HBOOST_PP_IIF \
                    ( \
                    HBOOST_VMD_DETAIL_IS_RETURN_TYPE_LIST(id), \
                    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_SL, \
                    HBOOST_PP_IIF \
                        ( \
                        HBOOST_VMD_DETAIL_IS_RETURN_NO_TYPE(id), \
                        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NT, \
                        HBOOST_PP_IIF \
                            ( \
                            HBOOST_VMD_DETAIL_IS_RETURN_AFTER(id), \
                            HBOOST_VMD_DETAIL_MODS_OP_CURRENT_AFT, \
                            HBOOST_PP_IIF \
                                ( \
                                HBOOST_VMD_DETAIL_IS_RETURN_NO_AFTER(id), \
                                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NOAFT, \
                                HBOOST_PP_IIF \
                                    ( \
                                    HBOOST_VMD_DETAIL_IS_RETURN_ONLY_AFTER(id), \
                                    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ONLY_AFTER, \
                                    HBOOST_PP_IIF \
                                        ( \
                                        HBOOST_VMD_DETAIL_IS_RETURN_INDEX(id), \
                                        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_IND, \
                                        HBOOST_PP_IIF \
                                            ( \
                                            HBOOST_VMD_DETAIL_IS_RETURN_NO_INDEX(id), \
                                            HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NO_IND, \
                                            HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN \
                                            ) \
                                        ) \
                                    ) \
                                ) \
                            ) \
                        ) \
                    ) \
                ) \
            ) \
        ) \
    (d,state,id) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_RETURN(d,state,id) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_RETURN_TYPE_TUPLE(id), \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_GTT, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_RETURN_TYPE(id), \
            HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ET, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_IS_RETURN_TYPE_ARRAY(id), \
                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_SA, \
                HBOOST_PP_IIF \
                    ( \
                    HBOOST_VMD_DETAIL_IS_RETURN_TYPE_LIST(id), \
                    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_SL, \
                    HBOOST_PP_IIF \
                        ( \
                        HBOOST_VMD_DETAIL_IS_RETURN_NO_TYPE(id), \
                        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NT, \
                        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN \
                        ) \
                    ) \
                ) \
            ) \
        ) \
    (d,state,id) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_AFTER(d,state,id) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_RETURN_AFTER(id), \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_AFT, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_RETURN_NO_AFTER(id), \
            HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NOAFT, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_IS_RETURN_ONLY_AFTER(id), \
                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ONLY_AFTER, \
                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN \
                ) \
            ) \
        ) \
    (d,state,id) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_INDEX(d,state,id) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_RETURN_AFTER(id), \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_AFT, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_RETURN_NO_AFTER(id), \
            HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NOAFT, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_IS_RETURN_ONLY_AFTER(id), \
                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ONLY_AFTER, \
                HBOOST_PP_IIF \
                    ( \
                    HBOOST_VMD_DETAIL_IS_RETURN_INDEX(id), \
                    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_IND, \
                    HBOOST_PP_IIF \
                        ( \
                        HBOOST_VMD_DETAIL_IS_RETURN_NO_INDEX(id), \
                        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_NO_IND, \
                        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UNKNOWN \
                        ) \
                    ) \
                ) \
            ) \
        ) \
    (d,state,id) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_UPDATE(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE(d,state) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ID(d,state,id) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_ALL(state), \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_ALL, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_RETURN(state), \
            HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_RETURN, \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_AFTER(state), \
                HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_AFTER, \
                HBOOST_PP_IIF \
                    ( \
                    HBOOST_VMD_DETAIL_MODS_STATE_IS_ALLOW_INDEX(state), \
                    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_INDEX, \
                    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ALLOW_UPDATE \
                    ) \
                ) \
            ) \
        ) \
    (d,state,id) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT_TUPLE(d,state,id) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT_UPDATE \
        ( \
        d, \
        HBOOST_PP_TUPLE_REPLACE_D \
            ( \
            d, \
            state, \
            HBOOST_VMD_DETAIL_MODS_DATA_RESULT, \
            HBOOST_PP_TUPLE_REPLACE_D \
                ( \
                d, \
                HBOOST_VMD_DETAIL_MODS_STATE_RESULT(state), \
                HBOOST_VMD_DETAIL_MODS_TUPLE_OTHER, \
                id \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP_CURRENT(d,state,id) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_IS_BEGIN_PARENS(id), \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_TUPLE, \
        HBOOST_VMD_DETAIL_MODS_OP_CURRENT_ID \
        ) \
    (d,state,id) \
/**/

#define HBOOST_VMD_DETAIL_MODS_OP(d,state) \
    HBOOST_VMD_DETAIL_MODS_OP_CURRENT(d,state,HBOOST_VMD_DETAIL_MODS_STATE_CURRENT(state)) \
/**/

#define HBOOST_VMD_DETAIL_MODS_LOOP(allow,tuple) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        3, \
        HBOOST_PP_WHILE \
            ( \
            HBOOST_VMD_DETAIL_MODS_PRED, \
            HBOOST_VMD_DETAIL_MODS_OP, \
                ( \
                tuple, \
                0, \
                HBOOST_PP_TUPLE_SIZE(tuple), \
                (0,0,0,,0,), \
                allow \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MODS_LOOP_D(d,allow,tuple) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        3, \
        HBOOST_PP_WHILE_ ## d \
            ( \
            HBOOST_VMD_DETAIL_MODS_PRED, \
            HBOOST_VMD_DETAIL_MODS_OP, \
                ( \
                tuple, \
                0, \
                HBOOST_PP_TUPLE_SIZE(tuple), \
                (0,0,0,,0,), \
                allow \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_NEW_MODS_VAR(allow,tuple) \
    HBOOST_VMD_DETAIL_MODS_LOOP \
        ( \
        allow, \
        HBOOST_PP_TUPLE_POP_FRONT(tuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_NEW_MODS_VAR_D(d,allow,tuple) \
    HBOOST_VMD_DETAIL_MODS_LOOP_D \
        ( \
        d, \
        allow, \
        HBOOST_PP_TUPLE_POP_FRONT(tuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_NEW_MODS_IR(allow,tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL(HBOOST_PP_TUPLE_SIZE(tuple),1), \
            HBOOST_VMD_IDENTITY((0,0,0,,0,)), \
            HBOOST_VMD_DETAIL_NEW_MODS_VAR \
            ) \
        (allow,tuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_NEW_MODS_IR_D(d,allow,tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL_D(d,HBOOST_PP_TUPLE_SIZE(tuple),1), \
            HBOOST_VMD_IDENTITY((0,0,0,,0,)), \
            HBOOST_VMD_DETAIL_NEW_MODS_VAR_D \
            ) \
        (d,allow,tuple) \
        ) \
/**/

/*

  Returns a six-element tuple:
  
  First tuple element  = 0 No type return
                         1 Exact type return
                         2 General tuple type return
                         3 Array return
                         4 List return
                         
  Second tuple element = 0 No after return
                         1 After return
                         
  Third tuple element  = 0 No identifier index
                         1 Identifier Index
                         
  Fourth tuple element = Tuple of other identifiers
  
  Fifth tuple element  = 0 No after only return
                         1 After only return
                         
  Sixth tuple element  = Type identifier
                         
  Input                = allow, either
                         HBOOST_VMD_ALLOW_ALL
                         HBOOST_VMD_ALLOW_RETURN
                         HBOOST_VMD_ALLOW_AFTER
                         HBOOST_VMD_ALLOW_INDEX
                         
                           ..., modifiers, first variadic is discarded
                         Possible modifiers are:
                         
                         HBOOST_VMD_RETURN_NO_TYPE = (0,0)
                         HBOOST_VMD_RETURN_TYPE = (1,0)
                         HBOOST_VMD_RETURN_TYPE_TUPLE = (2,0)
                         HBOOST_VMD_RETURN_TYPE_ARRAY = (3,0)
                         HBOOST_VMD_RETURN_TYPE_LIST = (4,0)
                         
                         HBOOST_VMD_RETURN_NO_AFTER = (0,0)
                         HBOOST_VMD_RETURN_AFTER = (0,1)
  
*/

#define HBOOST_VMD_DETAIL_NEW_MODS(allow,...) \
    HBOOST_VMD_DETAIL_NEW_MODS_IR(allow,HBOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_NEW_MODS_D(d,allow,...) \
    HBOOST_VMD_DETAIL_NEW_MODS_IR_D(d,allow,HBOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__)) \
/**/

#endif /* HBOOST_VMD_DETAIL_MODS_HPP */
