
#ifndef HBOOST_MPL_SET_AUX_INSERT_IMPL_HPP_INCLUDED
#define HBOOST_MPL_SET_AUX_INSERT_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2007
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

#include <hboost/mpl/insert_fwd.hpp>
#include <hboost/mpl/set/aux_/has_key_impl.hpp>
#include <hboost/mpl/set/aux_/item.hpp>
#include <hboost/mpl/set/aux_/tag.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/base.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/aux_/na.hpp>

#include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace mpl {

namespace aux {
template<  typename Set, typename T > struct set_insert_impl
    : eval_if< 
          has_key_impl<aux::set_tag>::apply<Set,T>
        , identity<Set>
        , eval_if< 
              is_same< T,typename Set::last_masked_ > 
            , base<Set>
            , identity< s_item<T,typename Set::item_> >
            >
        >
{
};
}

template<>
struct insert_impl< aux::set_tag >
{
    template< 
          typename Set
        , typename PosOrKey
        , typename KeyOrNA
        > 
    struct apply
        : aux::set_insert_impl<
              Set
            , typename if_na<KeyOrNA,PosOrKey>::type
            >
    {
    };
};

}}

#endif // HBOOST_MPL_SET_AUX_INSERT_IMPL_HPP_INCLUDED
