
#ifndef HBOOST_MPL_AUX_PREPROCESSOR_DEFAULT_PARAMS_HPP_INCLUDED
#define HBOOST_MPL_AUX_PREPROCESSOR_DEFAULT_PARAMS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
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

// HBOOST_MPL_PP_DEFAULT_PARAMS(0,T,int): <nothing>
// HBOOST_MPL_PP_DEFAULT_PARAMS(1,T,int): T1 = int
// HBOOST_MPL_PP_DEFAULT_PARAMS(2,T,int): T1 = int, T2 = int
// HBOOST_MPL_PP_DEFAULT_PARAMS(n,T,int): T1 = int, T2 = int, .., Tn = int

#if !defined(HBOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES)

#   include <hboost/preprocessor/cat.hpp>

#   define HBOOST_MPL_PP_DEFAULT_PARAMS(n,p,v) \
    HBOOST_PP_CAT(HBOOST_MPL_PP_DEFAULT_PARAMS_,n)(p,v) \
    /**/
    
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_0(p,v)
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_1(p,v) p##1=v
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_2(p,v) p##1=v,p##2=v
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_3(p,v) p##1=v,p##2=v,p##3=v
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_4(p,v) p##1=v,p##2=v,p##3=v,p##4=v
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_5(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_6(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v,p##6=v
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_7(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v,p##6=v,p##7=v
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_8(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v,p##6=v,p##7=v,p##8=v
#   define HBOOST_MPL_PP_DEFAULT_PARAMS_9(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v,p##6=v,p##7=v,p##8=v,p##9=v

#else

#   include <hboost/preprocessor/tuple/elem.hpp>
#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/repeat.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/cat.hpp>

#   define HBOOST_MPL_PP_AUX_DEFAULT_PARAM_FUNC(unused, i, pv) \
    HBOOST_PP_COMMA_IF(i) \
    HBOOST_PP_CAT( HBOOST_PP_TUPLE_ELEM(2,0,pv), HBOOST_PP_INC(i) ) \
        = HBOOST_PP_TUPLE_ELEM(2,1,pv) \
    /**/

#   define HBOOST_MPL_PP_DEFAULT_PARAMS(n, param, value) \
    HBOOST_PP_REPEAT( \
          n \
        , HBOOST_MPL_PP_AUX_DEFAULT_PARAM_FUNC \
        , (param,value) \
        ) \
    /**/

#endif

#endif // HBOOST_MPL_AUX_PREPROCESSOR_DEFAULT_PARAMS_HPP_INCLUDED
