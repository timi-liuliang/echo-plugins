
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_EQUAL_14_HPP)
#define HBOOST_VMD_DETAIL_EQUAL_14_HPP

#include <hboost/vmd/detail/recurse/equal/equal_headers.hpp>

#define HBOOST_VMD_DETAIL_EQUAL_14_CNI_CHK(vseq1,vseq2,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITOR \
            ( \
            HBOOST_PP_BITOR \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_ARRAY), \
                HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_LIST) \
                ), \
            HBOOST_PP_BITOR \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_SEQ), \
                HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_TUPLE) \
                ) \
            ), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_14, \
        HBOOST_VMD_DETAIL_EQUAL_CNI_SMP \
        ) \
    (vseq1,vseq2,vtype) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_CNI_CHK_D(d,vseq1,vseq2,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITOR \
            ( \
            HBOOST_PP_BITOR \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_ARRAY), \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_LIST) \
                ), \
            HBOOST_PP_BITOR \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_SEQ), \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_TUPLE) \
                ) \
            ), \
        HBOOST_VMD_DETAIL_DATA_EQUAL_14_D, \
        HBOOST_VMD_DETAIL_EQUAL_CNI_SMP_D \
        ) \
    (d,vseq1,vseq2,vtype) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_VSEQ(vseq1,vseq2,vtype) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_14 \
        ( \
        HBOOST_VMD_TO_SEQ(vseq1), \
        HBOOST_VMD_TO_SEQ(vseq2), \
        HBOOST_VMD_TYPE_SEQ \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_VSEQ_D(d,vseq1,vseq2,vtype) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_14_D \
        ( \
        d, \
        HBOOST_VMD_TO_SEQ_D(d,vseq1), \
        HBOOST_VMD_TO_SEQ_D(d,vseq2), \
        HBOOST_VMD_TYPE_SEQ \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_CNI(vseq1,vseq2,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE \
            ( \
            vtype, \
            HBOOST_VMD_TYPE_SEQUENCE \
            ), \
        HBOOST_VMD_DETAIL_EQUAL_14_VSEQ, \
        HBOOST_VMD_DETAIL_EQUAL_14_CNI_CHK \
        ) \
    (vseq1,vseq2,vtype) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_CNI_D(d,vseq1,vseq2,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
            ( \
            d, \
            vtype, \
            HBOOST_VMD_TYPE_SEQUENCE \
            ), \
        HBOOST_VMD_DETAIL_EQUAL_14_VSEQ_D, \
        HBOOST_VMD_DETAIL_EQUAL_14_CNI_CHK_D \
        ) \
    (d,vseq1,vseq2,vtype) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_WT_CHECK(vseq1,vseq2,vtype1,vtype2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE \
                ( \
                vtype1, \
                vtype2 \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_14_CNI, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq1,vseq2,vtype1) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_WT_CHECK_D(d,vseq1,vseq2,vtype1,vtype2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
                ( \
                d, \
                vtype1, \
                vtype2 \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_14_CNI_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq1,vseq2,vtype1) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_WT_CONVERT(vseq1,vseq2,vtype1,vtype2) \
    HBOOST_VMD_DETAIL_EQUAL_14_WT_CHECK \
        ( \
        vseq1, \
        vseq2, \
        HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE(vseq1,vtype1), \
        HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE(vseq2,vtype2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_WT_CONVERT_D(d,vseq1,vseq2,vtype1,vtype2) \
    HBOOST_VMD_DETAIL_EQUAL_14_WT_CHECK_D \
        ( \
        d, \
        vseq1, \
        vseq2, \
        HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_D(d,vseq1,vtype1), \
        HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_D(d,vseq2,vtype2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_WT(vseq1,vseq2,vtype1,vtype2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH(vseq1,vseq2,vtype1,vtype2), \
            HBOOST_VMD_IDENTITY(0), \
            HBOOST_VMD_DETAIL_EQUAL_14_WT_CONVERT \
            ) \
        (vseq1,vseq2,vtype1,vtype2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_WT_D(d,vseq1,vseq2,vtype1,vtype2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH_D(d,vseq1,vseq2,vtype1,vtype2), \
            HBOOST_VMD_IDENTITY(0), \
            HBOOST_VMD_DETAIL_EQUAL_14_WT_CONVERT_D \
            ) \
        (d,vseq1,vseq2,vtype1,vtype2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_GTYPE(vseq1,vseq2) \
    HBOOST_VMD_DETAIL_EQUAL_14_WT \
        ( \
        vseq1, \
        vseq2, \
        HBOOST_VMD_GET_TYPE(vseq1,HBOOST_VMD_RETURN_TYPE_TUPLE), \
        HBOOST_VMD_GET_TYPE(vseq2,HBOOST_VMD_RETURN_TYPE_TUPLE) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_GTYPE_D(d,vseq1,vseq2) \
    HBOOST_VMD_DETAIL_EQUAL_14_WT_D \
        ( \
        d, \
        vseq1, \
        vseq2, \
        HBOOST_VMD_GET_TYPE_D(d,vseq1,HBOOST_VMD_RETURN_TYPE_TUPLE), \
        HBOOST_VMD_GET_TYPE_D(d,vseq2,HBOOST_VMD_RETURN_TYPE_TUPLE) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_EBP(vseq1,vseq2,be1,be2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITAND \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_CHK_MATCH(be1,be2), \
                HBOOST_VMD_DETAIL_EQUAL_CHK_PARENS_MATCH(vseq1,vseq2) \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_14_GTYPE, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq1,vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_EBP_D(d,vseq1,vseq2,be1,be2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITAND \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_CHK_MATCH(be1,be2), \
                HBOOST_VMD_DETAIL_EQUAL_CHK_PARENS_MATCH(vseq1,vseq2) \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_14_GTYPE_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq1,vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_EMPTY(vseq1,vseq2,be1,be2) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITAND(be1,be2), \
        HBOOST_VMD_DETAIL_EQUAL_BOTH_EMPTY, \
        HBOOST_VMD_DETAIL_EQUAL_14_EBP \
        ) \
    (vseq1,vseq2,be1,be2) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_EMPTY_D(d,vseq1,vseq2,be1,be2) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITAND(be1,be2), \
        HBOOST_VMD_DETAIL_EQUAL_BOTH_EMPTY, \
        HBOOST_VMD_DETAIL_EQUAL_14_EBP_D \
        ) \
    (d,vseq1,vseq2,be1,be2) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14(vseq1,vseq2) \
    HBOOST_VMD_DETAIL_EQUAL_14_EMPTY \
        ( \
        vseq1, \
        vseq2, \
        HBOOST_VMD_IS_EMPTY(vseq1), \
        HBOOST_VMD_IS_EMPTY(vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_14_D(d,vseq1,vseq2) \
    HBOOST_VMD_DETAIL_EQUAL_14_EMPTY_D \
        ( \
        d, \
        vseq1, \
        vseq2, \
        HBOOST_VMD_IS_EMPTY(vseq1), \
        HBOOST_VMD_IS_EMPTY(vseq2) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_EQUAL_14_HPP */
