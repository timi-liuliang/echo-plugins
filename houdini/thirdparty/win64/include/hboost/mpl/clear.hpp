
#ifndef HBOOST_MPL_CLEAR_HPP_INCLUDED
#define HBOOST_MPL_CLEAR_HPP_INCLUDED

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

#include <hboost/mpl/clear_fwd.hpp>
#include <hboost/mpl/aux_/clear_impl.hpp>
#include <hboost/mpl/sequence_tag.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    >
struct clear
    : clear_impl< typename sequence_tag<Sequence>::type >
        ::template apply< Sequence >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,clear,(Sequence))
};

HBOOST_MPL_AUX_NA_SPEC(1, clear)

}}

#endif // HBOOST_MPL_CLEAR_HPP_INCLUDED
