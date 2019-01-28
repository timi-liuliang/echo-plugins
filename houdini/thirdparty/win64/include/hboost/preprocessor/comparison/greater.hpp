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
# ifndef HBOOST_PREPROCESSOR_COMPARISON_GREATER_HPP
# define HBOOST_PREPROCESSOR_COMPARISON_GREATER_HPP
#
# include <hboost/preprocessor/comparison/less.hpp>
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_GREATER */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_GREATER(x, y) HBOOST_PP_LESS(y, x)
# else
#    define HBOOST_PP_GREATER(x, y) HBOOST_PP_GREATER_I(x, y)
#    define HBOOST_PP_GREATER_I(x, y) HBOOST_PP_LESS(y, x)
# endif
#
# /* HBOOST_PP_GREATER_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_GREATER_D(d, x, y) HBOOST_PP_LESS_D(d, y, x)
# else
#    define HBOOST_PP_GREATER_D(d, x, y) HBOOST_PP_GREATER_D_I(d, x, y)
#    define HBOOST_PP_GREATER_D_I(d, x, y) HBOOST_PP_LESS_D(d, y, x)
# endif
#
# endif
