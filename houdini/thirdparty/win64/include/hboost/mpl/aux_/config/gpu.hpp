
#ifndef HBOOST_MPL_AUX_CONFIG_GPU_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_GPU_HPP_INCLUDED

// Copyright Eric Niebler 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/config.hpp>

#if !defined(HBOOST_MPL_CFG_GPU_ENABLED) \

#   define HBOOST_MPL_CFG_GPU_ENABLED HBOOST_GPU_ENABLED

#endif

#if defined __CUDACC__

#    define HBOOST_MPL_CFG_GPU 1

#else

#    define HBOOST_MPL_CFG_GPU 0

#endif

#endif // HBOOST_MPL_AUX_CONFIG_GPU_HPP_INCLUDED
