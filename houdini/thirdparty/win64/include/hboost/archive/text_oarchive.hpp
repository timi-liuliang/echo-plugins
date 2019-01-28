#ifndef HBOOST_ARCHIVE_TEXT_OARCHIVE_HPP
#define HBOOST_ARCHIVE_TEXT_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// text_oarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <ostream>
#include <cstddef> // std::size_t

#include <hboost/config.hpp>
#if defined(HBOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t; 
} // namespace std
#endif

#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/basic_text_oprimitive.hpp>
#include <hboost/archive/basic_text_oarchive.hpp>
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
    template<class Archive> class interface_oarchive;
} // namespace detail

template<class Archive>
class HBOOST_SYMBOL_VISIBLE text_oarchive_impl :
     /* protected ? */ public basic_text_oprimitive<std::ostream>,
     public basic_text_oarchive<Archive>
{
#ifdef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
protected:
    friend class detail::interface_oarchive<Archive>;
    friend class basic_text_oarchive<Archive>;
    friend class save_access;
#endif
    template<class T>
    void save(const T & t){
        this->newtoken();
        basic_text_oprimitive<std::ostream>::save(t);
    }
    void save(const version_type & t){
        save(static_cast<const unsigned int>(t));
    }
    void save(const hboost::serialization::item_version_type & t){
        save(static_cast<const unsigned int>(t));
    }
    HBOOST_ARCHIVE_DECL void 
    save(const char * t);
    #ifndef HBOOST_NO_INTRINSIC_WCHAR_T
    HBOOST_ARCHIVE_DECL void 
    save(const wchar_t * t);
    #endif
    HBOOST_ARCHIVE_DECL void 
    save(const std::string &s);
    #ifndef HBOOST_NO_STD_WSTRING
    HBOOST_ARCHIVE_DECL void 
    save(const std::wstring &ws);
    #endif
    HBOOST_ARCHIVE_DECL 
    text_oarchive_impl(std::ostream & os, unsigned int flags);
    // don't import inline definitions! leave this as a reminder.
    //HBOOST_ARCHIVE_DECL 
    ~text_oarchive_impl(){};
public:
    HBOOST_ARCHIVE_DECL void 
    save_binary(const void *address, std::size_t count);
};

// do not derive from this class.  If you want to extend this functionality
// via inhertance, derived from text_oarchive_impl instead.  This will
// preserve correct static polymorphism.
class HBOOST_SYMBOL_VISIBLE text_oarchive : 
    public text_oarchive_impl<text_oarchive>
{
public:
    text_oarchive(std::ostream & os_, unsigned int flags = 0) :
        // note: added _ to suppress useless gcc warning
        text_oarchive_impl<text_oarchive>(os_, flags)
    {}
    ~text_oarchive(){}
};

} // namespace archive
} // namespace hboost

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::archive::text_oarchive)

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#include <hboost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // HBOOST_ARCHIVE_TEXT_OARCHIVE_HPP
