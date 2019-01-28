# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_SEQ_TO_LIST_HPP
# define HBOOST_PREPROCESSOR_SEQ_TO_LIST_HPP
#
# include <hboost/preprocessor/punctuation/comma.hpp>
# include <hboost/preprocessor/punctuation/paren.hpp>
# include <hboost/preprocessor/seq/detail/binary_transform.hpp>
#
# /* HBOOST_PP_SEQ_TO_LIST */
#
# define HBOOST_PP_SEQ_TO_LIST(seq) HBOOST_PP_SEQ_TO_LIST_I(HBOOST_PP_SEQ_BINARY_TRANSFORM(seq))
# define HBOOST_PP_SEQ_TO_LIST_I(bseq) HBOOST_PP_SEQ_TO_LIST_A bseq HBOOST_PP_NIL HBOOST_PP_SEQ_TO_LIST_B bseq
# define HBOOST_PP_SEQ_TO_LIST_A(m, e) m(HBOOST_PP_LPAREN() e HBOOST_PP_COMMA() HBOOST_PP_SEQ_TO_LIST_A_ID)
# define HBOOST_PP_SEQ_TO_LIST_A_ID() HBOOST_PP_SEQ_TO_LIST_A
# define HBOOST_PP_SEQ_TO_LIST_B(m, e) m(HBOOST_PP_RPAREN() HBOOST_PP_SEQ_TO_LIST_B_ID)
# define HBOOST_PP_SEQ_TO_LIST_B_ID() HBOOST_PP_SEQ_TO_LIST_B
#
# endif
