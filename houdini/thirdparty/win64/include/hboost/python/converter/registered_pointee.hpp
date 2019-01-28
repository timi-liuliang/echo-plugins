// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_REGISTERED_POINTEE_DWA2002710_HPP
# define HBOOST_REGISTERED_POINTEE_DWA2002710_HPP
# include <hboost/python/converter/registered.hpp>
# include <hboost/python/converter/pointer_type_id.hpp>
# include <hboost/python/converter/registry.hpp>
# include <hboost/type_traits/transform_traits.hpp>
# include <hboost/type_traits/cv_traits.hpp>

namespace hboost { namespace python { namespace converter { 

struct registration;

template <class T>
struct registered_pointee
    : registered<
        typename remove_pointer<
           typename remove_cv<
              typename remove_reference<T>::type
           >::type
        >::type
    >
{
};
}}} // namespace hboost::python::converter

#endif // HBOOST_REGISTERED_POINTEE_DWA2002710_HPP
