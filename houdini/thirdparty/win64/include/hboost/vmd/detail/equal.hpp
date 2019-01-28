
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_EQUAL_HPP)
#define HBOOST_VMD_DETAIL_EQUAL_HPP

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/detail/auto_rec.hpp>
#include <hboost/preprocessor/facilities/empty.hpp>
#include <hboost/preprocessor/facilities/overload.hpp>
#include <hboost/preprocessor/logical/compl.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_headers.hpp>
#include <hboost/vmd/is_identifier.hpp>
#include <hboost/vmd/is_empty_list.hpp>
#include <hboost/vmd/detail/not_empty.hpp>

#include <hboost/vmd/detail/recurse/equal/equal_1.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_2.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_3.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_4.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_5.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_6.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_7.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_8.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_9.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_10.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_11.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_12.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_13.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_14.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_15.hpp>
#include <hboost/vmd/detail/recurse/equal/equal_16.hpp>

#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_1(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_1_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_2(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_2_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_3(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_3_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_4(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_4_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_5(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_5_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_6(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_6_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_7(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_7_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_8(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_8_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_9(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_9_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_10(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_10_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_11(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_11_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_12(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_12_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_13(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_13_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_14(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_14_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_15(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_15_D(d,vseq1,vseq2)
#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_16(d,vseq1,vseq2) HBOOST_VMD_DETAIL_EQUAL_16_D(d,vseq1,vseq2)

#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_AUTO_REC_D(n) \
    HBOOST_VMD_DETAIL_EQUAL_IS_1 \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_ ## n(1,,) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D \
    HBOOST_PP_CAT \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_SIMPLE_D_, \
        HBOOST_PP_AUTO_REC(HBOOST_VMD_DETAIL_EQUAL_SIMPLE_AUTO_REC_D,16) \
        ) \
/**/


