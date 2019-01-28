
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HPP)
#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HPP

#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/comparison/greater.hpp>
#include <hboost/preprocessor/control/expr_if.hpp>
#include <hboost/preprocessor/control/expr_iif.hpp>
#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/logical/bitor.hpp>
#include <hboost/preprocessor/logical/compl.hpp>
#include <hboost/preprocessor/seq/elem.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/push_back.hpp>
#include <hboost/preprocessor/tuple/replace.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/is_identifier.hpp>
#include <hboost/vmd/detail/empty_result.hpp>
#include <hboost/vmd/detail/equal_type.hpp>
#include <hboost/vmd/detail/match_identifier.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/not_empty.hpp>
#include <hboost/vmd/detail/only_after.hpp>
#include <hboost/vmd/detail/sequence_common.hpp>

/*

    Given modifications and the requested type, 
    determine whether or not we should be checking for specific identifiers
    
    1 = check for specific identifiers
    0 = do no check for specific identifiers

*/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_CHECK_FOR_IDENTIFIERS(nm,type) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE \
            ( \
            type, \
            HBOOST_VMD_TYPE_IDENTIFIER \
            ), \
        HBOOST_VMD_DETAIL_NOT_EMPTY \
            ( \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(nm) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_CHECK_FOR_IDENTIFIERS_D(d,nm,type) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
            ( \
            d, \
            type, \
            HBOOST_VMD_TYPE_IDENTIFIER \
            ), \
        HBOOST_VMD_DETAIL_NOT_EMPTY \
            ( \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(nm) \
            ) \
        ) \
/**/

