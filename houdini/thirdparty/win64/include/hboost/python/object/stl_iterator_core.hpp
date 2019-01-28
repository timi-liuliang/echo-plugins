// Copyright Eric Niebler 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef STL_ITERATOR_CORE_EAN20051028_HPP
# define STL_ITERATOR_CORE_EAN20051028_HPP

# include <hboost/python/object_fwd.hpp>
# include <hboost/python/handle_fwd.hpp>

namespace hboost { namespace python { namespace objects {

struct HBOOST_PYTHON_DECL stl_input_iterator_impl
{
    stl_input_iterator_impl();
    stl_input_iterator_impl(hboost::python::object const &ob);
    void increment();
    bool equal(stl_input_iterator_impl const &that) const;
    hboost::python::handle<> const &current() const;
private:
    hboost::python::object it_;
    hboost::python::handle<> ob_;
};

}}} // namespace hboost::python::object

#endif // STL_ITERATOR_CORE_EAN20051028_HPP
