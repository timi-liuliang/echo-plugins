#ifndef HBOOST_SERIALIZATION_LIST_HPP
#define HBOOST_SERIALIZATION_LIST_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// list.hpp: serialization for stl list templates

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <list>

#include <hboost/config.hpp>

#include <hboost/serialization/collections_save_imp.hpp>
#include <hboost/serialization/collections_load_imp.hpp>

#include <hboost/archive/detail/basic_iarchive.hpp>
#include <hboost/serialization/access.hpp>
#include <hboost/serialization/nvp.hpp>
#include <hboost/serialization/collection_size_type.hpp>
#include <hboost/serialization/item_version_type.hpp>
#include <hboost/serialization/split_free.hpp>
#include <hboost/serialization/detail/stack_constructor.hpp>

namespace hboost { 
namespace serialization {

template<class Archive, class U, class Allocator>
inline void save(
    Archive & ar,
    const std::list<U, Allocator> &t,
    const unsigned int /* file_version */
){
    hboost::serialization::stl::save_collection<
        Archive, 
        std::list<U, Allocator> 
    >(ar, t);
}

template<class Archive, class U, class Allocator>
inline void load(
    Archive & ar,
    std::list<U, Allocator> &t,
    const unsigned int /* file_version */
){
    const hboost::archive::library_version_type library_version(
        ar.get_library_version()
    );
    // retrieve number of elements
    item_version_type item_version(0);
    collection_size_type count;
    ar >> HBOOST_SERIALIZATION_NVP(count);
    if(hboost::archive::library_version_type(3) < library_version){
        ar >> HBOOST_SERIALIZATION_NVP(item_version);
    }
    stl::collection_load_impl(ar, t, count, item_version);
}

// split non-intrusive serialization function member into separate
// non intrusive save/load member functions
template<class Archive, class U, class Allocator>
inline void serialize(
    Archive & ar,
    std::list<U, Allocator> & t,
    const unsigned int file_version
){
    hboost::serialization::split_free(ar, t, file_version);
}

} // serialization
} // namespace hboost

#include <hboost/serialization/collection_traits.hpp>

HBOOST_SERIALIZATION_COLLECTION_TRAITS(std::list)

#endif // HBOOST_SERIALIZATION_LIST_HPP
