// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_CONCEPTS_HPP_INCLUDED
#define HBOOST_IOSTREAMS_CONCEPTS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <hboost/config.hpp>  // HBOOST_MSVC
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/categories.hpp>
#include <hboost/iostreams/detail/default_arg.hpp>
#include <hboost/iostreams/detail/ios.hpp>  // openmode.
#include <hboost/iostreams/positioning.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/type_traits/is_convertible.hpp>

namespace hboost { namespace iostreams {

//--------------Definitions of helper templates for device concepts-----------//

template<typename Mode, typename Ch = HBOOST_IOSTREAMS_DEFAULT_ARG(char)>
struct device {
    typedef Ch char_type;
    struct category
        : Mode,
          device_tag,
          closable_tag,
          localizable_tag
        { };

    void close()
    {
        using namespace detail;
        HBOOST_STATIC_ASSERT((!is_convertible<Mode, two_sequence>::value));
    }

    void close(HBOOST_IOS::openmode)
    {
        using namespace detail;
        HBOOST_STATIC_ASSERT((is_convertible<Mode, two_sequence>::value));
    }

    template<typename Locale>
    void imbue(const Locale&) { }
};

template<typename Mode, typename Ch = HBOOST_IOSTREAMS_DEFAULT_ARG(wchar_t)>
struct wdevice : device<Mode, Ch> { };

typedef device<input>    source;
typedef wdevice<input>   wsource;
typedef device<output>   sink;
typedef wdevice<output>  wsink;

//--------------Definitions of helper templates for simple filter concepts----//

template<typename Mode, typename Ch = HBOOST_IOSTREAMS_DEFAULT_ARG(char)>
struct filter {
    typedef Ch char_type;
    struct category
        : Mode,
          filter_tag,
          closable_tag,
          localizable_tag
        { };

    template<typename Device>
    void close(Device&)
    {
        using namespace detail;
        HBOOST_STATIC_ASSERT((!is_convertible<Mode, two_sequence>::value));
        HBOOST_STATIC_ASSERT((!is_convertible<Mode, dual_use>::value));
    }

    template<typename Device>
    void close(Device&, HBOOST_IOS::openmode)
    {
        using namespace detail;
        HBOOST_STATIC_ASSERT(
            (is_convertible<Mode, two_sequence>::value) ||
            (is_convertible<Mode, dual_use>::value)
        );
    }

    template<typename Locale>
    void imbue(const Locale&) { }
};

template<typename Mode, typename Ch = HBOOST_IOSTREAMS_DEFAULT_ARG(wchar_t)>
struct wfilter : filter<Mode, Ch> { };

typedef filter<input>      input_filter;
typedef wfilter<input>     input_wfilter;
typedef filter<output>     output_filter;
typedef wfilter<output>    output_wfilter;
typedef filter<seekable>   seekable_filter;
typedef wfilter<seekable>  seekable_wfilter;
typedef filter<dual_use>   dual_use_filter;
typedef wfilter<dual_use>  dual_use_wfilter;
        
//------Definitions of helper templates for multi-character filter cncepts----//

template<typename Mode, typename Ch = char>
struct multichar_filter : filter<Mode, Ch> {
    struct category : filter<Mode, Ch>::category, multichar_tag { };
};

template<typename Mode, typename Ch = wchar_t>
struct multichar_wfilter : multichar_filter<Mode, Ch> { };

typedef multichar_filter<input>      multichar_input_filter;
typedef multichar_wfilter<input>     multichar_input_wfilter;
typedef multichar_filter<output>     multichar_output_filter;
typedef multichar_wfilter<output>    multichar_output_wfilter;
typedef multichar_filter<dual_use>   multichar_dual_use_filter;
typedef multichar_wfilter<dual_use>  multichar_dual_use_wfilter;

//----------------------------------------------------------------------------//

} } // End namespaces iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_CONCEPTS_HPP_INCLUDED
