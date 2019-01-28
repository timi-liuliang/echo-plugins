// (C) Copyright 2005 Matthias Troyer 

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_DETAIL_FORWARD_SKELETON_IARCHIVE_HPP
#define HBOOST_MPI_DETAIL_FORWARD_SKELETON_IARCHIVE_HPP

#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/detail/iserializer.hpp>
#include <hboost/archive/detail/interface_iarchive.hpp>
#include <hboost/archive/detail/common_iarchive.hpp>
#include <hboost/serialization/collection_size_type.hpp>

namespace hboost { namespace mpi { namespace detail {

template<class Archive, class ImplementationArchive>
class forward_skeleton_iarchive 
  : public archive::detail::common_iarchive<Archive>
{
public:

    typedef ImplementationArchive implementation_archive_type;

    forward_skeleton_iarchive(implementation_archive_type& ar) 
      : archive::detail::common_iarchive<Archive>(archive::no_header),
        implementation_archive(ar)
    {
    }

#ifdef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class archive::detail::interface_iarchive<Archive>;
    friend class archive::load_access;
protected:
#endif

  template<class T>
  void load_override(T & t)
  {
    archive::load(* this->This(), t);
  }

#define HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(T) \
    void load_override(T & t)                   \
    {                                           \
      implementation_archive >> t;              \
    }

HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(archive::class_id_optional_type)
HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(archive::version_type)
HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(archive::class_id_type)
HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(archive::class_id_reference_type)
HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(archive::object_id_type)
HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(archive::object_reference_type)
HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(archive::tracking_type)
HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(archive::class_name_type)
HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(serialization::collection_size_type)

    void load_override(std::string & s)    
    {                            
      serialization::collection_size_type length(s.size());              
      load_override(length);       
      s.resize(length);
    }

#undef HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION
protected:
    /// the actual archive used to serialize the information we actually want to store
    implementation_archive_type& implementation_archive;
};


} } } // end namespace hboost::mpi::detail

#endif // HBOOST_MPI_DETAIL_FORWARD_SKELETON_IARCHIVE_HPP