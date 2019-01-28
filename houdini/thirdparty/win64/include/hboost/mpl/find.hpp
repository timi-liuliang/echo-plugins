
#ifndef HBOOST_MPL_FIND_HPP_INCLUDED
#define HBOOST_MPL_FIND_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2002
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/find_if.hpp>
#include <hboost/mpl/same_as.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename HBOOST_MPL_AUX_NA_PARAM(T)
    >
struct find
    : find_if< Sequence,same_as<T> >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,find,(Sequence,T))
};

HBOOST_MPL_AUX_NA_SPEC(2, find)

}}

#endif // HBOOST_MPL_FIND_HPP_INCLUDED
