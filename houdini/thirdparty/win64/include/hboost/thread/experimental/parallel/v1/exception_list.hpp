#ifndef HBOOST_THREAD_EXPERIMENTAL_PARALLEL_V1_EXCEPTION_LIST_HPP
#define HBOOST_THREAD_EXPERIMENTAL_PARALLEL_V1_EXCEPTION_LIST_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/thread for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/experimental/parallel/v1/inline_namespace.hpp>

#include <hboost/exception_ptr.hpp>
#include <exception>
#include <list>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
namespace experimental
{
namespace parallel
{
HBOOST_THREAD_INLINE_NAMESPACE(v1)
{

  class HBOOST_SYMBOL_VISIBLE exception_list: public std::exception
  {
    typedef std::list<exception_ptr> exception_ptr_list;
    exception_ptr_list list_;
  public:
    typedef exception_ptr_list::const_iterator const_iterator;

    ~exception_list() HBOOST_NOEXCEPT_OR_NOTHROW {}

    void add(exception_ptr const& e)
    {
      list_.push_back(e);
    }
    size_t size() const HBOOST_NOEXCEPT
    {
      return list_.size();
    }
    const_iterator begin() const HBOOST_NOEXCEPT
    {
      return list_.begin();
    }
    const_iterator end() const HBOOST_NOEXCEPT
    {
      return list_.end();
    }
    const char* what() const HBOOST_NOEXCEPT_OR_NOTHROW
    {
      return "exception_list";
    }

  };
}

} // parallel
} // experimental
} // boost
#include <hboost/config/abi_suffix.hpp>

#endif
