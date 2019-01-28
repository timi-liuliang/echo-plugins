// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_MODULE_INIT_DWA20020722_HPP
# define HBOOST_MODULE_INIT_DWA20020722_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/stringize.hpp>

# ifndef HBOOST_PYTHON_MODULE_INIT

namespace hboost { namespace python { namespace detail {

#  if PY_VERSION_HEX >= 0x03000000

HBOOST_PYTHON_DECL PyObject* init_module(PyModuleDef&, void(*)());

#else

HBOOST_PYTHON_DECL PyObject* init_module(char const* name, void(*)());

#endif

}}}

#  if PY_VERSION_HEX >= 0x03000000

#   define _HBOOST_PYTHON_MODULE_INIT(name) \
  PyObject* HBOOST_PP_CAT(PyInit_, name)()  \
  { \
    static PyModuleDef_Base initial_m_base = { \
        PyObject_HEAD_INIT(NULL) \
        0, /* m_init */ \
        0, /* m_index */ \
        0 /* m_copy */ };  \
    static PyMethodDef initial_methods[] = { { 0, 0, 0, 0 } }; \
 \
    static struct PyModuleDef moduledef = { \
        initial_m_base, \
        HBOOST_PP_STRINGIZE(name), \
        0, /* m_doc */ \
        -1, /* m_size */ \
        initial_methods, \
        0,  /* m_reload */ \
        0, /* m_traverse */ \
        0, /* m_clear */ \
        0,  /* m_free */ \
    }; \
 \
    return hboost::python::detail::init_module( \
        moduledef, HBOOST_PP_CAT(init_module_, name) ); \
  } \
  void HBOOST_PP_CAT(init_module_, name)()

#  else

#   define _HBOOST_PYTHON_MODULE_INIT(name)              \
  void HBOOST_PP_CAT(init,name)()                        \
{                                                       \
    hboost::python::detail::init_module(                 \
        HBOOST_PP_STRINGIZE(name),&HBOOST_PP_CAT(init_module_,name)); \
}                                                       \
  void HBOOST_PP_CAT(init_module_,name)()

#  endif

#  if (defined(_WIN32) || defined(__CYGWIN__)) && !defined(HBOOST_PYTHON_STATIC_MODULE)

#   define HBOOST_PYTHON_MODULE_INIT(name)                           \
  void HBOOST_PP_CAT(init_module_,name)();                           \
extern "C" __declspec(dllexport) _HBOOST_PYTHON_MODULE_INIT(name)

#  elif HBOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY

#   define HBOOST_PYTHON_MODULE_INIT(name)                               \
  void HBOOST_PP_CAT(init_module_,name)();                               \
extern "C" __attribute__ ((__visibility__("default"))) _HBOOST_PYTHON_MODULE_INIT(name)

#  else

#   define HBOOST_PYTHON_MODULE_INIT(name)                               \
  void HBOOST_PP_CAT(init_module_,name)();                               \
extern "C" _HBOOST_PYTHON_MODULE_INIT(name)

#  endif

# endif

#endif // HBOOST_MODULE_INIT_DWA20020722_HPP
