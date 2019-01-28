#if !defined(HBOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
# ifndef HBOOST_CALL_METHOD_DWA2002411_HPP
#  define HBOOST_CALL_METHOD_DWA2002411_HPP

# include <hboost/python/detail/prefix.hpp>

#  include <hboost/type.hpp>

#  include <hboost/python/converter/arg_to_python.hpp>
#  include <hboost/python/converter/return_from_python.hpp>
#  include <hboost/python/detail/preprocessor.hpp>
#  include <hboost/python/detail/void_return.hpp>

#  include <hboost/preprocessor/comma_if.hpp>
#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/repeat.hpp>
#  include <hboost/preprocessor/debug/line.hpp>
#  include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#  include <hboost/preprocessor/repetition/enum_binary_params.hpp>

namespace hboost { namespace python {

# define HBOOST_PYTHON_FAST_ARG_TO_PYTHON_GET(z, n, _) \
    , converter::arg_to_python<A##n>(a##n).get()

# define HBOOST_PP_ITERATION_PARAMS_1 (3, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/call_method.hpp>))
# include HBOOST_PP_ITERATE()

# undef HBOOST_PYTHON_FAST_ARG_TO_PYTHON_GET

}} // namespace hboost::python

# endif // HBOOST_CALL_METHOD_DWA2002411_HPP

// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, call_method.hpp)
# endif 

# define N HBOOST_PP_ITERATION()

template <
    class R
    HBOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class A)
    >
typename detail::returnable<R>::type
call_method(PyObject* self, char const* name
    HBOOST_PP_COMMA_IF(N) HBOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, const& a)
    , hboost::type<R>* = 0
    )
{
    PyObject* const result = 
        PyEval_CallMethod(
            self
            , const_cast<char*>(name)
            , const_cast<char*>("(" HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_FIXED, "O") ")")
            HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_FAST_ARG_TO_PYTHON_GET, nil)
            );
    
    // This conversion *must not* be done in the same expression as
    // the call, because, in the special case where the result is a
    // reference a Python object which was created by converting a C++
    // argument for passing to PyEval_CallFunction, its reference
    // count will be 2 until the end of the full expression containing
    // the conversion, and that interferes with dangling
    // pointer/reference detection.
    converter::return_from_python<R> converter;
    return converter(result);
}

# undef N

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif // HBOOST_PP_IS_ITERATING
