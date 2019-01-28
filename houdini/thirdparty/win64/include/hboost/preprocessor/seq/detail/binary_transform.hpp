# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_SEQ_DETAIL_BINARY_TRANSFORM_HPP
# define HBOOST_PREPROCESSOR_SEQ_DETAIL_BINARY_TRANSFORM_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
# include <hboost/preprocessor/variadic/detail/is_single_return.hpp>
#
# /* HBOOST_PP_SEQ_BINARY_TRANSFORM */
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM(seq) HBOOST_PP_SEQ_BINARY_TRANSFORM_I(, seq)
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM_I(p, seq) HBOOST_PP_SEQ_BINARY_TRANSFORM_II(p ## seq)
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM_II(seq) HBOOST_PP_SEQ_BINARY_TRANSFORM_III(seq)
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM_III(seq) HBOOST_PP_CAT(HBOOST_PP_SEQ_BINARY_TRANSFORM_A seq, 0)
# else
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM(seq) HBOOST_PP_CAT(HBOOST_PP_SEQ_BINARY_TRANSFORM_A seq, 0)
# endif
# if HBOOST_PP_VARIADICS
#    if HBOOST_PP_VARIADICS_MSVC
#		define HBOOST_PP_SEQ_BINARY_TRANSFORM_GET_REM(...) \
			HBOOST_PP_VARIADIC_IS_SINGLE_RETURN(HBOOST_PP_REM_CAT,HBOOST_PP_REM,__VA_ARGS__) \
		/**/
#	 else
#		define HBOOST_PP_SEQ_BINARY_TRANSFORM_GET_REM(...) HBOOST_PP_REM
#	 endif
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM_A(...) (HBOOST_PP_SEQ_BINARY_TRANSFORM_GET_REM(__VA_ARGS__), __VA_ARGS__)() HBOOST_PP_SEQ_BINARY_TRANSFORM_B
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM_B(...) (HBOOST_PP_SEQ_BINARY_TRANSFORM_GET_REM(__VA_ARGS__), __VA_ARGS__)() HBOOST_PP_SEQ_BINARY_TRANSFORM_A
# else
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM_A(e) (HBOOST_PP_REM, e)() HBOOST_PP_SEQ_BINARY_TRANSFORM_B
#    define HBOOST_PP_SEQ_BINARY_TRANSFORM_B(e) (HBOOST_PP_REM, e)() HBOOST_PP_SEQ_BINARY_TRANSFORM_A
# endif
# define HBOOST_PP_SEQ_BINARY_TRANSFORM_A0 (HBOOST_PP_EAT, ?)
# define HBOOST_PP_SEQ_BINARY_TRANSFORM_B0 (HBOOST_PP_EAT, ?)
#
# endif
