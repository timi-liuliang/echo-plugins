// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_ARG_FROM_PYTHON_DWA2002128_HPP
# define HBOOST_ARG_FROM_PYTHON_DWA2002128_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/converter/arg_from_python.hpp>
# if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1400)) \
    || HBOOST_WORKAROUND(HBOOST_INTEL_WIN, HBOOST_TESTED_AT(800))
# include <hboost/type_traits/remove_cv.hpp>
#endif

namespace hboost { namespace python { 

template <class T>
struct arg_from_python
    : converter::select_arg_from_python<
# if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1400)) \
    || HBOOST_WORKAROUND(HBOOST_INTEL_WIN, HBOOST_TESTED_AT(800))
          typename hboost::remove_cv<T>::type
# else
          T
# endif 
      >::type
{
    typedef typename converter::select_arg_from_python<
# if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1400)) \
    || HBOOST_WORKAROUND(HBOOST_INTEL_WIN, HBOOST_TESTED_AT(800))
          typename hboost::remove_cv<T>::type
# else
          T
# endif 
        >::type base;
    
    arg_from_python(PyObject*);
};

// specialization for PyObject*
template <>
struct arg_from_python<PyObject*>
{
    typedef PyObject* result_type;
    
    arg_from_python(PyObject* p) : m_source(p) {}
    bool convertible() const { return true; }
    PyObject* operator()() const { return m_source; }
 private:
    PyObject* m_source;
};

template <>
struct arg_from_python<PyObject* const&>
{
    typedef PyObject* const& result_type;
    
    arg_from_python(PyObject* p) : m_source(p) {}
    bool convertible() const { return true; }
    PyObject*const& operator()() const { return m_source; }
 private:
    PyObject* m_source;
};

//
// implementations
//
template <class T>
inline arg_from_python<T>::arg_from_python(PyObject* source)
    : base(source)
{
}

}} // namespace hboost::python

#endif // HBOOST_ARG_FROM_PYTHON_DWA2002128_HPP
