// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CLASS_FWD_DWA200222_HPP
# define HBOOST_CLASS_FWD_DWA200222_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/detail/not_specified.hpp>

namespace hboost { namespace python { 

template <
    class T // class being wrapped
    // arbitrarily-ordered optional arguments. Full qualification needed for MSVC6
    , class X1 = ::hboost::python::detail::not_specified
    , class X2 = ::hboost::python::detail::not_specified
    , class X3 = ::hboost::python::detail::not_specified
    >
class class_;

}} // namespace hboost::python

#endif // HBOOST_CLASS_FWD_DWA200222_HPP
