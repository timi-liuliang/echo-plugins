
#ifndef HBOOST_MPL_AUX_CONFIG_BIND_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_BIND_HPP_INCLUDED

// Copyright David Abrahams 2002
// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if    !defined(HBOOST_MPL_CFG_NO_BIND_TEMPLATE) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE) \
    && (   HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) \
        || HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) \
        )

#   define HBOOST_MPL_CFG_NO_BIND_TEMPLATE

#endif

//#define HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT

#endif // HBOOST_MPL_AUX_CONFIG_BIND_HPP_INCLUDED
