// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_SLICE_NIL_DWA2002620_HPP
# define HBOOST_SLICE_NIL_DWA2002620_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/object_core.hpp>

namespace hboost { namespace python { namespace api {

class slice_nil : public object
{
 public:
    slice_nil() : object() {}
};

# ifndef _ // Watch out for GNU gettext users, who #define _(x)
static const slice_nil _ = slice_nil();
# endif 

template <class T>
struct slice_bound
{
    typedef object type;
};

template <>
struct slice_bound<slice_nil>
{
    typedef slice_nil type;
};

}

using api::slice_nil;
# ifndef _ // Watch out for GNU gettext users, who #define _(x)
using api::_;
# endif 

}} // namespace hboost::python

#endif // HBOOST_SLICE_NIL_DWA2002620_HPP
