//  hboost/system/config.hpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 2003, 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef HBOOST_SYSTEM_CONFIG_HPP                  
#define HBOOST_SYSTEM_CONFIG_HPP

#include <hboost/config.hpp>
#include <hboost/predef/platform.h>
#include <hboost/system/api_config.hpp>  // for HBOOST_POSIX_API or HBOOST_WINDOWS_API

// This header implements separate compilation features as described in
// http://www.boost.org/more/separate_compilation.html

//  normalize macros  ------------------------------------------------------------------//

#if !defined(HBOOST_SYSTEM_DYN_LINK) && !defined(HBOOST_SYSTEM_STATIC_LINK) \
  && !defined(HBOOST_ALL_DYN_LINK) && !defined(HBOOST_ALL_STATIC_LINK)
# define HBOOST_SYSTEM_STATIC_LINK
#endif

#if defined(HBOOST_ALL_DYN_LINK) && !defined(HBOOST_SYSTEM_DYN_LINK)
# define HBOOST_SYSTEM_DYN_LINK 
#elif defined(HBOOST_ALL_STATIC_LINK) && !defined(HBOOST_SYSTEM_STATIC_LINK)
# define HBOOST_SYSTEM_STATIC_LINK 
#endif

#if defined(HBOOST_SYSTEM_DYN_LINK) && defined(HBOOST_SYSTEM_STATIC_LINK)
# error Must not define both HBOOST_SYSTEM_DYN_LINK and HBOOST_SYSTEM_STATIC_LINK
#endif

//  enable dynamic or static linking as requested --------------------------------------//

#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_SYSTEM_DYN_LINK)
# if defined(HBOOST_SYSTEM_SOURCE)
#   define HBOOST_SYSTEM_DECL HBOOST_SYMBOL_EXPORT
# else 
#   define HBOOST_SYSTEM_DECL HBOOST_SYMBOL_IMPORT
# endif
#else
# define HBOOST_SYSTEM_DECL
#endif

//  enable automatic library variant selection  ----------------------------------------// 

#if !defined(HBOOST_SYSTEM_SOURCE) && !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_SYSTEM_NO_LIB)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define HBOOST_LIB_NAME hboost_system
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_SYSTEM_DYN_LINK)
#  define HBOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <hboost/config/auto_link.hpp>
#endif  // auto-linking disabled

#endif // HBOOST_SYSTEM_CONFIG_HPP

