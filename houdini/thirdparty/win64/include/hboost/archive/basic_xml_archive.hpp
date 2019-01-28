#ifndef HBOOST_ARCHIVE_BASIC_XML_TEXT_ARCHIVE_HPP
#define HBOOST_ARCHIVE_BASIC_XML_TEXT_ARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_xml_archive.hpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <hboost/archive/archive_exception.hpp>

#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/detail/abi_prefix.hpp> // must be the last header

namespace hboost { 
namespace archive {

// constant strings used in xml i/o

extern 
HBOOST_ARCHIVE_DECL const char *
HBOOST_ARCHIVE_XML_OBJECT_ID();

extern 
HBOOST_ARCHIVE_DECL const char *
HBOOST_ARCHIVE_XML_OBJECT_REFERENCE();

extern 
HBOOST_ARCHIVE_DECL const char *
HBOOST_ARCHIVE_XML_CLASS_ID();

extern 
HBOOST_ARCHIVE_DECL const char *
HBOOST_ARCHIVE_XML_CLASS_ID_REFERENCE();

extern 
HBOOST_ARCHIVE_DECL const char *
HBOOST_ARCHIVE_XML_CLASS_NAME();

extern 
HBOOST_ARCHIVE_DECL const char *
HBOOST_ARCHIVE_XML_TRACKING();

extern 
HBOOST_ARCHIVE_DECL const char *
HBOOST_ARCHIVE_XML_VERSION();

extern 
HBOOST_ARCHIVE_DECL const char *
HBOOST_ARCHIVE_XML_SIGNATURE();

}// namespace archive
}// namespace hboost

#include <hboost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // HBOOST_ARCHIVE_BASIC_XML_TEXT_ARCHIVE_HPP

