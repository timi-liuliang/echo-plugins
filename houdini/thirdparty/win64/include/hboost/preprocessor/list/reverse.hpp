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
# ifndef HBOOST_PREPROCESSOR_LIST_REVERSE_HPP
# define HBOOST_PREPROCESSOR_LIST_REVERSE_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/fold_left.hpp>
#
# /* HBOOST_PP_LIST_REVERSE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_REVERSE(list) HBOOST_PP_LIST_FOLD_LEFT(HBOOST_PP_LIST_REVERSE_O, HBOOST_PP_NIL, list)
# else
#    define HBOOST_PP_LIST_REVERSE(list) HBOOST_PP_LIST_REVERSE_I(list)
#    define HBOOST_PP_LIST_REVERSE_I(list) HBOOST_PP_LIST_FOLD_LEFT(HBOOST_PP_LIST_REVERSE_O, HBOOST_PP_NIL, list)
# endif
#
# define HBOOST_PP_LIST_REVERSE_O(d, s, x) (x, s)
#
# /* HBOOST_PP_LIST_REVERSE_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_REVERSE_D(d, list) HBOOST_PP_LIST_FOLD_LEFT_ ## d(HBOOST_PP_LIST_REVERSE_O, HBOOST_PP_NIL, list)
# else
#    define HBOOST_PP_LIST_REVERSE_D(d, list) HBOOST_PP_LIST_REVERSE_D_I(d, list)
#    define HBOOST_PP_LIST_REVERSE_D_I(d, list) HBOOST_PP_LIST_FOLD_LEFT_ ## d(HBOOST_PP_LIST_REVERSE_O, HBOOST_PP_NIL, list)
# endif
#
# endif
