
#ifndef HBOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED
#define HBOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2008
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#if !defined(HBOOST_MPL_CFG_COMPILER_DIR)

#   include <hboost/mpl/aux_/config/dtp.hpp>
#   include <hboost/mpl/aux_/config/ttp.hpp>
#   include <hboost/mpl/aux_/config/ctps.hpp>
#   include <hboost/mpl/aux_/config/msvc.hpp>
#   include <hboost/mpl/aux_/config/gcc.hpp>
#   include <hboost/mpl/aux_/config/workaround.hpp>

#   if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
#       define HBOOST_MPL_CFG_COMPILER_DIR msvc60

#   elif HBOOST_WORKAROUND(HBOOST_MSVC, == 1300)
#       define HBOOST_MPL_CFG_COMPILER_DIR msvc70

#   elif HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, HBOOST_TESTED_AT(0x0304))
#       define HBOOST_MPL_CFG_COMPILER_DIR gcc

#   elif HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610))
#       if !defined(HBOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#           define HBOOST_MPL_CFG_COMPILER_DIR bcc551
#       elif HBOOST_WORKAROUND(__BORLANDC__, >= 0x590)
#           define HBOOST_MPL_CFG_COMPILER_DIR bcc
#       else
#           define HBOOST_MPL_CFG_COMPILER_DIR bcc_pre590
#       endif

#   elif HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840))
#       define HBOOST_MPL_CFG_COMPILER_DIR dmc

#   elif defined(__MWERKS__)
#       if defined(HBOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#           define HBOOST_MPL_CFG_COMPILER_DIR mwcw
#       else
#           define HBOOST_MPL_CFG_COMPILER_DIR plain
#       endif

#   elif defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#       define HBOOST_MPL_CFG_COMPILER_DIR no_ctps

#   elif defined(HBOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS)
#       define HBOOST_MPL_CFG_COMPILER_DIR no_ttp

#   else
#       define HBOOST_MPL_CFG_COMPILER_DIR plain
#   endif

#endif // HBOOST_MPL_CFG_COMPILER_DIR

#endif // HBOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED
