
#ifndef HBOOST_MPL_AUX_CONFIG_TTP_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_TTP_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/gcc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if !defined(HBOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS) \
    && ( defined(HBOOST_NO_TEMPLATE_TEMPLATES) \
      || HBOOST_WORKAROUND( __BORLANDC__, HBOOST_TESTED_AT( 0x590) ) \
       )

#   define HBOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS

#endif


#if    !defined(HBOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE) \
    && (   HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, HBOOST_TESTED_AT(0x0302)) \
        || HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) \
        )

#   define HBOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING

#endif

#endif // HBOOST_MPL_AUX_CONFIG_TTP_HPP_INCLUDED
