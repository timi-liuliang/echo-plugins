
#ifndef HBOOST_MPL_AUX_NA_ASSERT_HPP_INCLUDED
#define HBOOST_MPL_AUX_NA_ASSERT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/na.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if !HBOOST_WORKAROUND(_MSC_FULL_VER, <= 140050601)    \
    && !HBOOST_WORKAROUND(__EDG_VERSION__, <= 243)
#   include <hboost/mpl/assert.hpp>
#   define HBOOST_MPL_AUX_ASSERT_NOT_NA(x) \
    HBOOST_MPL_ASSERT_NOT((hboost::mpl::is_na<type>)) \
/**/
#else
#   include <hboost/static_assert.hpp>
#   define HBOOST_MPL_AUX_ASSERT_NOT_NA(x) \
    HBOOST_STATIC_ASSERT(!hboost::mpl::is_na<x>::value) \
/**/
#endif

#endif // HBOOST_MPL_AUX_NA_ASSERT_HPP_INCLUDED
