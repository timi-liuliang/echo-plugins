// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_UNWRAP_TYPE_ID_DWA2004722_HPP
# define HBOOST_UNWRAP_TYPE_ID_DWA2004722_HPP

# include <hboost/python/type_id.hpp>

# include <hboost/mpl/bool.hpp>

namespace hboost { namespace python {

template <class T> class wrapper;

namespace detail { 

template <class T>
inline type_info unwrap_type_id(T*, ...)
{
    return type_id<T>();
}

template <class U, class T>
inline type_info unwrap_type_id(U*, wrapper<T>*)
{
    return type_id<T>();
}

}}} // namespace hboost::python::detail

#endif // HBOOST_UNWRAP_TYPE_ID_DWA2004722_HPP
