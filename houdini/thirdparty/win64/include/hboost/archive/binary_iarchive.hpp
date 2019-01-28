#ifndef HBOOST_ARCHIVE_BINARY_IARCHIVE_HPP
#define HBOOST_ARCHIVE_BINARY_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// binary_iarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <istream>
#include <hboost/archive/binary_iarchive_impl.hpp>
#include <hboost/archive/detail/register_archive.hpp>

#ifdef HBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace hboost {
namespace archive {

// do not derive from this class.  If you want to extend this functionality
// via inhertance, derived from binary_iarchive_impl instead.  This will
// preserve correct static polymorphism.
class HBOOST_SYMBOL_VISIBLE binary_iarchive :
    public binary_iarchive_impl<
        hboost::archive::binary_iarchive, 
        std::istream::char_type, 
        std::istream::traits_type
    >{
public:
    binary_iarchive(std::istream & is, unsigned int flags = 0) :
        binary_iarchive_impl<
            binary_iarchive, std::istream::char_type, std::istream::traits_type
        >(is, flags)
    {}
    binary_iarchive(std::streambuf & bsb, unsigned int flags = 0) :
        binary_iarchive_impl<
            binary_iarchive, std::istream::char_type, std::istream::traits_type
        >(bsb, flags)
    {}
};

} // namespace archive
} // namespace hboost

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::archive::binary_iarchive)
HBOOST_SERIALIZATION_USE_ARRAY_OPTIMIZATION(hboost::archive::binary_iarchive)

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#endif // HBOOST_ARCHIVE_BINARY_IARCHIVE_HPP
