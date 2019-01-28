
#ifndef HBOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED
#define HBOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/lambda.hpp>

#if !defined(HBOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)

#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) /**/
#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT(i,name,params) /**/

#else

#   include <hboost/mpl/int_fwd.hpp>
#   include <hboost/mpl/aux_/yes_no.hpp>
#   include <hboost/mpl/aux_/na_fwd.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/enum.hpp>
#   include <hboost/mpl/aux_/config/msvc.hpp>
#   include <hboost/mpl/aux_/config/workaround.hpp>

#   include <hboost/preprocessor/tuple/to_list.hpp>
#   include <hboost/preprocessor/list/for_each_i.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/cat.hpp>

#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC(R,typedef_,i,param) \
    typedef_ param HBOOST_PP_CAT(arg,HBOOST_PP_INC(i)); \
    /**/

// agurt, 07/mar/03: restore an old revision for the sake of SGI MIPSpro C++
#if HBOOST_WORKAROUND(__EDG_VERSION__, <= 238) 

#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    typedef HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::int_<i> arity; \
    HBOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , HBOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , HBOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    struct rebind \
    { \
        template< HBOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
            : name< HBOOST_MPL_PP_PARAMS(i,U) > \
        { \
        }; \
    }; \
    /**/

#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    /**/

#elif HBOOST_WORKAROUND(__EDG_VERSION__, <= 244) && !defined(HBOOST_INTEL_CXX_VERSION)
// agurt, 18/jan/03: old EDG-based compilers actually enforce 11.4 para 9
// (in strict mode), so we have to provide an alternative to the 
// MSVC-optimized implementation

#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    typedef HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::int_<i> arity; \
    HBOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , HBOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , HBOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    struct rebind; \
/**/

#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
template< HBOOST_MPL_PP_PARAMS(i,typename T) > \
struct name<HBOOST_MPL_PP_PARAMS(i,T)>::rebind \
{ \
    template< HBOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
        : name< HBOOST_MPL_PP_PARAMS(i,U) > \
    { \
    }; \
/**/

#else // __EDG_VERSION__

namespace hboost { namespace mpl { namespace aux {
template< typename T > struct has_rebind_tag;
}}}

#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    typedef HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::int_<i> arity; \
    HBOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , HBOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , HBOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    friend class HBOOST_PP_CAT(name,_rebind); \
    typedef HBOOST_PP_CAT(name,_rebind) rebind; \
/**/

#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610))
#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
template< HBOOST_MPL_PP_PARAMS(i,typename T) > \
::hboost::mpl::aux::yes_tag operator|( \
      ::hboost::mpl::aux::has_rebind_tag<int> \
    , name<HBOOST_MPL_PP_PARAMS(i,T)>* \
    ); \
::hboost::mpl::aux::no_tag operator|( \
      ::hboost::mpl::aux::has_rebind_tag<int> \
    , name< HBOOST_MPL_PP_ENUM(i,::hboost::mpl::na) >* \
    ); \
/**/
#elif !HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
template< HBOOST_MPL_PP_PARAMS(i,typename T) > \
::hboost::mpl::aux::yes_tag operator|( \
      ::hboost::mpl::aux::has_rebind_tag<int> \
    , ::hboost::mpl::aux::has_rebind_tag< name<HBOOST_MPL_PP_PARAMS(i,T)> >* \
    ); \
/**/
#else
#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) /**/
#endif

#   if !defined(__BORLANDC__)
#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
HBOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
class HBOOST_PP_CAT(name,_rebind) \
{ \
 public: \
    template< HBOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
        : name< HBOOST_MPL_PP_PARAMS(i,U) > \
    { \
    }; \
/**/
#   else
#   define HBOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
HBOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
class HBOOST_PP_CAT(name,_rebind) \
{ \
 public: \
    template< HBOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
    { \
        typedef typename name< HBOOST_MPL_PP_PARAMS(i,U) >::type type; \
    }; \
/**/
#   endif // __BORLANDC__

#endif // __EDG_VERSION__

#endif // HBOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT

#endif // HBOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED
