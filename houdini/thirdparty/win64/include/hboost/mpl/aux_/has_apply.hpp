
#ifndef HBOOST_MPL_AUX_HAS_APPLY_HPP_INCLUDED
#define HBOOST_MPL_AUX_HAS_APPLY_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/has_xxx.hpp>
#include <hboost/mpl/aux_/config/has_apply.hpp>

namespace hboost { namespace mpl { namespace aux {
#if !defined(HBOOST_MPL_CFG_NO_HAS_APPLY)
HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_apply, apply, false)
#else
template< typename T, typename fallback_ = false_ >
struct has_apply
    : fallback_
{
};
#endif
}}}

#endif // HBOOST_MPL_AUX_HAS_APPLY_HPP_INCLUDED