/*

    Given modifications, determine whether or not an index should be part of the result
    
    1 = index should be part of the result
    0 = index should not be part of the result

*/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_IS_INDEX_RESULT(nm) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_INDEX(nm), \
        HBOOST_PP_BITAND \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE \
                ( \
                HBOOST_VMD_DETAIL_MODS_RESULT_TYPE(nm), \
                HBOOST_VMD_TYPE_IDENTIFIER \
                ), \
            HBOOST_VMD_DETAIL_NOT_EMPTY \
                ( \
                HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(nm) \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_IS_INDEX_RESULT_D(d,nm) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_INDEX(nm), \
        HBOOST_PP_BITAND \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
                ( \
                d, \
                HBOOST_VMD_DETAIL_MODS_RESULT_TYPE(nm), \
                HBOOST_VMD_TYPE_IDENTIFIER \
                ), \
            HBOOST_VMD_DETAIL_NOT_EMPTY \
                ( \
                HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(nm) \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_INDEX(nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(nm), \
        (,,), \
        (,) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_NO_INDEX(nm) \
    HBOOST_PP_EXPR_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(nm), \
        (,) \
        ) \
/**/

/*

    Returns a failure result given the modifications

*/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT(nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_IS_INDEX_RESULT(nm), \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_INDEX, \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_NO_INDEX \
            ) \
        (nm),nm, \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_D(d,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_IS_INDEX_RESULT_D(d,nm), \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_INDEX, \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_NO_INDEX \
            ) \
        (nm),nm, \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE_TUPLE(res) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        0, \
        HBOOST_PP_TUPLE_ELEM(0,res) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE_ONLY(res) \
    HBOOST_PP_TUPLE_ELEM(0,res) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_TUPLE(res) \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        1, \
        HBOOST_PP_TUPLE_ELEM(0,res) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_ONLY(res) \
    HBOOST_PP_TUPLE_ELEM(1,res) \
/**/

/*

  Gets the 'data' of the result given the result and modifications

*/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA(res,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_TUPLE, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_ONLY \
        ) \
    (res) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_D(d,res,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER_D(d,nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_TUPLE, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_ONLY \
        ) \
    (res) \
/**/

/*

  Gets the 'type' of the result given the result and modifications

*/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE(res,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE_TUPLE, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE_ONLY \
        ) \
    (res) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE_D(d,res,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER_D(d,nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE_TUPLE, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE_ONLY \
        ) \
    (res) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HAS_FAILED_TUPLE(res) \
    HBOOST_VMD_IS_EMPTY \
        ( \
        HBOOST_PP_TUPLE_ELEM(0,res) \
        ) \
/**/

/*

    Determines whether the result from the element access has failed or not
    
    returns 1 if it has failed, otherwise 0.

*/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HAS_FAILED(res,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HAS_FAILED_TUPLE, \
        HBOOST_VMD_IS_EMPTY \
        ) \
    (res) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HAS_FAILED_D(d,res,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER_D(d,nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HAS_FAILED_TUPLE, \
        HBOOST_VMD_IS_EMPTY \
        ) \
    (res) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_ONLY_CHELM(seq,elem) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_GREATER(HBOOST_PP_SEQ_SIZE(seq),elem), \
        HBOOST_PP_SEQ_ELEM, \
        HBOOST_VMD_EMPTY \
        ) \
    (elem,seq) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_ONLY_CHELM_D(d,seq,elem) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_GREATER_D(d,HBOOST_PP_SEQ_SIZE(seq),elem), \
        HBOOST_PP_SEQ_ELEM, \
        HBOOST_VMD_EMPTY \
        ) \
    (elem,seq) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_CHELM_RES(data,elem) \
    ( \
    HBOOST_PP_SEQ_ELEM(elem,HBOOST_PP_TUPLE_ELEM(0,data)), \
    HBOOST_PP_TUPLE_ELEM(1,data) \
    ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_CHELM(data,elem) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_GREATER(HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(0,data)),elem), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_CHELM_RES, \
        HBOOST_VMD_DETAIL_EMPTY_RESULT \
        ) \
    (data,elem) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_CHELM_D(d,data,elem) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_GREATER_D(d,HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(0,data)),elem), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_CHELM_RES, \
        HBOOST_VMD_DETAIL_EMPTY_RESULT \
        ) \
    (data,elem) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_ONLY(seq,elem) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(seq), \
        HBOOST_VMD_EMPTY, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_ONLY_CHELM \
        ) \
    (seq,elem) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_ONLY_D(d,seq,elem) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(seq), \
        HBOOST_VMD_EMPTY, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_ONLY_CHELM_D \
        ) \
    (d,seq,elem) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER(data,elem) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(0,data)), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_CHELM \
        ) \
    (data,elem) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_D(d,data,elem) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(0,data)), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_CHELM_D \
        ) \
    (d,data,elem) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ(seq,elem,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_ONLY \
        ) \
    (seq,elem) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_D(d,seq,elem,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER_D(d,nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_AFTER_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_ONLY_D \
        ) \
    (d,seq,elem) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_VSEQ(...) \
    HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PROCESS(elem,vseq,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE \
            ( \
            vseq, \
            elem, \
            HBOOST_VMD_TYPE_SEQ, \
            nm \
            ), \
        elem, \
        nm \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PROCESS_D(d,elem,vseq,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FSEQ_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE_D \
            ( \
            d, \
            vseq, \
            elem, \
            HBOOST_VMD_TYPE_SEQ, \
            nm \
            ), \
        elem, \
        nm \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_EMPTY(elem,vseq,nm) \
    HBOOST_PP_EXPR_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER(nm), \
        (,) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_EMPTY_D(d,elem,vseq,nm) \
    HBOOST_PP_EXPR_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER_D(d,nm), \
        (,) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_CE(elem,vseq,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_NOT_EMPTY(vseq), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PROCESS, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_EMPTY \
        ) \
    (elem,vseq,nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_CE_D(d,elem,vseq,nm) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_NOT_EMPTY(vseq), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PROCESS_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_EMPTY_D \
        ) \
    (d,elem,vseq,nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN_D(d,res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN(res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN(res,nm,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_MODS_IS_RESULT_ONLY_AFTER(nm), \
            HBOOST_PP_TUPLE_ELEM, \
            HBOOST_VMD_IDENTITY(res) \
            ) \
        (1,res) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_INDEX_JRES(data,index) \
    HBOOST_PP_IF \
        ( \
        index, \
        (data,HBOOST_PP_DEC(index)), \
        (,) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_JRES(data,index) \
    HBOOST_PP_EXPR_IF \
        ( \
        index, \
        data \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_MATCH(data,nm,index) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_INDEX(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_INDEX_JRES, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_JRES \
        ) \
    (data,index) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_ID_TUP(data,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_MATCH \
        ( \
        data, \
        nm, \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER \
            ( \
            data, \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(nm) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_ID_TUP_D(d,data,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_MATCH \
        ( \
        data, \
        nm, \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_D \
            ( \
            d, \
            data, \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(nm) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_ID(data,nm,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_CHECK_FOR_IDENTIFIERS(nm,type), \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_ID_TUP,    \
            HBOOST_VMD_IDENTITY(data) \
            ) \
        (data,nm) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_ID_D(d,data,nm,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_CHECK_FOR_IDENTIFIERS_D(d,nm,type), \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_ID_TUP_D,    \
            HBOOST_VMD_IDENTITY(data) \
            ) \
        (d,data,nm) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY(res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_ID \
        ( \
        HBOOST_PP_TUPLE_ELEM(1,res), \
        nm, \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_D(d,res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_ID_D \
        ( \
        d, \
        HBOOST_PP_TUPLE_ELEM(1,res), \
        nm, \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_CHANGE(res,nm,type) \
    HBOOST_PP_TUPLE_REPLACE \
        ( \
        res, \
        0, \
        HBOOST_PP_TUPLE_ELEM \
            ( \
            1, \
            HBOOST_PP_TUPLE_ELEM(0,res) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_CHANGE_D(d,res,nm,type) \
    HBOOST_PP_TUPLE_REPLACE_D \
        ( \
        d, \
        res, \
        0, \
        HBOOST_PP_TUPLE_ELEM \
            ( \
            1, \
            HBOOST_PP_TUPLE_ELEM(0,res) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ONLY_CAFTER(res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_ONLY_AFTER(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_CHANGE \
        ) \
    (res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ONLY_CAFTER_D(d,res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_ONLY_AFTER(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_CHANGE_D \
        ) \
    (d,res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_SUCCESS(res,nm,type,index) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ONLY_CAFTER \
        ( \
        HBOOST_PP_TUPLE_PUSH_BACK(res,HBOOST_PP_DEC(index)), \
        nm, \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_SUCCESS_D(d,res,nm,type,index) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ONLY_CAFTER_D \
        ( \
        d, \
        HBOOST_PP_TUPLE_PUSH_BACK(res,HBOOST_PP_DEC(index)), \
        nm, \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_FAILURE(res,nm,type,index) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT(nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_FAILURE_D(d,res,nm,type,index) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_D(d,nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_INDEX_JRES(res,nm,type,index) \
    HBOOST_PP_IF \
        ( \
        index, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_SUCCESS, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_FAILURE \
        ) \
    (res,nm,type,index)    \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_INDEX_JRES_D(d,res,nm,type,index) \
    HBOOST_PP_IF \
        ( \
        index, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_SUCCESS_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_FAILURE_D \
        ) \
    (d,res,nm,type,index)    \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_JRES_FAILURE(res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT(nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_JRES_FAILURE_D(d,res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_D(d,nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_JRES(res,nm,type,index) \
    HBOOST_PP_IF \
        ( \
        index, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ONLY_CAFTER, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_JRES_FAILURE \
        ) \
    (res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_JRES_D(d,res,nm,type,index) \
    HBOOST_PP_IF \
        ( \
        index, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ONLY_CAFTER_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_JRES_FAILURE_D \
        ) \
    (d,res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ID_RES(res,nm,type,index) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_INDEX(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_INDEX_JRES, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_JRES \
        ) \
    (res,nm,type,index)    \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ID_RES_D(d,res,nm,type,index) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_INDEX(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_INDEX_JRES_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_JRES_D \
        ) \
    (d,res,nm,type,index)    \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ID(res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ID_RES \
        ( \
        res, \
        nm, \
        type, \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA(res,nm), \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(nm) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ID_D(d,res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ID_RES_D \
        ( \
        d, \
        res, \
        nm, \
        type, \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_D(d,res,nm), \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(nm) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT(res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_CHECK_FOR_IDENTIFIERS(nm,type), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ID, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ONLY_CAFTER \
        ) \
    (res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_D(d,res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_CHECK_FOR_IDENTIFIERS_D(d,nm,type), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ID_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_ONLY_CAFTER_D \
        ) \
    (d,res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK(res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER(nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY \
        ) \
    (res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_D(d,res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_STATE_IS_AFTER_D(d,nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_SPLIT_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_ONLY_D \
        ) \
    (d,res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_FAILED(res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT(nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_FAILED_D(d,res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_D(d,nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_ID(res,nm,type) \
    HBOOST_VMD_IS_IDENTIFIER \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA(res,nm) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_ID_D(d,res,nm,type) \
    HBOOST_VMD_IS_IDENTIFIER_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_DATA_D(d,res,nm) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_OTHER(res,nm,type) \
    HBOOST_VMD_DETAIL_EQUAL_TYPE \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE(res,nm), \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_OTHER_D(d,res,nm,type) \
    HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_RESULT_TYPE_D(d,res,nm), \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE(res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE \
            ( \
            HBOOST_VMD_TYPE_IDENTIFIER, \
            type \
            ), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_ID, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_OTHER \
        ) \
    (res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_D(d,res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_EQUAL_TYPE_D \
            ( \
            d, \
            HBOOST_VMD_TYPE_IDENTIFIER, \
            type \
            ), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_ID_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_OTHER_D \
        ) \
    (d,res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT(res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE(res,nm,type), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_FAILED \
        ) \
    (res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_D(d,res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_MATCHING_TYPE_D(d,res,nm,type), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_CHECK_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_FAILED_D \
        ) \
    (d,res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_NF(res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(type), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT \
        ) \
    (res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_NF_D(d,res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(type), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_FIN_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_PT_D \
        ) \
    (d,res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_FAILED(res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT(nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_FAILED_D(d,res,nm,type) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_FAILURE_RESULT_D(d,nm) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE(res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HAS_FAILED(res,nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_FAILED, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_NF \
        ) \
    (res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_D(d,res,nm,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HAS_FAILED_D(d,res,nm), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_FAILED_D, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_NF_D \
        ) \
    (d,res,nm,type) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_COA(res,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE \
        ( \
        res, \
        nm, \
        HBOOST_VMD_DETAIL_MODS_RESULT_TYPE(nm) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_COA_D(d,res,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_TYPE_D \
        ( \
        d, \
        res, \
        nm, \
        HBOOST_VMD_DETAIL_MODS_RESULT_TYPE(nm) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM(elem,vseq,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_COA \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_CE(elem,vseq,nm), \
        nm \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_D(d,elem,vseq,nm) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_COA_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_CE_D(d,elem,vseq,nm), \
        nm \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM(allow,elem,...) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM \
        ( \
        elem, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_VSEQ(__VA_ARGS__), \
        HBOOST_VMD_DETAIL_NEW_MODS(allow,__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_ELEM_D(d,allow,elem,...) \
    HBOOST_VMD_DETAIL_SEQUENCE_ELEM_NM_D \
        ( \
        d, \
        elem, \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_VSEQ(__VA_ARGS__), \
        HBOOST_VMD_DETAIL_NEW_MODS_D(d,allow,__VA_ARGS__) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_SEQUENCE_ELEM_HPP */
