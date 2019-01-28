//    Copyright (c) 2004 Hartmut Kaiser
//
//    Use, modification and distribution is subject to the Boost Software
//    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROGRAM_OPTIONS_CONFIG_HK_2004_01_11
#define HBOOST_PROGRAM_OPTIONS_CONFIG_HK_2004_01_11

#include <hboost/config.hpp>
#include <hboost/version.hpp>

// Support for autolinking.
#if HBOOST_VERSION >= 103100   // works beginning from Boost V1.31.0

///////////////////////////////////////////////////////////////////////////////
// enable automatic library variant selection 
#if !defined(HBOOST_PROGRAM_OPTIONS_SOURCE) && !defined(HBOOST_ALL_NO_LIB) && \
    !defined(HBOOST_PROGRAM_OPTIONS_NO_LIB)

// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
#define HBOOST_LIB_NAME hboost_program_options
// tell the auto-link code to select a dll when required:
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_PROGRAM_OPTIONS_DYN_LINK)
#  define HBOOST_DYN_LINK
#endif

// And include the header that does the work:
#include <hboost/config/auto_link.hpp>

#endif  // auto-linking disabled

#endif  // HBOOST_VERSION

///////////////////////////////////////////////////////////////////////////////
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_PROGRAM_OPTIONS_DYN_LINK)
// export if this is our own source, otherwise import:
#ifdef HBOOST_PROGRAM_OPTIONS_SOURCE
# define HBOOST_PROGRAM_OPTIONS_DECL HBOOST_SYMBOL_EXPORT
#else
# define HBOOST_PROGRAM_OPTIONS_DECL HBOOST_SYMBOL_IMPORT
#endif  // HBOOST_PROGRAM_OPTIONS_SOURCE
#endif  // DYN_LINK

#ifndef HBOOST_PROGRAM_OPTIONS_DECL
#define HBOOST_PROGRAM_OPTIONS_DECL
#endif


#endif // PROGRAM_OPTIONS_CONFIG_HK_2004_01_11

