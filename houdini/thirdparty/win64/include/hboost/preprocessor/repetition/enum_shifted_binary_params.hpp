# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2005.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_BINARY_PARAMS_HPP
# define HBOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_BINARY_PARAMS_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/punctuation/comma_if.hpp>
# include <hboost/preprocessor/repetition/repeat.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS(count, p1, p2) HBOOST_PP_REPEAT(HBOOST_PP_DEC(count), HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M, (p1, p2))
# else
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS(count, p1, p2) HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_I(count, p1, p2)
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_I(count, p1, p2) HBOOST_PP_REPEAT(HBOOST_PP_DEC(count), HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M, (p1, p2))
# endif
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M(z, n, pp) HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_IM(z, n, HBOOST_PP_TUPLE_REM_2 pp)
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_IM(z, n, im) HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_I(z, n, im)
# else
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M(z, n, pp) HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_I(z, n, HBOOST_PP_TUPLE_ELEM(2, 0, pp), HBOOST_PP_TUPLE_ELEM(2, 1, pp))
# endif
#
# define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_I(z, n, p1, p2) HBOOST_PP_COMMA_IF(n) HBOOST_PP_CAT(p1, HBOOST_PP_INC(n)) HBOOST_PP_CAT(p2, HBOOST_PP_INC(n))
#
# /* HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z(z, count, p1, p2) HBOOST_PP_REPEAT_ ## z(HBOOST_PP_DEC(count), HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M, (p1, p2))
# else
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z(z, count, p1, p2) HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z_I(z, count, p1, p2)
#    define HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z_I(z, count, p1, p2) HBOOST_PP_REPEAT_ ## z(HBOOST_PP_DEC(count), HBOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M, (p1, p2))
# endif
#
# endif
