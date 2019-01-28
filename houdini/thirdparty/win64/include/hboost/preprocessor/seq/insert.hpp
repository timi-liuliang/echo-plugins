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
# ifndef HBOOST_PREPROCESSOR_SEQ_INSERT_HPP
# define HBOOST_PREPROCESSOR_SEQ_INSERT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/seq/first_n.hpp>
# include <hboost/preprocessor/seq/rest_n.hpp>
#
# /* HBOOST_PP_SEQ_INSERT */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_INSERT(seq, i, elem) HBOOST_PP_SEQ_FIRST_N(i, seq) (elem) HBOOST_PP_SEQ_REST_N(i, seq)
# else
#    define HBOOST_PP_SEQ_INSERT(seq, i, elem) HBOOST_PP_SEQ_INSERT_I(seq, i, elem)
#    define HBOOST_PP_SEQ_INSERT_I(seq, i, elem) HBOOST_PP_SEQ_FIRST_N(i, seq) (elem) HBOOST_PP_SEQ_REST_N(i, seq)
# endif
#
# endif
