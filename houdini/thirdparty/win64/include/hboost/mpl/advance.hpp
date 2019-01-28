
#ifndef HBOOST_MPL_ADVANCE_HPP_INCLUDED
#define HBOOST_MPL_ADVANCE_HPP_INCLUDED

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

#include <hboost/mpl/advance_fwd.hpp>
#include <hboost/mpl/less.hpp>
#include <hboost/mpl/negate.hpp>
#include <hboost/mpl/long.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/tag.hpp>
#include <hboost/mpl/apply_wrap.hpp>
#include <hboost/mpl/aux_/advance_forward.hpp>
#include <hboost/mpl/aux_/advance_backward.hpp>
#include <hboost/mpl/aux_/value_wknd.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/nttp_decl.hpp>

namespace hboost { namespace mpl {

// default implementation for forward/bidirectional iterators
template< typename Tag >
struct advance_impl
{
    template< typename Iterator, typename N > struct apply
    {
        typedef typename less< N,long_<0> >::type backward_;
        typedef typename if_< backward_, negate<N>, N >::type offset_;

        typedef typename if_<
              backward_
            , aux::advance_backward< HBOOST_MPL_AUX_VALUE_WKND(offset_)::value >
            , aux::advance_forward< HBOOST_MPL_AUX_VALUE_WKND(offset_)::value >
            >::type f_;

        typedef typename apply_wrap1<f_,Iterator>::type type;
    };
};


template<
      typename HBOOST_MPL_AUX_NA_PARAM(Iterator)
    , typename HBOOST_MPL_AUX_NA_PARAM(N)
    >
struct advance
    : advance_impl< typename tag<Iterator>::type >
        ::template apply<Iterator,N>
{
};

template<
      typename Iterator
    , HBOOST_MPL_AUX_NTTP_DECL(long, N)
    >
struct advance_c
    : advance_impl< typename tag<Iterator>::type >
        ::template apply<Iterator,long_<N> >
{
};

HBOOST_MPL_AUX_NA_SPEC(2, advance)

}}

#endif // HBOOST_MPL_ADVANCE_HPP_INCLUDED
