# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_ARRAY_INSERT_HPP
# define HBOOST_PREPROCESSOR_ARRAY_INSERT_HPP
#
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/array/elem.hpp>
# include <hboost/preprocessor/array/push_back.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/comparison/not_equal.hpp>
# include <hboost/preprocessor/control/deduce_d.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_ARRAY_INSERT */
#
# define HBOOST_PP_ARRAY_INSERT(array, i, elem) HBOOST_PP_ARRAY_INSERT_I(HBOOST_PP_DEDUCE_D(), array, i, elem)
# define HBOOST_PP_ARRAY_INSERT_I(d, array, i, elem) HBOOST_PP_ARRAY_INSERT_D(d, array, i, elem)
#
# /* HBOOST_PP_ARRAY_INSERT_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ARRAY_INSERT_D(d, array, i, elem) HBOOST_PP_TUPLE_ELEM(5, 3, HBOOST_PP_WHILE_ ## d(HBOOST_PP_ARRAY_INSERT_P, HBOOST_PP_ARRAY_INSERT_O, (0, i, elem, (0, ()), array)))
# else
#    define HBOOST_PP_ARRAY_INSERT_D(d, array, i, elem) HBOOST_PP_ARRAY_INSERT_D_I(d, array, i, elem)
#    define HBOOST_PP_ARRAY_INSERT_D_I(d, array, i, elem) HBOOST_PP_TUPLE_ELEM(5, 3, HBOOST_PP_WHILE_ ## d(HBOOST_PP_ARRAY_INSERT_P, HBOOST_PP_ARRAY_INSERT_O, (0, i, elem, (0, ()), array)))
# endif
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_ARRAY_INSERT_P(d, state) HBOOST_PP_ARRAY_INSERT_P_I state
# else
#    define HBOOST_PP_ARRAY_INSERT_P(d, state) HBOOST_PP_ARRAY_INSERT_P_I(nil, nil, nil, HBOOST_PP_TUPLE_ELEM(5, 3, state), HBOOST_PP_TUPLE_ELEM(5, 4, state))
# endif
#
# define HBOOST_PP_ARRAY_INSERT_P_I(_i, _ii, _iii, res, arr) HBOOST_PP_NOT_EQUAL(HBOOST_PP_ARRAY_SIZE(res), HBOOST_PP_INC(HBOOST_PP_ARRAY_SIZE(arr)))
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_ARRAY_INSERT_O(d, state) HBOOST_PP_ARRAY_INSERT_O_I state
# else
#    define HBOOST_PP_ARRAY_INSERT_O(d, state) HBOOST_PP_ARRAY_INSERT_O_I(HBOOST_PP_TUPLE_ELEM(5, 0, state), HBOOST_PP_TUPLE_ELEM(5, 1, state), HBOOST_PP_TUPLE_ELEM(5, 2, state), HBOOST_PP_TUPLE_ELEM(5, 3, state), HBOOST_PP_TUPLE_ELEM(5, 4, state))
# endif
#
# define HBOOST_PP_ARRAY_INSERT_O_I(n, i, elem, res, arr) (HBOOST_PP_IIF(HBOOST_PP_NOT_EQUAL(HBOOST_PP_ARRAY_SIZE(res), i), HBOOST_PP_INC(n), n), i, elem, HBOOST_PP_ARRAY_PUSH_BACK(res, HBOOST_PP_IIF(HBOOST_PP_NOT_EQUAL(HBOOST_PP_ARRAY_SIZE(res), i), HBOOST_PP_ARRAY_ELEM(n, arr), elem)), arr)
#
# endif
