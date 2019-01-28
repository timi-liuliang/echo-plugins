// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_HAS_BACK_REFERENCE_DWA2002323_HPP
# define HBOOST_HAS_BACK_REFERENCE_DWA2002323_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/mpl/bool.hpp>

namespace hboost { namespace python { 

// traits class which users can specialize to indicate that a class
// contains a back-reference to its owning PyObject*
template <class T>
struct has_back_reference
  : mpl::false_
{
};


}} // namespace hboost::python

#endif // HBOOST_HAS_BACK_REFERENCE_DWA2002323_HPP
