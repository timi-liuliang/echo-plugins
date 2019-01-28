
//  (C) Copyright Joel de Guzman 2003.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef PY_CONTAINER_UTILS_JDG20038_HPP
# define PY_CONTAINER_UTILS_JDG20038_HPP

# include <utility>
# include <hboost/foreach.hpp>
# include <hboost/python/object.hpp>
# include <hboost/python/handle.hpp>
# include <hboost/python/extract.hpp>
# include <hboost/python/stl_iterator.hpp>

namespace hboost { namespace python { namespace container_utils {
        
    template <typename Container>
    void
    extend_container(Container& container, object l)
    {
        typedef typename Container::value_type data_type;
        
        //  l must be iterable
        HBOOST_FOREACH(object elem,
            std::make_pair(
              hboost::python::stl_input_iterator<object>(l),
              hboost::python::stl_input_iterator<object>()
              ))
        {
            extract<data_type const&> x(elem);
            //  try if elem is an exact data_type type
            if (x.check())
            {
                container.push_back(x());
            }
            else
            {
                //  try to convert elem to data_type type
                extract<data_type> x(elem);
                if (x.check())
                {
                    container.push_back(x());
                }
                else
                {
                    PyErr_SetString(PyExc_TypeError, "Incompatible Data Type");
                    throw_error_already_set();
                }
            }
        }          
    }

}}} // namespace hboost::python::container_utils

#endif
