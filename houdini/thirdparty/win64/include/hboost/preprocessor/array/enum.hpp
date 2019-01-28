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
# ifndef HBOOST_PREPROCESSOR_ARRAY_ENUM_HPP
# define HBOOST_PREPROCESSOR_ARRAY_ENUM_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_ARRAY_ENUM */
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_ARRAY_ENUM(array) HBOOST_PP_ARRAY_ENUM_I(HBOOST_PP_TUPLE_REM_CTOR, array)
#    define HBOOST_PP_ARRAY_ENUM_I(m, args) HBOOST_PP_ARRAY_ENUM_II(m, args)
#    define HBOOST_PP_ARRAY_ENUM_II(m, args) HBOOST_PP_CAT(m ## args,)
# elif HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_ARRAY_ENUM(array) HBOOST_PP_ARRAY_ENUM_I(array)
#    define HBOOST_PP_ARRAY_ENUM_I(array) HBOOST_PP_TUPLE_REM_CTOR ## array
# else
#    define HBOOST_PP_ARRAY_ENUM(array) HBOOST_PP_TUPLE_REM_CTOR array
# endif
#
# endif
