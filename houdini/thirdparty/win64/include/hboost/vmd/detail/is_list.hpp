
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_LIST_HPP)
#define HBOOST_VMD_DETAIL_IS_LIST_HPP

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/control/while.hpp>
#include <hboost/preprocessor/debug/assert.hpp>
#include <hboost/preprocessor/facilities/empty.hpp>
#include <hboost/preprocessor/logical/bitor.hpp>
#include <hboost/preprocessor/logical/compl.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/preprocessor/variadic/size.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/is_identifier.hpp>
#include <hboost/vmd/is_tuple.hpp>
#include <hboost/vmd/detail/nil_registration.hpp>

#define HBOOST_VMD_DETAIL_IS_LIST_PROCESS_TUPLE(d,x) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_VMD_IS_TUPLE(x), \
      HBOOST_VMD_DETAIL_IS_LIST_PROCESS_TUPLE_SIZE, \
      HBOOST_VMD_DETAIL_IS_LIST_ASSERT \
      ) \
    (d,x) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_PROCESS_TUPLE_SIZE(d,x) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_EQUAL_D(d,2,HBOOST_PP_TUPLE_SIZE(x)), \
      HBOOST_VMD_DETAIL_IS_LIST_RETURN_SECOND, \
      HBOOST_VMD_DETAIL_IS_LIST_ASSERT \
      ) \
    (x) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_PRED(d,state) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
          ( \
          HBOOST_PP_IS_BEGIN_PARENS(state), \
          HBOOST_VMD_IDENTITY(1), \
          HBOOST_VMD_DETAIL_IS_LIST_NOT_HBOOST_PP_NIL \
          ) \
        (state) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_OP(d,state) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(state), \
      HBOOST_VMD_DETAIL_IS_LIST_PROCESS_TUPLE, \
      HBOOST_VMD_DETAIL_IS_LIST_PROCESS_IF_HBOOST_PP_NIL \
      ) \
    (d,state) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_PROCESS_IF_HBOOST_PP_NIL(d,x) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_VMD_DETAIL_IS_LIST_HBOOST_PP_NIL(x), \
      HBOOST_PP_NIL, \
      HBOOST_VMD_IS_LIST_FAILURE \
      ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_ASSERT(...) \
    HBOOST_VMD_IS_LIST_FAILURE \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_NOT_HBOOST_PP_NIL(x) \
    HBOOST_PP_COMPL \
      ( \
      HBOOST_PP_BITOR \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST_HBOOST_PP_NIL(x), \
        HBOOST_VMD_DETAIL_IS_LIST_IS_FAILURE(x) \
        ) \
      ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_HBOOST_PP_NIL(x) \
    HBOOST_VMD_IS_EMPTY \
      ( \
      HBOOST_PP_CAT \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST_NIL_HELPER_, \
        x \
        ) HBOOST_PP_EMPTY() \
      ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_NIL_HELPER_HBOOST_PP_NIL

#define HBOOST_VMD_DETAIL_IS_LIST_IS_FAILURE(x) \
    HBOOST_VMD_IS_EMPTY \
      ( \
      HBOOST_PP_CAT(HBOOST_VMD_DETAIL_IS_LIST_FHELPER_,x) HBOOST_PP_EMPTY() \
      ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_FHELPER_HBOOST_VMD_IS_LIST_FAILURE

#define HBOOST_VMD_DETAIL_IS_LIST_RETURN_SECOND(x) \
    HBOOST_PP_TUPLE_ELEM(1,x) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_RESULT(x) \
    HBOOST_PP_COMPL \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST_IS_FAILURE(x) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_WLOOP(list) \
    HBOOST_VMD_DETAIL_IS_LIST_RESULT \
      ( \
      HBOOST_PP_WHILE \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST_PRED, \
        HBOOST_VMD_DETAIL_IS_LIST_OP, \
        list \
        ) \
      ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_WLOOP_D(d,list) \
    HBOOST_VMD_DETAIL_IS_LIST_RESULT \
      ( \
      HBOOST_PP_WHILE_ ## d \
        ( \
        HBOOST_VMD_DETAIL_IS_LIST_PRED, \
        HBOOST_VMD_DETAIL_IS_LIST_OP, \
        list \
        ) \
      ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_IS_EMPTY_LIST_PROCESS(list) \
    HBOOST_VMD_IS_IDENTIFIER(list,HBOOST_PP_NIL) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_IS_EMPTY_LIST_PROCESS_D(d,list) \
    HBOOST_VMD_IS_IDENTIFIER_D(d,list,HBOOST_PP_NIL) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_PROCESS(param) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(param), \
      HBOOST_VMD_DETAIL_IS_LIST_WLOOP, \
      HBOOST_VMD_DETAIL_IS_LIST_IS_EMPTY_LIST_PROCESS \
      ) \
    (param) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST(param) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
          ( \
          HBOOST_VMD_IS_EMPTY(param), \
          HBOOST_VMD_IDENTITY(0), \
          HBOOST_VMD_DETAIL_IS_LIST_PROCESS \
          ) \
        (param) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_PROCESS_D(d,param) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(param), \
      HBOOST_VMD_DETAIL_IS_LIST_WLOOP_D, \
      HBOOST_VMD_DETAIL_IS_LIST_IS_EMPTY_LIST_PROCESS_D \
      ) \
    (d,param) \
/**/

#define HBOOST_VMD_DETAIL_IS_LIST_D(d,param) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
          ( \
          HBOOST_VMD_IS_EMPTY(param), \
          HBOOST_VMD_IDENTITY(0), \
          HBOOST_VMD_DETAIL_IS_LIST_PROCESS_D \
          ) \
        (d,param) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_LIST_HPP */
