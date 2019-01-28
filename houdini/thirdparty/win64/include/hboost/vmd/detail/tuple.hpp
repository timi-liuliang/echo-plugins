
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_TUPLE_HPP)
#define HBOOST_VMD_DETAIL_TUPLE_HPP

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/detail/empty_result.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/parens.hpp>
  
#define HBOOST_VMD_DETAIL_TUPLE_BST_CS(vseq) \
    HBOOST_PP_EQUAL(HBOOST_PP_TUPLE_SIZE(HBOOST_VMD_DETAIL_BEGIN_PARENS(vseq)),1) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_BST_CS_D(d,vseq) \
    HBOOST_PP_EQUAL_D(d,HBOOST_PP_TUPLE_SIZE(HBOOST_VMD_DETAIL_BEGIN_PARENS(vseq)),1) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_BST(vseq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_IS_BEGIN_PARENS(vseq), \
            HBOOST_VMD_DETAIL_TUPLE_BST_CS, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_BST_D(d,vseq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_IS_BEGIN_PARENS(vseq), \
            HBOOST_VMD_DETAIL_TUPLE_BST_CS_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_BSEQ_NXT(vseq) \
    HBOOST_VMD_DETAIL_TUPLE_BST(HBOOST_VMD_DETAIL_AFTER_PARENS_DATA(vseq)) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_BSEQ_NXT_D(d,vseq) \
    HBOOST_VMD_DETAIL_TUPLE_BST_D(d,HBOOST_VMD_DETAIL_AFTER_PARENS_DATA(vseq)) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_BSEQ(vseq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_TUPLE_BST(vseq), \
            HBOOST_VMD_DETAIL_TUPLE_BSEQ_NXT, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_BSEQ_D(d,vseq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_TUPLE_BST_D(d,vseq), \
            HBOOST_VMD_DETAIL_TUPLE_BSEQ_NXT_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_SPLIT_CHK_SEQ(vseq) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_TUPLE_BSEQ(vseq), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_SPLIT_PARENS \
        ) \
    (vseq) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_SPLIT_CHK_SEQ_D(d,vseq) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_TUPLE_BSEQ_D(d,vseq), \
        HBOOST_VMD_DETAIL_EMPTY_RESULT, \
        HBOOST_VMD_DETAIL_SPLIT_PARENS \
        ) \
    (vseq) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_SPLIT(vseq) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(vseq), \
      HBOOST_VMD_DETAIL_TUPLE_SPLIT_CHK_SEQ, \
      HBOOST_VMD_DETAIL_EMPTY_RESULT \
      ) \
    (vseq) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_SPLIT_D(d,vseq) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(vseq), \
      HBOOST_VMD_DETAIL_TUPLE_SPLIT_CHK_SEQ_D, \
      HBOOST_VMD_DETAIL_EMPTY_RESULT \
      ) \
    (d,vseq) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_PROCESS_CHK_SEQ(vseq) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_TUPLE_BSEQ(vseq), \
        HBOOST_VMD_EMPTY, \
        HBOOST_VMD_DETAIL_BEGIN_PARENS \
        ) \
    (vseq) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_PROCESS_CHK_SEQ_D(d,vseq) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_TUPLE_BSEQ_D(d,vseq), \
        HBOOST_VMD_EMPTY, \
        HBOOST_VMD_DETAIL_BEGIN_PARENS \
        ) \
    (vseq) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_PROCESS(vseq) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(vseq), \
      HBOOST_VMD_DETAIL_TUPLE_PROCESS_CHK_SEQ, \
      HBOOST_VMD_EMPTY \
      ) \
    (vseq) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_PROCESS_D(d,vseq) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(vseq), \
      HBOOST_VMD_DETAIL_TUPLE_PROCESS_CHK_SEQ_D, \
      HBOOST_VMD_EMPTY \
      ) \
    (d,vseq) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE(...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_TUPLE_SPLIT, \
        HBOOST_VMD_DETAIL_TUPLE_PROCESS \
        ) \
    (HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_TUPLE_D(d,...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_TUPLE_SPLIT_D, \
        HBOOST_VMD_DETAIL_TUPLE_PROCESS_D \
        ) \
    (d,HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#endif /* HBOOST_VMD_DETAIL_TUPLE_HPP */
