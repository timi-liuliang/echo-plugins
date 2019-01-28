#ifndef HBOOST_ARCHIVE_XML_ARCHIVE_EXCEPTION_HPP
#define HBOOST_ARCHIVE_XML_ARCHIVE_EXCEPTION_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// xml_archive_exception.hpp:

// (C) Copyright 2007 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <exception>
#include <hboost/assert.hpp>

#include <hboost/config.hpp> 
#include <hboost/archive/detail/decl.hpp>
#include <hboost/archive/archive_exception.hpp>

#include <hboost/archive/detail/abi_prefix.hpp> // must be the last header

namespace hboost {
namespace archive {

//////////////////////////////////////////////////////////////////////
// exceptions thrown by xml archives
//
class HBOOST_SYMBOL_VISIBLE xml_archive_exception : 
    public virtual hboost::archive::archive_exception
{
public:
    typedef enum {
        xml_archive_parsing_error,    // see save_register
        xml_archive_tag_mismatch,
        xml_archive_tag_name_error
    } exception_code;
    HBOOST_ARCHIVE_DECL xml_archive_exception(
        exception_code c, 
        const char * e1 = NULL,
        const char * e2 = NULL
    );
    HBOOST_ARCHIVE_DECL xml_archive_exception(xml_archive_exception const &) ;
    virtual HBOOST_ARCHIVE_DECL ~xml_archive_exception() HBOOST_NOEXCEPT_OR_NOTHROW ;
};

}// namespace archive
}// namespace hboost

#include <hboost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif //HBOOST_XML_ARCHIVE_ARCHIVE_EXCEPTION_HPP
