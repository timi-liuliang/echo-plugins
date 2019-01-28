
#ifndef HBOOST_MPL_LIST_AUX_PUSH_BACK_HPP_INCLUDED
#define HBOOST_MPL_LIST_AUX_PUSH_BACK_HPP_INCLUDED

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

#include <hboost/mpl/push_back_fwd.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/list/aux_/tag.hpp>

namespace hboost { namespace mpl {

template< typename Tag > struct has_push_back_impl;

template<>
struct has_push_back_impl< aux::list_tag >
{
    template< typename Seq > struct apply
        : false_
    {
    };
};

}}

#endif // HBOOST_MPL_LIST_AUX_PUSH_BACK_HPP_INCLUDED
