
#ifndef HBOOST_MPL_INTEGRAL_C_TAG_HPP_INCLUDED
#define HBOOST_MPL_INTEGRAL_C_TAG_HPP_INCLUDED

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


#include <hboost/mpl/aux_/adl_barrier.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
struct integral_c_tag { HBOOST_STATIC_CONSTANT(int, value = 0); };
HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
HBOOST_MPL_AUX_ADL_BARRIER_DECL(integral_c_tag)

#endif // HBOOST_MPL_INTEGRAL_C_TAG_HPP_INCLUDED
