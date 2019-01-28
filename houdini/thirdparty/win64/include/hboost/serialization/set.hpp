#ifndef  HBOOST_SERIALIZATION_SET_HPP
#define HBOOST_SERIALIZATION_SET_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// set.hpp: serialization for stl set templates

// (C) Copyright 2002-2014 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <set>

#include <hboost/config.hpp>

#include <hboost/archive/detail/basic_iarchive.hpp>
#include <hboost/serialization/access.hpp>
#include <hboost/serialization/nvp.hpp>
#include <hboost/serialization/detail/stack_constructor.hpp>
#include <hboost/serialization/collection_size_type.hpp>
#include <hboost/serialization/item_version_type.hpp>

#include <hboost/serialization/collections_save_imp.hpp>
#include <hboost/serialization/split_free.hpp>

namespace hboost { 
namespace serialization {

template<class Archive, class Container>
inline void load_set_collection(Archive & ar, Container &s)
{
    s.clear();
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
    typename Container::iterator hint;
    hint = s.begin();
    while(count-- > 0){
        typedef typename Container::value_type type;
        detail::stack_construct<Archive, type> t(ar, item_version);
        // borland fails silently w/o full namespace
        ar >> hboost::serialization::make_nvp("item", t.reference());
        typename Container::iterator result =
            #ifdef HBOOST_NO_CXX11_HDR_UNORDERED_SET
                s.insert(hint, t.reference());
            #else
                s.emplace_hint(hint, t.reference());
            #endif
        ar.reset_object_address(& (* result), & t.reference());
        hint = result;
    }
}

template<class Archive, class Key, class Compare, class Allocator >
inline void save(
    Archive & ar,
    const std::set<Key, Compare, Allocator> &t,
    const unsigned int /* file_version */
){
    hboost::serialization::stl::save_collection<
        Archive, std::set<Key, Compare, Allocator> 
    >(ar, t);
}

template<class Archive, class Key, class Compare, class Allocator >
inline void load(
    Archive & ar,
    std::set<Key, Compare, Allocator> &t,
    const unsigned int /* file_version */
){
    load_set_collection(ar, t);
}

// split non-intrusive serialization function member into separate
// non intrusive save/load member functions
template<class Archive, class Key, class Compare, class Allocator >
inline void serialize(
    Archive & ar,
    std::set<Key, Compare, Allocator> & t,
    const unsigned int file_version
){
    hboost::serialization::split_free(ar, t, file_version);
}

// multiset
template<class Archive, class Key, class Compare, class Allocator >
inline void save(
    Archive & ar,
    const std::multiset<Key, Compare, Allocator> &t,
    const unsigned int /* file_version */
){
    hboost::serialization::stl::save_collection<
        Archive, 
        std::multiset<Key, Compare, Allocator> 
    >(ar, t);
}

template<class Archive, class Key, class Compare, class Allocator >
inline void load(
    Archive & ar,
    std::multiset<Key, Compare, Allocator> &t,
    const unsigned int /* file_version */
){
    load_set_collection(ar, t);
}

// split non-intrusive serialization function member into separate
// non intrusive save/load member functions
template<class Archive, class Key, class Compare, class Allocator >
inline void serialize(
    Archive & ar,
    std::multiset<Key, Compare, Allocator> & t,
    const unsigned int file_version
){
    hboost::serialization::split_free(ar, t, file_version);
}

} // namespace serialization
} // namespace hboost

#include <hboost/serialization/collection_traits.hpp>

HBOOST_SERIALIZATION_COLLECTION_TRAITS(std::set)
HBOOST_SERIALIZATION_COLLECTION_TRAITS(std::multiset)

#endif // HBOOST_SERIALIZATION_SET_HPP
