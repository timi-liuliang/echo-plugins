// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// To do: handle bidirection streams and output-seekable components.

#ifndef HBOOST_IOSTREAMS_SKIP_HPP_INCLUDED
#define HBOOST_IOSTREAMS_SKIP_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <hboost/iostreams/char_traits.hpp>
#include <hboost/iostreams/detail/ios.hpp>  // failure.
#include <hboost/iostreams/operations.hpp>
#include <hboost/iostreams/seek.hpp>
#include <hboost/iostreams/traits.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/or.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/type_traits/is_convertible.hpp>

namespace hboost { namespace iostreams {

namespace detail {

template<typename Device>
void skip(Device& dev, stream_offset off, mpl::true_)
{ iostreams::seek(dev, off, HBOOST_IOS::cur); }

template<typename Device>
void skip(Device& dev, stream_offset off, mpl::false_)
{   // gcc 2.95 needs namespace qualification for char_traits.
    typedef typename char_type_of<Device>::type  char_type;
    typedef iostreams::char_traits<char_type>    traits_type;
    for (stream_offset z = 0; z < off; ) {
        typename traits_type::int_type c;
        if (traits_type::is_eof(c = iostreams::get(dev)))
            hboost::throw_exception(HBOOST_IOSTREAMS_FAILURE("bad skip offset"));
        if (!traits_type::would_block(c))
            ++z;
    }
}

template<typename Filter, typename Device>
void skip( Filter& flt, Device& dev, stream_offset off,
           HBOOST_IOS::openmode which, mpl::true_ )
{ hboost::iostreams::seek(flt, dev, off, HBOOST_IOS::cur, which); }

template<typename Filter, typename Device>
void skip( Filter& flt, Device& dev, stream_offset off,
           HBOOST_IOS::openmode, mpl::false_ )
{ 
    typedef typename char_type_of<Device>::type char_type;
    char_type c;
    for (stream_offset z = 0; z < off; ) {
        std::streamsize amt;
        if ((amt = iostreams::read(flt, dev, &c, 1)) == -1)
            hboost::throw_exception(HBOOST_IOSTREAMS_FAILURE("bad skip offset"));
        if (amt == 1)
            ++z;
    }
}

} // End namespace detail.

template<typename Device>
void skip(Device& dev, stream_offset off)
{ 
    typedef typename mode_of<Device>::type     mode;
    typedef mpl::or_<
        is_convertible<mode, input_seekable>,
        is_convertible<mode, output_seekable>
    >                                          can_seek;
    HBOOST_STATIC_ASSERT(
        (can_seek::value || is_convertible<mode, input>::value)
    );
    detail::skip(dev, off, can_seek());
}

template<typename Filter, typename Device>
void skip( Filter& flt, Device& dev, stream_offset off, 
           HBOOST_IOS::openmode which = HBOOST_IOS::in | HBOOST_IOS::out )
{ 
    typedef typename mode_of<Filter>::type                 filter_mode;
    typedef typename mode_of<Device>::type                 device_mode;
    typedef mpl::or_<
        mpl::and_<
            is_convertible<filter_mode, input_seekable>,
            is_convertible<device_mode, input_seekable>
        >,
        mpl::and_<
            is_convertible<filter_mode, output_seekable>,
            is_convertible<device_mode, output_seekable>
        >
    >                                                      can_seek;
    HBOOST_STATIC_ASSERT(
        ( can_seek::value || 
          (is_convertible<filter_mode, input>::value &&
          is_convertible<device_mode, input>::value) )
    );
    detail::skip(flt, dev, off, which, can_seek());
}

} } // End namespaces iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_SKIP_HPP_INCLUDED //------------------------//
