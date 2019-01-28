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
# ifndef HBOOST_PREPROCESSOR_SEQ_CAT_HPP
# define HBOOST_PREPROCESSOR_SEQ_CAT_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/seq/fold_left.hpp>
# include <hboost/preprocessor/seq/seq.hpp>
# include <hboost/preprocessor/seq/size.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
#
# /* HBOOST_PP_SEQ_CAT */
#
# define HBOOST_PP_SEQ_CAT(seq) \
    HBOOST_PP_IF( \
        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(seq)), \
        HBOOST_PP_SEQ_CAT_I, \
        HBOOST_PP_SEQ_HEAD \
    )(seq) \
    /**/
# define HBOOST_PP_SEQ_CAT_I(seq) HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_PP_SEQ_CAT_O, HBOOST_PP_SEQ_HEAD(seq), HBOOST_PP_SEQ_TAIL(seq))
#
# define HBOOST_PP_SEQ_CAT_O(s, st, elem) HBOOST_PP_SEQ_CAT_O_I(st, elem)
# define HBOOST_PP_SEQ_CAT_O_I(a, b) a ## b
#
# /* HBOOST_PP_SEQ_CAT_S */
#
# define HBOOST_PP_SEQ_CAT_S(s, seq) \
    HBOOST_PP_IF( \
        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(seq)), \
        HBOOST_PP_SEQ_CAT_S_I_A, \
        HBOOST_PP_SEQ_CAT_S_I_B \
    )(s, seq) \
    /**/
# define HBOOST_PP_SEQ_CAT_S_I_A(s, seq) HBOOST_PP_SEQ_FOLD_LEFT_ ## s(HBOOST_PP_SEQ_CAT_O, HBOOST_PP_SEQ_HEAD(seq), HBOOST_PP_SEQ_TAIL(seq))
# define HBOOST_PP_SEQ_CAT_S_I_B(s, seq) HBOOST_PP_SEQ_HEAD(seq)
#
# endif
