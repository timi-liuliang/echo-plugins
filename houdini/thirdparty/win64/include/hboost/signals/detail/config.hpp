/*
 *
 * Copyright (c) 1998-2002
 * John Maddock
 *
 * Copyright (c) 2003-2004
 * Douglas Gregor
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef HBOOST_SIGNALS_CONFIG_HPP
#define HBOOST_SIGNALS_CONFIG_HPP

#include <hboost/config.hpp>

#ifdef HBOOST_HAS_DECLSPEC
#  if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_SIGNALS_DYN_LINK)
#    ifdef HBOOST_SIGNALS_SOURCE
#      define HBOOST_SIGNALS_DECL __declspec(dllexport)
#    else
#      define HBOOST_SIGNALS_DECL __declspec(dllimport)
#    endif  // HBOOST_SIGNALS_SOURCE
#  endif  // DYN_LINK
#endif  // HBOOST_HAS_DECLSPEC

#ifndef HBOOST_SIGNALS_DECL
#  define HBOOST_SIGNALS_DECL
#endif

// Setup autolinking
#if !defined(HBOOST_SIGNALS_SOURCE) && !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_SIGNALS_NO_LIB)
#  define HBOOST_LIB_NAME hboost_signals

#  if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_SIGNALS_DYN_LINK)
#    define HBOOST_DYN_LINK
#  endif

#  include <hboost/config/auto_link.hpp>
#endif // autolinking on

#endif // HBOOST_SIGNALS_CONFIG_HPP









