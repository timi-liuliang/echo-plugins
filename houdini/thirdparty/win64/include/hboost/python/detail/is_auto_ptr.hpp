// Copyright David Abrahams 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_IS_AUTO_PTR_DWA2003224_HPP
# define HBOOST_IS_AUTO_PTR_DWA2003224_HPP

# ifndef HBOOST_NO_AUTO_PTR
#  include <hboost/python/detail/is_xxx.hpp>
#  include <memory>
# endif

namespace hboost { namespace python { namespace detail { 

# if !defined(HBOOST_NO_AUTO_PTR)

HBOOST_PYTHON_IS_XXX_DEF(auto_ptr, std::auto_ptr, 1)

# else

template <class T>
struct is_auto_ptr : mpl::false_
{
};

# endif
    
}}} // namespace hboost::python::detail

#endif // HBOOST_IS_AUTO_PTR_DWA2003224_HPP
