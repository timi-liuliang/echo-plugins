// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_IMPLICIT_DWA2002325_HPP
# define HBOOST_IMPLICIT_DWA2002325_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/type.hpp>
# include <hboost/python/converter/implicit.hpp>
# include <hboost/python/converter/registry.hpp>
#ifndef HBOOST_PYTHON_NO_PY_SIGNATURES
# include <hboost/python/converter/pytype_function.hpp>
#endif
# include <hboost/python/type_id.hpp>

namespace hboost { namespace python { 

template <class Source, class Target>
void implicitly_convertible(hboost::type<Source>* = 0, hboost::type<Target>* = 0)
{
    typedef converter::implicit<Source,Target> functions;
    
    converter::registry::push_back(
          &functions::convertible
        , &functions::construct
        , type_id<Target>()
#ifndef HBOOST_PYTHON_NO_PY_SIGNATURES
        , &converter::expected_from_python_type_direct<Source>::get_pytype
#endif
        );
}

}} // namespace hboost::python

#endif // HBOOST_IMPLICIT_DWA2002325_HPP
