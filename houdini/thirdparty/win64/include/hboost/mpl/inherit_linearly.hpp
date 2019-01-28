
#ifndef HBOOST_MPL_INHERIT_FRONT_TO_BACK_HPP_INCLUDED
#define HBOOST_MPL_INHERIT_FRONT_TO_BACK_HPP_INCLUDED

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

#include <hboost/mpl/fold.hpp>
#include <hboost/mpl/empty_base.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Types_)
    , typename HBOOST_MPL_AUX_NA_PARAM(Node_)
    , typename Root_ = empty_base
    >
struct inherit_linearly
    : fold<Types_,Root_,Node_>
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(3,inherit_linearly,(Types_,Node_,Root_))
};

HBOOST_MPL_AUX_NA_SPEC(2, inherit_linearly)

}}

#endif // HBOOST_MPL_INHERIT_FRONT_TO_BACK_HPP_INCLUDED
