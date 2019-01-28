// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_SEEK_HPP_INCLUDED
#define HBOOST_IOSTREAMS_SEEK_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <hboost/config.hpp>  // DEDUCED_TYPENAME, MSVC.
#include <hboost/integer_traits.hpp>
#include <hboost/iostreams/categories.hpp>
#include <hboost/iostreams/detail/dispatch.hpp>
#include <hboost/iostreams/detail/ios.hpp>       // streamsize, seekdir, openmode.
#include <hboost/iostreams/detail/streambuf.hpp>
#include <hboost/iostreams/detail/wrap_unwrap.hpp>
#include <hboost/iostreams/operations_fwd.hpp>
#include <hboost/iostreams/positioning.hpp>
#include <hboost/mpl/if.hpp>

// Must come last.
#include <hboost/iostreams/detail/config/disable_warnings.hpp>

namespace hboost { namespace iostreams {

namespace detail {

template<typename T>
struct seek_device_impl;

template<typename T>
struct seek_filter_impl;

} // End namespace detail.

template<typename T>
inline std::streampos
seek( T& t, stream_offset off, HBOOST_IOS::seekdir way,
      HBOOST_IOS::openmode which = HBOOST_IOS::in | HBOOST_IOS::out )
{ 
    using namespace detail;
    return seek_device_impl<T>::seek(detail::unwrap(t), off, way, which); 
}

template<typename T, typename Device>
inline std::streampos
seek( T& t, Device& dev, stream_offset off, HBOOST_IOS::seekdir way,
      HBOOST_IOS::openmode which = HBOOST_IOS::in | HBOOST_IOS::out )
{ 
    using namespace detail;
    return seek_filter_impl<T>::seek(detail::unwrap(t), dev, off, way, which);
}

namespace detail {

//------------------Definition of seek_device_impl----------------------------//

template<typename T>
struct seek_device_impl
    : mpl::if_<
          is_custom<T>,
          operations<T>,
          seek_device_impl<
              HBOOST_DEDUCED_TYPENAME
              dispatch<
                  T, iostream_tag, istream_tag, ostream_tag,
                  streambuf_tag, two_head, any_tag
              >::type
          >
      >::type
    { };

struct seek_impl_basic_ios {
    template<typename T>
    static std::streampos seek( T& t, stream_offset off,
                                HBOOST_IOS::seekdir way,
                                HBOOST_IOS::openmode which )
    {
        if ( way == HBOOST_IOS::beg &&
             ( off < integer_traits<std::streamoff>::const_min ||
               off > integer_traits<std::streamoff>::const_max ) )
        {
            return t.rdbuf()->pubseekpos(offset_to_position(off));
        } else {
            return t.rdbuf()->pubseekoff(off, way, which);
        }
    }
};

template<>
struct seek_device_impl<iostream_tag> : seek_impl_basic_ios { };

template<>
struct seek_device_impl<istream_tag> : seek_impl_basic_ios { };

template<>
struct seek_device_impl<ostream_tag> : seek_impl_basic_ios { };

template<>
struct seek_device_impl<streambuf_tag> {
    template<typename T>
    static std::streampos seek( T& t, stream_offset off,
                                HBOOST_IOS::seekdir way,
                                HBOOST_IOS::openmode which )
    {
        if ( way == HBOOST_IOS::beg &&
             ( off < integer_traits<std::streamoff>::const_min ||
               off > integer_traits<std::streamoff>::const_max ) )
        {
            return t.HBOOST_IOSTREAMS_PUBSEEKPOS(offset_to_position(off));
        } else {
            return t.HBOOST_IOSTREAMS_PUBSEEKOFF(off, way, which);
        }
    }
};

template<>
struct seek_device_impl<two_head> {
    template<typename T>
    static std::streampos seek( T& t, stream_offset off,
                                HBOOST_IOS::seekdir way,
                                HBOOST_IOS::openmode which )
    { return t.seek(off, way, which); }
};

template<>
struct seek_device_impl<any_tag> {
    template<typename T>
    static std::streampos seek( T& t, stream_offset off,
                                HBOOST_IOS::seekdir way,
                                HBOOST_IOS::openmode )
    { return t.seek(off, way); }
};

//------------------Definition of seek_filter_impl----------------------------//

template<typename T>
struct seek_filter_impl
    : mpl::if_<
          is_custom<T>,
          operations<T>,
          seek_filter_impl<
              HBOOST_DEDUCED_TYPENAME
              dispatch<T, two_head, any_tag>::type
          >
      >::type
    { };

template<>
struct seek_filter_impl<two_head> {
    template<typename T, typename Device>
    static std::streampos seek( T& t, Device& d,
                                stream_offset off,
                                HBOOST_IOS::seekdir way,
                                HBOOST_IOS::openmode which )
    { return t.seek(d, off, way, which); }
};

template<>
struct seek_filter_impl<any_tag> {
    template<typename T, typename Device>
    static std::streampos seek( T& t, Device& d,
                                stream_offset off,
                                HBOOST_IOS::seekdir way,
                                HBOOST_IOS::openmode )
    { return t.seek(d, off, way); }
};

} // End namespace detail.

} } // End namespaces iostreams, boost.

#include <hboost/iostreams/detail/config/enable_warnings.hpp>

#endif // #ifndef HBOOST_IOSTREAMS_SEEK_HPP_INCLUDED