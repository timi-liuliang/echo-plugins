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
# ifndef HBOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_PARAMS_HPP
# define HBOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_PARAMS_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/punctuation/comma_if.hpp>
# include <hboost/preprocessor/repetition/repeat.hpp>
#
# /* HBOOST_PP_ENUM_SHIFTED_PARAMS */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ENUM_SHIFTED_PARAMS(count, param) HBOOST_PP_REPEAT(HBOOST_PP_DEC(count), HBOOST_PP_ENUM_SHIFTED_PARAMS_M, param)
# else
#    define HBOOST_PP_ENUM_SHIFTED_PARAMS(count, param) HBOOST_PP_ENUM_SHIFTED_PARAMS_I(count, param)
#    define HBOOST_PP_ENUM_SHIFTED_PARAMS_I(count, param) HBOOST_PP_REPEAT(HBOOST_PP_DEC(count), HBOOST_PP_ENUM_SHIFTED_PARAMS_M, param)
# endif
#
# define HBOOST_PP_ENUM_SHIFTED_PARAMS_M(z, n, param) HBOOST_PP_COMMA_IF(n) HBOOST_PP_CAT(param, HBOOST_PP_INC(n))
#
# /* HBOOST_PP_ENUM_SHIFTED_PARAMS_Z */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ENUM_SHIFTED_PARAMS_Z(z, count, param) HBOOST_PP_REPEAT_ ## z(HBOOST_PP_DEC(count), HBOOST_PP_ENUM_SHIFTED_PARAMS_M, param)
# else
#    define HBOOST_PP_ENUM_SHIFTED_PARAMS_Z(z, count, param) HBOOST_PP_ENUM_SHIFTED_PARAMS_Z_I(z, count, param)
#    define HBOOST_PP_ENUM_SHIFTED_PARAMS_Z_I(z, count, param) HBOOST_PP_REPEAT_ ## z(HBOOST_PP_DEC(count), HBOOST_PP_ENUM_SHIFTED_PARAMS_M, param)
# endif
#
# endif
