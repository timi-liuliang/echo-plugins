#ifndef HBOOST_ARCHIVE_XML_WIARCHIVE_HPP
#define HBOOST_ARCHIVE_XML_WIARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// xml_wiarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <hboost/config.hpp>
#ifdef HBOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <istream>

#include <hboost/smart_ptr/scoped_ptr.hpp>
#include <hboost/archive/detail/auto_link_warchive.hpp>
#include <hboost/archive/basic_text_iprimitive.hpp>
#include <hboost/archive/basic_xml_iarchive.hpp>
#include <hboost/archive/detail/register_archive.hpp>
#include <hboost/serialization/item_version_type.hpp>
// #include <hboost/archive/detail/utf8_codecvt_facet.hpp>

#include <hboost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef HBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace hboost {
namespace archive {

namespace detail {
    template<class Archive> class interface_iarchive;
} // namespace detail

template<class CharType>
class basic_xml_grammar;
typedef basic_xml_grammar<wchar_t> xml_wgrammar;

template<class Archive>
class HBOOST_SYMBOL_VISIBLE xml_wiarchive_impl : 
    public basic_text_iprimitive<std::wistream>,
    public basic_xml_iarchive<Archive>
{
#ifdef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
protected:
    friend class detail::interface_iarchive<Archive>;
    friend class basic_xml_iarchive<Archive>;
    friend class load_access;
#endif
    hboost::scoped_ptr<xml_wgrammar> gimpl;
    std::wistream & get_is(){
        return is;
    }
    template<class T>
    void 
    load(T & t){
        basic_text_iprimitive<std::wistream>::load(t);
    }
    void 
    load(version_type & t){
        unsigned int v;
        load(v);
        t = version_type(v);
    }
    void 
    load(hboost::serialization::item_version_type & t){
        unsigned int v;
        load(v);
        t = hboost::serialization::item_version_type(v);
    }
    HBOOST_WARCHIVE_DECL void
    load(char * t);
    #ifndef HBOOST_NO_INTRINSIC_WCHAR_T
    HBOOST_WARCHIVE_DECL void
    load(wchar_t * t);
    #endif
    HBOOST_WARCHIVE_DECL void
    load(std::string &s);
    #ifndef HBOOST_NO_STD_WSTRING
    HBOOST_WARCHIVE_DECL void
    load(std::wstring &ws);
    #endif
    template<class T>
    void load_override(T & t){
        basic_xml_iarchive<Archive>::load_override(t);
    }
    HBOOST_WARCHIVE_DECL void
    load_override(class_name_type & t);
    HBOOST_WARCHIVE_DECL void 
    init();
    HBOOST_WARCHIVE_DECL 
    xml_wiarchive_impl(std::wistream & is, unsigned int flags) ;
    HBOOST_WARCHIVE_DECL 
    ~xml_wiarchive_impl();
};

} // namespace archive
} // namespace hboost

#ifdef HBOOST_MSVC
#  pragma warning(pop) 
#endif

#include <hboost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#ifdef HBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace hboost { 
namespace archive {

class HBOOST_SYMBOL_VISIBLE xml_wiarchive :
    public xml_wiarchive_impl<xml_wiarchive>{
public:
    xml_wiarchive(std::wistream & is, unsigned int flags = 0) :
        xml_wiarchive_impl<xml_wiarchive>(is, flags)
    {}
    ~xml_wiarchive(){}
};

} // namespace archive
} // namespace hboost

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::archive::xml_wiarchive)

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#endif // HBOOST_NO_STD_WSTREAMBUF
#endif // HBOOST_ARCHIVE_XML_WIARCHIVE_HPP
