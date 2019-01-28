#ifndef HBOOST_SERIALIZATION_COLLECTIONS_SAVE_IMP_HPP
#define HBOOST_SERIALIZATION_COLLECTIONS_SAVE_IMP_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// collections_save_imp.hpp: serialization for stl collections

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// helper function templates for serialization of collections

#include <hboost/config.hpp>
#include <hboost/serialization/nvp.hpp>
#include <hboost/serialization/serialization.hpp>
#include <hboost/serialization/version.hpp>
#include <hboost/serialization/collection_size_type.hpp>
#include <hboost/serialization/item_version_type.hpp>

namespace hboost{
namespace serialization {
namespace stl {

//////////////////////////////////////////////////////////////////////
// implementation of serialization for STL containers
//

template<class Archive, class Container>
inline void save_collection(
    Archive & ar,
    const Container &s,
    collection_size_type count)
{
    ar << HBOOST_SERIALIZATION_NVP(count);
    // record number of elements
    const item_version_type item_version(
        version<typename Container::value_type>::value
    );
    #if 0
        hboost::archive::library_version_type library_version(
            ar.get_library_version()
        );
        if(hboost::archive::library_version_type(3) < library_version){
            ar << HBOOST_SERIALIZATION_NVP(item_version);
        }
    #else
        ar << HBOOST_SERIALIZATION_NVP(item_version);
    #endif

    typename Container::const_iterator it = s.begin();
    while(count-- > 0){
        // note borland emits a no-op without the explicit namespace
        hboost::serialization::save_construct_data_adl(
            ar, 
            &(*it), 
            item_version
        );
        ar << hboost::serialization::make_nvp("item", *it++);
    }
}

template<class Archive, class Container>
inline void save_collection(Archive & ar, const Container &s)
{
    // record number of elements
    collection_size_type count(s.size());
    save_collection(ar, s, count);
}

} // namespace stl 
} // namespace serialization
} // namespace hboost

#endif //HBOOST_SERIALIZATION_COLLECTIONS_SAVE_IMP_HPP
