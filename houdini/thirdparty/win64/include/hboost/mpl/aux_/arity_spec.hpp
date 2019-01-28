
#ifndef HBOOST_MPL_AUX_ARITY_SPEC_HPP_INCLUDED
#define HBOOST_MPL_AUX_ARITY_SPEC_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/int.hpp>
#include <hboost/mpl/limits/arity.hpp>
#include <hboost/mpl/aux_/config/dtp.hpp>
#include <hboost/mpl/aux_/preprocessor/params.hpp>
#include <hboost/mpl/aux_/arity.hpp>
#include <hboost/mpl/aux_/template_arity_fwd.hpp>
#include <hboost/mpl/aux_/config/ttp.hpp>
#include <hboost/mpl/aux_/config/lambda.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>

#if defined(HBOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#   define HBOOST_MPL_AUX_NONTYPE_ARITY_SPEC(i,type,name) \
namespace aux { \
template< HBOOST_MPL_AUX_NTTP_DECL(int, N), HBOOST_MPL_PP_PARAMS(i,type T) > \
struct arity< \
      name< HBOOST_MPL_PP_PARAMS(i,T) > \
    , N \
    > \
{ \
    HBOOST_STATIC_CONSTANT(int \
        , value = HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        ); \
}; \
} \
/**/
#else
#   define HBOOST_MPL_AUX_NONTYPE_ARITY_SPEC(i,type,name) /**/
#endif

#   define HBOOST_MPL_AUX_ARITY_SPEC(i,name) \
    HBOOST_MPL_AUX_NONTYPE_ARITY_SPEC(i,typename,name) \
/**/


#if defined(HBOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING) \
    && !defined(HBOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
#   define HBOOST_MPL_AUX_TEMPLATE_ARITY_SPEC(i, name) \
namespace aux { \
template< HBOOST_MPL_PP_PARAMS(i,typename T) > \
struct template_arity< name<HBOOST_MPL_PP_PARAMS(i,T)> > \
    : int_<i> \
{ \
}; \
} \
/**/
#else
#   define HBOOST_MPL_AUX_TEMPLATE_ARITY_SPEC(i, name) /**/
#endif


#endif // HBOOST_MPL_AUX_ARITY_SPEC_HPP_INCLUDED
