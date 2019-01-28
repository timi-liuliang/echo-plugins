#ifndef HBOOST_ARCHIVE_XML_IARCHIVE_HPP
#define HBOOST_ARCHIVE_XML_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// xml_iarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <istream>

#include <hboost/scoped_ptr.hpp>
#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/basic_text_iprimitive.hpp>
#include <hboost/archive/basic_xml_iarchive.hpp>
#include <hboost/archive/detail/register_archive.hpp>
#include <hboost/serialization/item_version_type.hpp>

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
typedef basic_xml_grammar<char> xml_grammar;

template<class Archive>
class HBOOST_SYMBOL_VISIBLE xml_iarchive_impl : 
    public basic_text_iprimitive<std::istream>,
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
    // use boost:scoped_ptr to implement automatic deletion;
    hboost::scoped_ptr<xml_grammar> gimpl;

    std::istream & get_is(){
        return is;
    }
    template<class T>
    void load(T & t){
        basic_text_iprimitive<std::istream>::load(t);
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
    HBOOST_ARCHIVE_DECL void
    load(char * t);
    #ifndef HBOOST_NO_INTRINSIC_WCHAR_T
    HBOOST_ARCHIVE_DECL void
    load(wchar_t * t);
    #endif
    HBOOST_ARCHIVE_DECL void
    load(std::string &s);
    #ifndef HBOOST_NO_STD_WSTRING
    HBOOST_ARCHIVE_DECL void
    load(std::wstring &ws);
    #endif
    template<class T>
    void load_override(T & t){
        basic_xml_iarchive<Archive>::load_override(t);
    }
    HBOOST_ARCHIVE_DECL void
    load_override(class_name_type & t);
    HBOOST_ARCHIVE_DECL void
    init();
    HBOOST_ARCHIVE_DECL 
    xml_iarchive_impl(std::istream & is, unsigned int flags);
    HBOOST_ARCHIVE_DECL
    ~xml_iarchive_impl();
};

} // namespace archive
} // namespace hboost

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#include <hboost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas
#ifdef HBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace hboost { 
namespace archive {

class HBOOST_SYMBOL_VISIBLE xml_iarchive : 
    public xml_iarchive_impl<xml_iarchive>{
public:
    xml_iarchive(std::istream & is, unsigned int flags = 0) :
        xml_iarchive_impl<xml_iarchive>(is, flags)
    {}
    ~xml_iarchive(){};
};

} // namespace archive
} // namespace hboost

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::archive::xml_iarchive)

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#endif // HBOOST_ARCHIVE_XML_IARCHIVE_HPP
