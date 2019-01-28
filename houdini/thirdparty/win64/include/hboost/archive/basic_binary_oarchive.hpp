#ifndef HBOOST_ARCHIVE_BASIC_BINARY_OARCHIVE_HPP
#define HBOOST_ARCHIVE_BASIC_BINARY_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_binary_oarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// archives stored as native binary - this should be the fastest way
// to archive the state of a group of obects.  It makes no attempt to
// convert to any canonical form.

// IN GENERAL, ARCHIVES CREATED WITH THIS CLASS WILL NOT BE READABLE
// ON PLATFORM APART FROM THE ONE THEY ARE CREATE ON

#include <hboost/assert.hpp>
#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>

#include <hboost/integer.hpp>
#include <hboost/integer_traits.hpp>

#include <hboost/archive/detail/common_oarchive.hpp>
#include <hboost/serialization/string.hpp>
#include <hboost/serialization/collection_size_type.hpp>
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

//////////////////////////////////////////////////////////////////////
// class basic_binary_oarchive - write serialized objects to a binary output stream
// note: this archive has no pretensions to portability.  Archive format
// may vary across machine architectures and compilers.  About the only
// guarentee is that an archive created with this code will be readable
// by a program built with the same tools for the same machne.  This class
// does have the virtue of buiding the smalles archive in the minimum amount
// of time.  So under some circumstances it may be he right choice.
template<class Archive>
class HBOOST_SYMBOL_VISIBLE basic_binary_oarchive : 
    public detail::common_oarchive<Archive>
{
#ifdef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
protected:
    #if HBOOST_WORKAROUND(HBOOST_MSVC, < 1500)
        // for some inexplicable reason insertion of "class" generates compile erro
        // on msvc 7.1
        friend detail::interface_oarchive<Archive>;
    #else
        friend class detail::interface_oarchive<Archive>;
    #endif
#endif
    // any datatype not specifed below will be handled by base class
    typedef detail::common_oarchive<Archive> detail_common_oarchive;
    template<class T>
    void save_override(const T & t){
      this->detail_common_oarchive::save_override(t);
    }

    // include these to trap a change in binary format which
    // isn't specifically handled
    HBOOST_STATIC_ASSERT(sizeof(tracking_type) == sizeof(bool));
    // upto 32K classes
    HBOOST_STATIC_ASSERT(sizeof(class_id_type) == sizeof(int_least16_t));
    HBOOST_STATIC_ASSERT(sizeof(class_id_reference_type) == sizeof(int_least16_t));
    // upto 2G objects
    HBOOST_STATIC_ASSERT(sizeof(object_id_type) == sizeof(uint_least32_t));
    HBOOST_STATIC_ASSERT(sizeof(object_reference_type) == sizeof(uint_least32_t));

    // binary files don't include the optional information 
    void save_override(const class_id_optional_type & /* t */){}

    // enable this if we decide to support generation of previous versions
    #if 0
    void save_override(const hboost::archive::version_type & t){
        library_version_type lvt = this->get_library_version();
        if(hboost::archive::library_version_type(7) < lvt){
            this->detail_common_oarchive::save_override(t);
        }
        else
        if(hboost::archive::library_version_type(6) < lvt){
            const hboost::uint_least16_t x = t;
            * this->This() << x;
        }
        else{
            const unsigned int x = t;
            * this->This() << x;
        }
    }
    void save_override(const hboost::serialization::item_version_type & t){
        library_version_type lvt = this->get_library_version();
        if(hboost::archive::library_version_type(7) < lvt){
            this->detail_common_oarchive::save_override(t);
        }
        else
        if(hboost::archive::library_version_type(6) < lvt){
            const hboost::uint_least16_t x = t;
            * this->This() << x;
        }
        else{
            const unsigned int x = t;
            * this->This() << x;
        }
    }

    void save_override(class_id_type & t){
        library_version_type lvt = this->get_library_version();
        if(hboost::archive::library_version_type(7) < lvt){
            this->detail_common_oarchive::save_override(t);
        }
        else
        if(hboost::archive::library_version_type(6) < lvt){
            const hboost::int_least16_t x = t;
            * this->This() << x;
        }
        else{
            const int x = t;
            * this->This() << x;
        }
    }
    void save_override(class_id_reference_type & t){
        save_override(static_cast<class_id_type &>(t));
    }

    #endif

    // explicitly convert to char * to avoid compile ambiguities
    void save_override(const class_name_type & t){
        const std::string s(t);
        * this->This() << s;
    }

    #if 0
    void save_override(const serialization::collection_size_type & t){
        if (get_library_version() < hboost::archive::library_version_type(6)){
            unsigned int x=0;
            * this->This() >> x;
            t = serialization::collection_size_type(x);
        } 
        else{
            * this->This() >> t;
        }
    }
    #endif
    HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
    init();

    basic_binary_oarchive(unsigned int flags) :
        detail::common_oarchive<Archive>(flags)
    {}
};

} // namespace archive
} // namespace hboost

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#include <hboost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // HBOOST_ARCHIVE_BASIC_BINARY_OARCHIVE_HPP
