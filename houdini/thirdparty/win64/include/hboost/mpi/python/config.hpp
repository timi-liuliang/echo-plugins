// Copyright (C) 2006 Douglas Gregor <doug.gregor -at- gmail.com>

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file config.hpp
 *
 *  This header provides auto-linking and configuration support for the 
 *  Boost.MPI python bindings.
 */
#ifndef HBOOST_MPI_PYTHON_CONFIG_HPP
#define HBOOST_MPI_PYTHON_CONFIG_HPP

#include <hboost/mpi/config.hpp>

/*****************************************************************************
 *                                                                           *
 *  DLL import/export options                                                *  
 *                                                                           *
 *****************************************************************************/

#if (defined(HBOOST_MPI_PYTHON_DYN_LINK) || defined(HBOOST_ALL_DYN_LINK)) && !defined(HBOOST_MPI_PYTHON_STATIC_LINK)
#  if defined(HBOOST_MPI_PYTHON_SOURCE)
#     define HBOOST_MPI_PYTHON_DECL HBOOST_SYMBOL_EXPORT
#     define HBOOST_MPI_PYTHON_BUILD_DLL
#  else
#     define HBOOST_MPI_PYTHON_DECL HBOOST_SYMBOL_IMPORT
#  endif
#endif

#ifndef HBOOST_MPI_PYTHON_DECL
#  define HBOOST_MPI_PYTHON_DECL
#endif

#if !defined(HBOOST_MPI_PYTHON_NO_LIB) && !defined(HBOOST_MPI_PYTHON_SOURCE) && !defined(HBOOST_ALL_NO_LIB)
#  define HBOOST_LIB_NAME hboost_mpi_python
#  if defined(HBOOST_MPI_PYTHON_DYN_LINK) || defined(HBOOST_ALL_DYN_LINK)
#     define HBOOST_DYN_LINK
#  endif
#  ifdef HBOOST_MPI_PYTHON_DIAG
#     define HBOOST_LIB_DIAGNOSTIC
#  endif
#  include <hboost/config/auto_link.hpp>
#endif

#endif // HBOOST_MPI_PYTHON_CONFIG_HPP
