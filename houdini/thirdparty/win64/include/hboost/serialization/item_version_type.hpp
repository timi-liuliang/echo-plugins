#ifndef HBOOST_SERIALIZATION_ITEM_VERSION_TYPE_HPP
#define HBOOST_SERIALIZATION_ITEM_VERSION_TYPE_HPP

// (C) Copyright 2010 Robert Ramey
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/cstdint.hpp> // uint_least8_t
#include <hboost/integer_traits.hpp>
#include <hboost/serialization/level.hpp>
#include <hboost/serialization/is_bitwise_serializable.hpp>

// fixes broken example build on x86_64-linux-gnu-gcc-4.6.0
#include <hboost/assert.hpp>

namespace hboost { 
namespace serialization {

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4244 4267 )
#endif

class item_version_type {
private:
    typedef unsigned int base_type;
    base_type t;
public:
    // should be private - but MPI fails if it's not!!!
    item_version_type(): t(0) {};
    explicit item_version_type(const unsigned int t_) : t(t_){
        HBOOST_ASSERT(t_ <= hboost::integer_traits<base_type>::const_max);
    }
    item_version_type(const item_version_type & t_) : 
        t(t_.t)
    {}
    item_version_type & operator=(item_version_type rhs){
        t = rhs.t; 
        return *this;
    }
    // used for text output
    operator base_type () const {
        return t;
    }                
    // used for text input
    operator base_type & () {
        return t;
    }
    bool operator==(const item_version_type & rhs) const {
        return t == rhs.t;
    } 
    bool operator<(const item_version_type & rhs) const {
        return t < rhs.t;
    }
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

} } // end namespace hboost::serialization

HBOOST_IS_BITWISE_SERIALIZABLE(item_version_type)

HBOOST_CLASS_IMPLEMENTATION(item_version_type, primitive_type)

#endif //HBOOST_SERIALIZATION_ITEM_VERSION_TYPE_HPP
