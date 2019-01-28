
#ifndef HBOOST_MPL_MULTIPLIES_HPP_INCLUDED
#define HBOOST_MPL_MULTIPLIES_HPP_INCLUDED

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

#include <hboost/mpl/times.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>
#include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#include <hboost/mpl/aux_/preprocessor/params.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>

// backward compatibility header, deprecated

namespace hboost { namespace mpl {

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#   define AUX778076_OP_ARITY HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
#else
#   define AUX778076_OP_ARITY 2
#endif

template<
      HBOOST_MPL_PP_DEFAULT_PARAMS(AUX778076_OP_ARITY, typename N, na)
    >
struct multiplies
    : times< HBOOST_MPL_PP_PARAMS(AUX778076_OP_ARITY, N) >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(
          AUX778076_OP_ARITY
        , multiplies
        , ( HBOOST_MPL_PP_PARAMS(AUX778076_OP_ARITY, N) )
        )
};

HBOOST_MPL_AUX_NA_SPEC(AUX778076_OP_ARITY, multiplies)

#undef AUX778076_OP_ARITY

}}

#endif // HBOOST_MPL_MULTIPLIES_HPP_INCLUDED
