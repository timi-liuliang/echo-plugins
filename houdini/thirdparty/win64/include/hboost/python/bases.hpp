// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_BASES_DWA2002321_HPP
# define HBOOST_BASES_DWA2002321_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/type_traits/object_traits.hpp>
# include <hboost/python/detail/type_list.hpp>
# include <hboost/mpl/if.hpp>
# include <hboost/mpl/bool.hpp>
# include <hboost/preprocessor/enum_params_with_a_default.hpp>
# include <hboost/preprocessor/enum_params.hpp>

namespace hboost { namespace python { 

# define HBOOST_PYTHON_BASE_PARAMS HBOOST_PP_ENUM_PARAMS_Z(1, HBOOST_PYTHON_MAX_BASES, Base)

  // A type list for specifying bases
  template < HBOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(HBOOST_PYTHON_MAX_BASES, typename Base, mpl::void_) >
  struct bases : detail::type_list< HBOOST_PYTHON_BASE_PARAMS >::type
  {};

  namespace detail
  {
    template <class T> struct specifies_bases
        : mpl::false_
    {
    };
    
    template < HBOOST_PP_ENUM_PARAMS_Z(1, HBOOST_PYTHON_MAX_BASES, class Base) >
    struct specifies_bases< bases< HBOOST_PYTHON_BASE_PARAMS > >
        : mpl::true_
    {
    };
    template <class T, class Prev = bases<> >
    struct select_bases
        : mpl::if_<
                specifies_bases<T>
                , T
                , Prev
          >
    {
    };
  }
# undef HBOOST_PYTHON_BASE_PARAMS
}} // namespace hboost::python

#endif // HBOOST_BASES_DWA2002321_HPP
