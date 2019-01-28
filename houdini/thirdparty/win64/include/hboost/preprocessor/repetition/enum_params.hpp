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
# ifndef HBOOST_PREPROCESSOR_REPETITION_ENUM_PARAMS_HPP
# define HBOOST_PREPROCESSOR_REPETITION_ENUM_PARAMS_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/punctuation/comma_if.hpp>
# include <hboost/preprocessor/repetition/repeat.hpp>
#
# /* HBOOST_PP_ENUM_PARAMS */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ENUM_PARAMS(count, param) HBOOST_PP_REPEAT(count, HBOOST_PP_ENUM_PARAMS_M, param)
# else
#    define HBOOST_PP_ENUM_PARAMS(count, param) HBOOST_PP_ENUM_PARAMS_I(count, param)
#    define HBOOST_PP_ENUM_PARAMS_I(count, param) HBOOST_PP_REPEAT(count, HBOOST_PP_ENUM_PARAMS_M, param)
# endif
#
# define HBOOST_PP_ENUM_PARAMS_M(z, n, param) HBOOST_PP_COMMA_IF(n) param ## n
#
# /* HBOOST_PP_ENUM_PARAMS_Z */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ENUM_PARAMS_Z(z, count, param) HBOOST_PP_REPEAT_ ## z(count, HBOOST_PP_ENUM_PARAMS_M, param)
# else
#    define HBOOST_PP_ENUM_PARAMS_Z(z, count, param) HBOOST_PP_ENUM_PARAMS_Z_I(z, count, param)
#    define HBOOST_PP_ENUM_PARAMS_Z_I(z, count, param) HBOOST_PP_REPEAT_ ## z(count, HBOOST_PP_ENUM_PARAMS_M, param)
# endif
#
# endif
