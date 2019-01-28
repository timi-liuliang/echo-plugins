
#ifndef HBOOST_MPL_AUX_CONFIG_PREPROCESSOR_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_PREPROCESSOR_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/workaround.hpp>

#if !defined(HBOOST_MPL_CFG_BROKEN_PP_MACRO_EXPANSION) \
    && (   HBOOST_WORKAROUND(__MWERKS__, <= 0x3003) \
        || HBOOST_WORKAROUND(__BORLANDC__, < 0x582) \
        || HBOOST_WORKAROUND(__IBMCPP__, HBOOST_TESTED_AT(502)) \
        )

#   define HBOOST_MPL_CFG_BROKEN_PP_MACRO_EXPANSION

#endif

#if !defined(HBOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES)
#   define HBOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES
#endif

#if !defined(HBOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING) \
    && HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840))
#   define HBOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING
#endif


#endif // HBOOST_MPL_AUX_CONFIG_PREPROCESSOR_HPP_INCLUDED
