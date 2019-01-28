// (C) Copyright 2005-2007 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_BINARY_BUFFER_OPRIMITIVE_HPP
#define HBOOST_MPI_BINARY_BUFFER_OPRIMITIVE_HPP

#include <mpi.h>
#include <iostream>
#include <cstddef> // size_t
#include <hboost/config.hpp>

#include <hboost/serialization/array.hpp>
#include <hboost/serialization/is_bitwise_serializable.hpp>
#include <hboost/assert.hpp>
#include <hboost/mpl/assert.hpp>
#include <vector>
#include <hboost/mpi/allocator.hpp>
#include <hboost/mpl/always.hpp>
#include <hboost/type_traits/remove_const.hpp>

namespace hboost { namespace mpi {

/// serialization using binary copy into a buffer

class HBOOST_MPI_DECL binary_buffer_oprimitive
{
public:
    /// the type of the buffer into which the data is packed upon serialization
    typedef std::vector<char, allocator<char> > buffer_type;

    binary_buffer_oprimitive(buffer_type & b, MPI_Comm const &)
     : buffer_(b)
    {
    }

    void const * address() const
    {
      return &buffer_.front();
    }

    const std::size_t& size() const
    {
      return size_ = buffer_.size();
    }

    void save_binary(void const *address, std::size_t count)
    {
      save_impl(address,count);
    }

    // fast saving of arrays
    template<class T>
    void save_array(serialization::array_wrapper<T> const& x, unsigned int /* file_version */)
    {
    
      HBOOST_MPL_ASSERT((serialization::is_bitwise_serializable<HBOOST_DEDUCED_TYPENAME remove_const<T>::type>));
      if (x.count())
        save_impl(x.address(), x.count()*sizeof(T));
    }

    template<class T>
    void save(serialization::array_wrapper<T> const& x)
    {
      save_array(x,0u);
    }

    typedef serialization::is_bitwise_serializable<mpl::_1> use_array_optimization;

    // default saving of primitives.
    template<class T>
    void save(const T & t)
    {
      HBOOST_MPL_ASSERT((serialization::is_bitwise_serializable<HBOOST_DEDUCED_TYPENAME remove_const<T>::type>));
      save_impl(&t, sizeof(T));
    }

    template<class CharType>
    void save(const std::basic_string<CharType> &s)
    {
      unsigned int l = static_cast<unsigned int>(s.size());
      save(l);
      save_impl(s.data(),s.size());
    }

private:

    void save_impl(void const * p, int l)
    {
      char const* ptr = reinterpret_cast<char const*>(p);
      buffer_.insert(buffer_.end(),ptr,ptr+l);
    }

  buffer_type& buffer_;
  mutable std::size_t size_;
};

} } // end namespace hboost::mpi

#endif // HBOOST_MPI_BINARY_BUFFER_OPRIMITIVE_HPP
