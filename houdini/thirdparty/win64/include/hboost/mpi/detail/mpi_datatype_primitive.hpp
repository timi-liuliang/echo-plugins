// (C) Copyright 2005 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_DETAIL_MPI_DATATYPE_OPRIMITIVE_HPP
#define HBOOST_MPI_DETAIL_MPI_DATATYPE_OPRIMITIVE_HPP

#include <hboost/mpi/config.hpp>
#include <cstddef> // size_t

#include <hboost/config.hpp>
#if defined(HBOOST_NO_STDC_NAMESPACE)
namespace std{
    using ::size_t;
} // namespace std
#endif

#include <hboost/mpi/datatype_fwd.hpp>
#include <hboost/mpi/exception.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/assert.hpp>
#include <hboost/mpl/placeholders.hpp>
#include <hboost/serialization/array.hpp>
#include <hboost/serialization/detail/get_data.hpp>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace hboost { namespace mpi { namespace detail {

/////////////////////////////////////////////////////////////////////////
// class mpi_data_type_oprimitive - creation of custom MPI data types

class mpi_datatype_primitive
{
public:

    // trivial default constructor
    mpi_datatype_primitive()
     : is_committed(false),
       origin(0)
    {}

    mpi_datatype_primitive(void const* orig)
     : is_committed(false),
       origin()
    {
#if defined(MPI_VERSION) && MPI_VERSION >= 2
      HBOOST_MPI_CHECK_RESULT(MPI_Get_address,(const_cast<void*>(orig), &origin));
#else
      HBOOST_MPI_CHECK_RESULT(MPI_Address,(const_cast<void*>(orig), &origin));
#endif
    }

    void save_binary(void const *address, std::size_t count)
    {
      save_impl(address,MPI_BYTE,count);
    }

    // fast saving of arrays of MPI types
    template<class T>
    void save_array(serialization::array_wrapper<T> const& x, unsigned int /* version */)
    {
      if (x.count())
        save_impl(x.address(), hboost::mpi::get_mpi_datatype(*x.address()), x.count());
    }

    typedef is_mpi_datatype<mpl::_1> use_array_optimization;

    // create and return the custom MPI data type
    MPI_Datatype get_mpi_datatype()
    {
      if (!is_committed)
      {
#if defined(MPI_VERSION) && MPI_VERSION >= 2
       HBOOST_MPI_CHECK_RESULT(MPI_Type_create_struct,
                    (
                      addresses.size(),
                      hboost::serialization::detail::get_data(lengths),
                      hboost::serialization::detail::get_data(addresses),
                      hboost::serialization::detail::get_data(types),
                      &datatype_
                    ));
#else
        HBOOST_MPI_CHECK_RESULT(MPI_Type_struct,
                               (
                                addresses.size(),
                                hboost::serialization::detail::get_data(lengths),
                                hboost::serialization::detail::get_data(addresses),
                                hboost::serialization::detail::get_data(types),
                                &datatype_
                                ));
#endif
        HBOOST_MPI_CHECK_RESULT(MPI_Type_commit,(&datatype_));
        
        is_committed = true;
      }

      return datatype_;
    }

    // default saving of primitives.
    template<class T>
    void save(const T & t)
    {
        save_impl(&t, hboost::mpi::get_mpi_datatype(t), 1);
    }

private:

    void save_impl(void const * p, MPI_Datatype t, int l)
    {
      HBOOST_ASSERT ( !is_committed );

      // store address, type and length

      MPI_Aint a;
#if defined(MPI_VERSION) && MPI_VERSION >= 2
     HBOOST_MPI_CHECK_RESULT(MPI_Get_address,(const_cast<void*>(p), &a));
#else
     HBOOST_MPI_CHECK_RESULT(MPI_Address,(const_cast<void*>(p), &a));
#endif
      addresses.push_back(a-origin);
      types.push_back(t);
      lengths.push_back(l);
    }

    std::vector<MPI_Aint> addresses;
    std::vector<MPI_Datatype> types;
    std::vector<int> lengths;

    bool is_committed;
    MPI_Datatype datatype_;
    MPI_Aint origin;
};


} } } // end namespace hboost::mpi::detail


#endif // HBOOST_MPI_DETAIL_MPI_DATATYPE_OPRIMITIVE_HPP
