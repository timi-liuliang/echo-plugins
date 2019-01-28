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
# ifndef HBOOST_PREPROCESSOR_LIST_FOLD_RIGHT_HPP
# define HBOOST_PREPROCESSOR_LIST_FOLD_RIGHT_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/debug/error.hpp>
# include <hboost/preprocessor/detail/auto_rec.hpp>
#
# if 0
#    define HBOOST_PP_LIST_FOLD_RIGHT(op, state, list)
# endif
#
# define HBOOST_PP_LIST_FOLD_RIGHT HBOOST_PP_CAT(HBOOST_PP_LIST_FOLD_RIGHT_, HBOOST_PP_AUTO_REC(HBOOST_PP_WHILE_P, 256))
#
# define HBOOST_PP_LIST_FOLD_RIGHT_257(o, s, l) HBOOST_PP_ERROR(0x0004)
#
# define HBOOST_PP_LIST_FOLD_RIGHT_D(d, o, s, l) HBOOST_PP_LIST_FOLD_RIGHT_ ## d(o, s, l)
# define HBOOST_PP_LIST_FOLD_RIGHT_2ND HBOOST_PP_LIST_FOLD_RIGHT
# define HBOOST_PP_LIST_FOLD_RIGHT_2ND_D HBOOST_PP_LIST_FOLD_RIGHT_D
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    include <hboost/preprocessor/list/detail/edg/fold_right.hpp>
# else
#    include <hboost/preprocessor/list/detail/fold_right.hpp>
# endif
#
# endif
