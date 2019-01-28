// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_MANAGE_NEW_OBJECT_DWA200222_HPP
# define HBOOST_MANAGE_NEW_OBJECT_DWA200222_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/detail/indirect_traits.hpp>
# include <hboost/mpl/if.hpp>
# include <hboost/python/to_python_indirect.hpp>
# include <hboost/type_traits/composite_traits.hpp>

namespace hboost { namespace python { 

namespace detail
{
  template <class R>
  struct manage_new_object_requires_a_pointer_return_type
# if defined(__GNUC__) || defined(__EDG__)
  {}
# endif
  ;
}

struct manage_new_object
{
    template <class T>
    struct apply
    {
        typedef typename mpl::if_c<
            hboost::is_pointer<T>::value
            , to_python_indirect<T, detail::make_owning_holder>
            , detail::manage_new_object_requires_a_pointer_return_type<T>
        >::type type;
    };
};

}} // namespace hboost::python

#endif // HBOOST_MANAGE_NEW_OBJECT_DWA200222_HPP
