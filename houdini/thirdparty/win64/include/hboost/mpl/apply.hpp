
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_APPLY_HPP_INCLUDED
#define HBOOST_MPL_APPLY_HPP_INCLUDED

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

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/apply_fwd.hpp>
#   include <hboost/mpl/apply_wrap.hpp>
#   include <hboost/mpl/placeholders.hpp>
#   include <hboost/mpl/lambda.hpp>
#   include <hboost/mpl/aux_/na.hpp>
#   include <hboost/mpl/aux_/lambda_support.hpp>
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER apply.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/partial_spec_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/enum.hpp>
#   include <hboost/mpl/aux_/config/lambda.hpp>
#   include <hboost/mpl/aux_/config/dtp.hpp>
#   include <hboost/mpl/aux_/nttp_decl.hpp>
#   include <hboost/mpl/aux_/config/eti.hpp>
#   include <hboost/mpl/aux_/config/msvc.hpp>
#   include <hboost/mpl/aux_/config/workaround.hpp>

#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/cat.hpp>

namespace hboost { namespace mpl {

// local macros, #undef-ined at the end of the header
#   define AUX778076_APPLY_PARAMS(param) \
    HBOOST_MPL_PP_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        ) \
    /**/

#   define AUX778076_APPLY_DEF_PARAMS(param, value) \
    HBOOST_MPL_PP_DEFAULT_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        , value \
        ) \
    /**/

#   define AUX778076_APPLY_N_PARAMS(n, param) \
    HBOOST_MPL_PP_PARAMS(n, param) \
    /**/

#   define AUX778076_APPLY_N_COMMA_PARAMS(n, param) \
    HBOOST_PP_COMMA_IF(n) \
    HBOOST_MPL_PP_PARAMS(n, param) \
    /**/

#   define AUX778076_APPLY_N_PARTIAL_SPEC_PARAMS(n, param, def) \
    HBOOST_PP_COMMA_IF(n) \
    HBOOST_MPL_PP_PARTIAL_SPEC_PARAMS(n, param, def) \
    /**/
    
#   define AUX778076_APPLY_N_SPEC_PARAMS(n, param) \
    HBOOST_MPL_PP_ENUM(HBOOST_PP_INC(n), param) \
    /**/


#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/apply.hpp>))
#include HBOOST_PP_ITERATE()

#   if !defined(HBOOST_MPL_CFG_NO_APPLY_TEMPLATE)
// real C++ version is already taken care of
#   if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace aux {
// apply_count_args
#define AUX778076_COUNT_ARGS_PREFIX apply
#define AUX778076_COUNT_ARGS_DEFAULT na
#define AUX778076_COUNT_ARGS_ARITY HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
#include <hboost/mpl/aux_/count_args.hpp>
}


template<
      typename F, AUX778076_APPLY_DEF_PARAMS(typename T, na)
    >
struct apply
    : aux::apply_chooser< 
          aux::apply_count_args< AUX778076_APPLY_PARAMS(T) >::value
        >::template result_< F, AUX778076_APPLY_PARAMS(T) >::type
{
};

#   endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#   endif // HBOOST_MPL_CFG_NO_APPLY_TEMPLATE

#   undef AUX778076_APPLY_N_SPEC_PARAMS
#   undef AUX778076_APPLY_N_PARTIAL_SPEC_PARAMS
#   undef AUX778076_APPLY_N_COMMA_PARAMS
#   undef AUX778076_APPLY_N_PARAMS
#   undef AUX778076_APPLY_DEF_PARAMS
#   undef AUX778076_APPLY_PARAMS

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_APPLY_HPP_INCLUDED

///// iteration, depth == 1

// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1

#   define i_ HBOOST_PP_FRAME_ITERATION(1)

template<
      typename F AUX778076_APPLY_N_COMMA_PARAMS(i_, typename T)
    >
struct HBOOST_PP_CAT(apply,i_)
#if !HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    : HBOOST_PP_CAT(apply_wrap,i_)< 
          typename lambda<F>::type
        AUX778076_APPLY_N_COMMA_PARAMS(i_, T)
        >
{
#else
{
    typedef typename HBOOST_PP_CAT(apply_wrap,i_)< 
          typename lambda<F>::type
        AUX778076_APPLY_N_COMMA_PARAMS(i_, T)
        >::type type;
#endif
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(
          HBOOST_PP_INC(i_)
        , HBOOST_PP_CAT(apply,i_)
        , (F AUX778076_APPLY_N_COMMA_PARAMS(i_,T))
        )
};


#if defined(HBOOST_MPL_CFG_MSVC_ETI_BUG)
/// workaround for ETI bug
template<>
struct HBOOST_PP_CAT(apply,i_)<AUX778076_APPLY_N_SPEC_PARAMS(i_, int)>
{
    typedef int type;
};
#endif

#   if !defined(HBOOST_MPL_CFG_NO_APPLY_TEMPLATE)
#   if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#if i_ == HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
/// primary template (not a specialization!)
template<
      typename F AUX778076_APPLY_N_COMMA_PARAMS(i_, typename T)
    >
struct apply
    : HBOOST_PP_CAT(apply,i_)< F AUX778076_APPLY_N_COMMA_PARAMS(i_, T) >
{
};
#else
template<
      typename F AUX778076_APPLY_N_COMMA_PARAMS(i_, typename T)
    >
struct apply< F AUX778076_APPLY_N_PARTIAL_SPEC_PARAMS(i_, T, na) >
    : HBOOST_PP_CAT(apply,i_)< F AUX778076_APPLY_N_COMMA_PARAMS(i_, T) >
{
};
#endif

#   else // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#if !defined(HBOOST_MPL_CFG_NO_APPLY_TEMPLATE)
namespace aux {

template<>
struct apply_chooser<i_>
{
    template<
          typename F, AUX778076_APPLY_PARAMS(typename T)
        >
    struct result_
    {
        typedef HBOOST_PP_CAT(apply,i_)<
              F AUX778076_APPLY_N_COMMA_PARAMS(i_, T)
            > type;
    };
};

} // namespace aux
#endif

#   endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#   endif // HBOOST_MPL_CFG_NO_APPLY_TEMPLATE

#   undef i_

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif // HBOOST_PP_IS_ITERATING
