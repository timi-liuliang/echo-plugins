// (C) Copyright 2005 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_DETAIL_CONTENT_OARCHIVE_HPP
#define HBOOST_MPI_DETAIL_CONTENT_OARCHIVE_HPP

#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/basic_archive.hpp>
#include <hboost/mpi/detail/ignore_skeleton_oarchive.hpp>
#include <hboost/mpi/detail/mpi_datatype_primitive.hpp>
#include <hboost/mpi/datatype.hpp>
#include <hboost/archive/detail/register_archive.hpp>

namespace hboost { namespace mpi {

namespace detail {
  // an archive wrapper that stores only the data members but not the
  // special types defined by the serialization library
  // to define the data skeletons (classes, pointers, container sizes, ...)

  class HBOOST_MPI_DECL content_oarchive
    : public mpi_datatype_primitive,
      public ignore_skeleton_oarchive<content_oarchive>
  {
  public:
      content_oarchive()
       : committed(false)
          {}

      content get_content()
      {
        if (!committed)
        {
          // create the content holder only once
          c=this->get_mpi_datatype();
          committed=true;
        }
        return c;
      }

  private:
    bool committed;
    content c;
  };
} // end namespace detail

template <class T>
const content get_content(const T& x)
{
  detail::content_oarchive ar;
  ar << x;
  return ar.get_content();
}

} } // end namespace hboost::mpi

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::mpi::detail::content_oarchive)
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::mpi::detail::ignore_skeleton_oarchive<hboost::mpi::detail::content_oarchive>)
HBOOST_SERIALIZATION_USE_ARRAY_OPTIMIZATION(hboost::mpi::detail::content_oarchive)
#endif // HBOOST_MPI_DETAIL_CONTENT_OARCHIVE_HPP
