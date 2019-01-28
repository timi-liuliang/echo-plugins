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
# ifndef HBOOST_PREPROCESSOR_LIST_CAT_HPP
# define HBOOST_PREPROCESSOR_LIST_CAT_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/adt.hpp>
# include <hboost/preprocessor/list/fold_left.hpp>
#
# /* HBOOST_PP_LIST_CAT */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_CAT(list) HBOOST_PP_LIST_FOLD_LEFT(HBOOST_PP_LIST_CAT_O, HBOOST_PP_LIST_FIRST(list), HBOOST_PP_LIST_REST(list))
# else
#    define HBOOST_PP_LIST_CAT(list) HBOOST_PP_LIST_CAT_I(list)
#    define HBOOST_PP_LIST_CAT_I(list) HBOOST_PP_LIST_FOLD_LEFT(HBOOST_PP_LIST_CAT_O, HBOOST_PP_LIST_FIRST(list), HBOOST_PP_LIST_REST(list))
# endif
#
# define HBOOST_PP_LIST_CAT_O(d, s, x) HBOOST_PP_CAT(s, x)
#
# /* HBOOST_PP_LIST_CAT_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_CAT_D(d, list) HBOOST_PP_LIST_FOLD_LEFT_ ## d(HBOOST_PP_LIST_CAT_O, HBOOST_PP_LIST_FIRST(list), HBOOST_PP_LIST_REST(list))
# else
#    define HBOOST_PP_LIST_CAT_D(d, list) HBOOST_PP_LIST_CAT_D_I(d, list)
#    define HBOOST_PP_LIST_CAT_D_I(d, list) HBOOST_PP_LIST_FOLD_LEFT_ ## d(HBOOST_PP_LIST_CAT_O, HBOOST_PP_LIST_FIRST(list), HBOOST_PP_LIST_REST(list))
# endif
#
# endif
