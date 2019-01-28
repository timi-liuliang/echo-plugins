// (C) Copyright 2005 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_DETAIL_MPI_DATATYPE_OARCHIVE_HPP
#define HBOOST_MPI_DETAIL_MPI_DATATYPE_OARCHIVE_HPP

#include <hboost/type_traits/is_enum.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/archive/detail/oserializer.hpp>
#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/basic_archive.hpp>
#include <hboost/mpi/detail/ignore_skeleton_oarchive.hpp>
#include <hboost/mpi/detail/mpi_datatype_primitive.hpp>
#include <hboost/mpi/datatype_fwd.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/integer.hpp>
#include <hboost/archive/detail/register_archive.hpp>

namespace hboost { namespace mpi { namespace detail {


// an archive wrapper that stores only the data members but not the
// special types defined by the serialization library
// to define the data skeletons (classes, pointers, container sizes, ...)

class mpi_datatype_oarchive
  : public mpi_datatype_primitive,
    public ignore_skeleton_oarchive<mpi_datatype_oarchive>
{
public:
    template <class T>
    mpi_datatype_oarchive(const T& x)
         :  mpi_datatype_primitive(&x) // register address
        {
          HBOOST_MPL_ASSERT((is_mpi_datatype<T>));
          *this << x;                   // serialize the object
        }

    template<class T>
    void save_override(T const& t)
    {
      save_enum(t,hboost::is_enum<T>());
    }

    template<class T>
    void save_enum(T const& t, mpl::false_)
    {
      ignore_skeleton_oarchive<mpi_datatype_oarchive>::save_override(t);
    }

    template<class T>
    void save_enum(T const& t, mpl::true_)
    {
      // select the right sized integer for the enum
      typedef typename hboost::uint_t<8*sizeof(T)>::least int_type;
      HBOOST_STATIC_ASSERT((sizeof(T)==sizeof(int_type)));
      this->save(*reinterpret_cast<int_type const*>(&t));
    }

};

} } } // end namespace hboost::mpi::detail

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::mpi::detail::mpi_datatype_oarchive)
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::mpi::detail::ignore_skeleton_oarchive<hboost::mpi::detail::mpi_datatype_oarchive>)
HBOOST_SERIALIZATION_USE_ARRAY_OPTIMIZATION(hboost::mpi::detail::mpi_datatype_oarchive)

#endif // HBOOST_MPI_DETAIL_MPI_DATATYPE_OARCHIVE_HPP
