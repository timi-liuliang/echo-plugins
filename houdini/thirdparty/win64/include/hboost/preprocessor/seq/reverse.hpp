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
# ifndef HBOOST_PREPROCESSOR_SEQ_REVERSE_HPP
# define HBOOST_PREPROCESSOR_SEQ_REVERSE_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/facilities/empty.hpp>
# include <hboost/preprocessor/seq/fold_left.hpp>
#
# /* HBOOST_PP_SEQ_REVERSE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_REVERSE(seq) HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_PP_SEQ_REVERSE_O, HBOOST_PP_EMPTY, seq)()
# else
#    define HBOOST_PP_SEQ_REVERSE(seq) HBOOST_PP_SEQ_REVERSE_I(seq)
#    define HBOOST_PP_SEQ_REVERSE_I(seq) HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_PP_SEQ_REVERSE_O, HBOOST_PP_EMPTY, seq)()
# endif
#
# define HBOOST_PP_SEQ_REVERSE_O(s, state, elem) (elem) state
#
# /* HBOOST_PP_SEQ_REVERSE_S */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_REVERSE_S(s, seq) HBOOST_PP_SEQ_FOLD_LEFT_ ## s(HBOOST_PP_SEQ_REVERSE_O, HBOOST_PP_EMPTY, seq)()
# else
#    define HBOOST_PP_SEQ_REVERSE_S(s, seq) HBOOST_PP_SEQ_REVERSE_S_I(s, seq)
#    define HBOOST_PP_SEQ_REVERSE_S_I(s, seq) HBOOST_PP_SEQ_FOLD_LEFT_ ## s(HBOOST_PP_SEQ_REVERSE_O, HBOOST_PP_EMPTY, seq)()
# endif
#
# endif
