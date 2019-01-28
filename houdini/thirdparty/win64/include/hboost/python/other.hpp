#ifndef HBOOST_OTHER_DWA20020601_HPP
# define HBOOST_OTHER_DWA20020601_HPP

# include <hboost/python/detail/prefix.hpp>
// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# pragma once

# include <hboost/config.hpp>

namespace hboost { namespace python {

template<class T> struct other
{ 
    typedef T type;
};

namespace detail
{
  template<typename T>
  class is_other
  {
   public:
      HBOOST_STATIC_CONSTANT(bool, value = false); 
  };

  template<typename T>
  class is_other<other<T> >
  {
   public:
      HBOOST_STATIC_CONSTANT(bool, value = true);
  };

  template<typename T>
  class unwrap_other
  {
   public:
      typedef T type;
  };

  template<typename T>
  class unwrap_other<other<T> >
  {
   public:
      typedef T type;
  };
}

}} // namespace hboost::python

#endif // #ifndef HBOOST_OTHER_DWA20020601_HPP
