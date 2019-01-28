
#ifndef HBOOST_MPL_SIZEOF_HPP_INCLUDED
#define HBOOST_MPL_SIZEOF_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/size_t.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(T)
    >
struct sizeof_
    : mpl::size_t< sizeof(T) >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,sizeof_,(T))
};

HBOOST_MPL_AUX_NA_SPEC_NO_ETI(1, sizeof_)

}}

#endif // HBOOST_MPL_SIZEOF_HPP_INCLUDED
