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
# ifndef HBOOST_PREPROCESSOR_LIST_AT_HPP
# define HBOOST_PREPROCESSOR_LIST_AT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/adt.hpp>
# include <hboost/preprocessor/list/rest_n.hpp>
#
# /* HBOOST_PP_LIST_AT */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_AT(list, index) HBOOST_PP_LIST_FIRST(HBOOST_PP_LIST_REST_N(index, list))
# else
#    define HBOOST_PP_LIST_AT(list, index) HBOOST_PP_LIST_AT_I(list, index)
#    define HBOOST_PP_LIST_AT_I(list, index) HBOOST_PP_LIST_FIRST(HBOOST_PP_LIST_REST_N(index, list))
# endif
#
# /* HBOOST_PP_LIST_AT_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_AT_D(d, list, index) HBOOST_PP_LIST_FIRST(HBOOST_PP_LIST_REST_N_D(d, index, list))
# else
#    define HBOOST_PP_LIST_AT_D(d, list, index) HBOOST_PP_LIST_AT_D_I(d, list, index)
#    define HBOOST_PP_LIST_AT_D_I(d, list, index) HBOOST_PP_LIST_FIRST(HBOOST_PP_LIST_REST_N_D(d, index, list))
# endif
#
# endif
