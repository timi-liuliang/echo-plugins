#ifndef HBOOST_ARCHIVE_XML_WOARCHIVE_HPP
#define HBOOST_ARCHIVE_XML_WOARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// xml_woarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <hboost/config.hpp>
#ifdef HBOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else
#include <cstddef> // size_t
#if defined(HBOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t; 
} // namespace std
#endif

#include <ostream>

//#include <hboost/smart_ptr/scoped_ptr.hpp>
#include <hboost/archive/detail/auto_link_warchive.hpp>
#include <hboost/archive/basic_text_oprimitive.hpp>
#include <hboost/archive/basic_xml_oarchive.hpp>
#include <hboost/archive/detail/register_archive.hpp>
#include <hboost/serialization/item_version_type.hpp>
//#include <hboost/archive/detail/utf8_codecvt_facet.hpp>

#include <hboost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef HBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace hboost {
namespace archive {

namespace detail {
    template<class Archive> class interface_oarchive;
} // namespace detail

template<class Archive>
class HBOOST_SYMBOL_VISIBLE xml_woarchive_impl :
    public basic_text_oprimitive<std::wostream>,
    public basic_xml_oarchive<Archive>
{
#ifdef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
protected:
    friend class detail::interface_oarchive<Archive>;
    friend class basic_xml_oarchive<Archive>;
    friend class save_access;
#endif
    //void end_preamble(){
    //    basic_xml_oarchive<Archive>::end_preamble();
    //}
    template<class T>
    void
    save(const T & t){
        basic_text_oprimitive<std::wostream>::save(t);
    }
    void
    save(const version_type & t){
        save(static_cast<const unsigned int>(t));
    }
    void 
    save(const hboost::serialization::item_version_type & t){
        save(static_cast<const unsigned int>(t));
    }
    HBOOST_WARCHIVE_DECL void
    save(const char * t);
    #ifndef HBOOST_NO_INTRINSIC_WCHAR_T
    HBOOST_WARCHIVE_DECL void 
    save(const wchar_t * t);
    #endif
    HBOOST_WARCHIVE_DECL void 
    save(const std::string &s);
    #ifndef HBOOST_NO_STD_WSTRING
    HBOOST_WARCHIVE_DECL void
    save(const std::wstring &ws);
    #endif
    HBOOST_WARCHIVE_DECL 
    xml_woarchive_impl(std::wostream & os, unsigned int flags);
    HBOOST_WARCHIVE_DECL
    ~xml_woarchive_impl();
public:
    HBOOST_WARCHIVE_DECL void
    save_binary(const void *address, std::size_t count);

};

// we use the following because we can't use
// typedef xml_woarchive_impl<xml_woarchive_impl<...> > xml_woarchive;

// do not derive from this class.  If you want to extend this functionality
// via inhertance, derived from xml_woarchive_impl instead.  This will
// preserve correct static polymorphism.
class HBOOST_SYMBOL_VISIBLE xml_woarchive :
    public xml_woarchive_impl<xml_woarchive>
{
public:
    xml_woarchive(std::wostream & os, unsigned int flags = 0) :
        xml_woarchive_impl<xml_woarchive>(os, flags)
    {}
    ~xml_woarchive(){}
};

} // namespace archive
} // namespace hboost

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::archive::xml_woarchive)

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#include <hboost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // HBOOST_NO_STD_WSTREAMBUF
#endif // HBOOST_ARCHIVE_XML_OARCHIVE_HPP
