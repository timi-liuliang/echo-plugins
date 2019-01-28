// (C) Copyright 2005 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_PACKED_IPRIMITIVE_HPP
#define HBOOST_MPI_PACKED_IPRIMITIVE_HPP

#include <hboost/mpi/config.hpp>
#include <cstddef> // size_t
#include <hboost/config.hpp>
#include <hboost/mpi/datatype.hpp>
#include <hboost/mpi/exception.hpp>
#include <hboost/assert.hpp>
#include <hboost/serialization/array.hpp>
#include <hboost/serialization/detail/get_data.hpp>
#include <vector>
#include <hboost/mpi/allocator.hpp>

namespace hboost { namespace mpi {

/// deserialization using MPI_Unpack

class HBOOST_MPI_DECL packed_iprimitive
{
public:
    /// the type of the buffer from which the data is unpacked upon deserialization
    typedef std::vector<char, allocator<char> > buffer_type;

    packed_iprimitive(buffer_type & b, MPI_Comm const & comm, int position = 0)
         : buffer_(b),
           comm(comm),
           position(position)
        {
        }

    void* address ()
    {
      return &buffer_[0];
    }

    void const* address () const
    {
      return &buffer_[0];
    }

    const std::size_t& size() const
    {
      return size_ = buffer_.size();
    }

    void resize(std::size_t s)
    {
      buffer_.resize(s);
    }

    void load_binary(void *address, std::size_t count)
        {
          load_impl(address,MPI_BYTE,count);
        }

    // fast saving of arrays of fundamental types
    template<class T>
    void load_array(serialization::array_wrapper<T> const& x, unsigned int /* file_version */)
    {
      if (x.count())
        load_impl(x.address(), get_mpi_datatype(*x.address()), x.count());
    }

/*
    template<class T>
    void load(serialization::array_wrapper<T> const& x)
    {
      load_array(x,0u);
    }
*/

    typedef is_mpi_datatype<mpl::_1> use_array_optimization;

    // default saving of primitives.
    template<class T>
    void load( T & t)
    {
      load_impl(&t, get_mpi_datatype(t), 1);
    }

    template<class CharType>
    void load(std::basic_string<CharType> & s)
    {
        unsigned int l;
        load(l);
        s.resize(l);
        // note breaking a rule here - could be a problem on some platform
        if (l)
          load_impl(const_cast<CharType *>(s.data()),
                    get_mpi_datatype(CharType()),l);
    }

private:

    void load_impl(void * p, MPI_Datatype t, int l)
    {
      HBOOST_MPI_CHECK_RESULT(MPI_Unpack,
        (const_cast<char*>(hboost::serialization::detail::get_data(buffer_)), buffer_.size(), &position, p, l, t, comm));
    }

    buffer_type & buffer_;
    mutable std::size_t size_;
    MPI_Comm comm;
    int position;
};

} } // end namespace hboost::mpi

#endif // HBOOST_MPI_PACKED_IPRIMITIVE_HPP
