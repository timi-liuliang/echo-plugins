//  hboost/timer/config.hpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 2003, 2006, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/timer for documentation.

#ifndef HBOOST_TIMER_CONFIG_HPP
#define HBOOST_TIMER_CONFIG_HPP

#include <hboost/config.hpp>

#include <hboost/system/api_config.hpp> 

// This header implements separate compilation features as described in
// http://www.boost.org/more/separate_compilation.html

//  enable dynamic or static linking as requested --------------------------------------//

#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_TIMER_DYN_LINK)
# if defined(HBOOST_TIMER_SOURCE)
#   define HBOOST_TIMER_DECL HBOOST_SYMBOL_EXPORT
# else
#   define HBOOST_TIMER_DECL HBOOST_SYMBOL_IMPORT
# endif
#else
# define HBOOST_TIMER_DECL
#endif

//  enable automatic library variant selection  ----------------------------------------//

#if !defined(HBOOST_TIMER_SOURCE) && !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_TIMER_NO_LIB)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define HBOOST_LIB_NAME hboost_timer
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_TIMER_DYN_LINK)
#  define HBOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <hboost/config/auto_link.hpp>
#endif  // auto-linking disabled

#endif // HBOOST_TIMER_CONFIG_HPP

