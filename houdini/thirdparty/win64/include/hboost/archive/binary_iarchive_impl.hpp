#ifndef HBOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP
#define HBOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// binary_iarchive_impl.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <istream>
#include <hboost/archive/basic_binary_iprimitive.hpp>
#include <hboost/archive/basic_binary_iarchive.hpp>

#ifdef HBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace hboost { 
namespace archive {

namespace detail {
    template<class Archive> class interface_iarchive;
} // namespace detail

template<class Archive, class Elem, class Tr>
class HBOOST_SYMBOL_VISIBLE binary_iarchive_impl : 
    public basic_binary_iprimitive<Archive, Elem, Tr>,
    public basic_binary_iarchive<Archive>
{
#ifdef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
protected:
    #if HBOOST_WORKAROUND(HBOOST_MSVC, < 1500)
        // for some inexplicable reason insertion of "class" generates compile erro
        // on msvc 7.1
        friend detail::interface_iarchive<Archive>;
        friend basic_binary_iarchive<Archive>;
        friend load_access;
    #else
        friend class detail::interface_iarchive<Archive>;
        friend class basic_binary_iarchive<Archive>;
        friend class load_access;
    #endif
#endif
    template<class T>
    void load_override(T & t){
        this->basic_binary_iarchive<Archive>::load_override(t);
    }
    void init(unsigned int flags){
        if(0 != (flags & no_header)){
            return;
        }
        #if ! defined(__MWERKS__)
            this->basic_binary_iarchive<Archive>::init();
            this->basic_binary_iprimitive<Archive, Elem, Tr>::init();
        #else
            basic_binary_iarchive<Archive>::init();
            basic_binary_iprimitive<Archive, Elem, Tr>::init();
        #endif
    }
    binary_iarchive_impl(
        std::basic_streambuf<Elem, Tr> & bsb, 
        unsigned int flags
    ) :
        basic_binary_iprimitive<Archive, Elem, Tr>(
            bsb, 
            0 != (flags & no_codecvt)
        ),
        basic_binary_iarchive<Archive>(flags)
    {
        init(flags);
    }
    binary_iarchive_impl(
        std::basic_istream<Elem, Tr> & is, 
        unsigned int flags
    ) :
        basic_binary_iprimitive<Archive, Elem, Tr>(
            * is.rdbuf(), 
            0 != (flags & no_codecvt)
        ),
        basic_binary_iarchive<Archive>(flags)
    {
        init(flags);
    }
};

} // namespace archive
} // namespace hboost

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#endif // HBOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP
