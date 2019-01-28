
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IDENTIFIER_HPP)
#define HBOOST_VMD_DETAIL_IDENTIFIER_HPP

#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/expr_iif.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/logical/bitor.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/identifier_concat.hpp>
#include <hboost/vmd/detail/is_entire.hpp>
#include <hboost/vmd/detail/match_identifier.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/not_empty.hpp>
#include <hboost/vmd/detail/parens.hpp>

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_SUCCESS(id,rest,keymatch,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_SUCCESS_MODS(id,rest,HBOOST_PP_DEC(keymatch),mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_FAILURE(id,rest,keymatch,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_MODS(mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_TUPLE_RESULT(id,rest,keymatch,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL(keymatch,0), \
        HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_FAILURE, \
        HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_SUCCESS \
        ) \
    (id,rest,keymatch,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_TUPLE_RESULT_D(d,id,rest,keymatch,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL_D(d,keymatch,0), \
        HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_FAILURE, \
        HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_SUCCESS \
        ) \
    (id,rest,keymatch,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_TUPLE(id,rest,keytuple,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_TUPLE_RESULT \
        ( \
        id, \
        rest, \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER(id,keytuple), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_TUPLE_D(d,id,rest,keytuple,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_TUPLE_RESULT_D \
        ( \
        d, \
        id, \
        rest, \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_D(d,id,keytuple), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_MAKE_SPLIT(tuple) \
    ( \
    HBOOST_PP_TUPLE_ELEM \
        ( \
        0, \
        HBOOST_PP_TUPLE_ELEM(0,tuple) \
        ), \
    HBOOST_PP_TUPLE_ELEM(1,tuple) \
    ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SPLIT_SEQUENCE_CONCAT_DATA(tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY \
                ( \
                HBOOST_PP_TUPLE_ELEM(0,tuple) \
                ), \
            HBOOST_VMD_IDENTITY(tuple), \
            HBOOST_VMD_DETAIL_IDENTIFIER_MAKE_SPLIT \
            ) \
        (tuple) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SPLIT_SEQUENCE_CONCAT(vcseq) \
    HBOOST_VMD_DETAIL_IDENTIFIER_SPLIT_SEQUENCE_CONCAT_DATA \
        ( \
        HBOOST_VMD_DETAIL_PARENS(vcseq,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_GETID_TID(tid) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(tid), \
            HBOOST_VMD_IDENTITY(tid), \
            HBOOST_PP_TUPLE_ELEM \
            ) \
        (0,tid) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_GETID_SEQUENCE(vseq) \
    HBOOST_VMD_DETAIL_IDENTIFIER_GETID_TID \
        ( \
        HBOOST_VMD_DETAIL_PARENS(HBOOST_VMD_DETAIL_IDENTIFIER_CONCATENATE(vseq)) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SPLIT_SEQUENCE(vseq) \
     HBOOST_VMD_DETAIL_IDENTIFIER_SPLIT_SEQUENCE_CONCAT \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_CONCATENATE(vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS(id,rest,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_TUPLE \
        ( \
        id, \
        rest, \
        HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(mods), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_D(d,id,rest,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_TUPLE_D \
        ( \
        d, \
        id, \
        rest, \
        HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(mods), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_JUST(id,rest,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_SUCCESS_MODS(id,rest,0,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_JUST_D(d,id,rest,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_SUCCESS_MODS(id,rest,0,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_CHECK_KEYS(id,rest,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(mods) \
            ), \
        HBOOST_VMD_DETAIL_IDENTIFIER_JUST, \
        HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS \
        ) \
    (id,rest,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_CHECK_KEYS_D(d,id,rest,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY \
            ( \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(mods) \
            ), \
        HBOOST_VMD_DETAIL_IDENTIFIER_JUST_D, \
        HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_KEYS_D \
        ) \
    (d,id,rest,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_MKEYS(mods) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_INDEX(mods), \
        HBOOST_VMD_DETAIL_NOT_EMPTY \
            ( \
            HBOOST_VMD_DETAIL_MODS_RESULT_OTHER(mods) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SUCCESS_MODS(id,rest,keymatch,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_MKEYS(mods), \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(mods), \
            (id,rest,keymatch), \
            (id,keymatch) \
            ), \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(mods), \
            (id,rest), \
            id \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_MODS(mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_MKEYS(mods), \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(mods), \
            (,,), \
            (,) \
            ), \
        HBOOST_PP_EXPR_IIF \
            ( \
            HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(mods), \
            (,) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_REST(id,rest,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_MODS(mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_REST_D(d,id,rest,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_MODS(mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_ID_REST(id,rest,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(id), \
        HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_REST, \
        HBOOST_VMD_DETAIL_IDENTIFIER_CHECK_KEYS \
        ) \
    (id,rest,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_ID_REST_D(d,id,rest,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(id), \
        HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_REST_D, \
        HBOOST_VMD_DETAIL_IDENTIFIER_CHECK_KEYS_D \
        ) \
    (d,id,rest,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_DATA(tuple,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_ID_REST \
        ( \
        HBOOST_PP_TUPLE_ELEM(0,tuple), \
        HBOOST_PP_TUPLE_ELEM(1,tuple), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_ID(id,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(id), \
        HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_REST, \
        HBOOST_VMD_DETAIL_IDENTIFIER_CHECK_KEYS \
        ) \
    (id,,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_ID_D(d,id,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(id), \
        HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_REST_D, \
        HBOOST_VMD_DETAIL_IDENTIFIER_CHECK_KEYS_D \
        ) \
    (d,id,,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_DATA_D(d,tuple,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_ID_REST_D \
        ( \
        d, \
        HBOOST_PP_TUPLE_ELEM(0,tuple), \
        HBOOST_PP_TUPLE_ELEM(1,tuple), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_AFTER(vseq,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_DATA \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_SPLIT_SEQUENCE(vseq), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_AFTER_D(d,vseq,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_DATA_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_IDENTIFIER_SPLIT_SEQUENCE(vseq), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_ID(vseq,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_ID \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_GETID_SEQUENCE(vseq), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_ID_D(d,vseq,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_PROCESS_ID_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_IDENTIFIER_GETID_SEQUENCE(vseq), \
        mods \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE(vseq,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(mods), \
        HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_AFTER, \
        HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_ID \
        ) \
    (vseq,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_D(d,vseq,mods) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER(mods), \
        HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_AFTER_D, \
        HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_ID_D \
        ) \
    (d,vseq,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_EX_FAILURE(vseq,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_MODS(mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_EX_FAILURE_D(d,vseq,mods) \
    HBOOST_VMD_DETAIL_IDENTIFIER_FAILURE_MODS(mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_EX(vseq,mods) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_BITOR \
          ( \
          HBOOST_VMD_IS_EMPTY(vseq), \
          HBOOST_PP_IS_BEGIN_PARENS(vseq) \
          ), \
      HBOOST_VMD_DETAIL_IDENTIFIER_EX_FAILURE, \
      HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE \
      ) \
    (vseq,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_EX_D(d,vseq,mods) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_BITOR \
          ( \
          HBOOST_VMD_IS_EMPTY(vseq), \
          HBOOST_PP_IS_BEGIN_PARENS(vseq) \
          ), \
      HBOOST_VMD_DETAIL_IDENTIFIER_EX_FAILURE_D, \
      HBOOST_VMD_DETAIL_IDENTIFIER_SEQUENCE_D \
      ) \
    (d,vseq,mods) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER(...) \
    HBOOST_VMD_DETAIL_IDENTIFIER_EX \
        ( \
        HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__), \
        HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_INDEX,__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_D(d,...) \
    HBOOST_VMD_DETAIL_IDENTIFIER_EX_D \
        ( \
        d, \
        HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__), \
        HBOOST_VMD_DETAIL_NEW_MODS_D(d,HBOOST_VMD_ALLOW_INDEX,__VA_ARGS__) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_MULTIPLE(...) \
    HBOOST_VMD_DETAIL_IS_ENTIRE \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER(__VA_ARGS__,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_MULTIPLE_D(d,...) \
    HBOOST_VMD_DETAIL_IS_ENTIRE \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_D(d,__VA_ARGS__,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IDENTIFIER_HPP */
