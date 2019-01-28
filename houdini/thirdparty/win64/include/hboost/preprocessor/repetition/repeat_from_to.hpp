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
# ifndef HBOOST_PREPROCESSOR_REPETITION_REPEAT_FROM_TO_HPP
# define HBOOST_PREPROCESSOR_REPETITION_REPEAT_FROM_TO_HPP
#
# include <hboost/preprocessor/arithmetic/add.hpp>
# include <hboost/preprocessor/arithmetic/sub.hpp>
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/debug/error.hpp>
# include <hboost/preprocessor/detail/auto_rec.hpp>
# include <hboost/preprocessor/repetition/repeat.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_REPEAT_FROM_TO */
#
# if 0
#    define HBOOST_PP_REPEAT_FROM_TO(first, last, macro, data)
# endif
#
# define HBOOST_PP_REPEAT_FROM_TO HBOOST_PP_CAT(HBOOST_PP_REPEAT_FROM_TO_, HBOOST_PP_AUTO_REC(HBOOST_PP_REPEAT_P, 4))
#
# define HBOOST_PP_REPEAT_FROM_TO_1(f, l, m, dt) HBOOST_PP_REPEAT_FROM_TO_D_1(HBOOST_PP_AUTO_REC(HBOOST_PP_WHILE_P, 256), f, l, m, dt)
# define HBOOST_PP_REPEAT_FROM_TO_2(f, l, m, dt) HBOOST_PP_REPEAT_FROM_TO_D_2(HBOOST_PP_AUTO_REC(HBOOST_PP_WHILE_P, 256), f, l, m, dt)
# define HBOOST_PP_REPEAT_FROM_TO_3(f, l, m, dt) HBOOST_PP_REPEAT_FROM_TO_D_3(HBOOST_PP_AUTO_REC(HBOOST_PP_WHILE_P, 256), f, l, m, dt)
# define HBOOST_PP_REPEAT_FROM_TO_4(f, l, m, dt) HBOOST_PP_ERROR(0x0003)
#
# define HBOOST_PP_REPEAT_FROM_TO_1ST HBOOST_PP_REPEAT_FROM_TO_1
# define HBOOST_PP_REPEAT_FROM_TO_2ND HBOOST_PP_REPEAT_FROM_TO_2
# define HBOOST_PP_REPEAT_FROM_TO_3RD HBOOST_PP_REPEAT_FROM_TO_3
#
# /* HBOOST_PP_REPEAT_FROM_TO_D */
#
# if 0
#    define HBOOST_PP_REPEAT_FROM_TO_D(d, first, last, macro, data)
# endif
#
# define HBOOST_PP_REPEAT_FROM_TO_D HBOOST_PP_CAT(HBOOST_PP_REPEAT_FROM_TO_D_, HBOOST_PP_AUTO_REC(HBOOST_PP_REPEAT_P, 4))
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_REPEAT_FROM_TO_D_1(d, f, l, m, dt) HBOOST_PP_REPEAT_1(HBOOST_PP_SUB_D(d, l, f), HBOOST_PP_REPEAT_FROM_TO_M_1, (d, f, m, dt))
#    define HBOOST_PP_REPEAT_FROM_TO_D_2(d, f, l, m, dt) HBOOST_PP_REPEAT_2(HBOOST_PP_SUB_D(d, l, f), HBOOST_PP_REPEAT_FROM_TO_M_2, (d, f, m, dt))
#    define HBOOST_PP_REPEAT_FROM_TO_D_3(d, f, l, m, dt) HBOOST_PP_REPEAT_3(HBOOST_PP_SUB_D(d, l, f), HBOOST_PP_REPEAT_FROM_TO_M_3, (d, f, m, dt))
# else
#    define HBOOST_PP_REPEAT_FROM_TO_D_1(d, f, l, m, dt) HBOOST_PP_REPEAT_FROM_TO_D_1_I(d, f, l, m, dt)
#    define HBOOST_PP_REPEAT_FROM_TO_D_2(d, f, l, m, dt) HBOOST_PP_REPEAT_FROM_TO_D_2_I(d, f, l, m, dt)
#    define HBOOST_PP_REPEAT_FROM_TO_D_3(d, f, l, m, dt) HBOOST_PP_REPEAT_FROM_TO_D_3_I(d, f, l, m, dt)
#    define HBOOST_PP_REPEAT_FROM_TO_D_1_I(d, f, l, m, dt) HBOOST_PP_REPEAT_1(HBOOST_PP_SUB_D(d, l, f), HBOOST_PP_REPEAT_FROM_TO_M_1, (d, f, m, dt))
#    define HBOOST_PP_REPEAT_FROM_TO_D_2_I(d, f, l, m, dt) HBOOST_PP_REPEAT_2(HBOOST_PP_SUB_D(d, l, f), HBOOST_PP_REPEAT_FROM_TO_M_2, (d, f, m, dt))
#    define HBOOST_PP_REPEAT_FROM_TO_D_3_I(d, f, l, m, dt) HBOOST_PP_REPEAT_3(HBOOST_PP_SUB_D(d, l, f), HBOOST_PP_REPEAT_FROM_TO_M_3, (d, f, m, dt))
# endif
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_REPEAT_FROM_TO_M_1(z, n, dfmd) HBOOST_PP_REPEAT_FROM_TO_M_1_IM(z, n, HBOOST_PP_TUPLE_REM_4 dfmd)
#    define HBOOST_PP_REPEAT_FROM_TO_M_2(z, n, dfmd) HBOOST_PP_REPEAT_FROM_TO_M_2_IM(z, n, HBOOST_PP_TUPLE_REM_4 dfmd)
#    define HBOOST_PP_REPEAT_FROM_TO_M_3(z, n, dfmd) HBOOST_PP_REPEAT_FROM_TO_M_3_IM(z, n, HBOOST_PP_TUPLE_REM_4 dfmd)
#    define HBOOST_PP_REPEAT_FROM_TO_M_1_IM(z, n, im) HBOOST_PP_REPEAT_FROM_TO_M_1_I(z, n, im)
#    define HBOOST_PP_REPEAT_FROM_TO_M_2_IM(z, n, im) HBOOST_PP_REPEAT_FROM_TO_M_2_I(z, n, im)
#    define HBOOST_PP_REPEAT_FROM_TO_M_3_IM(z, n, im) HBOOST_PP_REPEAT_FROM_TO_M_3_I(z, n, im)
# else
#    define HBOOST_PP_REPEAT_FROM_TO_M_1(z, n, dfmd) HBOOST_PP_REPEAT_FROM_TO_M_1_I(z, n, HBOOST_PP_TUPLE_ELEM(4, 0, dfmd), HBOOST_PP_TUPLE_ELEM(4, 1, dfmd), HBOOST_PP_TUPLE_ELEM(4, 2, dfmd), HBOOST_PP_TUPLE_ELEM(4, 3, dfmd))
#    define HBOOST_PP_REPEAT_FROM_TO_M_2(z, n, dfmd) HBOOST_PP_REPEAT_FROM_TO_M_2_I(z, n, HBOOST_PP_TUPLE_ELEM(4, 0, dfmd), HBOOST_PP_TUPLE_ELEM(4, 1, dfmd), HBOOST_PP_TUPLE_ELEM(4, 2, dfmd), HBOOST_PP_TUPLE_ELEM(4, 3, dfmd))
#    define HBOOST_PP_REPEAT_FROM_TO_M_3(z, n, dfmd) HBOOST_PP_REPEAT_FROM_TO_M_3_I(z, n, HBOOST_PP_TUPLE_ELEM(4, 0, dfmd), HBOOST_PP_TUPLE_ELEM(4, 1, dfmd), HBOOST_PP_TUPLE_ELEM(4, 2, dfmd), HBOOST_PP_TUPLE_ELEM(4, 3, dfmd))
# endif
#
# define HBOOST_PP_REPEAT_FROM_TO_M_1_I(z, n, d, f, m, dt) HBOOST_PP_REPEAT_FROM_TO_M_1_II(z, HBOOST_PP_ADD_D(d, n, f), m, dt)
# define HBOOST_PP_REPEAT_FROM_TO_M_2_I(z, n, d, f, m, dt) HBOOST_PP_REPEAT_FROM_TO_M_2_II(z, HBOOST_PP_ADD_D(d, n, f), m, dt)
# define HBOOST_PP_REPEAT_FROM_TO_M_3_I(z, n, d, f, m, dt) HBOOST_PP_REPEAT_FROM_TO_M_3_II(z, HBOOST_PP_ADD_D(d, n, f), m, dt)
#
# define HBOOST_PP_REPEAT_FROM_TO_M_1_II(z, n, m, dt) m(z, n, dt)
# define HBOOST_PP_REPEAT_FROM_TO_M_2_II(z, n, m, dt) m(z, n, dt)
# define HBOOST_PP_REPEAT_FROM_TO_M_3_II(z, n, m, dt) m(z, n, dt)
#
# endif
