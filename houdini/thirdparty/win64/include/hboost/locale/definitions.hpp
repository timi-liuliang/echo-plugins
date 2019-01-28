//
//  Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef HBOOST_LOCALE_DEFINITIONS_HPP_INCLUDED
#define HBOOST_LOCALE_DEFINITIONS_HPP_INCLUDED

#include <hboost/config.hpp>

// Support older ICU versions
#ifndef HBOOST_SYMBOL_VISIBLE
# define HBOOST_SYMBOL_VISIBLE
#endif

#ifdef HBOOST_HAS_DECLSPEC 
#   if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_LOCALE_DYN_LINK)
#       ifdef HBOOST_LOCALE_SOURCE
#           define HBOOST_LOCALE_DECL HBOOST_SYMBOL_EXPORT
#       else
#           define HBOOST_LOCALE_DECL HBOOST_SYMBOL_IMPORT
#       endif  // HBOOST_LOCALE_SOURCE
#   endif  // DYN_LINK
#endif  // HBOOST_HAS_DECLSPEC

#ifndef HBOOST_LOCALE_DECL
#   define HBOOST_LOCALE_DECL
#endif

#endif // hboost/locale/config.hpp
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

