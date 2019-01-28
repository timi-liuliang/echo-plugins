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
# ifndef HBOOST_PREPROCESSOR_LIST_APPEND_HPP
# define HBOOST_PREPROCESSOR_LIST_APPEND_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/fold_right.hpp>
#
# /* HBOOST_PP_LIST_APPEND */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_APPEND(a, b) HBOOST_PP_LIST_FOLD_RIGHT(HBOOST_PP_LIST_APPEND_O, b, a)
# else
#    define HBOOST_PP_LIST_APPEND(a, b) HBOOST_PP_LIST_APPEND_I(a, b)
#    define HBOOST_PP_LIST_APPEND_I(a, b) HBOOST_PP_LIST_FOLD_RIGHT(HBOOST_PP_LIST_APPEND_O, b, a)
# endif
#
# define HBOOST_PP_LIST_APPEND_O(d, s, x) (x, s)
#
# /* HBOOST_PP_LIST_APPEND_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_APPEND_D(d, a, b) HBOOST_PP_LIST_FOLD_RIGHT_ ## d(HBOOST_PP_LIST_APPEND_O, b, a)
# else
#    define HBOOST_PP_LIST_APPEND_D(d, a, b) HBOOST_PP_LIST_APPEND_D_I(d, a, b)
#    define HBOOST_PP_LIST_APPEND_D_I(d, a, b) HBOOST_PP_LIST_FOLD_RIGHT_ ## d(HBOOST_PP_LIST_APPEND_O, b, a)
# endif
#
# endif
