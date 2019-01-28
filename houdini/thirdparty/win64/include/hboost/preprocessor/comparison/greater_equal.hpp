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
# ifndef HBOOST_PREPROCESSOR_COMPARISON_GREATER_EQUAL_HPP
# define HBOOST_PREPROCESSOR_COMPARISON_GREATER_EQUAL_HPP
#
# include <hboost/preprocessor/comparison/less_equal.hpp>
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_GREATER_EQUAL */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_GREATER_EQUAL(x, y) HBOOST_PP_LESS_EQUAL(y, x)
# else
#    define HBOOST_PP_GREATER_EQUAL(x, y) HBOOST_PP_GREATER_EQUAL_I(x, y)
#    define HBOOST_PP_GREATER_EQUAL_I(x, y) HBOOST_PP_LESS_EQUAL(y, x)
# endif
#
# /* HBOOST_PP_GREATER_EQUAL_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_GREATER_EQUAL_D(d, x, y) HBOOST_PP_LESS_EQUAL_D(d, y, x)
# else
#    define HBOOST_PP_GREATER_EQUAL_D(d, x, y) HBOOST_PP_GREATER_EQUAL_D_I(d, x, y)
#    define HBOOST_PP_GREATER_EQUAL_D_I(d, x, y) HBOOST_PP_LESS_EQUAL_D(d, y, x)
# endif
#
# endif