#define HBOOST_VMD_DETAIL_EQUAL_CNI_CHK(vseq1,vseq2,vtype) \
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
        HBOOST_VMD_DETAIL_DATA_EQUAL, \
        HBOOST_VMD_DETAIL_EQUAL_CNI_SMP \
        ) \
    (vseq1,vseq2,vtype) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_CNI_CHK_D(d,vseq1,vseq2,vtype) \
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
        HBOOST_VMD_DETAIL_DATA_EQUAL_D, \
        HBOOST_VMD_DETAIL_EQUAL_CNI_SMP_D \
        ) \
    (d,vseq1,vseq2,vtype) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_VSEQ(vseq1,vseq2,vtype) \
    HBOOST_VMD_DETAIL_DATA_EQUAL \
        ( \
        HBOOST_VMD_TO_SEQ(vseq1), \
        HBOOST_VMD_TO_SEQ(vseq2), \
        HBOOST_VMD_TYPE_SEQ \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_VSEQ_D(d,vseq1,vseq2,vtype) \
    HBOOST_VMD_DETAIL_DATA_EQUAL_D \
        ( \
        d, \
        HBOOST_VMD_TO_SEQ_D(d,vseq1), \
        HBOOST_VMD_TO_SEQ_D(d,vseq2), \
        HBOOST_VMD_TYPE_SEQ \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_CNI(vseq1,vseq2,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE \
            ( \
            vtype, \
            HBOOST_VMD_TYPE_SEQUENCE \
            ), \
        HBOOST_VMD_DETAIL_EQUAL_VSEQ, \
        HBOOST_VMD_DETAIL_EQUAL_CNI_CHK \
        ) \
    (vseq1,vseq2,vtype) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_CNI_D(d,vseq1,vseq2,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
            ( \
            d, \
            vtype, \
            HBOOST_VMD_TYPE_SEQUENCE \
            ), \
        HBOOST_VMD_DETAIL_EQUAL_VSEQ_D, \
        HBOOST_VMD_DETAIL_EQUAL_CNI_CHK_D \
        ) \
    (d,vseq1,vseq2,vtype) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_TUPLE(vseq1,vtype1,type) \
    HBOOST_PP_BITOR \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE \
            ( \
            HBOOST_VMD_TYPE_ARRAY, \
            vtype1 \
            ), \
        HBOOST_PP_BITAND \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE \
                ( \
                HBOOST_VMD_TYPE_LIST, \
                vtype1 \
                ), \
            HBOOST_PP_COMPL \
                ( \
                HBOOST_VMD_IS_EMPTY_LIST(vseq1) \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_TUPLE_D(d,vseq1,vtype1,type) \
    HBOOST_PP_BITOR \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
            ( \
            d, \
            HBOOST_VMD_TYPE_ARRAY, \
            vtype1 \
            ), \
        HBOOST_PP_BITAND \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
                ( \
                d, \
                HBOOST_VMD_TYPE_LIST, \
                vtype1 \
                ), \
            HBOOST_PP_COMPL \
                ( \
                HBOOST_VMD_IS_EMPTY_LIST_D(d,vseq1) \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_IDENTIFIER(vseq1,vtype1,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE \
                ( \
                HBOOST_VMD_TYPE_IDENTIFIER, \
                type \
                ), \
            HBOOST_VMD_IS_IDENTIFIER, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq1) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_IDENTIFIER_D(d,vseq1,vtype1,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
                ( \
                d, \
                HBOOST_VMD_TYPE_IDENTIFIER, \
                type \
                ), \
            HBOOST_VMD_IS_IDENTIFIER_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq1) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK(vseq1,vtype1,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE \
            ( \
            HBOOST_VMD_TYPE_TUPLE, \
            type \
            ), \
        HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_TUPLE, \
        HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_IDENTIFIER \
        ) \
    (vseq1,vtype1,type) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_D(d,vseq1,vtype1,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
            ( \
            d, \
            HBOOST_VMD_TYPE_TUPLE, \
            type \
            ), \
        HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_TUPLE_D, \
        HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_IDENTIFIER_D \
        ) \
    (d,vseq1,vtype1,type) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE(vseq1,vtype1,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE \
                ( \
                vtype1, \
                type \
                ), \
            HBOOST_VMD_IDENTITY(1), \
            HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK \
            ) \
        (vseq1,vtype1,type)    \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_D(d,vseq1,vtype1,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
                ( \
                d, \
                vtype1, \
                type \
                ), \
            HBOOST_VMD_IDENTITY(1), \
            HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_CHECK_D \
            ) \
        (d,vseq1,vtype1,type)    \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_WT_CHECK(vseq1,vseq2,vtype1,vtype2,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITAND \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE \
                    ( \
                    vtype1, \
                    vtype2 \
                    ), \
                HBOOST_VMD_IDENTITY_RESULT \
                    ( \
                    HBOOST_PP_IIF \
                        ( \
                        HBOOST_VMD_DETAIL_NOT_EMPTY(type), \
                        HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE, \
                        HBOOST_VMD_IDENTITY(1) \
                        ) \
                    (vseq1,vtype1,type) \
                    ) \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_CNI, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq1,vseq2,vtype1) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_WT_CHECK_D(d,vseq1,vseq2,vtype1,vtype2,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITAND \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
                    ( \
                    d, \
                    vtype1, \
                    vtype2 \
                    ), \
                HBOOST_VMD_IDENTITY_RESULT \
                    ( \
                    HBOOST_PP_IIF \
                        ( \
                        HBOOST_VMD_DETAIL_NOT_EMPTY(type), \
                        HBOOST_VMD_DETAIL_EQUAL_TEST_TYPE_D, \
                        HBOOST_VMD_IDENTITY(1) \
                        ) \
                    (d,vseq1,vtype1,type) \
                    ) \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_CNI_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq1,vseq2,vtype1) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_WT_CONVERT(vseq1,vseq2,vtype1,vtype2,type) \
    HBOOST_VMD_DETAIL_EQUAL_WT_CHECK \
        ( \
        vseq1, \
        vseq2, \
        HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE(vseq1,vtype1), \
        HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE(vseq2,vtype2), \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_WT_CONVERT_D(d,vseq1,vseq2,vtype1,vtype2,type) \
    HBOOST_VMD_DETAIL_EQUAL_WT_CHECK_D \
        ( \
        d, \
        vseq1, \
        vseq2, \
        HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_D(d,vseq1,vtype1), \
        HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_D(d,vseq2,vtype2), \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_WT(vseq1,vseq2,vtype1,vtype2,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH(vseq1,vseq2,vtype1,vtype2), \
            HBOOST_VMD_IDENTITY(0), \
            HBOOST_VMD_DETAIL_EQUAL_WT_CONVERT \
            ) \
        (vseq1,vseq2,vtype1,vtype2,type) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_WT_D(d,vseq1,vseq2,vtype1,vtype2,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH_D(d,vseq1,vseq2,vtype1,vtype2), \
            HBOOST_VMD_IDENTITY(0), \
            HBOOST_VMD_DETAIL_EQUAL_WT_CONVERT_D \
            ) \
        (d,vseq1,vseq2,vtype1,vtype2,type) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_GTYPE(vseq1,vseq2,type) \
    HBOOST_VMD_DETAIL_EQUAL_WT \
        ( \
        vseq1, \
        vseq2, \
        HBOOST_VMD_GET_TYPE(vseq1,HBOOST_VMD_RETURN_TYPE_TUPLE), \
        HBOOST_VMD_GET_TYPE(vseq2,HBOOST_VMD_RETURN_TYPE_TUPLE), \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_GTYPE_D(d,vseq1,vseq2,type) \
    HBOOST_VMD_DETAIL_EQUAL_WT_D \
        ( \
        d, \
        vseq1, \
        vseq2, \
        HBOOST_VMD_GET_TYPE_D(d,vseq1,HBOOST_VMD_RETURN_TYPE_TUPLE), \
        HBOOST_VMD_GET_TYPE_D(d,vseq2,HBOOST_VMD_RETURN_TYPE_TUPLE), \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_EBP(vseq1,vseq2,be1,be2,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITAND \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_CHK_MATCH(be1,be2), \
                HBOOST_VMD_DETAIL_EQUAL_CHK_PARENS_MATCH(vseq1,vseq2) \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_GTYPE, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq1,vseq2,type) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_EBP_D(d,vseq1,vseq2,be1,be2,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITAND \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_CHK_MATCH(be1,be2), \
                HBOOST_VMD_DETAIL_EQUAL_CHK_PARENS_MATCH(vseq1,vseq2) \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_GTYPE_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq1,vseq2,type) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_EMPTY(vseq1,vseq2,be1,be2,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITAND(be1,be2), \
        HBOOST_VMD_DETAIL_EQUAL_BOTH_EMPTY, \
        HBOOST_VMD_DETAIL_EQUAL_EBP \
        ) \
    (vseq1,vseq2,be1,be2,type) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_EMPTY_D(d,vseq1,vseq2,be1,be2,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITAND(be1,be2), \
        HBOOST_VMD_DETAIL_EQUAL_BOTH_EMPTY, \
        HBOOST_VMD_DETAIL_EQUAL_EBP_D \
        ) \
    (d,vseq1,vseq2,be1,be2,type) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_OV1(vseq1,vseq2) \
    HBOOST_VMD_DETAIL_EQUAL_EMPTY \
        ( \
        vseq1, \
        vseq2, \
        HBOOST_VMD_IS_EMPTY(vseq1), \
        HBOOST_VMD_IS_EMPTY(vseq2), \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_OV2(vseq1,vseq2,type) \
    HBOOST_VMD_DETAIL_EQUAL_EMPTY \
        ( \
        vseq1, \
        vseq2, \
        HBOOST_VMD_IS_EMPTY(vseq1), \
        HBOOST_VMD_IS_EMPTY(vseq2), \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_D_OV1(d,vseq1,vseq2) \
    HBOOST_VMD_DETAIL_EQUAL_EMPTY_D \
        ( \
        d, \
        vseq1, \
        vseq2, \
        HBOOST_VMD_IS_EMPTY(vseq1), \
        HBOOST_VMD_IS_EMPTY(vseq2), \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_D_OV2(d,vseq1,vseq2,type) \
    HBOOST_VMD_DETAIL_EQUAL_EMPTY_D \
        ( \
        d, \
        vseq1, \
        vseq2, \
        HBOOST_VMD_IS_EMPTY(vseq1), \
        HBOOST_VMD_IS_EMPTY(vseq2), \
        type \
        ) \
/**/

#if HBOOST_VMD_MSVC

#define HBOOST_VMD_DETAIL_EQUAL(vseq1,...) \
    HBOOST_PP_CAT(HBOOST_PP_OVERLOAD(HBOOST_VMD_DETAIL_EQUAL_OV,__VA_ARGS__)(vseq1,__VA_ARGS__),HBOOST_PP_EMPTY()) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_D(d,vseq1,...) \
    HBOOST_PP_CAT(HBOOST_PP_OVERLOAD(HBOOST_VMD_DETAIL_EQUAL_D_OV,__VA_ARGS__)(d,vseq1,__VA_ARGS__),HBOOST_PP_EMPTY()) \
/**/

#else

#define HBOOST_VMD_DETAIL_EQUAL(vseq1,...) \
    HBOOST_PP_OVERLOAD(HBOOST_VMD_DETAIL_EQUAL_OV,__VA_ARGS__)(vseq1,__VA_ARGS__) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_D(d,vseq1,...) \
    HBOOST_PP_OVERLOAD(HBOOST_VMD_DETAIL_EQUAL_D_OV,__VA_ARGS__)(d,vseq1,__VA_ARGS__) \
/**/

#endif

#endif /* HBOOST_VMD_DETAIL_EQUAL_HPP */
