/*=============================================================================
    Copyright (c) 2005-2008 Hartmut Kaiser
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2014-2015 John Fletcher

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_VERSION_HPP
#define HBOOST_PHOENIX_VERSION_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  This is the version of the library
//
///////////////////////////////////////////////////////////////////////////////
#define HBOOST_PHOENIX_VERSION   0x3200    // 3.2.0

// hboost/predef is not in Boost before 1.55.0
#include <hboost/version.hpp>
#if HBOOST_VERSION >= 105500
// Also note that it has a nonstandard header which could change,
// so I have not relied on its own internal define.
#include <hboost/predef.h>
#define HBOOST_PHOENIX_HAVE_HBOOST_PREDEF
#endif

#ifdef HBOOST_PHOENIX_HAVE_HBOOST_PREDEF
#define HBOOST_PHOENIX_VERSION_NUMBER = HBOOST_VERSION_NUMBER(3,2,0)
#endif

#endif
