
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SEQ_HPP)
#define HBOOST_VMD_DETAIL_SEQ_HPP

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/control/while.hpp>
#include <hboost/preprocessor/facilities/empty.hpp>
#include <hboost/preprocessor/seq/push_back.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/push_back.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/empty_result.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/not_empty.hpp>
#include <hboost/vmd/detail/parens.hpp>

#define HBOOST_VMD_DETAIL_SEQ_STATE_INIT(seq) \
    HBOOST_PP_TUPLE_PUSH_BACK \
        ( \
        HBOOST_PP_TUPLE_PUSH_BACK \
            ( \
            HBOOST_VMD_DETAIL_PARENS(seq,HBOOST_VMD_RETURN_AFTER), \
            HBOOST_PP_EMPTY() \
            ), \
        HBOOST_PP_EMPTY() \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_INIT_D(d,seq) \
    HBOOST_PP_TUPLE_PUSH_BACK \
        ( \
        HBOOST_PP_TUPLE_PUSH_BACK \
            ( \
            HBOOST_VMD_DETAIL_PARENS_D(d,seq,HBOOST_VMD_RETURN_AFTER), \
            HBOOST_PP_EMPTY() \
            ), \
        HBOOST_PP_EMPTY() \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_PRED(d,state) \
    HBOOST_VMD_DETAIL_NOT_EMPTY \
        ( \
        HBOOST_PP_TUPLE_ELEM(0,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_OP_SET_FAILURE(d,state) \
    ( \
    HBOOST_PP_EMPTY(), \
    HBOOST_PP_EMPTY(), \
    HBOOST_PP_TUPLE_ELEM(2,state), \
    HBOOST_PP_TUPLE_ELEM(3,state) \
    ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_OP_MAKE_SEQ(d,state) \
    HBOOST_PP_TUPLE_ELEM(0,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_OP_UPGRADE_SEQ(d,state) \
    HBOOST_PP_SEQ_PUSH_BACK \
        ( \
        HBOOST_PP_TUPLE_ELEM(2,state), \
        HBOOST_PP_TUPLE_ELEM \
            ( \
            0, \
            HBOOST_PP_TUPLE_ELEM(0,state) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_OP_SET_SUCCESS(d,state) \
    ( \
    HBOOST_PP_EMPTY(), \
    HBOOST_PP_EMPTY(), \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
              HBOOST_PP_TUPLE_ELEM(2,state) \
            ), \
        HBOOST_VMD_DETAIL_SEQ_STATE_OP_MAKE_SEQ, \
        HBOOST_VMD_DETAIL_SEQ_STATE_OP_UPGRADE_SEQ \
        ) \
    (d,state), \
    HBOOST_PP_EMPTY() \
    ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_OP_SET_CONTINUE(d,state) \
    HBOOST_PP_TUPLE_PUSH_BACK \
        ( \
        HBOOST_PP_TUPLE_PUSH_BACK \
            ( \
            HBOOST_VMD_DETAIL_PARENS_D(d,HBOOST_PP_TUPLE_ELEM(1,state),HBOOST_VMD_RETURN_AFTER), \
            HBOOST_PP_IIF \
                ( \
                HBOOST_VMD_IS_EMPTY \
                    ( \
                    HBOOST_PP_TUPLE_ELEM(2,state) \
                    ), \
                HBOOST_VMD_DETAIL_SEQ_STATE_OP_MAKE_SEQ, \
                HBOOST_VMD_DETAIL_SEQ_STATE_OP_UPGRADE_SEQ \
                ) \
            (d,state) \
            ), \
        HBOOST_PP_TUPLE_ELEM(1,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_OP_CHECK_REST(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
              HBOOST_PP_TUPLE_ELEM(1,state) \
            ), \
        HBOOST_VMD_DETAIL_SEQ_STATE_OP_SET_SUCCESS, \
        HBOOST_VMD_DETAIL_SEQ_STATE_OP_SET_CONTINUE \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_OP(d,state) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL_D \
            ( \
            d, \
            1, \
            HBOOST_PP_TUPLE_SIZE \
                ( \
                  HBOOST_PP_TUPLE_ELEM(0,state) \
                ) \
            ), \
        HBOOST_VMD_DETAIL_SEQ_STATE_OP_CHECK_REST, \
        HBOOST_VMD_DETAIL_SEQ_STATE_OP_SET_FAILURE \
        ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_STATE_RESULT(state) \
    ( \
    HBOOST_PP_TUPLE_ELEM(2,state), \
    HBOOST_PP_TUPLE_ELEM(3,state) \
    ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_PROCESS(seq) \
    HBOOST_VMD_DETAIL_SEQ_STATE_RESULT \
      ( \
      HBOOST_PP_WHILE \
        ( \
        HBOOST_VMD_DETAIL_SEQ_STATE_PRED, \
        HBOOST_VMD_DETAIL_SEQ_STATE_OP, \
        HBOOST_VMD_DETAIL_SEQ_STATE_INIT(seq) \
        ) \
      ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_SPLIT(seq) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(seq), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_SEQ_PROCESS \
        ) \
    (seq) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_BEGIN(seq) \
    HBOOST_PP_TUPLE_ELEM(0,HBOOST_VMD_DETAIL_SEQ_SPLIT(seq)) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_PROCESS_D(d,seq) \
    HBOOST_VMD_DETAIL_SEQ_STATE_RESULT \
      ( \
      HBOOST_PP_WHILE_ ## d \
        ( \
        HBOOST_VMD_DETAIL_SEQ_STATE_PRED, \
        HBOOST_VMD_DETAIL_SEQ_STATE_OP, \
        HBOOST_VMD_DETAIL_SEQ_STATE_INIT_D(d,seq) \
        ) \
      ) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_SPLIT_D(d,seq) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(seq), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_SEQ_PROCESS_D \
        ) \
    (d,seq) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_BEGIN_D(d,seq) \
    HBOOST_PP_TUPLE_ELEM(0,HBOOST_VMD_DETAIL_SEQ_SPLIT_D(d,seq)) \
/**/

#define HBOOST_VMD_DETAIL_SEQ_D(d,...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS_D(d,HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_SEQ_SPLIT_D, \
        HBOOST_VMD_DETAIL_SEQ_BEGIN_D \
        ) \
    (d,HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_SEQ(...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_SEQ_SPLIT, \
        HBOOST_VMD_DETAIL_SEQ_BEGIN \
        ) \
    (HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#endif /* HBOOST_VMD_DETAIL_SEQ_HPP */
