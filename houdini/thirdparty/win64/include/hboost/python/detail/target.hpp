#if !defined(HBOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# ifndef HBOOST_TARGET_DWA2002521_HPP
#  define HBOOST_TARGET_DWA2002521_HPP

#  include <hboost/python/detail/preprocessor.hpp>

#  include <hboost/type.hpp>

#  include <hboost/preprocessor/comma_if.hpp>
#  include <hboost/preprocessor/if.hpp>
#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/debug/line.hpp>
#  include <hboost/preprocessor/enum_params.hpp>
#  include <hboost/preprocessor/repetition/enum_trailing_params.hpp>

namespace hboost { namespace python { namespace detail {

#  define HBOOST_PP_ITERATION_PARAMS_1                                                                   \
    (4, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/detail/target.hpp>, HBOOST_PYTHON_FUNCTION_POINTER))
#  include HBOOST_PP_ITERATE()

#  define HBOOST_PP_ITERATION_PARAMS_1                                                                    \
    (4, (0, HBOOST_PYTHON_CV_COUNT - 1, <hboost/python/detail/target.hpp>, HBOOST_PYTHON_POINTER_TO_MEMBER))
#  include HBOOST_PP_ITERATE()

template <class R, class T>
T& (* target(R (T::*)) )() { return 0; }

}}} // namespace hboost::python::detail

# endif // HBOOST_TARGET_DWA2002521_HPP

/* --------------- function pointers --------------- */
// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1 && HBOOST_PP_ITERATION_FLAGS() == HBOOST_PYTHON_FUNCTION_POINTER
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, target.hpp(function_pointers))
# endif 

# define N HBOOST_PP_ITERATION()

template <class R HBOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class A)>
HBOOST_PP_IF(N, A0, void)(* target(R (*)(HBOOST_PP_ENUM_PARAMS_Z(1, N, A))) )()
{
    return 0;
}

# undef N

/* --------------- pointers-to-members --------------- */
#elif HBOOST_PP_ITERATION_DEPTH() == 1 && HBOOST_PP_ITERATION_FLAGS() == HBOOST_PYTHON_POINTER_TO_MEMBER
// Outer over cv-qualifiers

# define HBOOST_PP_ITERATION_PARAMS_2 (3, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/detail/target.hpp>))
# include HBOOST_PP_ITERATE()

#elif HBOOST_PP_ITERATION_DEPTH() == 2
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, target.hpp(pointers-to-members))
# endif 
// Inner over arities

# define N HBOOST_PP_ITERATION()
# define Q HBOOST_PYTHON_CV_QUALIFIER(HBOOST_PP_RELATIVE_ITERATION(1))

template <class R, class T HBOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class A)>
T& (* target(R (T::*)(HBOOST_PP_ENUM_PARAMS_Z(1, N, A)) Q) )()
{
    return 0;
}

# undef N
# undef Q

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif
