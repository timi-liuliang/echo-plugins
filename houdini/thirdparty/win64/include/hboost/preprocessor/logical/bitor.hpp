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
# ifndef HBOOST_PREPROCESSOR_LOGICAL_BITOR_HPP
# define HBOOST_PREPROCESSOR_LOGICAL_BITOR_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_BITOR */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_BITOR(x, y) HBOOST_PP_BITOR_I(x, y)
# else
#    define HBOOST_PP_BITOR(x, y) HBOOST_PP_BITOR_OO((x, y))
#    define HBOOST_PP_BITOR_OO(par) HBOOST_PP_BITOR_I ## par
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_BITOR_I(x, y) HBOOST_PP_BITOR_ ## x ## y
# else
#    define HBOOST_PP_BITOR_I(x, y) HBOOST_PP_BITOR_ID(HBOOST_PP_BITOR_ ## x ## y)
#    define HBOOST_PP_BITOR_ID(id) id
# endif
#
# define HBOOST_PP_BITOR_00 0
# define HBOOST_PP_BITOR_01 1
# define HBOOST_PP_BITOR_10 1
# define HBOOST_PP_BITOR_11 1
#
# endif
