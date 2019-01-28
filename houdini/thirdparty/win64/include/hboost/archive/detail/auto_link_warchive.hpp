#ifndef HBOOST_ARCHIVE_DETAIL_AUTO_LINK_WARCHIVE_HPP
#define HBOOST_ARCHIVE_DETAIL_AUTO_LINK_WARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
//  auto_link_warchive.hpp
//
//  (c) Copyright Robert Ramey 2004
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/serialization

//----------------------------------------------------------------------------// 

// This header implements separate compilation features as described in
// http://www.boost.org/more/separate_compilation.html

//  enable automatic library variant selection  ------------------------------// 

#include <hboost/archive/detail/decl.hpp>

#if !defined(HBOOST_WARCHIVE_SOURCE) \
&& !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_SERIALIZATION_NO_LIB)

// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define HBOOST_LIB_NAME hboost_wserialization
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_SERIALIZATION_DYN_LINK)
#  define HBOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <hboost/config/auto_link.hpp>
#endif  // auto-linking disabled

#endif // ARCHIVE_DETAIL_AUTO_LINK_ARCHIVE_HPP
