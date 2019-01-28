// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_REFERENCE_EXISTING_OBJECT_DWA200222_HPP
# define HBOOST_REFERENCE_EXISTING_OBJECT_DWA200222_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/detail/indirect_traits.hpp>
# include <hboost/mpl/if.hpp>
# include <hboost/python/to_python_indirect.hpp>
# include <hboost/type_traits/composite_traits.hpp>

namespace hboost { namespace python { 

namespace detail
{
  template <class R>
  struct reference_existing_object_requires_a_pointer_or_reference_return_type
# if defined(__GNUC__) || defined(__EDG__)
  {}
# endif
  ;
}

template <class T> struct to_python_value;

struct reference_existing_object
{
    template <class T>
    struct apply
    {
        HBOOST_STATIC_CONSTANT(
            bool, ok = is_pointer<T>::value || is_reference<T>::value);
        
        typedef typename mpl::if_c<
            ok
            , to_python_indirect<T, detail::make_reference_holder>
            , detail::reference_existing_object_requires_a_pointer_or_reference_return_type<T>
        >::type type;
    };
};

}} // namespace hboost::python

#endif // HBOOST_REFERENCE_EXISTING_OBJECT_DWA200222_HPP
