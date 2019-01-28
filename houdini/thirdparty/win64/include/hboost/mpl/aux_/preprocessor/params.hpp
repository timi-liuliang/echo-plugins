
#ifndef HBOOST_MPL_AUX_PREPROCESSOR_PARAMS_HPP_INCLUDED
#define HBOOST_MPL_AUX_PREPROCESSOR_PARAMS_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/preprocessor.hpp>

// HBOOST_MPL_PP_PARAMS(0,T): <nothing>
// HBOOST_MPL_PP_PARAMS(1,T): T1
// HBOOST_MPL_PP_PARAMS(2,T): T1, T2
// HBOOST_MPL_PP_PARAMS(n,T): T1, T2, .., Tn

#if !defined(HBOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES)

#   include <hboost/preprocessor/cat.hpp>

#   define HBOOST_MPL_PP_PARAMS(n,p) \
    HBOOST_PP_CAT(HBOOST_MPL_PP_PARAMS_,n)(p) \
    /**/

#   define HBOOST_MPL_PP_PARAMS_0(p)
#   define HBOOST_MPL_PP_PARAMS_1(p) p##1
#   define HBOOST_MPL_PP_PARAMS_2(p) p##1,p##2
#   define HBOOST_MPL_PP_PARAMS_3(p) p##1,p##2,p##3
#   define HBOOST_MPL_PP_PARAMS_4(p) p##1,p##2,p##3,p##4
#   define HBOOST_MPL_PP_PARAMS_5(p) p##1,p##2,p##3,p##4,p##5
#   define HBOOST_MPL_PP_PARAMS_6(p) p##1,p##2,p##3,p##4,p##5,p##6
#   define HBOOST_MPL_PP_PARAMS_7(p) p##1,p##2,p##3,p##4,p##5,p##6,p##7
#   define HBOOST_MPL_PP_PARAMS_8(p) p##1,p##2,p##3,p##4,p##5,p##6,p##7,p##8
#   define HBOOST_MPL_PP_PARAMS_9(p) p##1,p##2,p##3,p##4,p##5,p##6,p##7,p##8,p##9

#else

#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/repeat.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/cat.hpp>

#   define HBOOST_MPL_PP_AUX_PARAM_FUNC(unused, i, param) \
    HBOOST_PP_COMMA_IF(i) \
    HBOOST_PP_CAT(param, HBOOST_PP_INC(i)) \
    /**/

#   define HBOOST_MPL_PP_PARAMS(n, param) \
    HBOOST_PP_REPEAT( \
          n \
        , HBOOST_MPL_PP_AUX_PARAM_FUNC \
        , param \
        ) \
    /**/

#endif 

#endif // HBOOST_MPL_AUX_PREPROCESSOR_PARAMS_HPP_INCLUDED
