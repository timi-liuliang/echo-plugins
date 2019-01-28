#ifndef HBOOST_SERIALIZATION_BASIC_OSERIALIZER_HPP
#define HBOOST_SERIALIZATION_BASIC_OSERIALIZER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_oserializer.hpp: extenstion of type_info required for serialization.

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <cstddef> // NULL
#include <hboost/config.hpp>
#include <hboost/noncopyable.hpp>

#include <hboost/archive/basic_archive.hpp>
#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/detail/basic_serializer.hpp>

#include <hboost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef HBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace hboost {
namespace serialization {
    class extended_type_info;
} // namespace serialization

// forward declarations
namespace archive {
namespace detail {

class basic_oarchive;
class basic_pointer_oserializer;

class HBOOST_SYMBOL_VISIBLE basic_oserializer :
    public basic_serializer
{
private:
    basic_pointer_oserializer *m_bpos;
protected:
    explicit HBOOST_ARCHIVE_DECL basic_oserializer(
        const hboost::serialization::extended_type_info & type_
    );
    virtual HBOOST_ARCHIVE_DECL ~basic_oserializer();
public:
    bool serialized_as_pointer() const {
        return m_bpos != NULL;
    }
    void set_bpos(basic_pointer_oserializer *bpos){
        m_bpos = bpos;
    }
    const basic_pointer_oserializer * get_bpos() const {
        return m_bpos;
    }
    virtual void save_object_data(
        basic_oarchive & ar, const void * x
    ) const = 0;
    // returns true if class_info should be saved
    virtual bool class_info() const = 0;
    // returns true if objects should be tracked
    virtual bool tracking(const unsigned int flags) const = 0;
    // returns class version
    virtual version_type version() const = 0;
    // returns true if this class is polymorphic
    virtual bool is_polymorphic() const = 0;
};

} // namespace detail
} // namespace serialization
} // namespace hboost

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#include <hboost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // HBOOST_SERIALIZATION_BASIC_OSERIALIZER_HPP
