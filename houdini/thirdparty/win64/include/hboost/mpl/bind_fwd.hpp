
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_BIND_FWD_HPP_INCLUDED
#define HBOOST_MPL_BIND_FWD_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/bind.hpp>
#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER bind_fwd.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#   include <hboost/mpl/aux_/config/dmc_ambiguous_ctps.hpp>

#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/cat.hpp>

namespace hboost { namespace mpl {

// local macros, #undef-ined at the end of the header

#   if defined(HBOOST_MPL_CFG_DMC_AMBIGUOUS_CTPS)
#       define AUX778076_DMC_PARAM() , int dummy_ = 0
#   else
#       define AUX778076_DMC_PARAM()
#   endif

#   define AUX778076_BIND_DEFAULT_PARAMS(param, value) \
    HBOOST_MPL_PP_DEFAULT_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        , value \
        ) \
    AUX778076_DMC_PARAM() \
    /**/

#   define AUX778076_BIND_N_PARAMS(n, param) \
    HBOOST_PP_COMMA_IF(n) HBOOST_MPL_PP_PARAMS(n, param) \
    AUX778076_DMC_PARAM() \
    /**/

#if !defined(HBOOST_MPL_CFG_NO_BIND_TEMPLATE)
template<
      typename F, AUX778076_BIND_DEFAULT_PARAMS(typename T, na)
    >
struct bind;
#endif

#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/bind_fwd.hpp>))
#include HBOOST_PP_ITERATE()

#   undef AUX778076_BIND_N_PARAMS
#   undef AUX778076_BIND_DEFAULT_PARAMS
#   undef AUX778076_DMC_PARAM
}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_BIND_FWD_HPP_INCLUDED

///// iteration

#else
#define i_ HBOOST_PP_FRAME_ITERATION(1)

template<
      typename F AUX778076_BIND_N_PARAMS(i_, typename T)
    >
struct HBOOST_PP_CAT(bind,i_);

#undef i_
#endif // HBOOST_PP_IS_ITERATING
