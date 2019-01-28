
#ifndef HBOOST_MPL_MAP_AUX_CONTAINS_IMPL_HPP_INCLUDED
#define HBOOST_MPL_MAP_AUX_CONTAINS_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/contains_fwd.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/map/aux_/at_impl.hpp>
#include <hboost/mpl/map/aux_/tag.hpp>

#include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace mpl {

template<>
struct contains_impl< aux::map_tag >
{
    template< typename Map, typename Pair > struct apply
        : is_same< 
              typename at_impl<aux::map_tag>::apply<
                  Map
                , typename Pair::first
                >::type
            , typename Pair::second
            >
    {
    };
};

}}

#endif // HBOOST_MPL_MAP_AUX_CONTAINS_IMPL_HPP_INCLUDED
