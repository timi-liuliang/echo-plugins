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
# ifndef HBOOST_PREPROCESSOR_FACILITIES_APPLY_HPP
# define HBOOST_PREPROCESSOR_FACILITIES_APPLY_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/expr_iif.hpp>
# include <hboost/preprocessor/detail/is_unary.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_APPLY */
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_APPLY(x) HBOOST_PP_APPLY_I(x)
#    define HBOOST_PP_APPLY_I(x) HBOOST_PP_EXPR_IIF(HBOOST_PP_IS_UNARY(x), HBOOST_PP_TUPLE_REM_1 x)
# elif HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_BCC()
#    define HBOOST_PP_APPLY(x) HBOOST_PP_APPLY_I(x)
#    define HBOOST_PP_APPLY_I(x) HBOOST_PP_APPLY_ ## x
#    define HBOOST_PP_APPLY_(x) x
#    define HBOOST_PP_APPLY_HBOOST_PP_NIL
# else
#    define HBOOST_PP_APPLY(x) HBOOST_PP_EXPR_IIF(HBOOST_PP_IS_UNARY(x), HBOOST_PP_TUPLE_REM_1 x)
# endif
#
# endif
