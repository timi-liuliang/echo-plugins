
#ifndef HBOOST_MPL_MAP_AUX_HAS_KEY_IMPL_HPP_INCLUDED
#define HBOOST_MPL_MAP_AUX_HAS_KEY_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/has_key_fwd.hpp>
#include <hboost/mpl/map/aux_/tag.hpp>
#include <hboost/mpl/map/aux_/at_impl.hpp>
#include <hboost/mpl/void.hpp>
#include <hboost/mpl/aux_/config/typeof.hpp>

namespace hboost { namespace mpl {

template<>
struct has_key_impl< aux::map_tag >
{
    template< typename Map, typename Key > struct apply
#if defined(HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)
        : is_not_void_< 
              typename at_impl<aux::map_tag>
                ::apply<Map,Key>::type
            >
#else
        : bool_< ( x_order_impl<Map,Key>::value > 1 ) >
#endif
    {
    };
};

}}

#endif // HBOOST_MPL_MAP_AUX_HAS_KEY_IMPL_HPP_INCLUDED
