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
# ifndef HBOOST_PREPROCESSOR_WSTRINGIZE_HPP
# define HBOOST_PREPROCESSOR_WSTRINGIZE_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_WSTRINGIZE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_WSTRINGIZE(text) HBOOST_PP_WSTRINGIZE_I(text)
# else
#    define HBOOST_PP_WSTRINGIZE(text) HBOOST_PP_WSTRINGIZE_OO((text))
#    define HBOOST_PP_WSTRINGIZE_OO(par) HBOOST_PP_WSTRINGIZE_I ## par
# endif
#
# define HBOOST_PP_WSTRINGIZE_I(text) HBOOST_PP_WSTRINGIZE_II(#text)
# define HBOOST_PP_WSTRINGIZE_II(str) L ## str
#
# endif
