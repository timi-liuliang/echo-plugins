
#ifndef HBOOST_MPL_AUX_CONFIG_BCC_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_BCC_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2008
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2004-09-02 10:41:37 -0500 (Thu, 02 Sep 2004) $
// $Revision: 24874 $

#include <hboost/mpl/aux_/config/workaround.hpp>

#if    !defined(HBOOST_MPL_CFG_BCC590_WORKAROUNDS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE) \
    && HBOOST_WORKAROUND(__BORLANDC__, >= 0x590) \
    && HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610))

#   define HBOOST_MPL_CFG_BCC590_WORKAROUNDS

#endif

#endif // HBOOST_MPL_AUX_CONFIG_BCC_HPP_INCLUDED
