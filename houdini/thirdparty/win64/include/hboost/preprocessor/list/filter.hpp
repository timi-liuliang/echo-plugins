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
# ifndef HBOOST_PREPROCESSOR_LIST_FILTER_HPP
# define HBOOST_PREPROCESSOR_LIST_FILTER_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/list/fold_right.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_LIST_FILTER */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FILTER(pred, data, list) HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_LIST_FOLD_RIGHT(HBOOST_PP_LIST_FILTER_O, (pred, data, HBOOST_PP_NIL), list))
# else
#    define HBOOST_PP_LIST_FILTER(pred, data, list) HBOOST_PP_LIST_FILTER_I(pred, data, list)
#    define HBOOST_PP_LIST_FILTER_I(pred, data, list) HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_LIST_FOLD_RIGHT(HBOOST_PP_LIST_FILTER_O, (pred, data, HBOOST_PP_NIL), list))
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FILTER_O(d, pdr, elem) HBOOST_PP_LIST_FILTER_O_D(d, HBOOST_PP_TUPLE_ELEM(3, 0, pdr), HBOOST_PP_TUPLE_ELEM(3, 1, pdr), HBOOST_PP_TUPLE_ELEM(3, 2, pdr), elem)
# else
#    define HBOOST_PP_LIST_FILTER_O(d, pdr, elem) HBOOST_PP_LIST_FILTER_O_I(d, HBOOST_PP_TUPLE_REM_3 pdr, elem)
#    define HBOOST_PP_LIST_FILTER_O_I(d, im, elem) HBOOST_PP_LIST_FILTER_O_D(d, im, elem)
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_DMC()
#    define HBOOST_PP_LIST_FILTER_O_D(d, pred, data, res, elem) (pred, data, HBOOST_PP_IF(pred(d, data, elem), (elem, res), res))
# else
#    define HBOOST_PP_LIST_FILTER_O_D(d, pred, data, res, elem) (pred, data, HBOOST_PP_IF(pred##(d, data, elem), (elem, res), res))
# endif
#
# /* HBOOST_PP_LIST_FILTER_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_FILTER_D(d, pred, data, list) HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_LIST_FOLD_RIGHT_ ## d(HBOOST_PP_LIST_FILTER_O, (pred, data, HBOOST_PP_NIL), list))
# else
#    define HBOOST_PP_LIST_FILTER_D(d, pred, data, list) HBOOST_PP_LIST_FILTER_D_I(d, pred, data, list)
#    define HBOOST_PP_LIST_FILTER_D_I(d, pred, data, list) HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_LIST_FOLD_RIGHT_ ## d(HBOOST_PP_LIST_FILTER_O, (pred, data, HBOOST_PP_NIL), list))
# endif
#
# endif
