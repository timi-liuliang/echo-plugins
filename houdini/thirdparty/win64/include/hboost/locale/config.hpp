//
//  Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef HBOOST_LOCALE_CONFIG_HPP_INCLUDED
#define HBOOST_LOCALE_CONFIG_HPP_INCLUDED

#include <hboost/locale/definitions.hpp>

//
// Automatically link to the correct build variant where possible. 
// 
#if !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_LOCALE_NO_LIB) && !defined(HBOOST_LOCALE_SOURCE)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define HBOOST_LIB_NAME hboost_locale
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_LOCALE_DYN_LINK)
#  define HBOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <hboost/config/auto_link.hpp>
#endif  // auto-linking disabled


#endif // hboost/locale/config.hpp
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

