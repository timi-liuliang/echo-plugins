
#ifndef HBOOST_MPL_AUX_CONFIG_DMC_AMBIGUOUS_CTPS_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_DMC_AMBIGUOUS_CTPS_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/workaround.hpp>

#if    !defined(HBOOST_MPL_CFG_DMC_AMBIGUOUS_CTPS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE) \
    && HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840))

#   define HBOOST_MPL_CFG_DMC_AMBIGUOUS_CTPS

#endif

#endif // HBOOST_MPL_AUX_CONFIG_DMC_AMBIGUOUS_CTPS_HPP_INCLUDED
