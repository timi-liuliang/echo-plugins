// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CLASS_DETAIL_DWA200295_HPP
# define HBOOST_CLASS_DETAIL_DWA200295_HPP

# include <hboost/python/handle.hpp>
# include <hboost/python/type_id.hpp>

namespace hboost { namespace python { namespace objects { 

HBOOST_PYTHON_DECL type_handle registered_class_object(type_info id);
HBOOST_PYTHON_DECL type_handle class_metatype();
HBOOST_PYTHON_DECL type_handle class_type();

}}} // namespace hboost::python::object

#endif // HBOOST_CLASS_DETAIL_DWA200295_HPP
