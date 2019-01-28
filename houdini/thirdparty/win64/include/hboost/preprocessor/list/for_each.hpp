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
# ifndef HBOOST_PREPROCESSOR_LIST_FOR_EACH_HPP
# define HBOOST_PREPROCESSOR_LIST_FOR_EACH_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/for_each_i.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_LIST_FOR_EACH */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FOR_EACH(macro, data, list) HBOOST_PP_LIST_FOR_EACH_I(HBOOST_PP_LIST_FOR_EACH_O, (macro, data), list)
# else
#    define HBOOST_PP_LIST_FOR_EACH(macro, data, list) HBOOST_PP_LIST_FOR_EACH_X(macro, data, list)
#    define HBOOST_PP_LIST_FOR_EACH_X(macro, data, list) HBOOST_PP_LIST_FOR_EACH_I(HBOOST_PP_LIST_FOR_EACH_O, (macro, data), list)
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FOR_EACH_O(r, md, i, elem) HBOOST_PP_LIST_FOR_EACH_O_D(r, HBOOST_PP_TUPLE_ELEM(2, 0, md), HBOOST_PP_TUPLE_ELEM(2, 1, md), elem)
# else
#    define HBOOST_PP_LIST_FOR_EACH_O(r, md, i, elem) HBOOST_PP_LIST_FOR_EACH_O_I(r, HBOOST_PP_TUPLE_REM_2 md, elem)
#    define HBOOST_PP_LIST_FOR_EACH_O_I(r, im, elem) HBOOST_PP_LIST_FOR_EACH_O_D(r, im, elem)
# endif
#
# define HBOOST_PP_LIST_FOR_EACH_O_D(r, m, d, elem) m(r, d, elem)
#
# /* HBOOST_PP_LIST_FOR_EACH_R */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FOR_EACH_R(r, macro, data, list) HBOOST_PP_LIST_FOR_EACH_I_R(r, HBOOST_PP_LIST_FOR_EACH_O, (macro, data), list)
# else
#    define HBOOST_PP_LIST_FOR_EACH_R(r, macro, data, list) HBOOST_PP_LIST_FOR_EACH_R_X(r, macro, data, list)
#    define HBOOST_PP_LIST_FOR_EACH_R_X(r, macro, data, list) HBOOST_PP_LIST_FOR_EACH_I_R(r, HBOOST_PP_LIST_FOR_EACH_O, (macro, data), list)
# endif
#
# endif
