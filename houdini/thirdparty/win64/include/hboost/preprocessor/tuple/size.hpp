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
# ifndef HBOOST_PREPROCESSOR_TUPLE_SIZE_HPP
# define HBOOST_PREPROCESSOR_TUPLE_SIZE_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/variadic/size.hpp>
#
# if HBOOST_PP_VARIADICS
#    if HBOOST_PP_VARIADICS_MSVC
#        define HBOOST_PP_TUPLE_SIZE(tuple) HBOOST_PP_CAT(HBOOST_PP_VARIADIC_SIZE tuple,)
#    else
#        define HBOOST_PP_TUPLE_SIZE(tuple) HBOOST_PP_VARIADIC_SIZE tuple
#    endif
# endif
#
# endif
