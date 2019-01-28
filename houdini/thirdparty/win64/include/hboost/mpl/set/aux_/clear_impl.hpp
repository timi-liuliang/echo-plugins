
#ifndef HBOOST_MPL_SET_AUX_CLEAR_IMPL_HPP_INCLUDED
#define HBOOST_MPL_SET_AUX_CLEAR_IMPL_HPP_INCLUDED

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

#include <hboost/mpl/clear_fwd.hpp>
#include <hboost/mpl/set/aux_/set0.hpp>
#include <hboost/mpl/set/aux_/tag.hpp>

namespace hboost { namespace mpl {

template<>
struct clear_impl< aux::set_tag >
{
    template< typename Set > struct apply
    {
        typedef set0<> type;
    };
};

}}

#endif // HBOOST_MPL_SET_AUX_CLEAR_IMPL_HPP_INCLUDED
