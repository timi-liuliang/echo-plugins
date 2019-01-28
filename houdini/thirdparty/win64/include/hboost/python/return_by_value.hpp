// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_BY_VALUE_DWA20021015_HPP
# define HBOOST_BY_VALUE_DWA20021015_HPP

# include <hboost/python/detail/prefix.hpp>

# include <hboost/python/to_python_value.hpp>
# include <hboost/type_traits/add_reference.hpp>
# include <hboost/type_traits/add_const.hpp>

# include <hboost/python/detail/value_arg.hpp>

namespace hboost { namespace python { 

struct return_by_value
{
    template <class R>
    struct apply
    {
       typedef to_python_value<
           typename detail::value_arg<R>::type
       > type;
    };
};

}} // namespace hboost::python

#endif // HBOOST_BY_VALUE_DWA20021015_HPP
