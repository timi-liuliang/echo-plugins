//  (C) Copyright David Abrahams 2000.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  The author gratefully acknowleges the support of Dragon Systems, Inc., in
//  producing this work.

//  Revision History:
//  04 Mar 01  Some fixes so it will compile with Intel C++ (Dave Abrahams)

#ifndef HBOOST_CONFIG_DWA052200_H_
# define HBOOST_CONFIG_DWA052200_H_

# include <hboost/config.hpp>
# include <hboost/detail/workaround.hpp>

# ifdef HBOOST_NO_OPERATORS_IN_NAMESPACE
   // A gcc bug forces some symbols into the global namespace
#  define HBOOST_PYTHON_BEGIN_CONVERSION_NAMESPACE
#  define HBOOST_PYTHON_END_CONVERSION_NAMESPACE
#  define HBOOST_PYTHON_CONVERSION
#  define HBOOST_PYTHON_IMPORT_CONVERSION(x) using ::x
# else
#  define HBOOST_PYTHON_BEGIN_CONVERSION_NAMESPACE namespace hboost { namespace python {
#  define HBOOST_PYTHON_END_CONVERSION_NAMESPACE }} // namespace hboost::python
#  define HBOOST_PYTHON_CONVERSION hboost::python
#  define HBOOST_PYTHON_IMPORT_CONVERSION(x) void never_defined() // so we can follow the macro with a ';'
# endif

# if defined(HBOOST_MSVC)

#  pragma warning (disable : 4786) // disable truncated debug symbols
#  pragma warning (disable : 4251) // disable exported dll function
#  pragma warning (disable : 4800) //'int' : forcing value to bool 'true' or 'false'
#  pragma warning (disable : 4275) // non dll-interface class

# elif defined(__ICL) && __ICL < 600 // Intel C++ 5

#  pragma warning(disable: 985) // identifier was truncated in debug information

# endif

// The STLport puts all of the standard 'C' library names in std (as far as the
// user is concerned), but without it you need a fix if you're using MSVC or
// Intel C++
# if defined(HBOOST_NO_STDC_NAMESPACE)
#  define HBOOST_CSTD_
# else
#  define HBOOST_CSTD_ std
# endif

/*****************************************************************************
 *
 *  Set up dll import/export options:
 *
 ****************************************************************************/

// backwards compatibility:
#ifdef HBOOST_PYTHON_STATIC_LIB
#  define HBOOST_PYTHON_STATIC_LINK
# elif !defined(HBOOST_PYTHON_DYNAMIC_LIB)
#  define HBOOST_PYTHON_DYNAMIC_LIB
#endif

#if defined(HBOOST_PYTHON_DYNAMIC_LIB)

#  if !defined(_WIN32) && !defined(__CYGWIN__)                                  \
    && !defined(HBOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY)                         \
    && HBOOST_WORKAROUND(__GNUC__, >= 3) && (__GNUC_MINOR__ >=5 || __GNUC__ > 3)
#    define HBOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY 1
#  endif 

#  if HBOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY
#     if defined(HBOOST_PYTHON_SOURCE)
#        define HBOOST_PYTHON_DECL __attribute__ ((__visibility__("default")))
#        define HBOOST_PYTHON_BUILD_DLL
#     else
#        define HBOOST_PYTHON_DECL
#     endif
#     define HBOOST_PYTHON_DECL_FORWARD
#     define HBOOST_PYTHON_DECL_EXCEPTION __attribute__ ((__visibility__("default")))
#  elif (defined(_WIN32) || defined(__CYGWIN__))
#     if defined(HBOOST_PYTHON_SOURCE)
#        define HBOOST_PYTHON_DECL __declspec(dllexport)
#        define HBOOST_PYTHON_BUILD_DLL
#     else
#        define HBOOST_PYTHON_DECL __declspec(dllimport)
#     endif
#  endif

#endif

#ifndef HBOOST_PYTHON_DECL
#  define HBOOST_PYTHON_DECL
#endif

#ifndef HBOOST_PYTHON_DECL_FORWARD
#  define HBOOST_PYTHON_DECL_FORWARD HBOOST_PYTHON_DECL
#endif

#ifndef HBOOST_PYTHON_DECL_EXCEPTION
#  define HBOOST_PYTHON_DECL_EXCEPTION HBOOST_PYTHON_DECL
#endif

#if HBOOST_WORKAROUND(__DECCXX_VER, HBOOST_TESTED_AT(60590042))
// Replace broken Tru64/cxx offsetof macro
# define HBOOST_PYTHON_OFFSETOF(s_name, s_member) \
        ((size_t)__INTADDR__(&(((s_name *)0)->s_member)))
#else
# define HBOOST_PYTHON_OFFSETOF offsetof
#endif

//  enable automatic library variant selection  ------------------------------// 

#if !defined(HBOOST_PYTHON_SOURCE) && !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_PYTHON_NO_LIB)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define HBOOST_LIB_NAME hboost_python
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#ifdef HBOOST_PYTHON_DYNAMIC_LIB
#  define HBOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <hboost/config/auto_link.hpp>
#endif  // auto-linking disabled

#ifndef HBOOST_PYTHON_NO_PY_SIGNATURES
#define HBOOST_PYTHON_SUPPORTS_PY_SIGNATURES // enables smooth transition
#endif

#endif // HBOOST_CONFIG_DWA052200_H_
