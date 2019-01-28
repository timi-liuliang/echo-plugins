
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_EQUAL_COMMON_HPP)
#define HBOOST_VMD_DETAIL_EQUAL_COMMON_HPP

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/comparison/not_equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/logical/bitor.hpp>
#include <hboost/preprocessor/logical/bitxor.hpp>
#include <hboost/preprocessor/logical/compl.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/get_type.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/match_single_identifier.hpp>
#include <hboost/vmd/detail/equal_type.hpp>

#define HBOOST_VMD_DETAIL_EQUAL_CONCAT_1 (1)

#define HBOOST_VMD_DETAIL_EQUAL_IS_1(res) \
    HBOOST_PP_IS_BEGIN_PARENS \
        ( \
        HBOOST_PP_CAT \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_CONCAT_, \
            res \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_MATCH_SINGLE_IDENTIFIER(d,vseq1,vseq2) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(vseq1,vseq2) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_CNI_SMP(vseq1,vseq2,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_NUMBER), \
        HBOOST_PP_EQUAL, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype,HBOOST_VMD_TYPE_TYPE), \
            HBOOST_VMD_DETAIL_EQUAL_TYPE, \
            HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER \
            ) \
        ) \
    (vseq1,vseq2) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_CNI_SMP_D(d,vseq1,vseq2,vtype) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_NUMBER), \
        HBOOST_PP_EQUAL_D, \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype,HBOOST_VMD_TYPE_TYPE), \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D, \
            HBOOST_VMD_DETAIL_EQUAL_MATCH_SINGLE_IDENTIFIER \
            ) \
        ) \
    (d,vseq1,vseq2) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH_SIZE(vseq1,vseq2) \
    HBOOST_PP_NOT_EQUAL \
        ( \
        HBOOST_PP_TUPLE_SIZE(vseq1), \
        HBOOST_PP_TUPLE_SIZE(vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH_SIZE_D(d,vseq1,vseq2) \
    HBOOST_PP_NOT_EQUAL_D \
        ( \
        d, \
        HBOOST_PP_TUPLE_SIZE(vseq1), \
        HBOOST_PP_TUPLE_SIZE(vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH(vseq1,vseq2,vtype1,vtype2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITAND \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype1,HBOOST_VMD_TYPE_TUPLE), \
                HBOOST_VMD_DETAIL_EQUAL_TYPE(vtype2,HBOOST_VMD_TYPE_TUPLE) \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH_SIZE, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq1,vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH_D(d,vseq1,vseq2,vtype1,vtype2) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_BITAND \
                ( \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype1,HBOOST_VMD_TYPE_TUPLE), \
                HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,vtype2,HBOOST_VMD_TYPE_TUPLE) \
                ), \
            HBOOST_VMD_DETAIL_EQUAL_IS_TUPLE_MISMATCH_SIZE_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq1,vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_BOTH_EMPTY(...) 1

#define HBOOST_VMD_DETAIL_EQUAL_CHK_MATCH(bp1,bp2) \
    HBOOST_PP_COMPL \
        ( \
        HBOOST_PP_BITXOR \
            ( \
            bp1, \
            bp2 \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_CHK_PARENS_MATCH(vseq1,vseq2) \
    HBOOST_VMD_DETAIL_EQUAL_CHK_MATCH \
        ( \
        HBOOST_PP_IS_BEGIN_PARENS(vseq1), \
        HBOOST_PP_IS_BEGIN_PARENS(vseq2) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_CHK_EMPTY_MATCH(vseq1,vseq2) \
    HBOOST_VMD_DETAIL_EQUAL_CHK_MATCH \
        ( \
        HBOOST_VMD_IS_EMPTY(vseq1), \
        HBOOST_VMD_IS_EMPTY(vseq2) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_EQUAL_COMMON_HPP */
