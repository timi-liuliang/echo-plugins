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
# ifndef HBOOST_PREPROCESSOR_SEQ_POP_FRONT_HPP
# define HBOOST_PREPROCESSOR_SEQ_POP_FRONT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/seq/seq.hpp>
#
# /* HBOOST_PP_SEQ_POP_FRONT */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_POP_FRONT(seq) HBOOST_PP_SEQ_TAIL(seq)
# else
#    define HBOOST_PP_SEQ_POP_FRONT(seq) HBOOST_PP_SEQ_POP_FRONT_I(seq)
#    define HBOOST_PP_SEQ_POP_FRONT_I(seq) HBOOST_PP_SEQ_TAIL(seq)
# endif
#
# endif
