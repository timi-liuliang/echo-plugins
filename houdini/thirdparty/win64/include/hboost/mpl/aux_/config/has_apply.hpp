
#ifndef HBOOST_MPL_AUX_CONFIG_HAS_APPLY_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_HAS_APPLY_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/has_xxx.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if !defined(HBOOST_MPL_CFG_NO_HAS_APPLY) \
    && (   defined(HBOOST_MPL_CFG_NO_HAS_XXX) \
        || HBOOST_WORKAROUND(__EDG_VERSION__, < 300) \
        || HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) \
        || HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3202)) \
        )

#   define HBOOST_MPL_CFG_NO_HAS_APPLY

#endif

#endif // HBOOST_MPL_AUX_CONFIG_HAS_APPLY_HPP_INCLUDED
