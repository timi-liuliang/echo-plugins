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
# ifndef HBOOST_PREPROCESSOR_SELECTION_MIN_HPP
# define HBOOST_PREPROCESSOR_SELECTION_MIN_HPP
#
# include <hboost/preprocessor/comparison/less_equal.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/iif.hpp>
#
# /* HBOOST_PP_MIN */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_MIN(x, y) HBOOST_PP_IIF(HBOOST_PP_LESS_EQUAL(y, x), y, x)
# else
#    define HBOOST_PP_MIN(x, y) HBOOST_PP_MIN_I(x, y)
#    define HBOOST_PP_MIN_I(x, y) HBOOST_PP_IIF(HBOOST_PP_LESS_EQUAL(y, x), y, x)
# endif
#
# /* HBOOST_PP_MIN_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_MIN_D(d, x, y) HBOOST_PP_IIF(HBOOST_PP_LESS_EQUAL_D(d, y, x), y, x)
# else
#    define HBOOST_PP_MIN_D(d, x, y) HBOOST_PP_MIN_D_I(d, x, y)
#    define HBOOST_PP_MIN_D_I(d, x, y) HBOOST_PP_IIF(HBOOST_PP_LESS_EQUAL_D(d, y, x), y, x)
# endif
#
# endif
