//-----------------------------------------------------------------------------
// boost variant/detail/config.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_CONFIG_HPP
#define HBOOST_VARIANT_DETAIL_CONFIG_HPP

#include "hboost/config.hpp"
#include "hboost/detail/workaround.hpp"

///////////////////////////////////////////////////////////////////////////////
// macro HBOOST_VARIANT_AUX_BROKEN_CONSTRUCTOR_TEMPLATE_ORDERING
//
#if HBOOST_WORKAROUND(__MWERKS__, <= 0x3201) \
 || HBOOST_WORKAROUND(HBOOST_INTEL, <= 700) \
 && !defined(HBOOST_VARIANT_AUX_BROKEN_CONSTRUCTOR_TEMPLATE_ORDERING)
#   define HBOOST_VARIANT_AUX_BROKEN_CONSTRUCTOR_TEMPLATE_ORDERING
#endif

///////////////////////////////////////////////////////////////////////////////
// macro HBOOST_VARIANT_AUX_HAS_CONSTRUCTOR_TEMPLATE_ORDERING_SFINAE_WKND
//
#if !defined(HBOOST_NO_SFINAE) \
 && !HBOOST_WORKAROUND(HBOOST_INTEL, <= 700) \
 && !defined(HBOOST_VARIANT_AUX_HAS_CONSTRUCTOR_TEMPLATE_ORDERING_SFINAE_WKND)
#   define HBOOST_VARIANT_AUX_HAS_CONSTRUCTOR_TEMPLATE_ORDERING_SFINAE_WKND
#endif

#endif // HBOOST_VARIANT_DETAIL_CONFIG_HPP
