#ifndef HBOOST_ARCHIVE_BASIC_TEXT_IPRIMITIVE_HPP
#define HBOOST_ARCHIVE_BASIC_TEXT_IPRIMITIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_text_iprimitive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// archives stored as text - note these are templated on the basic
// stream templates to accommodate wide (and other?) kind of characters
//
// Note the fact that on libraries without wide characters, ostream is
// not a specialization of basic_ostream which in fact is not defined
// in such cases.   So we can't use basic_ostream<IStream::char_type> but rather
// use two template parameters

#include <locale>
#include <cstddef> // size_t

#include <hboost/config.hpp>
#if defined(HBOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t; 
    #if ! defined(HBOOST_DINKUMWARE_STDLIB) && ! defined(__SGI_STL_PORT)
        using ::locale;
    #endif
} // namespace std
#endif

#include <hboost/io/ios_state.hpp>
#include <hboost/static_assert.hpp>

#include <hboost/detail/workaround.hpp>
#if HBOOST_WORKAROUND(HBOOST_DINKUMWARE_STDLIB, == 1)
#include <hboost/archive/dinkumware.hpp>
#endif
#include <hboost/serialization/throw_exception.hpp>
#include <hboost/archive/codecvt_null.hpp>
#include <hboost/archive/archive_exception.hpp>
#include <hboost/archive/basic_streambuf_locale_saver.hpp>
#include <hboost/archive/detail/abi_prefix.hpp> // must be the last header

namespace hboost {
namespace archive {

/////////////////////////////////////////////////////////////////////////
// class basic_text_iarchive - load serialized objects from a input text stream
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4244 4267 )
#endif

template<class IStream>
class HBOOST_SYMBOL_VISIBLE basic_text_iprimitive {
protected:
    IStream &is;
    io::ios_flags_saver flags_saver;
    io::ios_precision_saver precision_saver;

    #ifndef HBOOST_NO_STD_LOCALE
    // note order! - if you change this, libstd++ will fail!
    // a) create new locale with new codecvt facet
    // b) save current locale
    // c) change locale to new one
    // d) use stream buffer
    // e) change locale back to original
    // f) destroy new codecvt facet
    hboost::archive::codecvt_null<typename IStream::char_type> codecvt_null_facet;
    std::locale archive_locale;
    basic_istream_locale_saver<
        typename IStream::char_type,
        typename IStream::traits_type
    > locale_saver;
    #endif

    template<class T>
    void load(T & t)
    {
        if(is >> t)
            return;
        hboost::serialization::throw_exception(
            archive_exception(archive_exception::input_stream_error)
        );
    }

    void load(char & t)
    {
        short int i;
        load(i);
        t = i;
    }
    void load(signed char & t)
    {
        short int i;
        load(i);
        t = i;
    }
    void load(unsigned char & t)
    {
        unsigned short int i;
        load(i);
        t = i;
    }

    #ifndef HBOOST_NO_INTRINSIC_WCHAR_T
    void load(wchar_t & t)
    {
        HBOOST_STATIC_ASSERT(sizeof(wchar_t) <= sizeof(int));
        int i;
        load(i);
        t = i;
    }
    #endif
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL 
    basic_text_iprimitive(IStream  &is, bool no_codecvt);
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL 
    ~basic_text_iprimitive();
public:
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
    load_binary(void *address, std::size_t count);
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

} // namespace archive
} // namespace hboost

#include <hboost/archive/detail/abi_suffix.hpp> // pop pragmas

#endif // HBOOST_ARCHIVE_BASIC_TEXT_IPRIMITIVE_HPP
