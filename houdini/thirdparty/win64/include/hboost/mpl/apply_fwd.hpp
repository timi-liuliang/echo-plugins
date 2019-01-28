
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_APPLY_FWD_HPP_INCLUDED
#define HBOOST_MPL_APPLY_FWD_HPP_INCLUDED

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
#   include <hboost/mpl/aux_/na.hpp>
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER apply_fwd.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#   include <hboost/mpl/aux_/config/ctps.hpp>
#   include <hboost/mpl/aux_/nttp_decl.hpp>

#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/cat.hpp>

// agurt, 15/jan/02: top-level 'apply' template gives an ICE on MSVC
// (for known reasons)
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
#   define HBOOST_MPL_CFG_NO_APPLY_TEMPLATE
#endif

namespace hboost { namespace mpl {

// local macro, #undef-ined at the end of the header
#   define AUX778076_APPLY_DEF_PARAMS(param, value) \
    HBOOST_MPL_PP_DEFAULT_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        , value \
        ) \
    /**/

#   define AUX778076_APPLY_N_COMMA_PARAMS(n, param) \
    HBOOST_PP_COMMA_IF(n) \
    HBOOST_MPL_PP_PARAMS(n, param) \
    /**/

#   if !defined(HBOOST_MPL_CFG_NO_APPLY_TEMPLATE)

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
// forward declaration
template<
      typename F, AUX778076_APPLY_DEF_PARAMS(typename T, na)
    >
struct apply;
#else
namespace aux {
template< HBOOST_AUX_NTTP_DECL(int, arity_) > struct apply_chooser;
}
#endif

#   endif // HBOOST_MPL_CFG_NO_APPLY_TEMPLATE

#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/apply_fwd.hpp>))
#include HBOOST_PP_ITERATE()


#   undef AUX778076_APPLY_N_COMMA_PARAMS
#   undef AUX778076_APPLY_DEF_PARAMS

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_APPLY_FWD_HPP_INCLUDED

///// iteration

#else
#define i_ HBOOST_PP_FRAME_ITERATION(1)

template<
      typename F AUX778076_APPLY_N_COMMA_PARAMS(i_, typename T)
    >
struct HBOOST_PP_CAT(apply,i_);

#undef i_
#endif // HBOOST_PP_IS_ITERATING
