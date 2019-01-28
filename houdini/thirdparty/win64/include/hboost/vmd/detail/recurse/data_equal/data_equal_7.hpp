
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_DATA_EQUAL_7_HPP)
#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_HPP

#include <hboost/vmd/detail/recurse/data_equal/data_equal_headers.hpp>

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_CMP_PARENS(d,em1,em2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_DATA_EQUAL_IS_BOTH_COMPOSITE(em1,em2), \
            HBOOST_VMD_IDENTITY(2), \
            HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D \
            ) \
        (d,em1,em2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_CMP_PARENS_D(d,em1,em2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_DATA_EQUAL_IS_BOTH_COMPOSITE(em1,em2), \
            HBOOST_VMD_IDENTITY(2), \
            HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D \
            ) \
        (d,em1,em2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_CMP(d,state,em1,em2) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PROCESSING(d,state), \
        HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_CMP_PARENS \
        ) \
    (d,em1,em2) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_CMP_D(d,state,em1,em2) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_COMP_PROCESSING(d,state), \
        HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_CMP_PARENS_D \
        ) \
    (d,em1,em2) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ(d,state) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_CMP \
        ( \
        d, \
        state, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_FIRST_ELEMENT(d,state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_SECOND_ELEMENT(d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_D(d,state) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_CMP_D \
        ( \
        d, \
        state, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_FIRST_ELEMENT(d,state), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_STATE_GET_SECOND_ELEMENT(d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP(d,state) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_OP_RESULT \
        ( \
        d, \
        state, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ(d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_D(d,state) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_OP_RESULT \
        ( \
        d, \
        state, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_TEQ_D(d,state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_LOOP(dataf,datas,sz,vtype) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        0, \
        HBOOST_PP_WHILE \
            ( \
            HBOOST_VMD_DETAIL_DATA_EQUAL_PRED, \
            HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP, \
                ( \
                1, \
                dataf, \
                datas, \
                sz, \
                vtype, \
                0, \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_LOOP_D(d,dataf,datas,sz,vtype) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        0, \
        HBOOST_PP_WHILE_ ## d \
            ( \
            HBOOST_VMD_DETAIL_DATA_EQUAL_PRED, \
            HBOOST_VMD_DETAIL_DATA_EQUAL_7_OP_D, \
                ( \
                1, \
                dataf, \
                datas, \
                sz, \
                vtype, \
                0, \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_SZ(dataf,datas,szf,szs,vtype) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL(szf,szs), \
            HBOOST_VMD_DETAIL_DATA_EQUAL_7_LOOP, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (dataf,datas,szf,vtype) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_SZ_D(d,dataf,datas,szf,szs,vtype) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL_D(d,szf,szs), \
            HBOOST_VMD_DETAIL_DATA_EQUAL_7_LOOP_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,dataf,datas,szf,vtype) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7(dataf,datas,vtype) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_7_SZ \
        ( \
        dataf, \
        datas, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE(dataf,vtype), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE(datas,vtype), \
        vtype \
        ) \
/**/

#define HBOOST_VMD_DETAIL_DATA_EQUAL_7_D(d,dataf,datas,vtype) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_7_SZ_D \
        ( \
        d, \
        dataf, \
        datas, \
        HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_D(d,dataf,vtype), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_GET_SIZE_D(d,datas,vtype), \
        vtype \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_DATA_EQUAL_7_HPP */
