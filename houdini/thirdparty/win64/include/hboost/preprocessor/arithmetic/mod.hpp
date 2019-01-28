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
# ifndef HBOOST_PREPROCESSOR_ARITHMETIC_MOD_HPP
# define HBOOST_PREPROCESSOR_ARITHMETIC_MOD_HPP
#
# include <hboost/preprocessor/arithmetic/detail/div_base.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_MOD */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_MOD(x, y) HBOOST_PP_TUPLE_ELEM(3, 1, HBOOST_PP_DIV_BASE(x, y))
# else
#    define HBOOST_PP_MOD(x, y) HBOOST_PP_MOD_I(x, y)
#    define HBOOST_PP_MOD_I(x, y) HBOOST_PP_TUPLE_ELEM(3, 1, HBOOST_PP_DIV_BASE(x, y))
# endif
#
# /* HBOOST_PP_MOD_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_MOD_D(d, x, y) HBOOST_PP_TUPLE_ELEM(3, 1, HBOOST_PP_DIV_BASE_D(d, x, y))
# else
#    define HBOOST_PP_MOD_D(d, x, y) HBOOST_PP_MOD_D_I(d, x, y)
#    define HBOOST_PP_MOD_D_I(d, x, y) HBOOST_PP_TUPLE_ELEM(3, 1, HBOOST_PP_DIV_BASE_D(d, x, y))
# endif
#
# endif
