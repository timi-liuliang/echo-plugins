// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2004-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// Inspired by Daryle Walker's nullbuf from his More I/O submission.

#ifndef HBOOST_IOSTREAMS_NULL_HPP_INCLUDED
#define HBOOST_IOSTREAMS_NULL_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <hboost/iostreams/categories.hpp>
#include <hboost/iostreams/detail/ios.hpp> // openmode, streamsize.
#include <hboost/iostreams/positioning.hpp>

namespace hboost { namespace iostreams {

template<typename Ch, typename Mode>
class basic_null_device {
public:
    typedef Ch char_type;
    struct category
        : public Mode,
          public device_tag,
          public closable_tag
        { };
    std::streamsize read(Ch*, std::streamsize) { return 0; }
    std::streamsize write(const Ch*, std::streamsize n) { return n; }
    std::streampos seek( stream_offset, HBOOST_IOS::seekdir,
                         HBOOST_IOS::openmode = 
                             HBOOST_IOS::in | HBOOST_IOS::out ) 
    { return -1; }
    void close() { }
    void close(HBOOST_IOS::openmode) { }
};

template<typename Ch>
struct basic_null_source : private basic_null_device<Ch, input> {
    typedef Ch          char_type;
    typedef source_tag  category;
    using basic_null_device<Ch, input>::read;
    using basic_null_device<Ch, input>::close;
};

typedef basic_null_source<char>     null_source;
typedef basic_null_source<wchar_t>  wnull_source;

template<typename Ch>
struct basic_null_sink : private basic_null_device<Ch, output> {
    typedef Ch        char_type;
    typedef sink_tag  category;
    using basic_null_device<Ch, output>::write;
    using basic_null_device<Ch, output>::close;
};

typedef basic_null_sink<char>     null_sink;
typedef basic_null_sink<wchar_t>  wnull_sink;

} } // End namespaces iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_NULL_HPP_INCLUDED
