
#ifndef HBOOST_MPL_DISTANCE_HPP_INCLUDED
#define HBOOST_MPL_DISTANCE_HPP_INCLUDED

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

#include <hboost/mpl/distance_fwd.hpp>
#include <hboost/mpl/iter_fold.hpp>
#include <hboost/mpl/iterator_range.hpp>
#include <hboost/mpl/long.hpp>
#include <hboost/mpl/next.hpp>
#include <hboost/mpl/tag.hpp>
#include <hboost/mpl/apply_wrap.hpp>
#include <hboost/mpl/aux_/msvc_eti_base.hpp>
#include <hboost/mpl/aux_/value_wknd.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/config/forwarding.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>


namespace hboost { namespace mpl {

// default implementation for forward/bidirectional iterators
template< typename Tag > struct distance_impl
{
    template< typename First, typename Last > struct apply
#if !defined(HBOOST_MPL_CFG_NO_NESTED_FORWARDING)
        : aux::msvc_eti_base< typename iter_fold<
              iterator_range<First,Last>
            , mpl::long_<0>
            , next<>
            >::type >
    {
#else
    {
        typedef typename iter_fold<
              iterator_range<First,Last>
            , mpl::long_<0>
            , next<>
            >::type type;
        
        HBOOST_STATIC_CONSTANT(long, value =
              (iter_fold<
                  iterator_range<First,Last>
                , mpl::long_<0>
                , next<>
                >::type::value)
            );
#endif
    };
};

template<
      typename HBOOST_MPL_AUX_NA_PARAM(First)
    , typename HBOOST_MPL_AUX_NA_PARAM(Last)
    >
struct distance
    : distance_impl< typename tag<First>::type >
        ::template apply<First, Last>
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2, distance, (First, Last))
};

HBOOST_MPL_AUX_NA_SPEC(2, distance)

}}

#endif // HBOOST_MPL_DISTANCE_HPP_INCLUDED
