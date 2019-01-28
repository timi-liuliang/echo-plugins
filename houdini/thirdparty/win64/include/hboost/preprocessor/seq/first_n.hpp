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
# ifndef HBOOST_PREPROCESSOR_SEQ_FIRST_N_HPP
# define HBOOST_PREPROCESSOR_SEQ_FIRST_N_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/seq/detail/split.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_SEQ_FIRST_N */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_FIRST_N(n, seq) HBOOST_PP_IF(n, HBOOST_PP_TUPLE_ELEM, HBOOST_PP_TUPLE_EAT_3)(2, 0, HBOOST_PP_SEQ_SPLIT(n, seq (nil)))
# else
#    define HBOOST_PP_SEQ_FIRST_N(n, seq) HBOOST_PP_SEQ_FIRST_N_I(n, seq)
#    define HBOOST_PP_SEQ_FIRST_N_I(n, seq) HBOOST_PP_IF(n, HBOOST_PP_TUPLE_ELEM, HBOOST_PP_TUPLE_EAT_3)(2, 0, HBOOST_PP_SEQ_SPLIT(n, seq (nil)))
# endif
#
# endif
