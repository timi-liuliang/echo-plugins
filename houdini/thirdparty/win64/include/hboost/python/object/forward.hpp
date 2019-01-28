// Copyright David Abrahams 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_FORWARD_DWA20011215_HPP
# define HBOOST_FORWARD_DWA20011215_HPP

# include <hboost/mpl/if.hpp>
# include <hboost/type_traits/is_scalar.hpp>
# include <hboost/type_traits/add_const.hpp>
# include <hboost/type_traits/add_reference.hpp>
# include <hboost/ref.hpp>
# include <hboost/python/detail/value_arg.hpp>
# include <hboost/python/detail/copy_ctor_mutates_rhs.hpp>
# include <hboost/mpl/or.hpp>

namespace hboost { namespace python { namespace objects { 

// Very much like hboost::reference_wrapper<T>, except that in this
// case T can be a reference already without causing a
// reference-to-reference error.
template <class T>
struct reference_to_value
{
    typedef typename add_reference<typename add_const<T>::type>::type reference;
    
    reference_to_value(reference x) : m_value(x) {}
    reference get() const { return m_value; }
 private:
    reference m_value;
};

// A little metaprogram which selects the type to pass through an
// intermediate forwarding function when the destination argument type
// is T.
template <class T>
struct forward
    : mpl::if_<
          mpl::or_<python::detail::copy_ctor_mutates_rhs<T>, is_scalar<T> >
        , T
        , reference_to_value<T>
      >
{
};

template<typename T>
struct unforward
{
    typedef typename unwrap_reference<T>::type& type;
};

template<typename T>
struct unforward<reference_to_value<T> >
{
    typedef T type;
};

template <typename T>
struct unforward_cref
  : python::detail::value_arg<
        typename unwrap_reference<T>::type
    >
{
};

template<typename T>
struct unforward_cref<reference_to_value<T> >
  : add_reference<typename add_const<T>::type>
{
};


template <class T>
typename reference_to_value<T>::reference
do_unforward(reference_to_value<T> const& x, int)
{
    return x.get();
}

template <class T>
typename reference_wrapper<T>::type&
do_unforward(reference_wrapper<T> const& x, int)
{
    return x.get();
}

template <class T>
T const& do_unforward(T const& x, ...)
{
    return x;
}

}}} // namespace hboost::python::objects

#endif // HBOOST_FORWARD_DWA20011215_HPP
