#if !defined(HBOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# ifndef HBOOST_RESULT_DWA2002521_HPP
#  define HBOOST_RESULT_DWA2002521_HPP

#  include <hboost/type.hpp>

#  include <hboost/python/detail/preprocessor.hpp>

#  include <hboost/type_traits/object_traits.hpp>
#  include <hboost/mpl/if.hpp>

#  include <hboost/preprocessor/comma_if.hpp>
#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/debug/line.hpp>
#  include <hboost/preprocessor/enum_params.hpp>
#  include <hboost/preprocessor/repetition/enum_trailing_params.hpp>

namespace hboost { namespace python { namespace detail {

// Defines a family of overloaded function which, given x, a function
// pointer, member [function] pointer, or an AdaptableFunction object,
// returns a pointer to type<R>*, where R is the result type of
// invoking the result of bind(x).
//
// In order to work around bugs in deficient compilers, if x might be
// an AdaptableFunction object, you must pass OL as a second argument
// to get this to work portably.

#  define HBOOST_PP_ITERATION_PARAMS_1                                                                   \
    (4, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/detail/result.hpp>, HBOOST_PYTHON_FUNCTION_POINTER))
#  include HBOOST_PP_ITERATE()

#  define HBOOST_PP_ITERATION_PARAMS_1                                                                     \
    (4, (0, HBOOST_PYTHON_CV_COUNT - 1, <hboost/python/detail/result.hpp>, HBOOST_PYTHON_POINTER_TO_MEMBER))
#  include HBOOST_PP_ITERATE()

template <class R, class T>
hboost::type<R>* result(R (T::*), int = 0) { return 0; }

#  if (defined(__MWERKS__) && __MWERKS__ < 0x3000)
// This code actually works on all implementations, but why use it when we don't have to?
template <class T>
struct get_result_type
{
    typedef hboost::type<typename T::result_type> type;
};

struct void_type
{
    typedef void type;
};

template <class T>
struct result_result
{
    typedef typename mpl::if_c<
        is_class<T>::value
        , get_result_type<T>
        , void_type
        >::type t1;

    typedef typename t1::type* type;
};

template <class X>
typename result_result<X>::type
result(X const&, short) { return 0; }

#  else // Simpler code for more-capable compilers
template <class X>
hboost::type<typename X::result_type>*
result(X const&, short = 0) { return 0; }

#  endif

}}} // namespace hboost::python::detail

# endif // HBOOST_RESULT_DWA2002521_HPP

/* --------------- function pointers --------------- */
// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1 && HBOOST_PP_ITERATION_FLAGS() == HBOOST_PYTHON_FUNCTION_POINTER
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, result.hpp(function pointers))
# endif

# define N HBOOST_PP_ITERATION()

template <class R HBOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class A)>
hboost::type<R>* result(R (*)(HBOOST_PP_ENUM_PARAMS_Z(1, N, A)), int = 0)
{
    return 0;
}

# undef N

/* --------------- pointers-to-members --------------- */
#elif HBOOST_PP_ITERATION_DEPTH() == 1 && HBOOST_PP_ITERATION_FLAGS() == HBOOST_PYTHON_POINTER_TO_MEMBER
// Outer over cv-qualifiers

# define HBOOST_PP_ITERATION_PARAMS_2 (3, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/detail/result.hpp>))
# include HBOOST_PP_ITERATE()

#elif HBOOST_PP_ITERATION_DEPTH() == 2
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, result.hpp(pointers-to-members))
# endif 
// Inner over arities

# define N HBOOST_PP_ITERATION()
# define Q HBOOST_PYTHON_CV_QUALIFIER(HBOOST_PP_RELATIVE_ITERATION(1))

template <class R, class T HBOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class A)>
hboost::type<R>* result(R (T::*)(HBOOST_PP_ENUM_PARAMS_Z(1, N, A)) Q, int = 0)
{
    return 0;
}

# undef N
# undef Q

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif
