
#ifndef HBOOST_MPL_VECTOR_AUX_EMPTY_HPP_INCLUDED
#define HBOOST_MPL_VECTOR_AUX_EMPTY_HPP_INCLUDED

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

#include <hboost/mpl/empty_fwd.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/vector/aux_/tag.hpp>
#include <hboost/mpl/aux_/config/typeof.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>
#include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace mpl {

#if defined(HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

template<>
struct empty_impl< aux::vector_tag >
{
    template< typename Vector > struct apply
        : is_same<
              typename Vector::lower_bound_
            , typename Vector::upper_bound_
            >
    {
    };
};

#else

template<>
struct empty_impl< aux::vector_tag<0> >
{
    template< typename Vector > struct apply
        : true_
    {
    };
};

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< long N >
struct empty_impl< aux::vector_tag<N> >
{
    template< typename Vector > struct apply
        : false_
    {
    };
};

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

}}

#endif // HBOOST_MPL_VECTOR_AUX_EMPTY_HPP_INCLUDED
