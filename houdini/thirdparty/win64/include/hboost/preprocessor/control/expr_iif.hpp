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
# ifndef HBOOST_PREPROCESSOR_CONTROL_EXPR_IIF_HPP
# define HBOOST_PREPROCESSOR_CONTROL_EXPR_IIF_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_EXPR_IIF */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_EXPR_IIF(bit, expr) HBOOST_PP_EXPR_IIF_I(bit, expr)
# else
#    define HBOOST_PP_EXPR_IIF(bit, expr) HBOOST_PP_EXPR_IIF_OO((bit, expr))
#    define HBOOST_PP_EXPR_IIF_OO(par) HBOOST_PP_EXPR_IIF_I ## par
# endif
#
# define HBOOST_PP_EXPR_IIF_I(bit, expr) HBOOST_PP_EXPR_IIF_ ## bit(expr)
#
# define HBOOST_PP_EXPR_IIF_0(expr)
# define HBOOST_PP_EXPR_IIF_1(expr) expr
#
# endif
