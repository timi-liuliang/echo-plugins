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
# ifndef HBOOST_PREPROCESSOR_SEQ_TRANSFORM_HPP
# define HBOOST_PREPROCESSOR_SEQ_TRANSFORM_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/seq/fold_left.hpp>
# include <hboost/preprocessor/seq/seq.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_SEQ_TRANSFORM */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_TRANSFORM(op, data, seq) HBOOST_PP_SEQ_TAIL(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# else
#    define HBOOST_PP_SEQ_TRANSFORM(op, data, seq) HBOOST_PP_SEQ_TRANSFORM_I(op, data, seq)
#    define HBOOST_PP_SEQ_TRANSFORM_I(op, data, seq) HBOOST_PP_SEQ_TAIL(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# endif
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_SEQ_TRANSFORM_O(s, state, elem) HBOOST_PP_SEQ_TRANSFORM_O_IM(s, HBOOST_PP_TUPLE_REM_3 state, elem)
#    define HBOOST_PP_SEQ_TRANSFORM_O_IM(s, im, elem) HBOOST_PP_SEQ_TRANSFORM_O_I(s, im, elem)
# else
#    define HBOOST_PP_SEQ_TRANSFORM_O(s, state, elem) HBOOST_PP_SEQ_TRANSFORM_O_I(s, HBOOST_PP_TUPLE_ELEM(3, 0, state), HBOOST_PP_TUPLE_ELEM(3, 1, state), HBOOST_PP_TUPLE_ELEM(3, 2, state), elem)
# endif
#
# define HBOOST_PP_SEQ_TRANSFORM_O_I(s, op, data, res, elem) (op, data, res (op(s, data, elem)))
#
# /* HBOOST_PP_SEQ_TRANSFORM_S */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_TRANSFORM_S(s, op, data, seq) HBOOST_PP_SEQ_TAIL(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_SEQ_FOLD_LEFT_ ## s(HBOOST_PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# else
#    define HBOOST_PP_SEQ_TRANSFORM_S(s, op, data, seq) HBOOST_PP_SEQ_TRANSFORM_S_I(s, op, data, seq)
#    define HBOOST_PP_SEQ_TRANSFORM_S_I(s, op, data, seq) HBOOST_PP_SEQ_TAIL(HBOOST_PP_TUPLE_ELEM(3, 2, HBOOST_PP_SEQ_FOLD_LEFT_ ## s(HBOOST_PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# endif
#
# endif
