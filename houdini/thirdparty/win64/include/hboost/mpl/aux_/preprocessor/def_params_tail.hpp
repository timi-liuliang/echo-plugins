
#ifndef HBOOST_MPL_AUX_PREPROCESSOR_DEF_PARAMS_TAIL_HPP_INCLUDED
#define HBOOST_MPL_AUX_PREPROCESSOR_DEF_PARAMS_TAIL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/limits/arity.hpp>
#include <hboost/mpl/aux_/config/dtp.hpp>
#include <hboost/mpl/aux_/config/preprocessor.hpp>

#include <hboost/preprocessor/comma_if.hpp>
#include <hboost/preprocessor/logical/and.hpp>
#include <hboost/preprocessor/identity.hpp>
#include <hboost/preprocessor/empty.hpp>

// HBOOST_MPL_PP_DEF_PARAMS_TAIL(1,T,value): , T1 = value, .., Tn = value
// HBOOST_MPL_PP_DEF_PARAMS_TAIL(2,T,value): , T2 = value, .., Tn = value
// HBOOST_MPL_PP_DEF_PARAMS_TAIL(n,T,value): <nothing>

#if !defined(HBOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES)

#   include <hboost/mpl/aux_/preprocessor/filter_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/sub.hpp>

#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_IMPL(i, param, value_func) \
    HBOOST_MPL_PP_DEF_PARAMS_TAIL_DELAY_1( \
          i \
        , HBOOST_MPL_PP_SUB(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY,i) \
        , param \
        , value_func \
        ) \
    /**/

#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_DELAY_1(i, n, param, value_func) \
    HBOOST_MPL_PP_DEF_PARAMS_TAIL_DELAY_2(i,n,param,value_func) \
    /**/

#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_DELAY_2(i, n, param, value_func) \
    HBOOST_PP_COMMA_IF(HBOOST_PP_AND(i,n)) \
    HBOOST_MPL_PP_DEF_PARAMS_TAIL_##i(n,param,value_func) \
    /**/

#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_0(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##1 v(),p##2 v(),p##3 v(),p##4 v(),p##5 v(),p##6 v(),p##7 v(),p##8 v(),p##9 v())
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_1(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##2 v(),p##3 v(),p##4 v(),p##5 v(),p##6 v(),p##7 v(),p##8 v(),p##9 v(),p1)
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_2(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##3 v(),p##4 v(),p##5 v(),p##6 v(),p##7 v(),p##8 v(),p##9 v(),p1,p2)
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_3(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##4 v(),p##5 v(),p##6 v(),p##7 v(),p##8 v(),p##9 v(),p1,p2,p3)
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_4(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##5 v(),p##6 v(),p##7 v(),p##8 v(),p##9 v(),p1,p2,p3,p4)
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_5(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##6 v(),p##7 v(),p##8 v(),p##9 v(),p1,p2,p3,p4,p5)
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_6(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##7 v(),p##8 v(),p##9 v(),p1,p2,p3,p4,p5,p6)
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_7(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##8 v(),p##9 v(),p1,p2,p3,p4,p5,p6,p7)
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_8(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p##9 v(),p1,p2,p3,p4,p5,p6,p7,p8)
#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_9(i,p,v) HBOOST_MPL_PP_FILTER_PARAMS_##i(p1,p2,p3,p4,p5,p6,p7,p8,p9)

#else

#   include <hboost/preprocessor/arithmetic/add.hpp>
#   include <hboost/preprocessor/arithmetic/sub.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/tuple/elem.hpp>
#   include <hboost/preprocessor/repeat.hpp>
#   include <hboost/preprocessor/cat.hpp>

#   define HBOOST_MPL_PP_AUX_TAIL_PARAM_FUNC(unused, i, op) \
    , HBOOST_PP_CAT( \
          HBOOST_PP_TUPLE_ELEM(3, 1, op) \
        , HBOOST_PP_ADD_D(1, i, HBOOST_PP_INC(HBOOST_PP_TUPLE_ELEM(3, 0, op))) \
        ) HBOOST_PP_TUPLE_ELEM(3, 2, op)() \
    /**/

#   define HBOOST_MPL_PP_DEF_PARAMS_TAIL_IMPL(i, param, value_func) \
    HBOOST_PP_REPEAT( \
          HBOOST_PP_SUB_D(1, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, i) \
        , HBOOST_MPL_PP_AUX_TAIL_PARAM_FUNC \
        , (i, param, value_func) \
        ) \
    /**/


#endif // HBOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES

#define HBOOST_MPL_PP_DEF_PARAMS_TAIL(i, param, value) \
    HBOOST_MPL_PP_DEF_PARAMS_TAIL_IMPL(i, param, HBOOST_PP_IDENTITY(=value)) \
    /**/

#if !defined(HBOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#   define HBOOST_MPL_PP_NESTED_DEF_PARAMS_TAIL(i, param, value) \
    HBOOST_MPL_PP_DEF_PARAMS_TAIL_IMPL(i, param, HBOOST_PP_IDENTITY(=value)) \
    /**/
#else
#   define HBOOST_MPL_PP_NESTED_DEF_PARAMS_TAIL(i, param, value) \
    HBOOST_MPL_PP_DEF_PARAMS_TAIL_IMPL(i, param, HBOOST_PP_EMPTY) \
    /**/
#endif

#endif // HBOOST_MPL_AUX_PREPROCESSOR_DEF_PARAMS_TAIL_HPP_INCLUDED
