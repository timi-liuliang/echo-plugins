// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2005-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_NEWLINE_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_NEWLINE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace hboost { namespace iostreams { namespace detail {

template<typename Ch>
struct newline;

template<>
struct newline<char> {
    HBOOST_STATIC_CONSTANT(char, value = '\n');
};

template<>
struct newline<wchar_t> {
    HBOOST_STATIC_CONSTANT(wchar_t, value = L'\n');
};

} } } // End namespaces detaill, iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_NEWLINE_HPP_INCLUDED