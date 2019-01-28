
#ifndef HBOOST_MPL_AUX_PREPROCESSOR_PARTIAL_SPEC_PARAMS_HPP_INCLUDED
#define HBOOST_MPL_AUX_PREPROCESSOR_PARTIAL_SPEC_PARAMS_HPP_INCLUDED

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
#include <hboost/mpl/aux_/preprocessor/params.hpp>
#include <hboost/mpl/aux_/preprocessor/enum.hpp>
#include <hboost/mpl/aux_/preprocessor/sub.hpp>
#include <hboost/preprocessor/comma_if.hpp>

#define HBOOST_MPL_PP_PARTIAL_SPEC_PARAMS(n, param, def) \
HBOOST_MPL_PP_PARAMS(n, param) \
HBOOST_PP_COMMA_IF(HBOOST_MPL_PP_SUB(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY,n)) \
HBOOST_MPL_PP_ENUM( \
      HBOOST_MPL_PP_SUB(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY,n) \
    , def \
    ) \
/**/

#endif // HBOOST_MPL_AUX_PREPROCESSOR_PARTIAL_SPEC_PARAMS_HPP_INCLUDED
