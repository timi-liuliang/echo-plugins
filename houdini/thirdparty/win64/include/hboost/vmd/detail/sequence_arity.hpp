
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SEQUENCE_ARITY_HPP)
#define HBOOST_VMD_DETAIL_SEQUENCE_ARITY_HPP

#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/logical/bitnor.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/modifiers.hpp>
#include <hboost/vmd/detail/not_empty.hpp>
#include <hboost/vmd/detail/sequence_elem.hpp>

#define HBOOST_VMD_DETAIL_IS_MULTI_TUPLE(tuple) \
    HBOOST_PP_BITNOR \
        ( \
        HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(0,tuple)), \
        HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(1,tuple)) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_UNARY_TUPLE(tuple) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_NOT_EMPTY(HBOOST_PP_TUPLE_ELEM(0,tuple)), \
        HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(1,tuple)) \
        ) \
/**/

// UNARY

#define HBOOST_VMD_DETAIL_IS_UNARY(vseq) \
    HBOOST_VMD_DETAIL_IS_UNARY_TUPLE \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM(HBOOST_VMD_ALLOW_AFTER,0,vseq,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_UNARY_D(d,vseq) \
    HBOOST_VMD_DETAIL_IS_UNARY_TUPLE \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_D(d,HBOOST_VMD_ALLOW_AFTER,0,vseq,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

// MULTI

#define HBOOST_VMD_DETAIL_IS_MULTI(vseq) \
    HBOOST_VMD_DETAIL_IS_MULTI_TUPLE \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM(HBOOST_VMD_ALLOW_AFTER,0,vseq,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_MULTI_D(d,vseq) \
    HBOOST_VMD_DETAIL_IS_MULTI_TUPLE \
        ( \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_D(d,HBOOST_VMD_ALLOW_AFTER,0,vseq,HBOOST_VMD_RETURN_AFTER) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_SEQUENCE_ARITY_HPP */
