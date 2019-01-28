# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# ifndef HBOOST_PREPROCESSOR_DETAIL_SPLIT_HPP
# define HBOOST_PREPROCESSOR_DETAIL_SPLIT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_SPLIT */
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_SPLIT(n, im) HBOOST_PP_SPLIT_I((n, im))
#    define HBOOST_PP_SPLIT_I(par) HBOOST_PP_SPLIT_II ## par
#    define HBOOST_PP_SPLIT_II(n, a, b) HBOOST_PP_SPLIT_ ## n(a, b)
# elif HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_SPLIT(n, im) HBOOST_PP_SPLIT_I(n((im)))
#    define HBOOST_PP_SPLIT_I(n) HBOOST_PP_SPLIT_ID(HBOOST_PP_SPLIT_II_ ## n)
#    define HBOOST_PP_SPLIT_II_0(s) HBOOST_PP_SPLIT_ID(HBOOST_PP_SPLIT_0 s)
#    define HBOOST_PP_SPLIT_II_1(s) HBOOST_PP_SPLIT_ID(HBOOST_PP_SPLIT_1 s)
#    define HBOOST_PP_SPLIT_ID(id) id
# else
#    define HBOOST_PP_SPLIT(n, im) HBOOST_PP_SPLIT_I(n)(im)
#    define HBOOST_PP_SPLIT_I(n) HBOOST_PP_SPLIT_ ## n
# endif
#
# define HBOOST_PP_SPLIT_0(a, b) a
# define HBOOST_PP_SPLIT_1(a, b) b
#
# endif
