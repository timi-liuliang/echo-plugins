// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_VALUE_ARG_DWA2004312_HPP
# define HBOOST_VALUE_ARG_DWA2004312_HPP

# include <hboost/python/detail/copy_ctor_mutates_rhs.hpp>
# include <hboost/mpl/if.hpp>
# include <hboost/type_traits/add_reference.hpp>
# include <hboost/type_traits/add_const.hpp>

namespace hboost { namespace python { namespace detail { 

template <class T>
struct value_arg
  : mpl::if_<
        copy_ctor_mutates_rhs<T>
      , T
      , typename add_reference<
            typename add_const<T>::type
        >::type
  >
{};
  
}}} // namespace hboost::python::detail

#endif // HBOOST_VALUE_ARG_DWA2004312_HPP
