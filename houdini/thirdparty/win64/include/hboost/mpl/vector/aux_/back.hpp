
#ifndef HBOOST_MPL_VECTOR_AUX_BACK_HPP_INCLUDED
#define HBOOST_MPL_VECTOR_AUX_BACK_HPP_INCLUDED

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
#include <hboost/mpl/next_prior.hpp>
#include <hboost/mpl/vector/aux_/at.hpp>
#include <hboost/mpl/vector/aux_/tag.hpp>
#include <hboost/mpl/aux_/config/typeof.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>

namespace hboost { namespace mpl {

#if defined(HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

template<>
struct back_impl< aux::vector_tag >
{
    template< typename Vector > struct apply
        : v_at<
              Vector
            , prior<typename Vector::size>::type::value
            >
    {
    };
};

#else

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< long n_ >
struct back_impl< aux::vector_tag<n_> >
{
    template< typename Vector > struct apply
    {
        typedef typename Vector::back type;
    };
};

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

}}

#endif // HBOOST_MPL_VECTOR_AUX_BACK_HPP_INCLUDED
