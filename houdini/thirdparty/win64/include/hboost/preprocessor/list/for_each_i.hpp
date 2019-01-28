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
# ifndef HBOOST_PREPROCESSOR_LIST_LIST_FOR_EACH_I_HPP
# define HBOOST_PREPROCESSOR_LIST_LIST_FOR_EACH_I_HPP
#
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/adt.hpp>
# include <hboost/preprocessor/repetition/for.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_LIST_FOR_EACH_I */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG() && ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_LIST_FOR_EACH_I(macro, data, list) HBOOST_PP_FOR((macro, data, list, 0), HBOOST_PP_LIST_FOR_EACH_I_P, HBOOST_PP_LIST_FOR_EACH_I_O, HBOOST_PP_LIST_FOR_EACH_I_M)
# else
#    define HBOOST_PP_LIST_FOR_EACH_I(macro, data, list) HBOOST_PP_LIST_FOR_EACH_I_I(macro, data, list)
#    define HBOOST_PP_LIST_FOR_EACH_I_I(macro, data, list) HBOOST_PP_FOR((macro, data, list, 0), HBOOST_PP_LIST_FOR_EACH_I_P, HBOOST_PP_LIST_FOR_EACH_I_O, HBOOST_PP_LIST_FOR_EACH_I_M)
# endif
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_LIST_FOR_EACH_I_P(r, x) HBOOST_PP_LIST_FOR_EACH_I_P_D x
#    define HBOOST_PP_LIST_FOR_EACH_I_P_D(m, d, l, i) HBOOST_PP_LIST_IS_CONS(l)
# else
#    define HBOOST_PP_LIST_FOR_EACH_I_P(r, x) HBOOST_PP_LIST_IS_CONS(HBOOST_PP_TUPLE_ELEM(4, 2, x))
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_LIST_FOR_EACH_I_O(r, x) HBOOST_PP_LIST_FOR_EACH_I_O_D x
#    define HBOOST_PP_LIST_FOR_EACH_I_O_D(m, d, l, i) (m, d, HBOOST_PP_LIST_REST(l), HBOOST_PP_INC(i))
# else
#    define HBOOST_PP_LIST_FOR_EACH_I_O(r, x) (HBOOST_PP_TUPLE_ELEM(4, 0, x), HBOOST_PP_TUPLE_ELEM(4, 1, x), HBOOST_PP_LIST_REST(HBOOST_PP_TUPLE_ELEM(4, 2, x)), HBOOST_PP_INC(HBOOST_PP_TUPLE_ELEM(4, 3, x)))
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FOR_EACH_I_M(r, x) HBOOST_PP_LIST_FOR_EACH_I_M_D(r, HBOOST_PP_TUPLE_ELEM(4, 0, x), HBOOST_PP_TUPLE_ELEM(4, 1, x), HBOOST_PP_TUPLE_ELEM(4, 2, x), HBOOST_PP_TUPLE_ELEM(4, 3, x))
# else
#    define HBOOST_PP_LIST_FOR_EACH_I_M(r, x) HBOOST_PP_LIST_FOR_EACH_I_M_I(r, HBOOST_PP_TUPLE_REM_4 x)
#    define HBOOST_PP_LIST_FOR_EACH_I_M_I(r, x_e) HBOOST_PP_LIST_FOR_EACH_I_M_D(r, x_e)
# endif
#
# define HBOOST_PP_LIST_FOR_EACH_I_M_D(r, m, d, l, i) m(r, d, i, HBOOST_PP_LIST_FIRST(l))
#
# /* HBOOST_PP_LIST_FOR_EACH_I_R */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FOR_EACH_I_R(r, macro, data, list) HBOOST_PP_FOR_ ## r((macro, data, list, 0), HBOOST_PP_LIST_FOR_EACH_I_P, HBOOST_PP_LIST_FOR_EACH_I_O, HBOOST_PP_LIST_FOR_EACH_I_M)
# else
#    define HBOOST_PP_LIST_FOR_EACH_I_R(r, macro, data, list) HBOOST_PP_LIST_FOR_EACH_I_R_I(r, macro, data, list)
#    define HBOOST_PP_LIST_FOR_EACH_I_R_I(r, macro, data, list) HBOOST_PP_FOR_ ## r((macro, data, list, 0), HBOOST_PP_LIST_FOR_EACH_I_P, HBOOST_PP_LIST_FOR_EACH_I_O, HBOOST_PP_LIST_FOR_EACH_I_M)
# endif
#
# endif
