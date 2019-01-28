// (C) Copyright 2005 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_DETAIL_IGNORE_IPRIMITIVE_HPP
#define HBOOST_MPI_DETAIL_IGNORE_IPRIMITIVE_HPP

#include <hboost/config.hpp>
#include <hboost/mpi/datatype.hpp>
#include <hboost/serialization/array.hpp>


namespace hboost { namespace mpi { namespace detail {

/// @brief a minimal input archive, which ignores any load
///
/// This class implements a minimal input archive, probably an input archive
/// archetype, doing nothing at any load. It's use, besides acting as an
/// archetype is as a base class to implement special archives that ignore
/// loading of most types

class ignore_iprimitive
{
public:
    /// a trivial default constructor
    ignore_iprimitive()
    {}


        /// don't do anything when loading binary data
    void load_binary(void *, std::size_t )
        {}

        /// don't do anything when loading arrays
    template<class T>
    void load_array(serialization::array_wrapper<T> &, unsigned int )
    {}

    typedef is_mpi_datatype<mpl::_1> use_array_optimization;

        /// don't do anything when loading primitive types
    template<class T>
    void load(T &)
    {
    }
};

} } } // end namespace hboost::mpi::detail

#endif // HBOOST_MPI_DETAIL_IGNORE_IPRIMITIVE_HPP
