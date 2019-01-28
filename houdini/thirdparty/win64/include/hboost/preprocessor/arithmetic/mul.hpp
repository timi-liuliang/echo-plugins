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
# ifndef HBOOST_PREPROCESSOR_ARITHMETIC_MUL_HPP
# define HBOOST_PREPROCESSOR_ARITHMETIC_MUL_HPP
#
# include <hboost/preprocessor/arithmetic/add.hpp>
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_MUL */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_MUL(x, y) HBOOST_PP_TUPLE_ELEM(3, 0, HBOOST_PP_WHILE(HBOOST_PP_MUL_P, HBOOST_PP_MUL_O, (0, x, y)))
# else
#    define HBOOST_PP_MUL(x, y) HBOOST_PP_MUL_I(x, y)
#    define HBOOST_PP_MUL_I(x, y) HBOOST_PP_TUPLE_ELEM(3, 0, HBOOST_PP_WHILE(HBOOST_PP_MUL_P, HBOOST_PP_MUL_O, (0, x, y)))
# endif
#
# define HBOOST_PP_MUL_P(d, rxy) HBOOST_PP_TUPLE_ELEM(3, 2, rxy)
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_MUL_O(d, rxy) HBOOST_PP_MUL_O_IM(d, HBOOST_PP_TUPLE_REM_3 rxy)
#    define HBOOST_PP_MUL_O_IM(d, im) HBOOST_PP_MUL_O_I(d, im)
# else
#    define HBOOST_PP_MUL_O(d, rxy) HBOOST_PP_MUL_O_I(d, HBOOST_PP_TUPLE_ELEM(3, 0, rxy), HBOOST_PP_TUPLE_ELEM(3, 1, rxy), HBOOST_PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define HBOOST_PP_MUL_O_I(d, r, x, y) (HBOOST_PP_ADD_D(d, r, x), x, HBOOST_PP_DEC(y))
#
# /* HBOOST_PP_MUL_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_MUL_D(d, x, y) HBOOST_PP_TUPLE_ELEM(3, 0, HBOOST_PP_WHILE_ ## d(HBOOST_PP_MUL_P, HBOOST_PP_MUL_O, (0, x, y)))
# else
#    define HBOOST_PP_MUL_D(d, x, y) HBOOST_PP_MUL_D_I(d, x, y)
#    define HBOOST_PP_MUL_D_I(d, x, y) HBOOST_PP_TUPLE_ELEM(3, 0, HBOOST_PP_WHILE_ ## d(HBOOST_PP_MUL_P, HBOOST_PP_MUL_O, (0, x, y)))
# endif
#
# endif
