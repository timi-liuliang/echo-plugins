# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2012.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_SEQ_VARIADIC_SEQ_TO_SEQ_HPP
# define HBOOST_PREPROCESSOR_SEQ_VARIADIC_SEQ_TO_SEQ_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_VARIADIC_SEQ_TO_SEQ */
#
# if HBOOST_PP_VARIADICS
#    define HBOOST_PP_VARIADIC_SEQ_TO_SEQ(vseq) HBOOST_PP_CAT(HBOOST_PP_VARIADIC_SEQ_TO_SEQ_A vseq, 0)
#    define HBOOST_PP_VARIADIC_SEQ_TO_SEQ_A(...) ((__VA_ARGS__)) HBOOST_PP_VARIADIC_SEQ_TO_SEQ_B
#    define HBOOST_PP_VARIADIC_SEQ_TO_SEQ_B(...) ((__VA_ARGS__)) HBOOST_PP_VARIADIC_SEQ_TO_SEQ_A
#    define HBOOST_PP_VARIADIC_SEQ_TO_SEQ_A0
#    define HBOOST_PP_VARIADIC_SEQ_TO_SEQ_B0
# endif
#
# endif
