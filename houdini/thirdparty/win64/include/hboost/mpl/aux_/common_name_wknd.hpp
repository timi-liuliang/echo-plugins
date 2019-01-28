
#ifndef HBOOST_MPL_AUX_COMMON_NAME_WKND_HPP_INCLUDED
#define HBOOST_MPL_AUX_COMMON_NAME_WKND_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
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

#if HBOOST_WORKAROUND(__BORLANDC__, < 0x561)
// agurt, 12/nov/02: to suppress the bogus "Cannot have both a template class 
// and function named 'xxx'" diagnostic
#   define HBOOST_MPL_AUX_COMMON_NAME_WKND(name) \
namespace name_##wknd { \
template< typename > void name(); \
} \
/**/

#else

#   define HBOOST_MPL_AUX_COMMON_NAME_WKND(name) /**/

#endif // __BORLANDC__

#endif // HBOOST_MPL_AUX_COMMON_NAME_WKND_HPP_INCLUDED
