
#ifndef HBOOST_MPL_INSERT_RANGE_HPP_INCLUDED
#define HBOOST_MPL_INSERT_RANGE_HPP_INCLUDED

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

#include <hboost/mpl/insert_range_fwd.hpp>
#include <hboost/mpl/sequence_tag.hpp>
#include <hboost/mpl/aux_/insert_range_impl.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename HBOOST_MPL_AUX_NA_PARAM(Pos)
    , typename HBOOST_MPL_AUX_NA_PARAM(Range)
    >
struct insert_range
    : insert_range_impl< typename sequence_tag<Sequence>::type >
        ::template apply< Sequence,Pos,Range >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(3,insert_range,(Sequence,Pos,Range))
};

HBOOST_MPL_AUX_NA_SPEC(3, insert_range)

}}

#endif // HBOOST_MPL_INSERT_RANGE_HPP_INCLUDED
