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
# ifndef HBOOST_PREPROCESSOR_TUPLE_TO_ARRAY_HPP
# define HBOOST_PREPROCESSOR_TUPLE_TO_ARRAY_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/facilities/overload.hpp>
# include <hboost/preprocessor/tuple/size.hpp>
# include <hboost/preprocessor/variadic/size.hpp>
#
# /* HBOOST_PP_TUPLE_TO_ARRAY */
#
# if HBOOST_PP_VARIADICS
#    if HBOOST_PP_VARIADICS_MSVC
#        define HBOOST_PP_TUPLE_TO_ARRAY(...) HBOOST_PP_TUPLE_TO_ARRAY_I(HBOOST_PP_OVERLOAD(HBOOST_PP_TUPLE_TO_ARRAY_, __VA_ARGS__), (__VA_ARGS__))
#        define HBOOST_PP_TUPLE_TO_ARRAY_I(m, args) HBOOST_PP_TUPLE_TO_ARRAY_II(m, args)
#        define HBOOST_PP_TUPLE_TO_ARRAY_II(m, args) HBOOST_PP_CAT(m ## args,)
#        define HBOOST_PP_TUPLE_TO_ARRAY_1(tuple) (HBOOST_PP_TUPLE_SIZE(tuple), tuple)
#    else
#        define HBOOST_PP_TUPLE_TO_ARRAY(...) HBOOST_PP_OVERLOAD(HBOOST_PP_TUPLE_TO_ARRAY_, __VA_ARGS__)(__VA_ARGS__)
#        define HBOOST_PP_TUPLE_TO_ARRAY_1(tuple) (HBOOST_PP_VARIADIC_SIZE tuple, tuple)
#    endif
#    define HBOOST_PP_TUPLE_TO_ARRAY_2(size, tuple) (size, tuple)
# else
#    define HBOOST_PP_TUPLE_TO_ARRAY(size, tuple) (size, tuple)
# endif
#
# endif
