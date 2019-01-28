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
# ifndef HBOOST_PREPROCESSOR_SEQ_POP_BACK_HPP
# define HBOOST_PREPROCESSOR_SEQ_POP_BACK_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/seq/first_n.hpp>
# include <hboost/preprocessor/seq/size.hpp>
#
# /* HBOOST_PP_SEQ_POP_BACK */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_POP_BACK(seq) HBOOST_PP_SEQ_FIRST_N(HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(seq)), seq)
# else
#    define HBOOST_PP_SEQ_POP_BACK(seq) HBOOST_PP_SEQ_POP_BACK_I(seq)
#    define HBOOST_PP_SEQ_POP_BACK_I(seq) HBOOST_PP_SEQ_FIRST_N(HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(seq)), seq)
# endif
#
# endif
