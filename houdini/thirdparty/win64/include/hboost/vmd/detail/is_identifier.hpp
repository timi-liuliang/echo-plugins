
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_IDENTIFIER_HPP)
#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_HPP

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/logical/bitor.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/variadic/size.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/is_tuple.hpp>
#include <hboost/vmd/detail/identifier.hpp>
#include <hboost/vmd/detail/identifier_concat.hpp>
#include <hboost/vmd/detail/not_empty.hpp>

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ_MATCH(tup) \
    HBOOST_VMD_DETAIL_NOT_EMPTY \
        ( \
        HBOOST_PP_TUPLE_ELEM(0,tup) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ_TUPLE(ptup) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_TUPLE(ptup), \
            HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ_MATCH, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (ptup) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ_ID(vseq) \
    HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ_TUPLE \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_CONCATENATE(vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ(vseq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
          ( \
          HBOOST_PP_BITOR \
            ( \
            HBOOST_VMD_IS_EMPTY(vseq), \
            HBOOST_PP_IS_BEGIN_PARENS(vseq) \
            ), \
          HBOOST_VMD_IDENTITY(0), \
          HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ_ID \
          ) \
        (vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_SINGLE(...) \
    HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ(HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_SINGLE_D(d,...) \
    HBOOST_VMD_DETAIL_IS_IDENTIFIER_VSEQ(HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER(...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL(HBOOST_PP_VARIADIC_SIZE(__VA_ARGS__),1), \
        HBOOST_VMD_DETAIL_IS_IDENTIFIER_SINGLE, \
        HBOOST_VMD_DETAIL_IS_IDENTIFIER_MULTIPLE \
        ) \
    (__VA_ARGS__) \
/**/

#define HBOOST_VMD_DETAIL_IS_IDENTIFIER_D(d,...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL_D(d,HBOOST_PP_VARIADIC_SIZE(__VA_ARGS__),1), \
        HBOOST_VMD_DETAIL_IS_IDENTIFIER_SINGLE_D, \
        HBOOST_VMD_DETAIL_IS_IDENTIFIER_MULTIPLE_D \
        ) \
    (d,__VA_ARGS__) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_IDENTIFIER_HPP */
