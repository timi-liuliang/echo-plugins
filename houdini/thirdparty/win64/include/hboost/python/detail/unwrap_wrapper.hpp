// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_UNWRAP_WRAPPER_DWA2004723_HPP
# define HBOOST_UNWRAP_WRAPPER_DWA2004723_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/detail/is_wrapper.hpp>
# include <hboost/mpl/eval_if.hpp>
# include <hboost/mpl/identity.hpp>

namespace hboost { namespace python { namespace detail { 

template <class T>
struct unwrap_wrapper_helper
{
    typedef typename T::_wrapper_wrapped_type_ type;
};

template <class T>
struct unwrap_wrapper_
  : mpl::eval_if<is_wrapper<T>,unwrap_wrapper_helper<T>,mpl::identity<T> >
{};

template <class T>
typename unwrap_wrapper_<T>::type*
unwrap_wrapper(T*)
{
    return 0;
}

}}} // namespace hboost::python::detail

#endif // HBOOST_UNWRAP_WRAPPER_DWA2004723_HPP
