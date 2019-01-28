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
# ifndef HBOOST_PREPROCESSOR_SELECTION_MAX_HPP
# define HBOOST_PREPROCESSOR_SELECTION_MAX_HPP
#
# include <hboost/preprocessor/comparison/less_equal.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/iif.hpp>
#
# /* HBOOST_PP_MAX */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_MAX(x, y) HBOOST_PP_IIF(HBOOST_PP_LESS_EQUAL(x, y), y, x)
# else
#    define HBOOST_PP_MAX(x, y) HBOOST_PP_MAX_I(x, y)
#    define HBOOST_PP_MAX_I(x, y) HBOOST_PP_IIF(HBOOST_PP_LESS_EQUAL(x, y), y, x)
# endif
#
# /* HBOOST_PP_MAX_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_MAX_D(d, x, y) HBOOST_PP_IIF(HBOOST_PP_LESS_EQUAL_D(d, x, y), y, x)
# else
#    define HBOOST_PP_MAX_D(d, x, y) HBOOST_PP_MAX_D_I(d, x, y)
#    define HBOOST_PP_MAX_D_I(d, x, y) HBOOST_PP_IIF(HBOOST_PP_LESS_EQUAL_D(d, x, y), y, x)
# endif
#
# endif
