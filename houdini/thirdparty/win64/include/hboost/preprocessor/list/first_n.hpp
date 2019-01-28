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
# ifndef HBOOST_PREPROCESSOR_LIST_FIRST_N_HPP
# define HBOOST_PREPROCESSOR_LIST_FIRST_N_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/list/adt.hpp>
# include <hboost/preprocessor/list/reverse.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_LIST_FIRST_N */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FIRST_N(count, list) HBOOST_PP_LIST_REVERSE(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_WHILE(HBOOST_PP_LIST_FIRST_N_P, HBOOST_PP_LIST_FIRST_N_O, (count, list, HBOOST_PP_NIL))))
# else
#    define HBOOST_PP_LIST_FIRST_N(count, list) HBOOST_PP_LIST_FIRST_N_I(count, list)
#    define HBOOST_PP_LIST_FIRST_N_I(count, list) HBOOST_PP_LIST_REVERSE(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_WHILE(HBOOST_PP_LIST_FIRST_N_P, HBOOST_PP_LIST_FIRST_N_O, (count, list, HBOOST_PP_NIL))))
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FIRST_N_P(d, data) HBOOST_PP_TUPLE_ELEM(3, 0, data)
# else
#    define HBOOST_PP_LIST_FIRST_N_P(d, data) HBOOST_PP_LIST_FIRST_N_P_I data
#    define HBOOST_PP_LIST_FIRST_N_P_I(c, l, nl) c
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_LIST_FIRST_N_O(d, data) HBOOST_PP_LIST_FIRST_N_O_D data
# else
#    define HBOOST_PP_LIST_FIRST_N_O(d, data) HBOOST_PP_LIST_FIRST_N_O_D(HBOOST_PP_TUPLE_ELEM(3, 0, data), HBOOST_PP_TUPLE_ELEM(3, 1, data), HBOOST_PP_TUPLE_ELEM(3, 2, data))
# endif
#
# define HBOOST_PP_LIST_FIRST_N_O_D(c, l, nl) (HBOOST_PP_DEC(c), HBOOST_PP_LIST_REST(l), (HBOOST_PP_LIST_FIRST(l), nl))
#
# /* HBOOST_PP_LIST_FIRST_N_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FIRST_N_D(d, count, list) HBOOST_PP_LIST_REVERSE_D(d, HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_WHILE_ ## d(HBOOST_PP_LIST_FIRST_N_P, HBOOST_PP_LIST_FIRST_N_O, (count, list, HBOOST_PP_NIL))))
# else
#    define HBOOST_PP_LIST_FIRST_N_D(d, count, list) HBOOST_PP_LIST_FIRST_N_D_I(d, count, list)
#    define HBOOST_PP_LIST_FIRST_N_D_I(d, count, list) HBOOST_PP_LIST_REVERSE_D(d, HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_WHILE_ ## d(HBOOST_PP_LIST_FIRST_N_P, HBOOST_PP_LIST_FIRST_N_O, (count, list, HBOOST_PP_NIL))))
# endif
#
# endif
