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
# ifndef HBOOST_PREPROCESSOR_ARRAY_REMOVE_HPP
# define HBOOST_PREPROCESSOR_ARRAY_REMOVE_HPP
#
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/array/elem.hpp>
# include <hboost/preprocessor/array/push_back.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/comparison/not_equal.hpp>
# include <hboost/preprocessor/control/deduce_d.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_ARRAY_REMOVE */
#
# define HBOOST_PP_ARRAY_REMOVE(array, i) HBOOST_PP_ARRAY_REMOVE_I(HBOOST_PP_DEDUCE_D(), array, i)
# define HBOOST_PP_ARRAY_REMOVE_I(d, array, i) HBOOST_PP_ARRAY_REMOVE_D(d, array, i)
#
# /* HBOOST_PP_ARRAY_REMOVE_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ARRAY_REMOVE_D(d, array, i) HBOOST_PP_TUPLE_ELEM(4, 2, HBOOST_PP_WHILE_ ## d(HBOOST_PP_ARRAY_REMOVE_P, HBOOST_PP_ARRAY_REMOVE_O, (0, i, (0, ()), array)))
# else
#    define HBOOST_PP_ARRAY_REMOVE_D(d, array, i) HBOOST_PP_ARRAY_REMOVE_D_I(d, array, i)
#    define HBOOST_PP_ARRAY_REMOVE_D_I(d, array, i) HBOOST_PP_TUPLE_ELEM(4, 2, HBOOST_PP_WHILE_ ## d(HBOOST_PP_ARRAY_REMOVE_P, HBOOST_PP_ARRAY_REMOVE_O, (0, i, (0, ()), array)))
# endif
#
# define HBOOST_PP_ARRAY_REMOVE_P(d, st) HBOOST_PP_NOT_EQUAL(HBOOST_PP_TUPLE_ELEM(4, 0, st), HBOOST_PP_ARRAY_SIZE(HBOOST_PP_TUPLE_ELEM(4, 3, st)))
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_ARRAY_REMOVE_O(d, st) HBOOST_PP_ARRAY_REMOVE_O_I st
# else
#    define HBOOST_PP_ARRAY_REMOVE_O(d, st) HBOOST_PP_ARRAY_REMOVE_O_I(HBOOST_PP_TUPLE_ELEM(4, 0, st), HBOOST_PP_TUPLE_ELEM(4, 1, st), HBOOST_PP_TUPLE_ELEM(4, 2, st), HBOOST_PP_TUPLE_ELEM(4, 3, st))
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_DMC()
#    define HBOOST_PP_ARRAY_REMOVE_O_I(n, i, res, arr) (HBOOST_PP_INC(n), i, HBOOST_PP_IIF(HBOOST_PP_NOT_EQUAL(n, i), HBOOST_PP_ARRAY_PUSH_BACK, res HBOOST_PP_TUPLE_EAT_2)(res, HBOOST_PP_ARRAY_ELEM(n, arr)), arr)
# else
#    define HBOOST_PP_ARRAY_REMOVE_O_I(n, i, res, arr) (HBOOST_PP_INC(n), i, HBOOST_PP_IIF(HBOOST_PP_NOT_EQUAL(n, i), HBOOST_PP_ARRAY_PUSH_BACK, HBOOST_PP_TUPLE_ELEM_2_0)(res, HBOOST_PP_ARRAY_ELEM(n, arr)), arr)
# endif
#
# endif
