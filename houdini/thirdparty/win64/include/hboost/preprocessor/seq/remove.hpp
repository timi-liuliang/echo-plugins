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
# ifndef HBOOST_PREPROCESSOR_SEQ_REMOVE_HPP
# define HBOOST_PREPROCESSOR_SEQ_REMOVE_HPP
#
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/seq/first_n.hpp>
# include <hboost/preprocessor/seq/rest_n.hpp>
#
# /* HBOOST_PP_SEQ_REMOVE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_REMOVE(seq, i) HBOOST_PP_SEQ_FIRST_N(i, seq) HBOOST_PP_SEQ_REST_N(HBOOST_PP_INC(i), seq)
# else
#    define HBOOST_PP_SEQ_REMOVE(seq, i) HBOOST_PP_SEQ_REMOVE_I(seq, i)
#    define HBOOST_PP_SEQ_REMOVE_I(seq, i) HBOOST_PP_SEQ_FIRST_N(i, seq) HBOOST_PP_SEQ_REST_N(HBOOST_PP_INC(i), seq)
# endif
#
# endif
