
#ifndef HBOOST_MPL_BEGIN_END_HPP_INCLUDED
#define HBOOST_MPL_BEGIN_END_HPP_INCLUDED

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

#include <hboost/mpl/begin_end_fwd.hpp>
#include <hboost/mpl/aux_/begin_end_impl.hpp>
#include <hboost/mpl/sequence_tag.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

// agurt, 13/sep/02: switched from inheritance to typedef; MSVC is more
// happy this way (less ETI-related errors), and it doesn't affect 
// anything else
template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    >
struct begin
{
    typedef typename sequence_tag<Sequence>::type tag_;
    typedef typename begin_impl< tag_ >
        ::template apply< Sequence >::type type;

    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,begin,(Sequence))
};

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    >
struct end
{
    typedef typename sequence_tag<Sequence>::type tag_;
    typedef typename end_impl< tag_ >
        ::template apply< Sequence >::type type;

    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,end,(Sequence))
};

HBOOST_MPL_AUX_NA_SPEC(1, begin)
HBOOST_MPL_AUX_NA_SPEC(1, end)

}}

#endif // HBOOST_MPL_BEGIN_END_HPP_INCLUDED
