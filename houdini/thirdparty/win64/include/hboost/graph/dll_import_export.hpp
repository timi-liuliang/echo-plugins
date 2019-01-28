//=======================================================================
// Copyright 2001 University of Notre Dame.
// Copyright 2003 Jeremy Siek
// Authors: Lie-Quan Lee, Jeremy Siek, and Douglas Gregor
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef HBOOST_GRAPH_DLL_IMPORT_EXPORT_HPP
#define HBOOST_GRAPH_DLL_IMPORT_EXPORT_HPP

#include <hboost/config.hpp>

#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_GRAPH_DYN_LINK)
#  ifdef HBOOST_GRAPH_SOURCE
#    define HBOOST_GRAPH_DECL HBOOST_SYMBOL_EXPORT
#  else
#    define HBOOST_GRAPH_DECL HBOOST_SYMBOL_IMPORT
#  endif  // HBOOST_GRAPH_SOURCE
#endif  // DYN_LINK

#ifndef HBOOST_GRAPH_DECL
#  define HBOOST_GRAPH_DECL
#endif

#endif // HBOOST_GRAPH_DLL_IMPORT_EXPORT_HPP
