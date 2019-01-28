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
# ifndef HBOOST_PREPROCESSOR_LOGICAL_COMPL_HPP
# define HBOOST_PREPROCESSOR_LOGICAL_COMPL_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_COMPL */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_COMPL(x) HBOOST_PP_COMPL_I(x)
# else
#    define HBOOST_PP_COMPL(x) HBOOST_PP_COMPL_OO((x))
#    define HBOOST_PP_COMPL_OO(par) HBOOST_PP_COMPL_I ## par
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_COMPL_I(x) HBOOST_PP_COMPL_ ## x
# else
#    define HBOOST_PP_COMPL_I(x) HBOOST_PP_COMPL_ID(HBOOST_PP_COMPL_ ## x)
#    define HBOOST_PP_COMPL_ID(id) id
# endif
#
# define HBOOST_PP_COMPL_0 1
# define HBOOST_PP_COMPL_1 0
#
# endif
