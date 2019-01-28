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
# ifndef HBOOST_PREPROCESSOR_SEQ_TO_TUPLE_HPP
# define HBOOST_PREPROCESSOR_SEQ_TO_TUPLE_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/seq/enum.hpp>
#
# /* HBOOST_PP_SEQ_TO_TUPLE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_TO_TUPLE(seq) (HBOOST_PP_SEQ_ENUM(seq))
# else
#    define HBOOST_PP_SEQ_TO_TUPLE(seq) HBOOST_PP_SEQ_TO_TUPLE_I(seq)
#    define HBOOST_PP_SEQ_TO_TUPLE_I(seq) (HBOOST_PP_SEQ_ENUM(seq))
# endif
#
# endif
