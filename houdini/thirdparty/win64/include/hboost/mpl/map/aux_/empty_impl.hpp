
#ifndef HBOOST_MPL_MAP_AUX_EMPTY_IMPL_HPP_INCLUDED
#define HBOOST_MPL_MAP_AUX_EMPTY_IMPL_HPP_INCLUDED

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

#include <hboost/mpl/empty_fwd.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/mpl/map/aux_/tag.hpp>

namespace hboost { namespace mpl {

template<>
struct empty_impl< aux::map_tag >
{
    template< typename Map > struct apply
        : not_< typename Map::size >
    {
    };
};

}}

#endif // HBOOST_MPL_MAP_AUX_EMPTY_IMPL_HPP_INCLUDED
