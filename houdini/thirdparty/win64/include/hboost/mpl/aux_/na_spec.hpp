
#ifndef HBOOST_MPL_AUX_NA_SPEC_HPP_INCLUDED
#define HBOOST_MPL_AUX_NA_SPEC_HPP_INCLUDED

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

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/lambda_fwd.hpp>
#   include <hboost/mpl/int.hpp>
#   include <hboost/mpl/bool.hpp>
#   include <hboost/mpl/aux_/na.hpp>
#   include <hboost/mpl/aux_/arity.hpp>
#   include <hboost/mpl/aux_/template_arity_fwd.hpp>
#endif

#include <hboost/mpl/aux_/preprocessor/params.hpp>
#include <hboost/mpl/aux_/preprocessor/enum.hpp>
#include <hboost/mpl/aux_/preprocessor/def_params_tail.hpp>
#include <hboost/mpl/aux_/lambda_arity_param.hpp>
#include <hboost/mpl/aux_/config/dtp.hpp>
#include <hboost/mpl/aux_/config/eti.hpp>
#include <hboost/mpl/aux_/nttp_decl.hpp>
#include <hboost/mpl/aux_/config/ttp.hpp>
#include <hboost/mpl/aux_/config/lambda.hpp>
#include <hboost/mpl/aux_/config/overload_resolution.hpp>


#define HBOOST_MPL_AUX_NA_PARAMS(i) \
    HBOOST_MPL_PP_ENUM(i, na) \
/**/

#if defined(HBOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#   define HBOOST_MPL_AUX_NA_SPEC_ARITY(i, name) \
namespace aux { \
template< HBOOST_MPL_AUX_NTTP_DECL(int, N) > \
struct arity< \
          name< HBOOST_MPL_AUX_NA_PARAMS(i) > \
        , N \
        > \
    : int_< HBOOST_MPL_LIMIT_METAFUNCTION_ARITY > \
{ \
}; \
} \
/**/
#else
#   define HBOOST_MPL_AUX_NA_SPEC_ARITY(i, name) /**/
#endif

#define HBOOST_MPL_AUX_NA_SPEC_MAIN(i, name) \
template<> \
struct name< HBOOST_MPL_AUX_NA_PARAMS(i) > \
{ \
    template< \
          HBOOST_MPL_PP_PARAMS(i, typename T) \
        HBOOST_MPL_PP_NESTED_DEF_PARAMS_TAIL(i, typename T, na) \
        > \
    struct apply \
        : name< HBOOST_MPL_PP_PARAMS(i, T) > \
    { \
    }; \
}; \
/**/

#if defined(HBOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
#   define HBOOST_MPL_AUX_NA_SPEC_LAMBDA(i, name) \
template<> \
struct lambda< \
      name< HBOOST_MPL_AUX_NA_PARAMS(i) > \
    , void_ \
    , true_ \
    > \
{ \
    typedef false_ is_le; \
    typedef name< HBOOST_MPL_AUX_NA_PARAMS(i) > type; \
}; \
template<> \
struct lambda< \
      name< HBOOST_MPL_AUX_NA_PARAMS(i) > \
    , void_ \
    , false_ \
    > \
{ \
    typedef false_ is_le; \
    typedef name< HBOOST_MPL_AUX_NA_PARAMS(i) > type; \
}; \
/**/
#else
#   define HBOOST_MPL_AUX_NA_SPEC_LAMBDA(i, name) \
template< typename Tag > \
struct lambda< \
      name< HBOOST_MPL_AUX_NA_PARAMS(i) > \
    , Tag \
    HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(int_<-1>) \
    > \
{ \
    typedef false_ is_le; \
    typedef name< HBOOST_MPL_AUX_NA_PARAMS(i) > result_; \
    typedef name< HBOOST_MPL_AUX_NA_PARAMS(i) > type; \
}; \
/**/
#endif

#if defined(HBOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING) \
    || defined(HBOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT) \
        && defined(HBOOST_MPL_CFG_BROKEN_OVERLOAD_RESOLUTION)
#   define HBOOST_MPL_AUX_NA_SPEC_TEMPLATE_ARITY(i, j, name) \
namespace aux { \
template< HBOOST_MPL_PP_PARAMS(j, typename T) > \
struct template_arity< \
          name< HBOOST_MPL_PP_PARAMS(j, T) > \
        > \
    : int_<j> \
{ \
}; \
\
template<> \
struct template_arity< \
          name< HBOOST_MPL_PP_ENUM(i, na) > \
        > \
    : int_<-1> \
{ \
}; \
} \
/**/
#else
#   define HBOOST_MPL_AUX_NA_SPEC_TEMPLATE_ARITY(i, j, name) /**/
#endif

#if defined(HBOOST_MPL_CFG_MSVC_ETI_BUG)
#   define HBOOST_MPL_AUX_NA_SPEC_ETI(i, name) \
template<> \
struct name< HBOOST_MPL_PP_ENUM(i, int) > \
{ \
    typedef int type; \
    enum { value = 0 }; \
}; \
/**/
#else
#   define HBOOST_MPL_AUX_NA_SPEC_ETI(i, name) /**/
#endif

#define HBOOST_MPL_AUX_NA_PARAM(param) param = na

#define HBOOST_MPL_AUX_NA_SPEC_NO_ETI(i, name) \
HBOOST_MPL_AUX_NA_SPEC_MAIN(i, name) \
HBOOST_MPL_AUX_NA_SPEC_LAMBDA(i, name) \
HBOOST_MPL_AUX_NA_SPEC_ARITY(i, name) \
HBOOST_MPL_AUX_NA_SPEC_TEMPLATE_ARITY(i, i, name) \
/**/

#define HBOOST_MPL_AUX_NA_SPEC(i, name) \
HBOOST_MPL_AUX_NA_SPEC_NO_ETI(i, name) \
HBOOST_MPL_AUX_NA_SPEC_ETI(i, name) \
/**/

#define HBOOST_MPL_AUX_NA_SPEC2(i, j, name) \
HBOOST_MPL_AUX_NA_SPEC_MAIN(i, name) \
HBOOST_MPL_AUX_NA_SPEC_ETI(i, name) \
HBOOST_MPL_AUX_NA_SPEC_LAMBDA(i, name) \
HBOOST_MPL_AUX_NA_SPEC_ARITY(i, name) \
HBOOST_MPL_AUX_NA_SPEC_TEMPLATE_ARITY(i, j, name) \
/**/


#endif // HBOOST_MPL_AUX_NA_SPEC_HPP_INCLUDED
