# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_REPETITION_ENUM_TRAILING_HPP
# define HBOOST_PREPROCESSOR_REPETITION_ENUM_TRAILING_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/debug/error.hpp>
# include <hboost/preprocessor/detail/auto_rec.hpp>
# include <hboost/preprocessor/repetition/repeat.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_ENUM_TRAILING */
#
# if 0
#    define HBOOST_PP_ENUM_TRAILING(count, macro, data)
# endif
#
# define HBOOST_PP_ENUM_TRAILING HBOOST_PP_CAT(HBOOST_PP_ENUM_TRAILING_, HBOOST_PP_AUTO_REC(HBOOST_PP_REPEAT_P, 4))
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ENUM_TRAILING_1(c, m, d) HBOOST_PP_REPEAT_1(c, HBOOST_PP_ENUM_TRAILING_M_1, (m, d))
#    define HBOOST_PP_ENUM_TRAILING_2(c, m, d) HBOOST_PP_REPEAT_2(c, HBOOST_PP_ENUM_TRAILING_M_2, (m, d))
#    define HBOOST_PP_ENUM_TRAILING_3(c, m, d) HBOOST_PP_REPEAT_3(c, HBOOST_PP_ENUM_TRAILING_M_3, (m, d))
# else
#    define HBOOST_PP_ENUM_TRAILING_1(c, m, d) HBOOST_PP_ENUM_TRAILING_1_I(c, m, d)
#    define HBOOST_PP_ENUM_TRAILING_2(c, m, d) HBOOST_PP_ENUM_TRAILING_2_I(c, m, d)
#    define HBOOST_PP_ENUM_TRAILING_3(c, m, d) HBOOST_PP_ENUM_TRAILING_3_I(c, m, d)
#    define HBOOST_PP_ENUM_TRAILING_1_I(c, m, d) HBOOST_PP_REPEAT_1(c, HBOOST_PP_ENUM_TRAILING_M_1, (m, d))
#    define HBOOST_PP_ENUM_TRAILING_2_I(c, m, d) HBOOST_PP_REPEAT_2(c, HBOOST_PP_ENUM_TRAILING_M_2, (m, d))
#    define HBOOST_PP_ENUM_TRAILING_3_I(c, m, d) HBOOST_PP_REPEAT_3(c, HBOOST_PP_ENUM_TRAILING_M_3, (m, d))
# endif
#
# define HBOOST_PP_ENUM_TRAILING_4(c, m, d) HBOOST_PP_ERROR(0x0003)
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_ENUM_TRAILING_M_1(z, n, md) HBOOST_PP_ENUM_TRAILING_M_1_IM(z, n, HBOOST_PP_TUPLE_REM_2 md)
#    define HBOOST_PP_ENUM_TRAILING_M_2(z, n, md) HBOOST_PP_ENUM_TRAILING_M_2_IM(z, n, HBOOST_PP_TUPLE_REM_2 md)
#    define HBOOST_PP_ENUM_TRAILING_M_3(z, n, md) HBOOST_PP_ENUM_TRAILING_M_3_IM(z, n, HBOOST_PP_TUPLE_REM_2 md)
#    define HBOOST_PP_ENUM_TRAILING_M_1_IM(z, n, im) HBOOST_PP_ENUM_TRAILING_M_1_I(z, n, im)
#    define HBOOST_PP_ENUM_TRAILING_M_2_IM(z, n, im) HBOOST_PP_ENUM_TRAILING_M_2_I(z, n, im)
#    define HBOOST_PP_ENUM_TRAILING_M_3_IM(z, n, im) HBOOST_PP_ENUM_TRAILING_M_3_I(z, n, im)
# else
#    define HBOOST_PP_ENUM_TRAILING_M_1(z, n, md) HBOOST_PP_ENUM_TRAILING_M_1_I(z, n, HBOOST_PP_TUPLE_ELEM(2, 0, md), HBOOST_PP_TUPLE_ELEM(2, 1, md))
#    define HBOOST_PP_ENUM_TRAILING_M_2(z, n, md) HBOOST_PP_ENUM_TRAILING_M_2_I(z, n, HBOOST_PP_TUPLE_ELEM(2, 0, md), HBOOST_PP_TUPLE_ELEM(2, 1, md))
#    define HBOOST_PP_ENUM_TRAILING_M_3(z, n, md) HBOOST_PP_ENUM_TRAILING_M_3_I(z, n, HBOOST_PP_TUPLE_ELEM(2, 0, md), HBOOST_PP_TUPLE_ELEM(2, 1, md))
# endif
#
# define HBOOST_PP_ENUM_TRAILING_M_1_I(z, n, m, d) , m(z, n, d)
# define HBOOST_PP_ENUM_TRAILING_M_2_I(z, n, m, d) , m(z, n, d)
# define HBOOST_PP_ENUM_TRAILING_M_3_I(z, n, m, d) , m(z, n, d)
#
# endif
