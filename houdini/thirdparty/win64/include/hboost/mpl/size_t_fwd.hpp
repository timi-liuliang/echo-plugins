
#ifndef HBOOST_MPL_SIZE_T_FWD_HPP_INCLUDED
#define HBOOST_MPL_SIZE_T_FWD_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
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
#include <hboost/config.hpp> // make sure 'size_t' is placed into 'std'
#include <cstddef>

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

template< std::size_t N > struct size_t;

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
HBOOST_MPL_AUX_ADL_BARRIER_DECL(size_t)

#endif // HBOOST_MPL_SIZE_T_FWD_HPP_INCLUDED
