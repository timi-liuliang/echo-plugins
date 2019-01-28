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
# ifndef HBOOST_PREPROCESSOR_LIST_REST_N_HPP
# define HBOOST_PREPROCESSOR_LIST_REST_N_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/list/adt.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_LIST_REST_N */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_REST_N(count, list) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE(HBOOST_PP_LIST_REST_N_P, HBOOST_PP_LIST_REST_N_O, (list, count)))
# else
#    define HBOOST_PP_LIST_REST_N(count, list) HBOOST_PP_LIST_REST_N_I(count, list)
#    define HBOOST_PP_LIST_REST_N_I(count, list) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE(HBOOST_PP_LIST_REST_N_P, HBOOST_PP_LIST_REST_N_O, (list, count)))
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_REST_N_P(d, lc) HBOOST_PP_TUPLE_ELEM(2, 1, lc)
# else
#    define HBOOST_PP_LIST_REST_N_P(d, lc) HBOOST_PP_LIST_REST_N_P_I lc
#    define HBOOST_PP_LIST_REST_N_P_I(list, count) count
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_REST_N_O(d, lc) (HBOOST_PP_LIST_REST(HBOOST_PP_TUPLE_ELEM(2, 0, lc)), HBOOST_PP_DEC(HBOOST_PP_TUPLE_ELEM(2, 1, lc)))
# else
#    define HBOOST_PP_LIST_REST_N_O(d, lc) HBOOST_PP_LIST_REST_N_O_I lc
#    define HBOOST_PP_LIST_REST_N_O_I(list, count) (HBOOST_PP_LIST_REST(list), HBOOST_PP_DEC(count))
# endif
#
# /* HBOOST_PP_LIST_REST_N_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_REST_N_D(d, count, list) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE_ ## d(HBOOST_PP_LIST_REST_N_P, HBOOST_PP_LIST_REST_N_O, (list, count)))
# else
#    define HBOOST_PP_LIST_REST_N_D(d, count, list) HBOOST_PP_LIST_REST_N_D_I(d, count, list)
#    define HBOOST_PP_LIST_REST_N_D_I(d, count, list) HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PP_WHILE_ ## d(HBOOST_PP_LIST_REST_N_P, HBOOST_PP_LIST_REST_N_O, (list, count)))
# endif
#
# endif
