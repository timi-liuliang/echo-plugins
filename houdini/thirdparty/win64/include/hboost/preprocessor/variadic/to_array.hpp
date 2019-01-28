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
# ifndef HBOOST_PREPROCESSOR_VARIADIC_TO_ARRAY_HPP
# define HBOOST_PREPROCESSOR_VARIADIC_TO_ARRAY_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/tuple/to_array.hpp>
# if HBOOST_PP_VARIADICS_MSVC
#    include <hboost/preprocessor/variadic/size.hpp>
# endif
#
# /* HBOOST_PP_VARIADIC_TO_ARRAY */
#
# if HBOOST_PP_VARIADICS
#    if HBOOST_PP_VARIADICS_MSVC
#        define HBOOST_PP_VARIADIC_TO_ARRAY(...) HBOOST_PP_TUPLE_TO_ARRAY_2(HBOOST_PP_VARIADIC_SIZE(__VA_ARGS__),(__VA_ARGS__))
#    else
#        define HBOOST_PP_VARIADIC_TO_ARRAY(...) HBOOST_PP_TUPLE_TO_ARRAY((__VA_ARGS__))
#    endif
# endif
#
# endif
