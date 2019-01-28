// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_COPY_CONST_REFERENCE_DWA2002131_HPP
# define HBOOST_COPY_CONST_REFERENCE_DWA2002131_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/detail/indirect_traits.hpp>
# include <hboost/mpl/if.hpp>
# include <hboost/python/to_python_value.hpp>

namespace hboost { namespace python { 

namespace detail
{
  template <class R>
  struct copy_const_reference_expects_a_const_reference_return_type
# if defined(__GNUC__) || defined(__EDG__)
  {}
# endif
  ;
}

template <class T> struct to_python_value;

struct copy_const_reference
{
    template <class T>
    struct apply
    {
        typedef typename mpl::if_c<
            indirect_traits::is_reference_to_const<T>::value
          , to_python_value<T>
          , detail::copy_const_reference_expects_a_const_reference_return_type<T>
        >::type type;
    };
};


}} // namespace hboost::python

#endif // HBOOST_COPY_CONST_REFERENCE_DWA2002131_HPP
