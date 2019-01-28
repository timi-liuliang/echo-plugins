#ifndef HBOOST_ARCHIVE_BINARY_IPRIMITIVE_HPP
#define HBOOST_ARCHIVE_BINARY_IPRIMITIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4800 )
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_binary_iprimitive.hpp
//
// archives stored as native binary - this should be the fastest way
// to archive the state of a group of obects.  It makes no attempt to
// convert to any canonical form.

// IN GENERAL, ARCHIVES CREATED WITH THIS CLASS WILL NOT BE READABLE
// ON PLATFORM APART FROM THE ONE THEY ARE CREATED ON

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <iosfwd>
#include <hboost/assert.hpp>
#include <locale>
#include <cstring> // std::memcpy
#include <cstddef> // std::size_t
#include <streambuf> // basic_streambuf
#include <string>

#include <hboost/config.hpp>
#if defined(HBOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::memcpy; 
    using ::size_t;
} // namespace std
#endif

#include <hboost/cstdint.hpp>
#include <hboost/serialization/throw_exception.hpp>
#include <hboost/integer.hpp>
#include <hboost/integer_traits.hpp>

//#include <hboost/mpl/placeholders.hpp>
#include <hboost/serialization/is_bitwise_serializable.hpp>
#include <hboost/serialization/array.hpp>

#include <hboost/archive/basic_streambuf_locale_saver.hpp>
#include <hboost/archive/codecvt_null.hpp>
#include <hboost/archive/archive_exception.hpp>
#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/detail/abi_prefix.hpp> // must be the last header

namespace hboost { 
namespace archive {

/////////////////////////////////////////////////////////////////////////////
// class binary_iarchive - read serialized objects from a input binary stream
template<class Archive, class Elem, class Tr>
class HBOOST_SYMBOL_VISIBLE basic_binary_iprimitive {
#ifndef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
    friend class load_access;
protected:
#else
public:
#endif
    std::basic_streambuf<Elem, Tr> & m_sb;
    // return a pointer to the most derived class
    Archive * This(){
        return static_cast<Archive *>(this);
    }

    #ifndef HBOOST_NO_STD_LOCALE
    // note order! - if you change this, libstd++ will fail!
    // a) create new locale with new codecvt facet
    // b) save current locale
    // c) change locale to new one
    // d) use stream buffer
    // e) change locale back to original
    // f) destroy new codecvt facet
    hboost::archive::codecvt_null<Elem> codecvt_null_facet;
    basic_streambuf_locale_saver<Elem, Tr> locale_saver;
    std::locale archive_locale;
    #endif

    // main template for serilization of primitive types
    template<class T>
    void load(T & t){
        load_binary(& t, sizeof(T));
    }

    /////////////////////////////////////////////////////////
    // fundamental types that need special treatment
    
    // trap usage of invalid uninitialized boolean 
    void load(bool & t){
        load_binary(& t, sizeof(t));
        int i = t;
        HBOOST_ASSERT(0 == i || 1 == i);
        (void)i; // warning suppression for release builds.
    }
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
    load(std::string &s);
    #ifndef HBOOST_NO_STD_WSTRING
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
    load(std::wstring &ws);
    #endif
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
    load(char * t);
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
    load(wchar_t * t);

    HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
    init();
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL 
    basic_binary_iprimitive(
        std::basic_streambuf<Elem, Tr> & sb, 
        bool no_codecvt
    );
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL 
    ~basic_binary_iprimitive();
public:
    // we provide an optimized load for all fundamental types
    // typedef serialization::is_bitwise_serializable<mpl::_1> 
    // use_array_optimization;
    struct use_array_optimization {  
        template <class T>  
        #if defined(HBOOST_NO_DEPENDENT_NESTED_DERIVATIONS)  
            struct apply {  
                typedef typename hboost::serialization::is_bitwise_serializable< T >::type type;  
            };
        #else
            struct apply : public hboost::serialization::is_bitwise_serializable< T > {};  
        #endif
    };

    // the optimized load_array dispatches to load_binary 
    template <class ValueType>
    void load_array(serialization::array_wrapper<ValueType>& a, unsigned int)
    {
      load_binary(a.address(),a.count()*sizeof(ValueType));
    }

    void
    load_binary(void *address, std::size_t count);
};

template<class Archive, class Elem, class Tr>
inline void
basic_binary_iprimitive<Archive, Elem, Tr>::load_binary(
    void *address, 
    std::size_t count
){
    // note: an optimizer should eliminate the following for char files
    HBOOST_ASSERT(
        static_cast<std::streamsize>(count / sizeof(Elem)) 
        <= hboost::integer_traits<std::streamsize>::const_max
    );
    std::streamsize s = static_cast<std::streamsize>(count / sizeof(Elem));
    std::streamsize scount = m_sb.sgetn(
        static_cast<Elem *>(address), 
        s
    );
    if(scount != s)
        hboost::serialization::throw_exception(
            archive_exception(archive_exception::input_stream_error)
        );
    // note: an optimizer should eliminate the following for char files
    HBOOST_ASSERT(count % sizeof(Elem) <= hboost::integer_traits<std::streamsize>::const_max);
    s = static_cast<std::streamsize>(count % sizeof(Elem));
    if(0 < s){
//        if(is.fail())
//            hboost::serialization::throw_exception(
//                archive_exception(archive_exception::stream_error)
//        );
        Elem t;
        scount = m_sb.sgetn(& t, 1);
        if(scount != 1)
            hboost::serialization::throw_exception(
                archive_exception(archive_exception::input_stream_error)
            );
        std::memcpy(static_cast<char*>(address) + (count - s), &t, static_cast<std::size_t>(s));
    }
}

} // namespace archive
} // namespace hboost

#include <hboost/archive/detail/abi_suffix.hpp> // pop pragmas

#endif // HBOOST_ARCHIVE_BINARY_IPRIMITIVE_HPP
