#ifndef HBOOST_PP_IS_ITERATING
// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
# ifndef HBOOST_TYPE_LIST_IMPL_DWA2002913_HPP
#  define HBOOST_TYPE_LIST_IMPL_DWA2002913_HPP

#  include <hboost/python/detail/type_list.hpp>

#  include <hboost/preprocessor/enum_params.hpp>
#  include <hboost/preprocessor/enum_params_with_a_default.hpp>
#  include <hboost/preprocessor/repetition/enum.hpp>
#  include <hboost/preprocessor/comma_if.hpp>
#  include <hboost/preprocessor/arithmetic/sub.hpp>
#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/repetition/enum_trailing.hpp>

namespace hboost { namespace python { namespace detail { 

template <HBOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(HBOOST_PYTHON_LIST_SIZE, class T, mpl::void_)>
struct type_list
    : HBOOST_PP_CAT(mpl::vector,HBOOST_PYTHON_LIST_SIZE)<HBOOST_PP_ENUM_PARAMS_Z(1, HBOOST_PYTHON_LIST_SIZE, T)>
{
};

#  define HBOOST_PP_ITERATION_PARAMS_1                                                                   \
        (3, (0, HBOOST_PP_DEC(HBOOST_PYTHON_LIST_SIZE), <hboost/python/detail/type_list_impl.hpp>))
#  include HBOOST_PP_ITERATE()


}}} // namespace hboost::python::detail

# endif // HBOOST_TYPE_LIST_IMPL_DWA2002913_HPP

#else // HBOOST_PP_IS_ITERATING

# define N HBOOST_PP_ITERATION()
# define HBOOST_PYTHON_VOID_ARGS HBOOST_PP_SUB_D(1,HBOOST_PYTHON_LIST_SIZE,N)

template <
    HBOOST_PP_ENUM_PARAMS_Z(1, N, class T)
    >
struct type_list<
    HBOOST_PP_ENUM_PARAMS_Z(1, N, T)
    HBOOST_PP_COMMA_IF(N)
    HBOOST_PP_ENUM(
        HBOOST_PYTHON_VOID_ARGS, HBOOST_PYTHON_FIXED, mpl::void_)
    >
   : HBOOST_PP_CAT(mpl::vector,N)<HBOOST_PP_ENUM_PARAMS_Z(1, N, T)>
{
};

# undef HBOOST_PYTHON_VOID_ARGS
# undef N

#endif // HBOOST_PP_IS_ITERATING 
