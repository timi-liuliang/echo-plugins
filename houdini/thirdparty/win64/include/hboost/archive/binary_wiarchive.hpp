#ifndef HBOOST_ARCHIVE_BINARY_WIARCHIVE_HPP
#define HBOOST_ARCHIVE_BINARY_WIARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// binary_wiarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <hboost/config.hpp>
#ifdef HBOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <istream> // wistream
#include <hboost/archive/binary_iarchive_impl.hpp>
#include <hboost/archive/detail/register_archive.hpp>

namespace hboost { 
namespace archive {

class binary_wiarchive : 
    public binary_iarchive_impl<
        binary_wiarchive, std::wistream::char_type, std::wistream::traits_type
    >
{
public:
    binary_wiarchive(std::wistream & is, unsigned int flags = 0) :
        binary_iarchive_impl<
            binary_wiarchive, std::wistream::char_type, std::wistream::traits_type
        >(is, flags)
    {}
    binary_wiarchive(std::wstreambuf & bsb, unsigned int flags = 0) :
        binary_iarchive_impl<
            binary_wiarchive, std::wistream::char_type, std::wistream::traits_type
        >(bsb, flags)
    {}
};

} // namespace archive
} // namespace hboost

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::archive::binary_wiarchive)

#endif // HBOOST_NO_STD_WSTREAMBUF
#endif // HBOOST_ARCHIVE_BINARY_WIARCHIVE_HPP
