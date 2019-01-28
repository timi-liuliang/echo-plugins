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
# ifndef HBOOST_PREPROCESSOR_ARITHMETIC_DETAIL_DIV_BASE_HPP
# define HBOOST_PREPROCESSOR_ARITHMETIC_DETAIL_DIV_BASE_HPP
#
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/arithmetic/sub.hpp>
# include <hboost/preprocessor/comparison/less_equal.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_DIV_BASE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_DIV_BASE(x, y) HBOOST_PP_WHILE(HBOOST_PP_DIV_BASE_P, HBOOST_PP_DIV_BASE_O, (0, x, y))
# else
#    define HBOOST_PP_DIV_BASE(x, y) HBOOST_PP_DIV_BASE_I(x, y)
#    define HBOOST_PP_DIV_BASE_I(x, y) HBOOST_PP_WHILE(HBOOST_PP_DIV_BASE_P, HBOOST_PP_DIV_BASE_O, (0, x, y))
# endif
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_DIV_BASE_P(d, rxy) HBOOST_PP_DIV_BASE_P_IM(d, HBOOST_PP_TUPLE_REM_3 rxy)
#    define HBOOST_PP_DIV_BASE_P_IM(d, im) HBOOST_PP_DIV_BASE_P_I(d, im)
# else
#    define HBOOST_PP_DIV_BASE_P(d, rxy) HBOOST_PP_DIV_BASE_P_I(d, HBOOST_PP_TUPLE_ELEM(3, 0, rxy), HBOOST_PP_TUPLE_ELEM(3, 1, rxy), HBOOST_PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define HBOOST_PP_DIV_BASE_P_I(d, r, x, y) HBOOST_PP_LESS_EQUAL_D(d, y, x)
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_DIV_BASE_O(d, rxy) HBOOST_PP_DIV_BASE_O_IM(d, HBOOST_PP_TUPLE_REM_3 rxy)
#    define HBOOST_PP_DIV_BASE_O_IM(d, im) HBOOST_PP_DIV_BASE_O_I(d, im)
# else
#    define HBOOST_PP_DIV_BASE_O(d, rxy) HBOOST_PP_DIV_BASE_O_I(d, HBOOST_PP_TUPLE_ELEM(3, 0, rxy), HBOOST_PP_TUPLE_ELEM(3, 1, rxy), HBOOST_PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define HBOOST_PP_DIV_BASE_O_I(d, r, x, y) (HBOOST_PP_INC(r), HBOOST_PP_SUB_D(d, x, y), y)
#
# /* HBOOST_PP_DIV_BASE_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_DIV_BASE_D(d, x, y) HBOOST_PP_WHILE_ ## d(HBOOST_PP_DIV_BASE_P, HBOOST_PP_DIV_BASE_O, (0, x, y))
# else
#    define HBOOST_PP_DIV_BASE_D(d, x, y) HBOOST_PP_DIV_BASE_D_I(d, x, y)
#    define HBOOST_PP_DIV_BASE_D_I(d, x, y) HBOOST_PP_WHILE_ ## d(HBOOST_PP_DIV_BASE_P, HBOOST_PP_DIV_BASE_O, (0, x, y))
# endif
#
# endif
