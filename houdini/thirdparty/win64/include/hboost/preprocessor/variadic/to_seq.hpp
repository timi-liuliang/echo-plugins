# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_VARIADIC_TO_SEQ_HPP
# define HBOOST_PREPROCESSOR_VARIADIC_TO_SEQ_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/tuple/to_seq.hpp>
#
# /* HBOOST_PP_VARIADIC_TO_SEQ */
#
# if HBOOST_PP_VARIADICS
#    define HBOOST_PP_VARIADIC_TO_SEQ(...) HBOOST_PP_TUPLE_TO_SEQ((__VA_ARGS__))
# endif
#
# endif
