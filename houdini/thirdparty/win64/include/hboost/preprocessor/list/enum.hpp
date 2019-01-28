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
# ifndef HBOOST_PREPROCESSOR_LIST_ENUM_HPP
# define HBOOST_PREPROCESSOR_LIST_ENUM_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/for_each_i.hpp>
# include <hboost/preprocessor/punctuation/comma_if.hpp>
#
# /* HBOOST_PP_LIST_ENUM */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_ENUM(list) HBOOST_PP_LIST_FOR_EACH_I(HBOOST_PP_LIST_ENUM_O, HBOOST_PP_NIL, list)
# else
#    define HBOOST_PP_LIST_ENUM(list) HBOOST_PP_LIST_ENUM_I(list)
#    define HBOOST_PP_LIST_ENUM_I(list) HBOOST_PP_LIST_FOR_EACH_I(HBOOST_PP_LIST_ENUM_O, HBOOST_PP_NIL, list)
# endif
#
# define HBOOST_PP_LIST_ENUM_O(r, _, i, elem) HBOOST_PP_COMMA_IF(i) elem
#
# /* HBOOST_PP_LIST_ENUM_R */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_ENUM_R(r, list) HBOOST_PP_LIST_FOR_EACH_I_R(r, HBOOST_PP_LIST_ENUM_O, HBOOST_PP_NIL, list)
# else
#    define HBOOST_PP_LIST_ENUM_R(r, list) HBOOST_PP_LIST_ENUM_R_I(r, list)
#    define HBOOST_PP_LIST_ENUM_R_I(r, list) HBOOST_PP_LIST_FOR_EACH_I_R(r, HBOOST_PP_LIST_ENUM_O, HBOOST_PP_NIL, list)
# endif
#
# endif
