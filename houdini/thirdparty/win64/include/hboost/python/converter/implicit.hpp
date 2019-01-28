// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_IMPLICIT_DWA2002326_HPP
# define HBOOST_IMPLICIT_DWA2002326_HPP

# include <hboost/python/converter/rvalue_from_python_data.hpp>
# include <hboost/python/converter/registrations.hpp>
# include <hboost/python/converter/registered.hpp>

# include <hboost/python/extract.hpp>

namespace hboost { namespace python { namespace converter { 

template <class Source, class Target>
struct implicit
{
    static void* convertible(PyObject* obj)
    {
        // Find a converter which can produce a Source instance from
        // obj. The user has told us that Source can be converted to
        // Target, and instantiating construct() below, ensures that
        // at compile-time.
        return implicit_rvalue_convertible_from_python(obj, registered<Source>::converters)
            ? obj : 0;
    }
      
    static void construct(PyObject* obj, rvalue_from_python_stage1_data* data)
    {
        void* storage = ((rvalue_from_python_storage<Target>*)data)->storage.bytes;

        arg_from_python<Source> get_source(obj);
        bool convertible = get_source.convertible();
        HBOOST_VERIFY(convertible);
        
        new (storage) Target(get_source());
        
        // record successful construction
        data->convertible = storage;
    }
};

}}} // namespace hboost::python::converter

#endif // HBOOST_IMPLICIT_DWA2002326_HPP