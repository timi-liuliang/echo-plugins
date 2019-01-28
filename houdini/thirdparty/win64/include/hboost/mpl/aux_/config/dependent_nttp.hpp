
#ifndef HBOOST_MPL_AUX_CONFIG_DEPENDENT_NTTP_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_DEPENDENT_NTTP_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/gcc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

// GCC and EDG-based compilers incorrectly reject the following code:
//   template< typename T, T n > struct a;
//   template< typename T > struct b;
//   template< typename T, T n > struct b< a<T,n> > {};

#if    !defined(HBOOST_MPL_CFG_NO_DEPENDENT_NONTYPE_PARAMETER_IN_PARTIAL_SPEC) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE) \
    && (   HBOOST_WORKAROUND(__EDG_VERSION__, HBOOST_TESTED_AT(300)) \
        || HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, HBOOST_TESTED_AT(0x0302)) \
        )

#   define HBOOST_MPL_CFG_NO_DEPENDENT_NONTYPE_PARAMETER_IN_PARTIAL_SPEC

#endif

#endif // HBOOST_MPL_AUX_CONFIG_DEPENDENT_NTTP_HPP_INCLUDED
