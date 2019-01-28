
#ifndef HBOOST_MPL_AUX_CONFIG_HAS_XXX_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_HAS_XXX_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
// Copyright David Abrahams 2002-2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/config/overload_resolution.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

// agurt, 11/jan/03: signals a stub-only 'has_xxx' implementation

#if !defined(HBOOST_MPL_CFG_NO_HAS_XXX) \
    && (   defined(HBOOST_MPL_CFG_BROKEN_OVERLOAD_RESOLUTION) \
        || HBOOST_WORKAROUND(__GNUC__, <= 2) \
        || HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840)) \
        )

#   define HBOOST_MPL_CFG_NO_HAS_XXX
#   define HBOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE

#endif

#endif // HBOOST_MPL_AUX_CONFIG_HAS_XXX_HPP_INCLUDED
