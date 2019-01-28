// (C) Copyright 2005 Matthias Troyer 

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_DETAIL_FORWARD_SKELETON_OARCHIVE_HPP
#define HBOOST_MPI_DETAIL_FORWARD_SKELETON_OARCHIVE_HPP

#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/detail/oserializer.hpp>
#include <hboost/archive/detail/interface_oarchive.hpp>
#include <hboost/archive/detail/common_oarchive.hpp>
#include <hboost/serialization/collection_size_type.hpp>

namespace hboost { namespace mpi { namespace detail {

template<class Archive, class ImplementationArchive>
class forward_skeleton_oarchive 
  : public archive::detail::common_oarchive<Archive>
{
public:

    typedef ImplementationArchive implementation_archive_type;

    forward_skeleton_oarchive(implementation_archive_type& ar) 
      : archive::detail::common_oarchive<Archive>(archive::no_header),
        implementation_archive(ar)
    {
    }

#ifdef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class archive::detail::interface_oarchive<Archive>;
    friend class archive::save_access;
protected:
#endif

  template<class T>
  void save_override(T const& t)
  {
    archive::save(* this->This(), t);
  }

#define HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION(T) \
    void save_override(T const & t)             \
    {                                           \
      implementation_archive << t;              \
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

    void save_override(std::string const & t)    
    {                                          
      save_override(serialization::collection_size_type(t.size()));       
    }


#undef HBOOST_ARCHIVE_FORWARD_IMPLEMENTATION
protected:
    /// the actual archive used to serialize the information we actually want to store
    implementation_archive_type& implementation_archive;
};

} } } // end namespace hboost::mpi::detail

#endif // HBOOST_MPI_DETAIL_FORWARD_SKELETON_OARCHIVE_HPP
