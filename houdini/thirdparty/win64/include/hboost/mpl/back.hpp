
#ifndef HBOOST_MPL_BACK_HPP_INCLUDED
#define HBOOST_MPL_BACK_HPP_INCLUDED

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

#include <hboost/mpl/back_fwd.hpp>
#include <hboost/mpl/aux_/back_impl.hpp>
#include <hboost/mpl/sequence_tag.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    >
struct back
    : back_impl< typename sequence_tag<Sequence>::type >
        ::template apply< Sequence >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,back,(Sequence))
};

HBOOST_MPL_AUX_NA_SPEC(1, back)

}}

#endif // HBOOST_MPL_BACK_HPP_INCLUDED
