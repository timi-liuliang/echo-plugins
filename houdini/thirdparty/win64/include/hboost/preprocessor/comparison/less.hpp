# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_COMPARISON_LESS_HPP
# define HBOOST_PREPROCESSOR_COMPARISON_LESS_HPP
#
# include <hboost/preprocessor/comparison/less_equal.hpp>
# include <hboost/preprocessor/comparison/not_equal.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/logical/bitand.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
#
# /* HBOOST_PP_LESS */
#
# if HBOOST_PP_CONFIG_FLAGS() & (HBOOST_PP_CONFIG_MWCC() | HBOOST_PP_CONFIG_DMC())
#    define HBOOST_PP_LESS(x, y) HBOOST_PP_BITAND(HBOOST_PP_NOT_EQUAL(x, y), HBOOST_PP_LESS_EQUAL(x, y))
# elif ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LESS(x, y) HBOOST_PP_IIF(HBOOST_PP_NOT_EQUAL(x, y), HBOOST_PP_LESS_EQUAL, 0 HBOOST_PP_TUPLE_EAT_2)(x, y)
# else
#    define HBOOST_PP_LESS(x, y) HBOOST_PP_LESS_I(x, y)
#    define HBOOST_PP_LESS_I(x, y) HBOOST_PP_IIF(HBOOST_PP_NOT_EQUAL(x, y), HBOOST_PP_LESS_EQUAL, 0 HBOOST_PP_TUPLE_EAT_2)(x, y)
# endif
#
# /* HBOOST_PP_LESS_D */
#
# if HBOOST_PP_CONFIG_FLAGS() & (HBOOST_PP_CONFIG_MWCC() | HBOOST_PP_CONFIG_DMC())
#    define HBOOST_PP_LESS_D(d, x, y) HBOOST_PP_BITAND(HBOOST_PP_NOT_EQUAL(x, y), HBOOST_PP_LESS_EQUAL_D(d, x, y))
# elif ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LESS_D(d, x, y) HBOOST_PP_IIF(HBOOST_PP_NOT_EQUAL(x, y), HBOOST_PP_LESS_EQUAL_D, 0 HBOOST_PP_TUPLE_EAT_3)(d, x, y)
# else
#    define HBOOST_PP_LESS_D(d, x, y) HBOOST_PP_LESS_D_I(d, x, y)
#    define HBOOST_PP_LESS_D_I(d, x, y) HBOOST_PP_IIF(HBOOST_PP_NOT_EQUAL(x, y), HBOOST_PP_LESS_EQUAL_D, 0 HBOOST_PP_TUPLE_EAT_3)(d, x, y)
# endif
#
# endif
