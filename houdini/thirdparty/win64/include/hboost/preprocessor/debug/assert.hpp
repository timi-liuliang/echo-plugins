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
# ifndef HBOOST_PREPROCESSOR_DEBUG_ASSERT_HPP
# define HBOOST_PREPROCESSOR_DEBUG_ASSERT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/expr_iif.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/logical/not.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
#
# /* HBOOST_PP_ASSERT */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ASSERT HBOOST_PP_ASSERT_D
# else
#    define HBOOST_PP_ASSERT(cond) HBOOST_PP_ASSERT_D(cond)
# endif
#
# define HBOOST_PP_ASSERT_D(cond) HBOOST_PP_IIF(HBOOST_PP_NOT(cond), HBOOST_PP_ASSERT_ERROR, HBOOST_PP_TUPLE_EAT_1)(...)
# define HBOOST_PP_ASSERT_ERROR(x, y, z)
#
# /* HBOOST_PP_ASSERT_MSG */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ASSERT_MSG HBOOST_PP_ASSERT_MSG_D
# else
#    define HBOOST_PP_ASSERT_MSG(cond, msg) HBOOST_PP_ASSERT_MSG_D(cond, msg)
# endif
#
# define HBOOST_PP_ASSERT_MSG_D(cond, msg) HBOOST_PP_EXPR_IIF(HBOOST_PP_NOT(cond), msg)
#
# endif
