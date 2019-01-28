// (C) Copyright 2005 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_DETAIL_TYPE_MPI_DATATYPE_CACHE_HPP
#define HBOOST_MPI_DETAIL_TYPE_MPI_DATATYPE_CACHE_HPP

#include <hboost/mpi/datatype_fwd.hpp>
#include <hboost/mpi/detail/mpi_datatype_oarchive.hpp>
#include <hboost/mpi/exception.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/noncopyable.hpp>
#include <typeinfo>

// The std::type_info::before function in Visual C++ 8.0 (and probably earlier)
// incorrectly returns an "int" instead of a "bool". Then the compiler has the
// audacity to complain when that "int" is converted to a "bool". Silence
// this warning.
#ifdef HBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4800)
#endif

namespace hboost { namespace mpi { namespace detail {

/// @brief comparison function object for two std::type_info pointers
///
/// is implemented using the before() member function of the std::type_info
/// class

struct type_info_compare
{
  bool operator()(std::type_info const* lhs, std::type_info const* rhs) const
  {
    return lhs->before(*rhs);
  }
};


/// @brief a map of MPI data types, indexed by their type_info
///
///
class HBOOST_MPI_DECL mpi_datatype_map
 : public hboost::noncopyable
{
  struct implementation;

  implementation *impl;

public:
  mpi_datatype_map();
  ~mpi_datatype_map();

  template <class T>
  MPI_Datatype datatype(const T& x = T(), typename hboost::enable_if<is_mpi_builtin_datatype<T> >::type* =0)
  {
    return get_mpi_datatype<T>(x);
  }

  template <class T>
  MPI_Datatype datatype(const T& x =T(), typename hboost::disable_if<is_mpi_builtin_datatype<T> >::type* =0 )
  {
    HBOOST_MPL_ASSERT((is_mpi_datatype<T>));

    // check whether the type already exists
    std::type_info const* t = &typeid(T);
    MPI_Datatype datatype = get(t);
    if (datatype == MPI_DATATYPE_NULL) {
      // need to create a type
      mpi_datatype_oarchive ar(x);
      datatype = ar.get_mpi_datatype();
      set(t, datatype);
    }

    return datatype;
  }
  
  void clear(); 

private:
  MPI_Datatype get(const std::type_info* t);
  void set(const std::type_info* t, MPI_Datatype datatype);
};

/// Retrieve the MPI datatype cache
HBOOST_MPI_DECL mpi_datatype_map& mpi_datatype_cache();

} } } // end namespace hboost::mpi::detail

#ifdef HBOOST_MSVC
#  pragma warning(pop)
#endif

#endif // HBOOST_MPI_DETAIL_TYPE_MPI_DATATYPE_CACHE_HPP
