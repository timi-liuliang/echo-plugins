/* boost random auto_link.hpp header file
 *
 * Copyright Steven Watanabe 2010
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef HBOOST_RANDOM_DETAIL_AUTO_LINK_HPP
#define HBOOST_RANDOM_DETAIL_AUTO_LINK_HPP

#include <hboost/config.hpp>

#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_RANDOM_DYN_LINK)
    #if defined(HBOOST_RANDOM_SOURCE)
        #define HBOOST_RANDOM_DECL HBOOST_SYMBOL_EXPORT
    #else
        #define HBOOST_RANDOM_DECL HBOOST_SYMBOL_IMPORT
    #endif
#endif

#ifndef HBOOST_RANDOM_DECL
    #define HBOOST_RANDOM_DECL
#endif

#if !defined(HBOOST_RANDOM_NO_LIB) && !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_RANDOM_SOURCE)

#define HBOOST_LIB_NAME hboost_random

#if defined(HBOOST_RANDOM_DYN_LINK) || defined(HBOOST_ALL_DYN_LINK)
    #define HBOOST_DYN_LINK
#endif

#include <hboost/config/auto_link.hpp>

#endif

#endif
