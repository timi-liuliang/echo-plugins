
#ifndef HBOOST_MPL_VECTOR_AUX_SIZE_HPP_INCLUDED
#define HBOOST_MPL_VECTOR_AUX_SIZE_HPP_INCLUDED

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

#include <hboost/mpl/size_fwd.hpp>
#include <hboost/mpl/vector/aux_/O1_size.hpp>
#include <hboost/mpl/vector/aux_/tag.hpp>
#include <hboost/mpl/aux_/config/typeof.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>

namespace hboost { namespace mpl {

#if defined(HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

template<>
struct size_impl< aux::vector_tag >
    : O1_size_impl< aux::vector_tag >
{
};

#else

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< long N >
struct size_impl< aux::vector_tag<N> >
    : O1_size_impl< aux::vector_tag<N> >
{
};

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

}}

#endif // HBOOST_MPL_VECTOR_AUX_SIZE_HPP_INCLUDED
