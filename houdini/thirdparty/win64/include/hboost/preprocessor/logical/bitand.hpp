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
# ifndef HBOOST_PREPROCESSOR_LOGICAL_BITAND_HPP
# define HBOOST_PREPROCESSOR_LOGICAL_BITAND_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_BITAND */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_BITAND(x, y) HBOOST_PP_BITAND_I(x, y)
# else
#    define HBOOST_PP_BITAND(x, y) HBOOST_PP_BITAND_OO((x, y))
#    define HBOOST_PP_BITAND_OO(par) HBOOST_PP_BITAND_I ## par
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_BITAND_I(x, y) HBOOST_PP_BITAND_ ## x ## y
# else
#    define HBOOST_PP_BITAND_I(x, y) HBOOST_PP_BITAND_ID(HBOOST_PP_BITAND_ ## x ## y)
#    define HBOOST_PP_BITAND_ID(res) res
# endif
#
# define HBOOST_PP_BITAND_00 0
# define HBOOST_PP_BITAND_01 0
# define HBOOST_PP_BITAND_10 0
# define HBOOST_PP_BITAND_11 1
#
# endif
