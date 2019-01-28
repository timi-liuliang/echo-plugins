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
# ifndef HBOOST_PREPROCESSOR_SEQ_SEQ_HPP
# define HBOOST_PREPROCESSOR_SEQ_SEQ_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/seq/elem.hpp>
#
# /* HBOOST_PP_SEQ_HEAD */
#
# define HBOOST_PP_SEQ_HEAD(seq) HBOOST_PP_SEQ_ELEM(0, seq)
#
# /* HBOOST_PP_SEQ_TAIL */
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_SEQ_TAIL(seq) HBOOST_PP_SEQ_TAIL_1((seq))
#    define HBOOST_PP_SEQ_TAIL_1(par) HBOOST_PP_SEQ_TAIL_2 ## par
#    define HBOOST_PP_SEQ_TAIL_2(seq) HBOOST_PP_SEQ_TAIL_I ## seq
# elif HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_SEQ_TAIL(seq) HBOOST_PP_SEQ_TAIL_ID(HBOOST_PP_SEQ_TAIL_I seq)
#    define HBOOST_PP_SEQ_TAIL_ID(id) id
# elif HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_TAIL(seq) HBOOST_PP_SEQ_TAIL_D(seq)
#    define HBOOST_PP_SEQ_TAIL_D(seq) HBOOST_PP_SEQ_TAIL_I seq
# else
#    define HBOOST_PP_SEQ_TAIL(seq) HBOOST_PP_SEQ_TAIL_I seq
# endif
#
# define HBOOST_PP_SEQ_TAIL_I(x)
#
# /* HBOOST_PP_SEQ_NIL */
#
# define HBOOST_PP_SEQ_NIL(x) (x)
#
# endif
