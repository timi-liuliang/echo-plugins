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
# ifndef HBOOST_PREPROCESSOR_LIST_SIZE_HPP
# define HBOOST_PREPROCESSOR_LIST_SIZE_HPP
#
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/list/adt.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_LIST_SIZE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_SIZE(list) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE(HBOOST_PP_LIST_SIZE_P, HBOOST_PP_LIST_SIZE_O, (0, list)))
# else
#    define HBOOST_PP_LIST_SIZE(list) HBOOST_PP_LIST_SIZE_I(list)
#    define HBOOST_PP_LIST_SIZE_I(list) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE(HBOOST_PP_LIST_SIZE_P, HBOOST_PP_LIST_SIZE_O, (0, list)))
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_SIZE_P(d, rl) HBOOST_PP_LIST_IS_CONS(HBOOST_PP_TUPLE_ELEM(2, 1, rl))
# else
#    define HBOOST_PP_LIST_SIZE_P(d, rl) HBOOST_PP_LIST_SIZE_P_I(HBOOST_PP_TUPLE_REM_2 rl)
#    define HBOOST_PP_LIST_SIZE_P_I(im) HBOOST_PP_LIST_SIZE_P_II(im)
#    define HBOOST_PP_LIST_SIZE_P_II(r, l) HBOOST_PP_LIST_IS_CONS(l)
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_SIZE_O(d, rl) (HBOOST_PP_INC(HBOOST_PP_TUPLE_ELEM(2, 0, rl)), HBOOST_PP_LIST_REST(HBOOST_PP_TUPLE_ELEM(2, 1, rl)))
# else
#    define HBOOST_PP_LIST_SIZE_O(d, rl) HBOOST_PP_LIST_SIZE_O_I(HBOOST_PP_TUPLE_REM_2 rl)
#    define HBOOST_PP_LIST_SIZE_O_I(im) HBOOST_PP_LIST_SIZE_O_II(im)
#    define HBOOST_PP_LIST_SIZE_O_II(r, l) (HBOOST_PP_INC(r), HBOOST_PP_LIST_REST(l))
# endif
#
# /* HBOOST_PP_LIST_SIZE_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_SIZE_D(d, list) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE_ ## d(HBOOST_PP_LIST_SIZE_P, HBOOST_PP_LIST_SIZE_O, (0, list)))
# else
#    define HBOOST_PP_LIST_SIZE_D(d, list) HBOOST_PP_LIST_SIZE_D_I(d, list)
#    define HBOOST_PP_LIST_SIZE_D_I(d, list) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE_ ## d(HBOOST_PP_LIST_SIZE_P, HBOOST_PP_LIST_SIZE_O, (0, list)))
# endif
#
# endif
