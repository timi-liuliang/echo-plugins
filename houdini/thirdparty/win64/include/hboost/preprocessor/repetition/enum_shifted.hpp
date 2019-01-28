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
# ifndef HBOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_HPP
# define HBOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/debug/error.hpp>
# include <hboost/preprocessor/detail/auto_rec.hpp>
# include <hboost/preprocessor/punctuation/comma_if.hpp>
# include <hboost/preprocessor/repetition/repeat.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_ENUM_SHIFTED */
#
# if 0
#    define HBOOST_PP_ENUM_SHIFTED(count, macro, data)
# endif
#
# define HBOOST_PP_ENUM_SHIFTED HBOOST_PP_CAT(HBOOST_PP_ENUM_SHIFTED_, HBOOST_PP_AUTO_REC(HBOOST_PP_REPEAT_P, 4))
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ENUM_SHIFTED_1(c, m, d) HBOOST_PP_REPEAT_1(HBOOST_PP_DEC(c), HBOOST_PP_ENUM_SHIFTED_M_1, (m, d))
#    define HBOOST_PP_ENUM_SHIFTED_2(c, m, d) HBOOST_PP_REPEAT_2(HBOOST_PP_DEC(c), HBOOST_PP_ENUM_SHIFTED_M_2, (m, d))
#    define HBOOST_PP_ENUM_SHIFTED_3(c, m, d) HBOOST_PP_REPEAT_3(HBOOST_PP_DEC(c), HBOOST_PP_ENUM_SHIFTED_M_3, (m, d))
# else
#    define HBOOST_PP_ENUM_SHIFTED_1(c, m, d) HBOOST_PP_ENUM_SHIFTED_1_I(c, m, d)
#    define HBOOST_PP_ENUM_SHIFTED_2(c, m, d) HBOOST_PP_ENUM_SHIFTED_1_2(c, m, d)
#    define HBOOST_PP_ENUM_SHIFTED_3(c, m, d) HBOOST_PP_ENUM_SHIFTED_1_3(c, m, d)
#    define HBOOST_PP_ENUM_SHIFTED_1_I(c, m, d) HBOOST_PP_REPEAT_1(HBOOST_PP_DEC(c), HBOOST_PP_ENUM_SHIFTED_M_1, (m, d))
#    define HBOOST_PP_ENUM_SHIFTED_2_I(c, m, d) HBOOST_PP_REPEAT_2(HBOOST_PP_DEC(c), HBOOST_PP_ENUM_SHIFTED_M_2, (m, d))
#    define HBOOST_PP_ENUM_SHIFTED_3_I(c, m, d) HBOOST_PP_REPEAT_3(HBOOST_PP_DEC(c), HBOOST_PP_ENUM_SHIFTED_M_3, (m, d))
# endif
#
# define HBOOST_PP_ENUM_SHIFTED_4(c, m, d) HBOOST_PP_ERROR(0x0003)
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_ENUM_SHIFTED_M_1(z, n, md) HBOOST_PP_ENUM_SHIFTED_M_1_IM(z, n, HBOOST_PP_TUPLE_REM_2 md)
#    define HBOOST_PP_ENUM_SHIFTED_M_2(z, n, md) HBOOST_PP_ENUM_SHIFTED_M_2_IM(z, n, HBOOST_PP_TUPLE_REM_2 md)
#    define HBOOST_PP_ENUM_SHIFTED_M_3(z, n, md) HBOOST_PP_ENUM_SHIFTED_M_3_IM(z, n, HBOOST_PP_TUPLE_REM_2 md)
#    define HBOOST_PP_ENUM_SHIFTED_M_1_IM(z, n, im) HBOOST_PP_ENUM_SHIFTED_M_1_I(z, n, im)
#    define HBOOST_PP_ENUM_SHIFTED_M_2_IM(z, n, im) HBOOST_PP_ENUM_SHIFTED_M_2_I(z, n, im)
#    define HBOOST_PP_ENUM_SHIFTED_M_3_IM(z, n, im) HBOOST_PP_ENUM_SHIFTED_M_3_I(z, n, im)
# else
#    define HBOOST_PP_ENUM_SHIFTED_M_1(z, n, md) HBOOST_PP_ENUM_SHIFTED_M_1_I(z, n, HBOOST_PP_TUPLE_ELEM(2, 0, md), HBOOST_PP_TUPLE_ELEM(2, 1, md))
#    define HBOOST_PP_ENUM_SHIFTED_M_2(z, n, md) HBOOST_PP_ENUM_SHIFTED_M_2_I(z, n, HBOOST_PP_TUPLE_ELEM(2, 0, md), HBOOST_PP_TUPLE_ELEM(2, 1, md))
#    define HBOOST_PP_ENUM_SHIFTED_M_3(z, n, md) HBOOST_PP_ENUM_SHIFTED_M_3_I(z, n, HBOOST_PP_TUPLE_ELEM(2, 0, md), HBOOST_PP_TUPLE_ELEM(2, 1, md))
# endif
#
# define HBOOST_PP_ENUM_SHIFTED_M_1_I(z, n, m, d) HBOOST_PP_COMMA_IF(n) m(z, HBOOST_PP_INC(n), d)
# define HBOOST_PP_ENUM_SHIFTED_M_2_I(z, n, m, d) HBOOST_PP_COMMA_IF(n) m(z, HBOOST_PP_INC(n), d)
# define HBOOST_PP_ENUM_SHIFTED_M_3_I(z, n, m, d) HBOOST_PP_COMMA_IF(n) m(z, HBOOST_PP_INC(n), d)
#
# endif
