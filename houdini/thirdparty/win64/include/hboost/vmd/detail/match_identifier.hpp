
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_HPP)
#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_HPP

#include <hboost/preprocessor/arithmetic/inc.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/comparison/not_equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/control/while.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/replace.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/idprefix.hpp>
#include <hboost/vmd/detail/match_identifier_common.hpp>

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_ID(state) \
    HBOOST_PP_TUPLE_ELEM(0,state) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_KEY_TUPLE(state) \
    HBOOST_PP_TUPLE_ELEM(1,state) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_KEY_TUPLE_SIZE(state) \
    HBOOST_PP_TUPLE_ELEM(2,state) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_KEY_CURRENT(state) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_INDEX(state), \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_KEY_TUPLE(state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_INDEX(state) \
    HBOOST_PP_TUPLE_ELEM(3,state) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_RESULT(state) \
    HBOOST_PP_TUPLE_ELEM(4,state) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_PRED(d,state) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_PP_EQUAL_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_RESULT(state), \
            0 \
            ), \
        HBOOST_PP_NOT_EQUAL_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_INDEX(state), \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_KEY_TUPLE_SIZE(state) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP_FOUND(d,state) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        state, \
        4, \
        HBOOST_PP_INC \
            ( \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_INDEX(state) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP_CONTINUE(d,state) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        state, \
        3, \
        HBOOST_PP_INC \
            ( \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_INDEX(state) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP_CMP_IDS \
            ( \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_ID(state), \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_STATE_KEY_CURRENT(state) \
            ), \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP_FOUND, \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP_CONTINUE \
        ) \
    (d,state) \
/**/

/*

  Return index starting with 1 of the tuple id the identifier matches, or 0 if there are no matches.

*/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER(id,keytuple) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        4, \
        HBOOST_PP_WHILE \
            ( \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_PRED, \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP, \
                ( \
                id, \
                keytuple, \
                HBOOST_PP_TUPLE_SIZE(keytuple), \
                0, \
                0 \
                ) \
            ) \
        ) \
/**/

/*

  Return index starting with 1 of the tuple id the identifier matches, or 0 if there are no matches.

*/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_D(d,id,keytuple) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        4, \
        HBOOST_PP_WHILE_ ## d \
            ( \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_PRED, \
            HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP, \
                ( \
                id, \
                keytuple, \
                HBOOST_PP_TUPLE_SIZE(keytuple), \
                0, \
                0 \
                ) \
            ) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_HPP */
