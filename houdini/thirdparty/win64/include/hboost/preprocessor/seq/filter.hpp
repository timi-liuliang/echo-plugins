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
# ifndef HBOOST_PREPROCESSOR_SEQ_FILTER_HPP
# define HBOOST_PREPROCESSOR_SEQ_FILTER_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/expr_if.hpp>
# include <hboost/preprocessor/facilities/empty.hpp>
# include <hboost/preprocessor/seq/fold_left.hpp>
# include <hboost/preprocessor/seq/seq.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_SEQ_FILTER */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_FILTER(pred, data, seq) HBOOST_PP_SEQ_TAIL(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# else
#    define HBOOST_PP_SEQ_FILTER(pred, data, seq) HBOOST_PP_SEQ_FILTER_I(pred, data, seq)
#    define HBOOST_PP_SEQ_FILTER_I(pred, data, seq) HBOOST_PP_SEQ_TAIL(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# endif
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_SEQ_FILTER_O(s, st, elem) HBOOST_PP_SEQ_FILTER_O_IM(s, HBOOST_PP_TUPLE_REM_3 st, elem)
#    define HBOOST_PP_SEQ_FILTER_O_IM(s, im, elem) HBOOST_PP_SEQ_FILTER_O_I(s, im, elem)
# else
#    define HBOOST_PP_SEQ_FILTER_O(s, st, elem) HBOOST_PP_SEQ_FILTER_O_I(s, HBOOST_PP_TUPLE_ELEM(3, 0, st), HBOOST_PP_TUPLE_ELEM(3, 1, st), HBOOST_PP_TUPLE_ELEM(3, 2, st), elem)
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_DMC()
#   define HBOOST_PP_SEQ_FILTER_O_I(s, pred, data, res, elem) (pred, data, res HBOOST_PP_EXPR_IF(pred(s, data, elem), (elem)))
# else
#   define HBOOST_PP_SEQ_FILTER_O_I(s, pred, data, res, elem) (pred, data, res HBOOST_PP_EXPR_IF(pred##(s, data, elem), (elem)))
# endif
#
# /* HBOOST_PP_SEQ_FILTER_S */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_FILTER_S(s, pred, data, seq) HBOOST_PP_SEQ_TAIL(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_SEQ_FOLD_LEFT_ ## s(HBOOST_PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# else
#    define HBOOST_PP_SEQ_FILTER_S(s, pred, data, seq) HBOOST_PP_SEQ_FILTER_S_I(s, pred, data, seq)
#    define HBOOST_PP_SEQ_FILTER_S_I(s, pred, data, seq) HBOOST_PP_SEQ_TAIL(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_SEQ_FOLD_LEFT_ ## s(HBOOST_PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# endif
#
# endif
