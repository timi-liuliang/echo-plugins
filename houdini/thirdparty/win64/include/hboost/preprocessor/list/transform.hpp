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
# ifndef HBOOST_PREPROCESSOR_LIST_TRANSFORM_HPP
# define HBOOST_PREPROCESSOR_LIST_TRANSFORM_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/list/fold_right.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_LIST_TRANSFORM */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_TRANSFORM(op, data, list) HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_LIST_FOLD_RIGHT(HBOOST_PP_LIST_TRANSFORM_O, (op, data, HBOOST_PP_NIL), list))
# else
#    define HBOOST_PP_LIST_TRANSFORM(op, data, list) HBOOST_PP_LIST_TRANSFORM_I(op, data, list)
#    define HBOOST_PP_LIST_TRANSFORM_I(op, data, list) HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_LIST_FOLD_RIGHT(HBOOST_PP_LIST_TRANSFORM_O, (op, data, HBOOST_PP_NIL), list))
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_TRANSFORM_O(d, odr, elem) HBOOST_PP_LIST_TRANSFORM_O_D(d, HBOOST_PP_TUPLE_ELEM(3, 0, odr), HBOOST_PP_TUPLE_ELEM(3, 1, odr), HBOOST_PP_TUPLE_ELEM(3, 2, odr), elem)
# else
#    define HBOOST_PP_LIST_TRANSFORM_O(d, odr, elem) HBOOST_PP_LIST_TRANSFORM_O_I(d, HBOOST_PP_TUPLE_REM_3 odr, elem)
#    define HBOOST_PP_LIST_TRANSFORM_O_I(d, im, elem) HBOOST_PP_LIST_TRANSFORM_O_D(d, im, elem)
# endif
#
# define HBOOST_PP_LIST_TRANSFORM_O_D(d, op, data, res, elem) (op, data, (op(d, data, elem), res))
#
# /* HBOOST_PP_LIST_TRANSFORM_D */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LIST_TRANSFORM_D(d, op, data, list) HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_LIST_FOLD_RIGHT_ ## d(HBOOST_PP_LIST_TRANSFORM_O, (op, data, HBOOST_PP_NIL), list))
# else
#    define HBOOST_PP_LIST_TRANSFORM_D(d, op, data, list) HBOOST_PP_LIST_TRANSFORM_D_I(d, op, data, list)
#    define HBOOST_PP_LIST_TRANSFORM_D_I(d, op, data, list) HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_LIST_FOLD_RIGHT_ ## d(HBOOST_PP_LIST_TRANSFORM_O, (op, data, HBOOST_PP_NIL), list))
# endif
#
# endif
