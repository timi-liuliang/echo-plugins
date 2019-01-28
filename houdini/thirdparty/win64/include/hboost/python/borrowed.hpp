// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_BORROWED_DWA2002614_HPP
# define HBOOST_BORROWED_DWA2002614_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/detail/borrowed_ptr.hpp>

namespace hboost { namespace python { 

template <class T>
inline python::detail::borrowed<T>* borrowed(T* p)
{
    return (detail::borrowed<T>*)p;
}
    
}} // namespace hboost::python

#endif // HBOOST_BORROWED_DWA2002614_HPP
