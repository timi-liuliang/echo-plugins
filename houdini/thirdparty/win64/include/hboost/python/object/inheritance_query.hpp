// Copyright David Abrahams 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_INHERITANCE_QUERY_DWA2003520_HPP
# define HBOOST_INHERITANCE_QUERY_DWA2003520_HPP

# include <hboost/python/type_id.hpp>

namespace hboost { namespace python { namespace objects {

HBOOST_PYTHON_DECL void* find_static_type(void* p, type_info src, type_info dst);
HBOOST_PYTHON_DECL void* find_dynamic_type(void* p, type_info src, type_info dst);

}}} // namespace hboost::python::object

#endif // HBOOST_INHERITANCE_QUERY_DWA2003520_HPP
