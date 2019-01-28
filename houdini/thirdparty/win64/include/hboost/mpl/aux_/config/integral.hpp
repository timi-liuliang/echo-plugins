
#ifndef HBOOST_MPL_AUX_CONFIG_INTEGRAL_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_INTEGRAL_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if    !defined(HBOOST_MPL_CFG_BCC_INTEGRAL_CONSTANTS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE) \
    && HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610))

#   define HBOOST_MPL_CFG_BCC_INTEGRAL_CONSTANTS

#endif

#if    !defined(HBOOST_MPL_CFG_NO_NESTED_VALUE_ARITHMETIC) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE) \
    && ( HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) \
        || HBOOST_WORKAROUND(__EDG_VERSION__, <= 238) \
        )

#   define HBOOST_MPL_CFG_NO_NESTED_VALUE_ARITHMETIC

#endif

#endif // HBOOST_MPL_AUX_CONFIG_INTEGRAL_HPP_INCLUDED
