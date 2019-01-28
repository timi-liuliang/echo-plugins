// (C) Copyright 2005-2007 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_BINARY_BUFFER_IPRIMITIVE_HPP
#define HBOOST_MPI_BINARY_BUFFER_IPRIMITIVE_HPP

#include <mpi.h>
#include <iostream>
#include <cstddef> // size_t
#include <hboost/config.hpp>
#include <hboost/mpi/exception.hpp>
#include <hboost/assert.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/serialization/array.hpp>
#include <hboost/serialization/is_bitwise_serializable.hpp>
#include <vector>
#include <hboost/mpi/allocator.hpp>
#include <cstring> // for memcpy
#include <cassert>

namespace hboost { namespace mpi {

/// deserialization using MPI_Unpack

class HBOOST_MPI_DECL binary_buffer_iprimitive
{
public:
    /// the type of the buffer from which the data is unpacked upon deserialization
    typedef std::vector<char, allocator<char> > buffer_type;

    binary_buffer_iprimitive(buffer_type & b, MPI_Comm const &, int position = 0)
     : buffer_(b),
       position(position)
    {
    }

    void* address ()
    {
      return &buffer_.front();
    }

    void const* address () const
    {
      return &buffer_.front();
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
      load_impl(address,count);
    }

    // fast saving of arrays of fundamental types
    template<class T>
    void load_array(serialization::array_wrapper<T> const& x, unsigned int /* file_version */)
    {
      HBOOST_MPL_ASSERT((serialization::is_bitwise_serializable<HBOOST_DEDUCED_TYPENAME remove_const<T>::type>));
      if (x.count())
        load_impl(x.address(), sizeof(T)*x.count());
    }

    typedef serialization::is_bitwise_serializable<mpl::_1> use_array_optimization;

    template<class T>
    void load(serialization::array_wrapper<T> const& x)
    {
      load_array(x,0u);
    }

    // default saving of primitives.
    template<class T>
    void load( T & t)
    {
      HBOOST_MPL_ASSERT((serialization::is_bitwise_serializable<HBOOST_DEDUCED_TYPENAME remove_const<T>::type>));
      load_impl(&t, sizeof(T));
    }

    template<class CharType>
    void load(std::basic_string<CharType> & s)
    {
      unsigned int l;
      load(l);
      // borland de-allocator fixup
      #if HBOOST_WORKAROUND(_RWSTD_VER, HBOOST_TESTED_AT(20101))
      if(NULL != s.data())
      #endif
      s.resize(l);
      // note breaking a rule here - could be a problem on some platform
      load_impl(const_cast<char *>(s.data()),l);
    }

private:

    void load_impl(void * p, int l)
    {
      assert(position+l<=static_cast<int>(buffer_.size()));
      if (l)
        std::memcpy(p,&buffer_[position],l);
      position += l;
    }

    buffer_type & buffer_;
    mutable std::size_t size_;
    int position;
};

} } // end namespace hboost::mpi

#endif // HBOOST_MPI_PACKED_IPRIMITIVE_HPP
