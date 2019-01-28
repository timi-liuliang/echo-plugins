
#ifndef HBOOST_MPL_MAP_AUX_ERASE_IMPL_HPP_INCLUDED
#define HBOOST_MPL_MAP_AUX_ERASE_IMPL_HPP_INCLUDED

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

#include <hboost/mpl/erase_fwd.hpp>
#include <hboost/mpl/map/aux_/erase_key_impl.hpp>
#include <hboost/mpl/map/aux_/tag.hpp>

namespace hboost { namespace mpl {

template<>
struct erase_impl< aux::map_tag >
{
    template< 
          typename Map
        , typename Pos
        , typename unused_
        > 
    struct apply
        : erase_key_impl<aux::map_tag>
            ::apply<Map,typename Pos::type::first>
    {
    };
};

}}

#endif // HBOOST_MPL_MAP_AUX_ERASE_IMPL_HPP_INCLUDED
