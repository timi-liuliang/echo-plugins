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
# ifndef HBOOST_PREPROCESSOR_ARITHMETIC_ADD_HPP
# define HBOOST_PREPROCESSOR_ARITHMETIC_ADD_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_ADD */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ADD(x, y) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE(HBOOST_PP_ADD_P, HBOOST_PP_ADD_O, (x, y)))
# else
#    define HBOOST_PP_ADD(x, y) HBOOST_PP_ADD_I(x, y)
#    define HBOOST_PP_ADD_I(x, y) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE(HBOOST_PP_ADD_P, HBOOST_PP_ADD_O, (x, y)))
# endif
#
# define HBOOST_PP_ADD_P(d, xy) HBOOST_PP_TUPLE_ELEM(2, 1, xy)
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_ADD_O(d, xy) HBOOST_PP_ADD_O_I xy
# else
#    define HBOOST_PP_ADD_O(d, xy) HBOOST_PP_ADD_O_I(HBOOST_PP_TUPLE_ELEM(2, 0, xy), HBOOST_PP_TUPLE_ELEM(2, 1, xy))
# endif
#
# define HBOOST_PP_ADD_O_I(x, y) (HBOOST_PP_INC(x), HBOOST_PP_DEC(y))
#
# /* HBOOST_PP_ADD_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ADD_D(d, x, y) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE_ ## d(HBOOST_PP_ADD_P, HBOOST_PP_ADD_O, (x, y)))
# else
#    define HBOOST_PP_ADD_D(d, x, y) HBOOST_PP_ADD_D_I(d, x, y)
#    define HBOOST_PP_ADD_D_I(d, x, y) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE_ ## d(HBOOST_PP_ADD_P, HBOOST_PP_ADD_O, (x, y)))
# endif
#
# endif
