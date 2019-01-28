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
# ifndef HBOOST_PREPROCESSOR_LIST_TO_TUPLE_HPP
# define HBOOST_PREPROCESSOR_LIST_TO_TUPLE_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/enum.hpp>
# include <hboost/preprocessor/control/iif.hpp>
#
# /* HBOOST_PP_LIST_TO_TUPLE */
#
# define HBOOST_PP_LIST_TO_TUPLE(list) \
	HBOOST_PP_IIF \
		( \
		HBOOST_PP_LIST_IS_NIL(list), \
		HBOOST_PP_LIST_TO_TUPLE_EMPTY, \
		HBOOST_PP_LIST_TO_TUPLE_DO \
		) \
	(list) \
/**/
# define HBOOST_PP_LIST_TO_TUPLE_EMPTY(list)
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_TO_TUPLE_DO(list) (HBOOST_PP_LIST_ENUM(list))
# else
#    define HBOOST_PP_LIST_TO_TUPLE_DO(list) HBOOST_PP_LIST_TO_TUPLE_I(list)
#    define HBOOST_PP_LIST_TO_TUPLE_I(list) (HBOOST_PP_LIST_ENUM(list))
# endif
#
# /* HBOOST_PP_LIST_TO_TUPLE_R */
#
# define HBOOST_PP_LIST_TO_TUPLE_R(r, list) \
	HBOOST_PP_IIF \
		( \
		HBOOST_PP_LIST_IS_NIL(list), \
		HBOOST_PP_LIST_TO_TUPLE_R_EMPTY, \
		HBOOST_PP_LIST_TO_TUPLE_R_DO \
		) \
	(r, list) \
/**/
# define HBOOST_PP_LIST_TO_TUPLE_R_EMPTY(r,list)
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_TO_TUPLE_R_DO(r, list) (HBOOST_PP_LIST_ENUM_R(r, list))
# else
#    define HBOOST_PP_LIST_TO_TUPLE_R_DO(r, list) HBOOST_PP_LIST_TO_TUPLE_R_I(r, list)
#    define HBOOST_PP_LIST_TO_TUPLE_R_I(r, list) (HBOOST_PP_LIST_ENUM_R(r, list))
# endif
#
# endif
