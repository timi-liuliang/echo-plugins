
#ifndef HBOOST_MPL_AUX_CONFIG_ARRAYS_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_ARRAYS_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if    !defined(HBOOST_MPL_CFG_NO_DEPENDENT_ARRAY_TYPES) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE) \
    && ( HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) \
        || HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) \
        )

#   define HBOOST_MPL_CFG_NO_DEPENDENT_ARRAY_TYPES

#endif

#endif // HBOOST_MPL_AUX_CONFIG_ARRAYS_HPP_INCLUDED
