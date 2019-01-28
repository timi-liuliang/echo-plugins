
#ifndef HBOOST_MPL_AUX_CONFIG_OPERATORS_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_OPERATORS_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/gcc.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if !defined(HBOOST_MPL_CFG_USE_OPERATORS_OVERLOADING) \
    && ( HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) \
        || HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) \
        || HBOOST_WORKAROUND(__EDG_VERSION__, <= 245) \
        || HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, <= 0x0295) \
        || HBOOST_WORKAROUND(__IBMCPP__, HBOOST_TESTED_AT(600)) \
        || HBOOST_WORKAROUND(__NVCC__, HBOOST_TESTED_AT(1)) \
        )

#   define HBOOST_MPL_CFG_USE_OPERATORS_OVERLOADING

#endif

#endif // HBOOST_MPL_AUX_CONFIG_OPERATORS_HPP_INCLUDED
